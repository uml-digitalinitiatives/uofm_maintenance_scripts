<?php
/**
 * @file
 * Drush scripts and functions
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
    'callback' => 'uofm_maintenance_fix_tif_preprocess',
    'description' => 'Add a list of objects to a queue to have mimetypes (image/tif) changed to (image/tiff).',
    'drupal dependencies' => array(
      'islandora',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_maintenance_fix_tif_mimetype_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_fix_mt_run'),
    'callback' => 'uofm_maintenance_fix_tif_run',
    'description' => 'Process the queue of objects generated with uofm_fix_mt_pp, correct all image/tif mimetypes to image/tiff',
    'drupal dependencies' => array(
      'islandora',
    ),
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
    'callback' => 'uofm_maintenance_regen_jp2_preprocess',
    'description' => 'Add a list of objects to a queue to have the JP2 regenerated',
    'drupal dependencies' => array(
      'islandora',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
    $items['uofm_maintenance_regen_jp2_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_regen_jp2_run'),
    'callback' => 'uofm_maintenance_regen_jp2_run',
    'description' => 'Process the queue of objects generated with uofm_regen_jp2_pp, regenerate JP2 for all objects',
    'drupal dependencies' => array(
      'islandora',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Loads PIDs to a queue to process for changing old mimeTypes
 */
function uofm_maintenance_fix_tif_preprocess() {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $choices = (strlen(drush_get_options('query', '')) > 0 ? 1 : 0 +
    strlen(drush_get_options('pid', '')) > 0 ? 1 : 0 +
    strlen(drush_get_options('pidlist', '')) > 0 ? 1 : 0 +
    strlen(drush_get_options('pidfile', '')) > 0 ? 1 : 0);
  if ($choices == 0) {
    drush_set_error("You must set one of the options 'pid', 'pidlist, 'pidfile' or 'query'.");
  }
  elseif ($choices > 1) {
    drush_set_error("You can only use ONE of the options 'pid', 'pidlist', 'pidfile' or 'query' at a time.");
  }
  if (!is_null(drush_get_options('query', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'query',
      'value' => drush_get_options('query', ''),
    ));
  }
  elseif (!is_null(drush_get_options('pid', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'pid',
      'value' => drush_get_options('pid', ''),
    ));
  }
  elseif (!is_null(drush_get_options('pidlist', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'pidlist',
      'value' => drush_get_options('pidlist', ''),
    ));
  }
  elseif (!is_null(drush_get_options('pidfile', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'pidfile',
      'value' => drush_get_options('pidfile', ''),
    ));
  }
  if (!is_array($results)) {
    if ($results == -8) {
      drush_set_error("File does not exist.");
    }
    else if ($results == -9) {
      drush_set_error("File cannot be read.");
    }
  }

  $queue = DrupalQueue::get(UOFM_MAINTENANCE_FIX_TIFFS_QUEUE);
  $count = 0;
  foreach ($results as $obj) {
    if (strlen(trim($obj)) > 0 && islandora_is_valid_pid($obj)) {
      $queue->createItem($obj);
      $count += 1;
    }
  }
  drush_log("Added $count items to queue", 'ok');

}


/**
 * Pull from the queue and send to worker.
 */
function uofm_maintenance_fix_tif_run() {
  $timeout = drush_get_option('timeout', ini_get('max_execution_time'));
  $queue = DrupalQueue::get(UOFM_MAINTENANCE_FIX_TIFFS_QUEUE);
  timer_start(UOFM_MAINTENANCE_FIX_TIFFS_TIMER);
  while (($timeout == 0 || (timer_read(UOFM_MAINTENANCE_FIX_TIFFS_TIMER) / 1000) < $timeout) && $queue->numberOfItems() > 0) {
    $item = $queue->claimItem(100);
    if ($item instanceof SystemQueue ) {
      $res = uofm_maintenance_fix_tiff_worker($item->data);
      if ($res === TRUE) {
        $queue->deleteItem($item);
        drush_log("Processed object ({$item->data})", "ok");
      }
      else {
        $queue->releaseItem($item);
      }
    }
  }
  timer_stop(UOFM_MAINTENANCE_FIX_TIFFS_TIMER);
}

/**
 * Loads PIDs to a queue to process for regenerating JP2 datastreams.
 */
function uofm_maintenance_regen_jp2_preprocess() {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $choices = (strlen(drush_get_options('query', '')) > 0 ? 1 : 0 +
    strlen(drush_get_options('pid', '')) > 0 ? 1 : 0 +
    strlen(drush_get_options('pidlist', '')) > 0 ? 1 : 0 +
    strlen(drush_get_options('pidfile', '')) > 0 ? 1 : 0);
  if ($choices == 0) {
    drush_set_error("You must set one of the options 'pid', 'pidlist, 'pidfile' or 'query'.");
  }
  elseif ($choices > 1) {
    drush_set_error("You can only use ONE of the options 'pid', 'pidlist', 'pidfile' or 'query' at a time.");
  }
  if (!is_null(drush_get_options('query', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'query',
      'value' => drush_get_options('query', ''),
    ));
  }
  elseif (!is_null(drush_get_options('pid', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'pid',
      'value' => drush_get_options('pid', ''),
    ));
  }
  elseif (!is_null(drush_get_options('pidlist', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'pidlist',
      'value' => drush_get_options('pidlist', ''),
    ));
  }
  elseif (!is_null(drush_get_options('pidfile', NULL))) {
    $results = uofm_maintenance_load_pids(array(
      'type' => 'pidfile',
      'value' => drush_get_options('pidfile', ''),
    ));
  }
  if (!is_array($results)) {
    if ($results == -8) {
      drush_set_error("File does not exist.");
    }
    else if ($results == -9) {
      drush_set_error("File cannot be read.");
    }
  }

  $queue = DrupalQueue::get(UOFM_MAINTENANCE_REGEN_JP2_QUEUE);
  $count = 0;
  foreach ($results as $obj) {
    if (strlen(trim($obj)) > 0 && islandora_is_valid_pid($obj)) {
      $queue->createItem($obj);
      $count += 1;
    }
  }
  drush_log("Added $count items to queue", 'ok');

}

/**
 * Pull from the queue and send to worker.
 */
function uofm_maintenance_regen_jp2_run() {
  $timeout = drush_get_option('timeout', ini_get('max_execution_time'));
  $queue = DrupalQueue::get(UOFM_MAINTENANCE_REGEN_JP2_QUEUE);
  timer_start(UOFM_MAINTENANCE_REGEN_JP2_TIMER);
  while (($timeout == 0 || (timer_read(UOFM_MAINTENANCE_REGEN_JP2_TIMER) / 1000) < $timeout) && $queue->numberOfItems() > 0) {
    $item = $queue->claimItem(100);
    if ($item instanceof SystemQueue ) {
      $res = uofm_maintenance_regen_jp2_worker($item->data);
      if ($res === TRUE) {
        $queue->deleteItem($item);
        drush_log("Processed object ({$item->data})", "ok");
      }
      else {
        $queue->releaseItem($item);
      }
    }
  }
  timer_stop(UOFM_MAINTENANCE_REGEN_JP2_TIMER);
}