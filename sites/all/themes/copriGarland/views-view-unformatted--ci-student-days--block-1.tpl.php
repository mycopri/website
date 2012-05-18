<?php
// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
//dsm(get_defined_vars());
//dsm($view);

// We are going to completely override the rows specific to the view, which means if
// fields are added in the view, they will not appear on the page
// Everything can be found in $view->result
$results = $view->result;
$rows = array();
global $user;
foreach ($results as $key => $result) {
	$pane  = '<div class="panel-pane pane-node">';
	$pane .= '  <h2 class="pane-title">' . $result->node_title . '</h2>';
	//<div class="views-admin-links views-hide">
  //    <ul class="links"><li class="0 first"><a href="/admin/build/views/edit/ci_student_days?destination=student-days#views-tab-block_1">Edit</a></li>
  //<li class="1"><a href="/admin/build/views/export/ci_student_days">Export</a></li>
  //<li class="2 last"><a href="/admin/build/views/clone/ci_student_days">Clone</a></li>
  //</ul>    </div>
	if (user_access('administer nodes')) {
		$pane .= '  <div class="admin-links panel-hide">';
	  $pane .= '    <ul class="links">';
	  $pane .= '      <li class="update first">';
	  $pane .=          l('Edit node', 'node/' . $result->nid . '/edit', array('query' => drupal_get_destination()));
	  $pane .= '      </li>';
    $pane .= '      <li class="update last">';
    $pane .=          l('Delete', 'node/' . $result->nid . '/delete', array('query' => drupal_get_destination()));
    $pane .= '      </li>';
	  $pane .= '    </ul>';    
	  $pane .= '  </div>';
	}
  $pane .= '  <div class="pane-content">';
	$pane .= '    <div class="node-"' . $result->nid . '" class="node">';
	$pane .= '      <span class="submitted">' . format_date($result->node_changed) . '</span>';
	$pane .= '      <div class="content clear-block">' . $result->node_revisions_body . '</div>';	
	$pane .= '    </div>';
	$pane .= '  </div>';
	$pane .= '</div>';
	$rows[] = $pane;
}

?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
  <div class="<?php print $classes[$id]; ?>">
    <?php print $row; ?>
  </div>
<?php endforeach; ?>
