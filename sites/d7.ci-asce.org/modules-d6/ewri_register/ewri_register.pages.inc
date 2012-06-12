<?php
// $Id$

/**
 * @file
 * Page callbacks
 */

function ewri_register_home() {
	$output = l('Submit session proposal', 'ewri2012/submit-session-proposal');
	return $output;
}

function ewri_register_admin() {
  return 'Home Page';
}

function ewri_register_submit_proposal() {
  $breadcrumb = drupal_get_breadcrumb();
  dsm($breadcrumb);
  $breadcrumb[] = l('EWRI 2012', 'ewri2012');
  drupal_set_breadcrumb($breadcrumb);
	module_load_include('inc', 'node', 'node.pages');
  $output = node_add('congress_session_proposals');
  drupal_set_title('Submit a session proposal for the 2012 World Environmental and Water Resources Congress');
  return $output;	
}
