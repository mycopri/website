<?php
function asce_student_block_application() {
	global $user;

	if (user_access('administer nodes')) {
		$edit .= '  <div class="admin-links panel-hide">';
		$edit .= '    <ul class="links">';
		$edit .= '      <li class="update first last">';
		//$edit .=          l('Edit text', 'admin/build/block/configure/asce_student/0', array('query' => drupal_get_destination()));
		$edit .= '      </li>';
		$edit .= '    </ul>';
		$edit .= '  </div>';
	}

	$block['content'] .= $edit;

	if (!user_is_anonymous()) {
		// Check to see if the user already has created an application.
		$info = _asce_student_percent_complete($user);
		$status = $info['status'];
		$node = $info['data'];
		$still_need = $info['still_need'];
		$percent = $info['percent'];
		
		// Development stuff
		//if ($user->uid == 102) {
		//	$percent = 100;
		//}
		// End development stuff
		
		if ($status == 'STARTED') {
			if ($percent < 100) {
				$still_needs .= '<ul>';
				foreach($still_need as $text) {
					$still_needs .= '<li>' . $text . '</li>';
				}
				$still_needs .= '</ul>';
				$output .= '<div id="progress" class="progress">'; //style="width: 200px;
				$output .= '<div style="background: #fff;border: 1px solid #00375a;height: 1.5em;margin: 0 0.2em;"><div class="filled" style="width: '. $percent .'%"></div></div>';
				$output .= '<div class="percentage">'. $percent .'%</div>';
				if ($percent < 100) {
					$output .= '<div class="message">Application progress.</div><div>You still need to provide: <br/>' . $still_needs . '</div>';
				}
				$output .= '</div>';
				$output .= _asce_student_process_application($app);
				$picture = file_create_url($user->picture);
			}
			elseif ($percent == 100) {
				if ($node->field_submit_application[0]['value'] != 'Yes') {
          $output .= '<p>Thanks for completing your application. Once your comfortable with the information ';
          $output .= 'you have provided click on submit.</p>';
          $output .= drupal_get_form('asce_student_submit_app', $node);
          $picture = file_create_url($user->picture);   
				}
				else {
          //$output .= '<p>Thanks for submitting your application.</p>';
          //$output .= '<p>As you know, one way to get accepted and invited is to ';
          //$output .= 'be a Team Captain. Stay tuned on how to be one...</p>';
          // Load up Marvin's picture.
          //$marvin = user_load(102);
          //$picture = file_create_url($marvin->picture);
				}
			}			
//			if (isset($picture)) {
//        $alt = t("@user's picture", array('@user' => $user->name ? $user->name : variable_get('anonymous', t('Anonymous'))));
//        $output .= '<div style="float:left;">';
//        $output .= theme('image', $picture, $alt, $alt, '', FALSE);
//        $output .= '</div>';
//        if ($marvin) {
//        	$output .= '<p><br/><br/>Marvin Oey, Ph.D., P.E., M.ASCE<br/>';
//          $output .= 'Director, Construction Institute of ASCE</p>';
//        }
//        $output .= '<div class="clear-block"></div>';
//      }			
		}
		else {
			// User does not have an application, check to see if they have uploaded a photo
			if (!$user->picture) {
				//$output .= '<p>In order to start an application we ask that you upload a photo ';
				//$output .= 'of yourself below. Once you have uploaded a photo, you will be able ';
				//$output .= 'to start an application.</p>';
				$output .= drupal_get_form('asce_student_upload_picture_form', $user);
			}
			else {
				if (isset($user->picture)) {
					$alt = t("@user's picture", array('@user' => $user->name ? $user->name : variable_get('anonymous', t('Anonymous'))));
					$output .= '<div style="float:left;">';
					$output .= theme('image', $user->picture, $alt, $alt, '', FALSE);
					$output .= '</div>';
				}
				$output .= '<p style="margin-left: 90px;">Thanks for uploading your photo. Click on the link below to begin the application process!</p>';
				$output .= '<div class="clear-block"></div>';
				$output .= '<p style="text-align: center;">';
				$output .= '<a href="/student-days/apply"><strong><span style="font-size: medium;">';
				$output .= 'Create your profile/application</span></strong></a><br>';
			}
		}
	}
	$block['subject'] = '<a href="student-days">Student Days!</a>';
	$block['content'] .= $output;
	return $block;
}

function asce_student_submit_app(&$form_state, $node) {
  //dsm($node);
	$form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit Application',
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  return $form;	
}

function asce_student_submit_app_submit($form, &$form_state) {
	$node = $form_state['values']['node'];
	$node->field_submit_application[0]['value'] = 'Yes';
	node_save($node);
}

function asce_student_block_administration() {
  global $user;

  $app = asce_student_application_status($user);
  $status = $app['status'];
  if (user_access('administer nodes')) {
    $links = asce_student_admin_links();
    $block['content'] .= theme('item_list', $links);
  }
  $block['subject'] = t('Administration');
    
  return $block;
}

function asce_student_block_team_links() {
  global $user;

  if (user_access('administer nodes')) {
    $links = asce_student_team_links();
    $links[] = l('Edit the team page layout', 'admin/build/pages/nojs/operation/node_view/handlers/node_view_panel_context_12/content', array('query' => drupal_get_destination()));
    $links = array_merge($links, asce_student_team_page_create());
    $block['content'] .= theme('item_list', $links);
  }
  $block['subject'] = t('Team Links');
    
  return $block;
}

/*
 *  Assemble links that are specific to the student team page
 */
function asce_student_team_page_create() {
  $links = array();
  if(arg(1)) {
    $node = node_load(arg(1));
  }
  if (user_access('administer student days') && $node->type == 'ci_student_team') {
    $links[] = l('Add members to team', 'og/users/' . arg(1) . '/add_user', array('query' => drupal_get_destination()));
  }
  return $links;
}

function asce_student_block_team_roster() {
  
  $block['content'] = t('Photos of team members.');
  $block['subject'] = t('Team Roster');
    
  return $block;
}

function asce_student_block_team_captain_admin() {
  
  $block['subject'] = t('Administer');
  
  $links[] = l('Post your message', 'node/add/ci-student-team-content', array(query => array('gids[]' => arg(1))));
  $links[] = l('Membership Details', 'og/users/' . arg(1), array('query' => array('destination' => $_GET['q'], 'team' => arg(1))));
  
  if (user_access('administer nodes')) {
  	$links[] = l('Edit student_team_views', 'admin/build/views/edit/student_team_members', array(query => drupal_get_destination()));
  	$links[] = l('Edit the student team panel page', 'admin/build/pages/nojs/operation/node_view/handlers/node_view_panel_context_12/content', array(query => drupal_get_destination()));
  }
  
  $block['content'] = theme('item_list', $links);  
  return $block;
}

function asce_student_block_team_status() {
  global $user;

  $block['subject'] = t('Team Status');
  $block['content'] = t('Info on team development will appear here.');  
  return $block;
}

function asce_student_block_team_captain_form(&$form_state) {
	// Define some of the form values
	global $user;
	$sql = "SELECT captain FROM asce_student_captain WHERE uid = %d";
	if ($row = db_fetch_object(db_query($sql, $user->uid))) {
		$captain = $row->captain;
	}
	else {
		$captain = NULL;
	}
	
	if (is_null($captain)) {
		$content = '<p>Thank you for completing and submitting your application. As you are aware, the next step is to ';
		$content .= 'identify five team captains (TCs) who will be responsible for identifying three candidates ';
		$content .= 'from among the other applicants to serve as officers on their team.</p>';
		$content .= '<p>If you are interested in serving as one of the five team captains, please click on the ';
		$content .= 'appropriate button below.</p>';
		
		$form = array(
	    'intro' => array(
	      '#value' => $content,
	    ),
	    'interested' => array(
	      '#type' => 'submit',
	      '#value' => 'I am interested in being a Team Captain',
	    ),
	    'not_interested' => array(
	      '#type' => 'submit',
	      '#value' => 'I am not interested in being a Team Captain',
	    ),
	  );
	}
	elseif ($captain == 0) {
		$content = '<p>You have indicated that you are not interested in being a Team Captain. If ';
    $content .= 'you change your mind, you can let us know by clicking on the button below.</p> ';
    $form = array(
      'intro' => array(
        '#value' => $content,
      ),
    'interested' => array(
        '#type' => 'submit',
        '#value' => 'I am now interested in being a Team Captain',
      ),
    ); 
	}
  elseif ($captain == 1) {
    $content = '<p>You have indicated that you would like to be considered as a Team Captain. If ';
    $content .= 'you change your mind, you can let us know by clicking on the button below.</p> ';
    $form = array(
      'intro' => array(
        '#value' => $content,
      ),
    'interested' => array(
        '#type' => 'submit',
        '#value' => 'I am no longer interested in being a Team Captain',
      ),
    ); 
  }
  return $form;
}

function asce_student_block_team_captain_form_submit($form, &$form_state) {
	global $user;
	$op = $form_state['values']['op'];
	if ($op == 'I am interested in being a Team Captain') {
		$row->uid = $user->uid;
		$row->captain = TRUE;
		drupal_write_record('asce_student_captain', $row);		
	}
	elseif ($op == 'I am not interested in being a Team Captain') {
    $row->uid = $user->uid;
    $row->captain = FALSE;
    drupal_write_record('asce_student_captain', $row);    
	}
  elseif ($op == 'I am now interested in being a Team Captain') {
    $row->uid = $user->uid;
    $row->captain = 1;
    drupal_write_record('asce_student_captain', $row, 'uid');    
  }
  elseif ($op == 'I am no longer interested in being a Team Captain') {
    $row->uid = $user->uid;
    $row->captain = 0;
    drupal_write_record('asce_student_captain', $row, 'uid');    
  }
}

function asce_student_block_messages() {
	
	if (!privatemsg_user_access()) {
    return $block;
  }

  $count = privatemsg_unread_count();
  if ($count || variable_get('privatemsg_no_messages_notification', 0)) {
    $block = array(
      'subject' => $count ? format_plural($count, 'New message', 'New messages') : t('No new messages'),
      'content' => theme('privatemsg_new_block', $count),
    );
    return $block;
  }
  
  global $user;
  $block = array();
  $links = array();
  if (privatemsg_user_access('read privatemsg') || privatemsg_user_access('read all private messages') ) {
    $link = l('Go to your message box', privatemsg_get_dynamic_url_prefix($user->uid));
  }
  $block = array(
    'subject' => '',
    'content' => $link,
  );
  return $block;
}

function asce_student_block_front() {
  global $user;
  
  if (!user_access('view student days content', $user)) {
    return;
  }
  $links[] = l('Go to student days page', 'student-days');
  $block['content'] .= theme('item_list', $links);
  $block['subject'] = t('Student Days');
    
  return $block;
}

function asce_student_block_things_you_can_do() {
  global $user;
  $student = asce_student_get_student_data($user->uid);
  //dsm($student);
  
  if (asce_student_access() || asce_student_is_candidate(arg(1))) {
    $content .= l('Add a post to the team page', 'student-days/content/add', array('query' => array('gids[]' => arg(1), 'destination' => $_GET['q'])));
  }
  $block['content'] .= $content;
  $block['subject'] = t('Create Content');
    
  return $block;
}

function asce_student_admin_links() {
  $links[] = l('View Dashboard', 'student-days/staff/dashboard');  
  $links[] = l('Add an announcement', 'node/add/ci-content', 
    array('query' => array('student-announcement' => TRUE, 'destination' => $_GET['q'])));
  //$links['application'] = l('Edit the application form', 'admin/content/node-type/ci-student-days-seattle/fields', array('query' => drupal_get_destination()));
  $links[] = l('Manage applications', 'student-days/staff/applications/manage');
  $links[] = l('Manage Automatic e-mails', 'student-days/staff/mail/settings');
  //$links['email-templates'] = l('Mail Templates', 'student-days/staff/mail/settings');
  //$links['students'] = l('View Students', 'student-days/staff/students');
  //$links['students-export'] = l('Export Student List', 'student-days/staff/students/export');
  //$links['applications'] = l('View Applications', 'student-days/staff/applications');
  $links[] = l('Edit the page view', 'admin/build/mini-panels/list/student_top/edit/content', array('query' => drupal_get_destination()));
  $links[] = l('Edit the views', 'admin/build/views/edit/ci_student_days', array('query' => drupal_get_destination()));
  $links[] = l('Sync asce_student table', 'student-days/update-tables');
  return $links;
}

function asce_student_team_links() {
  $sql = "SELECT * FROM {node} WHERE type = 'ci_student_team'";
  $results = db_query($sql);
  while ($row = db_fetch_object($results)) {
    $links[] = l($row->title, 'node/' . $row->nid);
  }
  return $links;
}

function asce_student_create_links($group) {
  global $user;
  $post_types = og_get_types('group_post');
  $types = node_get_types();
  foreach ($post_types as $post_type) {
    // We used to check for node_access(create) but then node admins would get a false positive and see node types that they could not create.
    // When this becomes a proper menu in D6, we get sorting for free
    $type_name = $types[$post_type]->name;
    $type_url_str = str_replace('_', '-', $post_type);
    if (module_invoke($types[$post_type]->module, 'access', 'create', $post_type, $user)) {
      $links["create_$post_type"] = l(t('Create !type', array('!type' => $type_name)), "node/add/$type_url_str", array(
        'attributes' => array('title' => t('Add a new !type in this group.', array('!type' => $type_name))),
        'query' => "gids[]=$group->nid",
        ));
    }
  }
  return isset($links) ? $links : array();
}

function asce_student_block_team_captain() {
  // First check to see if the student has completed and submitted their application
  // Load up their application
  global $user;
  $sql = "SELECT n.nid FROM {node} n JOIN {users} u ON n.uid = u.uid WHERE u.uid = %d AND n.type = 'ci_student_days_seattle';";
  $result = db_query($sql, $user->uid);
  $nid = db_result($result);
  $total = mysql_num_rows($result);
  if ($total == 1) {
    $node = node_load($nid);
    $block['content'] = drupal_get_form('asce_student_block_team_captain_form');
  }
  else {
    //$block['content'] = t('Looks like there are two applications for this user! Check on this.');
    $block['content'] = '';
  }
  $block['subject'] = 'Be a Team Captain!';
  $block['content'] = '<p>Thanks to all of you who have expressed interest in being a team captain. We are in the process ';
  $block['content'] .= 'of looking over the information you have provided and may be contacting some of you directly.</p>';
  $block['content'] .= '<p>In the meantime, should you need to contact us, please email Erin Santiago at esantiago@asce.org.</p>';
  $block['content'] = check_markup($block['content'], 2);
  return $block;
}

function asce_student_block_leaders() {
	global $user;
	$block['subject'] = t('Leaders');
	
  $links[] = l('View list of potential captains', 'student-days/staff/applications/1');
	
  if (user_access('is a leader')) {
  	$content = theme('item_list', $links);
	  $block['content'] = $content;
  }
  return $block;
}

function asce_student_block_challenge() {
	global $user;
  $node = node_load(arg(1));
  $is_main_student_days_page = $_GET['q'] == 'student-days' ? 1 : 0;
  $is_admin = user_access('administer student days');
  $teams = asce_student_get_teams();
  foreach($teams as $team) {
  	foreach($team['roster'] as $uid => $value) {
  		$students[$uid] = $uid;
  	}
  }
  if (!in_array($user->uid, $students) && !user_access('administer student days')) {
    return;	
  }
  $letter = node_load(7465);
  if(user_access('administer student days')) {
  	$content .= '<p>' . l('Edit text', 'node/7465/edit', array('query' => drupal_get_destination())) . '</p>';
  }
  //dsm($letter);
  $block['subject'] = 'The Project Challenge Has Begun!';
  $content .= '<p>Below is the first important bit of information that you are going to need for the CI Student Days Challenge.</p>';
  $content .= '<p>Be sure to look at the RFI Questions/Responses as it contains some important information.</p>';
  $content .= '<p>Here are the Leaders of the Challenge, you can send them messages by clicking on their photo below.<br/>';
  
  // display the leaders
  global $base_url;
  $leaders[0] = user_load(972); //John Balzer
  $leaders[1] = user_load(1152); //Nikki
  $leaders[2] = user_load(1151); //Greg
  foreach ($leaders as $key => $member) {
  	switch ($key) {
  		case 0:
  			$name = 'Jon Balzer';
  			$company = 'Granite';
  			break;
  		case 1;
  		  $name = 'Nikki Levarin';
        $company = 'Parsons Brinkerhoff';
        break;
  		case 2;
  		  $name = 'Greg Moore';
        $company = 'Parsons Brinkerhoff';
        break;     
  	}
  	if (user_access('administer student days')) {
  		$title = $name . ', ' . $member->name;
  	}
  	else {
  		$title = $name;
  	}

  	if (!$member->picture) {
  		$member['account']->picture = $base_url . '/sites/default/files/pictures/who.jpg';
  	}

  	$picture  = '<img src="' . $base_url . '/' . $member->picture . '" title = "' . $title . '"></br>';
  	$picture .= $name . '</br>' . $company;

  	if (user_is_logged_in()) {
  		$content .= 
        '<div style="float:left;align:center;height:200px;width:150px">' .
      '<a href="' . $base_url . '/priv-messages/new/' . $member->uid . '?destination=' . $_GET['q'] . '">' .  
      $picture . '</div>';
  	}
  	else {
  		$content .= 
      '<div style="float:left;align:center;height:200px;width:150px">' .  
      $picture . '</div>';
  	}
  }
  $content .= '<div class="clear-block"></div>';
  $content .= '<fieldset class="fieldgroup group-general">';
  //$content .= check_markup($letter->body, 2) . '</fieldset>';
  $block['content'] = $content;
  
  return $block;
}

function asce_student_block_main() {
	global $user;
	$node = node_load(arg(1));
	$is_main_student_days_page = $_GET['q'] == 'student-days' ? 1 : 0;
	$is_admin = user_access('administer student days');
	
	// Check to see if student was selected on a team
	// because this is all manual right now, the only way to do this
	// is to check the og_uid table
	$teams = asce_student_get_teams();
	//dsm($teams);
	foreach($teams as $team_nid => $team) {
		$sql = "SELECT uid FROM og_uid WHERE nid = $team_nid";
    $results = db_query($sql);
    while ($uid = db_result($results)) {			
		  $selected_students[$uid] = $uid; 
		} 
	}
	if (in_array($user->uid, $selected_students)) {
		$block['subject'] = 'Congratulations!';
		// Set the default text
    $content .= '<p>Congratulations for being selected to a team. If you haven\'t already, ' ;
    $content .= 'you should be contacted by your team captains. You have already been added to the ' ;
    $content .= 'team page. Things are still being developed on the site, but eventually ';
    $content .= 'you will be able to see updates on the right. Right now there isn\'t too much ';
    $content .= 'you can do on the site.';
	}
	else {
		$block['subject'] = 'Congratulations to the Students Below!';
		// Set the default text
    $content .= '<p>Although you were not selected to a team this year, please continue to keep your ';
    $content .= 'information up to date as we will keep your information in the database for future ';
    $content .= 'events and keep you informed on other activities that we will be organizing.<p>';
	}
	// display all the students
	foreach ($teams as $team_nid => $team) {
		$content .= '<fieldset>';
		$content .= '<h1>' . $team['node']->title . '</h1>';
		$content .= '<div id="team-' . $team_nid . '">';
		foreach($team['roster'] as $member) {
			if (user_access('administer student days')) {
				$title = $member['full_name'] . ', ' . $member['account']->name;
			}
			else {
				$title = $member['full_name'];
			}
			
			if (!$member['account']->picture) {
				$member['account']->picture = 'sites/default/files/pictures/who.jpg';
			}
      global $base_url;
			$picture  = '<img src="' . $base_url . '/' . $member['account']->picture . '" title = "' . $title . '"></br>';
			$picture .= $member['full_name'] . '</br>' . $member['school'];
			//$content .= '<span>' . $picture . '</span>';
			$content .= '<div style="float:left;align:center;height:200px;width:150px">' . $picture . '</div>';
		}
    $content .= '</div></fieldset>';
    $content .= '<div class="clear-block"></div>';
	}
	
	$block['content'] = $content;
	return $block;
}

function asce_student_block_bio() {
	global $user;
	$node = node_load(arg(1));
	if (user_access('administer nodes') && $_GET['q'] == 'student-days') {
	  $block['content'] = l('Edit captain text', 'student-days/set-text/captain') . '<br/>';
	  $block['content'] .= l('Edit student text', 'student-days/set-text/student') . '<br/>';
	}
	if (user_is_anonymous()) {
		$block['content'] = 'Please login';
		return $block;
	}
	// If the page being viewed is the main student days page
	if ($node->type != 'ci_student_team') {
		$info = db_fetch_array(db_query("SELECT * FROM {asce_student_captain} WHERE uid = {$user->uid}"));
		if (!$info) {
			$info['uid'] = $user->uid;
			$info['captain'] = 0;
			$info['selected'] = 0;
			drupal_write_record($info);
		}
		$block['subject'] = t('Let\'s get started!');
		if ($info['selected']) {
			if (($is_valid_student && time() >= mktime(0,0,0,5,4)) || user_access ('administer nodes')) {
				//$content .= '<h1>The officer selection is now open!</h1>';
				$content .= '<h1>The officer selection is now closed!</h1>';
				//$content .= '<p>Click ';
				//$content .= l('here', 'student-days', array('query' => array('others' => TRUE)));
				//$content .= ' to see the message that the others were sent.</p>';
			}
			$content .= variable_get('asce_student_captain_instructions', 'No text provided yet.');
			$sql = "SELECT nid FROM {node} WHERE type = 'ci_student_team' AND uid = %d";
			$results = db_query($sql, $user->uid);
			$nid = db_result($results);
			$content .= l('Click here to go to your team page', 'node/' . $nid) . '<br/>';
		}
		if(!$info['selected'] || $_GET['others'] == TRUE) {
			$content = variable_get('asce_student_instructions', $content);
		  $is_valid_student = db_result(db_query("SELECT uid FROM {asce_student} WHERE uid = %d", $user->uid));
      if (($is_valid_student && time() >= mktime(0,0,0,5,4)) || user_access ('administer nodes')) {
        //$content .= '<p>Note: You should now see links to each team page in the box to the right. ';
        //$content .= 'You will also notice that each team has only two officer positions open. On May 13th, ';
        //$content .= 'these positions will be closed.</p>';
        //$content .= '<p>Here\'s how this works, when you go to the team page ';
        //$content .= 'you will be able to select that team to be considered for. Each team can only hold a certain ';
        //$content .= 'number of applicants. The "No. of Applicants" for each team on the right provides a gauge ';
        //$content .= 'on how quickly any one team is filling up. Once a team has reached its limit of applicants ';
        //$content .= 'you will not be able to select that team for consideration.</p>';
      }
		}
		$content .= '<p style="float: right;">' . l('Click here to create/edit your bio', 'student-days/bio/' . $user->uid, array('query' => drupal_get_destination())) . '</p>';
	}
	else {
		$team_user = user_load($node->uid);
		$nid = db_result(db_query("SELECT nid FROM {node} WHERE uid = %d AND type = 'ci_student_days_seattle'", $team_user->uid));
		$app = node_load($nid);
	  $picture = '<div style="float:left; padding-right:1em;">' . theme('image', $team_user->picture) . '</div>';
	  $send = l('Send ' . $team_user->profile_first_name . ' a message', 'priv-messages/new/' . $team_user->uid, array('query' => drupal_get_destination()));
	  $picture .= '<div style="float: right;">' . $send . '</div>';
	  $picture .= $team_user->profile_first_name . ' ' . $team_user->profile_last_name . '<br/>';
	  $picture .= $app->field_school_writein[0]['value'] . '<br/>';
		$picture .= '<div class="clear-block"></div>';
		if (($user->uid == $team_user->uid) || user_access('administer nodes')) {
			$bio = l('Edit bio', 'student-days/bio/' . $team_user->uid, array('query' => drupal_get_destination())) . '<br/>';
		}
	  $bio .= db_result(db_query("SELECT bio FROM {asce_student_captain} WHERE uid = %d", $team_user->uid));
		$content = $bio;
	}
	$block['content'] .= $picture . $content;
	return $block;
}

function asce_student_block_team_page($op) {
	global $user;
	$is_captain = db_result(db_query("SELECT selected FROM {asce_student_captain} WHERE uid = %d", $user->uid));
	$block['subject'] = '';
	switch ($op) {
		case 'header':
			$node = node_load(arg(1));
			//$content = kpr($node, TRUE);
			//$content .= '<h2 class="pane-title">Mission</h2>';
			$content .= check_markup($node->body, 2);		
			break;
		case 'team status':
	    $sql = "SELECT uid FROM {asce_student_teams} WHERE uid = %d";
      $no_selected = mysql_num_rows(db_query($sql, $user->uid));
      if ($no_selected == 2) {
        return;
      }
      // Get total applicants
			$sql = "SELECT nid FROM {node} WHERE type = 'ci_student_days_seattle'";
			$results = db_query($sql);
			$max = 17; //round((mysql_num_rows($results) - 7)/5);
			
			// Get total applied
			$sql = "SELECT uid FROM {asce_student_teams} WHERE team_id = %d AND exclude = 0 AND is_officer = 0";
			$results = db_query($sql, arg(1));
			$total_applied = mysql_num_rows($results);
			
			// Get total of open positions
			$total_open = 2;
			
			$block['subject'] = 'Team Status';
			//$content .= '<p>Open Officer Positions: ' .$total_open . '<br/>';
			$content .= 'No. of people applied: ' . $total_applied . ' of ' . $max . '<br/></p>';
			
			if (!$is_captain) {
				// check to see if this person already has two
				$count = db_result(db_query("SELECT count(uid) as count FROM {asce_student_teams} WHERE uid = {$user->uid}"));
				//if ($count < 2) {
					$gid = arg(1);
					$already_selected = db_result(db_query("SELECT uid FROM {asce_student_teams} WHERE uid = %d AND team_id = %d", $user->uid, $gid));

					if (!$already_selected || time() <= mktime(0,0,0,5,25)) {
						$params = array(
					  'gid' => arg(1),
					  'op' => 'add',
					  'destination' => $_GET['q'],
						);
						$link = '<p>' . l('Add this team to your selection', 'student-days/select-team', array('query' => $params)) . '</p>';
					}
					else {
						$params = array(
            'gid' => arg(1),
            'op' => 'remove',
					  'destination' => $_GET['q'],
						);
						$link = '<p>' . l('Remove this team from your selection', 'student-days/select-team', array('query' => $params)) . '</p>';
					}
					// Check to see if this team is maxed out
					// Get total applicants
					$sql = "SELECT nid FROM {node} WHERE type = 'ci_student_days_seattle'";
					$results = db_query($sql);
					$max = 17; //round((mysql_num_rows($results) - 7)/5);
					$sql = "SELECT uid FROM {asce_student_teams} WHERE team_id = %d AND exclude = 0 AND is_officer = 0";
					$results = db_query($sql, arg(1));
					if (mysql_num_rows($results) >= $max) {
						$link = '<p>This team has reached it\'s maximum number of applicants.</p>';
					}
					$content = $content . $link;
				//}
			}
			break;
		case 'selected teams':
			// No need to show to team captains
			if ($is_captain) {
        return;
      }			
			$block['subject'] = 'My Selected Teams';
      //$content = '<p>Show selected teams here.</p>';
      
      break;
		case 'roster':
			$captain = db_result(db_query("SELECT selected FROM {asce_student_captain} WHERE uid = $user->uid"));
			if (time() < mktime(0,0,0,5,9)) {
				if (user_access('view officer candidates')) {
					$block['subject'] = 'Officer Candidates';
					$nid = arg(1);
					$sql = "SELECT asce_student.uid, asce_student.nid FROM {asce_student_teams} JOIN {asce_student} ";
					$sql .= "ON asce_student.uid = asce_student_teams.uid WHERE team_id = $nid";
					$results = db_query($sql);
					$content = '<p>Right now you can only view and send a message to the applicant. You will be able to select your two officers once everyone has selected a team or on Friday - whichever comes first.</p>';
					while ($row = db_fetch_object($results)) {
						$account = user_load($row->uid);
						$node = node_load($row->nid);
						$name = $account->profile_first_name . ' ' . $account->profile_last_name;
						$picture = '<div style="float: left; width:100px;">' . theme('image', $account->picture) . '</div>';
						$link = l($name, 'student-days/bio/view/' . $row->uid, array('query' => drupal_get_destination()));

						if (user_access('administer student days')) {
							$user_name = '<p>' . $account->name . '</p>';
						}
						$content .= $picture . $user_name . $link . '<br/>' . $node->field_school_writein[0]['value'] . '<br/>';
						$content .= '<div class="clear-block"></div>';
					}
				}
				if (user_access('administer student days')) {
					$content .= l('Add ranking', 'node/add/advpoll-ranking', array('query' => array('gids[]' => arg(1), 'auto' => TRUE, 'destination' => $_GET['q'])));
				}
			}
			else {
				//if (og_is_group_member(arg(1)) || user_access('administer student days')) {
					$block['subject'] = 'Members';
					$members = asce_student_get_team_members(arg(1));
					foreach($members as $member) {
						$link = user_is_logged_in() ? $member['profile_link'] . ', ' : $member['full_name'] . ', '; 
						$content .= $member['picture'] . $link . $member['role'] . '<br/>' . $member['school'] . '<br/>';
						$content .= '<div class="clear-block"></div>';
					}
				//}
			}
			break;
			case 'member candidates':
				//drupal_set_message('test');
				$check_user = asce_student_get_student_data($user->uid);
				if (!asce_student_access()) {
					break;
				}
				$sql = "SELECT uid FROM {asce_student_teams} WHERE team_id = %d";
				$results = db_query($sql, arg(1));
				while ($uid = db_result($results)) {
					$candidates[$uid] = asce_student_get_student_data($uid);
				}
				$block['subject'] = 'Candidates';
				if ($candidates) {
					foreach($candidates as $candidate) {
						if ($candidate->role != 'Captain' && $candidate->role != 'Officer') {
					  $link = user_is_logged_in() ? $candidate->profile_link : $candidate->full_name;
					  $content .= $candidate->picture . $link . '<br/>' . $candidate->school . '<br/>';
					  $content .= '<div class="clear-block"></div>';
						}
					}
				}
				break;
	}
	
	$block['content'] = $content;
	return $block;
}

function asce_student_team_add_form(&$form_state, $team_id) {
	$form['text'] = array(
    '#value' => $intro,
  );
  $form['statement'] = array(
    '#type' => 'textarea',
    '#title' => 'Your statement',
    '#default_value' => $info['statement'],
    '#rows' => 20,
  );

  $form['format'] = filter_form(2);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function asce_student_block_teams_per_candidate() {
	global $user;
	if (time() <= mktime(0,0,0,6,1)) {
		$sql = "SELECT uid FROM {asce_student_teams} WHERE uid = %d";
		$no_selected = mysql_num_rows(db_query($sql, $user->uid));
		if ($no_selected != 2) {
			return;
		}
		$user_student_data = asce_student_get_student_data($user->uid);
		$sql = "SELECT team_id FROM {asce_student_teams} WHERE uid = %d";
		$results = db_query($sql, $user->uid);
		while ($team_id = db_result($results)) {
			$teams[$team_id] = node_load($team_id);
		}
		if ($teams) {
			foreach($teams as $team) {
				$captains[$team->nid] = asce_student_get_student_data($team->uid);
			}
		}
		if (!$teams) {
			return;
		}
		if (!($user_student_data->role == 'Captain' || $user_student_data->role == 'Officer')) {
			$block['subject'] = 'Your Teams';
			foreach($teams as $team_id => $team) {
				$captain = $captains[$team_id];
				$link = user_is_logged_in() ? $captain->link . ', ' : $captain->full_name . ', ';
				$content .= $captain->picture . $link . '<br/>' . $captain->school . '<br/>';
				$content .= l('Go to Team Page', 'node/' . $team_id);
				$content .= '<div class="clear-block"></div>';
			}
		}
		else {
			$block['subject'] = 'Your Team';
			$sql = "SELECT team_id FROM {asce_student_teams} WHERE uid = %d AND is_officer = 1";
			$team_id = db_result(db_query($sql, $user->uid));
			$team = node_load($team_id);
			$captain = asce_student_get_student_data($team->uid);
			$link = user_is_logged_in() ? $captain->link . ', ' : $captain->full_name . ', ';
			$content .= $captain->picture . $link . '<br/>' . $captain->school . '<br/>';
			$content .= l('Go to Team Page', 'node/' . $team_id);
			$content .= '<div class="clear-block"></div>';
		}
		//drupal_set_message('<pre>' . print_r($teams, TRUE) . '</pre>');
		//drupal_set_message('<pre>' . print_r($captains, TRUE) . '</pre>');
		$block['content'] = $content;
	}
	else {
		
	}
	return $block;
}

function asce_student_block_selected_captains() {
	global $user;
	if (time() <= mktime(0,0,0,6,1)) {
		$sql = "SELECT uid FROM {asce_student_teams} WHERE uid = %d";
		$no_selected = mysql_num_rows(db_query($sql, $user->uid));
		$uid = db_result(db_query($sql, $user->uid));
		if ($uid && $no_selected == 2) {
			return;
		}
		$teams = asce_student_get_teams();
		$sql = "SELECT node.nid AS nid,
            node.title AS node_title,
            profile_values_profile_first_name.value AS first_name,
            profile_values_profile_last_name.value AS last_name,
            users.name AS users_name,
            users.mail AS users_mail,
            node_data_field_school_writein.field_school_writein_value AS school,
            node.type AS node_type,
            node.vid AS node_vid,
            users.picture AS users_picture,
            users.uid AS users_uid,
            node_data_field_graduation.field_graduation_value AS node_data_field_graduation_field_graduation_value
          FROM node node 
          INNER JOIN users users ON node.uid = users.uid
          LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '28'
          LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '2'
          LEFT JOIN content_field_school_writein node_data_field_school_writein ON node.vid = node_data_field_school_writein.vid
          LEFT JOIN asce_student_captain asce_student_captain ON node.uid = asce_student_captain.uid
          LEFT JOIN content_field_graduation node_data_field_graduation ON node.vid = node_data_field_graduation.vid
          WHERE node.type in ('ci_student_days_seattle') AND asce_student_captain.selected = 1 AND users.name != 'oeymarvi' AND users.name != 'esantiago'";
		$results = db_query($sql);
		while ($row = db_fetch_object($results)) {
			$uid = $row->users_uid;
			$sql = "SELECT node.nid FROM {og_uid} JOIN {node} ON node.nid = og_uid.nid WHERE og_uid.uid = %d AND og_uid.is_admin = 1 AND node.type = 'ci_student_team';";
			$results2 = db_query($sql, $uid);
			$nid = db_result($results2);

			$content .= '<div class="views-row">';
			$content .= '<span class="views-field-picture" style="width:80px;float:left; padding-right:1em;">' . theme('image', $row->users_picture) . '</span>';
			$content .= '<div class="views-field-value">' . $row->first_name . ' ' . $row->last_name . '</div>';
			$content .= '<div class="views-field-value">' . $row->school . '</div>';

			// Disaply the "Go to team page" link only if:
			//  - the user is a valid student AND the date is May 4th or greater
			//  OR
			//  - the user is staff OR a leader
			$is_valid_student = db_result(db_query("SELECT uid FROM {asce_student} WHERE uid = %d", $user->uid));
			if ((($is_valid_student || user_access('view all team pages')) && time() >= mktime(0,0,0,5,4)) || user_access ('administer nodes')) {
				// Get total applicants
				$sql = "SELECT nid FROM {node} WHERE type = 'ci_student_days_seattle'";
				$results3 = db_query($sql);
				$max = 17; //round((mysql_num_rows($results3) - 7)/5);

				// Get total applied
				$sql = "SELECT uid FROM {asce_student_teams} WHERE team_id = %d AND exclude = 0 AND is_officer = 0";
				$results3 = db_query($sql, $nid);
				$total_applied = mysql_num_rows($results3);

				// Get total of open positions
				$total_open = 2;

				$block['subject'] = 'Team Status';
				//$content .= '<div class="views-field-value">Open Officer Positions: ' .$total_open . '</div>';
				$content .= '<div class="views-field-value">No. of people applied: ' . $total_applied . ' of ' . $max . '</div>';
				$captain = db_result(db_query("SELECT selected FROM {asce_student_captain} WHERE uid = $user->uid"));
				if (!$captain || user_access('administer nodes') || user_access('view all team pages')) {
			  $content .= '<div class="views-field-value">' . l('Go to team page', 'node/' . $nid) . '</div>';
				}
			}
			$content .= '</div>';
			$content .= '<div class="clear-block"><br/></div>';
		}
		$block['subject'] = 'Captains';
    $block['content'] = $content;
	}
	else {
		if (user_is_logged_in()) {
			// Find out if this person is on a team;
			// Check to see if student was selected on a team
			// because this is all manual right now, the only way to do this
			// is to check the og_uid table
			$teams = asce_student_get_teams();
			foreach($teams as $team_nid => $team) {
				$sql = "SELECT uid FROM og_uid WHERE nid = $team_nid";
				$results = db_query($sql);
				while ($uid = db_result($results)) {
					$selected_students[$uid] = $uid;
					$students[$uid] = $team_nid;
				}
			}
			if (in_array($user->uid, $selected_students)) {
        $content = l('Go to your team page', 'node/' . $students[$user->uid]);
        $block['subject'] = 'Your Team Updates';
        $block['content'] = $content;
			}
			else {
				if (!user_access('administer student days')) {
					$block['subject'] = 'Keep your info updated';
					$content .= '<p>CI has a lot of other activities planned in the future. ';
					$content .= 'Make sure your info is up to date in order to stay informed.</p>';
					$content .= '<p>' . l('My contact info', 'user') . '</p>';
	        $block['content'] = $content;
				}
				else {
					$block = asce_student_block_team_links();
				}
			}
		}
	}
	return $block;
}

function asce_student_block_activity() {

}

function asce_student_sort($a, $b) {
	if ($a['timestamp'] == $b['timestamp']) {
		return 0;
	}
	return $a['timestamp'] > $b['timestamp'] ? -1 : 1;
}

/*
 *  helper function that takes the number of seconds and 
 *  creates a string with hours, minutes, seconds and also
 *  accounts for plural
 */
function asce_student_get_str_time_elapsed($time_elapsed) {
	if ($time_elapsed < 60) {
		$interval = 1;
		$unit = 'second';
	}
	elseif ($time_elapsed >= 60 AND $time_elapsed < 3600) {
		$interval = 60;
		$unit = 'minute';
	}
	elseif ($time_elapsed >= 3600 AND $time_elapsed < 86400) {
		$interval = 3600;
		$unit = 'hour';
	}
	elseif ($time_elapsed >= 86400 AND $time_elapsed < 604800) {
		$interval = 86400;
		$unit = 'day';
	}
	elseif ($time_elapsed >= 604800) {
		$interval = 604800;
		$unit = 'week';
	}
	$total = round($time_elapsed/$interval);
	if ($total > 1) {
    return $total . ' ' . $unit . 's ago.';
	}
	else {
    return $total . ' ' . $unit . ' ago.';
  }
}

function asce_student_block_your_selection() {
	global $user;
	if (time() <= mktime(0,0,0,6,1)) {
		$sql = "SELECT s.uid FROM asce_student s LEFT JOIN asce_student_teams t ON s.uid = t.uid where t.team_id IS NULL AND percent_complete = 100";
		$results = db_query($sql);
		while($row = db_fetch_object($results)) {
			$student = asce_student_get_student_data($row->uid);
			if ($student->role != 'Captain') {
		  $uids[] = $row->uid;
			}
		}

		$sql = "SELECT a.team_id, a.uid as uid, first.uid as first FROM {asce_student_teams} a LEFT JOIN {asce_student_teams_first_choice} first ON a.uid = first.uid WHERE a.uid = %d";
		$results = db_query($sql, $user->uid);
		if (mysql_num_rows($results)) {
	  $block['content'] = t('Your selections:');
		}
		else {
			if (time() >= mktime(0,0,0,5,25)) {
		  $block['content'] = t('Team Member Application is now closed!');
			}
			else {
				$block['content'] = t('Please select two teams to be considered for:');
			}
		}

		while($team = db_fetch_object($results)) {
			$teams[$team->team_id] = array('uid' => $team->uid, 'first' => $team->first);
		}

		while($team_id = db_result($results)) {
			$team = node_load($team_id);
			$captain = user_load($team->uid);
			$text = $captain->profile_first_name . ' ' . $captain->profile_last_name . '\'s team';
			$remove = l('Remove', 'student-days/remove-team/' . $team->nid, array('query' => drupal_get_destination()));
			$links[] =  l($text, 'node/' . $team->nid, array('query' => drupal_get_destination())) . ', ' . $remove;
			$links[] =  $text . ', ' . $remove;
		}
		if(mysql_num_rows($results)) {
			foreach($teams as $team_id => $choice) {
				$team = node_load($team_id);
				$captain = user_load($team->uid);
				$text = $captain->profile_first_name . ' ' . $captain->profile_last_name . '\'s team';
				$remove = l('Remove', 'student-days/remove-team/' . $team->nid, array('query' => drupal_get_destination()));
				if (!$choice['first']) {
					$first = l('Make first choice', 'student-days/make-team-first/' . $team->nid);
				}
				$links[] =  $text . ', ' . $remove; // . ', ' . $first;
			}
		}
		$block['content'] .= theme('item_list', $links);
	}
	return $block;
}

function asce_student_block_main_front() {
	global $user;
	$block['subject'] = 'test';
	$block['content'] = check_markup(variable_get('asce_student_block_main_front_content' , 'Please provide content.'), 2);
	
	// Display team data
	$teams = asce_student_get_teams();
	//dsm($teams);
	//$members = asce_student_get_team_members();
	foreach($teams as $team_id => $team) {
		$account = user_load($team['node']->uid);
		$content .= $team['node']->title . ', ' . $account->name . ', ' .  $account->uid . '<br/>';
	}
	
	$block['content'] .= $content;
	return $block;	
}

function asce_student_block_river_of_news() {
	if (og_is_group_member(arg(1))) {
		$block['content'] .= asce_student_team_page_posts();
	}
	$block['subject'] = 'Posts';
	
	return $block;
}

function asce_student_block_report_problem() {
	$block['subject'] = t('Report a Problem');
	$block['content'] = l('Report a problem', 'priv-messages/new/102');
	return $block;
}

function asce_student_block_my_bio() {
	// Check access permissions
	// We only want to show to the owner
	// and only if this owner is a candidate
	global $user;
	if (arg(0) != 'node') {
		$block['suject'] = 'Not a node';
    $block['content'] = t('test');
		return $block;
	}
	elseif (arg(0) == 'node') {
		$block['subject'] = 'Node 4';
		$block['content'] = 'test';
		$nid = arg(1);
		if (is_numeric($nid)) {
			drupal_set_message($nid);
			$block['subject'] = 'Node 2';
      $block['content'] = 'test';
			$node = node_load(arg(1));
			dsm($node);
			if ($node->type == 'ci_student_team') {
        $block['subject'] = t('My Bio');
		    $student = asce_student_get_student_data(1125);
		    $content .= kpr($student, TRUE);
		    $content .= '<div>';
		    $content .= '<p>' . l('Edit Bio', 'student-days/bio/' . $student->uid, array('query' => drupal_get_destination())) . '</p>';
		    $content .= '</div>';
		    $content .= '<p>' . check_markup($student->bio) . '</p>';
		    $content .= '<p><b>Why do you want to come?</b><br/>' . $student->application->field_expected_outcome[0]['value'] . '</p>';
		    $content .= '<p><b>What does Civil Engineering mean to you?</b><br/>' . $student->application->field_mean_to_you[0]['value'] . '</p>';
		    $content .= '<p><b>What area of Civil Engineering do you want to be in?</b><br/>' . $student->application->field_area[0]['value'] . '</p>';
		    $block['content'] = $content;
			}
		}
		return $block;
	}
}

function asce_student_block_main_links() {
  global $user;
  
  $content  = '<p>';
  $content .= 'Welcome to the Student Days portion of the Construction Institute\'s interactive website. This portion of the website originally ';
  $content .= 'started off as the starting point for the CI Student Days. Since this portion will evolve into more than just the CI Student Days, ';
  $content .= 'this page is going to serve as a portal to the various Student Activities that we have planned in the future.';
  $content .= '</p>';
  
  //$links[] = array (
  //  'link' => l('Students', 'students'),
  //  'text' => 'Connect with other students on the site.',
  //);
  $links[] = array (
    'link' => l('Student Days Challenge', 'student-days/challenge'),
    'text' => 'If you were not selected to a team for this year\'s CI Student Days in Seattle, you can still see what is happening by visiting this area.',
  );
    
  if ($links) {
    $content .= '<dl>';
    foreach ($links as $link) {
      $content .= '<dt>' . $link['link'] . '</dt>';
      $content .= '<dd>' . $link['text'] . '</dd>';
    }
    $content .= '</dl><p><br/><br/></p>';
    $block['content'] = $content; //theme('item_list', $links);
    //$block['subject'] = t('Welcome');
  }
  return $block;
}

function asce_student_block_test() {
	$content .= '<div style="width:150px;float:right;">';
	$content .= '<p>This is a test of styles for blocks</p>';
	$content .= '</div>';
	
	$block['subject'] = 'Test';
	$block['content'] = $content;
	
	return $block;
}

function asce_student_block_rfi() {
	global $user;
  
  $content  = '<p>';
  $content .= 'If you have a question regarding documents or the project, ';
  $content .= 'submit it by clicking the link below. The question will be posted on this page as well as the response.';
  $content .= '</p>';
  
  //$links[] = array (
  //  'link' => l('Students', 'students'),
  //  'text' => 'Connect with other students on the site.',
  //);
  $links[] = array (
    'link' => l('Submit a request for information', 'node/add/student-challenge-question'),
  );
    
  if ($links) {
    $content .= '<dl>';
    foreach ($links as $link) {
      $content .= '<dt>' . $link['link'] . '</dt>';
      $content .= '<dd>' . $link['text'] . '</dd>';
    }
    $content .= '</dl><p><br/><br/></p>';
    $block['content'] = $content; //theme('item_list', $links);
    //$block['subject'] = t('Welcome');
  }
  return $block;
}