<?php
// $Id$

/**
 * @file
 * Page callbacks
 */

function asce_home_main() {
	global $user;
  if ($user->uid) {
    $output = '<p>';
    $output .= 'This is going to be the main page of the Student Days site. ';
    $output .= 'From here, the students will be able to apply and provide their information.';
    $output .= '</p><p>All of the navigation will be via the tabs at the top. In addition, staff will be ';
    $output .= 'to see other tabs that will take them to administrative pages.';
    $output .= '</p>';
    module_load_include('inc', 'page_manager', 'plugins/tasks/page');
    return page_manager_page_execute('student_days_main');
  }
  else {
    return drupal_get_form('user_login');
  }
}

function asce_home_application() {
	module_load_include('inc', 'node', 'node.pages');
	$type = 'ci-student-days-seattle'; 
	$output = node_add($type);
	return $output;
}

function asce_home_staff() {
	$output .= '<p>This page will contain links to reports and other administrative functions.</p>';
	$content[] = array(
	  'title' =>  'Edit Application Form',
	  'href' => 'admin/content/node-type/ci-student-days-seattle/fields',
	  'localized_options' => NULL,
	  'description' => 'Add, modify, or remove fields from the application form.', 
	);
	$output .= theme('asce_student_staff_menu', $content);
	return $output;
}

function theme_asce_student_staff_menu($content) {
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

function asce_home_add_content() {
	module_load_include('inc', 'node', 'node.pages');
	$output = node_add('ci_content');
	return $output;
}

function asce_home_edit_teaser($node){
	return drupal_get_form('asce_home_teaser_form', $node);
}

function asce_home_teaser_form(&$form_state, $node) {
	$help .= '<p>This form allows you to edit the teaser of the node. The body of the node is provided ';
	$help .= 'to aid in creating an appropriate teaser.</p>';
	$help .= '<p><em>Note that changes to the body will not be save upon submit.</em> Only changes to the teaser will.</p>';
	$form_state['storage']['node'] = $node;
	$form['help'] = array(
	  '#value' => t($help),
	);
	$form['format_1']['teaser'] = array(
    '#type' => 'textarea',
    '#title' => t('Teaser'),
    '#default_value' => $node->teaser,
    '#rows' => 20,
  );
  $form['format_1']['format'] = filter_form(2, 1, array('format_1','teaser'));
  
  // Show the full body to aid in creating the teaser.
  $form['format_2']['body-preview'] = array(
    '#type' => 'textarea',
    '#title' => t('Body preview'),
    '#default_value' => $node->body,
    '#rows' => 20,
  );
  $form['format_2']['format'] = filter_form(2, 1, array('format_2'));
  
  // Add the buttons.
  $form['buttons'] = array();
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
  return $form;
}
function asce_home_teaser_form_validate($form, &$form_state) {
}

function asce_home_teaser_form_submit($form, &$form_state) {
	$node = $form_state['storage']['node'];
	$sql = "SELECT * FROM {node_revisions} WHERE vid = {$node->vid}";
	$row = db_fetch_object(db_query($sql));
  $row->teaser = $form_state['values']['teaser'];
	
  drupal_write_record('node_revisions', $row, 'vid');
  
	unset($form_state['storage']);
	$form_state['redirect'] = '';
}

