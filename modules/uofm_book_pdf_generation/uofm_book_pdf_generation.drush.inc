<?php

const UOFM_BOOK_PDF_GENERATION_QUEUE = "uofm_book_pdf_generation_queue";

const UOFM_BOOK_PDF_GENERATION_TIMER = "uofm_book_pdf_generation_timer";

function uofm_book_pdf_generation_drush_command() {
  return [
    'uofm_book_pdf_generation_preprocess' => [
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
      'aliases' => ['uofm_book_pdf_pp'],
      'callback' => 'drush_uofm_maintenance_preprocess',
      'callback arguments' => array(
        UOFM_BOOK_PDF_GENERATION_QUEUE,
        FALSE,
        'uofm_book_pdf_generation_queue_builder',
      ),
      'description' => 'Add items to the queue to be have a PDF generated.',
      'drupal dependencies' => array(
        'islandora',
      ),
      'category' => 'uofm_maintenance_scripts',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    ],
    'uofm_book_pdf_generation_run' => [
      'callback' => 'drush_uofm_maintenance_run',
      'callback arguments' => array(
        'uofm_book_pdf_generation_worker',
        UOFM_BOOK_PDF_GENERATION_QUEUE,
        UOFM_BOOK_PDF_GENERATION_TIMER,
        300,
      ),
      'description' => 'Process the queue of objects generated with uofm_book_pdf_generation_preprocess',
      'drupal dependencies' => array(
        'islandora',
        'uofm_book_pdf_generation'
      ),
      'aliases' => ['uofm_book_pdf_run'],
      'category' => 'uofm_maintenance_scripts',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    ],
  ];
}
