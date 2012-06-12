<?php
// $Id: image_gallery_access.admin.inc,v 1.31 2010/07/28 22:58:12 salvis Exp $

/**
 * @file image_gallery_access.admin.inc
 *
 * Include file for image_gallery_access.module, containing (sub-)page handling
 * (form_alter) and batch code.
 *
 */

/**
 * Rewrite the image gallery administration page with our new access rules.
 */
function _image_gallery_access_admin_form(&$form, &$form_state) {
  $tid = (isset($form['tid']['#value']) ? $form['tid']['#value'] : NULL);
  if (isset($tid) && !image_gallery_access_access($tid, 'view', NULL, FALSE)) {
    drupal_access_denied();  // Deny access if the user doesn't have View access.
    module_invoke_all('exit');
    exit;
  }

  $roles = array();
  $result = db_query(db_rewrite_sql("SELECT r.rid, r.name FROM {role} r ORDER BY r.name", 'r', 'rid'));
  while ($obj = db_fetch_object($result)) {
    $roles[$obj->rid] = check_plain($obj->name);
  }

  if (isset($tid)) {  // edit
    $template_tid = variable_get('image_gallery_access_default_template_tid', 0);
    $settings = _image_gallery_access_get_settings($tid);
  }
  else {  // create
    $template_tid = variable_get('image_gallery_access_new_template_tid', NULL);
    $settings = _image_gallery_access_get_settings($template_tid);
  }
  $iga_priority = $settings['priority'];

  $form['image_gallery_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access control'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  $tr = 't';
  $variables = array(
    '!access_content'                 => '<em>'. l($tr('access content'), 'admin/user/permissions', array('fragment' => 'module-node', 'html' => TRUE)) .'</em>',
    '!access_comments'                => '<em>'. l($tr('access comments'), 'admin/user/permissions', array('fragment' => 'module-comment', 'html' => TRUE)) .'</em>',
    '!create_images'                  => '<em>'. l($tr('create images'), 'admin/user/permissions', array('fragment' => 'module-image', 'html' => TRUE)) .'</em>',
    '!post_comments'                  => '<em>'. l($tr('post comments'), 'admin/user/permissions', array('fragment' => 'module-comment', 'html' => TRUE)) .'</em>',
    '!post_comments_without_approval' => '<em>'. l($tr('post comments without approval'), 'admin/user/permissions', array('fragment' => 'module-comment', 'html' => TRUE)) .'</em>',
    '!edit_own_images'                => '<em>'. l($tr('edit own images'), 'admin/user/permissions', array('fragment' => 'module-image', 'html' => TRUE)) .'</em>',
    '!edit_any_images'                => '<em>'. l($tr('edit any images'), 'admin/user/permissions', array('fragment' => 'module-image', 'html' => TRUE)) .'</em>',
    '!delete_own_images'              => '<em>'. l($tr('delete own images'), 'admin/user/permissions', array('fragment' => 'module-image', 'html' => TRUE)) .'</em>',
    '!delete_any_images'              => '<em>'. l($tr('delete any images'), 'admin/user/permissions', array('fragment' => 'module-image', 'html' => TRUE)) .'</em>',
    '!administer_comments'            => '<em>'. l($tr('administer comments'), 'admin/user/permissions', array('fragment' => 'module-comment', 'html' => TRUE)) .'</em>',
    '!administer_image_galleries'     => '<em>'. l($tr('administer image galleries'), 'admin/user/permissions', array('fragment' => 'module-image_gallery', 'html' => TRUE)) .'</em>',
    '!administer_nodes'               => '<em>'. l($tr('administer nodes'), 'admin/user/permissions', array('fragment' => 'module-node', 'html' => TRUE)) .'</em>',
  );
  $form['image_gallery_access']['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => $tr('Permissions information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['image_gallery_access']['permissions'][] = array(
    '#type' => 'markup',
    '#value' => '<div>'. t('Note that users need') .'<ul style="margin-top: 0"><li>'.
                         t('the !access_content permissions <strong>AND <em>View</em></strong> to be able to see content at all and', $variables) .'</li><li>'.
                         t('the !create_images permissions <strong>AND <em>Post</em></strong> to be able to upload images;', $variables) .'</li><li>'.
                         t('the !edit_own_images or !edit_any_images permissions (<strong>OR <em>Edit</em></strong>) can be added if desired, <strong>plus</strong>', $variables) .'</li><li>'.
                         t('the !delete_own_images or !delete_any_images permissions (<strong>OR <em>Delete</em></strong>) if desired;', $variables) .'</li><li>'.
                         t('the !administer_image_galleries permission <strong>AND <em>View</em></strong> to be able to administer galleries (and change access!).', $variables) .'</li></ul></div>',
  );
  $form['image_gallery_access']['permissions'][] = array(
    '#type' => 'markup',
    '#value' => '<div>'. t('If you allow comments in your galleries, users also need') .'<ul style="margin-top: 0"><li>'.
                         t('the !access_comments permission to be able to see comments and replies, and', $variables) .'</li><li>'.
                         t('the !post_comments or !post_comments_without_approval permission to be able to post comments and replies;', $variables) .'</li><li>'.
                         t('the !administer_comments permission to be able to administer comments and replies (everywhere!).', $variables) .'</li></ul>'.
                         t('Furthermore note that content which is not published is treated in a different way by Drupal: it can be viewed only by its author or by users with the !administer_nodes permission. Unpublished comments and replies are accessible to users with the !administer_comments permission only.', $variables) .'</div>',
  );

  // Load a template:
  $vid = variable_get('image_gallery_nav_vocabulary', '');
  $form['image_gallery_access']['template'] = array(
      '#type' => 'fieldset',
      '#title' => $tr('Template'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($template_tid),
  );
  $form['image_gallery_access']['template']['taxonomy'][$vid] = taxonomy_form($vid, array($template_tid), '');
  $form['image_gallery_access']['template']['taxonomy'][$vid]['#description'] = t("Select a gallery and click !Load to retrieve that gallery's settings as a starting point for this gallery.", array('!Load' => '['. t('Load') .']'));
  $form['image_gallery_access']['template']['load_button'] = array(
    '#type' => 'button',
    '#name' => 'load_template',
    '#value' => t('Load'),
    '#submit' => FALSE,
  );
  $form['image_gallery_access']['template']['template_tid'] = array(
    '#type' => 'value',
    '#value' => NULL,
  );
  $form['image_gallery_access']['template']['select_by_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember this selection.'),
    '#default_value' => FALSE,
  );
  $form['image_gallery_access']['template']['load_for_new'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use the selected gallery's settings as defaults for new galleries."),
    '#default_value' => FALSE,
  );
  $form['image_gallery_access']['#after_build'][] = '_image_gallery_access_admin_form_after_build';

  // Column titles:
  $form['image_gallery_access']['headers']['view'] = array(
    '#type' => 'item',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#title' => t('View this gallery'),
    '#suffix' => '</div>',
  );
  $form['image_gallery_access']['headers']['create'] = array(
    '#type' => 'item',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#title' => t('Post in this gallery'),
    '#suffix' => '</div>',
  );
  $form['image_gallery_access']['headers']['update'] = array(
    '#type' => 'item',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#title' => t('Edit images'),
    '#suffix' => '</div>',
  );
  $form['image_gallery_access']['headers']['delete'] = array(
    '#type' => 'item',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#title' => t('Delete images'),
    '#suffix' => '</div>',
  );
  $form['image_gallery_access']['headers']['clearer'] = array(
    '#value' => '<div class="image-gallery-access-clearer"></div>',
  );

  // Column content (checkboxes):
  $form['image_gallery_access']['view'] = array(
    '#type' => 'checkboxes',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#suffix' => '</div>',
    '#options' => $roles,
    '#default_value' => $settings['view'],
    '#process' => array('expand_checkboxes', '_image_gallery_access_admin_form_disable_checkboxes'),
  );
  $form['image_gallery_access']['create'] = array(
    '#type' => 'checkboxes',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#suffix' => '</div>',
    '#options' => $roles,
    '#default_value' => $settings['create'],
    '#process' => array('expand_checkboxes', '_image_gallery_access_admin_form_disable_checkboxes'),
  );
  $form['image_gallery_access']['update'] = array(
    '#type' => 'checkboxes',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#suffix' => '</div>',
    '#options' => $roles,
    '#default_value' => $settings['update'],
    '#process' => array('expand_checkboxes', '_image_gallery_access_admin_form_disable_checkboxes'),
  );
  $form['image_gallery_access']['delete'] = array(
    '#type' => 'checkboxes',
    '#prefix' => '<div class="image-gallery-access-div">',
    '#suffix' => '</div>',
    '#options' => $roles,
    '#default_value' => $settings['delete'],
    '#process' => array('expand_checkboxes', '_image_gallery_access_admin_form_disable_checkboxes'),
  );
  $form['image_gallery_access']['clearer'] = array(
    '#value' => '<div class="image-gallery-access-clearer"></div>',
  );

  drupal_add_css(drupal_get_path('module', 'image_gallery_access') .'/image_gallery_access.css');

  // Find our moderator ACL:
  if (isset($tid)) {  // edit, not new
    $acl_id = db_result(db_query("SELECT acl_id from {acl} WHERE module = 'image_gallery_access' AND name = '%d'", $tid));
    if (!$acl_id) { // create one
      $acl_id = acl_create_new_acl('image_gallery_access', $tid);
      // update every existing node in this image gallery to use this acl.
      $result = db_query("SELECT nid FROM {term_node} WHERE tid = %d", $tid);
      while ($node = db_fetch_object($result)) {
        // all privs to this ACL.
        acl_node_add_acl($node->nid, $acl_id, 1, 1, 1);
      }
    }
    $form['image_gallery_access']['acl'] = acl_edit_form($acl_id, t('Moderators'));
    $form['image_gallery_access']['acl'][] = array(
      '#type' => 'markup',
      '#value' => '<div>'. t('Moderators receive all grants above; to moderate comments, they also need !administer_comments (which is a global permission!).', $variables) .'</div>',
      '#weight' => -1,
    );
    $form['image_gallery_access']['acl']['note'] = array(
      '#type' => 'markup',
      '#value' => '<div>'. t('Note: Changes to moderators are not saved until you click [!Save] below.', array('!Save' => $tr('Save'))) .'</div>',
    );
    $form['image_gallery_access']['acl']['#after_build'][] = '_image_gallery_access_admin_form_after_build_acl0';
    $form['image_gallery_access']['acl']['#after_build'] = array_reverse($form['image_gallery_access']['acl']['#after_build']);
    $form['image_gallery_access']['acl']['#after_build'][] = '_image_gallery_access_admin_form_after_build_acl2';
  }

  foreach (module_implements('node_access_records') as $module) {
    $na_modules[$module] = $module;
  }
  unset($na_modules['image_gallery_access']);
  unset($na_modules['acl']);
  if (count($na_modules)) {
    $form['image_gallery_access']['interference'] = array(
      '#type' => 'fieldset',
      '#title' => t('Module interference'),
      '#collapsible' => TRUE,
    );
    $variables = array(
      '%content_type' => node_get_types('name', 'image'),
      '!Image_Gallery_Access' => 'Image Gallery Access',
      '!Content_Access' => l('Content Access', 'http://drupal.org/project/content_access'),
      '@Content_Access' => 'Content Access',
      '!ACL' => 'ACL',
      '!module_list' => '<ul><li>'. implode($na_modules, '</li><li>') .'</li></ul>',
    );
    $form['image_gallery_access']['interference'][] = array(
      '#type' => 'item',
      '#value' => '<p>'. t("Besides !Image_Gallery_Access (and !ACL) you have installed the following node access module(s): !module_list   The grants of every module are combined for each node. Access can only be granted, not removed&mdash;if a certain module grants a permission, the other(s) cannot deny it.", $variables) .'</p>',
    );

    if (module_exists('content_access')) {
      $ca_settings = variable_get('content_access_settings', array());
      foreach (array('view', 'update', 'delete', 'per_node') as $type) {
        $value = content_access_get_settings($type, 'image');
        if (!empty($value)) {
          $ca_interferes = TRUE;
        }
      }
      $ca_priority = content_access_get_settings('priority', 'image');
      $is_conflict = $ca_priority >= $iga_priority && !empty($ca_interferes) || $ca_priority > $iga_priority;
      $variables += array(
        '!link' => l(t('@Content_Access configuration for the %content_type type', $variables), 'admin/content/node-type/image/access', array('html' => TRUE)),
        '%Advanced' => $tr('Advanced'),
      );
      $specifically = ($ca_priority == $iga_priority ? t('Specifically, any grants given by @Content_Access cannot be taken back by !Image_Gallery_Access.', $variables) : '');
      if ($is_conflict) {
        $form['image_gallery_access']['interference']['by_content_access'] = array(
          '#type' => 'fieldset',
          '#title' => 'Content Access',
          '#collapsible' => FALSE,
          '#attributes' => array('class' => 'error'),
        );
        $form['image_gallery_access']['interference']['by_content_access'][] = array(
          '#value' => '<div>'. t('You have set the !Content_Access module to control access to content of type %content_type&mdash;this can interfere with proper operation of !Image_Gallery_Access!', $variables) ." $specifically</div>",
        );
        if ($ca_priority == $iga_priority) {
          $form['image_gallery_access']['interference']['by_content_access'][] = array(
            '#value' => '<div>'. t("Unless you really know what you're doing, we recommend that you go to the !link page and clear all checkboxes. This will instruct @Content_Access to leave the %content_type nodes alone.", $variables) .'</div>',
          );
        }
        else {
          $form['image_gallery_access']['interference']['by_content_access'][] = array(
            '#value' => '<div>'. t("The priority of @Content_Access ($ca_priority) is higher than the priority of !Image_Gallery_Access ($iga_priority), which means the latter is <strong>completely disabled</strong>! Unless you really know what you're doing, we recommend that you go to the !link page, change the priority (under %Advanced) to 0, and clear all checkboxes.", $variables) .'</div>',
          );
        }
        $form['image_gallery_access']['interference']['by_content_access'][] = array(
          '#value' => '<div>'. t("Alternatively, you can give !Image_Gallery_Access priority over @Content_Access by either raising the priority of !Image_Gallery_Access in every gallery above the priority of @Content_Access, or by lowering the priority of @Content_Access for the %content_type content type below the priority of !Image_Gallery_Access.", $variables) .'</div>',
        );
      }
      else {
        $form['image_gallery_access']['interference'][] = array(
          '#value' => '<p>'. t('Note: You have installed the !Content_Access module, which has the capability to grant access to content that would otherwise be protected by !Image_Gallery_Access. Be careful when configuring @Content_Access!', $variables) .'</p>',
        );
      }
    }

    $form['image_gallery_access']['interference']['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed' => !($iga_priority != 0),
    );
    $form['image_gallery_access']['interference']['advanced']['priority'] = array(
      '#type' => 'weight',
      '#title' => t('Priority of !Image_Gallery_Access node grants in this gallery', $variables),
      '#default_value' => $iga_priority,
      '#description' => t("If you have no other node access control modules installed, you should leave this at the default 0. <br /> Otherwise you can raise or lower the priority of !Image_Gallery_Access' grants. Out of all the grants contributed to a node, only those with the highest priority are used, and all others are discarded.", $variables),
    );
  }

  $variables = array(
    '!Image_Gallery_Access'     => l('Image Gallery Access', 'http://drupal.org/project/image_gallery_access'),
    '!ACL'                      => l('ACL', 'http://drupal.org/project/acl'),
    '%Module_interference'      => t('Module interference'),
    '!Image_Gallery_Access-dev' => l('Image&nbsp;Gallery&nbsp;Access&nbsp;6.x-1.x-dev', 'http://drupal.org/node/308939', array('html' => TRUE)),
    '!ACL-dev'                  => l('ACL&nbsp;6.x-1.x-dev', 'http://drupal.org/node/96794', array('html' => TRUE)),
    '%devel_node_access'        => 'devel_node_access',
    '!Devel'                    => l('Devel', 'http://drupal.org/project/devel'),
    '!DNA'                      => 'DNA',
    '!debug_mode'               => l('debug mode', 'admin/settings/devel', array('fragment' => 'edit-devel-node-access-debug-mode')),
    '!dna_summary'              => l('devel/node_access/summary', 'devel/node_access/summary'),
    '!Rebuild_permissions'      => '['. $tr('Rebuild permissions') .']',
    '!Post_settings_link'       => l('admin/content/node-settings', 'admin/content/node-settings'),
    '!Image_Gallery_Access_'    => l('Image Gallery Access', 'http://drupal.org/project/issues/image_gallery_access'),
    '!ACL_'                     => l('ACL', 'http://drupal.org/project/issues/acl'),
  );
  $form['image_gallery_access']['troubleshooting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trouble-shooting node access'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['image_gallery_access']['troubleshooting'][] = array(
    '#type' => 'item',
    '#value' => '<div>'.    t("In case of problems, follow these steps until you've got it worked out:") .'<ol style="margin-top: 0"><li>'.
                            t("Update to the 'recommended' !Image_Gallery_Access and !ACL releases for your version of Drupal.", $variables) .'</li><li>'.
      (count($na_modules) ? t("Read %Module_interference above and update your other node access modules.", $variables) .'</li><li>' : '') .
                            t("Check the release notes of the development snapshots for issues that might have been fixed in !Image_Gallery_Access-dev or !ACL-dev since the latest release.", $variables) .'</li><li>'.
                            t("Install the %devel_node_access module (!DNA, part of the !Devel module) and enable its !debug_mode: !DNA will show you all the grants that actually control your nodes in a footer block on each node's page.", $variables) .'</li><li>'.
                            t("Additional insight can be gained from !dna_summary and by enabling the second !DNA block.", $variables) .'</li><li>'.
                            t("Click !Rebuild_permissions on !Post_settings_link and check DNA for changes.", $variables) .'</li><li>'.
                            t("Check the issues queues of !Image_Gallery_Access_ and !ACL_ for existing reports and possible solutions.", $variables) .'</li><li>'.
                            t("If all of this hasn't helped, then pick ONE node that is misbehaving, look at it using an account that can see the node (and that should NOT have access if that's your problem!), create a new issue in the issues queue, describe the problem... <ul><li> what did you do? </li><li> what did you expect? </li><li> what happened instead? </li></ul> ... and <strong>attach a screenshot of all the DNA records</strong> for that one node. <br /> Be sure to indicate paths (URLs) for every page and module that you mention.") .'</li></ol></div>',
  );
  $form['image_gallery_access']['troubleshooting'][] = array(
    '#type' => 'item',
    '#value' => '<div>'. t("Note: You should not keep the !Devel module enabled on a production site.", $variables) .'</div>',
  );

  if (isset($tid) && !node_access_needs_rebuild()) {
    $count = db_result(db_query("SELECT COUNT(DISTINCT n.nid) FROM {node} n INNER JOIN {term_node} tn ON tn.vid = n.vid WHERE tn.tid = %d", $tid));
    $limit = 20;   // from _node_access_rebuild_batch_operation()
    $threshold = variable_get('image_gallery_access_batch_threshold', $limit);  // change the variable if you want
    $form['image_gallery_access']['update_limit'] = array(
      '#type' => 'value',
      '#value' => $limit,
    );
    $form['image_gallery_access']['update_choice'] = array(
      '#type' => 'radios',
      '#title' => 'Update the permissions',
      '#description' => t('<em>If</em> you make any node access changes, then each node in this gallery needs to be updated. Hover over the radiobuttons for details.'),
      '#options' => NULL,
      0 => array(
        '#type' => 'radio',
        '#title' => t('for all %count nodes immediately', array('%count' => $count)),
        '#attributes' => array('title' => t('This option is the fastest, but with many nodes it can still take considerable time and memory. If it fails, it will leave your !node_access table in an inconsistent state.', array('!node_access' => '{node_access}'))),
        '#return_value' => 0,
        '#default_value' => ($count <= $threshold ? 0 : 1),
        '#parents' => array('image_gallery_access', 'update_choice'),
      ),
      1 => array(
        '#type' => 'radio',
        '#title' => t('in batches of !limit now', array('!limit' => $limit)),
        '#attributes' => array('title' => t('The batch option will always work reliably, but it takes longer to complete.')),
        '#return_value' => 1,
        '#default_value' => ($count <= $threshold ? 0 : 1),
        '#parents' => array('image_gallery_access', 'update_choice'),
      ),
      2 => array(
        '#type' => 'radio',
        '#title' => t('rebuild <strong>all</strong> permissions later'),
        '#attributes' => array('title' => t("This option will only set a flag to remind you to rebuild all permissions later; this is useful if you want to make multiple changes to your node access settings quickly and delay the updating until you're done.")),
        '#return_value' => 2,
        '#default_value' => ($count <= $threshold ? 0 : 1),
        '#parents' => array('image_gallery_access', 'update_choice'),
      ),
      '#attributes' => array('class' => 'image-gallery-access-flowed'),
    );
    $form['image_gallery_access']['force_update'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update even if unchanged'),
    );
  }

  // Move some stuff down so our block goes in a nice place.
  $form['submit']['#weight'] = 10;
  $form['delete']['#weight'] = 10;

  $form['#validate'][] = '_image_gallery_access_form_validate';
  $form['#submit'][] = '_image_gallery_access_form_submit';
}

/**
 * Helper function to return a rid-indexed array of arrays, whose keys
 * are the permissions of the corresponding role.
 */
function _image_gallery_access_get_role_permissions() {
  static $permissions;
  if (empty($permissions)) {
    $permissions[DRUPAL_AUTHENTICATED_RID] = array();
    $result = db_query('SELECT r.rid, p.perm FROM {role} r INNER JOIN {permission} p ON r.rid = p.rid ORDER BY r.rid');
    while ($role = db_fetch_object($result)) {
      $permissions[$role->rid] = ($role->rid == DRUPAL_ANONYMOUS_RID ? array() : $permissions[DRUPAL_AUTHENTICATED_RID]);
      $permissions[$role->rid] += array_flip(explode(', ', $role->perm));
    }
  }
  return $permissions;
}

function _image_gallery_access_admin_form_disable_checkboxes($element) {
  global $user;
  $tr = 't';

  $permissions = _image_gallery_access_get_role_permissions();
  $element_children = element_children($element);
  foreach ($element_children as $rid) {
    if (!isset($permissions[$rid]['access content'])) {
      $element[$rid]['#disabled'] = TRUE;
      $element[$rid]['#default_value'] = FALSE;
      $element[$rid]['#prefix'] = '<span title="'. t("This role does not have the '@perm' permission.", array('@perm' => $tr('access content'))) .'">';
      $element[$rid]['#suffix'] = "</span>";
    }
    if ($element['#parents'][1] == 'view' && isset($permissions[$rid]['administer image galleries'])) {
      $element[$rid]['#title'] = '<em>'. $element[$rid]['#title'] .'</em>';
      $element[$rid]['#prefix'] = '<span title="'. t("This role has the '@perm' permission, and granting '@View' enables the role holders to change the settings on this page, including @Access_control!", array('@perm' => $tr('administer image galleries'), '@View' => t('View'), '@Access_control' => t('Access control'))) .'">';
      if (isset($permissions[$rid]['administer nodes'])) {
        $element[$rid]['#prefix'] = str_replace('">', ' '. t("Because the role also has the '@administer_nodes' permission, it provides full access to all nodes either way.", array('@administer_nodes' => $tr('administer nodes'))) .'">', $element[$rid]['#prefix']);
      }
      $element[$rid]['#suffix'] = "</span>";
    }
    elseif (isset($permissions[$rid]['administer nodes'])) {
      $element[$rid]['#disabled'] = TRUE;
      $element[$rid]['#default_value'] = TRUE;
      $element[$rid]['#prefix'] = '<span title="'. t("This role has the '@perm' permission and thus full access to all nodes.", array('@perm' => $tr('administer nodes'))) .'">';
      $element[$rid]['#suffix'] = "</span>";
    }
  }
  return $element;
}

function _image_gallery_access_admin_form_after_build_acl0($form, $form_state) {
  if (isset($form['#post']['image_gallery_access']['template']['taxonomy'])) {
    // Get ACL's user_list for the template and replace it before ACL's after_build function gets its shot at it.
    $template_tid = reset(array_values($form['#post']['image_gallery_access']['template']['taxonomy']));
    if ($acl_id = db_result(db_query("SELECT acl_id from {acl} WHERE module = 'image_gallery_access' AND name = '%d'", $template_tid))) {
      $f = acl_edit_form($acl_id, 'DUMMY');
      $form['user_list']['#value'] = $f['user_list']['#default_value'];
    }
  }
  return $form;
}

function _image_gallery_access_admin_form_after_build_acl2($form, $form_state) {
  if (!count(unserialize($form['user_list']['#default_value'])) && !count(unserialize($form['user_list']['#value']))) {
    $form['#collapsed'] = TRUE;
  }
  if ($form['user_list']['#default_value'] != $form['user_list']['#value']) {
    $form['note']['#value'] = preg_replace('/<div>/', '<div class="warning">', $form['note']['#value']);
  }
  return $form;
}

function _image_gallery_access_admin_form_after_build($form, &$form_state) {
  if (isset($form_state['clicked_button']['#name']) && $form_state['clicked_button']['#name'] == $form['template']['load_button']['#name']) {
    // Load a setting from a template:
    $template_tid = reset(array_values($form['#post']['image_gallery_access']['template']['taxonomy']));
    $form_state['values']['image_gallery_access']['template']['template_tid'] = $template_tid;
    $form['template']['#collapsed'] = FALSE;

    $settings = _image_gallery_access_get_settings($template_tid);
    foreach (array('view', 'create', 'update', 'delete') as $grant_type) {
      if (empty($form[$grant_type])) {
        continue;
      }
      foreach (element_children($form[$grant_type]) as $tid) {
        $checked = array_search($tid, $settings[$grant_type]) !== FALSE;
        $form[$grant_type][$tid]['#value'] = ($checked ? $tid : 0);
      }
    }
    $form['interference']['advanced']['priority']['#value'] = $settings['priority'];
    if ($settings['priority'] != 0) {
      $form['interference']['advanced']['#collapsed'] = FALSE;
    }
  }
  elseif (is_array(reset($form_state['values']['image_gallery_access']['template']['taxonomy']))) {
    $template_tid = reset(reset($form_state['values']['image_gallery_access']['template']['taxonomy']));
  }
  if (isset($template_tid)) {
    $form['template']['select_by_default']['#value'] = ($template_tid && $template_tid == variable_get('image_gallery_access_default_template_tid', 0));
    $form['template']['load_for_new']['#value'] = ($template_tid && $template_tid == variable_get('image_gallery_access_new_template_tid', 0));
  }
  return $form;
}

function _image_gallery_access_form_validate($form, &$form_state) {
  global $user;

  if ($user->uid == 1) {
    return;
  }
  $access = $form_state['values']['image_gallery_access']; // shortcut
  foreach ($access['view'] as $rid => $checked) {
    if ($checked && isset($user->roles[$rid])) {
      return;
    }
  }
  form_set_error('image_gallery_access][view', t('You must assign %View access to a role that you hold.', array('%View' => 'View')));
}

function _image_gallery_access_form_submit($form, &$form_state) {
  $access = $form_state['values']['image_gallery_access']; // shortcut

  // Save template choice:
  $template_tid = reset(array_values($access['template']['taxonomy']));
  if ($access['template']['select_by_default']) {
    variable_set('image_gallery_access_default_template_tid', $template_tid);
  }
  elseif (variable_get('image_gallery_access_default_template_tid', 0) == $template_tid) {
    variable_del('image_gallery_access_default_template_tid');
  }
  if ($access['template']['load_for_new']) {
    variable_set('image_gallery_access_new_template_tid', $template_tid);
  }
  elseif (variable_get('image_gallery_access_new_template_tid', 0) == $template_tid) {
    variable_del('image_gallery_access_new_template_tid');
  }

  // check for changes
  $is_changed = $is_new = strpos($_GET['q'] .'/', 'admin/content/image/add/') === 0;
  $is_changed = $is_changed || !empty($access['force_update']);
  $form_initial_values = $form;  // avoid Coder warning
  $form_initial_values = $form_initial_values['image_gallery_access'];
  foreach (array('view', 'create', 'update', 'delete') as $grant_type) {
    if (isset($form_initial_values[$grant_type])) {
      $defaults = $form_initial_values[$grant_type]['#default_value'];
      $defaults = array_flip($defaults);
      foreach ($access[$grant_type] as $rid => $checked) {
        $is_changed = $is_changed || (empty($form_initial_values[$grant_type][$rid]['#disabled']) && !empty($checked) != isset($defaults[$rid]));
      }
    }
  }
  if (!$is_changed && $access['acl']['user_list'] == $form_initial_values['acl']['user_list']['#default_value'] && $access['interference']['advanced']['priority'] == $form_initial_values['interference']['advanced']['priority']['#default_value']) {
    drupal_set_message(t('The content access permissions are unchanged.'));
    return;
  }

  $tid = $form_state['values']['tid'];
  db_query("DELETE FROM {image_gallery_access} WHERE tid = %d", $tid);

  $iga_priority = isset($access['interference']['advanced']['priority']) ? $access['interference']['advanced']['priority'] : 0;
  if (array_key_exists('acl', $access)) {
    acl_save_form($access['acl'], $iga_priority);
  }
  $permissions = _image_gallery_access_get_role_permissions();
  foreach ($access['view'] as $rid => $checked) {
    if (!empty($form_initial_values['view'][$rid]['#disabled'])) {
      continue;
    }
    if (isset($permissions[$rid]['administer nodes'])) {
      db_query("INSERT INTO {image_gallery_access} (tid, rid, grant_view, grant_update, grant_delete, grant_create, priority) VALUES (%d, %d, %d, %d, %d, %d, %d)",
        $tid, $rid, 0, 0, 0, !empty($access['create'][$rid]), $fa_priority);
    }
    else {
      db_query("INSERT INTO {image_gallery_access} (tid, rid, grant_view, grant_update, grant_delete, grant_create, priority) VALUES (%d, %d, %d, %d, %d, %d, %d)",
        $tid, $rid, (bool) $checked, !empty($access['update'][$rid]), !empty($access['delete'][$rid]), !empty($access['create'][$rid]), $fa_priority);
    }
  }
  $tr = 't';
  $link = l($tr('edit'), 'admin/content/image/edit/'. $tid);
  watchdog('access', 'Changed grants for %gallery gallery.', array('%gallery' => $form_state['values']['name']), WATCHDOG_NOTICE, $link);

  if (!$is_new) {
    if (!isset($access['update_choice']) || $access['update_choice'] == 2) {
      node_access_needs_rebuild(TRUE);
    }
    elseif ($access['update_choice'] == 0) {
      // update immediately (but use the batch functions anyway
      $save_redirect = $form_state['redirect'];
      $form_state['redirect'] = $_GET['q'];
      $context = array();
      $pending_error_messages = drupal_get_messages('error', FALSE);
      $our_error_message_index = (isset($pending_error_messages['error']) ? count($pending_error_messages['error']) : 0);
      _image_gallery_access_update_batch_finished(FALSE, array(), array());        // add our error message (in case we die underway)
      _image_gallery_access_update_batch_operation($tid, 999999, 1, $context);
      $pending_error_messages = drupal_get_messages('error', TRUE);                // still alive, get and clear all 'error' messages
      unset($pending_error_messages['error'][$our_error_message_index]);           // remove our error message
      foreach ($pending_error_messages['error'] as $message) {                     // replay any others
        drupal_set_message($message, 'error');
      }
      _image_gallery_access_update_batch_finished(TRUE, array(), array());
      $form_state['redirect'] = $save_redirect;
    }
    else {
      // mass update in batch mode, modeled after node.module
      $limit = $access['update_limit'];
      $count = db_result(db_query("SELECT COUNT(DISTINCT n.nid) FROM {node} n INNER JOIN {term_node} tn ON tn.vid = n.vid WHERE tn.tid = %d", $tid));
      $batch = array(
        'title' => t('Updating content access permissions'),
        'file' => drupal_get_path('module', 'image_gallery_access') .'/image_gallery_access.admin.inc',
        'operations' => array(
          array('_image_gallery_access_update_batch_operation', array($tid, $limit, $count)),
        ),
        'finished' => '_image_gallery_access_update_batch_finished'
      );
      batch_set($batch);
    }
  }

  variable_del('image_gallery_access_rids');  // clear cache
}

/**
 * Batch operation for image_gallery_access_form_submit().
 *
 * This is a multistep operation : we go through all nodes by packs of 20.
 * The batch processing engine interrupts processing and sends progress
 * feedback after 1 second execution time.
 */
function _image_gallery_access_update_batch_operation($tid, $limit, $count, &$context) {
  if (empty($context['sandbox'])) {
    // Initiate multistep processing.
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $count;
  }

  // Process the next 20 nodes.
  $result = db_query_range("SELECT DISTINCT n.nid FROM {node} n INNER JOIN {term_node} tn ON tn.vid = n.vid WHERE n.nid > %d AND tn.tid = %d ORDER BY n.nid ASC", $context['sandbox']['current_node'], $tid, 0, $limit);
  while ($row = db_fetch_array($result)) {
    $loaded_node = node_load($row['nid'], NULL, TRUE);
    // To preserve database integrity, only aquire grants if the node
    // loads successfully.
    if (!empty($loaded_node)) {
      node_access_acquire_grants($loaded_node);
    }
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $loaded_node->nid;
  }

  // Multistep processing : report progress.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Post-processing for image_gallery_access_form_submit().
 */
function _image_gallery_access_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The content access permissions have been updated.'));
    cache_clear_all();
  }
  else {
    drupal_set_message(t('The content access permissions have not been properly updated.'), 'error');
  }
}

/**
 * Helper function to retrieve the settings for a gallery.
 */
function _image_gallery_access_get_settings($tid = NULL) {
  $return = array('view' => array(), 'create' => array(), 'update' => array(), 'delete' => array(), 'priority' => 0);
  if (!isset($tid)) {
    // Default to all users can read; all logged in users can post.
    $return['view'] = array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID);
    $return['create'] = array(DRUPAL_AUTHENTICATED_RID);
  }
  else {
    $result = db_query("SELECT * FROM {image_gallery_access} where tid=%d", $tid);
    while ($access = db_fetch_object($result)) {
      $row_received = TRUE;
      if ($access->grant_view) {
        $return['view'][] = $access->rid;
      }
      if ($access->grant_update) {
        $return['update'][] = $access->rid;
      }
      if ($access->grant_delete) {
        $return['delete'][] = $access->rid;
      }
      if ($access->grant_create) {
        $return['create'][] = $access->rid;
      }
      if ($access->rid == DRUPAL_AUTHENTICATED_RID) {  // this is our reference
        $return['priority'] = $access->priority;
      }
    }
    if (empty($row_received)) {
      // Return default settings if no records found in database.
      $return = _image_gallery_access_get_settings();
    }
  }
  return $return;
}

/**
 * We must know when a role is deleted.
 */
function _image_gallery_access_user_admin_role_form(&$form, &$form_state) {
  $form['#submit'][] = '_image_gallery_access_user_admin_role_submit';
}

/**
 * If a role is deleted, we remove the grants it provided.
 */
function _image_gallery_access_user_admin_role_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] == $form_state['values']['delete']) {
    $rid = $form_state['values']['rid'];
    db_query("DELETE FROM {image_gallery_access} WHERE rid = %d", $rid);
    db_query("DELETE FROM {node_access} WHERE gid = %d AND realm = 'image_gallery_access'", $rid);
  }
}

/**
 * Add warnings on Content Access admin forms where CA wants
 * to control the same content types as we do.
 */
function _image_gallery_access_content_access_admin_form() {
  $tr = 't';
  $variables = array(
    '!Content_Access' => 'Content Access',
    '!Image_Gallery_Access' => 'Image Gallery Access',
    '!Image_Gallery_Access_link' => l('Image Gallery Access', 'admin/content/image'),
    '%anonymous_user' => $tr('anonymous user'),
    '%authenticated_user' => $tr('authenticated user'),
    '%Advanced' => $tr('Advanced'),
    '%content_type' => node_get_types('name', 'image'),
  );
  drupal_set_message(t('Note: In Drupal, access can only be granted, not taken away. Whatever access you grant here will not be reflected in the !Image_Gallery_Access_link settings, but !Image_Gallery_Access can only allow <i>more</i> access, not less.', $variables)
    .'<br /><span class="error">'. t('Specifically, any rights granted to the %anonymous_user and/or the %authenticated_user will <b>override</b> the settings of !Image_Gallery_Access!', $variables) .'</span>'
    .'<br />'. t('To avoid conflicts with !Image_Gallery_Access settings, you may want to lower the priority of !Content_Access (under %Advanced below) below the priority of !Image_Gallery_Access for the %content_type content type.', $variables), 'warning');
}
