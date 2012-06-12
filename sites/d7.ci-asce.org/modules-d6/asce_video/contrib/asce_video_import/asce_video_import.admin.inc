<?php
// $Id: image_import.admin.inc,v 1.3.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Form builder function for Image Import settings form.
 */
function asce_video_import_admin_settings() {
  $form['asce_video_import_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Import path'),
    '#default_value' => variable_get('asce_video_import_path', file_directory_temp() . '/video'),
    '#description' => t("The directory to import video nodes from. Drupal will need to have write access to this directory so we can move the file.") .'<br />'
      . t("<strong>Note:</strong> a path begining with a <kbd>/</kbd> indicates the path is relative to the server's root, one starting without specifies a path relative to Drupal's root. I.e. <kbd>/tmp/video</kbd> would be the temp directory off the root while <kbd>tmp/video</kbd> would be inside Drupal's directory."),
    '#required' => TRUE,
  );
  $form = system_settings_form($form);
  // Apply our validation and submit handlers to the submit button.
  $form['buttons']['submit']['#validate'][] = 'asce_video_import_admin_settings_submit_validate';
  $form['buttons']['submit']['#submit'][] = 'asce_video_import_admin_settings_submit_submit';
  $form['buttons']['submit']['#submit'][] = 'system_settings_form_submit';
  return $form;
}

/**
 * Form validation handler for Image Import settings form.
 *
 * Checks the existence of the directory specified in $form_element.
 *
 * @see system_check_directory()
 */
function asce_video_import_admin_settings_submit_validate($form, &$form_state) {
  $import_dir = $form_state['values']['asce_video_import_path'];
  file_check_directory($import_dir, 0, 'asce_video_import_path');
  $video_dir = variable_get('video_file_path', file_directory_path() . '/video');
  // TODO: need to modify this code to check for the video file paths that exist. This is complex
  // because video uses CCK, which means there can be different video file paths.
  if (realpath($import_dir) == realpath($video_dir)) {
    form_set_error('asce_video_import_path', t("You can't import from the video module's directory. The import deletes the original files so you would just be asking for trouble."));
  }
}

/**
 * Form submit handler for Image Import settings form.
 */
function asce_video_import_admin_settings_submit_submit($form, &$form_state) {
  // Ensure that 'asce_video_import_path' variable contains no trailing slash.
  $form_state['values']['asce_video_import_path'] = rtrim($form_state['values']['asce_video_import_path'], '/');

  drupal_set_message(t("Your settings are configured correctly, you can import videos <a href='!asce_video_import_page'>here</a>.", array('!asce_video_import_page' => url('admin/content/asce_video_import'))));
}

