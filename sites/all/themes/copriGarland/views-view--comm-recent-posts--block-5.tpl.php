<?php
// $Id: views-view.tpl.php,v 1.12.2.3 2010/03/25 20:25:23 merlinofchaos Exp $
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */
?>

<?php 
// Get the max image height
$node = node_load(arg(1));
//dsm($node);
if (basename($template_file) == 'views-view--comm-recent-posts--block-5.tpl.php') {
	$sql = "SELECT node.nid AS nid,
            node.title AS node_title,
            node.type AS node_type,
            node.sticky AS node_sticky,
            DATE_FORMAT((FROM_UNIXTIME(node.created) + INTERVAL -18000 SECOND), '%Y%m%d%H%i') AS node_created_minute
            FROM node node 
            INNER JOIN og_ancestry og_ancestry ON node.nid = og_ancestry.nid
            WHERE (node.status <> 0) AND (node.type in ('image')) AND (og_ancestry.group_nid = $node->nid)
            ORDER BY node_sticky DESC, node_created_minute DESC";
	$results = db_query($sql);
	$max_height = 0;
	while ($image = db_fetch_object($results)) {;
	  $file = node_load($image->nid)->images['preview'];
	  $info = image_get_info($file);
	  if ($info['height'] > $max_height) {
	  	$max_height = $info['height'];
	  }
  }
  $max_height = $max_height + 50;
} 
$style = 'height:' . $max_height . 'px;';
?>
<div style="<?php print $style; ?>" class="<?php print $classes; ?>">
  <?php if ($admin_links): ?>
    <div class="views-admin-links views-hide">
      <?php print $admin_links; ?>
    </div>
  <?php endif; ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>

  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div> <?php /* class view */ ?>
