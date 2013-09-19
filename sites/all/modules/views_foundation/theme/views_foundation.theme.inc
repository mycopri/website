<?php

/**
 * @file
 * The theme system, which controls the output of views foundation plugins.
 */

/**
 * Theme the Foundation Orbit Slider.
 */
function template_preprocess_views_foundation_orbit(&$variables) {

  // Give each Orbit slider a unique id.
  $orbit_id = $variables['view']->vid . $variables['view']->current_display;
  $variables['orbit_id'] = $orbit_id;

  $variables['options']['captions'] = FALSE;
  // Add caption field to $variables array if it is selected.
  if (!empty($variables['options']['caption_field'])) {
    foreach ($variables['view']->style_plugin->rendered_fields as $index => $row) {
      foreach ($row as $key => $value) {
        if ($key == $variables['options']['caption_field']) {
          $variables['captions'][] = check_plain(strip_tags($value));
        }
      }
    }
    $variables['options']['captions'] = TRUE;
  }
  // Prepare orbit slider settings.
  extract($variables['options']);
  $options['viewsFoundation'] = array(
    $orbit_id => array(
      'animation' => check_plain($animation),
      'animationSpeed' => (int) $animation_speed,
      'advanceSpeed' => (int) $advance_speed,
      'directionalNav' => (bool) $directional_nav,
      'timer' => (bool) $timer,
      'fluid' => $fluid == 'true' ? TRUE : check_plain($fluid),
      'bullets' => (bool) $bullets,
      'captions' => $captions,
    ),
  );

  // Add JavaScript files.
  $path = drupal_get_path('module', 'views_foundation');
  drupal_add_js(check_url(variable_get('views_foundation_js')) . '/jquery.foundation.orbit.js');
  drupal_add_js($path . '/views.foundation.js', array('group' => JS_THEME));
  // Add Orbit settings.
  drupal_add_js($options, 'setting');
}

/**
 * Theme the Foundation Orbit Clearing.
 */
function template_preprocess_views_foundation_clearing(&$variables) {
  // Preprare fields for the output.
  foreach ($variables['view']->style_plugin->rendered_fields as $index => $row) {
    foreach ($row as $key => $value) {
      if ($variables['options']['image'] == $key) {
        // Get image url from img tag.
        if ($value) {
          $url = simplexml_import_dom(DOMDocument::loadHTML($value))->xpath("//img/@src");
          $variables['fields'][$index]['image'] = check_url(reset($url));
        }
      }
      elseif ($variables['options']['thumbnail'] == $key) {
        // Get thumbnail url from img tag.
        if ($value) {
          $url = simplexml_import_dom(DOMDocument::loadHTML($value))->xpath("//img/@src");
          $variables['fields'][$index]['thumbnail'] = check_url(reset($url));
        }
      }
      elseif ($variables['options']['caption'] == $key) {
        $value ? $variables['fields'][$index]['caption'] = check_plain(strip_tags($value)) : '';
      }
    }
    if (!isset($variables['fields'][$index]['image'])) {
      // Unset row if main image not found.
      unset($variables['fields'][$index]);
    }
    elseif (!isset($variables['fields'][$index]['thumbnail'])) {
      // Use main image for thumbnail if thumbnail not set.
      $variables['fields'][$index]['thumbnail'] = $variables['fields'][$index]['image'];
    }
  }
  // Add classes for grid and thumbnail border.
  $grid = drupal_html_class($variables['options']['thumb_columns']);
  $mobile_grid = drupal_html_class($variables['options']['thumb_columns_mobile']);
  $variables['class']['link'] = $variables['options']['thumb_border'] ? 'clearing-link th' : 'clearing-link';
  $variables['class']['grid'] = 'clearing block-grid ' . $grid . '-up mobile-' . $mobile_grid . '-up';

  // Add Foundation Clearing JavaScript.
  drupal_add_js(check_url(variable_get('views_foundation_js')) . '/jquery.foundation.clearing.js');
}

/**
 * Theme the Foundation Navigation.
 */
function template_preprocess_views_foundation_navigation(&$variables) {
  $view = $variables['view'];
  $options = $variables['options'];
  $variables['fields'] = array();

  // Preprare fields for the output.
  foreach ($view->style_plugin->rendered_fields as $index => $row) {
    $variables['fields'][$index]['content'] = '';

    foreach ($row as $key => $value) {

      if ($options['nav_field'] == $key) {
        // New array for single row output.
        $variables['fields'][$index] = array(
          'is_active' => '',
          'link' => $value,
          'content' => '',
          'wrapper_prefix' => '',
          'wrapper_suffix' => '',
        );
        // Check if the link has "active" class.
        if (strpos($value, 'active') > 0) {
          $variables['fields'][$index]['is_active'] = 'active';
        }
        $class = 'flyout';
        // Add "right" class to submenu.
        if ($options['nav_count'] && $index >= $options['nav_count']) {
          $class .= ' right';
        }
        // Add "large" class to submenu (if selected in the options).
        if ($options['nav_large']) {
          $class .= ' large';
        }
        // Prepare submenu wrappers.
        if ($options['nav_link']) {
          $variables['fields'][$index]['wrapper_prefix'] = '<ul class="' . $class . '">';
          $variables['fields'][$index]['wrapper_suffix'] = '</ul>';
        }
        else {
          $variables['fields'][$index]['wrapper_prefix'] = '<div class="' . $class . '">';
          $variables['fields'][$index]['wrapper_suffix'] = '</div>';
        }
      }
      // Add "li" markup to submenu items if 'menu' selected in the options.
      elseif ($options['nav_link']) {
        $variables['fields'][$index]['content'] .= '<li>' . $value . '</li>';
      }
    }
  }
  if (!$options['nav_link']) {
    foreach ($variables['rows'] as $index => $row) {
      $variables['fields'][$index]['content'] = $row;
    }
  }
  drupal_add_js(check_url(variable_get('views_foundation_js')) . '/jquery.foundation.navigation.js');
}

/**
 * Theme the Foundation Pricing Tables row.
 */
function template_preprocess_views_foundation_pricing_tables(&$variables) {
  $view = $variables['view'];
  $options = $variables['options'];
  $variables['fields'] = array(
    'title_field' => '',
    'description_field' => '',
    'price_field' => '',
    'bullet_fields' => array(),
    'button_field' => '',
  );
  foreach ($view->field as $id => $field) {
    $item = $view->field[$id]->theme($variables['row']);
    if ($options['title_field'] == $id) {
      $variables['fields']['title_field'] = $item;
    }
    elseif ($options['price_field'] == $id) {
      $variables['fields']['price_field'] = $item;
    }
    elseif ($options['description_field'] == $id) {
      $variables['fields']['description_field'] = $item;
    }
    elseif ($options['button_field'] == $id) {
      $variables['fields']['button_field'] = $item;
    }
    elseif ($field) {
      $variables['fields']['bullet_fields'][] = $item;
    }
  }
}
