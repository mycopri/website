<?php
// $Id: image_gallery_access.views.inc,v 1.1 2009/11/21 13:42:00 salvis Exp $

/**
 * @file
 * Views integration for the mycopri_features module.
 */

/**
 * Implementation of hook_views_plugins().
 */
function mycopri_features_views_plugins() {
  return array(
    'access' => array(
      'mycopri_features' => array(
        'title' => t('Only group nodes'),
        'help' => t('Access will be granted to only group nodes.'),
        'handler' => 'mycopri_features_plugin_access',
        'uses options' => FALSE,
        'path' => drupal_get_path('module', 'mycopri_features') . '/views',
      ),
    ),
  );
}

