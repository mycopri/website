<?php

/**
 *  Menu callback: Home page for the Paper Management System
 */
function asce_conferences_home() {
	drupal_set_title('Paper and Abstract Submissions');

	// List all the conferences
	$conferences = cms_get_conferences();
	$content .= '<p>Welcome to the paper/abstract submission portal page. From here you will be able to go the specific conference you wish to submit ' .
							'a paper/abstract to by clicking on one of the links below.<p>';
	$content .= '<p>For questions/inquiries regarding the use of this site contact Katerina Lachinova at klachinova@asce.org</p>';
	if (module_exists('filter')) { 
		$content = _filter_url($content, -1);
	}
	$content .= '<h3>Upcoming Conferences</h3>';
	$content .= '<dl>';
	foreach($conferences as $conference) {
		$content .= '<dd>';
		$content .= l($conference->event, 'conferences/' . $conference->url);
		$content .= '</dd>';
	}
	$content .= '</dl>';

	return $content;
}
/**
 * Menu callback: conferences/%
 * The home page of a specific conference
 *
 * @param unknown_type $url
 */
function asce_conference($url) {
	global $user;
	
	$account = $user;
	// Check to make sure whatever argument is valid
	$conferences = cms_get_conferences();
	foreach($conferences as $conference) {
		$events[] = $conference->url;
	}
	if (!in_array($url, $events)) {
		drupal_goto('conferences');
	}
	
	$conference = cms_get_conference($url);
	drupal_set_title($conference->event . ' Call for Proposals');
	$content .= drupal_get_form('_new_abstract_button', $event);
	
	$submissions = cms_get_submissions($account->uid);
	$content .= theme('submissions', $submissions);
	$content .= '<p>' . variable_get($conference->event_id . '-abstract-page-text', 'No text provided yet. Please go to the conference settings and provide there.') . '</p>';
	$content .= drupal_get_form('_new_abstract_button', $event);
	return $content;
}

/**
 * Generate a form button array for use on a specific conference
 * home page
 *
 * @param $form_state
 */
function _new_abstract_button (&$form_state) {
	$event = arg(1);
	$form['event'] = array(
	 '#type' => 'value',
	 '#value' => $event,
	);
	$form['new_abstract_button'] = array (
	  '#type' => 'submit',
	  '#value' => 'Submit an Abstract',
	);
	return $form;
}

function _new_abstract_button_submit ($form, &$form_state) {
	$conference = cms_get_conference($form_state['values']['event']);
	drupal_goto('conferences/' . $conference->event_id . '/abstract/submit');
}

/**
 * Menu callback; view a single node.
 */
function cms_conf_home_page_view($node, $cid = NULL) {
	global $user;
	if(user_access('administer cms', $user)) {
		$o .= l('Edit', 'node/' . $node->nid . '/edit', array('query' => drupal_get_destination()));
	}
	drupal_set_title(check_plain($node->title));	
  $o .= cms_node_show($node, $cid);
	return $o;
}

/**
 * Menu callback: conferences/%/abstract/add
 * Present the abstract submission page
 * @param unknown_type $event_id
 */
function cms_abstract($event_id = NULL) {
	if (user_is_logged_in()) {
		drupal_goto('node/add/abstract', 'event_id=' . $event_id);
	}
	else {
		$content = drupal_get_form('user_login_block');
		return $content;
	}
}

/**
 * Menu callback: conferences/%/abstract/%/topic
 * This presents the topics selection page
 *
 * @param unknown_type $abstract_id
 */
function cms_assign_topic($conference_id, $abstract_nid) {
	if (!$abstract_nid) {
		drupal_goto('conferences');
	}
	
	$node = node_load($abstract_nid);
	$conference = cms_get_conference($conference_id);
	$info = array(
	 'node' => $node,
	 'op' => $op,
	 'conference' => $conference,
	);
	
	// Setup the title and breadcrumb navigation
	drupal_set_title($node->title . ' Topic');
	$bc = drupal_get_breadcrumb();
  $bc[] = l($conference->event, 'conferences/' . $conference->url);
  $bc[] = l($node->title, 'node/' . $node->nid);
  drupal_set_breadcrumb($bc);
	
  $o .= '<p>Suggested topics are provided below; however, if you wish to add a topic please click ';
  $o .= l('here', 'conferences/' . arg(1) . '/abstract/' . arg(3) . '/add-new-topic');
  $o .= ' to add a topic.</p>';
	$o .= drupal_get_form('cms_assign_topic_form', $info);
	$o .= '<div style="clear:both"></div>';
	return $o;
}

/**
 * Generate the array for the topic selection form
 */
function cms_assign_topic_form(&$form_state, $info) {	
	$node = $info['node'];
	// Add the topics selection elements
	$topics = cms_get_topics();
	foreach($topics as $topic_id => $topic) {
		$total = count($topic->subtopics);
		$i = 0;
		foreach($topic->subtopics as $id => $subtopic) {
			$form[$topic_id][$id]['subtopic'] = array(
			  '#type' => 'radio',
			  '#title' => $subtopic,
			  '#return_value' => $id,
			  '#tree' => FALSE,
			);
			if ($node->subtopic == $id ) {
        $form[$topic_id][$id]['subtopic']['#default_value'] = $id;
			}
        
			if ($i == 0) {
			  $form[$topic_id][$id]['subtopic']['#prefix'] = 
          '<fieldset style="width:45%;margin-right:10px;float:left;"><legend>' . $topic->title . '</legend>';
			}
			if ($i == $total - 1) {
				$form[$topic_id][$id]['subtopic']['#suffix'] = '</fieldset>';
			}
			$i = $i + 1;
		}
	}
	$form['submit'] = array(
    '#type' => 'submit',
	  '#prefix' => '<div style="clear:both;">',
    '#value' => 'Save',
	  '#suffix' => '</div>',
  );
  $form_state['cms'] = $info;
	return $form;
}

function cms_assign_topic_form_validation($form, &$form_state) {
  
}

function cms_assign_topic_form_submit($form, &$form_state) {
	$node = $form_state['cms']['node'];
	$op = $form_state['cms']['op'];
	$conference = $form_state['cms']['conference'];
	
	//drupal_set_message('You selected ' . $form_state['values']['subtopic']);
	
	$subtopic = cms_get_subtopic($node->subtopic);
  $record->event_id = $node->event;
  $record->abstract_nid = $node->nid;
  $record->subtopic = $form_state['values']['subtopic'];
  $subtopic = cms_get_subtopic($record->subtopic);
  $record->topic = $subtopic->topic_id;
  //dsm($record);
  
  if(!drupal_write_record('cms_abstract', $record, array('abstract_nid', 'event_id'))) {
    drupal_set_message('An error occurred when saving the topic information to your abstract.', 'error');
  }
  //$_SESSION['messages'] = array();  
  $path = 'node/' . $node->nid;
  //$path = 'conferences/' . $conference->url;
  drupal_goto($path);
}

/**
 * Generate the array for the topic selection form
 */
function cms_add_topic_form(&$form_state, $conference_id, $abstract_nid) {
	$topics = cms_get_topics();
	foreach ($topics as $topic_id => $topic) {
		$options[$topic_id] = $topic->title;
	}
	$form['topic'] = array(
    '#type' => 'select',
    '#options' => $options,
	  '#description' => t('Please select the appropriate Main Topic for which you want to add your suggestion.'),
	  '#required' => TRUE,
	);
	$form['subtopic'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Title of your suggestion'),
	  '#description' => t('Provide a short title that will appear on the topic selection screen.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
	);
	$form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => '<div style="clear:both;">',
    '#value' => 'Submit',
    '#suffix' => '</div>',
  );
  $conference = cms_get_conference($conference_id);
  $form['conference'] = array(
    '#type' => 'value',
    '#value' => $conference,
  );
  $form['abstract_nid'] = array(
    '#type' => 'value',
    '#value' => $abstract_nid,
  );
  return $form;	
}

/**
 * Generate the array for the topic selection form
 */
function cms_add_topic_form_submit($form, &$form_state) {
	dsm($form_state['values']);
	$record-> event_id = $form_state['values']['conference']->event_id;
	$record-> topic_id = $form_state['values']['topic'];
	$record-> title = $form_state['values']['subtopic'];
	dsm($record);
	drupal_write_record('cms_subtopic', $record);
	$path = 'conferences/' . $form_state['values']['conference']->url . ' /abstract/' . $form_state['values']['abstract_nid'] . '/topic';
	drupal_goto($path);
}

/**
 * Menu callback: conferences/%/abstract/%/author
 * This presents the author page
 *
 * @param unknown_type $abstract_id
 */
function cms_add_author($conference_id, $abstract_nid) {
  if (!$abstract_nid) {
    drupal_goto('conferences');
  }
  
  $node = node_load($abstract_nid);
  $conference = cms_get_conference($conference_id);
  $info = array(
   'node' => $node,
   'op' => $op,
   'conference' => $conference,
  );
  
  // Setup the title and breadcrumb navigation
  drupal_set_title('Add author to ' .$node->title);
  $bc = drupal_get_breadcrumb();
  $bc[] = l($conference->event, 'conferences/' . $conference->url);
  $bc[] = l($node->title, 'node/' . $node->nid);
  drupal_set_breadcrumb($bc);
  
  $o .= drupal_get_form('cms_add_author_form', $info);
  $o .= '<div style="clear:both"></div>';
  return $o;
}

/**
 * Generate the array for the author form
 */
function cms_add_author_form(&$form_state, $info) { 
  $node = $info['node'];
  
  $form['first'] = array(
    '#type' => 'textfield',
    '#title' => 'First name',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['last'] = array(
    '#type' => 'textfield',
    '#title' => 'Last name',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['credentials'] = array(
    '#type' => 'textfield',
    '#title' => 'Credentials (P.E., Ph.D, etc',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email address',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['organization'] = array(
    '#type' => 'textfield',
    '#title' => 'Organization',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['department'] = array(
    '#type' => 'textfield',
    '#title' => 'Department',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#default_value' => 'x',
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Job Title',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#default_value' => 'x',
  );
  $form['address1'] = array(
    '#type' => 'textfield',
    '#title' => 'Street Address',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['address2'] = array(
    '#type' => 'textfield',
    '#title' => 'Street Address',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#default_value' => 'x',
  );
  $form['address3'] = array(
    '#type' => 'textfield',
    '#title' => 'Street Address',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#default_value' => 'x',
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => 'City',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['state'] = array(
    '#type' => 'textfield',
    '#title' => 'State',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  $form['postal'] = array(
    '#type' => 'textfield',
    '#title' => 'Postal Code',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#default_value' => 'x',
  );
  $form['country'] = array(
    '#type' => 'textfield',
    '#title' => 'Country',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#default_value' => 'x',
  );
  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => 'Phone',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => 'x',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add author',
  );
  $form_state['cms'] = $info;
  return $form;
}

function cms_add_author_form_submit($form, &$form_state) {
  $node = $form_state['cms']['node'];
  $op = $form_state['cms']['op'];
  $conference = $form_state['cms']['conference'];
  
  // Add this author to cms_authors table
  // have to do this first in order to get the author id
  // to place in the cms_abstract_authors table
  foreach($form_state['values'] as $key => $value) {
  	$record[$key] = $value;
  }
  drupal_write_record('cms_authors', $record);
  $author_id = $record['aid'];
  
  // Now add this to the cms_abstract_authors table
  $record = array();
  $record = array(
    'abstract_nid' => $node->nid,
    'author_id' => $author_id,
  );
  drupal_write_record('cms_abstract_authors', $record);
  
  $path = 'node/' . $node->nid;
  drupal_goto($path);	
}

/**
 * Menu callback. Display the program page for a conference
 * 
 * @param $eid
 * 		The unique event id of a conference
 * 
 * @return
 * 		The HTML of the page.  
 */
function cms_abstracts_page($eid) {
	global $user;
		
	drupal_add_css(drupal_get_path('module', 'cms') . '/cms.css', 'module');
	$sql = "SELECT topic FROM cms_abstract WHERE event_id = %d GROUP BY topic";
	
	// Display the links for the user
	$items = array();
	$sql_submissions = "SELECT cms.* FROM cms_abstract cms JOIN node n ON cms.abstract_nid = n.nid WHERE cms.event_id = %d AND n.uid = %d";
	$submissions =  db_query($sql_submissions, $eid, $user->uid);
	while ($submission = db_fetch_object($submissions)) {
		$node = node_load($submission->abstract_nid);
		$items[] = l($node->title, 'node/' . $node->nid);
	}
	
	if ($items) {
		$intro = '<fieldset>';
		$intro .= '<p>You have submitted the following abstracts. You can click on the links below to view, modify or update.</p>';
		$intro .= theme('item_list', $items);
		$intro .= '</fieldset>';
	}
	
	$intro .= theme('cms_program_intro');
	
	$results = db_query($sql, $eid);
	while($row = db_fetch_object($results)) {
		if($topic = cms_get_topic($row->topic)) {
			$o1 .= theme('cms_program_group_heading', $topic, TRUE);
			$o2 .= theme('cms_program_group_heading', $topic, FALSE);
			$topics[$row->topic] = (array) $topic;
			$sub_topics = cms_get_subtopics($row->topic);
			foreach($sub_topics as $id => $sub_topic) {
				if($abstracts = cms_get_abstracts_by($eid, $id, 'subtopic')) {
					$topics[$row->topic]['sub_topics'][$id]['name'] = $sub_topic;
					$topics[$row->topic]['sub_topics'][$id]['abstracts'] = $abstracts;
					foreach($abstracts as $abstract) {
						$o1 .= theme('cms_program_abstract', $abstract, TRUE);
						$o2 .= theme('cms_program_abstract', $abstract, FALSE);
					}
				}	
			}		
		}
	}
	
	//return $intro . $o1 . "<h1><br/><br/>Full Abstracts</h1>" . $o2;
	return $intro . $o1;
}

