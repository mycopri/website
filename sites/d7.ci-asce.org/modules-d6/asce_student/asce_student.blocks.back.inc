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

  // if user is anonymous then take them to the create a profile
  if (user_is_anonymous()) {    
    $output .= t($message);
    $output .= '<p style="text-align: center;">';
    $output .= '<a href="student-days"><strong><span style="font-size: medium;">';
    $output .= 'Create An Account</span></strong></a><br><em>Already have an account?';
    $output .= '</em> <a href="student-days/login">Login</a></p>';
  }
  else{
    // Check to see if the user already has created an application.
    $app = asce_student_application_status($user);
    $status = $app['status'];
    $node = $app['data'];
    if ($status == 'STARTED') {
    	$total = 0;
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
    	$optional_fields = array(
          'field_chapter_officer',
          'field_conference_committee_work',
          'field_current_employer',
          'field_internship',
    	);
    	$total_complete = 0;
    	foreach($required_fields as $key => $text) {
    		$tmp = $node->$key;
    		$value = $tmp[0]['value'];
    		if ($key == 'field_resume') {
    			$value = $tmp[0]['filename'];
    		}
    		//drupal_set_message($key . '=' . $value);
    		if (!empty($value)) {
    			$total_complete = $total_complete + 1;
    		}
    		else {
    			$still_need[] = $text;
    		}
    	}
    	$still_needs .= '<ul>';
    	foreach($still_need as $text) {
    		$still_needs .= '<li>' . $text . '</li>';
    	}
    	$still_needs .= '</ul>';
    	$percent = round($total_complete/count($required_fields)*100, 0);
    	$output .= '<div id="progress" class="progress">'; //style="width: 200px;
    	$output .= '<div style="background: #fff;border: 1px solid #00375a;height: 1.5em;margin: 0 0.2em;"><div class="filled" style="width: '. $percent .'%"></div></div>';
    	$output .= '<div class="percentage">'. $percent .'%</div>';
    	if ($percent < 100) {
    		$output .= '<div class="message">Application progress.</div><div>You still need to provide: <br/>' . $still_needs . '</div>';
    	}
    	$output .= '</div>';
    	$picture = file_create_url($user->picture);
    	$output .= _asce_student_process_application($app);
    	if (isset($picture)) {
    		$alt = t("@user's picture", array('@user' => $user->name ? $user->name : variable_get('anonymous', t('Anonymous'))));
    		$output .= '<div style="float:left;">';
    		$output .= theme('image', $picture, $alt, $alt, '', FALSE);
    		$output .= '</div>';
    		$output .= '<div class="clear-block"></div>';
    	}
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
  $block['content'] = t('Links to actions that the team captain can do will go here e.g. offer invitations to members.');
  $block['subject'] = t('Team Captain Admin Items');
  return $block;
}

function asce_student_block_team_status() {
  global $user;

  $block['subject'] = t('Team Status');
  $block['content'] = t('Info on team development will appear here.');  
  return $block;
}

function asce_student_block_messages() {
  global $user;

  $block['subject'] = t('Messages');
  $block['content'] = t('Info on users messages will appear here.');  
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
  $is_admin = user_access('administer nodes');
  if ($is_admin) {
    $content .= t('<p>Action links for Team Captains</p>');
  }
  
  if ($is_admin) {
    $content .= t('<p>Action links for Team Officers</p>');
  }
  
  if ($is_admin) {
    $content .= t('<p>Action links for All Students</p>');
  }
  
  $block['content'] .= $content;
  $block['subject'] = t('Create Content');
    
  return $block;
}

function asce_student_admin_links() {
  $links['edit-page'] = l('Edit the page view', 'admin/build/mini-panels/list/student_top/edit/content', array('query' => drupal_get_destination()));
  $links['edit-view'] = l('Edit the views', 'admin/build/views/edit/ci_student_days', array('query' => drupal_get_destination()));
  $links['add-announcement'] = l('Add an announcement', 'node/add/ci-content', 
    array('query' => array('student-announcement' => TRUE, 'destination' => $_GET['q'])));
  $links['application'] = l('Edit the application form', 'admin/content/node-type/ci-student-days-seattle/fields', array('query' => drupal_get_destination()));
  $links['membership'] = l('Administer membership', 'og/users/' . $node->nid);
  $links['email'] = l('Manage Automatic e-mails', 'student-days/staff/mail/settings');
  //$links['email-templates'] = l('Mail Templates', 'student-days/staff/mail/settings');
  $links['students'] = l('View Students', 'student-days/staff/students');
  $links['students-export'] = l('Export Student List', 'student-days/staff/students/export');
  $links['applications'] = l('View Applications', 'student-days/staff/applications/list');
  
  
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

function asce_student_admin_dashboard() {
	// Get the list of students
	// In this case, rid = 10 is a Student, we need to be careful because if this changes it could be a differen group of users.
	// The asce_student module has a premission called 'receive student messages' this is what we use to filter instead of a role
	$sql = "SELECT uid FROM {users}";
  $results = db_query($sql);
  while ($row = db_fetch_object($results)) {
  	$user = user_load($row->uid);
  	if (user_access('receive student messages', $user)) {
  		$app = asce_student_application_status($user);
  		$status = $app['status'];
  		$node = $app['data'];
  		$total = 0;
  		if ($node) {
  			foreach($node as $key => $value) {
  				if(strpos($key, 'field') === 0) {
  					$tmp_value = $value['0']['value'];
  					if (!empty($tmp_value)) {
  						$fields_complete[$key] = $tmp_value;
  					}
  					else {
  						$fields_incomplete[$key] = $tmp_value;
  					}
  				}
  				$total = $total + 1;
  			}
  		}
  	}
  	if ($total > 0) {
		  $percent = round(count($fields_complete)/$total * 100, 0);
  	}
		if ($percent == 100) {
			$total_completed = $total_completed + 1;
		}
  }
  switch ($total_completed) {
  	case 0:
  		$block['content'] = t('No students have completed their application.');
  		break;
  	case 1:
  		$block['content'] = t('One student has completed an application.');
  		break;
  	default:
  		$block['content'] = t($total_completed . ' students have completed an application.');	
  }
  $block['subject'] = t('Application Status');
  return $block;  
}

