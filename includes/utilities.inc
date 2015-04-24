<?php
/**
 * @file
 * Utility and shared functions
 */

/**
 * Load/query a list, file, Sparql query or single PID and return a list.
 * @param array - with keys
 *   "type" = ("single", "list", "file" or "query") and
 *   "value" which is a PID for single, a list for list, etc.
 * @return array of PIDs
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
            // Unable to open file
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
          // File does not exist
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