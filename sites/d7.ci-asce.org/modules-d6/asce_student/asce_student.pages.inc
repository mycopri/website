<?php
// $Id$

/**
 * @file
 * Page callbacks
 */

function asce_student_main() {
	global $user;
  if ($user->uid) {
    $output = '<p>';
    $output .= 'This is going to be the main page of the Student Days site. ';
    $output .= 'From here, the students will be able to apply and provide their information.';
    $output .= '</p><p>All of the navigation will be via the tabs at the top. In addition, staff will be ';
    $output .= 'to see other tabs that will take them to administrative pages.';
    $output .= '</p>';
    module_load_include('inc', 'page_manager', 'plugins/tasks/page');
    //return page_manager_page_execute('student_days_main');
    $node = node_load(6401);
    module_load_include('inc', 'page_manager', 'plugins/tasks/node_view');
    drupal_set_title('');
    return page_manager_node_view($node);
  }
  else {
  	module_load_include('inc', 'page_manager', 'plugins/tasks/page');
    //return page_manager_page_execute('student_days_main');
    $node = node_load(6401);
    module_load_include('inc', 'page_manager', 'plugins/tasks/node_view');
    drupal_set_title('');
    $output = drupal_get_form('user_login');
    $output .= page_manager_node_view($node);
    return $output;    
  }
}

function asce_student_staff() {
	$output .= '<p>This page will contain links to reports and other administrative functions.</p>';
	$content[] = array(
	  'title' =>  'Edit Application Form',
	  'href' => 'admin/content/node-type/ci-student-days-seattle/fields',
	  'localized_options' => NULL,
	  'description' => 'Add, modify, or remove fields from the application form.', 
	);
	$content[] = array(
    'title' =>  'Send an email to Students',
    'href' => 'student-days/staff/mail',
    'localized_options' => NULL,
    'description' => 'Send and manage email to students.', 
  );
	$output .= theme('asce_student_staff_menu', $content);
	return $output;
}

function theme_asce_student_staff_menu($content) {
  $output = '';
  if ($content) {
    $output = '<dl class="node-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  return $output;
}

function theme_asce_student_block_style($content, $width = '33%', $float = FALSE) {
	if (empty($width)) {
		$width = '33%';
	}
	$output  = '<fieldset class="fieldgroup group-general" ';
	$output .= 'style="margin-right: 10px;';
	$output .= 'width: ' . $width . ';';
	if ($float) {
	 $output .= 'float: left;">';
	}
	else {
		$output .= '">';
	}
	$output .= $content . '</fieldset>';
	return $output;
}

function asce_student_add_content() {
	module_load_include('inc', 'node', 'node.pages');
	$output = node_add('ci_content');
	return $output;
}

function asce_student_application($edit = FALSE) {
  module_load_include('inc', 'node', 'node.pages');
  global $user;
  
  $bc[] = l(t('Home'), "");
  $bc[] = l(t('Student Days'), "student-days");
  drupal_set_breadcrumb($bc);$type = 'ci_student_days_seattle';
  
  // Need to check to see if the user already has an application
  $sql = "SELECT * FROM {node} WHERE type ='%s' AND uid = %d";
  $node_row = db_fetch_object(db_query($sql, $type, $user->uid));
  $node = node_load($node_row->nid);
  
  if($node && !$edit) { // View the node
  	$output = node_page_view($node);
  	drupal_set_title('Student Days Profile');
  }
  elseif ($node && $edit) {
  	$output = node_page_edit($node);
  	drupal_set_title('Student Days Profile');
  }
  else {
  	$output = node_add('ci-student-days-seattle');
  	drupal_set_title('Student Days Profile');
  }
  return $output;
}

function asce_student_days_application_form() {
	$form = drupal_retrieve_form('ci_student_days_seattle_node_form');
}

/**
 * Menu callback; process one time login link and redirects to the user page on success.
 */
function asce_student_pass_reset(&$form_state, $uid, $timestamp, $hashed_pass, $action = NULL) {
  global $user;

  // Check if the user is already logged in. The back button is often the culprit here.
  if ($user->uid) {
    drupal_set_message(t('You have already used this one-time login link. It is not necessary to use this link to login anymore. You are already logged in.'));
    drupal_goto();
  }
  else {
    // Time out, in seconds, until login URL expires. 24 hours = 86400 seconds.
    $timeout = 86400;
    $current = time();
    // Some redundant checks for extra security ?
    if ($timestamp < $current && $account = user_load(array('uid' => $uid, 'status' => 1)) ) {
      // Deny one-time login to blocked accounts.
      if (drupal_is_denied('user', $account->name) || drupal_is_denied('mail', $account->mail)) {
        drupal_set_message(t('You have tried to use a one-time login for an account which has been blocked.'), 'error');
        drupal_goto();
      }

      // No time out for first time login.
      if ($account->login && $current - $timestamp > $timeout) {
        drupal_set_message(t('You have tried to use a one-time login link that has expired. Please request a new one using the form below.'));
        drupal_goto('user/password');
      }
      else if ($account->uid && $timestamp > $account->login && $timestamp < $current && $hashed_pass == user_pass_rehash($account->pass, $timestamp, $account->login)) {
        // First stage is a confirmation form, then login
        if ($action == 'login') {
          watchdog('user', 'User %name used one-time login link at time %timestamp.', array('%name' => $account->name, '%timestamp' => $timestamp));
          // Set the new user.
          $user = $account;
          // user_authenticate_finalize() also updates the login timestamp of the
          // user, which invalidates further use of the one-time login link.
          user_authenticate_finalize($form_state['values']);
          drupal_set_message(t('You have just used your one-time login link. It is no longer necessary to use this link to login. Please change your password.'));
          drupal_goto('student-days/pass/'. $user->uid .'/reset');
        }
        else {
          $form['message'] = array('#value' => t('<p>This is a one-time login for %user_name and will expire on %expiration_date.</p><p>Click on this button to login to the site and set your password.</p>', array('%user_name' => $account->name, '%expiration_date' => format_date($timestamp + $timeout))));
          $form['help'] = array('#value' => '<p>'. t('This login can be used only once.') .'</p>');
          $form['submit'] = array('#type' => 'submit', '#value' => t('Log in'));
          $form['#action'] = url("student-days/reset/$uid/$timestamp/$hashed_pass/login");
          return $form;
        }
      }
      else {
        drupal_set_message(t('You have tried to use a one-time login link which has either been used or is no longer valid. Please request a new one using the form below.'));
        drupal_goto('user/password');
      }
    }
    else {
      // Deny access, no more clues.
      // Everything will be in the watchdog's URL for the administrator to check.
      drupal_access_denied();
    }
  }
}

function asce_student_pass_reset_form(&$form_state, $user) {
	$form['account']['pass'] = array('#type' => 'password_confirm',
    '#description' => t('Enter the new password in both fields.'),
    '#size' => 25,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form_state['storage']['user'] = $user;
  return $form;
}

function asce_student_pass_reset_form_submit($form, &$form_state) {
	$user = $form_state['storage']['user'];
	unset($form_state['storage']);
  $pass = md5($form_state['values']['pass']);
  $success = db_query("UPDATE {users} SET pass = '%s' WHERE uid = %d", $pass, $user->uid);
  if (!$success) {
  	drupal_set_message('There was a problem!' . $user->uid);
  	drupal_goto('student-days');
  }
  elseif ($success) {
  	drupal_set_message('Password succesfully changed!' . $user->uid);
    drupal_goto('student-days');
  }
}

/**
 * Form builder; Configure student mail settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function asce_student_mail_settings(&$form_state, $mid = NULL) {
  drupal_set_title('Students Days Automated e-mails');
  
  $form['mail_id'] = array(
    '#type' => 'value',
    '#value' => $mid,
  );
  
  // Just some development stuff - comment this out when done
  $uids = _asce_student_get_completed();
  $devel_out = 'Total completed: ' . count($uids);
  $form['devel'] = array(
    '#value' => $devel_out,
    '#weight' => -100,
  );
  
	if (is_numeric($mid) || $mid == 'new') {
		$breadcrumb = drupal_get_breadcrumb();
		$breadcrumb[] = l('Mail Settings', 'student-days/staff/mail/settings');
    drupal_set_breadcrumb($breadcrumb);
  
  	// Load up the information from the database
  	$sql = "SELECT * FROM {asce_student_mail} WHERE mid = %d";
  	$message = db_fetch_object(db_query($sql, $mid));
  	if (!$message) {
  		drupal_set_message('You are creating a new automated e-mail.');
  		$form['new'] = array(
  		  '#type' => 'value',
  		  '#value' => TRUE,
  		);
  	}
    $text  = 'Available variables are: ';
    $text .= '!username, !site, !password, !uri, !uri_brief, !mailto, !date, !login_uri, !edit_uri, !login_url, ';
    $text .= '!first_name, !last_name, !professor_first, !professor_last, !first_login, !days_left.';
  	$email_token_help = t($text);

  	$form['email'] = array(
	    '#type' => 'fieldset',
	    '#title' => t(''),
	    '#description' => t('Customize the e-mail messages sent to students who already have accounts.') .' '. $email_token_help,
	  	);
	  $form['email']['key'] = array(
      '#type' => 'textfield',
      '#title' => t('Key'),
	    '#description' => t('You need to provide a unique key here for use with drupal_mail. If not sure, leave this alone.'),
      '#default_value' => $message->mail_key ? $message->mail_key : 'Provide a key',
      '#maxlength' => 100,
    );
  	$form['email']['subject'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Subject'),
	    '#default_value' => $message->subject ? $message->subject : 'Provide an appropriate subject',
	    '#maxlength' => 180,
  	);
  	$form['email']['body'] = array(
	    '#type' => 'textarea',
	    '#title' => t('Body'),
	    '#default_value' => $message->body ? $message->body : 'Put the body here...',
	    '#rows' => 15,
  	);
  	$form['buttons'] = array(
  	  'save' => array(
  	    '#type' => 'submit',
  	    '#value' => t('Save'),
  	  ),
  	  'test' => array(
  	    '#type' => 'submit',
  	    '#value' => t('Staff Test'),
  	  ),
  	  'send_all' => array(
  	    '#type' => 'submit',
  	    '#value' => t('Send to All'),
  	  ),
  	  'send_completed' => array(
        '#type' => 'submit',
        '#value' => t('Send to Completed'),
      ),
  	  'send_all_test' => array(
        '#type' => 'submit',
        '#value' => t('Send to All Test'),
      ),
      'send_completed_test' => array(
        '#type' => 'submit',
        '#value' => t('Send to Completed Test'),
      ),
      
  	);
  }
  else {
    $message = '<p>Below is a list of the defined automated e-mails for the student days program.</p>';
    $sql = "SELECT * FROM {asce_student_mail}";
    $results = db_query($sql);
    while($row = db_fetch_object($results)) {
      $links[] = '{' . $row->mail_key . '} ' . l($row->subject, 
        'student-days/staff/mail/settings/edit/' . $row->mid, 
        array('query' => drupal_get_destination())
      );
    }
    $message .= '<div>';
    $message .= theme('item_list', $links);
    $message .= '</div>';
    
    $links = array();
    $links[] = l('Create an email', 'student-days/staff/mail/settings/new');
    $links[] = l('Send a reminder email', 'student-days/staff/mail/send/reminder/never-logged-in');
    $message .= '<div><p>';
    $message .= theme('item_list', $links);
    $message .= '</p></div>';
    
    $form['email'] = array(
      '#value' => $message,
    );  	
  }

  return $form;
}

function asce_student_mail_settings_submit($form, &$form_state) {
	global $user;
	
	$message = $form_state['values'];
	$message['mid'] = $message['mail_id'];
	$message['mail_key'] = $message['key'];
	$message['uid'] = $user->uid;
	$message['sender'] = $user->mail;
	$message['created'] = time();
		 
	//drupal_set_message(kpr($message, TRUE));
	//drupal_set_message(kpr($form, TRUE));
  
	if ($message['new']) {
		drupal_write_record('asce_student_mail', $message);
	}
	else {
		drupal_write_record('asce_student_mail', $message, 'mid');
	}
	$clicked = $form_state['clicked_button']['#value'];
	if ($clicked == 'Staff Test') {
		$uids = array(102,275,890);
		//$uids = array(102);  //275
		db_query("DELETE FROM {asce_student_mail_log} WHERE recipient_uid IN (" . implode(',',$uids) . ") AND mail_key='%s'", $message['key']);
		$emails = _asce_student_send_mail($uids, $message, FALSE, FALSE);
		drupal_set_message('A total of ' . count($emails) . ' e-mails were sent.');
    drupal_set_message(implode('<br/>', $emails));
    $form_state['redirect'] = 'student-days/staff/mail/settings';
    return;
	}

  if ($clicked == 'Send to All' || $clicked == 'Send to All Test') {
    // Get a list of all the students
    $sql = "SELECT u.uid, u.mail FROM {users} u JOIN {users_roles} ur ON u.uid = ur.uid WHERE ur.rid = 10";
    $results = db_query($sql);
    while ($row = db_fetch_object($results)) {
    	if (!empty($row->uid)) {
    	  $uids[] = $row->uid;
    	}
    }
    if ($clicked == 'Send to All Test') {
      $emails = _asce_student_send_mail($uids, $message, FALSE, TRUE);
      drupal_set_message('This was a test send.');
      if (!empty($emails)){
      	drupal_set_message('A total of ' . count($emails) . ' e-mails were sent.');
      	drupal_set_message(implode('<br/>', $emails));
      }
      else {
      	drupal_set_message('No emails sent.');
      }
    }
    else {
    	$emails = _asce_student_send_mail($uids, $message, TRUE, FALSE);
      drupal_set_message('A total of ' . count($emails) . ' e-mails were sent.');
      drupal_set_message(implode('<br/>', $emails));
    }
  }
  
  if ($clicked == 'Send to Completed' || $clicked == 'Send to Completed Test') {
  	// Get a list of students with completed applications
  	$uids = _asce_student_get_completed();
  	if ($clicked == 'Send to Completed Test') {
      $emails = _asce_student_send_mail($uids, $message, FALSE, TRUE);
      drupal_set_message('This was a test send.');
      if (!empty($emails)){
        drupal_set_message('A total of ' . count($emails) . ' e-mails were sent.');
        drupal_set_message(implode('<br/>', $emails));
      }
      else {
        drupal_set_message('No emails sent.');
      }
  	}
    else {
      $emails = _asce_student_send_mail($uids, $message, TRUE, FALSE);
      drupal_set_message('A total of ' . count($emails) . ' e-mails were sent.');
      drupal_set_message(implode('<br/>', $emails));      
  	}
  }  
  $form_state['redirect'] = 'student-days/staff/mail/settings';
}

function _asce_student_send_mail_backup ($uids = array(), $message, $test = FALSE) {
	$count = 0;
	foreach($uids as $uid) {
		$account = user_load($uid);
		// Need to check to see if the email has already been sent
		$check = db_result(db_query("SELECT log_id FROM {asce_student_mail_log} WHERE recipient_uid=%d AND mail_key='%s'", $uid, $message['key']));
		if(!$check) {
			if ($test) {
				$emails[] = "({$account->uid})" . $account->mail . ', ' . $account->profile_first_name . ' ' . $account->profile_last_name . ', ' . "{$account->profile_professor_last_name}";
				$success = FALSE;
			}
			else {
				//drupal_set_message('Would have sent an email...');
				$success = _asce_student_mail_notify($message['mail_key'], $account);
			}
			if($success) {
				$row->mid = $message['mid'];
				$row->mail_key = $message['mail_key'];
				$row->recipient_uid = $account->uid;
				$row->recipient_mail = $account->mail;
				$row->sent_on = time();
				drupal_write_record('asce_student_mail_log', $row);
				$emails[] = $row->recipient_mail;
			}
		}
	}
	return $emails;
}

function _asce_student_send_mail ($uids = array(), $message, $log = TRUE, $test = FALSE) {
	$count = 0;
	$staff = array(102,275,14,890);
	foreach($uids as $uid) {
		$account = user_load($uid);
		// Need to check to see if the email has already been sent
		$check = db_result(db_query("SELECT log_id FROM {asce_student_mail_log} WHERE recipient_uid=%d AND mail_key='%s'", $uid, $message['key']));
		if (in_array($uid, $staff) ) {
			unset($check);
		}
		if(!$check) {
			if ($test) {
				$emails[] = "({$account->uid})" . $account->mail . ', ' . $account->profile_first_name . ' ' . $account->profile_last_name . ', ' . "{$account->profile_professor_last_name}";
			}
			else {
				$success = _asce_student_mail_notify($message, $account);
				if($log && $success) {
					$row->mid = $message['mid'];
					$row->mail_key = $message['mail_key'];
					$row->recipient_uid = $account->uid;
					$row->recipient_mail = $account->mail;
					$row->sent_on = time();
					drupal_write_record('asce_student_mail_log', $row);
					$emails[] = $account->mail;
				}
			  if(!$log && $success) {
          $emails[] = $account->mail;
        }
			}
		}
	}
	return $emails;
}

function asce_student_check_mess_settings() {
	$sql = "SELECT ur.uid FROM {users_roles} ur JOIN {users} u ON u.uid = ur.uid WHERE ur.rid=10";
	$results = db_query($sql);
  $on = 0;
	while ($row = db_fetch_object($results)) {
		$user = user_load($row->uid);
    $default .= $user->uid .', ' . $user->messaging_default . '<br/>';
    $user->messaging_default = 'mail';
    user_save($user, array('messaging_default' => 'mail'));
		$notify = db_fetch_object(db_query('SELECT user_id as uid, email_notify_is_enabled as notify FROM {pm_email_notify} WHERE user_id = %d', $row->uid)); 
		if ($notify) {
			$pm->user_id = $row->uid;
			$pm->email_notify_is_enabled = 1;
			$uids .= $row->uid . '<br/>';
			$count = $count +1;
			drupal_write_record('pm_email_notify', $pm, 'user_id');
		}
		else {
			$pm->user_id = $row->uid;
      $pm->email_notify_is_enabled = 1;
      drupal_write_record('pm_email_notify', $pm);
		}
	}
	$output .= $default . '<br/><br/>';
	$output .= $uids;
	return $output;
}

function asce_student_dashboard() {
	// Set the breadcrumb
	_asce_student_breadcrumb();
  // Get the list of students
  // In this case, rid = 10 is a Student, we need to be careful because if this changes it could be a differen group of users.
  // The asce_student module has a premission called 'receive student messages' this is what we use to filter instead of a role
  $sql = "SELECT uid FROM {users}";
  $results = db_query($sql);
  $count = 0;
  $analysis = array();
  while ($row = db_fetch_object($results)) {
    $account = user_load($row->uid);
    if (user_access('receive student messages', $account)) {
    	// Get application progress information for each student
      $info = _asce_student_percent_complete($account);
      $name = "({$info['data']->field_submit_application[0]['value']}),({$account->uid}, {$account->mail}) {$account->profile_first_name} {$account->profile_last_name}, {$account->profile_university}, {$account->profile_professor_last_name}";
      $students[$account->name] = $info['percent'];
      if ($info['percent'] == 100) {
        $count = $count + 1;
        $students_completed[] = $name;
      }
      
      // Determine how many of the students have logged in
      $created = format_date($account->created);
      $access = format_date($account->access);
      $login = format_date($account->login);
      if ($login == $created) {
        $count_never_logged_in = $count_never_logged_in + 1;
        $never_logged_in[$account->uid] = $name;
        $email_string[] = $account->mail;
      }
      
      // Determine how many students have started an application
      $sql = "SELECT nid FROM {node} WHERE type = 'ci_student_days_seattle' AND uid=%d";
      $nid = db_result(db_query($sql, $account->uid));
      if ($nid) {
      	if($info['percent'] < 100) {
      		$count_started = $count_started + 1;
      	  $students_started[$account->uid] = array('percent' => $info['percent'], 'name' => $name);
      	  // Analyze the questions answered
      	  $app = asce_student_application_status($account);
          $status = $app['status'];
          $node = $app['data'];
      	  $required_fields = array(
	          'field_gender' => 'your gender in <em>General</em>',
	          'field_graduation' => 'your expected graduation date in <em>General</em>',
	          'field_chapter' => 'whether you are a member of an ASCE Student Chapter in <em>ASCE Student Chapter</em>',
	          'field_degree'=> 'what your degree will be in <em>General</em>',
	          'field_school_writein' => 'name of your school <em>General</em>',
	          'field_major_writein'=> 'your major in <em>General</em>',
	          'field_expected_outcome' => 'why you want to come in <em>Statement of Interest</em>',
	          'field_mean_to_you' => 'what Civil Engineering means to in <em>Statement of Interest</em>',
	          'field_area' => 'what area you want to be in <em>Statement of Interest</em>',
	          'field_resume' => 'a resume in <em>Resume</em>',
	          'field_gpa' => 'your GPA in <em>Education and Experience</em>',
	          'field_course_list' => 'a list of courses you have taken and are taking <em>Education and Experience</em>',
	          'field_total_funding_request' => 'an estimate of your travel budget in <em>Travel Information</em>',
	          'field_departure' => 'where you are coming from in <em>Travel Information</em>',
	          'field_travel_cost' => 'details of your travel costs in <em>Travel Information</em>',
          );
          foreach($required_fields as $key => $value) {
          	$tmp = $node->$key;
          	$value = $tmp[0]['value'];
          	if ($key == 'field_resume') {
          		$value = $tmp[0]['filename'];
          	}
          	if (!empty($value)) {
          		$analysis[$key] = $analysis[$key] + 1;
          	}
          }
      	}
      }
    }
  }
  asort($analysis);
  $analysis_text = '<ul>';
  foreach($analysis as $key => $value) {
  	$analysis_text .= '<li>' . $key . ', ' . round($value/$count_started*100) . '%</li>';
  }
  $analysis .= '</ul>';
  
  $text = implode(', ', $students_completed);
  arsort($students);
  uasort($students_started, '_sort_percent');
  
  $started_content = '<p>' . $count_started . ' students have started an application.</p>';
  $started_content .= '<ul>';
  foreach($students_started as $value) {
    $started_content .= '<li>' . $value['percent'] . '% complete, ' . $value['name'] . '</li>';
  }
  $started_content .= '</ul>';
  $started_content .= $analysis_text;
  $started_content = theme('asce_student_block_style', $started_content, '40%', FALSE);
  $completed_content = t('<p>' . $count . ' students with 100% complete applications.<br/>' . theme('item_list', $students_completed) . '</p>');
  $completed_content = theme('asce_student_block_style', $completed_content, '30%', FALSE);
  
  $never_logged_in_text .= $count_never_logged_in . ' students have never logged in.';
  $never_logged_in_text .= '<ul>';
  foreach($never_logged_in as $value) {
    $never_logged_in_text .= '<li>' . $value . '</li>';
  }
  $never_logged_in_text .= '</ul>';
  $never_logged_in_text = theme('asce_student_block_style', $never_logged_in_text, '33%', TRUE);
  
  $content = '<div>';
  $content .= $never_logged_in_text . $started_content . $completed_content;
  $content .= '<div style="clear: left;"></div>';
  $content .= '</div>';
  return $content;
}

function _sort_percent($a, $b) {
	if ($a['percent'] == $b['percent']) {
		return 0;
	}
	return ($a['percent'] > $b['percent']) ? -1 : 1;
}

function asce_student_mail_send_reminder(&$form_state, $type = NULL) {
	$form['intro'] = array(
	  '#value' => t('Just some intro text...'),
	);
	
	return $form;
}

function _asce_student_get_completed() {
	$sql = "SELECT n.nid, n.vid, ur.uid FROM node n "; 
  $sql .= "JOIN users_roles ur ON n.uid = ur.uid ";
  $sql .= "WHERE ur.rid = 10 AND n.type = 'ci_student_days_seattle'";
	$results = db_query($sql);
	while($row = db_fetch_object($results)) {
		$info = _asce_student_percent_complete(user_load($row->uid));
		$percent = $info['percent'];
		if ($percent == 100) {
			$uids[$row->uid] = $row->uid; 
		}
	}
	return $uids;
}

function asce_student_bio($uid = NULL) {
	// Set the breadcrumb
	$breadcrumb = drupal_get_breadcrumb();
	$breadcrumb[] = l('Student Days', 'student-days');
	drupal_set_breadcrumb($breadcrumb);
	if (!$uid) {
		global $user;
		$uid = $user->uid;
	}
	$exists = db_result(db_query("SELECT uid FROM {asce_student_captain} WHERE uid = $uid"));
	if (!$exists) {
		drupal_set_title('Create your bio');
		$op = 'add';
		$bio = NULL;
	}
	else {
		drupal_set_title('Edit your bio');
		$op = 'edit';
		$info = db_fetch_array(db_query("SELECT * FROM {asce_student_captain} WHERE uid = $uid"));		
	}
	return drupal_get_form('asce_student_bio_form', $op, $uid, $info);
}

function asce_student_bio_form(&$form_state, $op, $uid, $info) {
	if ($op == 'add') {
		$form['#new'] = TRUE;
	}
	else {
		$form['#new'] = FALSE;
	}
	if (isset($uid)) {
		$form['#user'] = user_load($uid);
	}
	else {
		global $user;
		$form['#user'] = $user;
	}
	if ($info['selected']) {
		$intro = '<p>As a Captain of a team, this bio will be displayed at the top of your team page.</p>';
	}
	else {
		$intro = '<p>The bio you provide below will be made available to the Captains for the teams you have selected to be considered for.</p>';
	}
	$form['text'] = array(
	  '#value' => $intro,
	);
  $form['bio'] = array(
    '#type' => 'textarea',
    '#title' => 'Bio',
    '#default_value' => $info['bio'],
    '#rows' => 20,
    '#required' => ($word_count > 0),
  );

  $form['format'] = filter_form(2);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;	
}

function asce_student_bio_form_submit($form, &$form_state) {
	$new = $form['#new'];
	$bio = $form_state['values']['bio'];
	$user = $form['#user'];
	$row->uid = $user->uid; 
  $row->bio = $bio;
	if ($new) {
		$status = drupal_write_record('asce_student_captain', $row);
	}
	else {
	  $status = drupal_write_record('asce_student_captain', $row, 'uid');
	}
	if ($status) {
		drupal_set_message('Bio was successfully saved for ' . $user->profile_first_name . ' ' . $user->profile_last_name . ' (' . $user->name . ')');
		$row->uid = $user->uid;
    $row->timestamp = time();
    $row->object = 'bio';
    $row->activity = $new ? 'create' : 'edit';
    drupal_write_record('asce_student_activity', $row);
	} 
	else {
		drupal_set_message('Bio was not successfully saved for ' . $user->profile_first_name . ' ' . $user->profile_last_name . ' (' . $user->name . ')');
	}
}

function asce_student_team_choices_form(&$form_state, $node) {
	$sql = "SELECT nid FROM {node} WHERE type = 'ci_student_team'"; 
	$results = db_query($sql); 
	while ($row = db_fetch_object($results)) { 
		$team = node_load($row->nid);
		$values[$team->nid] = t($team->title);
	}
	
	$form['team_selection']['teams'] = array(
	  '#type' => 'checkboxes',
	  '#options' => $values,
	  '#title' => t('Teams'),
	  '#description' => t('You can only select two teams.'),
	);
	
	$form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  drupal_set_title('');
  return $form;
}

function asce_student_team_choices_form_validate($form, &$form_state) {
	$choices = $form_state['values']['teams'];
	foreach($choices as $key => $value) {
		if ($value == 0) {
			unset($choices[$key]);
		}
	}
	dsm($choices);
	dsm(count($choices));
	if (count($choices) > 2) {
		drupal_set_message(t('You can only select two teams to be considered for.'), 'error');
	};
}

function asce_student_set_instructions_form(&$form_state, $op) {
	if ($op == 'captain') {
		$var = 'asce_student_captain_instructions';
		$title = 'Captains';
		$form['#var'] = $var;
	}
	else {
		$var = 'asce_student_instructions';
    $title = 'Students';
    $form['#var'] = $var;
	}
	     $content = '<p>';
      $content .= 'Congratulations on completing your application. As you know, we have a limited number of slots. The captains of the five teams ';
      $content .= 'have been identified and now it is up to you to get selected onto one of the teams. There are now only two ways left to get invited ';
      $content .= 'to this all expense paid event - as an officer of a team or a member of a team. We are now starting the process of identifying officers.</p>';
      $content .= '<p>You need to provide a bio of yourself that will help the Captains identify recommendations to us about who should be officers for ';
      $content .= 'their teams. Conversely, Captains have been asked to provide bios of themselves so that you can decide which teams you want to be ';
      $content .= 'considered for.';
      $content .= '</p>';
	
	$default = variable_get($var, $content);
	$form['content'] = array(
    '#type' => 'textarea',
    '#title' => $title,
    '#default_value' => $default,
    '#rows' => 20,
  );

  $form['format'] = filter_form(2);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function asce_student_set_instructions_form_submit($form, &$form_state) {
	variable_set($form['#var'], $form_state['values']['content']);
	drupal_goto('student-days');
}

function asce_student_update() {
	$sql = "SELECT n.uid, n.nid FROM {node} n WHERE n.type = 'ci_student_days_seattle' AND n.uid != 0";
	$results = db_query($sql);
	while ($row = db_fetch_object($results)) {
		$sql = "SELECT * FROM {asce_student} WHERE uid = %d";
		$results2 = db_query($sql, $row->uid);
		$exists = mysql_num_rows($results2);
		$user = user_load($row->uid);
		$info = _asce_student_percent_complete($user);
		if (!$exists) {
			$row = array('uid' => $row->uid, 'nid' => $row->nid, 'percent_complete' => $info['percent']);
			drupal_write_record('asce_student', $row);
			drupal_set_message('Created...  ' . $user->uid . '...' . $info['percent']);
		}
		else {
			$row = array('uid' => $row->uid, 'nid' => $row->nid, 'percent_complete' => $info['percent']);
			drupal_write_record('asce_student', $row, 'uid');
			drupal_set_message('Updated...  ' . $user->uid . '...' . $info['percent']);
		}
		 
	}
	drupal_goto('student-days');
}

function asce_student_select_team() {
	global $user;
	$op = $_GET['op'];
	$gid = $_GET['gid'];
	$destination = $_GET['destination'];
	$row->uid = $user->uid;
	$row->team_id = $gid;
	$row->statement = 'None';

	switch ($op) {
		case 'add':
			if(drupal_write_record('asce_student_teams', $row)) {
			  $message = 'Team has been added.';
			}
			break;
		case 'remove':
			if(db_query("DELETE FROM {asce_student_teams} WHERE uid = %d AND team_id = %d", $user->uid, $gid)) {
				$message = 'Team has been removed.';
			}
			break;
	}
	drupal_set_message($message);
	drupal_goto($destination);
}

function asce_student_make_team_first($nid) {
  global $user;
  db_query("DELETE FROM {asce_student_teams_first_choice} WHERE uid = %d", $user->uid);
  $row->uid = $user->uid;
  $row->team_id = $nid;
  if (drupal_write_record('asce_student_teams_first_choice', $row)) {
    $message = 'Your first choice has been saved.';
  }
  drupal_goto('student-days');
}

function asce_student_remove_team($nid) {
  global $user;
  
  if(db_query("DELETE FROM {asce_student_teams} WHERE uid = %d AND team_id = %d", $user->uid, $nid)) {
    $message = 'Team was removed.';
  }
  
  drupal_set_message($message);
  drupal_goto('student-days');
}

/*
 *   Menu callback: Send staff a private message
 */
function asce_student_send_staff_message() {
	module_load_include('inc', 'privatemsg', 'privatemsg.pages');
	$recipients = '102,275';
	return drupal_get_form('privatemsg_new', $recipients); //, $recipients);
}

/*
 *   Menu callback: Send students a private message
 */
function asce_student_send_students_message() {
	global $_domain;
  $site_mail = domain_conf_variable_get($_domain['domain_id'], 'site_mail');
  $language = user_preferred_language($object);
	$sql = "SELECT asce_student.uid, captain.selected FROM asce_student LEFT JOIN asce_student_captain captain ON asce_student.uid = captain.uid";
  $results = db_query($sql);
  while($row = db_fetch_object($results)) {
  	if ($row->selected != 1) {
  		$uids[$row->uid] = $row->uid;
  		$user = user_load($row->uid);
  		//$site_mail = 'ci@ci-asce.org';
      if(drupal_mail('asce_student', 'notify', $user->mail, $language, $params, $site_mail)) {
      	$row->uid = 102;
      	$row->sender = $site_mail;
      	$row->destination = $user->mail;
      	$row->subject = 'Team selections.'; 
      	drupal_write_record('asce_student_mail_store', $row);
      	$content .= $user->mail . '<br/>';
      }		 
  	}
  }
  drupal_mail('asce_student', 'notify', 'moey@asce.org', $language, $params, $site_mail);
  drupal_mail('asce_student', 'notify', 'esantiago@asce.org', $language, $params, $site_mail);
  dsm($content);
  drupal_goto('student-days'); 
}

function asce_student_bio_view($uid) {	
	$account = user_load($uid);
	$bio = db_result(db_query("SELECT bio FROM {asce_student_captain} WHERE uid = $uid"));
	$back = '<p>'. l('Go back to Team Page', $_GET['destination']) . '</p>';
	$send_mess_link = '<div style="float:right;">' . l('Send ' . $account->profile_first_name . ' a message', 'priv-messages/new/' . $account->uid, array('query' => drupal_get_destination())) . '</div><div class="clear-block"></div>';
  $picture = '<div style="float: left; padding-right:10px;">' . theme('image', $account->picture) . '</div>';
  drupal_set_title($account->profile_first_name . ' ' . $account->profile_last_name);
  if (user_access('administer student days')) {
  	$content .= '<p>' . $account->name . '</p>';
  }        
  
  $content .= $back . $send_mess_link . $picture . check_markup($bio);
  $student = asce_student_get_student_data($uid);
  $content .= '<p><b>Why do you want to come?</b><br/>' . $student->application->field_expected_outcome[0]['value'] . '</p>';
  $content .= '<p><b>What does Civil Engineering mean to you?</b><br/>' . $student->application->field_mean_to_you[0]['value'] . '</p>';
  $content .= '<p><b>What area of Civil Engineering do you want to be in?</b><br/>' . $student->application->field_area[0]['value'] . '</p>';
  
  return $content;
}

function asce_student_rank_officers($form_state, $nid) {
	drupal_set_title('Rank the Officer Candidates');
  $form = array();
  $form['#nid'] = $nid;
  _asce_student_rank_officers_table($nid, $form);
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save ranking'),
  );
  return $form;
}

function _asce_student_rank_officers_table($nid, &$form) {
	$form['table'] = array(
    '#theme' => 'asce_student_rank_officers_table',
    //'#tree' => TRUE,
  );
  $form['table'];
}

function asce_student_drupal_mail_form(&$form_state, $group, $send = 0) {
	$form['#asce_student_group'] = $group;
	$form['#asce_student_send'] = $send;
	
	$form['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Subject',
	  '#size' => 60,
    //'#required' => TRUE,
  );
  
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => 'Body',
    '#rows' => 25,
    //'#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  //drupal_set_message('test');
  return $form;
}

function asce_student_drupal_mail_form_submit($form, &$form_state) {
  $send = $form['#asce_student_send'];
  $group = $form['#asce_student_group'];
  
  if ($group == 'all' || $group == 'officers') {
  	$sql = "SELECT uid FROM {asce_student_teams} WHERE is_officer = 1";
  	$results = db_query($sql);
  	while($uid = db_result($results)) {
  		$account = user_load($uid);
  		$students[$uid] = $account->mail;
  	}
  }
  
  if ($group == 'all' || $group == 'captains') {
	  $sql = "SELECT uid FROM {asce_student_captain} WHERE selected = 1";
	  $results = db_query($sql);
	  while($uid = db_result($results)) {
	    $account = user_load($uid);
	    $students[$uid] = $account->mail;
	  }
  }
  
  if ($group == 'candidates') {
    $sql = "SELECT uid FROM {asce_student_teams} GROUP BY uid";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      $account = user_load($uid);
      $students[$uid] = $account->mail;
    }
    $sql = "SELECT uid FROM {asce_student_teams} WHERE exclude = 1";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      unset($students[$uid]);
    }
  }
  
  if ($group == 'new') {
  	$sql = "SELECT s.uid FROM asce_student s LEFT JOIN asce_student_teams t ON s.uid = t.uid where t.team_id IS NULL AND percent_complete = 100";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      $account = user_load($uid);
      $students[$uid] = $account->mail;
    }
    $sql = "SELECT uid FROM {asce_student_teams} WHERE exclude = 1";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      unset($students[$uid]);
    }
  }
  
  if ($group == 'new') {
    $sql = "SELECT uid FROM {asce_student_teams} WHERE is_officer = 1";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      $exclude[$uid] = $uid;
    }
    $sql = "SELECT uid FROM {asce_student_captain} WHERE selected = 1";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      $exclude[$uid] = $uid;
    }
    $sql = "SELECT uid FROM {asce_student_teams} GROUP BY uid";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      $exclude[$uid] = $uid;
    }
    $sql = "SELECT uid FROM {asce_student} GROUP BY uid";
    $results = db_query($sql);
    while($uid = db_result($results)) {
      $account = user_load($uid);
      $students[$uid] = $account->mail;
    }
    foreach($exclude as $uid) {
    	unset($students[$uid]);
    }
  }
  drupal_set_message('Total recipients: ' . count($students));
  $params = array(
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'], 
  );
  
  foreach($students as $student) {
  	$sent[$student] = drupal_mail('asce_student', 'no-template', $student, NULL, $params, 'ci@ci-asce.org', $send);
  	$recipients[] = $sent[$student]['to'];
  }
  dsm($sent);
  dsm($recipients);
}

function asce_student_content_add($origin = NULL, $type = 'ci-content') {
  global $user;
  module_load_include('inc', 'node', 'node.pages');
  $types = node_get_types();
  $type = isset($type) ? str_replace('-', '_', $type) : NULL;
  // If a node type has been specified, validate its existence.
  if (isset($types[$type])) {
    // Initialize settings:
    $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');
    $node['ci_content'] = TRUE;
    drupal_set_title(t('Create @name', array('@name' => $types[$type]->name)));
    $output = drupal_get_form($type .'_node_form', $node);
  }
  return $output;
}

function asce_student_tables($op = 'summary') {
	switch($op) {
		case 'consolidate':
			$sql = "SELECT cap.uid, node.nid AS team_id
	       FROM `asce_student_captain` cap 
	       LEFT JOIN asce_student_teams teams ON cap.uid = teams.uid 
	       JOIN node ON node.uid = cap.uid 
	       WHERE cap.selected = 1 AND node.type = 'ci_student_team'";
			$results = db_query($sql);
			while ($row = db_fetch_object($results)) {
				$data = array (
		      'uid' => $row->uid,
		      'nid' => $row->team_id,
		      'changed' => time(),
		      'created' => time(),
		      'team_role' => 1,
				);
				if(drupal_write_record('asce_student_og_uid', $data)) {
					drupal_set_message('Added ' . $data['uid']);
				}
			}
			$output .= '<h1>Consolidate Tables</h1>';
			$output .= kpr($rows, TRUE);
			break;
		case 'summary':
			$output .= '<h1>Summary</h1>';
			$sql = "SELECT * FROM {asce_student_og_uid} asce
			         JOIN {users} u ON u.uid = asce.uid WHERE team_role = 1";
			$results = db_query($sql);
			while ($row = db_fetch_object($results)) {
				$student = asce_student_get_student_data($row->uid);
				$output .= $student->full_name . '<br/>';
			}
			break;
		case 'candidates':
			break;
	}
	return $output;
}