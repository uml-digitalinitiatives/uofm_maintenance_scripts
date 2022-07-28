## Overview

This module allows the batch modification of MODS records.

**WARNING**: You are patching your MODS en masse, this can be dangerous. Please be warned!!!

## Usage

This works in the common (to the `uofm_maintenance_scripts`) process of pre-loading a queue
and then operating against the items on the queue.

### Preload

This script is `uofm_mods_patch_preprocess` or `uofm_mpp`

The preload script is defined as
```shell
drush uofm_mods_patch_preprocess (--pid|--pidlist|--pidfile|--query) --patch_file=<path to PHP script file> --patch_method=<name of method in file> [--dont_update_dc] [--force_update_dc]
```

You create a PHP function that accepts two arguments:
1. the original MODS as a string.
2. the AbstractObject of the Islandora object.

and returns the altered MODS string.

i.e. 
```php
function fixCorporateName(string $mods, AbstractObject $object): string {
```

If the above method (`fixCorporateName`) was stored in `/home/example/patch.php`, then an example invocation would be
```shell
> drush -r /path/to/drupal uofm_mpp --pid=islandora:11 --patch_file=/home/example/patch.php --patch_method=fixCorporateName
```

By default if the MODS record is altered then the DC record is rebuilt to match the changes.

The last two optional arguments are:
* `--dont_update_dc` - Don't update the DC record _even if_ the MODS record _DOES_ change. 
* `--force_update_dc` - Update the DC record _even if_ the MODS record _DOES NOT_ change.

The MODS record is **ONLY** updated if the returned MODS is different from the original MODS.

The preload drush script allows the addition of PIDS in the following ways.

1. a single PID
    ```shell
    > drush -r /path/to/drupal uofm_mods_patch_preprocess --pid=islandora:99 --patch_file=/home/example/patch.php --patch_method=fixCorporateName
    ```
2. a comma separated list of PIDS
    ```shell
    > drush -r /path/to/drupal uofm_mods_patch_preprocess --pidlist=islandora:99,islandora:54,islandora:20 --patch_file=/home/example/patch.php --patch_method=fixCorporateName
    ```
3. a file containing one PID per line
    ```shell
    > drush -r /path/to/drupal uofm_mods_patch_preprocess --pidfile=/full/path/to/file --patch_file=/home/example/patch.php --patch_method=fixCorporateName
    ```
4. a triplestore query argument defining `?object`
    ```shell
    > drush -r /path/to/drupal uofm_mods_patch_preprocess --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>' --patch_file=/home/example/patch.php --patch_method=fixCorporateName
    ```

### Run

To run all the queued items use:
```shell
drush uofm_mods_patch_run
```

**NOTE** You _MUST_ provide a user ID or username with permissions to create/update the objects or the process will fail.

So a full command _might_ look like:
```shell
> drush -r /path/to/drupal -u 1 uofm_mods_patch_run 
```
or to define a specific user use:
```shell
> drush -r /path/to/drupal --user=<my username> uofm_mods_patch_run
```

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT