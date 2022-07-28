## Overview

This drush script is to purge a list of objects from an Islandora 7.x repository.

## Usage
This works in the common (to the `uofm_maintenance_scripts`) process of pre-loading a queue
and then operating against the items on the queue.

### Preload

The preload script is defined as:
```shell
drush uofm_batch_purge_preprocess (--pid|--pidlist|--pidfile|--query) [--remove_orphans]
```
or its alias
```shell
drush uofm_batch_purge_pp (--pid|--pidlist|--pidfile|--query) [--remove_orphans]
```

**NOTE**: By default the script does _NOT_ purge children and so orphans can be created. By using the `--remove_orphans`
argument the process with remove any object that does not have any remaining parents.

So if an object (A) has multiple parents (B, C) and you purge one of those parent (B) then because there is a remaining
parent (C) the original child (A) should remain. However if an object A 
only has a single parent (B) and you purge this parent (B) using this option, then the child (A) will also be purged. 

The preload drush script allows the addition of PIDS in the following ways.

1. a single PID
    ```shell
    > drush -r /path/to/drupal uofm_batch_purge_preprocess --pid=islandora:99
    ```
2. a comma separated list of PIDS
    ```shell
    > drush -r /path/to/drupal uofm_batch_purge_preprocess --pidlist=islandora:99,islandora:54,islandora:20
    ```
3. a file containing one PID per line
    ```shell
    > drush -r /path/to/drupal uofm_batch_purge_preprocess --pidfile=/full/path/to/file
    ```
4. a triplestore query argument defining `?object`
    ```shell
    > drush -r /path/to/drupal uofm_batch_purge_preprocess --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>'
    ```

### Run

To run all the queued items use:
```shell
drush uofm_batch_purge_run
```

**NOTE** You _MUST_ provide a user ID or username with permissions to create/update the objects or the process will fail.

So a full command _might_ look like:
```shell
> drush -r /path/to/drupal -u 1 uofm_batch_purge_run 
```
or to define a specific user use:
```shell
> drush -r /path/to/drupal --user=<my username> uofm_batch_purge_run
```

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
