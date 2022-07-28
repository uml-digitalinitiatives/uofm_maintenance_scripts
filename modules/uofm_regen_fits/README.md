## Overview

This module allows the batch regeneration of FITS datastreams.

## Usage

This works in the common (to the `uofm_maintenance_scripts`) process of pre-loading a queue
and then operating against the items on the queue.

### Preload

The preload script is defined as
```shell
drush uofm_maintenance_regen_fits_pp (--pid|--pidlist|--pidfile|--query) [--recursive]
```

The `--recursive` option will cause the each PID to be searched for immediate children and add them to the queue.

There is an alias to shorten the command to
```shell
drush uofm_regen_fits_pp (--pid|--pidlist|--pidfile|--query) [--recursive]
```

The preload drush script allows the addition of PIDS in the following ways.

1. a single PID
    ```shell
    > drush -r /path/to/drupal uofm_maintenance_regen_fits_pp --pid=islandora:99
    ```
2. a comma separated list of PIDS
    ```shell
    > drush -r /path/to/drupal uofm_maintenance_regen_fits_pp --pidlist=islandora:99,islandora:54,islandora:20
    ```
3. a file containing one PID per line
    ```shell
    > drush -r /path/to/drupal uofm_maintenance_regen_fits_pp --pidfile=/full/path/to/file
    ```
4. a triplestore query argument defining `?object`
    ```shell
    > drush -r /path/to/drupal uofm_maintenance_regen_fits_pp --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>'
    ```

### Run

To run all the queued items use:
```shell
drush uofm_maintenance_regen_fits_run
```

**NOTE** You _MUST_ provide a user ID or username with permissions to create/update the objects or the process will fail.

So a full command _might_ look like:
```shell
> drush -r /path/to/drupal -u 1 uofm_maintenance_regen_fits_run 
```
or to define a specific user use:
```shell
> drush -r /path/to/drupal --user=<my username> uofm_maintenance_regen_fits_run
```

There is also an alias for this command defined as:
```shell
drush uofm_regen_fits_run
```

## Credit

* [Nick Ruest](https://github.com/ruebot)

## License

* MIT