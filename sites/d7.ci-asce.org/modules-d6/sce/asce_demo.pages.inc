<?php

/**
 * Menu callback -- ask for confirmation of menu table cleaning
 */
function asce_demo_clean_menu_tables_confirm(&$form_state, $node) {
  $title = 'Clean the menu_links table.';
	$sql = "SELECT * FROM menu_links WHERE router_path = '%s'";
  $results = db_query($sql, 'node/add');
  $temp = node_get_types();
  foreach ($temp as $key => $value) {
  	$types[] = $key;
  }
  while($link = db_fetch_object($results)) {
  	$router_path = explode('/', $link->link_path);
  	$type = str_replace('_', '-', $router_path[2]);
  	if (in_array($type, $types)) {
  		//$check[$type] = 'exists';
  	}
  	else {
  		if (!($link->link_path == $link->router_path)) {
  		  $check[$type] = $link;
  		}
  	}
  }
  $message = '<p>The following will be deleted from the menu_links table:<p>';
  $message .= '<ul>';
  foreach($check as $type => $link) {
  	$message .= '<li>';
  	$message .= '<b>' . $link->link_title . '</b> will be removed from the menu_links table. mlid = ' . $link->mlid . ', link_path = <b>' . $link->link_path  . '</b>, router_path = ' . $link->router_path; 
  	$message .= '</li>';
  }
  $message .= '</ul>';
  //dsm($types);
  //dsm($check);
  $form_state['#menu_links'] = $check;
  return confirm_form($form,
    $title,
    isset($_GET['destination']) ? $_GET['destination'] : '',
    t($message),
    t('Continue'),
    t('Cancel')
  );
}

/**
 * Execute menu table cleaning
 */
function asce_demo_clean_menu_tables_confirm_submit($form, &$form_state) {
	if ($form_state['values']['confirm']) {
		$message = '<p>The following has been deleted from the menu_links table:<p>';
		$message .= '<ul>';
		foreach($form_state['#menu_links'] as $type => $link) {
			db_query('DELETE FROM menu_links WHERE mlid = %d', $link->mlid);
			$message .= '<li>';
			$message .= '<b>' . $link->link_title . '</b> will be removed from the menu_links table. mlid = ' . $link->mlid . ', link_path = <b>' . $link->link_path  . '</b>, router_path = ' . $link->router_path;
			$message .= '</li>';
		}
		$message .= '</ul>';
	}
	drupal_set_message($message);
  $form_state['redirect'] = '<front>';
}
