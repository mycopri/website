<?php

function asce_image_attach_image_to_node_form(&$form_state, $update_node) {
	// Get images that are already attached to the node
  $result = db_query("SELECT * FROM image_attach WHERE nid = %d", $update_node->nid);
  $images = array();
  while($image = db_fetch_object($result)) {
    $images[$image->iid] = $image->iid;
  }
      
  $result = pager_query('SELECT i.nid, f.*, n.title FROM ({image} i INNER JOIN {files} f ON i.fid = f.fid) INNER JOIN {node} n ON n.nid = i.nid WHERE i.image_size = \'%s\'', 25, 0, NULL, 'thumbnail');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Attach Selected Images'),
  );
  
  $destination = drupal_get_destination();
  $nodes = array();
  while ($node = db_fetch_object($result)) {
    // Don't include those images that area already attached to the node
    if (!in_array($node->nid, $images)) {
      $nodes[$node->nid] = '';
      $title = '<a href="/node/' . $node->nid . '">';
      $title .= theme('image', $node->filepath, $node->title, NULL, TRUE);
      $title .= '</a>';
      //$form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid));
      $form['title'][$node->nid] = array('#value' => $title);
      $form['titles'][$node->nid] = array('#value' => $node->title);
    }
  }
  $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  $form['pager'] = array('#value' => theme('pager', NULL, 25, 0));
  $form['#theme'] = 'asce_attach_images';
  $form['#node'] = $update_node;
  return $form;
	}

function asce_image_attach_image_to_node_form_submit($form, &$form_state) {
	$update_node = $form['#node'];
	$titles = $form['titles'];
	// Filter out unchecked nodes
  $nodes = array_filter($form_state['values']['nodes']);
  if ($nodes) {
  	foreach($nodes as $key => $value) {
  		$row = array('nid' => $update_node->nid, 'iid' => $key);
  		drupal_write_record('image_attach', $row);
  		drupal_set_message('<i>' . $titles[$key]['#value'] . '</i> was attached to <i>' . $update_node->title . '</i>.');
  	}
  }
  $form_state['redirect'] = 'node/'. $update_node->nid;
  //dsm(debug_backtrace());
  //dsm($form_state);
}

/**
 * Theme select images page.
 *
 * @ingroup themeable
 */
function theme_asce_image_attach_image_to_node_form($form) {
  $has_posts = isset($form['title']) && is_array($form['title']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Image'), t('Title'));
  $output = '';

  if ($has_posts) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      $row[] = drupal_render($form['nodes'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['titles'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No images available.'), 'colspan' => '2'));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

























