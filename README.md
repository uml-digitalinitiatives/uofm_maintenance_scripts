SUMMARY
-------

U of M Maintenance

Collection of simple scripts sharing pre-processing and queue loading functions

REQUIREMENTS
------------

Dependent on:
* islandora
* islandora\_large\_image
* islandora\_solr

INSTALLATION
------------

Install as any other Drupal module.

USAGE
-----

Provides various drush scripts, most are split into a pre-process function and a run function.

The pre-process generally makes use of a DrupalQueue to store the PIDs of objects to be worked on by the associated run function.

The run function will read items from the queue and process them. In the case of the batch re-indexer it is instead called by the Drupal CRON and processes a few items at a time.

CUSTOMIZATION
-------------

To add a new process you can copy a previous preprocess and run drush command from the uofm\_maintenance\_drush\_command() function or copy the below and modify.

This is the drush command to preprocess a PID, comma separated list of PIDs, file of PIDs (one per line) or a Sparql query where clause and add them to a DrupalQueue.
```php
  $items['uofm_maintenance_DO_SOMETHING_preprocess'] = array(
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
        'description' => 'Also process the children of those provided in the --pid, --pidlist, --pidfile or --query option.',
      ),
      'force' => array(
        'description' => 'Force the processing if it would otherwise not be done.',
      ),
    ),
    'aliases' => array('uofm_DO_SM_pp'),
    // The callback is always the same
    'callback' => 'drush_uofm_maintenance_preprocess',
    'callback arguments' => array(
      // A constant or string defining the queue name to add your PIDs to.
      UOFM_MAINTENANCE_FIX_TIFFS_QUEUE,
      // Whether to filter out PIDs that are already in the Solr Index
      FALSE,
      // A function name to generate your queue items or FALSE to just store the PID, defaults to adding the PID as the queue data.
      // The function takes a single queue and returns the object/array/etc to add to the queue.
      'uofm_maintenance_custom_queue_maker',
    ),
    'description' => 'Add a list of objects to a queue to have mimetypes (image/tif) changed to (image/tiff).',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
```

This is the drush command to processes items in the queue for a limited time, or until the queue is empty.
```php
  $items['uofm_maintenance_DO_SOMETHING_run'] = array(
    'options' => array(
      'timeout' => array(
        'description' => 'Length of time to run, or until queue is empty if omitted',
      ),
    ),
    'aliases' => array('uofm_DO_SM_run'),
    // The callback is always the same
    'callback' => 'drush_uofm_maintenance_run',
    'callback arguments' => array(
      // First is the worker function to do the actual processing
      'uofm_maintenance_fix_tiff_worker',
      // The queue to read the objects from
      UOFM_MAINTENANCE_FIX_TIFFS_QUEUE,
      // The timer name to use
      UOFM_MAINTENANCE_FIX_TIFFS_TIMER,
      // Length of lease in seconds (default 100)
      300,
    ),
    'description' => 'Process the queue of objects generated with uofm_fix_mt_pp, correct all image/tif mimetypes to image/tiff',
    'drupal dependencies' => array(
      'islandora',
    ),
    'category' => 'uofm_maintenance_scripts',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
```

CURRENT FUNCTIONALITY
---
1. uofm\_maintenance\_fix\_tif\_mimetype\_pp (uofm\_fix\_mt\_pp) and uofm\_maintenance\_fix\_tif\_mimetype\_run (uofm\_fix\_mt\_run)

   Check the resources for OBJ or TIFF datastreams with a *image/tif* mime-type and change it to *image/tiff*.
  
2. uofm\_maintenance\_derivative\_fixer\_pp (uofm\_dfp) and uofm\_maintenance\_derivative\_fixer\_run (uofm\_dfr)

   Create any missing derivatives for the list of loaded PIDs.
3. uofm\_maintenance\_update\_parent\_pp and uofm\_maintenance\_update\_parent\_run

   Add/Replace a relationship to a group of objects (PIDs), used mostly for moving a group of resources from one collection or parent to another.
4. uofm\_maintenance\_copy\_xacml\_pp and uofm\_maintenance\_copy\_xacml\_run

   Copy the XACML policy from a resource to a group of resources. Useful to propogate XACML through large collections as the query for members can be done once and stored as a list of PIDs.

See modules directory for more functions.