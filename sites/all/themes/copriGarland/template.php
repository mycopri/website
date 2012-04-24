<?php
// $Id: template.php,v 1.16.2.3 2010/05/11 09:41:22 goba Exp $

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($left, $right) {
  if ($left != '' && $right != '') {
    $class = 'sidebars';
  }
  else {
    if ($left != '') {
      $class = 'sidebar-left';
    }
    if ($right != '') {
      $class = 'sidebar-right';
    }
  }

  if (isset($class)) {
    print ' class="'. $class .'"';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    /**
	/*orignal function code
	return '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
	*/
  	if (isset($_GET['team'])) {
  		$node = node_load($_GET['team']);
  		$breadcrumb[] = l($node->title, 'node/' . $_GET['team']);
  	}
  	
  	if (arg(0) == 'og' && arg(1) =='users') {
  		$group_node = node_load(arg(2));
  		$breadcrumb = asce_groups_get_breadcrumb($group_node);
  	}
  	
		return '<div class="breadcrumb">'. implode(' ', $breadcrumb) .'</div>';
  }
  
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function phptemplate_preprocess_page(&$vars) {
  $vars['tabs2'] = menu_secondary_local_tasks();
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

/**
 * Add a "Comments" heading above comments except on forum pages.
 */
function garland_preprocess_comment_wrapper(&$vars) {
  if ($vars['content'] && $vars['node']->type != 'forum') {
    $vars['content'] = '<h2 class="comments">'. t('Comments') .'</h2>'.  $vars['content'];
  }
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs. Overridden to split the secondary tasks.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  return menu_primary_local_tasks();
}

/**
 * Returns the themed submitted-by string for the comment.
 */
function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

/**
 * Returns the themed submitted-by string for the node.
 */
function phptemplate_node_submitted($node) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function phptemplate_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/fix-ie.css" />';
  if ($language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}

/**
 * Draw the flexible layout.
 */
function phptemplate_panels_flexible($id, $content, $settings, $display, $layout, $handler) {
  panels_flexible_convert_settings($settings, $layout);

  $renderer = panels_flexible_create_renderer(FALSE, $id, $content, $settings, $display, $layout, $handler);

  // CSS must be generated because it reports back left/middle/right
  // positions.
  $css = panels_flexible_render_css($renderer);

  if (!empty($renderer->css_cache_name) && empty($display->editing_layout)) {
    ctools_include('css');
    // Generate an id based upon rows + columns:
    $filename = ctools_css_retrieve($renderer->css_cache_name);
    if (!$filename) {
      $filename = ctools_css_store($renderer->css_cache_name, $css, FALSE);
    }

    // Give the CSS to the renderer to put where it wants.
    if ($handler) {
      $handler->add_css($filename, 'module', 'all', FALSE);
    }
    else {
      ctools_css_add_css($filename, 'module', 'all', FALSE);
    }
  }
  else {
    // If the id is 'new' we can't reliably cache the CSS in the filesystem
    // because the display does not truly exist, so we'll stick it in the
    // head tag. We also do this if we've been told we're in the layout
    // editor so that it always gets fresh CSS.
    drupal_set_html_head("<style type=\"text/css\">\n$css</style>\n");
  }

  // Also store the CSS on the display in case the live preview or something
  // needs it
  $display->add_css = $css;

  $output = "<div class=\"panel-flexible " . $renderer->base['canvas'] . " clear-block\" $renderer->id_str>\n";
  //$output .= kpr(get_defined_vars(), TRUE);
  $destination = drupal_get_destination();
  switch ($id) {
  	case 'mini-panel-action_item': 
      //$output .= '<a href="add/blog-post?gids[]=' . arg(1) . '&op=action&' . $destination . '">Add an Action Item</a>';
      break;
  	case 'mini-panel-comm_discussion_tab':
  		//$output .= '<a href="add/blog-post?gids[]=' . arg(1) . '&op=post&' . $destination . '">Add a Post</a>';
  		break;
  	case 'mini-panel-comm_members_tab':
  		//$output .= '<a href="add/blog-post?gids[]=' . arg(1) . '&' . $destination . '">Add a Member</a>';
  		break;
  }
  $output .= "<div class=\"panel-flexible-inside " . $renderer->base['canvas'] . "-inside\">\n";

  $output .= panels_flexible_render_items($renderer, $settings['items']['canvas']['children'], $renderer->base['canvas']);

  // Wrap the whole thing up nice and snug
  $output .= "</div>\n</div>\n";

  return $output;
}

/**
 * Process variables for node.tpl.php
 *
 * Most themes utilize their own copy of node.tpl.php. The default is located
 * inside "modules/node/node.tpl.php". Look in there for the full list of
 * variables.
 *
 * The $variables array contains the following arguments:
 * - $node
 * - $teaser
 * - $page
 *
 * @see node.tpl.php
 */
function copriGarland_preprocess_node(&$variables) {
  $node = $variables['node'];
  $variables['template_files'][] = 'node-'. $node->nid;
}