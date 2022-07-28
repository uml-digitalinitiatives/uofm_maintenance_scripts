<?php

const UOFM_AGGREGATE_PDFS_QUEUE = "uofm_aggregate_pdfs_queue";

const UOFM_AGGREGATE_PDFS_TIMER = "uofm_aggregate_pdfs_timer";

function uofm_aggregate_pdfs_drush_command() {
  return [
    'uofm_aggregate_pdfs_preprocess' => [
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
        'skip_pages' => array(
          'description' => 'Don\'t regenerate PDFs for pages that already have one.',
        )
      ),
      'aliases' => ['uofm_agg_pdfs_pp'],
      'callback' => 'drush_uofm_maintenance_preprocess',
      'callback arguments' => array(
        UOFM_AGGREGATE_PDFS_QUEUE,
        FALSE,
        'uofm_aggregate_pdfs_queue_builder',
      ),
      'description' => 'Add items to the queue to be have a PDF generated.',
      'drupal dependencies' => array(
        'islandora',
      ),
      'category' => 'uofm_maintenance_scripts',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    ],
    'uofm_aggregate_pdfs_run' => [
      'callback' => 'drush_uofm_maintenance_run',
      'callback arguments' => array(
        'uofm_aggregate_pdfs_worker',
        UOFM_AGGREGATE_PDFS_QUEUE,
        UOFM_AGGREGATE_PDFS_TIMER,
        300,
      ),
      'description' => 'Process the queue of objects generated with uofm_aggregate_pdfs_preprocess',
      'drupal dependencies' => array(
        'islandora',
        'uofm_aggregate_pdfs'
      ),
      'aliases' => ['uofm_agg_pdfs_run'],
      'category' => 'uofm_maintenance_scripts',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    ],
  ];
}
