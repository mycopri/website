<?php

/**
 * Instructions for the og_mail template forms. I use a template to make it easier to modify.
 */
?>

<div>
<p>Use this form to define a message to members of a group when new content is posted.<br/>
You can use the following tokens in the templates:
</p>

<ul>
  <li>@recipient:      The name of the user that this email is being sent to.</li>
  <li>@new_posts:      A list of new group posts that this user is a member of.</li>
  <li>@updated_posts:  A list of updated group posts that this user is a member of.</li>
  <li>@comments:       A list of comments in groups that this user is a member of.</li>
</ul>
</div>
