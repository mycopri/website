<?php
// $Id: image_gallery_access_plugin_access.inc,v 1.1 2009/11/21 13:42:00 salvis Exp $

/**
 * @file
 * Views access plugin for Image Gallery Access.
 */

/**
 * Access handler for image_gallery view.
 *
 * Validates View access to any one image gallery against Image Gallery Access.
 */
class image_gallery_access_plugin_access extends views_plugin_access {
  /**
   * Initialize the plugin.
   *
   * @param $view
   *   The view object.
   * @param $display
   *   The display handler.
   */
  function init(&$view, &$display) {
    $this->view = &$view;
    $this->display = &$display;
    $this->options = array();

    if (is_object($display->handler)) {
    // Note: The below is read only.
      $this->options = $display->handler->get_option('access');
    }
  }

  /**
   * Retrieve the default options when this is a new access
   * control plugin
   */
  //function option_defaults(&$options) { }

  /**
   * Provide the default form for setting options.
   */
  //function options_form(&$form, &$form_state) { }

  /**
   * Provide the default form form for validating options
   */
  //function options_validate(&$form, &$form_state) { }

  /**
   * Provide the default form form for submitting options
   */
  //function options_submit(&$form, &$form_state) { }

  /**
   * Return a string to display as the clickable title for the
   * access control.
   */
  function summary_title() {
    return t('by @Image_Gallery_Access', array('@Image_Gallery_Access' => 'Image Gallery Access'));
  }

  /**
   * Determine if the current user has access or not.
   */
  function access($account) {
    return _image_gallery_access_access_any_gallery($account);
  }

  /**
   * Determine the access callback and arguments.
   *
   * This information will be embedded in the menu in order to reduce
   * performance hits during menu item access testing, which happens
   * a lot.
   *
   * @return an array; the first item should be the function to call,
   *   and the second item should be an array of arguments. The first
   *   item may also be TRUE (bool only) which will indicate no
   *   access control.)
   */
  function get_access_callback() {
    return array('_image_gallery_access_access_any_gallery', array());
  }
}
