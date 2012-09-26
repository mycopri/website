<?php

/**
 * The contact form.
 */
function og_mail_form() {
  $form['intro'] = array(
    '#markup' => t(theme('og_mail_form_intro')),
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('og_mail_new_content_subject', t('Updates for you.')),
    '#required' => TRUE,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => variable_get('og_mail_new_content_body', t('Notification digest goes here.')),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * The developer/testing form.
 */
function og_mail_devel_form($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Generate events for testing') {
    
  }
  $form['test'] = array(
      '#type' => 'button',
      '#value' => t('Generate events for testing'),
    );
  return $form;
}

/**
 * Form submission logic for the contact form.
 */
function og_mail_form_submit($form, &$form_state) {
  $message = $form_state['values'];
  variable_set('og_mail_new_content_subject', $message['subject']);
  variable_set('og_mail_new_content_body', $message['body']);
  drupal_set_message('The template has been updated.');
}