<?php
// $Id: domain.admin.inc,v 1.41.2.12 2010/09/22 20:53:39 agentken Exp $

/**
 * @file
 * Administration functions for the domain module.
 *
 * These functions only need to be accessed from admin/build/domain, so we put them in a separate include file.
 * @ingroup domain
 */

/**
 * Configuration form for the mobile device detection, redirection and notification
 * 
 * @return
 *   The configuration form   
 */ 
function asce_mobile_configuration_form() {
  global $base_url;
  $form['asce_mobile_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automatic redirection of the mobile user'),
    '#default_value' => variable_get('asce_mobile_redirect', FALSE),
    '#description' => 'Mobile visitors will automatically be redirected to the mobile site. But mobile users can also -- if they prefer --
    view the desktop version. In order to do so, append ?nomobil=true to the URL. The module
    will set a cookie that remembers that the user does not want to be redirected. To undo, send the user to /gomobile. The module will redirect
    the user to the mobile site and reset the cookie!',
  );
  return system_settings_form($form);
}

/**
 * The main administration page, a list of active domains.
 *
 * @return
 *  A sortable table of active domain records, or an error message.
 */
function asce_mobile_view() {
  global $_domain;
  $output = t('<p>The following mobile actions have been created.  All active actions will take place for all domains.</p>');
  $header = array(
    array('data' => t('Id'), 'field' => 'm.mid'),
    array('data' => t('Action')),
    array('data' => t('')),
  );

  $sql = 'SELECT m.* FROM {asce_mobile} m' . tablesort_sql($header);
  $result = pager_query($sql, variable_get('domain_list_size', DOMAIN_LIST_SIZE));
  while ($data = db_fetch_array($result)) {
  	if (!empty($data['redirect_to'])) {
  		$action = '<b>' . $data['url'] . '</b> will redirect to ' . $data['redirect_to'] . ' and switch to <b>' . $data['switch_theme'] . '</b> theme.';
  	}
  	else {
  		$action = '<b>' . $data['url'] . '</b> will only switch to <b>' . $data['switch_theme'] . '</b> theme.';
  	}
  	unset($operations);
  	$operations[] = l('edit', 'admin/settings/asce-mobile/edit/' . $data['mid']);
  	$operations[] = l('delete', 'admin/settings/asce-mobile/delete/' . $data['mid']);  	
    $rows[] = array('mid'=>$data['mid'], $action, theme('item_list', $operations));
  }
  if (!empty($rows)) {
    $output .= theme_table($header, $rows, array('id' => 'domain-list'));
    $output .= theme('pager', NULL, variable_get('domain_list_size', DOMAIN_LIST_SIZE), 0);
    return $output;
  }
  else {
  	drupal_set_message('No actions have been created.');
    return t('');
  }
}

/**
 * Return an array of values suitable for both create and update sql statements
 *
 * The last element on this array is ignored by the create sql because it has
 * one fewer placeholder argument as it doesnt require a WHERE clause.
 *
 * @param $form_state
 *   From state from the submit_hook
 * @return array
 *   Array of domain values for insert and update sql operations
 */
function asce_mobile_values_from_form_state($form_state) {
  return array(
    'url' => $form_state['values']['url'],
    'redirect_to' => $form_state['values']['redirect_to'],
    'switch_theme' => $form_state['values']['switch_theme'],
    'mid' => $form_state['values']['mid'],
  );
}

function asce_mobile_form($form_state, $action = array()) {
  $form = array();
  
  $form['mid'] = array('#type' => 'value', '#value' => $action['mid']);
  $form['action'] = array(
    '#type' => 'fieldset',
    '#title' => t('Action record'),
    '#collapsible' => TRUE
  );
  $form['action']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to perform action'),
    '#size' => 40,
    '#maxlength' => 80,
    '#required' => TRUE,
    '#default_value' => $action['url'],
    '#description' => t('The path or alias for which this action is to occur.'),
  );
  $form['action']['redirect_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect_to'),
    '#size' => 40,
    '#maxlength' => 80,
    '#default_value' => $action['redirect_to'],
    '#description' => t('When accessing this path redirect to this url. If no redirection, leave blank.')
  );
  
  // Get a list of themes
  global $conf;
  $themes = list_themes();
  foreach($themes as $key=>$theme) {
  	if ($theme->status == 1) {
  	  $options[$key] = $theme->name;
  	}
  }  
  $form['action']['switch_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Switch theme to'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => !empty($action['switch_theme']) ? $action['switch_theme'] : $conf['theme_default'],
    '#description' => t('The theme to switch to when accessing the URL.')
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save action record'));
  $form['#validate'][] = 'asce_mobile_form_validate';
  $form['#redirect'] = array('admin/settings/asce-mobile/actions');
  return $form;
}

/**
 * Implement asce_mobile_form validate hook.
 */
function asce_mobile_form_validate($form, &$form_state) {
  $error_list = array();
}

/**
 * Implement domain_form submit hook.
 */
function asce_mobile_form_submit($form, &$form_state) {
  $values = asce_mobile_values_from_form_state($form_state);
  // Set the proper message
  if (!empty($values['mid'])) {
    $message = t('Action record updated.');
  }
  else {
    $message = t('Action record created.');
  }
  // Run the save routine.
  $action = asce_mobile_save($values, $form_state);

  // If return is not a $domain array, something went wrong.
  if ($domain == -1) {
    $message = t('Action record failed.');
  }
  drupal_set_message($message);
  return $action;
}

/**
 * asce_mobile action save function.
 *
 * @param $values
 *   Form value information.
 * @param $edit
 *   Form state information.
 * @return
 *   $domain array on success or -1 on failure.
 */
function asce_mobile_save($values, $edit) {
  // Update or insert a record?
  $update = (isset($values['mid']) && is_numeric($values['mid'])) ? array('mid') : array();

  drupal_write_record('asce_mobile', $values, $update);

  return $domain;
}

/**
 * Implement domain_delete_form.
 *
 * @param $form_state
 *    The current form state, passed by FormsAPI.
 * @param $domain
 *    An array containing the record from the {domain} table.
 * @param $arguments
 *    An array of additional hidden key/value pairs to pass to the form.
 *    Used by child modules to control behaviors.
 *    Currently supported arguments are:
 *      'user_submitted' => TRUE
 *      Indicates that a form should not process administrative messages and paths
 *      upon form submit.  Used by the Domain User module.
 */
function asce_mobile_delete_form($form_state, $action) {
  $form = array();
  $form['mid'] = array('#type' => 'value', '#value' => $action['mid']);

  return confirm_form(
    $form,
    t('Are you sure you wish to delete the action record for <strong>%url</strong>?', array('%url' => $action['url'])),
    'admin/settings/asce-mobile/actions',
    NULL,
    t('Delete action record'),
    t('Cancel')
  );
}

/**
 * Implement domain_delete_form submit hook.
 */
function asce_mobile_delete_form_submit($form, &$form_state) {
  db_query("DELETE FROM {asce_mobile} WHERE mid = %d", $form_state['values']['mid']);
  drupal_set_message(t('Action record deleted.'));
  $form_state['redirect'] = 'admin/settings/asce-mobile/actions';
  
}