<?php
/**
 * @file
 * Copy CC license
 */

/**
 * Invokes hook_drush_command().
 * @return array
 */
function uofm_copy_license_drush_command() {
  $items = array();

  $items['uofm_copy_license_preprocess'] = array(
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
      'source_pid' => array(
        'description' => 'PID to copy the creative commons from.',
        'required' => TRUE,
      ),
    ),
    'aliases' => array('uofm_copy_cc_pp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      'uofm_copy_license_queue',
      FALSE,
      'uofm_copy_license_custom_queue',
    ),
    'description' => 'Add a list of objects to a queue to have the Creative Commons license copied from the specified object.',
    'drupal dependencies' => array(
      'islandora',
      'uofm_copy_license',
    ),
    'drush dependencies' => array(
      'uofm_maintenance'
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  $items['uofm_copy_license_process'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_copy_license_worker',
      'uofm_copy_license_queue',
      'uofm_copy_license_timer',
      300,
    ),
    'description' => 'Process the Creative Commons copy operations added by \'uofm_copy_cc_license_preprocess\'',
    'drupal dependencies' => array(
      'islandora',
      'uofm_copy_license'
    ),
    'drush dependencies' => array(
      'uofm_maintenance'
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}
