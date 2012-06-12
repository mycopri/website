/* $Id: domain-user-default.js,v 1.1.2.1 2010/11/18 01:39:08 jhedstrom Exp $ */
Drupal.behaviors.domainUserDefaultSwitcher = function () {
  // Hide submit button on domain switcher form.
  $('#domain-user-default-domain-switcher-form .form-submit').hide();

  // Attach on change function to submit the form.
  $('#domain-user-default-domain-switcher-form').change(function() {
    this.submit();
  });
}