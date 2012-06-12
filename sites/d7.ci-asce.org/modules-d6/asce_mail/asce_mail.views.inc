<?php
// $Id$

/**
 * Implementation of hook_views().
 */
function asce_mail_views_data() {
	$data['asce_mail_store']['table']['group']  = t('ASCE eMail Manager');
	
  $data['asce_mail_store']['table']['base'] = array(
       'field' => 'mid',
       'title' => 'ASCE eMail Manager',
       'help' => t("ASCE Mail Module tables"),
  );
  $data['asce_mail_store']['mid'] = array(
    'title' => t('Mid'),
    'help' => t('The unique ID of each eMail.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_asce_mail',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    /*'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),*/
    // Information for accepting a nid as a filter
    'filter' => array(
       'handler' => 'views_handler_filter_numeric',
     ),
     // Information for sorting on a nid.
       'sort' => array(
          'handler' => 'views_handler_sort',
     ),
  );
  
  // subject
  // This definition has more items in it than it needs to as an example.
  $data['asce_mail_store']['subject'] = array(
    'title' => t('Subject'), // The item it appears as on the UI,
    'help' => t('The subject of the eMail.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => 'subject', // the real field. This could be left out since it is the same.
      'group' => t('ASCE eMail Manager'), // The group it appears in on the UI. Could be left out.
      'handler' => 'views_handler_field_asce_mail',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  // sent
  // This definition has more items in it than it needs to as an example.
  $data['asce_mail_store']['sent'] = array(
    'title' => t('Sent'), // The item it appears as on the UI,
    'help' => t('The eMail has been sent.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  
  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the handlers
 * asce_mail_views uses.
 */
 function asce_mail_views_handlers()
 {
     return array(
        'info' => array(
            'path' => drupal_get_path('module', 'asce_mail') . '/includes'
         ),
         'handlers' => array(
            'views_handler_field_asce_mail' => array(
                'parent' => 'views_handler_field',
            ),
            'views_handler_field_asce_mail_sent' => array(
                'parent' => 'views_handler_field',
            ),
         ),
     );
 }