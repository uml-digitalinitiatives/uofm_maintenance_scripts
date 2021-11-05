<?php

const UOFM_PBCORE_PATCH_FAKE_PBCORE = "<?xml version=\"1.0\"?>\n<pbcoreDescriptionDocument xmlns=\"http://www.pbcore.org/PBCore/PBCoreNamespace.html\" xmlns:pb=\"http://www.pbcore.org/PBCore/PBCoreNamespace.html\">\n<pbcoreTitle>Example Title</pbcoreTitle></pbcoreDescriptionDocument>";

const UOFM_PBCORE_PATCH_QUEUE = "uofm_pbcore_patch_queue";

function uofm_pbcore_patch_drush_command() {
  $items['uofm_pbcore_patch_preprocess'] = array(
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
      'patch_file' => array(
        'description' => 'File which defines a function to use when patching',
        'required' => TRUE,
      ),
      'patch_method' => array(
        'description' => 'A function inside the file defined by patch_file which accepts a string of PBCore and returns a string of PBCore',
        'required' => TRUE,
      ),
    ),
    'aliases' => array(
      'uofm_pbpp',
    ),
    'callback' => 'drush_uofm_xml_patch_preprocess',
    'callback arguments' => array(
      UOFM_PBCORE_PATCH_QUEUE,
      UOFM_PBCORE_PATCH_FAKE_PBCORE,
      FALSE,
      'uofm_xml_patch_custom_queue',
    ),
    'description' => 'Apply a PBCore Patcher patch against once or more Islandora objects.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_pbcore_patch_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_pbcore_patch_worker',
      UOFM_PBCORE_PATCH_QUEUE,
      'uofm_pbcore_patch_timer',
      300,
    ),
    'description' => 'Process the queue of objects generated with uofm_pbcore_patch_preprocess, update the PBCORE datastream. Must have privileges to update the objects.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}
