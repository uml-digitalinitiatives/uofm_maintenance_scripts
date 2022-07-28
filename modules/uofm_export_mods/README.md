## Overview

This module exports the MODS records from a set of objects, one record per file.

Files are named using the PID with colons replaced with an underscore followed by _MODS.xml

ie. `islandora:456` becomes `islandora_456_MODS.xml`

## Usage

This works in the common (to the `uofm_maintenance_scripts`) process of pre-loading a queue
and then operating against the items on the queue.

### Preload

The preload script is defined as
```shell
drush uofm_export_mods_preprocess (--pid|--pidlist|--pidfile|--query) --directory=/path/to/output/directory [--add_hdl] [--add_pid] [--add_ids] [--limit]
```

You must provide a `--directory` argument with the _full path_ to where to output your files to.

Additionally there are the options to allow modification of the output MODS record.

* `--add_hdl` - Insert a mods:identifier type="HDL" element.
* `--add_pid` - Insert a mods:identifier type="PID" element.
* `--add_ids` - Sets both the `--add_hdl` and `--add_pid` options above.

Lastly you can specify a `--limit` option to cap the number of records to export, this is most
useful with the `--query` option below where the PIDs are not feed to the script.

i.e.
```shell
> drush uofm_export_mods_preprocess --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>' --directory=/path/to/output --limit=10
```

There is an alias to shorten the command to
```shell
drush uofm_export_mods_pp ...
```

The preload drush script allows the addition of PIDS in the following ways.

1. a single PID
    ```shell
    > drush -r /path/to/drupal uofm_export_mods_preprocess --pid=islandora:99 --directory=/path/to/output
    ```
2. a comma separated list of PIDS
    ```shell
    > drush -r /path/to/drupal uofm_export_mods_preprocess --pidlist=islandora:99,islandora:54,islandora:20 --directory=/path/to/output
    ```
3. a file containing one PID per line
    ```shell
    > drush -r /path/to/drupal uofm_export_mods_preprocess --pidfile=/full/path/to/file --directory=/path/to/output
    ```
4. a triplestore query argument defining `?object`
    ```shell
    > drush -r /path/to/drupal uofm_export_mods_preprocess --query='?object fedora-rels-ext:isMemberOf <info:fedora/islandora:bookCollection>' --directory=/path/to/output
    ```

### Run

To run all the queued items use:
```shell
drush uofm_export_mods_process
```

This will start the processing of the items already in the queue. You _should_ be able to start 
multiple instances of this process as they use the Drupal queue process and should not double process
the items.

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
