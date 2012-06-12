<?php
// $Id$
/**
 * @file
 * Contains the basic 'asce_mail' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a node.
 */
class views_handler_field_asce_mail extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['mid'] = array('table' => 'asce_mail_store', 'field' => 'mid');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_asce_mail'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_asce_mail'] = array(
      '#title' => t('Link this field to the ASCE eMail'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_asce_mail']),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_asce_mail']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "student-days/staff/mail/edit/" . $values->{$this->aliases['mid']};
    }
    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}
