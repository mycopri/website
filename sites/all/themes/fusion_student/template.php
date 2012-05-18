<?php
/**
 * Block preprocessing
 */
function fusion_student_preprocess_block(&$vars) {
  global $theme_info, $user;
  static $regions, $sidebar_first_width, $sidebar_last_width, $grid_name, $grid_width, $grid_fixed;
  $vars['debug_block_first'] = kpr($vars, TRUE);
  // Initialize block region grid info once per page
  if (!isset($regions)) {
    $grid_name = substr(theme_get_setting('theme_grid'), 0, 7);
    $grid_width = (int)substr($grid_name, 4, 2);
    $grid_fixed = (substr(theme_get_setting('theme_grid'), 7) != 'fluid') ? 1 : 0;
    $sidebar_first_width = (block_list('sidebar_first')) ? theme_get_setting('sidebar_first_width') : 0;
    $sidebar_last_width = (block_list('sidebar_last')) ? theme_get_setting('sidebar_last_width') : 0;
    $regions = fusion_core_set_regions($grid_width, $sidebar_first_width, $sidebar_last_width);
  }

  // Increment block count for current block's region, add first/last position class
  $regions[$vars['block']->region]['count'] ++;
  $region_count = $regions[$vars['block']->region]['count'];
  $total_blocks = $regions[$vars['block']->region]['total'];
  $vars['position'] = ($region_count == 1) ? 'first' : '';
  $vars['position'] .= ($region_count == $total_blocks) ? ' last' : '';

  // Set a default block width if not already set by Skinr
  if (!isset($vars['skinr']) || (strpos($vars['skinr'], $grid_name) === false)) {
    // Stack blocks vertically in sidebars by setting to full sidebar width
    if ($vars['block']->region == 'sidebar_first') {
      $width = $sidebar_first_width;
    }
    elseif ($vars['block']->region == 'sidebar_last') {
      $width = $sidebar_last_width;
    }
    else {
      // Default block width = region width divided by total blocks, adding any extra width to last block
      $region_width = ($grid_fixed) ? $regions[$vars['block']->region]['width'] : $grid_width;  // fluid grid regions = 100%
      $width_adjust = (($region_count == $total_blocks) && ($region_width % $total_blocks)) ? $region_width % $total_blocks : 0;
      $width = ($total_blocks) ? floor($region_width / $total_blocks) + $width_adjust : 0;
    }
    $vars['skinr'] = (isset($vars['skinr'])) ? $vars['skinr'] . ' ' . $grid_name . $width : $grid_name . $width;
  }

  if (isset($vars['skinr']) && (strpos($vars['skinr'], 'superfish') !== false) &&
     ($vars['block']->module == 'menu' || ($vars['block']->module == 'user' && $vars['block']->delta == 1))) {
    $superfish = ' sf-menu';
    $superfish .= (strpos($vars['skinr'], 'superfish-vertical')) ? ' sf-vertical' : '';
    $vars['block']->content = preg_replace('/<ul class="menu/i', '<ul class="menu' . $superfish, $vars['block']->content, 1);
  }

  // Add block edit links for admins
  if (user_access('administer blocks', $user) && theme_get_setting('block_config_link')) {
    $vars['edit_links'] = '<div class="fusion-edit">'. implode(' ', fusion_core_edit_links($vars['block'])) .'</div>';
  }
  $vars['debug_block_done'] = kpr($vars, TRUE);
}

/**
 * Page preprocessing
 */
function fusion_student_preprocess_page(&$vars) {
  global $language, $theme_key, $theme_info, $user;
  // Remove sidebars if disabled e.g., for Panels
  if (!$vars['show_blocks']) {
    $vars['sidebar_first'] = '';
    $vars['sidebar_last'] = '';
  }
  // Set grid info & row widths
  $grid_name = substr(theme_get_setting('theme_grid'), 0, 7);
  $grid_type = substr(theme_get_setting('theme_grid'), 7);
  $grid_width = (int)substr($grid_name, 4, 2);
  $vars['grid_width'] = $grid_name . $grid_width;
  $sidebar_first_width = ($vars['sidebar_first']) ? theme_get_setting('sidebar_first_width') : 0;
  $sidebar_last_width = ($vars['sidebar_last']) ? theme_get_setting('sidebar_last_width') : 0;
  $vars['sidebar_first_width'] = $grid_name . $sidebar_first_width;
  $vars['main_group_width'] = $grid_name . ($grid_width - $sidebar_first_width);
  // For nested elements in a fluid grid calculate % widths & add inline
  if ($grid_type == 'fluid') {
    $sidebar_last_width = round(($sidebar_last_width/($grid_width - $sidebar_first_width)) * 100, 2);
    $vars['content_group_width'] = '" style="width:' . (100 - $sidebar_last_width) . '%';
    $vars['sidebar_last_width'] = '" style="width:' . $sidebar_last_width . '%';
  }
  else {
    $vars['content_group_width'] = $grid_name . ($grid_width - ($sidebar_first_width + $sidebar_last_width));
    $vars['sidebar_last_width'] = $grid_name . $sidebar_last_width;
  }

  // Add to array of helpful body classes
  $body_classes = explode(' ', $vars['body_classes']);                                               // Default classes
  if (isset($vars['node'])) {
    $body_classes[] = ($vars['node']) ? 'full-node' : '';                                            // Full node
    $body_classes[] = (($vars['node']->type == 'forum') || (arg(0) == 'forum')) ? 'forum' : '';      // Forum page
  }
  else {
    $body_classes[] = (arg(0) == 'forum') ? 'forum' : '';                                            // Forum page
  }
  if (module_exists('panels') && function_exists('panels_get_current_page_display')) {               // Panels page
    $body_classes[] = (panels_get_current_page_display()) ? 'panels' : '';
  }
  $body_classes[] = 'layout-'. (($vars['sidebar_first']) ? 'first-main' : 'main') . (($vars['sidebar_last']) ? '-last' : '');  // Sidebars active
  $body_classes[] = theme_get_setting('sidebar_layout');                                             // Sidebar layout
  $body_classes[] = (theme_get_setting('theme_font') != 'none') ? theme_get_setting('theme_font') : '';                        // Font family
  $body_classes[] = theme_get_setting('theme_font_size');                                            // Font size
  $body_classes[] = (user_access('administer blocks', $user) && theme_get_setting('grid_mask')) ? 'grid-mask-enabled' : '';    // Grid mask overlay
  $body_classes[] = 'grid-type-' . $grid_type;                                                       // Fixed width or fluid
  $body_classes[] = 'grid-width-' . $grid_width;                                                     // Grid width in units
  $body_classes[] = ($grid_type == 'fluid') ? theme_get_setting('fluid_grid_width') : '';            // Fluid grid width in %
  $body_classes = array_filter($body_classes);                                                       // Remove empty elements
  $vars['body_classes'] = implode(' ', $body_classes);                                               // Create class list separated by spaces
  $vars['body_id'] = 'pid-' . strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', drupal_get_path_alias($_GET['q'])));            // Add a unique page id

  // Generate links tree & add Superfish class if dropdown enabled, else make standard primary links
  $vars['primary_links_tree'] = '';
  if ($vars['primary_links']) {
    if (theme_get_setting('primary_menu_dropdown') == 1) {
      $vars['primary_links_tree'] = menu_tree(variable_get('menu_primary_links_source', 'primary-links'));
      $vars['primary_links_tree'] = preg_replace('/<ul class="menu/i', '<ul class="menu sf-menu', $vars['primary_links_tree'], 1);
    }
    else {
      $vars['primary_links_tree'] = theme('links', $vars['primary_links'], array('class' => 'menu'));
    }
  }

  // Remove breadcrumbs if disabled
  if (theme_get_setting('breadcrumb_display') == 0) {
    $vars['breadcrumb'] = '';
  }

  // Add grid, color, ie6, ie7, ie8 & local stylesheets, including inherited & rtl versions
  $grid_style = '/css/' . theme_get_setting('theme_grid');
  $themes = fusion_core_theme_paths($theme_key);
  $vars['setting_styles'] = $vars['ie6_styles'] = $vars['ie7_styles'] = $vars['ie8_styles'] = $vars['local_styles'] = '';
  foreach ($themes as $name => $path) {
    $link = '<link type="text/css" rel="stylesheet" media="all" href="' . base_path() . $path;
    $vars['setting_styles'] .= (file_exists($path . $grid_style . '.css')) ? $link . $grid_style . '.css" />' . "\n" : '';
    $vars['ie6_styles'] .= (file_exists($path . '/css/ie6-fixes.css')) ? $link . '/css/ie6-fixes.css" />' . "\n" : '';
    $vars['ie7_styles'] .= (file_exists($path . '/css/ie7-fixes.css')) ? $link . '/css/ie7-fixes.css" />' . "\n" : '';
    $vars['ie8_styles'] .= (file_exists($path . '/css/ie8-fixes.css')) ? $link . '/css/ie8-fixes.css" />' . "\n" : '';
    $vars['local_styles'] .= (file_exists($path . '/css/local.css')) ? $link . '/css/local.css" />' . "\n" : '';
    if (defined('LANGUAGE_RTL') && $language->direction == LANGUAGE_RTL) {
      $vars['setting_styles'] .= (file_exists($path . $grid_style . '-rtl.css')) ? $link . $grid_style . '-rtl.css" />' . "\n" : '';
      $vars['ie6_styles'] .= (file_exists($path . '/css/ie6-fixes-rtl.css')) ? $link . '/css/ie6-fixes-rtl.css" />' . "\n" : '';
      $vars['ie7_styles'] .= (file_exists($path . '/css/ie7-fixes-rtl.css')) ? $link . '/css/ie7-fixes-rtl.css" />' . "\n" : '';
      $vars['ie8_styles'] .= (file_exists($path . '/css/ie8-fixes-rtl.css')) ? $link . '/css/ie8-fixes-rtl.css" />' . "\n" : '';
      $vars['local_styles'] .= (file_exists($path . '/css/local-rtl.css')) ? $link . '/css/local-rtl.css" />' . "\n" : '';
    }
  }

  // Use grouped import setting to avoid 30 stylesheet limit in IE
  if (theme_get_setting('fix_css_limit') && !variable_get('preprocess_css', FALSE)) {
    $css = drupal_add_css();
    $style_count = substr_count($vars['setting_styles'] . $vars['ie6_styles'] . $vars['ie7_styles'] . $vars['ie8_styles'] . $vars['local_styles'], '<link');
    if (fusion_core_css_count($css) > (30 - $style_count)) {
      $styles = '';
      $suffix = "\n".'</style>'."\n";
      foreach ($css as $media => $types) {
        $prefix = '<style type="text/css" media="'. $media .'">'."\n";
        $imports = array();
        foreach ($types as $files) {
          foreach ($files as $file => $preprocess) {
            $imports[] = '@import "'. base_path() . $file .'";';
            if (count($imports) == 30) {
              $styles .= $prefix . implode("\n", $imports) . $suffix;
              $imports = array();
            }
          }
        }
        $styles .= (count($imports) > 0) ? ($prefix . implode("\n", $imports) . $suffix) : '';
      }
      $vars['styles'] = $styles;
    }
  }
}