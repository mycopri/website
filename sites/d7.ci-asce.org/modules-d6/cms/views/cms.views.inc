<?php

/**
 * Implementation of hook_views_data().
 */
function cms_views_data() {
	$data = array();

	$data['cms_abstract']['table']['group'] = 'Abstract';
	$data['cms_abstract']['table']['join'] = array(
		'node' => array(
			'left_field' => 'nid',
			'field' => 'abstract_nid',
		),
	);
	// Fields
	$data['cms_abstract']['abstract_nid'] = array(
		'title' => t('abstract_nid'),
		'help' => t('The node ID of the abstract.'),
		'field' => array(
			'handler' => 'views_handler_field_node',
			'click sortable' => TRUE,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);
	$data['cms_abstract']['full_paper'] = array(
		'title' => t('full_paper'),
		'help' => t('Is author submitting full paper.'),
		'field' => array(
			'handler' => 'views_handler_field_node',
			'click sortable' => TRUE,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);		
	// cms_abstract_authors table
	$data['cms_abstract_authors']['table']['group'] = 'CMS';
	$data['cms_abstract_authors']['table']['join']['node'] = array(
		'left_table' => 'cms_abstract',
		'left_field' => 'abstract_nid',
		'field' => 'abstract_nid',
	);
	//fields
	$data['cms_abstract_authors']['author_id'] = array(
		'title' => t('author_id'),
		'help' => t('The author ID of the abstract.'),
		'field' => array(
			'handler' => 'views_handler_field_node',
			'click sortable' => TRUE,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);

	// cms_authors table
	$data['cms_authors']['table']['join']['node'] = array(
		'left_table' => 'cms_abstract_authors',
		'left_field' => 'author_id',
		'field' => 'aid',
	);
	// $data['cms_authors']['table']['join']['cms_abstract_authors'] = array(
		// 'left_field' => 'author_id',
		// 'field' => 'aid',
	// );
	$fields = array('first', 'last', 'mail', 'organization', 'city', 'state', 'phone', 'department');
	foreach ($fields as $field) {
		$data['cms_authors'][$field] = array(
			'title' => t($field),
			'help' => t('{' . $field . '} field in cms_authors table.'),
			'field' => array(
				'handler' => 'views_handler_field_node',
				'click sortable' => TRUE,
			),
			'sort' => array(
				'handler' => 'views_handler_sort',
			),
		);		
	}
	
	// topics table
	$data['cms_topic']['table']['group'] = t('Abstract Topic');
	$data['cms_topic']['table']['join']['node'] = array(
		'left_table' => 'cms_abstract',
		'left_field' => 'topic',
		'field' => 'topic_id',
	);
	// Fields
	$data['cms_topic']['title'] = array(
		'title' => t('Main Topic'),
		'help' => t('The main topic of the abstract.'),
		'field' => array(
			'handler' => 'views_handler_field_node',
			'click sortable' => TRUE,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);

	// sub topics table
	$data['cms_subtopic']['table']['group'] = t('Abstract Topic');
	$data['cms_subtopic']['table']['join']['node'] = array(
		'left_table' => 'cms_abstract',
		'left_field' => 'subtopic',
		'field' => 'sub_id',
	);
	// Fields
	$data['cms_subtopic']['title'] = array(
		'title' => t('Subtopic'),
		'help' => t('The subtopic of the abstract.'),
		'field' => array(
			'handler' => 'views_handler_field_node',
			'click sortable' => TRUE,
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);
	
	//dpm($data);
	return $data;
}