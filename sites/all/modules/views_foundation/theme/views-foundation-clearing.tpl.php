<?php
/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * Available variables:
 * - $title : The title of this group of rows.  May be empty.
 * - $fields: The output for the images, thumbnails and captions.
 * - $th: CSS class for thumbnail border.
 *
 * @ingroup views_templates
 */
?>
<div class="row <?php print $classes; ?>">
  <div class="twelve columns">
    <?php if (!empty($title)) : ?>
      <h3><?php print $title; ?></h3>
    <?php endif; ?>
    <ul class="<?php print $class['grid']; ?>" data-clearing>
      <?php foreach ($fields as $field): ?>
        <li><a class="<?php print $class['link']; ?>" href="<?php print $field['image'] ?>">
            <img<?php if (!empty($field['caption'])) : ?> data-caption="<?php print $field['caption']; ?>"<?php endif; ?> src="<?php print $field['thumbnail']; ?>"></a>
        </li>
      <?php endforeach; ?>
    </ul>
  </div>
</div>
