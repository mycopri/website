<?php

/**
 * @file
 * Administration menu callbacks for ci_student_days.
 */

function ci_student_days_add() {
	return drupal_get_form('ci_student_days_add_form');
}

function ci_student_days_add_form(&$form_state) {
	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title of event',
		'#description' => 'What you put here will appear on web site pages as the title',
	);
	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Date of event',
		'#description' => 'Enter the start date of the event. The important thing is the year as this will make the url for the student days page',
		'#date_format' => 'm/d/Y',
	);
	$form['publish'] = array(
		'#type' => 'checkbox',
		'#title' => 'Publish',
		'#description' => 'If checked the student days will be available to logged in users i.e. people can start registering for it.',
	);
	$form['buttons'] = array();
	$form['buttons']['complete'] = array(
		'#type' => 'submit',
		'#value' => 'Add',
		'#weight' => 6,
	);
	return $form;
}

function ci_student_days_add_form_validate($form, &$form_state) {
}

function ci_student_days_add_form_submit($form, &$form_state) {
	$year = date_part_extract($form_state['values']['date'], 'year', DATE_DATETIME);
	$record['description'] = $form_state['values']['title'];
	$record['date'] = $form_state['values']['date'];
	$record['publish'] = $form_state['values']['publish'];
	if(drupal_write_record('ci_student_days', $record)) {
		drupal_set_message('Changes saved.');
	}
}

function ci_student_days_edit_form(&$form_state, $eid) {
	$sql = "SELECT * FROM ci_student_days WHERE eid = %d";
	$event = db_fetch_object(db_query($sql, $eid)); 
	$form['#eid'] = $eid;
	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title of event',
		'#description' => 'What you put here will appear on web site pages as the title',
		'#default_value' => $event->description,
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

function ci_student_days_edit_form_validate($form, &$form_state) {
}

function ci_student_days_edit_form_submit($form, &$form_state) {
	$year = date_part_extract($form_state['values']['date'], 'year', DATE_DATETIME);
	$record['eid'] = $form['#eid'];
	$record['description'] = $form_state['values']['title'];
	$record['date'] = $form_state['values']['date'];
	$record['publish'] = $form_state['values']['publish'];
	if(drupal_write_record('ci_student_days', $record, array('eid'))) {
		drupal_set_message('Changes saved.');
	}
}

function ci_student_days_content() {
	return 'Main page';
}

function ci_student_days_add_event_submit($form, &$form_state) {
	dpm($form_state);
	$record->description = $form_state['values']['description'];
	drupal_write_record('ci_student_days', $record);
	drupal_goto('admin/settings/ci-student-days');
}

function ci_student_days_applications($eid) {
	$sql = 	'SELECT n.nid, n.title, n.uid FROM ci_student_days_applications ci ' .
					'JOIN node n ON n.nid = ci.nid WHERE ci.eid = %d AND ci.submitted = %d';
	$results = db_query($sql, $eid, 1);
	$options = array(
		'query' => array(
			'destination' => $_GET['q'],
			'from_admin' => 1,
		)
	);	
	while($row = db_fetch_object($results)) {
		$list[] = l($row->title, 'node/' . $row->nid, $options);
	}
	$o .= '<h2>Submitted</h2>';
	if(count($list)) {
		$o .= '<p>' . count($list) . ' completed and submitted applications.</p>';
		$o .= theme('item_list', $list);
	}
	else {
		$o .= '<p>No completed and submitted applications yet.</p>';
	}
	$results = db_query($sql, $eid, 0);
	$list = array();
	while($row = db_fetch_object($results)) {
		$list[] = l($row->title, 'node/' . $row->nid, $options);
	}
	$o .= '<h2>In Progress</h2>';
	if(count($list)) {
		$o .= '<p>' . count($list) . ' applications started.</p>';
		$o .= theme('item_list', $list);
	}
	else {
		$o .= '<p>No applications started yet.</p>';
	}	
	return $o;
}

function ci_student_days_list() {
	$sql = "SELECT * FROM ci_student_days";
	$results = db_query($sql);
	$options = array(
		'query' => array(
			'destination' => $_GET['q'],
		)
	);
	while($row = db_fetch_object($results)) {
		$list[] = l($row->description, 'admin/content/ci-student-days/' . $row->eid . '/edit', $options);
	}
	return theme('item_list', $list);
}
