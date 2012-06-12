<?php
/*
 * ASCE Student Days Modules eMail functionality.
 */


function asce_student_mail() {
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l('Student Days', 'student-days');
  drupal_set_breadcrumb($breadcrumb);
  
  $output .= '<p>Welcome to the Student eMail Manager. Here you will be able to create and send emails to students that ';
  $output .= 'are in the database.<p>';
  
  $sql = "SELECT * FROM {asce_mail_store} WHERE type = '%s'";
  $results = db_query($sql, 'student days');
  $header = array(
    array(
      'data' => t('Subject'),
      'field' => 'subject',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Body'),
      'field' => 'body',
      //'sort' => 'asc',
    ),
    array(
      'data' => t('Sent'),
      'field' => 'sent',
      //'sort' => 'asc',
    ),
  );
  
  while ($row = db_fetch_array($results)) {
  	
    $subject = '<a href="mail/edit/' . $row['mid'] . '">' . $row['subject'] . '</a>'; 
    $rows[] = array($subject, $row['body'], $row['sent']);
  }
  
  $output .= theme('table', $header, $rows);
  $output .= drupal_get_form('asce_student_mail_form');
  return $output;   
}

function asce_student_mail_form(&$form_state) {
  $form['create'] = array(
   '#type' => 'submit',
   '#value' => 'Create new email',
  );
  return $form;
}

function asce_student_mail_form_submit($form, &$form_state) {
  $form_state['redirect'] ='student-days/staff/mail/create';
}

function asce_student_mail_create_form(&$form_state, $mid = NULL) {
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l('Student Days', 'student-days');
  $breadcrumb[] = l('Manage eMails', 'student-days/staff/mail');
  drupal_set_breadcrumb($breadcrumb);
  
  if (!$form_state['storage']['confirmed']) {
  	$form = asce_mail_form($form_state, $mid);
  	$form_state['storage']['mid'] = $mid;

  	$form['buttons']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
  	);

  	$form['buttons']['send'] = array(
      '#type' => 'submit',
      '#value' => t('Send now'),
  	);
  	$form['buttons']['send_test'] = array(
      '#type' => 'submit',
      '#value' => t('Send Test'),
  	);
  	if ($form_state['storage']['message']->sent) {
      $form['buttons'] = array();
      $form['go-back'] = array(
        '#type' => 'submit',
        '#value' => t('Go back'), //l('Go back', 'student-days/staff/mail'),
        '#submit' => array('asce_student_mail_back'),
      );
  	}
  }
  elseif ($form_state['storage']['confirmed'] == 1) {
  	$form = confirm_form($form,
      t('Are you sure you want to send this eMail to ALL students?'),
      isset($_GET['destination']) ? $_GET['destination'] : 'student-days/staff/mail/edit/'. $mid,
      t('This action cannot be undone.'),
      t('Send'),
      t('Cancel')
    );
    $form_state['storage']['confirmed'] = 2;
  }
  return $form;
}

function asce_student_mail_create_form_submit($form, &$form_state) {
	_asce_student_mail_save($form_state);
	$message = $form_state['values']['message'];
	//dsm($form_state);
	$clicked = $form_state['clicked_button']['#value'];
	if ($form_state['storage']['confirmed'] == 2 || $clicked == 'Send Test') {
		$sql = "SELECT users.uid, users.mail FROM users ";
		$sql .= "INNER JOIN users_roles ON users.uid = users_roles.uid ";
		if ($form_state['clicked_button']['#value'] == 'Send now') {
			$sql .= "WHERE (users_roles.rid = 10) AND (users.status <> 0)";
		}
		else {
			$sql .= "WHERE users.uid IN (102, 890, 275)";
		}
		$results = db_query($sql);
		while ($row = db_fetch_object($results)) {
			$recipients[$row->uid] = $row->mail;
		}
		$message = array(
      'type' => 'asce-mail',
      'subject' => $message['subject'],
      'body' => $message['body'],
      'new_sender' => $message['sender'],
      'params' => array(
        'mail' => array (
          'from' => $message['sender'],
		    ),
		  ),
		);
		//in order to allow for different recipients to get custom emails, we need to loop
		$count = 0;
		foreach($recipients as $uid => $mail) {
		  $recipient = array($uid => $mail);
			$send_message = $message;
			$result = messaging_message_send($recipient, $send_message, $send_method);
			$count = $count + 1;
		}
		drupal_set_message($result ? "The email was successfully sent to ALL <em>$count</em> students." : 'There was a problem sending the email.' );
		unset($form_state['storage']);
		unset($form_state['rebuild']);
    unset($form_state['process_input']);
	}
	elseif (!$form_state['storage']['confirmed'] && $clicked != 'Save changes') {
	  $form_state['storage']['confirmed'] = 1;  		
	}
	else {
	  unset($form_state['storage']);
    unset($form_state['rebuild']);
    unset($form_state['process_input']);
	}
  // Need to unset the following in $form_state; otherwise, the redirect
  // won't work because the form API thinks more processing needs to be done.
  $form_state['redirect'] = 'student-days/staff/mail';
}

function asce_student_mail_back ($form, &$form_state) {
  unset($form_state['storage']);
  unset($form_state['rebuild']);
  unset($form_state['process_input']);
  $form_state['redirect'] = 'student-days/staff/mail';
}

function _asce_student_mail_save($form_state) {
	//dsm($form_state);
  $message = $form_state['values']['message'];
  $mid = $form_state['storage']['mid'];
  if (!$message) {
    return;
  }
  global $user;
  $message['creator_uid'] = $user->uid;
  $message['type'] = 'student days';
  $message['created'] = time();
  $message = (object) $message;
  $message->sent = $form_state['storage']['message']->sent;
  
  if($form_state['storage']['confirmed'] == 2) {
  	$message->sent = TRUE;
  }
  
  if($form_state['clicked_button']['#value'] == 'Save changes') {
  	$message->sent = FALSE;
  }
  if($form_state['clicked_button']['#value'] == 'Send now') {
  	if ($form_state['storage']['confirmed'] == 1) {
  		$message->sent = FALSE;
  	}
  }
  if($form_state['clicked_button']['#value'] == 'Send Test' && !$form_state['storage']['message']->sent) {
    $message->sent = FALSE;
  }
  
  if ($mid) {
    $message->mid = $mid;
    $saved = drupal_write_record('asce_mail_store', $message, 'mid');
  }
  else {
    $saved = drupal_write_record('asce_mail_store', $message);
  }
  if ($saved) {
  	if ($form_state['clicked_button']['#value'] == 'Send now') {
  		$message  = 'The eMail was successfully saved, but not yet sent. ';
  		$message .= 'To send the message to ALL students click on the Send button below. ';
  		$message .= 'Otherwise hit cancel.';
  		drupal_set_message(t($message));
  	} 
  	elseif ($form_state['clicked_button']['#value'] == 'Send Test') {
  		drupal_set_message(t('You just sent a message to CI Staff only!'));
  	}      
  }
}
