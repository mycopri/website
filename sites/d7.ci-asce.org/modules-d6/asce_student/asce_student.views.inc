<?php
// $Id$

/**
 * Implementation of hook_views().
 */
function asce_student_views_data() {
  /**
   *  {asce_student_captain} table definitions
   */
	$data['asce_student_captain']['table']['group']  = t('ASCE Student Days');
  $data['asce_student_captain']['table']['base'] = array(
     'field' => 'uid',
     'title' => 'ASCE Student Days',
     'help' => t("ASCE Student Days Captain tables"),
  );
  $data['asce_student_captain']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  $data['asce_student_captain']['table']['join']['node'] = array(
    'left_table' => 'users',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  $data['users']['table']['join'] = array(
    'asce_student_captain' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  /*$data['users']['table']['join'] = array(
    'node' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );*/
  
  $data['asce_student_captain']['captain'] = array(
    'title' => t('asce_student_captain: captain'),
    'real field' => 'captain',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
      'help' => t('Is interested in being a team captain.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'help' => t('Is interested in being a team captain.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'help' => t('Is interested in being a team captain.'),
    ),
  );
  $data['asce_student_captain']['selected'] = array(
    'title' => t('asce_student_captain: selected captain'),
    'real field' => 'selected',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
      'help' => t('Has been selected a team captain.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'help' => t('Has been selected a team captain.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'help' => t('Has been selected a team captain.'),
    ),
  );
  $data['asce_student_captain']['bio'] = array(
    'title' => t('asce_student_captain: bio'),
    'real field' => 'bio',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
      'help' => t('Bio.'),
    ),
  );
  
  /**
   *  {asce_student} table definitions
   */
  $data['asce_student']['table']['group'] = t('ASCE Student Days');
  $data['asce_student']['table']['join'] = array(
     'node' => array(
       'left_field' => 'nid',
       'field' => 'nid',
    ),
  );
  $data['asce_student']['nid'] = array(
    'title' => t('asce_student: nid'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
      'help' => t('Node nid.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
      'help' => t('Node nid.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'help' => t('Node nid.'),
    ),
  );
  $data['asce_student']['uid'] = array(
    'title' => t('asce_student: uid'),
    'real field' => 'uid',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
      'help' => t('Node uid.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
      'help' => t('Node uid.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'help' => t('Node uid.'),
    ),
  );
  $data['asce_student']['percent_complete'] = array(
    'title' => t('asce_student: percent_complete'),
    'real field' => 'percent_complete',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
      'help' => t('Percent complete application.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Percent complete application.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'help' => t('Percent complete application.'),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the handlers
 * asce_student_views uses.
 */
 function asce_student_views_handlers()
 {
     return array(
        'info' => array(
            'path' => drupal_get_path('module', 'asce_student') . '/includes'
         ),
         'handlers' => array(
            'asce_student_handler_field_is_captain' => array (
                'parent' => 'views_handler_field',
            ),
         ),
     );
 }