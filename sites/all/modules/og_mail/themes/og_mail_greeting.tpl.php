<?php
/**
 * @file
 * Default theme implementation for og content activity notification email.
 */
?>

<div class="messages warning">
<?php
  $mess = "We are in the process of testing our automated email " .
          "notifications system. We apologize for any inconveniences. " .
          "Please address any issues with this email to " .
          '<a href=\"mailto:admin@lc.livingshorelines.mycopri.org?' . 
          'Subject=Notifications%20testing\">admin</a>';
  print $mess;
?>
  
</div>
<?php print "<div>Hello {$account->name},</div>" ?>