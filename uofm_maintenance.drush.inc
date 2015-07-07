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
  $items['uofm_maintenance_batch_index_pp'] = array(
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
    'aliases' => array('uofm_bi_pp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(UOFM_MAINTENANCE_BATCH_INDEX_QUEUE, TRUE),
    'description' => 'Add items to the queue to be re-indexed on cron runs.',
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
    'aliases' => array('uofm_bi_pp'),
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
    ),
    'aliases' => array('uofm_dfp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(UOFM_MAINTENANCE_DERIVATIVE_FIXER_QUEUE, FALSE),
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
  return $items;
}

/**
 * Loads PIDs to a queue for processing.
 *
 * @param string $queue_name
 *   the name of the queue to add these items to.
 * @param bool $filter
 *   whether to filter PIDs to remove objects already in the Solr index.
 */
function drush_uofm_maintenance_preprocess($queue_name, $filter = FALSE) {
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
      $queue->createItem($obj);
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
        drush_log("Processed object ({$item->data})", "notice");
      }
      else if (isset($bad_item) && $bad_item === $item->data) {
        drush_log("Second attempt (in a row) processing object ({$item->data}) failed, deleting from queue.", "warning");
        $queue->deleteItem($item);
      }
      else {
        drush_log("Error processing object ({$item->data})", "warning");
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

  $JMS_message =<<< EOF
<?xml version="1.0" encoding="UTF-8"?>
<entry xmlns="http://www.w3.org/2005/Atom" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:fedora-types="http://www.fedora.info/definitions/1/0/types/">
  <id>urn:uuid:4ea71518-9ff7-4cfe-8aac-ecaf02e2b709</id>
  <updated>@lastMod</updated>
  <author>
    <name>fedoraAdmin</name>
    <uri>http://localhost:8080/fedora</uri>
  </author>
  <title type="text">modifyDatastreamByReference</title>
  <category term="uofm:9333" scheme="fedora-types:pid" label="xsd:string"></category>
  <category term="null" scheme="fedora-types:dsID" label="xsd:string"></category>
  <category term="" scheme="fedora-types:altIDs" label="fedora-types:ArrayOfString"></category>
  <category term="null" scheme="fedora-types:dsLabel" label="xsd:string"></category>
  <category term="null" scheme="fedora-types:formatURI" label="xsd:string"></category>
  <category term="null" scheme="fedora-types:dsLocation" label="xsd:string"></category>
  <category term="null" scheme="fedora-types:checksumType" label="xsd:string"></category>
  <category term="null" scheme="fedora-types:checksum" label="xsd:string"></category>
  <category term="null" scheme="fedora-types:logMessage" label="xsd:string"></category>
  <summary type="text">uofm:9333</summary>
  <content type="text">@lastMod</content>
  <category term="3.7.0" scheme="info:fedora/fedora-system:def/view#version"></category>
  <category term="info:fedora/fedora-system:ATOM-APIM-1.0" scheme="http://www.fedora.info/definitions/1/0/types/formatURI"></category>
</entry>
EOF;

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
  if (count($results) > 0) {
    $gsearch_address = variable_get("uofm_maintenance_jms_broker", "tcp://localhost:61613");
    $topic_queue = variable_get("uofm_maintenance_jms_topic_queue", "topic");
    $qt_name = variable_get("uofm_maintenance_jms_topic_queue_name", "fedora.apim.update");
    $endpoint = "/$topic_queue/$qt_name";
    try {
      $stomp = new Stomp($gsearch_address);
    } catch (StompException $e) {
      drush_log(t("Error connecting to JMS broker at @b", array('@b' => $gsearch_address)), "error");
      return;
    }
    $count = 0;
    foreach ($results as $obj) {
      if (strlen(trim($obj)) > 0 && islandora_is_valid_pid($obj)) {
        $date = DateTime::format("Y-m-d\TH:i:s.u\Z");
        $mesg = str_replace('@lastMod', $date, $JMS_message);
        $stomp->send($endpoint, $mesg, array('receipt' => 'msg-' . sha1($mesg)));
        $count += 1;
      }
    }
    drush_log("Added $count items to the JMS ($endpoint)", 'ok');
  }
}