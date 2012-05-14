<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>

<?php
$nid = arg(2);
$user = user_load($row->users_uid);
if ($row->nid == 3565) {
  //dsm($row);
  //dsm($user);
}

foreach ($fields as $id => $field) {
	if ($row->nid == 3565) {
		//dsm($id);
	}
	//drupal_set_message($id);
	$show_pic = TRUE;
	if ($id == 'picture' || $id == 'name' || $id == 'title_1') {
		foreach($user->roles as $role) {
			if ($role == 'Staff') {
				$show_pic = FALSE;
			}
		}
	}
  
	if ($id == 'picture' || $id == 'name' || $id == 'field_event_author_title_value') {
		if ($show_pic) {
			if (!empty($field->separator)) {
				print $field->separator;
			}
			print '<' . $field->inline_html . ' class="views-field-' . $field->class . '">';
			if ($field->label) {
				print '<label class="views-label-' . $field->class . '">';
				print $field->label .  '</label>';
			}
			print '<' . $field->element_type . ' class="field-content">' . $field->content . '</' . $field->element_type . '>';
			print '</' . $field->inline_html . '>';
		}
	}
	else {
    if (!empty($field->separator)) {
      print $field->separator;
    }
    print '<' . $field->inline_html . ' class="views-field-' . $field->class . '">';
    if ($field->label) {
      print '<label class="views-label-' . $field->class . '">';
      print $field->label .  '</label>';
    }
    print '<' . $field->element_type . ' class="field-content">' . $field->content . '</' . $field->element_type . '>';
    print '</' . $field->inline_html . '>';
	}
}
//dsm('------------------------');
return;
?>

<?php foreach ($fields as $id => $field): ?>
  <?php drupal_set_message($id); ?>
  <?php if (!empty($field->separator)): ?>
    <?php print $field->separator; ?>
  <?php endif; ?>

  <<?php print $field->inline_html;?> class="views-field-<?php print $field->class; ?>">
    <?php if ($field->label): ?>
      <label class="views-label-<?php print $field->class; ?>">
        <?php print $field->label; ?>:
      </label>
    <?php endif; ?>
      <?php
      // $field->element_type is either SPAN or DIV depending upon whether or not
      // the field is a 'block' element type or 'inline' element type.
      ?>
      <<?php print $field->element_type; ?> class="field-content"><?php print $field->content; ?></<?php print $field->element_type; ?>>
  </<?php print $field->inline_html;?>>
<?php endforeach; ?>