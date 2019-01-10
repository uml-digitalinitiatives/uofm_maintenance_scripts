## Overview

This module exports the MODS records from a set of objects, one record per file.

Files are named using the PID with colons replaced with an underscore followed by _MODS.xml

ie. `islandora:456` becomes `islandora_456_MODS.xml`

## Usage

This tool is a two stage operation, stage one populates a queue with the objects to operation on.
Stage two iterates over the queue.

### Stage one

The command is `uofm_export_mods_preprocess` or `uofm_export_mods_pp`.

You must provide a `--directory` argument pointing to where to output your files to.

To populate the queue you must also provide one of the following options. 

* `--query` - Provide a SPARQL where clause, uses "?object" as the returned variable.
* `--pid` - A PID to operate on
* `--pidlist` - A comma seperated list of PIDs to operate on
* `--pidfile` - Path to a textfile of PIDs to operate on, one per line

Additionally there are the options to allow modification of the output MODS record.

* `--add_hdl` - Insert a mods:indetifier type="HDL" element.
* `--add_pid` - Insert a mods:indetifier type="PID" element.
* `--add_ids` - Sets both the `--add_hdl` and `--add_pid` options above.

Lastly you can specify a `--limit` option to cap the number of records to export, this is most 
useful with the `--query` option above where the PIDs are not feed to the script.

### Stage two

The command is `uofm_export_mods_process`.

This will start the processing of the items already in the queue. You _should_ be able to start 
multiple instances of this process as they use the Drupal queue process and should not double process
the items.

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
