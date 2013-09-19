<?php
/**
 * @file
 * View template to display a list of rows.
 *
 * Available variables:
 * - $view: The view object.
 * - $rows: The output for the rows.
 * - $captions: The output for the images captions.
 * - $title: The title of this group of rows. May be empty.
 * - $orbit_id: The unique id for this view.
 *
 * @ingroup views_templates
 */
?>
<div class="row">
  <div class="twelve columns">
    <?php if (!empty($title)) : ?>
      <h3><?php print $title; ?></h3>
    <?php endif; ?>
    <div id="views-orbit-<?php print $orbit_id; ?>">
      <?php foreach ($rows as $id => $row): ?>
        <?php if (!empty($captions)) : ?>
          <div data-caption="#caption-<?php print $orbit_id . $id ?>">
            <?php print $row; ?>
          </div>
          <span class="orbit-caption" id="caption-<?php print $orbit_id . $id; ?>">
            <?php print $captions[$id]; ?>
          </span>

        <?php else : ?>
          <div>
            <?php print $row; ?>
          </div>
        <?php endif; ?>
      <?php endforeach; ?>
    </div>
  </div>
</div>
