<?php
/**
 * @file
 * Drush script to purge objects.
 */

/**
 * Implements hook_drush_command().
 */
function uofm_batch_purge_drush_command() {
   $items = array();

   $items['uofm_batch_purge_preprocess'] = array(
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
       'remove_orphans' => array(
         'description' => 'Any child object that has no other parents, are also purged. Default to False.',
       ),
     ),
     'aliases' => array('uofm_batch_purge_pp'),
     'callback' => 'drush_uofm_maintenance_preprocess',
     'callback arguments' => array(
       'uofm_batch_purge_queue',
       FALSE,
       'uofm_batch_purge_custom_queue',
     ),
     'description' => 'Add a list of objects to a queue to have them purged.',
     'drupal dependencies' => array(
       'islandora',
       'uofm_batch_purge',
     ),
     'drush dependencies' => array(
       'uofm_maintenance'
     ),
     'category' => 'uofm_maintenance_scripts',
     'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
   );
  
   $items['uofm_batch_purge_run'] = array(
     'options' => array(
       'timeout' => array(
         'description' => 'Length of time to run, or until queue is empty if omitted',
       ),
     ),
     'callback' => 'drush_uofm_maintenance_run',
     'callback arguments' => array(
       'uofm_batch_purge_worker',
       'uofm_batch_purge_queue',
       'uofm_batch_purge_timer',
       300,
     ),
     'description' => 'Purge all the object in the "uofm_batch_purge" queue.',
     'drupal dependencies' => array(
       'islandora',
       'uofm_batch_purge'
     ),
     'drush dependencies' => array(
       'uofm_maintenance'
     ),
     'category' => 'uofm_maintenance_scripts',
     'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
   );

   return $items;
}
