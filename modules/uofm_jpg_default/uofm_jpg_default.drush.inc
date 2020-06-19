<?php
/**
 * @file
 * Drush commands for UofM Fix JPG defaults
 */

function uofm_jpg_default_drush_command() {
  $items['uofm_jpg_default_preprocess'] = array(
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
      'query' => array(
        'description' => 'The SPARQL where clause, uses "?object" as the returned variable.',
      ),
    ),
    'aliases' => array(
      'uofm_jpg_default_pp',
    ),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      UOFM_JPG_DEFAULT_QUEUE_NAME,
      FALSE,
      NULL,
    ),
    'description' => 'Load items to have the label on the JPG datastream changed to match the OBJ plus ".jpg". Run on cron.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}
