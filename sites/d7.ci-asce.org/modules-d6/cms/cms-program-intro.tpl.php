<?php

/**
 * @file cms-program-intro.tpl.php
 *
 * Theme implementation to display a conference program.
 *
 * @see template_preprocess()
 * @see template_preprocess_cms_program_intro()
 */
?>
<?php
$register = l('here', 'user/register', array('query' => drupal_get_destination()));
$login = l('here', 'user', array('query' => drupal_get_destination()));
?>
<p id="top" class="abstract-intro">
The conference is still accepting proposals for papers and presentations. Below is a list of the 
abstract titles that have been submitted. To view the full abstract, just click on
the title and you will be taken to the full abstract.	
</p>

<?php if (!user_is_logged_in()): ?>
<p>
If you wish to rate these abstracts to help us decide which sessions to accept please register 
<?php print $register ?> or if you already have an account login <?php print $login ?>.
</p>
<?php endif; ?>