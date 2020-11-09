<?php

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
    'callback' => 'drush_uofm_mods_patch_preprocess',
    'callback arguments' => array(
      'uofm_mods_patch_queue',
      FALSE,
      'uofm_mods_patch_custom',
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
      'uofm_mods_patch_queue',
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

/**
 * Custom preprocess to validate the patch.
 * @param string $queue_name the queue name
 * @param bool $filter whether to
 * @param null $custom_builder
 */
function drush_uofm_mods_patch_preprocess($queue_name, $filter = FALSE, $custom_builder = NULL) {
  $patch_file = drush_get_option("patch_file", NULL);
  $patch_method = drush_get_option("patch_method", NULL);
  if (is_file($patch_file)) {
    if (is_readable($patch_file)) {
      include_once($patch_file);
      if (isset($patch_method) && function_exists($patch_method)) {
        $fake_mods = "<?xml version=\"1.0\"?>\n<mods:mods xmlns:mods=\"http://www.loc.gov/mods/v3\"><mods:titleInfo><mods:title>Example title</mods:title></mods:titleInfo></mods:mods>";
        $new_mods = call_user_func_array($patch_method, [$fake_mods, new stdClass()]);
        if (is_string($new_mods)) {
          // Pass on to the normal preprocess function.
          drush_uofm_maintenance_preprocess($queue_name, $filter, $custom_builder);
        } else {
          drush_log("Interpreted function did not return a string.", "warning");
        }
      }
      else {
        drush_log("Method {$patch_method} is not a function in the file {$patch_file}." , "warning");
      }
    }
    else {
      drush_log("File {$patch_file} is not readable", "warning");
    }
  }
  else {
    drush_log("It appears {$patch_file} is not a file.", "warning");
  }
}
