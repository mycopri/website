<?php
// $Id$

/**
 * Implementation of hook_views_plugins
 */
function ci_content_views_plugins() {
  return array(
    'row' => array(
      'ci_content_teaser' => array(
        'title' => t('CI Content Teaser'),
        'help' => t('Display a nicely formatted teaser of CI Content.'),
        'handler' => 'ci_content_teaser_plugin_row_node_view',
        'theme' => 'ci_content_teaser_view_row_node',
        'theme file' => 'ci_content.views.inc',
        'theme path' => drupal_get_path('module', 'ci_content'),
        'base' => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
    ),
  );
}

/**
 * Template helper for theme_views_view_row_node
 */
function template_preprocess_ci_content_teaser_view_row_node(&$vars) {
  $options = $vars['options'];

  // Make sure the variables are defined.
  $vars['node'] = '';
  $vars['comments'] = '';

  if (!empty($vars['row']->{$vars['field_alias']})) {
    $nid = $vars['row']->{$vars['field_alias']};
  }
  if (!is_numeric($nid)) {
    return;
  }

  $node = node_load($nid);

  if (empty($node)) {
    return;
  }

  $node->view = $vars['view'];
  $node->build_mode = ($options['build_mode'] == 'teaser' || $options['build_mode'] == 'full') ? NODE_BUILD_NORMAL : $options['build_mode'];
  $vars['node'] = ci_content_teaser_view($node, $options['build_mode'] == 'teaser', FALSE, $options['links']);
	//$vars['node'] = node_view($node, $options['build_mode'] == 'teaser', FALSE, $options['links']);
	
  if (!empty($options['comments']) && function_exists('comment_render')) {
    $vars['comments'] = comment_render($node);
  }
}