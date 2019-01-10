## Overview

This copies a mods:accessCondition license from one source object to one or more target objects.

It only handles elements without a `@type` and elements with a `@type="Creative Commons License"`. 

It will overwrite the license in the target object with the same type from the source, but will 
merge in the case of different types of licenses.

Examples:

1. If we have:

* Source with license `<mods:accessCondition>My license</mods:accessCondition>`
* Target with no license

  The target will end up with `<mods:accessCondition>My license</mods:accessCondition>`


If we have:

* Source with license `<mods:accessCondition>My license</mods:accessCondition>`
* Target with license `<mods:accessCondition>Some license</mods:accessCondition>`

  The target will end up with `<mods:accessCondition>My license</mods:accessCondition>`

If we have: 

* Source with license `<mods:accessCondition>My license</mods:accessCondition>`
* Target has license `<mods:accessCondition @type="Creative Commons License">by-nc/sa</mods:accessCondition>`

  The target with end up with
  ```xml
  <mods:accessCondition>My license</mods:accessCondition>
  <mods:accessCondition @type="Creative Commons License">by-nc/sa</mods:accessCondition>
  ```

## Usage

This tool is a two stage operation, stage one populates a queue with the objects to operation on.
Stage two iterates over the queue.

### Stage one

The command is `uofm_copy_license_preprocess` or `uofm_copy_cc_pp`.

You must provide a `--source_pid` argument pointing to the object to copy the license from.

To populate the queue you must also provide one of the following options. 

* `--query` - Provide a SPARQL where clause, uses "?object" as the returned variable.
* `--pid` - A PID to operate on
* `--pidlist` - A comma seperated list of PIDs to operate on
* `--pidfile` - Path to a textfile of PIDs to operate on, one per line

### Stage two

The command is `uofm_copy_license_process`.

This will start the processing of the items already in the queue. You _should_ be able to start 
multiple instances of this process as they use the Drupal queue process and should not double process
the items.

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
