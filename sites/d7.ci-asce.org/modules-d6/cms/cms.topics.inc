<?php

/**
 * Find the depth of a topic's children relative to its depth.
 *
 * For example, if the topic has a depth of 2, and the maximum of any child in
 * the topic tree is 5, the relative depth is 3.
 *
 * @param $topic
 *   An array representing a topic.
 * @return
 *   The relative depth, or zero.
 *
 */
function cms_topic_children_relative_depth($topic) {
  $i = 1;
  $match = '';
  $args[] = $topic['tlid'];
  $p = 'p1';
  while ($i <= MENU_MAX_DEPTH && $topic[$p]) {
    $match .= " AND $p = %d";
    $args[] = $topic[$p];
    $p = 'p'. ++$i;
  }

  $max_depth = db_result(db_query_range("SELECT depth FROM {cms_topics} WHERE tlid = '%s'". $match ." ORDER BY depth DESC", $args, 0, 1));

  return ($max_depth > $topic['depth']) ? $max_depth - $topic['depth'] : 0;
}

/**
 * Helper function that sets the p1..p9 values for a menu link being saved.
 */
function _cms_topic_parents_set(&$topic, $parent) {
  $i = 1;
  while ($i < $topic['depth']) {
    $p = 'p'. $i++;
    $topic[$p] = $parent[$p];
  }
  $p = 'p'. $i++;
  // The parent (p1 - p9) corresponding to the depth always equals the tlid.
  $topic[$p] = $topic['tlid'];
  while ($i <= MENU_MAX_DEPTH) {
    $p = 'p'. $i++;
    $topic[$p] = 0;
  }
}

/**
 * Update the children of a menu link that's being moved.
 *
 * The menu name, parents (p1 - p6), and depth are updated for all children of
 * the link, and the has_children status of the previous parent is updated.
 */
function _cms_topic_move_children($topic, $existing_item) {

  $args[] = $topic['tlid'];
  $set[] = "menu_name = '%s'";

  $i = 1;
  while ($i <= $topic['depth']) {
    $p = 'p'. $i++;
    $set[] = "$p = %d";
    $args[] = $topic[$p];
  }
  $j = $existing_item['depth'] + 1;
  while ($i <= MENU_MAX_DEPTH && $j <= MENU_MAX_DEPTH) {
    $set[] = 'p'. $i++ .' = p'. $j++;
  }
  while ($i <= MENU_MAX_DEPTH) {
    $set[] = 'p'. $i++ .' = 0';
  }

  $shift = $topic['depth'] - $existing_item['depth'];
  if ($shift < 0) {
    $args[] = -$shift;
    $set[] = 'depth = depth - %d';
  }
  elseif ($shift > 0) {
    // The order of $set must be reversed so the new values don't overwrite the
    // old ones before they can be used because "Single-table UPDATE
    // assignments are generally evaluated from left to right"
    // see: http://dev.mysql.com/doc/refman/5.0/en/update.html
    $set = array_reverse($set);
    $args = array_reverse($args);

    $args[] = $shift;
    $set[] = 'depth = depth + %d';
  }
  $where[] = "menu_name = '%s'";
  $args[] = $existing_item['tlid'];
  $p = 'p1';
  for ($i = 1; $i <= MENU_MAX_DEPTH && $existing_item[$p]; $p = 'p'. ++$i) {
    $where[] = "$p = %d";
    $args[] = $existing_item[$p];
  }

  db_query("UPDATE {cms_topics} SET ". implode(', ', $set) ." WHERE ". implode(' AND ', $where), $args);
  // Check the has_children status of the parent, while excluding this item.
  _cms_topic_update_parental_status($existing_item, TRUE);
}

/**
 * Check and update the has_children status for the parent of a link.
 */
function _cms_topic_update_parental_status($topic, $exclude = FALSE) {
  // If plid == 0, there is nothing to update.
  if ($topic['plid']) {
    // We may want to exclude the passed link as a possible child.
    $where = $exclude ? " AND tlid != %d" : '';
    // Check if at least one visible child exists in the table.
    $parent_has_children = (bool)db_result(db_query_range("SELECT tlid FROM {cms_topics} WHERE eid = %d AND plid = %d " . $where, $topic['eid'], $topic['plid'], $topic['tlid'], 0, 1));
    db_query("UPDATE {cms_topics} SET has_children = %d WHERE tlid = %d", $parent_has_children, $topic['plid']);
  }
}

/**
 * Get the topic structure representing a conference topic tree.
 *
 * Since this can be the full tree including hidden items, the data returned
 * may be used for generating an an admin interface or a select.
 *
 * @param $eid
 *   The unique event id for which the topics reside
 * @param $topic
 *   A fully loaded topic object, or NULL.
 * @return
 *   An tree of topics in an array, in the order they should be rendered.
 */
function cms_topic_tree_all_data($eid, $topic = NULL) {
  static $tree = array();
  // Use $tlid as a flag for whether the data being loaded is for the whole tree.
  $tlid = isset($topic['tlid']) ? $topic['tlid'] : 0;
	
  // Build and run the query, and build the tree.
  if ($tlid) {
    // The tree is for a single topic, so we need to match the values in its
    // p columns and 0 (the top level) with the plid values of other links.
    $args = array(0);
    for ($i = 1; $i < MENU_MAX_DEPTH; $i++) {
      $args[] = $topic["p$i"];
    }
    $args = array_unique($args);
    $placeholders = implode(', ', array_fill(0, count($args), '%d'));
    $where = ' AND cms.plid IN ('. $placeholders .')';
    $parents = $args;
    $parents[] = $topic['tlid'];
  }
  else {
    // Get all topics in this conference.
    $where = '';
    $args = array();
    $parents = array();
  }
  array_unshift($args, $eid);
  // Select the links from the table, and recursively build the tree.  We
  // LEFT JOIN since there is no match in {menu_router} for an external
  // link.
  $sql = "SELECT cms.*
    FROM {cms_topics} cms
    WHERE cms.eid = %d". $where ."
      ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $data['tree'] = cms_topic_tree_data(db_query($sql, $args), $parents);
  
  return $data['tree'];
}

/**
 * Build the data representing a menu tree.
 *
 * @param $result
 *   The database result.
 * @param $parents
 *   An array of the plid values that represent the path from the current page
 *   to the root of the menu tree.
 * @param $depth
 *   The depth of the current menu tree.
 * @return
 *   See menu_tree_page_data for a description of the data structure.
 */
function cms_topic_tree_data($result = NULL, $parents = array(), $depth = 0) {
  list(, $tree) = _cms_topic_tree_data($result, $parents, $depth);
  return $tree;
}

/**
 * Recursive helper function to build the data representing a menu tree.
 *
 * The function is a bit complex because the rendering of an item depends on
 * the next menu item. So we are always rendering the element previously
 * processed not the current one.
 */
function _cms_topic_tree_data($result, $parents, $depth, $previous_element = '') {
  $remnant = NULL;
  $tree = array();
  while ($topic = db_fetch_array($result)) {
    // We need to determine if we're on the path to root so we can later build
    // the correct active trail and breadcrumb.
    $topic['in_active_trail'] = in_array($topic['tlid'], $parents);
    // The current item is the first in a new submenu.
    if ($topic['depth'] > $depth) {
      // _menu_tree returns an item and the menu tree structure.
      list($topic, $below) = _cms_topic_tree_data($result, $parents, $topic['depth'], $topic);
      if ($previous_element) {
        $tree[$previous_element['mlid']] = array(
          'link' => $previous_element,
          'below' => $below,
        );
      }
      else {
        $tree = $below;
      }
      // We need to fall back one level.
      if (!isset($topic) || $topic['depth'] < $depth) {
        return array($topic, $tree);
      }
      // This will be the link to be output in the next iteration.
      $previous_element = $topic;
    }
    // We are at the same depth, so we use the previous element.
    elseif ($topic['depth'] == $depth) {
      if ($previous_element) {
        // Only the first time.
        $tree[$previous_element['tlid']] = array(
          'topic' => $previous_element,
          'below' => FALSE,
        );
      }
      // This will be the link to be output in the next iteration.
      $previous_element = $topic;
    }
    // The submenu ended with the previous item, so pass back the current item.
    else {
      $remnant = $topic;
      break;
    }
  }
  if ($previous_element) {
    // We have one more link dangling.
    $tree[$previous_element['tlid']] = array(
      'topic' => $previous_element,
      'below' => FALSE,
    );
  }
  return array($remnant, $tree);
}