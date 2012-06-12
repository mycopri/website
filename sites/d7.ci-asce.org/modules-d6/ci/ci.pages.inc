<?php

/**
 * Form builder; Present the form to upload or change user photo.
 *
 * @ingroup forms
 * @see user_edit_validate()
 * @see user_edit_submit()
 */
function ci_user_photo($account) {
  drupal_set_title('Your photo');
  return drupal_get_form('ci_user_photo_form', $account);
}

/**
 * Form builder.
 */
function ci_user_photo_form(&$form_state, $account) {
  // Picture/avatar:
  if (variable_get('user_pictures', 0)) {
  	$form['picture'] = array('#type' => 'fieldset', '#title' => t('Photo'), '#weight' => 1);
		$form['_account'] = array(
			'#type' => 'value',
			'#value' => $account,	
		);
		$form['#uid'] = $account->uid;
    $picture = theme('user_picture', (object)$account);
    if ($account->picture) {
      $form['picture']['current_picture'] = array('#value' => $picture);
      $form['picture']['picture_delete'] = array('#type' => 'checkbox', '#title' => t('Delete picture'), '#description' => t('Check this box to delete your current picture.'));
    }
    else {
      $form['picture']['picture_delete'] = array('#type' => 'hidden');
    }
    $form['picture']['picture_upload'] = array(
    	'#type' => 'file',
    	'#title' => t('Upload picture'),
    	'#size' => 48,
    	'#description' => t('Your virtual face or picture. Maximum dimensions are %dimensions and the maximum size is %size kB.',
    		array(
    			'%dimensions' => variable_get('user_picture_dimensions', '85x85'),
    			'%size' => variable_get('user_picture_file_size', '30'))) .' '. variable_get('user_picture_guidelines', '')
			);
		$form['#attributes']['enctype'] = 'multipart/form-data';
		$form['#validate'][] = 'user_validate_picture';
		$form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => 30);    
  }
  return $form;
}

/**
 * Submit function for the photo uploading form.
 */
function ci_user_photo_form_submit($form, &$form_state) {
  $account = $form_state['values']['_account'];
	$account->picture = $form_state['values']['picture'];
  user_save($account, array('picture' => $form_state['values']['picture']));
  drupal_set_message(t('Your photo has been updated.'));
  return;
}