<?php

/**
 * @file cms-program-abstract.tpl.php
 *
 * Theme implementation to display a conference program.
 *
 * @see template_preprocess()
 * @see template_preprocess_cms_program_abstract()
 */
?>
<?php if ($teaser) { ?>
<div class = "abstract">
	<h1 class="abstract-title"><?php print $link_to_full_abstract; ?></h1>
</div>
<?php } else { ?>
<div id="<?php print $anchor_id ?>" class = "abstract">
	<h1 class="abstract-title"><?php print $title; ?><span class="go-top"><?php print $go_top_link; ?></span></h1>
	<p class="abstract-body"><?php print $body; print $rate;?></p>
</div>
<?php } ?>