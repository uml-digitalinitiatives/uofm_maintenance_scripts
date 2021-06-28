<?php
/**
 * @file Drush commands
 */

/**
 * Implements hook_drush_command().
 */
function uofm_regen_fits_drush_command() {
  $items = array();

  $items['uofm_maintenance_regen_fits_pp'] = array(
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
      'recursive' => array(
        'description' => 'Process any children of the objects listed/queried',
      ),
      'force' => array(
        'description' => 'Force indexing even if already in Solr, CAREFUL this could re-index ALOT of stuff.',
      ),
    ),
    'aliases' => array('uofm_regen_fits_pp'),
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array('uofm_maintenance_regenerate_fits'),
    'description' => 'Add a list of objects to a queue to have the TECHMD or TECHMD_FITS regenerated',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  $items['uofm_maintenance_regen_fits_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_regen_fits_run'),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_maintenance_regen_fits_worker',
      'uofm_maintenance_regenerate_fits',
      'uofm_maintenance_regenerate_fits',
      300,
    ),
    'description' => 'Process the queue of objects generated with uofm_regen_fits_pp, regenerate TECHMD or TECHMD_FITS for all objects',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

