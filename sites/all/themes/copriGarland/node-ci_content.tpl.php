<?php
// $Id$
//dsm(get_defined_vars());
foreach($og_groups as $group_nid) {
	$group_node = node_load($group_nid);
	if ($group_node->type == 'ci_student_team') {
		$override = TRUE;
	}
}
if ($override) {
	$student = asce_student_get_student_data($uid);
	$node_url = '/node/' . $nid;
	$submitted = format_date($created) . ' - ';
	$submitted .= l($student->full_name, 'priv-messages/new/' . $student->uid, array('query' => drupal_get_destination()));
	$post = node_load($nid);
	$output = '<div id="node-' . $nid .'">';
	$output .= $student->picture;
	$output .= '<h2><a href="' . $node_url . '" title="' . $title . '">' . $title . '</a></h2>';
	$output .= '<span class="submitted">' . $submitted . '</span>';
	$output .= '<div class="content clear-block">';
	$output .= check_markup($post->body, 2);
	$output .= '</div>';
	$output .= $links;
	print $output;
	return;
}
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>

  <div class="content clear-block">
    <?php print $content ?>
  </div>

  <div class="clear-block">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>
