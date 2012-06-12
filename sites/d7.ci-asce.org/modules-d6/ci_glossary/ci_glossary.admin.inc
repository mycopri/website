<?php

/**
 * @file
 * Administration menu callbacks for ci_student_days.
 */

/**
 * Form builder to list and manage vocabularies.
 *
 * @ingroup forms
 * @see taxonomy_overview_vocabularies_submit()
 * @see theme_taxonomy_overview_vocabularies()
 */
function ci_glossary_overview() {
	$o = 'Provide some links:<br/>';
	$o = l('Import terms from a csv file.', 'admin/content/ci-glossary/import');
	return $o;
}

function ci_glossary_import() {
	$sql = "SELECT * FROM files f JOIN ci_glossary_import_files i_f ON f.fid = i_f.fid";
	$results = db_query($sql);
	while ($file = db_fetch_object($results)) {
		$files[] = $file;
		$links[] = l($file->filename, 'admin/content/ci-glossary/import/' . $file->fid);
	}
	$o = theme('item_list', $links);
	$o .= drupal_get_form('ci_glossary_import_file');
	return $o;
}

function ci_glossary_import_file(&$form_state) {
  $form['file'] = array(
  	'#type' => 'file',
  	'#title' => t('import file'),
  	'#size' => 48,
  	'#description' => t('Import file.'),
	);
	$form['#attributes']['enctype'] = 'multipart/form-data';
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		'#weight' => 30
	);    
  return $form;
}

function ci_glossary_import_file_submit($form, &$form_state) {
	$default_path = file_directory_path();
	$destination = $default_path . '/glossary/' . $_FILES['name'];
	if (file_check_directory($destination, FILE_CREATE_DIRECTORY)) {
		$uploaded_file = file_save_upload('file', array(), $destination);
		$row->fid = $uploaded_file->fid;
		$row->create = time();
		drupal_write_record('ci_glossary_import_files', $row);
		dpm($uploaded_file);
	};
}