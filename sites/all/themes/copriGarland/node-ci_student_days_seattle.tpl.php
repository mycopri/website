<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $

$student = user_load($node->uid);
$student_name = $student->profile_first_name . ' ' . $student->profile_last_name;
$graduation = $field_graduation[0]['view'];
$school = $field_school_writein[0]['value'];
$major = $field_major_writein[0]['value'];
?>


<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php
//dsm($links); 
print '<div class="student-name-information">';
$picture  = '<div class="picture" style="float:left; margin-right:10px;">';
$picture .= '<img src="/' . $student->picture . '" alt="' . $student_name . '&#039;s picture" title="' . $student_name . '&#039;s picture" />';
$picture .= '</div>'; 
print $picture;
print '<p>' . $student_name . '<br/>';
print $school . '<br/>';
print $major . '<br/>';
print $graduation . ' Expected graduation' . '<br/>';
print '</p>';
print '</div>';
print '<div class="clear-block"></div>';

// Now produce the narrative based on provided data.
//print 
?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php //print $submitted; ?></span>
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