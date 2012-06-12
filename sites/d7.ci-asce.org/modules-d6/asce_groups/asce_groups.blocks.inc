<?php
/*
 *  Helper function to display CI Groups content block
 */
function asce_groups_block_content() {
  global $user;
  
  if (arg(1) != 7063) {
  	$links[] = l('Add a post', 'node/add/ci-content',
  	array('query' => array('gids[]' => arg(1), 'ci-group-front' => TRUE, 'destination' => $_GET['q'])));

  	$block['subject'] = t('Content');
  	$block['content'] = theme('item_list', $links);
  }
  else {
    $links[] = l('Add a post', 'node/add/student-challenge-document',
    array('query' => array('gids[]' => arg(1), 'destination' => $_GET['q'])));

    $block['subject'] = t('Content');
    $block['content'] = theme('item_list', $links);
  }
  return $block;
}

function asce_groups_block_administration() {
	global $user;
  
  $links[] = l('Edit this page', 'admin/build/pages/nojs/operation/node_view/handlers/node_view_panel_context_14/content', 
    array('query' => array('destination' => $_GET['q'])));
  $links[] = l('Member Admin', 'og/users/' . arg(1));
  
  $block['subject'] = t('Administration');  
  $block['content'] = theme('item_list', $links);
    
  return $block;
}