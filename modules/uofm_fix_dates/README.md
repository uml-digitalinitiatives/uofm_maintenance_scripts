## Overview

This module changes the year in the MODS/DC records for a bunch of items.

We have had errors in dates, especially in newspapers where the error propogates to all the pages
and this cause the order to be incorrect.
 
This module takes a set of PIDs and **old** year and **new** year.

It checks the following:
1. object label
1. these MODS XPaths
    * `/mods:mods/mods:originInfo/mods:dateIssued[@encoding="iso8601"]`
    * `/mods:mods/mods:titleInfo/mods:title`
    * `/mods:mods/mods:relatedItem[@type="host"]/mods:part/mods:date[@encoding="iso8601"]`
1. these DC XPaths
    * `/oai_dc:dc/dc:title`
    * `/oai_dc:dc/dc:date`
   
It attempts to do a string replace of the **old** year to the **new** year and if is alters the value
then patch is added using the Manidora Patcher. 

## Usage

This tool is a two stage operation, stage one populates a queue with the objects to operation on.
Stage two iterates over the queue.

### Stage one

The command is `uofm_fix_dates_preprocess` or `uofm_fix_dates_pp`.

You must provide a `--old_year` and `--new_year` argument to specify the replacement happening.

To populate the queue you must also provide one of the following options. 

* `--query` - Provide a SPARQL where clause, uses "?object" as the returned variable.
* `--pid` - A PID to operate on
* `--pidlist` - A comma seperated list of PIDs to operate on
* `--pidfile` - Path to a textfile of PIDs to operate on, one per line

### Stage two

The command is `uofm_fix_dates_run`.

This will start the processing of the items already in the queue. You _should_ be able to start 
multiple instances of this process as they use the Drupal queue process and should not double process
the items.

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
