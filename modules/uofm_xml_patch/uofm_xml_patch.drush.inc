<?php

/**
 * Custom preprocess to validate the patch.
 *
 * @param string $queue_name the queue name
 * @param string $fake_xml a string of fake xml to test with.
 * @param bool $filter whether to filter or not
 * @param string|null $custom_builder class
 */
function drush_uofm_xml_patch_preprocess($queue_name, $fake_xml, $filter = FALSE, $custom_builder = NULL) {
  $patch_file = drush_get_option("patch_file", NULL);
  $patch_method = drush_get_option("patch_method", NULL);
  if (is_file($patch_file)) {
    if (is_readable($patch_file)) {
      include_once($patch_file);
      if (isset($patch_method) && function_exists($patch_method)) {
        $new_xml = call_user_func_array($patch_method, [$fake_xml, new stdClass()]);
        if (is_string($new_xml)) {
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
