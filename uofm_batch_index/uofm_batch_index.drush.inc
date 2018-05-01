<?php
/**
 * @file
 * Batch ingest drush scripts.
 */


/**
 * @const
 *
 * Base URL for reindexing.
 */
define('UOFM_BATCH_INDEX_REINDEX_URL', 'http://localhost:8080/fedora3-solr-indexer/reindex/');

/**
 * Implements hook_drush_command().
 */
function uofm_batch_index_drush_command() {
  $items = array();

  $items['uofm_batch_index_jms'] = array(
    'options' => array(
      'query' => array(
        'description' => 'The SPARQL where clause, uses "?object" as the returned variable.',
      ),
      'pid' => array(
        'description' => 'A PID to operate on',
      ),
      'pidlist' => array(
        'description' => 'A comma seperated list of PIDs to operate on',
      ),
      'pidfile' => array(
        'description' => 'Path to a textfile of PIDs to operate on, one per line',
      ),
      'recursive' => array(
        'description' => 'Process any children of the objects listed/queried',
      ),
      'force' => array(
        'description' => 'Force indexing even if already in Solr, CAREFUL this could re-index ALOT of stuff.',
      ),
    ),
    'aliases' => array('uofm_bi_jms'),
    'callback' => 'drush_uofm_batch_index_process',
    'callback arguments' => array(TRUE),
    'description' => 'Add items to the JMS topic/queue to be re-indexed.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Load the PIDs but directly onto the JMS queue.
 *
 * @param bool $filter
 *   whether to filter PIDs to remove objects already in the Solr index.
 */
function drush_uofm_batch_index_process($filter = TRUE) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $choices = (strlen(drush_get_option('query', '')) > 0 ? 1 : 0 +
  strlen(drush_get_option('pid', '')) > 0 ? 1 : 0 +
  strlen(drush_get_option('pidlist', '')) > 0 ? 1 : 0 +
  strlen(drush_get_option('pidfile', '')) > 0 ? 1 : 0);
  if ($choices == 0) {
    drush_set_error("You must set one of the options 'pid', 'pidlist', 'pidfile' or 'query'.");
  }
  elseif ($choices > 1) {
    drush_set_error("You can only use ONE of the options 'pid', 'pidlist', 'pidfile' or 'query' at a time.");
  }
  $force = (bool) (!is_null(drush_get_option('force', NULL)));
  $recursive = (bool) (!is_null(drush_get_option('recursive', NULL)));
  if (!is_null(drush_get_option('query', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'query',
      'value' => drush_get_option('query', ''),
    ));
  }
  elseif (!is_null(drush_get_option('pid', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'single',
      'value' => drush_get_option('pid', ''),
    ));
  }
  elseif (!is_null(drush_get_option('pidlist', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'list',
      'value' => drush_get_option('pidlist', ''),
    ));
  }
  elseif (!is_null(drush_get_option('pidfile', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'file',
      'value' => drush_get_option('pidfile', ''),
    ));
  }
  if (!is_array($results)) {
    if ($results == -8) {
      drush_set_error("File does not exist.");
    }
    elseif ($results == -9) {
      drush_set_error("File cannot be read.");
    }
  }

  if ($recursive) {
    $new_results = uofm_maintenance_recursive_query($results);
    $results = array_unique($new_results);
  }
  if ($filter && !$force) {
    $results = uofm_batch_index_filter($results);
  }
  if (count($results) > 0) {
    $user = variable_get('islandora_collection_search_gsearch_user', NULL);
    $passwd = variable_get('islandora_collection_search_gsearch_password', NULL);
    $count = 0;
    if (!(is_null($user) || is_null($passwd))) {
      foreach ($results as $obj) {
        if (strlen(trim($obj)) > 0 && islandora_is_valid_pid($obj)) {
          $url = UOFM_BATCH_INDEX_REINDEX_URL . $obj;
          $ch = curl_init();
          curl_setopt($ch, CURLOPT_URL, $url);
          curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
          curl_setopt($ch, CURLOPT_HEADER, 1);
          curl_setopt($ch, CURLOPT_USERPWD, "$user:$passwd");
          $result = curl_exec($ch);
          curl_close($ch);
          $r = preg_match("~HTTP/1.1 200 OK~", $result);
          if (!$r) {
            drush_log(t("Problem adding item (@p) to JMS queue", array('@p' => $obj)), "error");
          }
          else {
            $count += 1;
          }
        }
      }
      drush_log("Added $count items to the JMS queue", 'ok');
    }
  }
}
