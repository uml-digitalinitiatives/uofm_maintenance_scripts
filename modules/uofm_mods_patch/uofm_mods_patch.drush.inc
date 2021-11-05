<?php

const UOFM_MODS_PATCH_FAKE_MODS = "<?xml version=\"1.0\"?>\n<mods:mods xmlns:mods=\"http://www.loc.gov/mods/v3\"><mods:titleInfo><mods:title>Example title</mods:title></mods:titleInfo></mods:mods>";

const UOFM_MODS_PATCH_QUEUE = "uofm_mods_patch_queue";

function uofm_mods_patch_drush_command() {
  $items['uofm_mods_patch_preprocess'] = array(
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
      'dont_update_dc' => array(
        'description' => "Don't update the DC when updating MODS (requires islandora_xml_forms)",
      ),
      'force_update_dc' => array(
        'description' => "Update the DC datastream with the MODS even if the MODS has not changed (requires islandora_xml_forms)",
      ),
      'patch_file' => array(
        'description' => 'File which defines a function to use when patching',
        'required' => TRUE,
      ),
      'patch_method' => array(
        'description' => 'A function inside the file defined by patch_file which accepts a string of MODS and returns a string of MODS',
        'required' => TRUE,
      ),
    ),
    'aliases' => array(
      'uofm_mpp',
    ),
    'callback' => 'drush_uofm_xml_patch_preprocess',
    'callback arguments' => array(
      UOFM_MODS_PATCH_QUEUE,
      UOFM_MODS_PATCH_FAKE_MODS,
      FALSE,
      'uofm_xml_patch_custom_queue',
    ),
    'description' => 'Apply a MODS Patcher patch against once or more Islandora objects.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_mods_patch_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_mods_patch_worker',
      UOFM_MODS_PATCH_QUEUE,
      'uofm_mods_patch_timer',
      300,
    ),
    'description' => 'Process the queue of objects generated with uofm_mods_patch_preprocess, update the MODS datastream. Must have privileges to update the objects.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}
