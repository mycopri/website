<?php
/**
 * @file
 * Admin page callback file for the socialloginandsocialshare module.
 */

/**
 * Implements Social Login setting for the socialloginandsocialshare module.
 */
function socialloginandsocialshare_admin_settings($form, &$form_state) {
   drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/socialloginandsocialshare.admin.css');
  $group = 'setup';
  $get_socialloginandsocialshare = '';
  if (!variable_get('socialloginandsocialshare_apikey', FALSE)) {
    $get_socialloginandsocialshare = t('For create a Social Login account, please visit <a href="@get_socialloginandsocialshare" target="_blank">LoginRadius</a>', array('@get_socialloginandsocialshare' => 'http://www.Loginradius.com/'));
  }
   $thanks_text = t('To activate the plugin, you will need to first configure it (manage your desired social networks, etc.) from your LoginRadius account. If you do not have an account, click <a href="@loginradius" target="_blank">here</a> and create one for FREE!<br/>We also offer Social Plugins for <a href="@joomla" target="_blank">Joomla</a>, <a href="@wordpress" target="_blank">Wordpress</a>, <a href="@vbulletin" target="_blank">vBulletin</a>, <a href="@vanillaforum" target="_blank">VanillaForum</a>, <a href="@magento" target="_blank">Magento</a>, <a href="@oscommerce" target="_blank">OSCommerce</a>, <a href="@prestashop" target="_blank">Prestashop</a>, <a href="@xcart" target="_blank">X-Cart</a>, <a href="@zencart" target="_blank">Zen-Cart</a>, <a href="@dotnetnuke" target="_blank">DotNetNuke</a> and  <a href="@blogengine" target="_blank">BlogEngine</a> <br/><a href="@loginradius" target="_blank"><br/><input class="form-submit" type="button" value="Set up my FREE account!"></a> (<a href="@get_sociallogin" target="_blank">How to set up an account?</a>)', array('@loginradius' => 'http://www.Loginradius.com/', '@joomla' => 'http://www.loginradius.com/addons/joomla', '@wordpress' => 'http://www.loginradius.com/addons/wordpress', '@vbulletin' => 'http://www.loginradius.com/addons/vbulletin', '@vanillaforum' => 'http://www.loginradius.com/addons/vanilla', '@magento' => 'http://www.loginradius.com/addons/magento', '@oscommerce' => 'http://www.loginradius.com/addons/oscommerce', '@prestashop' => 'http://www.loginradius.com/addons/prestashop', '@xcart' => 'http://www.loginradius.com/addons/Xcart', '@zencart' => 'http://www.loginradius.com/addons/zencart', '@dotnetnuke' => 'http://www.loginradius.com/addons/dotnetnuke', '@blogengine' => 'http://www.loginradius.com/addons/blogengine', '@get_sociallogin' => 'http://support.loginradius.com/customer/portal/articles/593954'));
    $form['thanks_block'] = array(
     '#type' => 'fieldset',
    '#title' => t('Thank you for installing the LoginRadius Social Plugin!'),
    '#description' =>  $thanks_text,
  );
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('LoginRadius API Settings'),
    '#id' => 'setup',
    '#collapsible' => TRUE,
    '#description' =>  $get_socialloginandsocialshare,
  );
  $form[$group]['socialloginandsocialshare_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('socialloginandsocialshare_apikey', ''),
    '#description' => t('To activate the plugin, insert LoginRadius API Key ( <a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a> )', array('@get_socialloginandsocialshare' => 'http://support.loginradius.com/customer/portal/articles/677100-how-to-get-loginradius-api-key-and-secret')),
  );
  $form[$group]['socialloginandsocialshare_apisecret'] = array(
    '#type' => 'textfield',
    '#title' => t('API Secret'),
    '#default_value' => variable_get('socialloginandsocialshare_apisecret', ''),
    '#description' => t('To activate the plugin, insert LoginRadius API Secret ( <a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a> )', array('@get_socialloginandsocialshare' => 'http://support.loginradius.com/customer/portal/articles/677100-how-to-get-loginradius-api-key-and-secret')),
  );
  $form[$group]['socialloginandsocialshare_api_handler'] = array(
    '#type' => 'radios',
    '#title' => t('What API Connection Method do you prefer to enable API communication?'),
    '#default_value' => variable_get('socialloginandsocialshare_api_handler', 0),
    '#options' => array(
      0 => t('FSOCKOPEN (Default drupal_http_request() connection method. <b>Recommended!</b>)'),
      1 => t('CURL (Alternative API connection method, in case default method is not working properly.)'),
    ),
  );
  $form[$group]['socialloginandsocialshare_protocol_handler'] = array(
    '#type' => 'hidden',
    '#title' => t('Select API handler to get api response.'),
    '#default_value' => variable_get('socialloginandsocialshare_protocol_handler', 0),
  );
  $form[$group]['socialloginandsocialshare_popup_def_ifwidth'] = array(
    '#type' => 'hidden',
    '#title' => t('IFrame default width'),
    '#size' => 5,
    '#default_value' => variable_get('socialloginandsocialshare_popup_def_ifwidth', 169),
    '#required' => TRUE,
  );
  $form[$group]['socialloginandsocialshare_popup_def_ifheight'] = array(
    '#type' => 'hidden',
    '#title' => t('IFrame default height'),
    '#size' => 5,
    '#default_value' => variable_get('socialloginandsocialshare_popup_def_ifheight', 50),
    '#required' => TRUE,
  );
  $form[$group]['checkApi'] = array(
    '#type' => 'button',
    '#value' => t('Verfy API Settings'),
    '#weight' => 30,
    '#ajax' => array(
    'callback' => 'socialloginandsocialshare_apicheck_connection',
    'wrapper' => $group,
    'method' => 'replace',
    'effect' => 'fade',
    ),
  );
  $group = 'frontend';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('SOCIAL LOGIN FRONT-END SETTINGS'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
    $form[$group]['interface'] = array(
    '#type' => 'fieldset',
    '#title' => t('SOCIAL LOGIN INTERFACE SETTINGS'),
  );
  $form[$group]['interface']['socialloginandsocialshare_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What text do you want to display above the Social Login interface?'),
    '#default_value' => variable_get('socialloginandsocialshare_label_string', 'Login with Social ID'),
    '#description' => t('Leave empty for no text'),
  );
   $form[$group]['interface']['socialloginandsocialshare_attach_login_form'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show Social Login interface on the User login block?'),
    '#default_value' => variable_get('socialloginandsocialshare_attach_login_form', 1),
     '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    ),
  );
    $form[$group]['interface']['socialloginandsocialshare_loginpage_position'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show Social Login interface with Drupal native login form?'),
    '#default_value' => variable_get('socialloginandsocialshare_loginpage_position', 1),
    '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    ),
  );
    $form[$group]['interface']['socialloginandsocialshare_registerpage_position'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show Social Login interface with Drupal native user registration form?'),
    '#default_value' => variable_get('socialloginandsocialshare_registerpage_position', 1),
     '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    ),
  );
  $form[$group]['interface']['socialloginandsocialshare_showicon_location'] = array(
    '#type' => 'radios',
    '#title' => t('how do you want Social Login interface to be shown on your Drupal traditional register/login form?'),
    '#default_value' => variable_get('socialloginandsocialshare_showicon_location', 0),
    '#options' => array(
      0 => t('Above native register/login form'),
      1 => t('Below native register/login form'),
    ),
  );
  $form[$group]['interface']['socialloginandsocialshare_block_required'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to hide the Social Login interface after a user logged in? (  <a href="@block_socialloginandsocialshare" target="_blank">Click here to set Social login Block</a> )', array('@block_socialloginandsocialshare' => url('admin/structure/block'))),
    '#default_value' => variable_get('socialloginandsocialshare_block_required', 1),
    '#options' => array(
      1 => t('Yes, I  want to hide Social Login interface after a user logged in'),
      0 => t('No, I do not want to hide Social Login interface after a user logged in'),
    ),
  );
   $form[$group]['interface']['socialloginandsocialshare_disable_traditional_login'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want users to log in using Social Login only?'),
    '#default_value' => variable_get('socialloginandsocialshare_disable_traditional_login', 0),
    '#options' => array(
      1 => t('Yes, I want to disable the  Drupal native login form.(In this case, admin can login with his/her linked social account or via /user [example(clean url): site-url/user or example(not clean url): site-url/?q=user])'),
      0 => t('No, I want my users to have both options at their disposal'),
    ),
  );
    $form[$group]['linking'] = array(
    '#type' => 'fieldset',
    '#title' => t('SOCIAL ACCOUNT LINKING'),
  );
  $form[$group]['linking']['socialloginandsocialshare_identity_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What heading do you want to display to the users to link multiple Social IDs to one account?'),
    '#default_value' => variable_get('socialloginandsocialshare_identity_label_string', 'Link to an existing social account'),
    '#description' => t('This text will be displayed just above Social Login add more identities interface.'),
  );
    $form[$group]['linking']['socialloginandsocialshare_identity_tab_text'] = array(
    '#type' => 'textarea',
    '#title' => t('What text do you want to display to the users to encourage them to link multiple Social IDs to one account?'),
    '#default_value' => socialloginandsocialshare_identities_help_text(),
    '#description' => t('The text will be displayed on social login identities tab'),
    '#rows' => 8,
  );
  $group = 'basic';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('SOCIAL LOGIN BASIC SETTINGS'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
    $form[$group]['socialloginandsocialshare_display_username'] = array(
    '#type' => 'radios',
    '#title' => t('How would you like Username to be created? Select the Username syntax as per your preference'),
    '#default_value' => variable_get('socialloginandsocialshare_display_username', 0),
    '#options' => array(
      0 => t('Firstname Lastname [Ex: James Bond]'),
      1 => t('Firstname-Lastname [Ex: James-Bond]'),
      2 => t('Lastname Firstname [Ex: Bond James]'),
      3 => t('Lastname-Firstname [Ex: Bond-James]'),
    ) 
  );
   $form[$group]['socialloginandsocialshare_skip_email_verification'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want users to skip email verification when logging in from social ID providers like Facebook, Google, Yahoo, etc. and email is already provided by the ID provider?'),
    '#default_value' => variable_get('socialloginandsocialshare_skip_email_verification', 0),
    '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    ) 
  );
  $form[$group]['socialloginandsocialshare_force_registration'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want users to get registered automatically or you want them to manually complete the registration process?'),
    '#default_value' => variable_get('socialloginandsocialshare_force_registration', 1),
    '#options' => array(
      1 => t('Automatically register a user and create a new user account'),
      0 => t('Let the user submit the user registration form after social login process.(Users will be redirected to registration page with user fields auto filled and users finally submit the form to create an account with your website.)'),
    ),
  );
  $form[$group]['socialloginandsocialshare_link_account'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want your existing user to automatically link their social accounts in case their Drupal account email addresses match with their social account email IDs?'),
    '#default_value' => variable_get('socialloginandsocialshare_link_account', 1),
    '#options' => array(
      1 => t('YES, link social accounts to Drupal account'),
      0 => t('NO, I want my existing users to continue using native Drupal login'),
    ) 
  );
  $form[$group]['socialloginandsocialshare_email_required'] = array(
    '#type' => 'radios',
    '#title' => t('A few ID Providers do not supply users e-mail IDs as part of user profile data. Do you want users to provide their email IDs before completing registration process?'),
    '#default_value' => variable_get('socialloginandsocialshare_email_required', 1),
    '#options' => array(
      1 => t('YES, get real email IDs from the users (Ask users to enter their email IDs in a pop-up)'),
      0 => t('NO, just auto-generate random email IDs for the users'),
    )
  );
   $form[$group]['socialloginandsocialshare_emailrequired_popup_top'] = array(
    '#type' => 'textarea',
    '#title' => t('Please enter the title of the pop-up asking users to enter their email address'),
    '#rows' => 2,
    '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_top', 'You are trying to connect with @provider'),
    '#description' => t('You may use @provider, it will be replaced by the Provider name.'),
  );
   $form[$group]['socialloginandsocialshare_emailrequired_popup_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Please enter the message to be displayed to the user in the pop-up asking for their email address'),
    '#rows' => 2,
    '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_text', 'Unfortunately we could not retrieve email from your @provider account Please enter your email in the form below in order to continue.'),
    '#description' => t('You may use @provider, it will be replaced by the Provider name.'),
  );
   $form[$group]['socialloginandsocialshare_emailrequired_popup_wrong'] = array(
    '#type' => 'textarea',
    '#title' => t('Please enter the message to be shown to the user in case of an already registered email'),
    '#rows' => 2,
    '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_wrong', 'This email is already registered. Please choose another one or link this account via account linking on your profile page'),
  );
  $form[$group]['socialloginandsocialshare_userlogin_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Redirection Settings'),
    '#default_value' => variable_get('socialloginandsocialshare_userlogin_redirect', 0),
    '#options' => array(
      0 => t('Redirect to same page of site'),
      1 => t('Redirect to home page of site'),
      2 => t('Redirect to profile page of site'),
      3 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after Login)'),
    ),
  );
  $form[$group]['socialloginandsocialshare_userlogin_redirect']['socialloginandsocialshare_custom_redirection'] = array(
    '#type' => 'textfield',
    '#weight' => 50,
    '#default_value' => variable_get('socialloginandsocialshare_custom_redirection'),
  );
  // Submit handler.
  $form['#submit'][] = 'socialloginandsocialshare_admin_settings_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Validate Social Login settings.
 */
function socialloginandsocialshare_admin_settings_validate($form, &$form_state) {
  $apikey = trim($form_state['values']['socialloginandsocialshare_apikey']);
  $apisecret = trim($form_state['values']['socialloginandsocialshare_apisecret']);
  if (empty($apikey) || empty($apisecret)) {
    form_set_error('', t('Your LoginRadius API key/secret is required.'));
    return;
  }
  if (!socialloginandsocialshare_isvalid_apikey($apikey)) {
    form_set_error('', t('Your LoginRadius API Key is not valid.'));
    return;
  }
  if (!socialloginandsocialshare_isvalid_apikey($apisecret)) {
    form_set_error('', t('Your LoginRadius API Secret is not valid.'));
    return;
  }
  $obj_auth = new LoginRadius();
  $api_handler = trim($form_state['values']['socialloginandsocialshare_api_handler']);
  $userauth = $obj_auth->loginradius_get_auth($apikey, $apisecret, $api_handler );
  if ($userauth == "connection error") {
    form_set_error('', t('Your API connection method "@handler" is not working properly. try to change API connection method and verify your API settings.', array('@handler' => ($api_handler == 1 ? 'CURL' : 'FSOCKOPEN' ))));
    return;
  }
  elseif ($userauth == "service connection timeout" || $userauth == "timeout") {
    form_set_error('', t('Uh oh, looks like something went wrong. Try again in a sec!'));
    return;
  }
   else {
    variable_set('socialloginandsocialshareandsocialshare_apicall_results', 1);
  }
}
/**
 * Submit Social Login settings.
 */
function socialloginandsocialshare_admin_settings_submit($form, &$form_state) {


  if (isset($form_state['values']['socialloginandsocialshare_user_fields'])) {
    variable_set('socialloginandsocialshare_user_fields', array_filter($form_state['values']['socialloginandsocialshare_user_fields']));
  }
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  
  drupal_set_message(t('The configuration options have been saved.'));
  // Clear the cached pages and blocks.
  cache_clear_all();
  menu_rebuild();
}

/**
 * Function that validate socialloginandsocialshare apikey and secret.
 */
function socialloginandsocialshare_isvalid_apikey($apikey) {
  return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $apikey);
}

/**
 * Function that Check API connection.
 */
function socialloginandsocialshare_apicheck_connection($form, &$form_state) {
  $api_handler = trim($form_state['values']['socialloginandsocialshare_api_handler']);
  $data=variable_get('socialloginandsocialshareandsocialshare_apicall_results');
  if (!empty($data)) {
    variable_del('socialloginandsocialshareandsocialshare_apicall_results');
    drupal_set_message(t('Your API Key/Secret as well as API connection method "@handler" is working correctly. Please Save your current Settings .', array('@handler' => ($api_handler == 1 ? 'CURL' : 'FSOCKOPEN' ))));
  }
  return $form['setup'];
}

  /**
 * Implements Share setting for the socialloginandsocialshare module.
 */
 function socialloginandsocialshare_sharebar_settings_form() { 
  drupal_add_js('http://code.jquery.com/ui/1.10.0/jquery-ui.js');
  drupal_add_js(drupal_get_path('module', 'socialloginandsocialshare') . '/socialloginandsocialshare.js');
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/socialloginandsocialshare.admin.css');
  $my_path = drupal_get_path('module', 'socialloginandsocialshare');
  global $base_url;
  $group = 'share';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Sharing-Basic Settings'),
    '#collapsible' => TRUE,
  );
  $form[$group]['socialloginandsocialshare_enable_share'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable Social Sharing for your website'),
    '#default_value' => variable_get('socialloginandsocialshare_enable_share', 0),
    '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    )
  );
  $form[$group]['socialshare_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What text do you want to display above the Social Sharing widget?'),
    '#default_value' => variable_get('socialshare_label_string', 'Share it now!'),
    '#description' => t('Leave empty for no text.'),
  );
   $group = 'theme_share';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Sharing Theme Selection'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$group]['socialloginandsocialshare_share_horizontal'] = array(
    '#type' => 'item',
    '#prefix' => '<div><b>' . t('What Social Sharing widget theme do you want to use across your website?') . '</b></div>',
    '#markup' => l(t('Horizontal'), "javascript:void(0)", array('fragment' => '', 'external' => TRUE, 'attributes' => array('id' => 'socialloginandsocialshare_horizontal' , ))) . "<b>|</b>" . l(t('Vertical'), "javascript:void(0)", array('fragment' => '', 'external' => TRUE, 'attributes' => array('id' => 'socialloginandsocialshare_veritical' , )))
  );
  $form[$group]['socialloginandsocialshare_selected_share_interface'] = array(
    '#type' => 'hidden',
    '#title' => t('selected share interface'),
    '#suffix' => '<div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div>',
    '#default_value' => variable_get('socialloginandsocialshare_selected_share_interface'),
  );
  $form[$group]['socialloginandsocialshare_share_horizontal_images'] = array(
    '#type' => 'radios',
    '#id' => 'socialloginandsocialshare_share_horizontal_images',
    '#default_value' => variable_get('socialloginandsocialshare_share_horizontal_images', 0),
    '#options' => array(
       0 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing32.png"></img>',
       1 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing16.png"></img>',
       2 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-large.png"></img>',
       3 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-small.png"></img>',
     ),
  );
  $form[$group]['socialloginandsocialshare_share_vertical_images'] = array(
    '#type' => 'radios',
    '#id' => 'socialloginandsocialshare_share_vertical_images',
    '#default_value' => variable_get('socialloginandsocialshare_share_vertical_images', 4),
    '#options' => array(
       4 => '<img src="' . $base_url . '/' . $my_path . '/images/32VerticlewithBox.png"></img>',
       5 => '<img id="VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/16VerticlewithBox.png"></img>',
     ),
     '#attributes' => array('style' => 'display:inline'),
  );
  $share_providers =variable_get("socialloginandsocialshare_rearrange_providers_list");
  if (empty($share_providers)) {
    variable_set('socialloginandsocialshare_rearrange_providers_list', array( "0" => "facebook", "1" => "googleplus", ));
  }
  $form[$group]['socialshare_show_providers_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('What Sharing Networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
    '#default_value' => variable_get('socialloginandsocialshare_rearrange_providers_list'),
    '#options' => array(
      'facebook' => t('Facebook'),
      'googleplus' => t('GooglePlus'),
      'twitter' => t('Twitter'),
      'google' => t('Google'),
      'yahoo' => t('Yahoo'),
      'reddit' => t('Reddit'),
      'email' => t('Email'),
      'tumblr' => t('Tumblr'),
      'linkedin' => t('LinkedIn'),
      'live' => t('Live'),
      'vkontakte' => t('Vkontakte'),
      'digg' => t('Digg'),
      'myspace' => t('MySpace'),
      'delicious' => t('Delicious'),
      'hyves' => t('Hyves'),
      'dotnetkicks' => t('DotNetKicks'),
      ),
      '#attributes' => array(
        'onchange' => "loginRadiusSharingLimit(this); loginRadiusRearrangeProviderList(this);",
       ),
       '#prefix' => '<div id="loginRadiusSharingLimit" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
       '#suffix' => '<div><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="socialloginandsocialshare_rearrange_providers" class="socialloginandsocialshare_rearrange_providers">',
  );
  foreach (variable_get("socialloginandsocialshare_rearrange_providers_list") as $provider) {
    if (!empty($provider)) {
      $form[$group]['lrshare_iconsprite32' . $provider] = array(
        '#type' => 'item',
        '#attributes' => array('class' => array('lrshare_' . $provider)),
        '#markup' => '<li id = "edit-lrshare-iconsprite32' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
	    <input type = "hidden" id = "input-lrshare-' . $provider . '" name = "socialloginandsocialshare_rearrange_providers_list[]" value = "' . $provider . '" /></li>', 
      );
    }
  }
  $form[$group]['socialloginandsocialshare_rearran_providers_list']= array(
    '#type' => 'hidden',
    '#prefix' => '</ul><div id="socialloginandsocialshare_question"></div>',
  );
  $form[$group]['socialloginandsocialshare_share_vertical_images']['socialshare_vertical_position'] = array(
     '#type' => 'radios',
     '#title' => t('Select the position of Social Sharing widget'),
     '#weight' => 50,
     '#default_value' => variable_get('socialshare_vertical_position', 0),
     '#options' => array(
        0 => t('Top Left'),
        1 => t('Top Right'),
        2 => t('Bottom Left'),
        3 => t('Bottom Right'),
      ),
  );
  $form[$group]['socialloginandsocialshare_share_vertical_images']['socialshare_vertical_position']['socialshare_vertical_topweight'] = array(
    '#type' => 'textfield',
    '#title' => t('Specify distance of vertical sharing interface from top (Leave empty for default behaviour)'),
    '#weight' => 100,
    '#size' => 0,
    '#default_value' => variable_get('socialshare_vertical_topweight'),
    '#description' => t('Enter a number (For example - 200). It will set the top CSS attribute of the interface to the value specified. Increase in the number pushes interface towards bottom.'),
  );
   $group = 'theme_position';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Sharing Widget Position'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$group]['socialshare_position_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the top of content.'),
    '#default_value' => variable_get('socialshare_position_top', 1) ? 1 : 0,
    '#prefix' => '<div> <b>Select the position of Social sharing interface</b></div>',
  );
   
  $form[$group]['socialshare_position_bottom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the bottom of content.'),
    '#default_value' => variable_get('socialshare_position_bottom', 1) ? 1 : 0,
  );
  $form[$group]['socialshare_show_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Show Social Share on specific pages'),
    '#default_value' => variable_get('socialshare_show_pages', 0),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
     ),
  );
  $form[$group]['socialshare_show_exceptpages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('socialshare_show_exceptpages', ''),
    '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma sepreted'),
    '#rows' => 5,
  );
  $form['#validate'][] = 'socialloginandsocialshare_sharebar_settings_validate';
  $form['#submit'][] = 'socialloginandsocialshare_sharebar_settings_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  
  return $form;
}
function socialloginandsocialshare_sharebar_settings_validate($form, &$form_state) {
 $api_key = trim(variable_get('socialloginandsocialshare_apikey'));
  if (empty($api_key)) {
    form_set_error('', t('Insert LoginRadius API key/secret is under Social Login settings tab.'));
    return;
  }
}
 /**
 * Submit the Social share settings.
 */
function socialloginandsocialshare_sharebar_settings_submit($form, &$form_state) {
   if (isset($form_state['input']['socialloginandsocialshare_rearrange_providers_list'])) {
     variable_set('socialloginandsocialshare_rearrange_providers_list', array_filter($form_state['input']['socialloginandsocialshare_rearrange_providers_list']));
   } 
   else {
     variable_set('socialloginandsocialshare_rearrange_providers_list', array( "0" => "facebook", "1" => "googleplus", ));
   }
     form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  drupal_set_message(t('The configuration options have been saved.'));
  // Clear the cached pages and blocks.
  cache_clear_all();
  menu_rebuild();
}

 /**
 * Implements Counter setting for the socialloginandsocialshare module.
 */
function socialloginandsocialshare_counterinterface_settings_form() {
  drupal_add_js(drupal_get_path('module', 'socialloginandsocialshare') . '/socialloginandsocialshare_counter.js');
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/socialloginandsocialshare.admin.css');
  $my_path = drupal_get_path('module', 'socialloginandsocialshare');
  global $base_url;
  $group = 'counter';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Counter-Basic Settings'),
    '#collapsible' => TRUE,
  );
  $form[$group]['socialloginandsocialshare_enable_counter'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable Social Counter for your website'),
    '#default_value' => variable_get('socialloginandsocialshare_enable_counter', 0),
    '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    )
  );
  $form[$group]['socialcounter_label_string'] = array(
    '#type' => 'textfield',
    '#title' => t('What text do you want to display above the Social Counter widget (Leave empty for no text)?'),
    '#default_value' => variable_get('socialcounter_label_string', 'Share it now!'),
    '#description' => t('This text will be displayed just above Social Counter interface.'),
  );
    $group = 'theme_counter';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Counter Theme Selection'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$group]['socialloginandsocialshare_counter_horizontal'] = array(
    '#type' => 'item',
    '#prefix' => '<div><b>' . t('What Social Counter theme do you want to use across your website?') . '</b></div>',
    '#markup' => l(t('Horizontal'), "javascript:void(0)", array('fragment' => '', 'external' => TRUE, 'attributes' => array('id' => 'socialloginandsocialshare_counter_horizontal' , ))) . "<b>|</b>" . l(t('Vertical'), "javascript:void(0)", array('fragment' => '', 'external' => TRUE, 'attributes' => array('id' => 'socialloginandsocialshare_counter_veritical' , )))
  );  
  $form[$group]['socialloginandsocialshare_selected_counter_interface'] = array(
    '#type' => 'hidden',
    '#title' => t('selected share interface'),
    '#suffix' => '<div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div>',
    '#default_value' => variable_get('socialloginandsocialshare_selected_counter_interface'),
  );
  $form[$group]['socialloginandsocialshare_counter_horizontal_images'] = array(
    '#type' => 'radios',
    '#id' => 'socialloginandsocialshare_counter_horizontal_images',
    '#default_value' => variable_get('socialloginandsocialshare_counter_horizontal_images', 0),
    '#options' => array(
      0 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontalvertical.png"></img>',
      1 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontal.png"></img>',
    ),
  );
  $form[$group]['socialloginandsocialshare_counter_vertical_images'] = array(
    '#type' => 'radios',
    '#id' => 'socialloginandsocialshare_counter_vertical_images',
    '#default_value' => variable_get('socialloginandsocialshare_counter_vertical_images', 2),
    '#options' => array(
      2 => '<img src="' . $base_url . '/' . $my_path . '/images/verticalvertical.png"></img>',
      3 => '<img id="verticalhorizontal" src="' . $base_url . '/' . $my_path . '/images/verticalhorizontal.png"></img>',
    ),
  );
  $form[$group]['socialloginandsocialshare_counter_vertical_images']['socialshare_counter_vertical_position'] = array(
    '#type' => 'radios',
    '#title' => t('Select the position of Social Counter widget'),
    '#weight' => 50,
    '#default_value' => variable_get('socialshare_counter_vertical_position', 0),
    '#options' => array(
      0 => t('Top Left'),
      1 => t('Top Right'),
      2 => t('Bottom Left'),
      3 => t('Bottom Right'),
    ),
  );
    $form[$group]['socialloginandsocialshare_counter_vertical_images']['socialshare_counter_vertical_position']['socialshare_counter_vertical_topweight'] = array(
     '#type' => 'textfield',
     '#title' => t('Specify distance of vertical Counter interface from top (Leave empty for default behaviour)'),
     '#weight' => 100,
     '#size' => 0,
     '#default_value' => variable_get('socialshare_counter_vertical_topweight'),
     '#description' => t('Enter a number (For example - 200). It will set the top CSS attribute of the interface to the value specified. Increase in the number pushes interface towards bottom.'),
   );
  $counter_providers =variable_get('socialshare_counter_show_providers_list');
  if (empty($counter_providers)) {
    variable_set('socialshare_counter_show_providers_list', array("Facebook Like",  "Google+ +1",));
  }
  $form[$group]['socialshare_counter_show_providers_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('What Counter Networks do you want to show in the counter interface?'),
    '#default_value' => variable_get('socialshare_counter_show_providers_list'),
    '#options' => array(
      'Facebook Like' => t('Facebook Like'),
      'Facebook Recommend' => t('Facebook Recommend'),
      'Facebook Send' => t('Facebook Send'),
      'Google+ +1' => t('Google+ +1'),
      'Google+ Share' => t('Google+ Share'),
      'LinkedIn Share' => t('LinkedIn Share'),
      'Twitter Tweet' => t('Twitter Tweet'),
      'StumbleUpon Badge' => t('StumbleUpon Badge'),
      'Reddit' => t('Reddit'),
    ),
  );
   $group = 'theme_counter_position';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Counter Widget Position'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$group]['socialcounter_position_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the top of content.'),
    '#default_value' => variable_get('socialcounter_position_top', 1) ? 1 : 0,
    '#prefix' => '<div><b>Select the position of Social sharing interface</b>',
    '#suffix' => '</div>',
  );
  $form[$group]['socialcounter_position_bottom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show at the bottom of content.'),
    '#default_value' => variable_get('socialcounter_position_bottom', 1) ? 1 : 0,
  );
  $form[$group]['socialcounter_show_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Show Social Counter on specific pages'),
    '#default_value' => variable_get('socialcounter_show_pages', 0),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
  );
  $form[$group]['socialcounter_show_exceptpages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('socialcounter_show_exceptpages', ''),
    '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma sepreted'),
    '#rows' => 5,
  );
  return system_settings_form($form);
}
  
 /**
 * Implements Field Mapping setting for the socialloginandsocialshare module.
 */
function socialloginandsocialshare_fieldmapping_settings_form() {
   // Adding field mapping.
  module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
  $group = 'fieldmap';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('SOCIAL LOGIN FIELD MAPPING'),
    '#description' => t('Each of your <a href="!url">fields that are attached to users</a> are listed below. Map social provider data to field by choosing from select list and configure your user fields from <a href="!url"> FIELD MAPPING </a>section.', array('!url' => url('admin/config/people/accounts/fields'))),
    
  );
  $form[$group]['socialloginandsocialshare_user_fields'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#weight' => 5,
    '#collapsible' => TRUE,
  );
  $properties = socialloginandsocialshare_user_properties(TRUE);
  $property_options = array();
  foreach ($properties as $property => $property_info) {
    if (isset($property_info['field_types'])) {
      foreach ($property_info['field_types'] as $field_type) {
        $property_options[$field_type][$property] = '[' . $property . '] ' . $property_info['label'];
      }
    }
  }
  $field_defaults = variable_get('socialloginandsocialshare_user_fields', array());
  $instances = field_info_instances('user', 'user');
  foreach ($instances as $field_name => $instance) {
  $field = field_info_field($instance['field_name']);
    if (isset($property_options[$field['type']])) {
      $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
      $form[$group]['socialloginandsocialshare_user_fields'][$field_name] = array(
        '#title' => check_plain(t($instance['label'])),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($field_defaults[$field_name]) ? $field_defaults[$field_name] : '',
      );
    }
    else {
      $form[$group]['socialloginandsocialshare_user_fields'][$field_name] = array(
        '#title' => check_plain(t($instance['label'])),
        '#type' => 'form_element',
        '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
        '#theme_wrappers' => array('form_element'),
      );
    }
  }
  return system_settings_form($form);
}
 
 /**
 * Implements help tab  for the socialloginandsocialshare module.
 */
function socialloginandsocialshare_user_help() {
  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/socialloginandsocialshare.admin.css');
  $help_text = t('<b>Help & Documentations</b><br/><a href="@install" target="_blank">Plugin Installation, Configuration and Troubleshooting</a><br/><a href="@api" target="_blank">How to get LoginRadius API Key & Secret</a><br/><a href="@forum" target="_blank">Discussion Forum</a><br><a href="@about" target="_blank">About LoginRadius</a><br><a href="@product" target="_blank">LoginRadius Products</a><br><a href="@plugin" target="_blank">Social Plugins</a><br><a href="@sdk" target="_blank">Social SDKs</a>', array('@loginradius' => 'http://www.Loginradius.com/', '@install' => 'http://support.loginradius.com/customer/portal/articles/995308-drupal-social-login-plugin-installation-configuration-and-troubleshooting', '@api' => 'http://support.loginradius.com/customer/portal/articles/677100-how-to-get-loginradius-api-key-and-secret', '@forum' => 'http://community.loginradius.com/', '@about' => 'http://www.loginradius.com/loginradius/about', '@product' => 'http://www.loginradius.com/product/sociallogin', '@plugin' => 'http://www.loginradius.com/addons', '@sdk' => 'http://www.loginradius.com/sdks/loginradiussdk'));
   $sociallogin_get_updates = t('<b>Get Updates</b><br/><br/>To receive updates on new features, releases, etc, please connect to one of our social media pages -<br/><br/><div style=float:left><iframe rel="tooltip" scrolling="no" frameborder="0" allowtransparency="true" style="border: none; overflow: hidden; width: 46px;
height: 61px; margin-right:10px" src="//www.facebook.com/plugins/like.php?app_id=194112853990900&href=http%3A%2F%2Fwww.facebook.com%2Fpages%2FLoginRadius%2F119745918110130&send=false&layout=box_count&width=90&show_faces=false&action=like&colorscheme=light&font=arial&height=90" data-original-title="Like us on Facebook"></iframe></div><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script><div class="twitter_box"><span id="followers"></span></div><a href="https://twitter.com/LoginRadius" class="twitter-follow-button" data-show-count="false" data-show-screen-name="false"></a>
');
drupal_add_js('jQuery(function() {function m(n, d) {P = Math.pow;R = Math.roundd = P(10, d);i = 7;while(i) {(s = P(10, i-- * 3)) <= n && (n = R(n * d / s) / d + "KMGTPE"[i])}
return n;}jQuery.ajax({url: "http://api.twitter.com/1/users/show.json", data: {screen_name: "LoginRadius"}, dataType: "jsonp",success: function(data) {count = data.followers_count;jQuery("#followers").html(m(count, 1));}});});', array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
  $support_text = t('<b>Support Us</b><br/><br/>If you liked our FREE open-source plugin, please send your feedback/testimonial to <a href="@feedback" target="_blank">feedback@loginradius.com</a>!' , array('@feedback' => 'mailto:feedback@loginradius.com'));
 $form['help_block'] = array(
    '#type' => 'item',
    '#markup' => '<div id=thanks_block>' . $help_text . '</div>',
 );
 $form['thanks_block'] = array(
   '#type' => 'item',
   '#markup' => '<div style=float:left; id=sociallogin_get_update>' . $sociallogin_get_updates . '</div>',
 );
 $form['support_block'] = array(
   '#type' => 'item',
   '#markup' => '<div style=margin-left:785px;margin-top:-56px; id=sociallogin_get_update>' . $support_text . '</div>',
  );
  return $form;
}