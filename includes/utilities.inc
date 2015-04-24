<?php
/**
 * @file
 * Utility and shared functions
 */

/**
 * Load/query a list, file, Sparql query or single PID and return a list.
 *
 * @param array $data
 *   with keys
 *   "type" = ("single", "list", "file" or "query") and
 *   "value" which is a PID for single, a list for list, etc.
 *
 * @return array
 *   array of PIDs
 */
function uofm_maintenance_load_pids(array $data) {

  $results = array();
  if (isset($data['type'])) {
    switch ($data['type']) {
      case 'single':
        $results[] = $data['value'];
        break;

      case 'list':
        $list = $data['value'];
        $tmp_res = explode(',', $list);
        foreach ($tmp_res as $t) {
          if (strlen(trim($t)) > 0) {
            $results[] = trim($t);
          }
        }
        break;

      case 'file':
        if (file_exists($data['value'])) {
          $fp = @fopen($data['value'], 'r');
          if (!$fp) {
            // Unable to open file.
            return -9;
          }
          while (!feof($fp)) {
            $line = trim(fgets($fp, 4096));
            if (!empty($line)) {
              $results[] = trim($line);
            }
          }
        }
        else {
          // File does not exist.
          return -8;
        }
        break;

      case 'query':
        $query_param = $data['value'];
        $results = uofm_maintenance_sparql_query($query_param);
        break;
    }
  }
  return $results;
}

/**
 * Perform our Sparql query and return an array of PIDs.
 *
 * @param string $where
 *   the where clause of a sparql query that defines what ?object will be.
 *
 * @return array
 *   an array of PIDs.
 */
function uofm_maintenance_sparql_query($where) {
  drupal_static_reset('islandora_get_tuque_connection');
  $connection = islandora_get_tuque_connection();

  $query = 'select ?object from <#ri> where { ';
  $query .= $where;
  $query .= ' }';

  $temp = $connection->repository->ri->sparqlQuery($query, 'unlimited');

  $get_pid = function($o) {
    if (is_array($o) && array_key_exists('object', $o)) {
      return $o['object']['value'];
    }
  };

  return array_map($get_pid, $temp);
}

/**
 * Recurse down to the children.
 *
 * @param array $pids
 *   array of pids to retrieve the children for.
 * @param array $results
 *   array of previous results to add new children to
 *
 * @return array
 *   array of pids
 */
function uofm_maintenance_recursive_query($pids, $results = array()) {
  foreach ($pids as $parent) {
    $results[] = $parent;
    $children = uofm_maintenance_query_get_children($parent);
    if (is_array($children) && count($children) > 0) {
      $results = uofm_maintenance_recursive_query($children, $results);
    }
  }
  return $results;
}

/**
 * Query for the children of an object.
 *
 * @param string $parent
 *   the pid of the parent to find children for.
 *
 * @return array
 *   array of pids.
 */
function uofm_maintenance_query_get_children($parent) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $results = array();
  if (islandora_is_valid_pid($parent)) {
    $query = "?object <fedora-rels-ext:isMemberOf> <info:fedora/$parent>";
    $results = uofm_maintenance_sparql_query($query);
  }
  return $results;
}

/**
 * Filter an array of PIDs to remove those in the Solr index.
 *
 * @param array $items
 *   array of PIDs
 *
 * @return array
 *   filtered array of PIDs
 */
function uofm_maintenance_batch_index_filter(array $items) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('php', 'islandora_solr', 'SolrPhpClient/Apache/Solr/Service');

  $queue = DrupalQueue::get(UOFM_BATCH_INDEX_QUEUE);
  $path_parts = parse_url(variable_get('islandora_solr_url', 'localhost:8080/solr'));
  $solr = new Apache_Solr_Service($path_parts['host'], $path_parts['port'], $path_parts['path'] . '/');
  $solr->setCreateDocuments(0);

  // Make a long string of colon escaped PIDs.
  $query_pids = str_replace(':', '\\:', implode($items, " "));
  // Search for all the possible PIDs.
  $solr_query = "+PID:($query_pids)";
  $solr_params = array('fl' => 'PID');
  $res = $solr->search($solr_query, 0, count($items), $solr_params);
  if ($res && $res->response->numFound < count($items)) {
    // At least one is missing, get the missing PIDs.
    $missing = array_diff($items, $res->response->docs);
    return $missing;
  }
  else {
    return array();
  }
}
