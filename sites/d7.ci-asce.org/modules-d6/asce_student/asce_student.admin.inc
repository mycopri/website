<?php
function asce_student_admin() {
	$items[] = l('Administer Captains', 'student-days/admin/captains');
	
	$content = theme('item_list', $items);
	return $content;
}

function asce_student_admin_captains() {
	// Going to add some clean up code in this to help straighten out the
	// tables. When I started the module, I created an asce_student_captain
	// table to store which students were willing to serve as captains
	// What I need to do now is consolidate it down to the main student
	// table.
	
	
	$students = asce_student_get_students();
	foreach($students as $key => $value) {
		$account = $value['account'];
		$application = $value['application'];
		$list[$key] = array(
		  'first' => $account->profile_first_name,
		  'last' => $account->profile_last_name,
		  'mail' => $account->mail,
		  'login' => $account->name,
		  'wanted_to_be_captain' => $value['wanted_to_be_captain'],
		  'is_captain' => $value['is_captain'],
		);
	}
	usort($list, 'asce_student_sort_students_by_last');
	foreach($list as $student) {
		if ($student['wanted_to_be_captain'] && !$student['is_captain']) {
		  $items[] = $student['last'] . ', ' . $student['first'];
		}
		if ($student['is_captain']) {
			$captains[] = $student['last'] . ', ' . $student['first'];
		}
	}	
	$content .= '<h1>Captains</h1>';
	$content .= '<p>' . theme('item_list', $captains) . '</p>';
	$content .= '<h1>Students who were interested in being Captains</h1>';
	$content .= '<p>Out of ' . count($students) . ' students, ' . count($items) . ' expressed interest in being a captain.';
	$content .= theme('item_list', $items);
	
	// Now list the actual captains
	return $content;
}

function asce_student_sort_students_by_last($a, $b) {
  if ($a['last'] == $b['last']) {
  	return 0;
  }
  return ($a['last'] < $b['last']) ? -1 : 1;
}