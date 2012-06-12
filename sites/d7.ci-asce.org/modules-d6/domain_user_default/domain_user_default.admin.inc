<?php

// $Id: domain_user_default.admin.inc,v 1.1.2.4 2009/12/04 22:42:07 nonsie Exp $

/**
 * @file
 * Domain User Default admin functions
 */

/**
 * User account form changes
 */
function _domain_user_default_user_form($op, &$edit, &$account) {
  $options = domain_domain_options();
  if ($account) {
    $default = $account->default_domain;
  }
  else {
    global $_domain;
    $default = $_domain['domain_id'];
  }
  $form['domain_user_default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Domain'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Choose the domain you wish to go to when you visit @name', array('@name' => variable_get('site_name', 'Drupal'))),
  );
  $form['domain_user_default']['default_domain'] = array(
    '#type' => 'select',
    '#title' => t('Domain'),
    '#options' => $options,
    '#default_value' => $default,
  );
  return $form;
}
