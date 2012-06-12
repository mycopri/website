<?php
function asce_demo_node_access_page($type = NULL) {
  $item = menu_get_item('node/add');
  $content = system_admin_menu_block($item);
  return theme('node_add_list', $content);
}

/**
 * Determine whether the current user may perform the given operation on the
 * specified node.
 *
 * @param $op
 *   The operation to be performed on the node. Possible values are:
 *   - "view"
 *   - "update"
 *   - "delete"
 *   - "create"
 * @param $node
 *   The node object (or node array) on which the operation is to be performed,
 *   or node type (e.g. 'forum') for "create" operation.
 * @param $account
 *   Optional, a user object representing the user for whom the operation is to
 *   be performed. Determines access for a user other than the current user.
 * @return
 *   TRUE if the operation may be performed, or FALSE otherwise.
 */
function asce_demo_node_access($op, $node, $account = NULL, $debug) {
  global $user;
  if (!$node || !in_array($op, array('view', 'update', 'delete', 'create'), TRUE)) {
    // If there was no node to check against, or the $op was not one of the
    // supported ones, we return access denied.
    return FALSE;
  }
  // Convert the node to an object if necessary:
  if ($op != 'create') {
    $node = (object)$node;
  }
  // If no user object is supplied, the access check is for the current user.
  if (empty($account)) {
    $account = $user;
  }
  // If the node is in a restricted format, disallow editing.
  if ($op == 'update' && !filter_access($node->format)) {
    return FALSE;
  }

  if (user_access('administer nodes', $account)) {
  	return TRUE;
  }
    
  if (!user_access('access content', $account)) {
    return FALSE;
  }
  
  // Can't use node_invoke(), because the access hook takes the $op parameter
  // before the $node parameter.
  $module = node_get_types('module', $node);
  if ($module == 'node') {
    $module = 'node_content'; // Avoid function name collisions.
  }
  $access = module_invoke($module, 'access', $op, $node, $account);
  if (!is_null($access)) {
    return $access;
  }
  
  // If the module did not override the access rights, use those set in the
  // node_access table.
  if ($op != 'create' && $node->nid && $node->status) {
    $grants = array();
    foreach (node_access_grants($op, $account) as $realm => $gids) {
      foreach ($gids as $gid) {
        $grants[] = "(gid = $gid AND realm = '$realm')";
      }
    }

    $grants_sql = '';
    if (count($grants)) {
      $grants_sql = 'AND ('. implode(' OR ', $grants) .')';
    }

    $sql = "SELECT 1 FROM {node_access} WHERE (nid = 0 OR nid = %d) $grants_sql AND grant_$op >= 1";
    $result = db_query_range($sql, $node->nid, 0, 1);
    return (bool) db_result($result);
  }

  // Let authors view their own nodes.
  if ($op == 'view' && $account->uid == $node->uid && $account->uid != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Provide a single block on the administration overview page.
 *
 * @param $item
 *   The menu item to be displayed.
 */
function asce_demo_system_admin_menu_block($item) {
  $content = array();
  if (!isset($item['mlid'])) {
    $item += db_fetch_array(db_query("SELECT mlid, menu_name FROM {menu_links} ml WHERE ml.router_path = '%s' AND module = 'system'", $item['path']));
  }
  $result = db_query("
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml
    LEFT JOIN {menu_router} m ON ml.router_path = m.path
    WHERE ml.plid = %d AND ml.menu_name = '%s' AND hidden = 0", $item['mlid'], $item['menu_name']);
  while ($item = db_fetch_array($result)) {
  	_asce_demo_menu_link_translate($item);
  	$message .= $item['title'] . ', ' . $item['router_path'] . ', ' . $item['access'] . '<br/>';
    if (!$item['access']) {
      continue;
    }
    // The link 'description' either derived from the hook_menu 'description' or
    // entered by the user via menu module is saved as the title attribute.
    if (!empty($item['localized_options']['attributes']['title'])) {
      $item['description'] = $item['localized_options']['attributes']['title'];
    }
    // Prepare for sorting as in function _menu_tree_check_access().
    // The weight is offset so it is always positive, with a uniform 5-digits.
    $content[(50000 + $item['weight']) .' '. drupal_strtolower($item['title']) .' '. $item['mlid']] = $item;
  }
  ksort($content);
  return $content;
}

function _asce_demo_menu_link_translate(&$item) {
	$debug = $item['router_path'] == 'node/add/blog-post-private' ? TRUE : FALSE;
  $item['options'] = unserialize($item['options']);
  $item['access_callback'] = 'asce_demo_node_access';
  if ($item['external']) {
    $item['access'] = 1;
    $map = array();
    $item['href'] = $item['link_path'];
    $item['title'] = $item['link_title'];
    $item['localized_options'] = $item['options'];
  }
  else {
    $map = explode('/', $item['link_path']);
    _menu_link_map_translate($map, $item['to_arg_functions']);
    $item['href'] = implode('/', $map);
  
    // Note - skip callbacks without real values for their arguments.
    if (strpos($item['href'], '%') !== FALSE) {
      $item['access'] = FALSE;
      return FALSE;
    }
    // menu_tree_check_access() may set this ahead of time for links to nodes.
    if (!isset($item['access'])) {
      if (!_menu_load_objects($item, $map)) {
        // An error occurred loading an object.
        $item['access'] = FALSE;
        return FALSE;
      }
      	_asce_demo_menu_check_access($item, $map, $debug);
    }
    // For performance, don't localize a link the user can't access.
    if ($item['access']) {
      _menu_item_localize($item, $map, TRUE);
    }
  }

  // Allow other customizations - e.g. adding a page-specific query string to the
  // options array. For performance reasons we only invoke this hook if the link
  // has the 'alter' flag set in the options array.
  if (!empty($item['options']['alter'])) {
    drupal_alter('translated_menu_link', $item, $map);
  }

  return $map;
}

function _asce_demo_menu_check_access(&$item, $map, $debug = TRUE) {
  // Determine access callback, which will decide whether or not the current
  // user has access to this path.
  $callback = empty($item['access_callback']) ? 0 : trim($item['access_callback']);
  // Check for a TRUE or FALSE value.
  if (is_numeric($callback)) {
    $item['access'] = (bool)$callback;
  }
  else {
    $arguments = menu_unserialize($item['access_arguments'], $map);
    // As call_user_func_array is quite slow and user_access is a very common
    // callback, it is worth making a special case for it.
    if ($callback == 'user_access') {
      $item['access'] = (count($arguments) == 1) ? user_access($arguments[0]) : user_access($arguments[0], $arguments[1]);
    }
    else {
      $item['access'] = call_user_func_array($callback, $arguments);
    }
  }
}
