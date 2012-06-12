<?php
/**
 * @file ci-fb-login-button.tpl.php
 *
 * Theme implementation to display the facebook login button.
 *
 */
//dpm(func_get_args());
?>
<script>
	$(document).ready(function() {
    $.getScript("http://platform.linkedin.com/in.js?async=true", function success() {
        IN.init({
        	api_key: <?php print '"' . $linkedin_api_key . '"'?>,
        	authorize: false,
          onLoad: "onLinkedInLoad"
        });
    });
    
	  // create the dialogs
		var email = $("#email");
		var _email = '';
		$("#ci-li-dialog-check-for-account").dialog(
			{
				modal: true,
				autoOpen: false,
				buttons: {
	  			OK: function() {
	  				$(this).dialog("close");
			      uri = 'ci-li/js/check-email';
			      $.getJSON(encodeURI(uri), email, function(data) {
				    	if (data.found === true) {
				    		window.location = '//'+window.location.hostname;
				    	}
				    	else {
				    		_email = email.val();
				    		$("#ci-li-dialog-register").dialog('open');
				    	}
				    });
	  			},
				}
			}
		);
		$("#ci-li-dialog-register").dialog({
			modal: true, 
	  	autoOpen: false,
	  	buttons: {
	  		'Connect Exsiting Account': function() {
	  			$(this).dialog("close");
	  			IN.API.Profile("me").result(function(profiles) {
						me = profiles.values[0];
						query_items = [
								'first_name=' + me.first-name,
								'middle_name=' + me.middle-name,
								'last_name=' + me.last-name,
								'id=' + me.id
						];
						destination = encodeURIComponent('ci-li/activate?email='+_email);
						destination = destination+'&'+encodeURIComponent('id='+me.id);
						destination = 'destination='+destination;
						data = 'data='+'{"email": "'+_email+'", "id": "'+me.id+'"}';
						uri = '//'+window.location.hostname+'/user?destination=ci-li/activate?' + data;
						uri = encodeURI(uri);
						window.location = uri;
					});
	  		},
	  		'Create a New Account': function() {
	  			$(this).dialog("close");
					IN.API.Profile("me").result(function(profiles) {
						me = profiles.values[0];
						query_items = [
								'first_name=' + me.first-name,
								'middle_name=' + me.middle-name,
								'last_name=' + me.last-name,
								'id=' + me.id
						];
						query = query_items.join('&');
						uri = '//'+window.location.hostname+'/ci-li/register?'+query;
						window.location = encodeURI(uri);
					});	  			
	  		}
	  	}	
		});
    // bind 'myForm' and provide a simple callback function 
    $('#checkEmail').ajaxForm(function() { 
        alert("Thank you for your comment!"); 
    }); 	
	});
	function onLinkedInLoad() {
		IN.Event.on(IN, "auth", onLinkedInAuth);
	}
	function onLinkedInAuth() {
		if (IN.User.isAuthorized()) {
			// Alright the user logged in successfully to LinkedIn and
			// granted the site access, now we got to check to see if
			// they are in the system already. We do this by checking to
			// see if their profile id exists in the system.
			IN.API.Profile("me").result(checkForID);
		}
	}
	function checkForID(profiles) {
    member = profiles.values[0];
    uri = 'ci-li/js/check-id';
    // uri = uri + '?id=' + member.id;
    $.getJSON(encodeURI(uri), member, function(data) {
    	if (data.found === true) {
    		window.location = '//'+window.location.hostname;
    	}
    	else {
    		$("#ci-li-dialog-check-for-account").dialog('open');
    	}
    });
  }
</script>
<script type="IN/Login">
</script>

<div id="ci-li-dialog-check-for-account" title="">
	<p>	It looks like you have never logged in to the site using your LinkedIn account.
			In the next couple of steps we will try to determine if yo have aexisting
			account on the site already. If you do, we can connect the existing account
			to your LinkedIn account.
	</p>
	<p> Otherwise, a new account will automatically be created for you.</p>
	<p> First thing we need to get started is the email you use for your LinkedIn account.
			Please enter it below and click OK.	
	</p>
	<form id="checkEmail" action="ci-li/js/check-email" method="post"> 
	    <input id="email" type="text" name="email" /> 
	</form>	
</div>
<div id="ci-li-dialog-register" title="">
	<p>	We did not find the email you use for your LinkedIn account in our 
			our system; however, you could have created an account using a different email.
			If so, we can connect the existing account to your LinkedIn account, just click
			"Connect existing account" below or click "Create new account" to create a new one
			based on your LinkedIn account.
	</p>
</div>
