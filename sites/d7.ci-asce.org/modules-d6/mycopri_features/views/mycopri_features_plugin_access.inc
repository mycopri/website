<?php
// $Id:$

/**
 * @file
 * Views access plugin for mycopri_features.
 */

/**
 * Access handler for mycopri_features view.
 *
 * Validates View access to specified content types.
 */
class mycopri_features_plugin_access extends views_plugin_access {
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
   * Return a string to display as the clickable title for the
   * access control.
   */
  function summary_title() {
    return t('Group nodes only');
  }

  /**
   * Determine if the current content has access or not.
   */
  function access($node) {
    return _mycopri_features_views_access($node);
  }

  function get_access_callback() {
  	if(arg(0) == 'node' && is_numeric(arg(1))) {
  		$node = node_load(arg(1));
  		return array('_mycopri_features_views_access', $node);
  	}
    return array('_mycopri_features_views_access', 'do_not_show');
  }
}
