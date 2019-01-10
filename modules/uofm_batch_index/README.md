## Overview

This submits one or more PIDs to be indexed or re-indexed in Solr. If the object already exists in Solr
and you haven't **forced** a reindex it is skipped.

**This is NOT compatible with FedoraGSearch**

## Usage

The command is `uofm_batch_index_jms` or `uofm_bi_jms`.

**Note**: Unlike other tools in this package this does not have a two step process. It begins
immediately.
 
To submit PIDs to be indexed/reindexed you must also provide one of the following options. 

* `--query` - Provide a SPARQL where clause, uses "?object" as the returned variable.
* `--pid` - A PID to operate on
* `--pidlist` - A comma seperated list of PIDs to operate on
* `--pidfile` - Path to a textfile of PIDs to operate on, one per line

Additional options:

`--force`

This will reindex an object even if it exists in the Solr index. 

**Be careful** this option could cause a lot of reindexing to occur.

`--recursive`

This option will search for children of the object being re-indexed using `isMemberOf`, `isMemberOfCollection`,
and `isConsituentOf` relationships.

## Configuration

The base URL used to submit the PID to be reindexed against is the constant `UOFM_BATCH_INDEX_REINDEX_URL` defined
in the `uofm_batch_index.drush.inc` file.

## Credit

* [Jared Whiklo](https://github.com/whikloj)

## License

* MIT
