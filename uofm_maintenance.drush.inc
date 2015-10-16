<?php
/**
 * @file
 * Drush scripts and functions
 */

/**
 * Need the utilities.
 */
require_once dirname(__FILE__) . '/includes/utilities.inc';

/**
 * Incase you didn't enable this module, we still want it.
 */
require_once dirname(__FILE__) . '/uofm_maintenance.module';

/**
 * Implements hook_drush_command().
 */
function uofm_maintenance_drush_command() {
  $items = array();

  $items['uofm_maintenance_fix_tif_mimetype_pp'] = array(
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
    ),
    'aliases' => array('uofm_fix_mt_pp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(UOFM_MAINTENANCE_FIX_TIFFS_QUEUE, FALSE),
    'description' => 'Add a list of objects to a queue to have mimetypes (image/tif) changed to (image/tiff).',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_fix_tif_mimetype_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_fix_mt_run'),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_maintenance_fix_tiff_worker',
      UOFM_MAINTENANCE_FIX_TIFFS_QUEUE,
      UOFM_MAINTENANCE_FIX_TIFFS_TIMER,
    ),
    'description' => 'Process the queue of objects generated with uofm_fix_mt_pp, correct all image/tif mimetypes to image/tiff',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_regen_jp2_pp'] = array(
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
    ),
    'aliases' => array('uofm_regen_jp2_pp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(UOFM_MAINTENANCE_REGEN_JP2_QUEUE, FALSE),
    'description' => 'Add a list of objects to a queue to have the JP2 regenerated',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_regen_jp2_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_regen_jp2_run'),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_maintenance_regen_jp2_worker',
      UOFM_MAINTENANCE_REGEN_JP2_QUEUE,
      UOFM_MAINTENANCE_REGEN_JP2_TIMER,
    ),
    'description' => 'Process the queue of objects generated with uofm_regen_jp2_pp, regenerate JP2 for all objects',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_batch_index_jms'] = array(
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
    'callback' => 'drush_uofm_maintenance_batch_index',
    'callback arguments' => array(TRUE),
    'description' => 'Add items to the JMS topic/queue to be re-indexed by GSearch.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_derivative_fixer_pp'] = array(
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
      'force' => array(
        'description' => 'Force derivative to be recreated even if one exists.',
      ),
      'source_dsid' => array(
        'description' => 'Filter derivative hooks by this source datastream ID',
      ),
    ),
    'aliases' => array('uofm_dfp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      UOFM_MAINTENANCE_DERIVATIVE_FIXER_QUEUE,
      FALSE,
      'uofm_maintenance_derivative_fixer_queue',
    ),
    'description' => 'Add items to the queue to be have missing derivatives created.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_derivative_fixer_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_dfr'),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_maintenance_derivative_fixer_worker',
      UOFM_MAINTENANCE_DERIVATIVE_FIXER_QUEUE,
      UOFM_MAINTENANCE_DERIVATIVE_FIXER_TIMER,
      300,
    ),
    'description' => 'Process the queue of objects generated with uofm_regen_jp2_pp, regenerate JP2 for all objects',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items["uofm_maintenance_update_parent_pp"] = array(
    'options' => array(
      'pid' => array(
        'description' => 'A PID to operate on',
      ),
      'pidlist' => array(
        'description' => 'A comma seperated list of PIDs to operate on',
      ),
      'pidfile' => array(
        'description' => 'Path to a textfile of PIDs to operate on, one per line',
      ),
      'parent_relationship_uri' => array(
        'description' => 'The namespace URI of the relationship to the parent.' .
        ' Defaults to "info:fedora/fedora-system:def/relations-external#".',
        'value' => 'optional',
      ),
      'parent_relationship_pred' => array(
        'description' => 'The predicate of the relationship to the parent. ' .
        'Defaults to "isMemberOf".',
        'value' => 'optional',
      ),
      'old_parent' => array(
        'description' => 'The old parent to match to, incase of multiple parents.' .
          ' If not provided the new parent is just added.',
      ),
      'new_parent' => array(
        'description' => 'The new parent to add.',
        'required' => TRUE,
      ),
      'update_xacml' => array(
        'description' => 'If the XACML module is enabled, copy the parent\'s XACML policy and update all children.',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      UOFM_MAINTENANCE_UPDATE_PARENT_QUEUE,
      FALSE,
      'uofm_maintenance_update_parent_custom_queue',
    ),
    'description' => 'Add items to a queue to have a parent changed/added and XACML updated.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items["uofm_maintenance_update_parent_run"] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_maintenance_update_parent_worker',
      UOFM_MAINTENANCE_UPDATE_PARENT_QUEUE,
      UOFM_MAINTENANCE_UPDATE_PARENT_QUEUE,
      300,
    ),
    'description' => 'Process the queue of objects generated with uofm_maintenance_update_parent_pp,' .
      ' update parent for all objects.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_copy_xacml_pp'] = array(
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
      'from_pid' => array(
        'description' => 'The PID to copy the XACML policy from.',
        'required' => TRUE,
      ),
    ),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      UOFM_MAINTENANCE_COPY_XACML_QUEUE,
      FALSE,
      'uofm_maintenance_copy_xacml_custom_queue',
    ),
    'description' => 'Add items to the queue to have XACML policies copied to them.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_copy_xacml_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_maintenance_update_xacml_worker',
      UOFM_MAINTENANCE_COPY_XACML_QUEUE,
      UOFM_MAINTENANCE_COPY_XACML_QUEUE,
      300,
    ),
    'description' => 'Process the queue of objects generated with preprocess, copy XACML policy from one object to a set of others.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Loads PIDs to a queue for processing.
 *
 * @param string $queue_name
 *   the name of the queue to add these items to.
 * @param bool $filter
 *   whether to filter PIDs to remove objects already in the Solr index.
 * @param string $item_creation
 *   function to generate the queue item, other than the default of PID.
 */
function drush_uofm_maintenance_preprocess($queue_name, $filter = FALSE, $custom_builder = NULL) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $choices = (strlen(drush_get_option('query', '')) > 0 ? 1 : 0 +
    strlen(drush_get_option('pid', '')) > 0 ? 1 : 0 +
    strlen(drush_get_option('pidlist', '')) > 0 ? 1 : 0 +
    strlen(drush_get_option('pidfile', '')) > 0 ? 1 : 0);
  if ($choices == 0) {
    drush_set_error("You must set one of the options 'pid', 'pidlist, 'pidfile' or 'query'.");
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
    $results = uofm_maintenance_batch_index_filter($results);
  }
  $queue = DrupalQueue::get($queue_name);
  $count = 0;
  foreach ($results as $obj) {
    if (strlen(trim($obj)) > 0 && islandora_is_valid_pid($obj)) {
      if (!is_null($custom_builder) && function_exists($custom_builder)) {
        $return_obj = $custom_builder($obj);
        $queue->createItem($return_obj);
      }
      else {
        $queue->createItem($obj);
      }
      $count += 1;
    }
  }
  drush_log("Added $count items to the queue ($queue_name)", 'ok');

}


/**
 * Pull from the queue and send to worker.
 *
 * @param string $worker_fn
 *   name of the function to pass the queue data to
 * @param string $queue_name
 *   name of the queue to read items from
 * @param string $timer
 *   name of the timer use for timeout limits
 * @param int $lease
 *   length of time to claim each item for
 */
function drush_uofm_maintenance_run($worker_fn, $queue_name, $timer, $lease = 100) {
  drupal_static_reset('islandora_get_tuque_connection');
  $timeout = drush_get_option('timeout', ini_get('max_execution_time'));
  $queue = DrupalQueue::get($queue_name);
  timer_start($timer);
  while (($timeout == 0 || (timer_read($timer) / 1000) < $timeout) && ($item = $queue->claimItem($lease)) !== FALSE) {
    if (is_object($item) && isset($item->data)) {
      $res = $worker_fn($item->data);
      if ($res === TRUE) {
        $queue->deleteItem($item);
        if (is_string($item->data)) {
          drush_log("Processed object ({$item->data})", "notice");
        }
        else {
          drush_log("Processed object...", "notice");
        }
      }
      else if (isset($bad_item) && $bad_item === $item->data) {
        if (is_string($item->data)) {
          drush_log("Second attempt (in a row) processing object ({$item->data}) failed, deleting from queue.", "warning");
        }
        $queue->deleteItem($item);
      }
      else {
        if (is_string($item->data)) {
          drush_log("Error processing object ({$item->data})", "warning");
        }
        else {
          drush_log("Error processing object...", "warning");
        }
        $queue->releaseItem($item);
        $bad_item = $item->data;
      }
    }
  }
  timer_stop($timer);
}

/**
 * Load the PIDs but directly onto the JMS queue.
 *
 * @param bool $filter
 *   whether to filter PIDs to remove objects already in the Solr index.
 */
function drush_uofm_maintenance_batch_index($filter = TRUE) {
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
    $results = uofm_maintenance_batch_index_filter($results);
  }
  if (count($results) > 0) {
    // @TODO: Hardcoding this now, should make it configurable in future.
    $base_url = 'http://localhost:8080/uofm-camel-route/rest/reindex/';
    $user = variable_get('islandora_collection_search_gsearch_user', NULL);
    $passwd = variable_get('islandora_collection_search_gsearch_password', NULL);
    $count = 0;
    if (!(is_null($user) || is_null($passwd))) {
      foreach ($results as $obj) {
        if (strlen(trim($obj)) > 0 && islandora_is_valid_pid($obj)) {
          $url = $base_url . $obj;
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
