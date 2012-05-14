<?php
// $Id: block.tpl.php 7123 2010-04-15 18:31:15Z jeremy $
?>

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> <?php print $block_zebra; ?> <?php print $position; ?> <?php print $skinr; ?>">
  <div class="inner">
    <?php if (isset($edit_links)): ?>
    <?php print $edit_links; ?>
    <?php endif; ?>
    <div class="icon-hand pngfix"></div>
    <div class="content-wrapper">
      <?php if ($block->subject): ?>
      <div class="block-icon pngfix"></div>
      <h2 class="title block-title"><?php print $block->subject ?></h2>
      <?php endif;?>
      <div class="content clearfix">
        <?php print $block->content ?>
      </div>
    </div><!-- /content-wrapper -->
  </div><!-- /block-inner -->
</div><!-- /block -->
