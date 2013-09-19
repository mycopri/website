<?php
/**
 * @file
 * View template to display a list of rows.
 *
 * Available variables:
 * - $view: The view object.
 * - $rows: The output for the rows.
 * - $fields: Array contain the output for the rows with classes.
 * - $title: The title of this group of rows. May be empty.
 * - $options: The CSS class for vertical navigation.
 *
 * @ingroup views_templates
 */
?>
<div class="row">
  <div class="twelve columns">
    <?php if (!empty($title)) : ?>
      <h3><?php print $title; ?></h3>
    <?php endif; ?>
    <ul class="nav-bar <?php if ($options['nav_vertical']) : ?> vertical <?php endif; ?>">
      <?php foreach ($fields as $id => $field): ?>
      <li class="has-flyout <?php print $field['is_active']; ?>">
        <?php print $field['link']; ?>
        <?php print $field['wrapper_prefix']; ?>
        <?php print $field['content']; ?>
        <?php print $field['wrapper_suffix']; ?>
      </li>
      <?php endforeach; ?>
    </ul>
  </div>
</div>
