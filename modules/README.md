## Available submodules

1. [uofm_aggregate_pdfs](uofm_aggregate_pdfs)

    This allows the generation of aggregate (read: combined) PDFs for book and newspaper issue objects.

2. [uofm_archives_scan](uofm_archives_scan)

    Generates custom JSON information for all objects in a collection.

3. [uofm_batch_index](uofm_batch_index)

    Only works with [islandora-1x-solr-indexer](https://github.com/uml-digitalinitiatives/islandora-1x-solr-indexer) and adds items to be reindexed.

4. [uofm_batch_purge](uofm_batch_purge)

    Purges a list of objects from the repository.

5. [uofm_copy_license](uofm_copy_licence)

    Copies a mods:accessCondition license element from a source MODS record to a list of target MODS records.

6. [uofm_export_mods](uofm_export_mods)

    Exports the MODS records from a list of objects into separate files in a directory

7. [uofm_fix_dates](uofm_fix_dates)

    Fix incorrect dates in MODS records and labels in a few specific locations.

8. [uofm_jpg_default](uofm_jpg_default)

    Update a list of objects to get the datastream label of the `JPG` datastream to match the label of the `OBJ` datastream with `.jpg` appended. This runs on Drupal cron.

9. [uofm_mods_patch](uofm_mods_patch)

    This performs repeated and configurable patching of MODS records, by allowing you to define a small PHP script to do the alteration.

10. [uofm_pbcore_patch](uofm_pbcore_patch)

    This performs repeated and configurable patching of PBCORE records, by allowing you to define a small PHP script to do the alteration.

11. [uofm_regen_fits](uofm_regen_fits)

    This regenerates the FITS database for a bunch of objects.

12. [uofm_xml_patch](uofm_xml_patch)

    This is the supporting module for [uofm_mods_patch](uofm_mods_patch) and [uofm_pbcore_patch](uofm_pbcore_patch) defined above. 