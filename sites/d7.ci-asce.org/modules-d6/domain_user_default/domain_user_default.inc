<?php

// $Id: domain_user_default.inc,v 1.2.2.11 2010/12/01 16:52:43 jhedstrom Exp $

/**
 * @file
 * Provides Domain User Default module functions not required for
 * all page loads.
 */

/**
 * Callback handler to set default domain
 * @param $domain_id - The id of the domain to set and redirect to.
 */
function domain_user_default_set($domain_id) {
  $domain = _domain_user_default_set($domain_id);
  if ($domain) {
    drupal_goto(domain_get_uri($domain));
  }
}

/**
 * Update user with new default domain
 * @param $domain_id int
 * @return mixed
 *   - Boolean on no update, or no change of current domain
 *   - Domain array defined by $domain_id
 */
function _domain_user_default_set($domain_id) {
  global $user, $_domain;

  $account = user_load($user->uid);
  if ($domain_id != $account->default_domain) {
    $edit['default_domain'] = $domain_id;

    if ($account->uid > 0) {
      user_save($account, $edit);
    }
    elseif (module_exists('domain_session_default')) {
      // call directly since can't resave uid = 0
      _domain_user_default_user_update($op, $edit, $account);
    }

    $domain = domain_lookup($edit['default_domain']);

    if (module_exists('domain_geolocalization')) {
      domain_geolocalization_set_user_location($domain);
      unset($_SESSION['domain_user_default_location_set']);
    }
    // set user's location to this domain if it hasn't already been done
    if (variable_get('domain_user_default_display_domain_set_message', 0)) {
      drupal_set_message(t('Default domain has been changed to @domain', array('@domain' => $domain['sitename'])));
    }
  }
  elseif ($domain_id == $_domain['domain_id']) {
    $domain = domain_lookup($domain_id);
    if (variable_get('domain_user_default_display_domain_set_message', 0)) {
      drupal_set_message(t('Default domain is already set to @domain', array('@domain' => $domain['sitename'])));
    }
  }
  else {
    $domain = domain_lookup($domain_id);
  }
  return $domain;
}

/**
 * Set user's default domain.
 *
 * Also, if the Domain GeoLocalization module is enabled, the user's
 * radius is set to the radius of the domain.
 */
function _domain_user_default_user_update($op, &$edit, &$account) {
  if (isset($edit['default_domain'])) {
    $account->default_domain = $domain_id = $edit['default_domain'];
    if ($account->uid) {
      // user default
      if (db_result(db_query("SELECT uid FROM {domain_user_default} WHERE uid = %d", $account->uid))) {
        $update = array('uid');
      }
      $record = new stdClass();
      $record->uid = $account->uid;
      $record->domain_id = $domain_id;
      drupal_write_record('domain_user_default', $record, $update);
    }
    elseif (module_exists('domain_session_default')) {
      _domain_session_default_session_update($account);
    }

    // If Domain GeoLocalization module is enabled, update user's radius with the domain's radius
    if (module_exists('domain_geolocalization') && $account->domain_geolocalization) {
      $domain = domain_lookup($domain_id);

      $edit['domain_geolocalization']['radius'] = $account->domain_geolocalization['radius'] = $domain['radius'];
      module_load_include('inc', 'domain_geolocalization');
      _domain_geolocalization_update_user($edit, $account);
    }
  }
}

/**
 * Domain default switcher
 */
function domain_user_default_domain_switcher_form() {
  global $user;

  $account = user_load($user->uid);

  $form['domain_id'] = array(
    '#type' => 'select',
    '#default_value' => $account->default_domain,
    '#options' => domain_domain_options(),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );

  // Attach dynamic behaviour.
  drupal_add_js(drupal_get_path('module', 'domain_user_default') . '/js/domain-user-default.js');

  return $form;
}

/**
 * Domain default switcher submit handler
 */
function domain_user_default_domain_switcher_form_submit($form, &$form_state) {
  $domain_id = $form_state['values']['domain_id'] == -1 ? 0 : $form_state['values']['domain_id'];

  if ($domain_id) {
    domain_user_default_set($domain_id);
  }
}
