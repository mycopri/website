<?php

/**
 * @file user-picture.tpl.php
 * Default theme implementation to present an picture configured for the
 * user's account.
 *
 * Available variables:
 * - $picture: Image set by the user or the site's default. Will be linked
 *   depending on the viewer's permission to view the users profile page.
 * - $account: Array of account information. Potentially unsafe. Be sure to
 *   check_plain() before use.
 *
 * @see template_preprocess_ci_student_days_picture()
 */
//dpm(get_defined_vars());
?>
<div class="ci-glossary-term">
	<?php print $term->name; ?>
</div>
<div class="ci-glossary-definition">
	<?php print $term->description; ?>
</div>