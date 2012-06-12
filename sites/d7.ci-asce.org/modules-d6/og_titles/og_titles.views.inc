<?php
/* $Id: og_titles.views.inc,v 1.1.2.5 2009/05/14 14:54:08 mradcliffe Exp $ */


 /**
  * basic views 
  */
 function og_titles_views_data()
 {
	$data['og_uid_titles']['table']['group'] = t('Organic groups');

	$data['og_uid_titles']['table']['join'] = array(
        'users' => array(
            'left_field' => 'uid',
            'field' => 'uid',
        ),
        'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
        ),
   	);

	$data['og_uid_titles']['title'] = array(
		'title' => t('Member Title'),
		'help' => t('Display the title associated with the group member.'),
		'field' => array(
			'handler' => 'views_handler_field_og_title',
			'click sortable' => TRUE,
		),
		'filter' => array(
			'handler' => 'views_handler_filter_string',
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		)
	);

    $data['og_uid_titles']['uid'] = array(
        'title' => t('Group Titles: Membership'),
        'help' => t('The user identifier'),
        'relationship' => array(
            'handler' => 'views_handler_relationship',
            'base' => 'og_uid',
            'base field' => 'uid',
            'relationship field' => 'uid',
            'label' => t('Group Member'),
            'extra' => '{og_uid_og_uid_titles}.nid = {og_uid_titles}.nid', // this is a hack?
        ),
    );

	$data['og_uid_titles']['weight'] = array(
		'title' => t('Weight'),
		'help' => t('The weight of a group title.'),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);

    $data['og_uid_titles']['nid'] = array(
        'title' => t('Group Titles: Group id'),
        'help' => t('The group id of the group'),
        'relationship' => array(
            'handler' => 'views_handler_relationship',
            'base' => 'node',
            'relationship field' => 'nid',
            'label' => t('Group node'),
        ),
        'argument' => array(
            'title' => t('Group node'),
            'handler' => 'og_views_handler_argument_og_uid_nid',
            'help' => t('This is the same argument as og_uid table.'),
        ),
    );

	return $data;

 } // function og_titles_views_data

 /**
  * og_titles_views_handlers
  */
 function og_titles_views_handlers()
 {
     return array(
        'info' => array(
            'path' => drupal_get_path('module', 'og_titles') . '/includes'
         ),
         'handlers' => array(
            'views_handler_field_og_title' => array(
                'parent' => 'views_handler_field',
            ),
         ),
     );

 } // function og_titles_views_handlers
