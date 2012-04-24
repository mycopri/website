<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php //dsm(get_defined_vars());print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <div class="content clear-block">
<?php
// Add some code to insert picture, probably an easier way to do this, but don't have time
$node = node_load($nid);
$user = user_load($node->uid);
$show_pic = TRUE;
foreach($user->roles as $role) {
	if ($role == 'Staff') {
		$show_pic = FALSE;
	}
}
if ($show_pic) {
	$picture = '<div class="picture-box">' . theme('user_picture', user_load($uid));
	$picture .= '<div class="views-field-name">' . theme('username', $node) . '</div>';
	$picture .= '<div class="views-field-name">' . $field_event_author_title[0]['value'] . '</div></div>';
	print $picture;
}
?>
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
