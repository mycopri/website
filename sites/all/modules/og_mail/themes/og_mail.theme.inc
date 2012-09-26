<?php

/**
 * Creates the HTML to display a digest of updated nodes
 * @param type $list
 *    the list of events
 * @param type $type
 *    the type of events
 */
function theme_og_mail_digest($vars) {
  $type = $vars['type'];
  $list = $vars['list'];
  // Here we will define the header before the list of events.
  switch ($type) {
    case 'new_posts':
      $html = '<div>The following has been recently added:</div>';
      break;
    case 'updated_posts':
      $html = '<div>The following has been recently updated:</div>';
      break;
    case 'comments':
      $html = '<div>In addition:</div>';
      break;
  }
  
  if (is_array($list)) {
    $html .= '<ul>';
    foreach($list as $key => $event_message) {
      $html .= "<li>$event_message</li>";
    }
    $html .= '</ul>';
    return $html;
  }
  else {
    return "<ul><li>$list</li></ul>";
  }
}

/**
 * Implementation of MODULE_preprocess_HOOK()
 * @param type $vars
 */
function og_mail_preprocess_og_mail_updated_post(&$vars) {
  _og_mail_preprocess_post($vars);
}

/**
 * Implementation of MODULE_preprocess_HOOK()
 * @param type $vars
 */
function og_mail_preprocess_og_mail_new_post(&$vars) {
  _og_mail_preprocess_post($vars);
}

/**
 * Implementation of MODULE_preprocess_HOOK()
 * @param type $vars
 */
function og_mail_preprocess_og_mail_comment(&$vars) {
  $comment = $vars['comment'];
  $author = user_load($comment->uid);
  $vars['comment'] = $vars['comment'];
  $vars['comment_author'] = $author->name;
  $vars['comment_link'] = l('commented', 'comment/' . $comment->pid, array('fragment' => 'comment-' . $comment->pid)); 
  $vars['node'] = node_load($comment->nid);
  $time_delta = time() - $comment->changed;
      
  if ($time_delta < 1) {
    $vars['comment_time'] = "just now.";
  }
  else {
    $vars['comment_time'] = format_interval(time() - $comment->changed, 2) . ' ago';
  }
  _og_mail_preprocess_post($vars);
}

/**
 * Helper function.
 */
function _og_mail_preprocess_post(&$vars) {
  $node = $vars['node'];
  // Provide the group for the node
  // It is possible that the node could be in more than one group
  // this function will handle that.
  // Also provide time interval in human readable format.
  $group_count = count($node->group_audience['und']);
  if($group_count <= 1) {
    $group = og_load($node->group_audience['und'][0]['gid']);
    $group_node = node_load($group->etid);
    $vars['group'] = l($group_node->title, 'node/' . $group_node->nid);
  }
  else {
    $vars['group'] = 'multiple groups';
  }
  $vars['post'] = l($node->title, 'node/' . $node->nid);
  $vars['author'] = l($node->name, 'user/' . $node->uid);
  $time_delta = time() - $node->changed;
      
  if ($time_delta < 1) {
    $vars['time'] = "just now.";
  }
  else {
    $vars['time'] = format_interval(time() - $node->changed, 2) . ' ago';
  }
}