<?php

function ewri_register_block_home() {
	global $user;

  if (user_access('create paes_project') || user_access('administer nodes')) {
    $links[] = array (
      'link' => l('Go to the PAES page', 'paes'),
      'text' => 'Takes you to a page that allows you to view and add PAES projects. Note: only certain users have permission to access this.',
    );
  };
  
  if (user_access('create congress_session_proposals') || user_access('administer nodes')) {
    $links[] = array (
      'link' => l('Submit a 2012 Congress Session', 'node/add/congress-session-proposals', array('query' => drupal_get_destination())),
      'text' => 'Submit a Session Proposal for the 2012 EWRI Congress.',
    );
  }
  
  if (user_access('view proposalsummaryreport')) {
    $links[] = array (
      'link' => l('View Submitted 2012 Session Proposals', 'proposalsummaryreport', array('query' => drupal_get_destination())),
      'text' => 'View the submitted 2012 EWRI Session Proposals. Note: This was previously open to the public, now you must have registered on the site to view.',
    );
    $links[] = array( 
      'link' => l('Export Submitted 2012 Session Proposals', 'proposalsummaryreport/export', array('query' => drupal_get_destination())),
      'text' => 'Download the an excel format of all submitted proposals to your computer.',
    );
  }
  
  if ($links) {
  	$content = '<dl>';
  	foreach ($links as $link) {
      $content .= '<dt>' . $link['link'] . '</dt>';
      $content .= '<dd>' . $link['text'] . '</dd>';
  	}
  	$content .= '</dl>';
  	$block['content'] = $content; //theme('item_list', $links);
    $block['subject'] = t('Welcome');
  }
  return $block;
}