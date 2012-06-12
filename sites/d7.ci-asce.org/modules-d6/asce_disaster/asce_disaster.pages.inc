<?php
// $Id$

/**
 * @file
 * Page callbacks
 */

function asce_disaster_main() {
	global $user;
  if ($user->uid) {
    $output = '<p>';
    $output .= 'This is going to be the main page of the Investiagtive Team site. ';
    $output .= '</p>';
    drupal_goto('node/6336');
  }
  else {
    return drupal_get_form('user_login');
  }
}

function asce_disaster_application() {
	module_load_include('inc', 'node', 'node.pages');
	$type = 'field-investigation-application'; 
	$output = node_add($type);
	return $output;
}

function asce_disaster_staff() {
	$output .= '<p>This page will contain links to reports and other administrative functions.</p>';
	$content[] = array(
	  'title' =>  'Edit Application Form',
	  'href' => 'admin/content/node-type/ci-student-days-seattle/fields',
	  'localized_options' => NULL,
	  'description' => 'Add, modify, or remove fields from the application form.', 
	);
	$output .= theme('asce_disaster_staff_menu', $content);
	return $output;
}

function theme_asce_disaster_staff_menu($content) {
  $output = '';
  if ($content) {
    $output = '<dl class="node-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  return $output;
}

function asce_disaster_add_content() {
	module_load_include('inc', 'node', 'node.pages');
	$output = node_add('ci_content');
	return $output;
}
