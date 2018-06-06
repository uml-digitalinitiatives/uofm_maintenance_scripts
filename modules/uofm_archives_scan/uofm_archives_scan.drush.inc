<?php

/**
 * Implements hook_drush_command().
 */
function uofm_archives_scan_drush_command() {
  $items = array();

  $items['uofm_archives_scan'] = array(
    'options' => array(
      'pid' => array(
        'description' => 'A collection PID to operate on',
        'required' => TRUE,
      ),
      'output' => array(
        'description' => 'A file to output the information to.',
        'required' => TRUE,
      ),
    ),
    'aliases' => array('uofm_arch_scan'),
    'callback' => 'drush_uofm_archives_scan_process',
    'callback arguments' => array(TRUE),
    'description' => 'Scan all objects in a collection and build a list of similar objects based on local identifier.',
    'drupal dependencies' => array(
      'islandora',
      'islandora_basic_collection',
      'uofm_archives_scan',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

function drush_uofm_archives_scan_process() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $pid = drush_get_option('pid', NULL);
  $file = drush_get_option('output', NULL);
  if (!islandora_is_valid_pid($pid)) {
    throw new Exception(format_string("!pid is not a valid pid", array(
      '!pid' => $pid,
    )));
  }
  $object = islandora_object_load($pid);
  if (!$object) {
    drush_log(format_string("Could not load object from pid !pid",
        array('!pid' => $pid)),
    "error");
    return;
  }
  if (!in_array("islandora:collectioncmodel", array_map('strtolower', $object->models))) {
    drush_log(format_string("Object !pid is not a collection",
        array('!pid' => $pid)),
    "error");
    return;
  }
  if (is_dir($file)) {
    drush_log("--output must be a filename, not just a directory name.", "error");
    return;
  }
  $base_dir = dirname($file);
  if (file_exists($base_dir)) {
    drush_log(format_string("File !file already exists. Exiting.",
      array('!file' => $file)),
    'error');
    return;
  }
  if (!file_exists($base_dir) || !is_writable($base_dir)) {
    drush_log(format_string("Unable to write to directory !dir",
      array('!dir' => $base_dir)),
    'error');
    return;
  }
  uofm_archives_scan_process_collection($object, $file);
}
