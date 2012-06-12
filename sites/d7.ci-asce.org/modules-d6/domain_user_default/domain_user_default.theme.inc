<?php

// $Id: domain_user_default.theme.inc,v 1.1.2.3 2009/12/04 22:42:07 nonsie Exp $

/**
 * @file
 * Domain User Default theme functions
 */

/**
 * Theme the display of a user's default domain. Attaches dynamic ahah
 * behavior as well.
 */
function theme_domain_user_default_default_domain($domain) {
  $current_node_id = 0;
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $current_node_id = arg(1);
  }
  // define ahah behavior
  $ahah_binding = array(
    'url'   => url('domain-geolocalization/search/form/js/'. $current_node_id), // the json callback
    'event' => 'click',
    'wrapper'  => 'domain-search', // define the id of the container into
                                   // which to dump the form
    'selector' => '#domain-user-default-change',
    'effect'   => 'slide',
    'method'   => 'replace',
    'progress' => array('type' => 'throbber'),
  );

  // add required js
  drupal_add_js('misc/jquery.form.js');
  drupal_add_js('misc/ahah.js');
  drupal_add_js(array('ahah' => array('domain-user-default-change' => $ahah_binding)), 'setting');
  // @todo this could also be done via thickbox or lightbox by adding the
  // appropriate class to the anchor tag.

  // The id #domain-user-default-change drives the jquery that will
  // drop down a form.
  return '<div class="domain-user-default">
  <span class="domain-user-default-current">' . $domain['sitename'] . '</span> '
    . l(t('Change'), 'search/domain', array('attributes' => array('title' => t('Change Domain'), 'id' => 'domain-user-default-change')))
    . '
  <div id="domain-search-wrapper">
    <a href="#" id="domain-search-close" style="display: none;">'. t('Close') .'</a>
    <div id="domain-search"></div>' // This is the container the search form is dumped into
    . '
  </div>
</div>
';
}
