<?php

function asce_summit_main() {
	global $user;
	
	drupal_set_title(t('1st Annual CI Construction Summit at Washington University, October 12-15, 2011'));
	$node = node_load(7382);
	
	if (user_access('administer nodes')) {
		$content = l('Edit the text', 'node/7382/edit', array('query' => drupal_get_destination()));
	}
	
	$content .= check_markup($node->body, 2);  
	
	if (user_is_logged_in()) {
		// Do nothing
	}
	else {
		$content .= '<p>You are not logged in, if you do not have a login please register here.</p>';
	}	  	  
	
	return $content;
	
}

function asce_summit_administer() {
	drupal_set_title(t('1st Annual CI Construction Summit at Washington University, October 12-15, 2011'));
	return 'Administration links will appear here';
}

function asce_summit_propose_session () {
	global $user;	
	module_load_include('inc', 'node', 'node.pages');
  $types = node_get_types();
  $type = 'ci_summit_session';
  // If a node type has been specified, validate its existence.
  if (isset($types[$type]) && node_access('create', $type)) {
    // Initialize settings:
    $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');

    drupal_set_title(t('Submit a session'));
    $output = drupal_get_form($type .'_node_form', $node);
  }
  else {
  	$output = 'In order to submit a proposed session, you need to register or login.';
  	$output .= drupal_get_form('asce_summit_login');
  }
  return $output;
}

function asce_summit_login(&$form_state) {
	// Took straight from user module
	// Display login form:
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Username'),
    '#size' => 60,
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#required' => TRUE,
  );

  $form['name']['#description'] = t('Enter your @s username.', array('@s' => variable_get('site_name', 'Drupal')));
  $form['pass'] = array('#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Enter the password that accompanies your username.'),
    '#required' => TRUE,
  );
  $form['#validate'] = user_login_default_validators();
  $form['submit'] = array('#type' => 'submit', '#value' => t('Log in'), '#weight' => 2);
  
  if (module_exists('logintoboggan')){
  	// Grab the message from settings for display at the top of the login block.
  	if ($login_msg = variable_get('logintoboggan_login_block_message', '')) {
  		$form['message'] = array(
          '#value' => filter_xss_admin($login_msg),
          '#weight' => -50,
  		);
  	}
  	if (variable_get('logintoboggan_login_with_email', 0)) {
  		$form_id = 'user_login';
  		// Ensure a valid validate array.
  		$form['#validate'] = is_array($form['#validate']) ? $form['#validate'] : array();
  		// LT's validation function must run first.
  		array_unshift($form['#validate'],'logintoboggan_user_login_validate');
  		// Use theme functions to print the username field's textual labels.
  		$form['name']['#title']       = theme('lt_username_title', $form_id);
  		$form['name']['#description'] = theme('lt_username_description', $form_id);
  		// Use theme functions to print the password field's textual labels.
  		$form['pass']['#title']       = theme('lt_password_title', $form_id);
  		$form['pass']['#description'] = theme('lt_password_description', $form_id);
  	}
  	if(isset($GLOBALS['logintoboggan_denied']) && $GLOBALS['logintoboggan_denied'] == TRUE) {
  		logintoboggan_destination();
  	}
  }
  
  $form['register'] = array(
    '#prefix' => '<p>',
    '#value' => 'or ' . l('register', 'summit/reg', array('query' => drupal_get_destination())),
    '#weight' => 10,
    '#suffix' => '</p>',
  );
  $form_state['storage']['redirect'] = $_GET['q'];
  return $form;
}

/**
 * Submit handler for the login form. Redirects the user to a page.
 *
 * The user is redirected to the Summit page. Setting the destination in
 * the query string (as done by the user login block) overrides the redirect.
 */
function asce_summit_login_submit($form, &$form_state) {
  global $user;
  if ($user->uid) {
  	$redirect = isset($form_state['storage']['redirect']) ? $form_state['storage']['redirect'] : 'summit';
    $form_state['redirect'] = $redirect;
    drupal_goto($redirect);
    return;
  }
}

function asce_summit_node_form_submit($form, &$form_state) {
  global $user;
  
  $node = node_submit($form_state['values']);
  node_save($node);
  $node_link = l(t('view'), 'node/'. $node->nid);
  $watchdog_args = array('@type' => $node->type, '%title' => $node->title);
  $t_args = array('@type' => node_get_types('name', $node), '%title' => $node->title);

  watchdog('content', '@type: added %title.', $watchdog_args, WATCHDOG_NOTICE, $node_link);
  drupal_set_message(t('@type %title has been submitted.', $t_args));
  
  $send = TRUE;
  $params['node'] = $node;
  drupal_mail('asce_summit', 'session-submitted', $user->mail, $language, $params, 'ci@ci-asce.org', $send);
  drupal_mail('asce_summit', 'session-submitted-notify', 'esantiago@asce.org', $language, $params, 'ci@ci-asce.org', $send);
  drupal_mail('asce_summit', 'session-submitted-notify', 'moey@asce.org', $language, $params, 'ci@ci-asce.org', $send);
  
  if ($node->nid) {
    unset($form_state['rebuild']);
    $form_state['nid'] = $node->nid;
    $form_state['redirect'] = 'summit'; //'node/'. $node->nid;
    drupal_goto('summit');
  }
  else {
    // In the unlikely case something went wrong on save, the node will be
    // rebuilt and node form redisplayed the same way as in preview.
    drupal_set_message(t('The post could not be saved.'), 'error');
  }
}