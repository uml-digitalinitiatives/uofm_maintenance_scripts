## Overview

This module allows the batch generation of aggregated PDFs for paged content objects in an Islandora 7 repository.

## Usage

This works in the common (to the uofm_maintenance_scripts) process of pre-loading a queue
and then operating against the items on the queue.

### Preload

The preload script is defined as
```shell
drush uofm_aggregate_pdfs_preprocess (--pid|--pidlist|--pidfile|--query) [--skip_pages]
```

By default the script generates new PDFs for all pages. The `--skip_pages` argument will cause the script to
_NOT_ regenerate page level PDFs (if one exists) before making the aggregated one.

There is an alias to shorten the command to
```shell
drush uofm_agg_pdfs_pp (--pid|--pidlist|--pidfile|--query) [--skip_pages]
```

The preload drush script allows the addition of PIDS in the following ways. 

1. a single PID
    ```shell
    > drush -r /path/to/drupal uofm_aggregate_pdfs_preprocess --pid=islandora:99
    ```
2. a comma separated list of PIDS
    ```shell
    > drush -r /path/to/drupal uofm_aggregate_pdfs_preprocess --pidlist=islandora:99,islandora:54,islandora:20
    ```
3. a file containing one PID per line
    ```shell
    > drush -r /path/to/drupal uofm_aggregate_pdfs_preprocess --pidfile=/full/path/to/file
    ```
4. a triplestore query argument defining `?object`
    ```shell
    > drush -r /path/to/drupal uofm_aggregate_pdfs_preprocess --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>'
    ```

### Run

To run all the queued items use:
```shell
drush uofm_aggregate_pdfs_run
```

**NOTE** You _MUST_ provide a user ID or username with permissions to create/update the objects or the process will fail.

So a full command _might_ look like:
```shell
> drush -r /path/to/drupal -u 1 uofm_aggregate_pdfs_run 
```
or to define a specific user use:
```shell
> drush -r /path/to/drupal --user=<my username> uofm_aggregate_pdfs_run
```

There is also an alias for this command defined as:
```shell
drush uofm_agg_pdfs_run
```

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT