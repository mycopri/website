<?php
	    
function og_titles_views_default_views()
{
	$view = new view;
	$view->name = 'og_member_titles';
	$view->description = 'A simple member list of users, their groups, and their member titles.';
	$view->tag = 'og';
	$view->view_php = '';
	$view->base_table = 'users';
	$view->is_cacheable = FALSE;
	$view->api_version = 2;
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	$handler = $view->new_display('default', 'Defaults', 'default');
	$handler->override_option('relationships', array(
	  'uid' => array(
	    'label' => 'Group Member',
	    'required' => 1,
	    'id' => 'uid',
	    'table' => 'og_uid_titles',
	    'field' => 'uid',
	    'relationship' => 'none',
	  ),
	  'nid' => array(
	    'label' => 'Group node',
	    'required' => 0,
	    'id' => 'nid',
	    'table' => 'og_uid_titles',
	    'field' => 'nid',
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('fields', array(
	  'name' => array(
	    'label' => '',
	    'link_to_user' => 1,
	    'exclude' => 1,
	    'id' => 'name',
	    'table' => 'users',
	    'field' => 'name',
	    'relationship' => 'none',
	  ),
	  'title' => array(
	    'label' => '',
	    'alter' => array(
	      'alter_text' => 0,
	      'text' => '',
	      'make_link' => 0,
	      'path' => '',
	      'link_class' => '',
	      'alt' => '',
	      'prefix' => '',
	      'suffix' => '',
	      'help' => '',
	      'trim' => 0,
	      'max_length' => '',
	      'word_boundary' => 1,
	      'ellipsis' => 1,
	      'strip_tags' => 0,
	      'html' => 0,
	    ),
	    'exclude' => 0,
	    'id' => 'title',
	    'table' => 'og_uid_titles',
	    'field' => 'title',
	    'relationship' => 'none',
	    'og_titles_manage_link' => '0',
	  ),
	  'title_1' => array(
	    'label' => '',
	    'alter' => array(
	      'alter_text' => 0,
	      'text' => '',
	      'make_link' => 0,
	      'path' => '',
	      'link_class' => '',
	      'alt' => '',
	      'prefix' => '',
	      'suffix' => '',
	      'help' => '',
	      'trim' => 0,
	      'max_length' => '',
	      'word_boundary' => 1,
	      'ellipsis' => 1,
	      'strip_tags' => 0,
	      'html' => 0,
	    ),
	    'link_to_node' => 1,
	    'exclude' => 0,
	    'id' => 'title_1',
	    'table' => 'node',
	    'field' => 'title',
	    'relationship' => 'nid',
	  ),
	));
	$handler->override_option('sorts', array(
	  'is_admin' => array(
	    'order' => 'DESC',
	    'id' => 'is_admin',
	    'table' => 'og_uid',
	    'field' => 'is_admin',
	    'relationship' => 'uid',
	  ),
	  'is_active' => array(
	    'order' => 'DESC',
	    'id' => 'is_active',
	    'table' => 'og_uid',
	    'field' => 'is_active',
	    'relationship' => 'uid',
	  ),
	));
	$handler->override_option('filters', array(
	  'status' => array(
	    'operator' => '=',
	    'value' => '1',
	    'group' => '0',
	    'exposed' => FALSE,
	    'expose' => array(
	      'operator' => FALSE,
	      'label' => '',
	    ),
	    'id' => 'status',
	    'table' => 'users',
	    'field' => 'status',
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('access', array(
	  'type' => 'none',
	));
	$handler->override_option('cache', array(
	  'type' => 'none',
	));
	$handler->override_option('title', 'Global Member List');
	$handler->override_option('header', 'A list of all members with their group and group title.');
	$handler->override_option('header_format', '2');
	$handler->override_option('header_empty', 0);
	$handler->override_option('use_pager', '1');
	$handler->override_option('style_plugin', 'list');
	$handler->override_option('style_options', array(
	  'grouping' => 'name',
	  'type' => 'ul',
	));
	$handler = $view->new_display('block', 'Global Member List', 'block_1');
	$handler->override_option('block_description', '');
	$handler->override_option('block_caching', -1);

	$view = new view;
	$view->name = 'og_my_titles';
	$view->description = 'List user\'s subscribed groups, provide link to manage subscription and member title';
	$view->tag = 'og_titles';
	$view->view_php = '';
	$view->base_table = 'node';
	$view->is_cacheable = FALSE;
	$view->api_version = 2;
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	$handler = $view->new_display('default', 'Defaults', 'default');
	$handler->override_option('relationships', array(
	  'nid' => array(
	    'label' => 'Group node',
	    'required' => 0,
	    'id' => 'nid',
	    'table' => 'og_uid_titles',
	    'field' => 'nid',
	    'relationship' => 'none',
	  ),
	  'uid' => array(
	    'label' => 'Group Member',
	    'required' => 0,
	    'id' => 'uid',
	    'table' => 'og_uid_titles',
	    'field' => 'uid',
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('fields', array(
	  'title' => array(
	    'id' => 'title',
	    'table' => 'node',
	    'field' => 'title',
	    'label' => 'Group',
	    'relationship' => 'none',
	    'link_to_node' => 1,
	  ),
	  'name' => array(
	    'id' => 'name',
	    'table' => 'users',
	    'field' => 'name',
	    'label' => 'Manager',
	    'link_to_user' => 1,
	    'relationship' => 'none',
	  ),
	  'managelinkmy' => array(
	    'label' => 'Manage',
	    'alter' => array(
	      'alter_text' => 0,
	      'text' => '',
	      'make_link' => 0,
	      'path' => '',
	      'link_class' => '',
	      'alt' => '',
	      'prefix' => '',
	      'suffix' => '',
	      'help' => '',
	      'trim' => 0,
	      'max_length' => '',
	      'word_boundary' => 1,
	      'ellipsis' => 1,
	      'strip_tags' => 0,
	      'html' => 0,
	    ),
	    'exclude' => 0,
	    'id' => 'managelinkmy',
	    'table' => 'og_uid',
	    'field' => 'managelinkmy',
	    'relationship' => 'uid',
	  ),
	  'title_1' => array(
	    'label' => 'Member Title',
	    'alter' => array(
	      'alter_text' => 0,
	      'text' => '',
	      'make_link' => 0,
	      'path' => '',
	      'link_class' => '',
	      'alt' => '',
	      'prefix' => '',
	      'suffix' => '',
	      'help' => '',
	      'trim' => 0,
	      'max_length' => '',
	      'word_boundary' => 1,
	      'ellipsis' => 1,
	      'strip_tags' => 0,
	      'html' => 0,
	    ),
	    'exclude' => 0,
	    'id' => 'title_1',
	    'table' => 'og_uid_titles',
	    'field' => 'title',
	    'relationship' => 'none',
	    'og_titles_manage_link' => '1',
	  ),
	));
	$handler->override_option('sorts', array(
	  'title' => array(
	    'id' => 'title',
	    'table' => 'node',
	    'field' => 'title',
	  ),
	));
	$handler->override_option('filters', array(
	  'status' => array(
	    'id' => 'status',
	    'table' => 'node',
	    'field' => 'status',
	    'operator' => '=',
	    'value' => 1,
	    'group' => 0,
	    'exposed' => FALSE,
	    'expose' => array(
	      'operator' => FALSE,
	      'label' => '',
	    ),
	    'relationship' => 'none',
	    'expose_button' => array(
	      'button' => 'Expose',
	    ),
	  ),
	  'type_groups_all' => array(
	    'operator' => '=',
	    'value' => '',
	    'group' => 0,
	    'exposed' => FALSE,
	    'expose' => array(
	      'operator' => FALSE,
	      'label' => '',
	    ),
	    'id' => 'type_groups_all',
	    'table' => 'og',
	    'field' => 'type_groups_all',
	    'relationship' => 'nid',
	    'expose_button' => array(
	      'button' => 'Expose',
	    ),
	  ),
	  'uid' => array(
	    'operator' => '=',
	    'value' => '1',
	    'group' => 0,
	    'exposed' => FALSE,
	    'expose' => array(
	      'operator' => FALSE,
	      'label' => '',
	    ),
	    'id' => 'uid',
	    'table' => 'og_uid',
	    'field' => 'uid',
	    'relationship' => 'uid',
	    'expose_button' => array(
	      'button' => 'Expose',
	    ),
	    'override' => array(
	      'button' => 'Override',
	    ),
	  ),
	));
	$handler->override_option('access', array(
	  'type' => 'role',
	  'role' => array(
	    '2' => 2,
	    '1' => 0,
	  ),
	  'perm' => 'view advanced help topic',
	));
	$handler->override_option('cache', array(
	  'type' => 'none',
	));
	$handler->override_option('title', 'My groups');
	$handler->override_option('empty', 'Not a member of any groups.');
	$handler->override_option('empty_format', '1');
	$handler->override_option('items_per_page', 50);
	$handler->override_option('use_pager', '1');
	$handler->override_option('use_more', 1);
	$handler->override_option('style_plugin', 'table');
	$handler->override_option('style_options', array(
	  'override' => 1,
	  'order' => 'asc',
	  'columns' => array(
	    'title' => 'title',
	    'name' => 'name',
	    'managelink' => 'managelink',
	  ),
	  'info' => array(
	    'title' => array(
	      'sortable' => 1,
	      'separator' => '',
	    ),
	    'name' => array(
	      'sortable' => 1,
	      'separator' => '',
	    ),
	    'managelink' => array(
	      'separator' => '',
	    ),
	  ),
	  'default' => 'title',
	));
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->override_option('path', 'og/my');
	$handler->override_option('menu', array(
	  'type' => 'tab',
	  'title' => 'My groups',
	  'description' => '',
	  'weight' => '0',
	  'name' => 'navigation',
	));
	$handler->override_option('tab_options', array(
	  'type' => 'none',
	  'title' => '',
	  'description' => '',
	  'weight' => 0,
	));
	$handler = $view->new_display('block', 'Block', 'block_1');
	$handler->override_option('fields', array(
	  'title' => array(
	    'id' => 'title',
	    'table' => 'node',
	    'field' => 'title',
	    'label' => 'Group',
	    'relationship' => 'none',
	    'link_to_node' => 1,
	  ),
	  'post_count_new' => array(
	    'label' => 'New',
	    'set_precision' => FALSE,
	    'precision' => 0,
	    'decimal' => '.',
	    'separator' => ',',
	    'prefix' => '',
	    'suffix' => '',
	    'exclude' => 0,
	    'id' => 'post_count_new',
	    'table' => 'og',
	    'field' => 'post_count_new',
	    'override' => array(
	      'button' => 'Use default',
	    ),
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('sorts', array(
	  'title' => array(
	    'order' => 'ASC',
	    'id' => 'title',
	    'table' => 'node',
	    'field' => 'title',
	    'override' => array(
	      'button' => 'Use default',
	    ),
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('use_pager', '0');
	$handler->override_option('block_description', 'My groups');
	$handler->override_option('block_caching', -1);
		
	$views[$view->name] = $view;
	return $views;
} // function og_titles_views_default_views
