<?php

/**
 * @file cms-program-heading.tpl.php
 *
 * Theme implementation to display a conference program.
 *
 * @see template_preprocess()
 * @see template_preprocess_cms_program_heading()
 */
//dpm(get_defined_vars());
?>
<?php if ($teaser) { ?>
<h2 class="abstract-heading"><?php print $heading_title; ?></h2>
<?php } else { ?>
	<h2 id="<?php print $anchor_id ?>" class="abstract-heading"><?php print $heading_title; ?></h2>
<?php } ?>