<?php
/*
 * Menu callback.
 * Output a list of conferences in the database *  
 */
function cms_list_page() {
	$top = '<p>Some basic text needs to be provided.</p>';
	$conferences = cms_get_conferences();
  $o = '<dl>';
  foreach($conferences as $conference) {
    $o .= '<dt>' . $conference->event . '</dt>';
    $o .= '<dd>' . l('http://ci-asce.org/conferences/' . $conference->url , 'conferences/' . $conference->url) . '</dd>';
    $o .= '<dd>' . l('Administer Abstracts', 'admin/build/cms/abstracts/' . $conference->url) . '</dd>';
    $o .= '<dd>' . l('Conference settings', 'admin/build/cms/settings/' . $conference->url) . '</dd>';
		$links[] = l($conference->event, 'admin/content/cms/' . $conference->url);
  }
  $o .= '</dl>';
	$o = theme('item_list', $links);
  return $top . $o;
}

function cms_conf_list() {
	$conferences = cms_get_conferences();
	foreach($conferences as $conference) {
		$links[] = l($conference->event, 'admin/content/cms/' . $conference->url . '/edit' );
	}
	return theme('item_list', $links);
}

function cms_conf_add_form(&$form_state) {
	$form['event'] = array(
		'#type' => 'textfield',
		'#title' => 'Conference Name',
		'#description' => 'Official name of the conference',
	);
	$form['url'] = array(
		'#type' => 'textfield',
		'#title' => 'URL',
		'#description' => 'A unique url fragment for the conference pages. What you put here ' .
											'will appear in the urls for example if you put 2011_conference, the site ' .
											'url will be ci-asce.org/conferences/2011_conference',
	);
	$form['override_url'] = array(
		'#type' => 'textfield',
		'#title' => 'URL override',
		'#disabled' => TRUE,
		'#description' => 'Not used at this time',
	);
	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Date of event',
		'#description' => 'Enter a start date of the event. ' .
											'This can always be changed later as ' . 
											'you can define specific milestones ' .
											'after the conference has been initialized.',
		'#date_format' => 'm/d/Y',
	);
	$form['publish'] = array(
		'#type' => 'checkbox',
		'#title' => 'Publish',
		'#description' => 'If checked the conference site will go live.',
	);
	$form['buttons'] = array();
	$form['buttons']['complete'] = array(
		'#type' => 'submit',
		'#value' => 'Add',
		'#weight' => 6,
	);
	return $form;
}

function cms_conf_add_form_validate($form, &$form_state) {
}

function cms_conf_add_form_submit($form, &$form_state) {
	global $user;
	$account = $user;
	$record = $form_state['values'];
	$record['created'] = time();
	$record['user'] = $account->uid;	
	if(drupal_write_record('cms_event', $record)) {
		drupal_set_message('Changes saved.');
	}
}

function cms_conf_edit_form(&$form_state, $url) {
	$event = cms_get_conference($url);
	$form['#eid'] = $eid;
	$form['event'] = array(
		'#type' => 'textfield',
		'#title' => 'Title of event',
		'#description' => 'What you put here will appear on web site pages as the title',
		'#default_value' => $event->event,
	);
	$form['url'] = array(
		'#type' => 'textfield',
		'#title' => 'URL',
		'#description' => 'A unique url fragment for the conference pages. What you put here ' .
											'will appear in the urls for example if you put 2011_conference, the site ' .
											'url will be ci-asce.org/conferences/2011_conference',
		'#default_value' => $event->url,
	);
	$form['override_url'] = array(
		'#type' => 'textfield',
		'#title' => 'URL override',
		'#disabled' => TRUE,
		'#description' => 'Not used at this time.',
		'#default_value' => $event->override_url,
	);
	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Date of event',
		'#description' => 'Enter the start date of the event. The important thing is the year as this will make the url for the student days page',
		'#date_format' => 'm/d/Y',
		'#default_value' => $event->date,
	);
	$form['publish'] = array(
		'#type' => 'checkbox',
		'#title' => 'Publish',
		'#description' => 'If checked the student days will be available to logged in users i.e. people can start registering for it.',
		'#default_value' => $event->publish,
	);
	$form['buttons'] = array();
	$form['buttons']['complete'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
		'#weight' => 6,
	);
	return $form;
}

function cms_conf_edit_form_validate($form, &$form_state) {
}

function cms_conf_edit_form_submit($form, &$form_state) {
	global $user;
	$record = $form_state['values'];
	if (!$record['created']) {
		$record['created'] = time();
	}
	if (!$record['user']) {
		$record['user'] = $user->uid; 
	}
	$record['event_id'] = $form['#eid'];
	if(drupal_write_record('cms_event', $record, array('event_id'))) {
		drupal_set_message('Changes saved.');
	}
}

/*
 * Menu callback.
 * Output the main page for the abtract management
 * for a specific conference. 
 */
function cms_manage_abstracts($url) {
	$conference = cms_get_conference($url);
	drupal_set_title('Manage Abstracts for ' . $conference->event);
	$bc = drupal_get_breadcrumb();
	array_pop($bc);
	drupal_set_breadcrumb($bc);
	
	// Display a summary of abstract submissions for the conference
	$summary = cms_get_abstracts_summary($conference->event_id);
	//dsm($summary);
	$o .= '<h3>Total submissions: ' . $summary->total_abstracts . '</h3>';
	$o .= '<h3>Number of submissions by main topic</h3>';
	$o .= '<ul>';
	foreach($summary->topics as $title => $topic) {
	  $link = 'admin/content/cms/' . $conference->url . '/abstracts/topic/' . $topic['id'];
		$o .= '<li>' . l($title, $link) . ": {$topic['count']}</li>"; 
	}
	$o .= '</ul>';
	
	$o .= '<h3>Number of submissions by sub topic</h3>';
  $o .= '<ul>';
  foreach($summary->subtopics as $title => $subtopic) {
    $link = 'admin/content/cms/' . $conference->url . '/abstracts/subtopic/' . $subtopic['id'];
    $o .= '<li>' . l($title, $link) . ": {$subtopic['count']}</li>"; 
  }
  $o .= '</ul>';
  
	return $o; 
}

function cms_abstract_list($op, $id) {
	$conference = cms_get_conference(arg(3));
	$bc = drupal_get_breadcrumb();
	$bc[] = l($conference->event, 'admin/content/cms/' . $conference->url . '/abstracts');
	drupal_set_breadcrumb($bc);
	
	if ($op != 'subtopic' && $op != 'topic') {
		drupal_goto('admin/content/cms/abstracts/' . arg(3));
	}
	
	if ($op == 'subtopic') {
		$sub = cms_get_subtopic($id);
		drupal_set_title('Abstracts (' . $sub->title . ')');
		$abstracts = cms_get_abstracts_by($conference->event_id, $id, 'subtopic');
	}
	elseif ($op == 'topic') {
		$topic = cms_get_topic($id);
		drupal_set_title('Abstracts (' . $topic->title . ')');
		$abstracts = cms_get_abstracts_by($conference->event_id, $id, 'topic');
	}
	$o = '<ul>';
	//dsm($abstracts);
	dpm(drupal_get_destination());
	foreach($abstracts as $abstract) {
	  $o .= '<li>' . l($abstract->title, 'node/' . $abstract->nid, array('query' => drupal_get_destination())) . '</li>';
	}
	$o .= '</ul>';
	return $o;	
}

function cms_abstract_submit_text_form(&$form_state, $event_url) {
	// Get the event id
	$conference = cms_get_conference($event_url);
	
	$text = variable_get($conference->event_id . '-abstract-page-text', 'Need to provide some text.');
	$form['body'] = array(
    '#type' => 'textarea',
    '#title' => 'Text to go on the abstract submission page',
    '#default_value' => $text,
    '#rows' => 20,
    '#required' => TRUE,
  );

  $form['format'] = filter_form($node->format);
  $form['event'] = array(
    '#type' => 'value',
    '#value' => $conference->event_id,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
	return $form;
}

function cms_abstract_submit_text_form_submit($form, &$form_state) {
  variable_set($form_state['values']['event'] . '-abstract-page-text', $form_state['values']['body']);
}
/**
 * Menu callback. Display the main Topics settings page
 */ 
function cms_topic() {
  // List the topics
  $topics = cms_get_topics();
  if (!$topics) {
  	$content = 'You have not defined any Topics yet.';
  }
  else {
  	foreach($topics as $topic_id=>$topic) {
  		$content .= '<p><ul>';
  		$content .= '<dt>';
  		$content .= $topic->title . '<a href="subtopic/add/' . $topic_id . '">  Add subtopic to ' . $topic->title . '</a>';
  		$content .= '</dt>';
  		foreach($topic->subtopics as $subtopic_id=>$subtopic) {
  			$content .= '<dd>' . $subtopic . '</dd>';
  		}
  		$content .= '</ul></p>';
  	}
  }
  $content .= drupal_get_form('cms_topic_buttons');
  
  return $content;
}

/*
 * Menu callback. Display the Add Topic form page.
 */
function cms_topic_add() {
	return drupal_get_form('cms_topic_add_form');	
}

/*
 * Menu callback. Display the Add sub-topic form page.
 */
function cms_subtopic_add($topic_id = NULL) {
	$breadcrumb = drupal_get_breadcrumb();
	$breadcrumb[] = l('Administer', 'admin');
	$breadcrumb[] = l('Paper Management System', 'admin/pms');
	$breadcrumb[] = l('Topics', 'admin/pms/topic');
	drupal_set_breadcrumb($breadcrumb);
	
	if ($topic_id) {
    $topic = cms_get_topic($topic_id);
    drupal_set_title('Add a Sub-topic to ' . $topic->title);
	}
	return drupal_get_form('cms_subtopic_add_form', $topic);
}

/*
 * Generate the Add Topic form array.
 */
function cms_topic_add_form(&$form_state) {
  $form['topic_add'] = array(
    '#type' => 'textfield',
    '#title' => 'Topic',
    '#description' => 'Provide a topic.',
    '#required' => TRUE,
    '#default_value' => NULL,
    '#maxlength' => 255,
    '#weight' => -5
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add',
  );
  return $form;
}

function cms_topic_add_form_submit($form, &$form_state) {
	$record->title = $form_state['values']['topic_add'];
	if(drupal_write_record('cms_topic', $record)) {
		drupal_set_message('Topic successfully added.', 'status');
	}
	drupal_goto('admin/pms/topic');
}

/*
 * Generate the Add Sub-topic form array.
 */
function cms_subtopic_add_form(&$form_state, $topic = FALSE) {
  $topics = cms_get_topics();
  if (!$topic) {
    foreach($topics as $topic_id=>$topic) {
      $options[$topic_id] = $topic->title;
    }
  }
  
	$form['topic'] = array(
    '#type' => 'select',
    '#title' => 'Topic',
    '#description' => 'You must select a topic for this sub-topic to be in.',
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => NULL,
    '#maxlength' => 255,
    '#weight' => -5
  );
  if ($topic) {
  	$form['topic']['#default_value'] = $topic->topic_id;
  	$form['topic']['#access'] = FALSE;
  }
	
	$form['subtopic'] = array(
    '#type' => 'textfield',
    '#title' => 'Sub-Topic',
    '#description' => 'Enter the name of the new sub-topic.',
    '#required' => TRUE,
    '#default_value' => NULL,
    '#maxlength' => 255,
    '#weight' => -5
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add',
  );
  return $form;
}

function cms_subtopic_add_form_submit($form, &$form_state) {
	$record->title = $form_state['values']['subtopic'];
	$record->topic_id = $form_state['values']['topic'];
	if(drupal_write_record('cms_subtopic', $record)) {
	  drupal_set_message($record->title . ' was added.', 'status');	
	}
	else {
		drupal_set_message('Subtopic was not added.', 'error');
	}
  
}
/*
 * Generate the buttons for the Topics settings Main page
 */
function cms_topic_buttons(&$form_state) {
	$form['add_topic'] = array(
    '#type' => 'submit',
    '#value' => 'Add Topic',
  );
  $form['add_subtopic'] = array(
    '#type' => 'submit',
    '#value' => 'Add Sub-topic',
  );
  return $form;
}

function cms_topic_buttons_submit($form, &$form_state) {
	if ($form_state['values']['op'] == 'Add Topic') {
	  drupal_goto('admin/pms/topic/add');
	}
	else {
		drupal_goto('admin/pms/subtopic/add');
	}
}

/**
 * Form for editing an entire topics tree for a conference at once.
 *
 * Shows for one topic the topic items accessible to the current user and
 * relevant operations.
 */
function cms_topics_form(&$form_state, $url) {
	$conference = cms_get_conference($url);
	drupal_set_title($conference->event);
	return '';
  global $menu_admin;
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = '%s'
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, $menu['menu_name']);
	while($row = db_fetch_object($result)) {
		$rows[] = $row;
	}
	//dpm($rows);
  $result = db_query($sql, $menu['menu_name']);
  $tree = menu_tree_data($result);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;

  $form = _menu_overview_tree_form($tree);
  $form['#menu'] =  $menu;
  if (element_children($form)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['empty_menu'] = array('#value' => t('There are no menu items yet.'));
  }
  return $form;
}

/**
 * Menu callback; Build the topic editing form.
 */
function cms_edit_topic(&$form_state, $type, $topic, $url) {
	module_load_include('inc', 'cms', 'cms.topics');
	$conference = cms_get_conference($url);
	drupal_set_title($conference->event);
	$eid = $conference->event_id;
	
  $form['topic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Topic'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#weight' => -2,
    '#attributes' => array('class' => 'topic-item-form'),
    '#item' => $topic,
  );
  if ($type == 'add' || empty($topic)) {
    // This is an add form, initialize the menu link.
    $topic = array(
    	'eid' => $eid,
    	'topic_name' => $topic['topic_name'],
    	'plid' => 0,
    	'weight' => 0, 
    	'module' => 'cms', 
    	'has_children' => 0);
  }
	$set_form_values = array(
		'eid', 
		'topic_name', 
		'module', 
		'has_children', 
		);
  foreach ($set_form_values as $key) {
    $form['topic'][$key] = array('#type' => 'value', '#value' => $topic[$key]);
  }
  
  $form['topic']['topic_name'] = array(
  	'#type' => 'textfield',
    '#title' => t('Topic'),
    '#default_value' => $topic['topic_name'],
    '#description' => t('The text corresponding to this topic that should appear in abstracts.'),
    '#required' => TRUE,
  );

  // Generate a list of possible parents (not including this item or descendants).
  $options = cms_topic_parent_options($eid, $topic);
	array_unshift($options, 'New Main Topic');
	$options = $options ? $options : array();
	
  $form['topic']['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent item'),
//    '#default_value' => $default,
    '#options' => $options,
    '#description' => t('The maximum depth for an item and all its children is fixed at !maxdepth. Some menu items may not be available as parents if selecting them would exceed this limit.', array('!maxdepth' => MENU_MAX_DEPTH)),
    '#attributes' => array('class' => 'menu-title-select'),
  );
  $form['topic']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 50,
    '#default_value' => $topic['weight'],
    '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Process menu and menu item add/edit form submissions.
 */
function cms_edit_topic_submit($form, &$form_state) {
  $topic = &$form_state['values']['topic'];
	
  $topic['options']['attributes']['title'] = $topic['description'];
  list($topic['eid'], $topic['plid']) = array($topic['eid'], $topic['parent']);
  if (!cms_topic_save($topic)) {
  	drupal_set_message(t('There was an error saving the menu link.'), 'error');
  }
  $form_state['redirect'] = 'admin/build/cms/topics/2012_summit/add';
}

function cms_debug() {
	module_load_include('inc', 'cms', 'cms.topics');
	
	$eid = 1;
  $topic = array(
  	'eid' => $eid,
  	'topic_name' => '',
  	'plid' => 0,
  	'weight' => 0, 
  	'module' => 'cms', 
  	'has_children' => 0
	);
	$o = 'Trying to figure out how the cms_topic_parent_options() works.';
	// Generate a list of possible parents (not including this item or descendants).
  $options = cms_topic_parent_options($eid, $topic);
		
	return $o;
}

function cms_manage_topics($url) {
	$conference = cms_get_conference($url);
	drupal_set_title('Manage topics for ' . $conference->event);
	$bc = drupal_get_breadcrumb();
	array_pop($bc);
	drupal_set_breadcrumb($bc);

	return '';
}

function cms_manage_users($url) {
	$conference = cms_get_conference($url);
	drupal_set_title('User Administration for ' . $conference->event);
	$bc = drupal_get_breadcrumb();
	array_pop($bc);
	drupal_set_breadcrumb($bc);

	$authors = _cms_get_users($conference->event_id);
	$o = '<fieldset>';
	
	$o .= '<h2>Users who will have access to the abstracts</h2>';
	$o .= '<p>The users listed below are being tracked by the system as having access to ' .
				'some of the data for this conference i.e. abstract review. You may not be listed because ' .
				'you are an administrator.</p><p>The users are presented in a manner that will allow you to '. 
				'quickly copy it and paste it into outlook for sending quick emails.';
	$sql = "SELECT * FROM cms_uid";
	$results = db_query($sql);
	while($user = db_fetch_object($results)) {
		$account = user_load($user->uid);
		$emails[] = $account->mail;
	}
	$o .= '<p><b>' . implode('; ', $emails) . '</b></p>';
	
	$o .= '<h2>Quickly add and register users to administer this conference</h2>';
	$o .= '<p>If you have a list of emails and you want to give them access to the abstracts ' .
				'this is the form you want to use. Enter the email addresses into the field box below separating them ' .
				'with commas. Once you press "Add Users" the system will automatically create new accounts and alert you ' .
				'of the users that already exist, at which point you can add the existing users in the box below.</p>';
	$o .= drupal_get_form('cms_add_user');
	$o .= '<h2>Add existing users to administer this conference</h2>';
	$o .= '<p>Use the field box below to add existing users. This box has a feature that will search for the user ' .
				'as you type in the username. It will only add existing users. If a user does not have an account you have to add ' .
				'them using the field box above.</p>';	
	$o .= drupal_get_form('cms_user_perm');
	$o .= '</fieldset>';
	$o .= '<fieldset>';
	$o .= '<p>Below is a list of users who have submitted papers</p>';
	$o .= drupal_get_form('cms_manage_users_form', $conference->event_id);
	$o .= '</fieldset>';
	return $o;	
}

function cms_manage_users_form(&$form_state, $event_id) {
	$header = array(
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Abstract Title'), 'field' => 'nr.title'),
    array('data' => t('Submitted on'), 'field' => 'nr.timestamp', 'sort' => 'desc'),
    t('Operations')
  );
  
  $sql = "SELECT nr.*, u.* FROM node n JOIN node_revisions nr ON n.vid=nr.vid " .
  				"JOIN users u ON u.uid=n.uid " .
  				"JOIN cms_abstract cms ON cms.abstract_nid=n.nid WHERE cms.event_id = %d";
	$query_count = "SELECT COUNT(nr.uid) FROM node n JOIN node_revisions nr ON n.vid=nr.vid " .
  				"JOIN users u ON u.uid=n.uid " .
  				"JOIN cms_abstract cms ON cms.abstract_nid=n.nid WHERE cms.event_id = %d";
	$sql .= tablesort_sql($header);

  $result = pager_query($sql, 50, 0, $query_count, $event_id);
	
	$destination = drupal_get_destination();

  $status = array(t('blocked'), t('active'));
  $roles = user_roles(TRUE);
  $accounts = array();
  while ($account = db_fetch_object($result)) {
  	$accounts[] = $account;
    $form['name'][$account->uid] = array('#value' => theme('username', $account));
    $form['abstract_title'][$account->uid] = array('#value' => l($account->title, "node/$account->nid", array('query' => $destination)));
    $form['submitted'][$account->uid] =  array('#value' => $account->timestamp ? t('@time ago', array('@time' => format_interval(time() - $account->timestamp))) : t('never'));
    $form['operations'][$account->uid] = array('#value' => l(t('edit'), "user/$account->uid/edit", array('query' => $destination)));
  }
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
	return $form;
}

/**
 * Masquerade block form.
 */
function cms_user_perm() {
  if (cms_menu_access('administer cms')) {
    $markup_value .= t('Enter the username to add to group.');
    $form['cms_user_field'] = array(
      '#prefix' => '<div class="container-inline">',
      '#type' => 'textfield',
      '#width' => 200,
      '#default_value' => '',
      '#autocomplete_path' => 'cms/autocomplete',
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Go'),
      '#suffix' => '</div>',
    );
		$form['conference'] = array(
			'#type' => 'value',
			'#value' => cms_get_conference(arg(3)),
		);
  }
  return $form;
}

/**
 * Masquerade block form validation.
 */
function cms_user_perm_validate($form, &$form_state) {
	$users = explode(',', $form_state['values']['cms_user_field']);
	dpm($users);
	foreach($users as $key => $user) {
		$users[$key] = "'" . trim($user) . "'";
	}
	$user_field = implode(',', $users);
	$sql = "SELECT uid, mail FROM users WHERE name IN ($user_field)";
	$results = db_query($sql);
	$users = array();
	while($user = db_fetch_object($results)) {
		$users[$user->uid] = $user->name;
	}
	$form_state['values']['users'] = $users;
}

/**
 * cms user permissions management
 */
function cms_user_perm_submit($form, &$form_state) {
	dpm($form_state);
	$values = $form_state['values'];
	foreach($values['users'] as $uid => $name) {
		cms_save_subscription($values['conference']->event_id, $uid);
	}
}

/**
 * Quick add and register user form.
 */
function cms_add_user() {
  if (cms_menu_access('administer cms')) {
    $markup_value .= t('Enter the username to add to group.');
    $form['cms_user_field'] = array(
      '#prefix' => '<div class="container-inline">',
      '#type' => 'textfield',
      '#width' => 200,
      '#default_value' => '',
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add users'),
      '#suffix' => '</div>',
    );
		$form['conference'] = array(
			'#type' => 'value',
			'#value' => cms_get_conference(arg(3)),
		);
  }
  return $form;
}

/**
 * Quick add and register user form validation.
 */
function cms_add_user_validate($form, &$form_state) {
	$users = explode(',', $form_state['values']['cms_user_field']);
	$i = 0;
	foreach($users as $key => $user) {
		$sql = 'SELECT count(uid) as count_uid, uid FROM users WHERE LOWER(name) = \'' . strtolower($user) . '\' OR mail = \'' . strtolower($user) .'\'';
		$row = db_fetch_object(db_query($sql));
		if ($row->count_uid > 0) {
			$exist[] = $row->uid;
		}
		else {
			$new[] = $user;
		}
	}
	$form_state['values']['users']['exist'] = $exist;
	$form_state['values']['users']['new'] = $new;
}

/**
 * Quick add and register user form submit
 */
function cms_add_user_submit($form, &$form_state) {
	$users = $form_state['values']['users'];
	// Create new accounts
	if($users['new']){
		foreach($users['new'] as $user) {
			$account['name'] = $user;
			$account['mail'] = $user;
			$account['pass'] = 'texas2012';
			$account['status'] = 1;
			$new_account = user_save(NULL, $account);
			cms_save_subscription($form_state['values']['conference']->event_id, $new_account->uid);
			$mess = 'Created a new account for ' . $user . '.<br/>';
			drupal_set_message($mess);
		};
	}
	if($users['exist']){
		foreach($users['exist'] as $user) {
			$mess = $user . ' already exists.<br/>';
			drupal_set_message($mess);
		}
	}
}

function cms_manage_email($url) {
	$conference = cms_get_conference($url);
	drupal_set_title('E-mail Administration for ' . $conference->event);
	$bc = drupal_get_breadcrumb();
	array_pop($bc);
	drupal_set_breadcrumb($bc);	
	return '';	
}

/**
 * Helper function for the user administration
 */
function _cms_get_users($event_id) {
	// We just want to get the node user so to be more efficient, we just
	// use sql as opposed to loading a whole node
	$sql_node = "(SELECT n.uid, n.nid FROM node n JOIN node_revisions nr ON n.vid = nr.vid) nl";
	$sql = "SELECT nl.uid, cms.* FROM cms_abstract cms JOIN $sql_node on nl.nid = cms.abstract_nid WHERE event_id = %d";
	//dpm($sql);
	$results = db_query($sql, $event_id);
	while($row = db_fetch_object($results)) {
		$authors[$row->uid] = user_load($row->uid);
	}
	return $authors;
}

/**
 * Theme user administration overview.
 *
 * @ingroup themeable
 */
function theme_cms_manage_users_form($form) {
  // Overview table:
  $header = array(
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Abstract Title'), 'field' => 'nr.title'),
    array('data' => t('Submitted on'), 'field' => 'nr.timestamp', 'sort' => 'desc'),
    t('Operations')
  );

  $output = drupal_render($form['options']);
  if (isset($form['name']) && is_array($form['name'])) {
    foreach (element_children($form['name']) as $key) {
      $rows[] = array(
        drupal_render($form['name'][$key]),
        drupal_render($form['abstract_title'][$key]),
        drupal_render($form['submitted'][$key]),
        drupal_render($form['operations'][$key]),
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No users available.'), 'colspan' => '7'));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }
  $output .= drupal_render($form);

  return $output;
}

function cms_conf_corpus() {
	global $user;
	$o = '<h3>Please submit your abstracts directly to <a href="mailto:bob@pierryconsulting.com">Bob Pierry</a> ' .
					'at <a href="mailto:bob@pierryconsulting.com">bob@pierryconsulting.com</a></h3>';
	$node = node_load(8671);
	if (user_access('administer nodes', $user)) {
		$o .= '<p><a href="../node/8671/edit">Edit</a></p>';
	}	 
	$o .= node_show($node, NULL);
	
	return $o;
}
