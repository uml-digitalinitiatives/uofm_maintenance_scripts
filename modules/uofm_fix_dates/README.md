## Overview

This module changes the date in the MODS/DC records for a bunch of items.

We have had errors in dates, especially in newspapers where the error propogates to all the pages
and this cause the order to be incorrect.
 
This module takes a set of PIDs and **old** date and **new** date.

It checks the following:
1. object label
1. these MODS XPaths
    * `/mods:mods/mods:originInfo/mods:dateIssued[@encoding="iso8601"]`
    * `/mods:mods/mods:titleInfo/mods:title`
    * `/mods:mods/mods:relatedItem[@type="host"]/mods:part/mods:date[@encoding="iso8601"]`
1. these DC XPaths
    * `/oai_dc:dc/dc:title`
    * `/oai_dc:dc/dc:date`
   
It attempts to do a string replace of the **old** date to the **new** date and if is alters the value
then patch is added using the [Manidora](https://github.com/uml-digitalinitiatives/manidora) Patcher. 

## Usage

This tool is a two stage operation, stage one populates a queue with the objects to operation on.
Stage two iterates over the queue.

### Stage one

The command is `uofm_fix_dates_preprocess` or `uofm_fix_dates_pp`.

You must provide a `--old_year` and `--new_year` argument to specify the replacement happening.

The `--old_year` and `--new_year` can have the format `YYYY` or `YYYY-MM` or `YYYY-MM-DD`.

But both `--old_year` and `--new_year` must have the same format! The script will validate this check for you.

The preload drush script allows the addition of PIDS in the following ways.

1. a single PID
    ```shell
    > drush -r /path/to/drupal uofm_fix_dates_preprocess --pid=islandora:99 --old_year=1910 --new_year=1911
    ```
2. a comma separated list of PIDS
    ```shell
    > drush -r /path/to/drupal uofm_fix_dates_preprocess --pidlist=islandora:99,islandora:54,islandora:20 --old_year=1910-12 --new_year=1911-12
    ```
3. a file containing one PID per line
    ```shell
    > drush -r /path/to/drupal uofm_fix_dates_preprocess --pidfile=/full/path/to/file --old_year=1910-12-24 --new_year=1910-12-01
    ```
4. a triplestore query argument defining `?object`
    ```shell
    > drush -r /path/to/drupal uofm_fix_dates_preprocess --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>' --old_year=1980 --new_year=1980
    ```

### Stage two

The command is `uofm_fix_dates_run`.

This will start the processing of the items already in the queue. You _should_ be able to start 
multiple instances of this process as they use the Drupal queue process and should not double process
the items.

**NOTE** You _MUST_ provide a user ID or username with permissions to create/update the objects or the process will fail.

So a full command _might_ look like:
```shell
> drush -r /path/to/drupal -u 1 uofm_fix_dates_run 
```
or to define a specific user use:
```shell
> drush -r /path/to/drupal --user=<my username> uofm_fix_dates_run
```

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
