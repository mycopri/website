<?php
// $Id: image_gallery_access.node.inc,v 1.6 2010/07/14 12:22:50 salvis Exp $

/**
 * @file image_gallery_access.node.inc
 *
 * Include file for image_gallery_access.module, containing (sub-)page handling
 * (form_alter) code for the node form.
 *
 */

/**
 * Rewrite the taxonomy item on the node form.
 */
function _image_gallery_access_node_form(&$form, &$form_state) {
  global $user;
  $vid = _image_gallery_get_vid();
  if (!isset($form['taxonomy'][$vid]['#options'])) {
    return;
  }

  // image and node administrators are all powerful and do NOT get their forms rewritten here.
  if (user_access('administer image galleries') || user_access('administer nodes')) {
    return;
  }

  $roles = array_keys($user->roles);
  $result = db_query("SELECT tid FROM {image_gallery_access} WHERE rid IN (". db_placeholders($roles) .") AND grant_create = 1", $roles);
  while ($obj = db_fetch_object($result)) {
    $tids[$obj->tid] = $obj->tid;
  }

  // Also get all image galleries they happen to be able to moderate.
  $result = db_query("SELECT a.name AS tid FROM {acl} a INNER JOIN {acl_user} u ON a.acl_id = u.acl_id WHERE a.module = 'image_gallery_access' AND u.uid = %d", $user->uid);
  while ($obj = db_fetch_object($result)) {
    $tids[$obj->tid] = $obj->tid;
  }

  // Ensure the image gallery they're trying to post to directly is allowed, otherwise
  // there will be much confusion.
  $gallery_tid = arg(3);
  if (isset($gallery_tid) && is_numeric($gallery_tid) && !isset($tids[$gallery_tid])) {
    drupal_access_denied();
    module_invoke_all('exit');
    exit;
  }

  foreach ($form['taxonomy'][$vid]['#options'] as $tid => $name) {
    if (!is_numeric($tid)) {
      $options[$tid] = $name;
    }
    elseif (is_object($name)) {
      foreach ($name->option as $sub_tid => $sub_name) {
        if (!empty($tids[$sub_tid])) {
          $options[$tid]->option[$sub_tid] = $sub_name;
        }
      }
    }
    elseif ($tids[$tid]) {
      $options[$tid] = $name;
    }
  }

  if ($options) {
    $form['taxonomy'][$vid]['#options'] = $options;
  }
  else {
    unset($form['taxonomy'][$vid]);
  }
}

