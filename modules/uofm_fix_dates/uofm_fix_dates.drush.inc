<?php

function uofm_fix_dates_drush_command() {
  $items['uofm_fix_dates_preprocess'] = array(
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
      'old_year' => array(
        'description' => 'Year to search for.',
        'required' => TRUE,
      ),
      'new_year' => array(
        'description' => 'Year to replace with.',
        'required' => TRUE,
      ),
    ),
    'aliases' => array(
      'uofm_fix_dates_pp',
    ),
    'callback' => 'drush_uofm_fix_dates_preprocess',
    'callback arguments' => array(
      'uofm_fix_dates_queue',
      FALSE,
      'uofm_fix_dates_custom',
    ),
    'description' => 'Change the year in multiple spots of a MODS record.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['uofm_fix_dates_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      'uofm_fix_dates_worker',
      'uofm_fix_dates_queue',
      'uofm_fix_dates_timer',
      300,
    ),
    'description' => 'Process the queue of objects generated with preprocess, update the MODS datastream to alter dates. Must have privileges to update the objects.',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

function drush_uofm_fix_dates_preprocess($queue_name, $filter = FALSE, $custom_builder = NULL) {
  $from = drush_get_option('old_year', NULL);
  $to = drush_get_option('new_year', NULL);
  if (is_null($from) || is_null($to)) {
    drush_log(t('old_year and new_year are required arguments'), 'warning');
    return FALSE;
  }
  if (!preg_match('/^([0-9]{4})(\-[0-9]{2})?(\-[0-9]{2})?$/', trim($from), $f_match) ||
    !preg_match('/^([0-9]{4})(\-[0-9]{2})?(\-[0-9]{2})?$/', trim($to), $t_match)) {
    drush_log(t('old_year and new_year arguments must be dates of the format YYYY, YYYY-MM or YYYY-MM-DD'), 'warning');
    return FALSE;
  }
  else {
    if (count($f_match) !== count($t_match)) {
      drush_log(t('old_year and new_year arguments must have the same format or you can create invalid dates'), 'warning');
      return FALSE;
    }
    drush_uofm_maintenance_preprocess($queue_name, $filter, $custom_builder);
  }
}