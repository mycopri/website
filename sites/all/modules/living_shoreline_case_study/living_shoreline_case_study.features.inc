<?php
/**
 * @file
 * living_shoreline_case_study.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function living_shoreline_case_study_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function living_shoreline_case_study_node_info() {
  $items = array(
    'living_shoreline_case_study' => array(
      'name' => t('Living Shoreline Case Study'),
      'base' => 'node_content',
      'description' => t('Living Shoreline Case Study'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
