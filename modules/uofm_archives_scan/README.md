## Overview

This script generates a JSON listing of objects inside a collection with each objects:
- PID
- Models
- Local ID (if defined in the MODS)
- Relationships (if one of)
  - 'isPartOf', 'isConstituentOf', 'isMemberOf', 'isMemberOfCollection'

## Usage

The command is `uofm_archives_scan` or `uofm_arch_scan`.

**Note**: Unlike other tools in this package this does not have a two step process. It begins
immediately.
 
To submit PIDs to be indexed/reindexed you must also provide a collection PID to operate on using
the `--pid` argument and an output file to store the JSON in using the `--output` argument.

An example run _might_ look like
```shell
> drush -r /path/to/drupal uofm_archives_scan --pid=islandora:bookCollection --output=/full/path/to/output.json
```

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT