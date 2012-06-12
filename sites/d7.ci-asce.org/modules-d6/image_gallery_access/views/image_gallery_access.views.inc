<?php
// $Id: image_gallery_access.views.inc,v 1.1 2009/11/21 13:42:00 salvis Exp $

/**
 * @file
 * Views integration for the Image Gallery Access module.
 */

/**
 * Implementation of hook_views_plugins().
 */
function image_gallery_access_views_plugins() {
  return array(
    'access' => array(
      'image_gallery_access' => array(
        'title' => t('by @Image_Gallery_Access', array('@Image_Gallery_Access' => 'Image Gallery Access')),
        'help' => t('Access will be granted to users who can view at least one gallery.'),
        'handler' => 'image_gallery_access_plugin_access',
        'uses options' => FALSE,
        'path' => drupal_get_path('module', 'image_gallery_access') . '/views',
      ),
    ),
/*
    'argument validator' => array(
      'image_gallery_access_taxonomy_term' => array(
        'title' => t('Image Gallery Access'),
        'handler' => 'image_gallery_access_plugin_argument_validate_taxonomy_term',
        'parent' => 'views_plugin_argument_validate_taxonomy_term',
        'path' => drupal_get_path('module', 'image_gallery_access') . '/views',
      ),
    ),
/**/
  );
}

