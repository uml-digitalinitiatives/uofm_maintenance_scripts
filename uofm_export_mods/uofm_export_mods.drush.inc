<?php

/**
 * Implements hook_drush_command().
 */
function uofm_export_mods_drush_command() {
  $items = array();
  
  $items['uofm_export_mods_preprocess'] = array(
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
      'add_ids' => array(
        'description' => 'Insert a mods:identifier type="HDL" and type="PID" elements. Implies --add_hdl and --add_pid.',
      ),
      'add_hdl' => array(
        'description' => 'Insert a mods:indetifier type="HDL" element.',
      ),
      'add_pid' => array(
        'description' => 'Insert a mods:indetifier type="PID" element.'
      ),
      'directory' => array(
        'description' => 'Directory to output MODS records',
        'required' => TRUE,
      ),
      'limit' => array(
        'description' => 'Limit the number of records to export, defaults to all records.'
      ),
    ),
    'aliases' => array('uofm_export_mods_pp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      'uofm_export_mods_queue',
      FALSE,
      'uofm_export_mods_custom_queue',
    ),
    'description' => 'Add a list of objects to a queue to have the MODS records exported.',
    'drupal dependencies' => array(
      'islandora',
      'uofm_export_mods',
    ),
    'drush dependencies' => array(
      'uofm_maintenance'
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  
  $items['uofm_export_mods_process'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_export_mods_worker',
      'uofm_export_mods_queue',
      'uofm_export_mods_timer',
      300,
    ),
    'description' => 'Export the MODS records for the objects on the "uofm_export_mods" queue.',
    'drupal dependencies' => array(
      'islandora',
      'uofm_export_mods'
    ),
    'drush dependencies' => array(
      'uofm_maintenance'
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  
  return $items;
}

function drush_uofm_export_mods_worker($data) {
  
}

