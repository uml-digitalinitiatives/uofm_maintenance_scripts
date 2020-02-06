<?php
/**
 * @file
 * Drush commands.
 */

function uofm_aggregate_pdfs_drush_command() {
  $items = array();

  $items['uofm_aggregate_pdfs'] = array(
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
    ),
    'aliases' => array('uofm_agg_pdf'),
    'callback' => 'drush_uofm_aggregate_pdfs_process',
    'callback arguments' => array(TRUE),
    'description' => 'Create a PDF out of the PDFs on all pages of a book or newspaper',
    'drupal dependencies' => array(
      'islandora',
      'islandora_paged_content',
      'uofm_aggregate_pdfs',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

function drush_uofm_aggregate_pdfs_process() {
  $results = drush_uofm_maintenance_load_options();
  if ($results === false) {
    // We failed, exit now.
    return;
  }
  foreach ($results as $result) {
    $outcome = (uofm_aggregate_pdfs_generate($result) ? "ok" : "error");
    drush_log("Processing {$result}", $outcome);
  }
}
