<?php
// $Id$
/**
 * @file
 * Contains the basic 'asce_mail' field handler.
 */

/**
 * Field handler to provide simple renderer to show eMail status.
 */
class views_handler_field_asce_mail_sent extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['sent'] = array('table' => 'asce_mail_store', 'field' => 'sent');
  }

  function render($values) {
    return $values->{$this->field_alias} ? 'Yes' : 'No';
  }
}
