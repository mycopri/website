<?php
// $Id$

/**
 * @file
 * Definition of the 'default' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('ASCE'),
  'description' => t('The ASCE panel rendering style.'),
  'render region' => 'asce_panels_style_render_region',
  'render pane' => 'asce_panels_style_render_pane',  
  'settings form' => 'asce_panels_style_settings_form',
  'pane settings form' => 'asce_panels_style_settings_form',
  'weight' => 0,
);



/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_asce_panels_style_render_region($display, $region_id, $panes, $settings) {
  drupal_add_js('misc/collapse.js');
  if ($settings['behavior'] == 'collapsible') {
    $output = '<fieldset class="fieldgroup group-general collapsible">';
  }
  elseif ($settings['behavior'] == 'collapsed') {
  	$output = '<fieldset class="fieldgroup group-general collapsed">';
  }
  else {
  	$output = '<fieldset class="fieldgroup group-general">';
  }
  if ($settings['behavior'] != 'open') {
    $output .= '<legend><a href="#">' . $settings['legend'] . '</a></legend>';
  }
  $print_separator = TRUE;
  $first_pane = TRUE;
  foreach ($panes as $pane_id => $pane_output) {
    // Add the separator if we've already displayed a pane.
    if ($print_separator && !$first_pane) {
      $output .= '<div class="panel-region-separator">&nbsp</div>';
    }

    $output .= $pane_output;
    // If we displayed a pane, this will become true; if not, it will become
    // false.
    $print_separator = (bool) $pane_output;
    $first_pane = FALSE;
  }
  $output .= '</fieldset>';
  return $output;
}

function theme_asce_panels_style_render_pane($content, $pane, $display) {
  $settings = $pane->style['settings'];
	drupal_add_js('misc/collapse.js');
  if ($settings['behavior'] == 'collapsible') {
    $output = '<fieldset class="fieldgroup group-general collapsible">';
  }
  elseif ($settings['behavior'] == 'collapsed') {
    $output = '<fieldset class="fieldgroup group-general collapsed">';
  }
  else {
    $output = '<fieldset class="fieldgroup group-general">';
  }
  if ($settings['behavior'] != 'open') {
    $output .= '<legend><a href="#">' . $settings['legend'] . '</a></legend>';
  }
  
  $output .= theme('panels_pane', $content, $pane, $display) . '</fieldset>';
  
  return $output;
	}

/**
 * Settings form callback.
 */
function asce_panels_style_settings_form($style_settings) {
  $form['behavior'] = array(
    '#type' => 'select',
    '#title' => t('Panel behavior'),
    '#options' => array(
      'open' => t('Always open'),
      'collapsible' => t('Collapsible'),
      'collapsed' => t('Collapsed'),
    ),
    '#default_value' => 'collapsible',
  );
  $form['legend'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => 'Please provide a title in the style settings',
    '#size' => 60,
    '#maxlength' => 128,
    '#description' => t('If you choose collapsible, you must provide a title.'),
  );
  return $form;
}
