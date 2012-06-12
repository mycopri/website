$Id: README.txt,v 1.1.2.2 2008/07/17 17:57:35 jhedstrom Exp $

/**
 * @file
 * README for the Domain User Default module
 */

Introduction
------------

The Domain User Default module is a sub-module of the Domain [1]
module that provides each users with a way to choose a default
subdomain. Depending on how the module is configured, the user will be
automatically directed to their chosen subdomain when they land on a
different subdomain.

In order to allow non-authenticated users to select a default domain,
the Domain Session Default module should be enabled. This module
stores a default domain via the Session API [2] module, and seamlessly
transitions this into the users table should the visitor login or
register.

For additional functionality, the Domain Geolocalization [3] module which
provides users with a way to search for nearby subdomains (if the
domains are organized geographically).


Sponsors
--------

The Domain User Default module is sponsored by One Economy
(http://one-economy.com) and developed and maintained by OpenSourcery
(http://opensourcery.com).


Installation
------------

  1) Move the module code into your modules directory, and install

  2) Read the README.txt file packaged with the Domain Access module.

  3) Configure behaviors for existing and new domains at admin/build/domain

  4) The module provides several permissions which need to be granted
     to the desired roles. NOTE: If installing Domain Session Default
     (bundled with this module) for unauthenticated users, then
     anonymous users should be granted the permission "set own domain
     default"

  5) The module provides a block for easy switching of default
     domains, as well as a field in the user account section.

  6) If installing the Domain Geolocalization module, the Domain
     Search block can be enabled and configured at
     admin/build/block. This block can be configured to display a
     user's current default domain, and then provide an AHAH pop-up or
     drop down for switching domains.


References
----------

1. http://drupal.org/project/domain
2. http://drupal.org/project/session_api
3. http://drupal.org/project/domain_geolocalization
