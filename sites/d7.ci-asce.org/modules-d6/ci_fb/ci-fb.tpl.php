<?php
/**
 * @file ci-fb-login-button.tpl.php
 *
 * Theme implementation to display the facebook login button.
 *
 */
//dpm(func_get_args());
?>

<div id="fb-root"></div>
<script>
// Load the SDK Asynchronously
	(function(d){
	   var js, id = 'facebook-jssdk', ref = d.getElementsByTagName('script')[0];
	   if (d.getElementById(id)) {return;}
	   js = d.createElement('script'); js.id = id; js.async = true;
	   js.src = "//connect.facebook.net/en_US/all.js";
	   ref.parentNode.insertBefore(js, ref);
	 }(document));
	
	window.fbAsyncInit = function() {
	  FB.init({
	    appId      : '<?php print $fb_app_id ?>', // App ID
	    channelUrl : '//'+window.location.hostname+'/channel.php', // Channel File
	    status     : true, // check login status
	    cookie     : true, // enable cookies to allow the server to access the session
	    xfbml      : true  // parse XFBML
	  });
	
	  // respond to clicks on the login and logout links
	  $('#fb-login').bind('click', function(response){
	  	// $('.ui-widget-overlay').css('display', 'inline');
	  	FB.login(drupal_fb_login, {scope: 'email,user_likes,friends_likes'});    			    		
	  });
	  
	  $('.ui-widget-overlay').css('display', 'none');
	
	  // create the dialogs
	  var options = {
	  	modal: true, 
	  	autoOpen: false,
	  	buttons: {
	  		Yes: function() {
	  			$(this).dialog("close");
	  			$("#ci-fb-dialog-yes").dialog("open");
	  		},
	  		No: function() {
	  			$(this).dialog("close");
					FB.api('/me', function(me) {
						if(me.email) {
							query_items = [
								'email=' + me.email,
								'first_name=' + me.first_name,
								'middle_name=' + me.middle_name,
								'last_name=' + me.last_name,
								'id=' + me.id
							];
							query = query_items.join('&');
							uri = '//'+window.location.hostname+'/ci-fb/register?'+query;
							window.location = encodeURI(uri);
						}
					});	  			
	  		}
	  	}
	  };
		$("#ci-fb-dialog-register").dialog(options);
		$("#ci-fb-dialog-yes").dialog(
			{
				modal: true,
				autoOpen: false,
				buttons: {
					Ok: function() {
						FB.api('/me', function(me) {
							if(me.email) {
								uri = '//'+window.location.hostname+'/user?destination=ci-fb/activate?email='+me.email+'&id='+me.id;
								window.location = encodeURI(uri);
							}
						});
					}
				}
			}
		);
		
		function drupal_fb_login(response) {
	    FB.api('/me', function(me){
	      if (me.email) {
		      uri = 'ci-fb/js/check-email';
		      uri = uri + '?email=' + me.email;
			    $.getJSON(encodeURI(uri), me, function(data) {
			    	if (data.found === true) {
			    		window.location = '//'+window.location.hostname;
			    	}
			    	else {
			    		$("#ci-fb-dialog-register").dialog('open');
			    	}
			    });
	      }			
			});
		}
	};
</script>

<a href="#" class="fb_button fb_button_medium" id="fb-login">
	<span class="fb_button_text">Login with Facebook</span>
</a>
<div class="ui-widget-overlay" style="display:none; margin-top:-200px; width: 1290px; height: 2000px; z-index: 1001;"></div>
<div id="ci-fb-dialog-register" title="">
	<p>Have you ever created an account on this site?</p>
</div>

<div id="ci-fb-dialog-yes" title="">
	<p>
		Great! <br/> Since this is your first time logging in with Facebook
		you will need to log in regularly first by clicking on the OK
		button below. <br/> After logging in, the Login with Facebook feature
		will be activated and the next time you login will be easy!
	</p>
</div>
