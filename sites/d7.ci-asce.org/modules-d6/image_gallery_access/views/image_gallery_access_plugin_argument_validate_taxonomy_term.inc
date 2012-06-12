<?php
// $Id: image_gallery_access_plugin_argument_validate_taxonomy_term.inc,v 1.1 2009/11/21 13:42:00 salvis Exp $

/**
 * @file
 * THIS FILE IS CURRENTLY NOT USED!
 * Views validation plugin for Image Gallery Access taxonomy argument.
 */

/**
 * Validation handler for image vocabulary.
 *
 * Validates read access to an image vocabulary term against Image Gallery Access.
 * /
class image_gallery_access_plugin_argument_validate_taxonomy_term extends views_plugin_argument_validate_taxonomy_term {

  function init(&$view, &$argument, $id = NULL) {
    parent::init($view, $argument, $id);
  }

  function validate_form(&$form, &$form_state) {
    return parent::validate_form($form, $form_state);
  }

  function validate_argument($argument) {
    $term = taxonomy_get_term($argument);
    return parent::validate_argument($argument);
  }
}
/**/

