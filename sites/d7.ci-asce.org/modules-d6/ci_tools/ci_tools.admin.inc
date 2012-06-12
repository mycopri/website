<?php

function ci_tools_db_backup() {
	global $active_db, $db_url, $db_type;
	
	$url = parse_url($db_url);
  // Decode url-encoded information in the db connection string
  $url['user'] = urldecode($url['user']);
  // Test if database url has a password.
  $url['pass'] = isset($url['pass']) ? urldecode($url['pass']) : '';
  $url['host'] = urldecode($url['host']);
  $url['path'] = urldecode($url['path']);
  if (!isset($url['port'])) {
    $url['port'] = NULL;
  }
	
	$pwd = getcwd();
	$base_dir = '/home/' . exec('whoami') . '/mysql-backs/';
	
	$db_name = substr($url['path'], 1);
	$output .= "Backing up $db_name<br/>";
	$dest_dir = $base_dir . $db_name;
	if(!is_dir($base_dir)) {
		mkdir($base_dir);
		drupal_set_message(t('The directory %directory has been created.', array('%directory' => $base_dir)));
  	@chmod($directory, 0775); // Necessary for non-webserver users.
	}
	if(!is_dir($dest_dir)) {
		mkdir($dest_dir);
		drupal_set_message(t('The directory %directory has been created.', array('%directory' => $dest_dir)));
  	@chmod($directory, 0775); // Necessary for non-webserver users.	
	}
	$date = format_date(time(), 'custom', 'd-m-Y-H:i:s');
	$file_name = $dest_dir . '/' . $db_name . '.' . $date . '.sql.gz';
	$cmd = 'mysqldump -u institut -pasce99 ' . $db_name . ' | /bin/gzip' . ' -9 > ' . $file_name;
	$cmd_masked = 'mysqldump -u USERNAME -pPASSWORD ' . $db_name . ' | /bin/gzip' . ' -9 > ' . $file_name;
	$output .= 'Running the command...<br/>' . $cmd_masked . '<br/>';
	exec($cmd, $out, $result);
	drupal_set_message(t('%file has been created in %directory.', array('%file' => $db_name . '.' . $date . '.sql.gz', '%directory' => $dest_dir)));
	return $output;
}
