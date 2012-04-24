<?php
// $Id: leaves-background-block.tpl.php 6555 2010-02-24 20:21:15Z chris $
?>

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> <?php print $block_zebra; ?> <?php print $position; ?> <?php print $skinr; ?>">
  <div class="inner">
    <?php if (isset($edit_links)): ?>
    <?php print $edit_links; ?>
    <?php endif; ?>
    <div class="icon-hand"></div>
    <div class="content-wrapper">
      <?php if ($block->subject): ?>
      <div class="block-icon pngfix"></div>
      <h2 class="title block-title"><?php print $block->subject ?></h2>
      <?php endif;?>
      <div class="content">
        <?php print $block->content ?>
      </div>
    </div><!-- /content-wrapper -->
  </div><!-- /block-inner -->
</div><!-- /block -->
