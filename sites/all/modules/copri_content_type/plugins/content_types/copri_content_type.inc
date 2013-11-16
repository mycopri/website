<?php
/**
* Plugins are described by creating a $plugin array which will be used
* by the system that includes this file.
*/
$plugin = array(
  'single' => TRUE,
  'title' => t('Advanced Poll'),
  'content_types' => 'advpoll',
  'icon' => 'icon_node.png',
  'description' => t('The poll associated with the node.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'render callback' => 'advpoll_render',
  'edit form' => 'advpoll_edit_form',
  'category' => t('Node'),
  'defaults' => array(
    'link' => TRUE,
  ),
);
/**
* Render the custom content type.
* Code stolen and adapted from hook_node_view of advpoll.module
*/
function advpoll_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }
  // Get a shortcut to the node.
  $node = $context->data;
  // Load information about the node type.
  $type = node_type_get_type($node);
  // Build the content type block.
  $block = new stdClass();
  $block->title   = $type->title_label;
  $block->delta   = $node->nid;
  $data = advpoll_get_data($node);
  if ($data->behavior == 'approval' || $data->behavior == 'pool')  {
    drupal_add_css(drupal_get_path('module', 'advpoll') . '/css/advpoll.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
// check for eligibility to vote
    if (advpoll_user_eligibility($node)) {
// print out voting form
      $voteform = drupal_get_form('advpoll_choice_form', $node);
      $block->content['advpoll_choice'] = array(0 => $voteform, '#weight' => 1);
    } else {
// get user's votes if they're logged in and if voting is normal
      $votes = array();
      if ($data->mode == 'normal') {
        $votes = advpoll_get_user_votes($node->nid);
      }
      $results = advpoll_display_results($node->nid, $data);
      $block->content['advpoll_choice'] = array('#markup' => $results, '#weight' => 1);
    }
  }
  return $block;
}
/**
* Returns an edit form for custom type settings.
*/
function advpoll_edit_form($form, &$form_state) {
  return $form;
}
/**
* Submit handler for the custom type settings form.
*/
function advpoll_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
/**
* Returns the administrative title for a type.
*/
function advpoll_admin_title($subtype, $conf, $context) {
  return t('"@s" title', array('@s' => $context->identifier));
}
?>