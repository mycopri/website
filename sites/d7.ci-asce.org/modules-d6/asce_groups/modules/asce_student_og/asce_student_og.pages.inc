<?php
/*
 *  Display a customized node form for students to use to submit their statement to
 *  join the team. We provide the node object of the team node and $op is what type of
 *  application it is - apply as an officer or as a member
 */
function asce_student_og_team_apply($node, $op) {
  global $user;
  module_load_include('inc', 'node', 'node.pages');
  if ($op == 'officer') {
  	$type = 'ci_team_officer';
  }
  if ($op == 'captain') {
    $type = 'ci_team_captain';
  }
  if ($op == 'member') {
    $type = 'ci_team_member';
  }
  // If a node type has been specified, validate its existence.
  $types = node_get_types();
  if (isset($types[$type]) && node_access('create', $type)) {
    // Initialize settings:
    $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');

    drupal_set_title(t('Apply @name', array('@name' => $types[$type]->name)));
    $output = drupal_get_form($type .'_node_form', $node);
  }
  return $output;
}