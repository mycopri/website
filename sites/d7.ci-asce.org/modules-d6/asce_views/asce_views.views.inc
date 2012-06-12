<?php
// $Id$

/**
 * Implementation of hook_views().
 */
function asce_views_views_data() {
  $data['og_uid_titles']['table']['base'] = array(
       'field' => 'nid',
       'title' => 'OG Titles',
       'help' => t("OG Titles tables"),
  );
  
  $data['node']['table']['join'] = array(
       'og_uid_titles' => array(
           'left_field' => 'uid',
           'field' => 'uid',
           'type' => 'INNER',
       ),
  );
 $data['users']['table']['join'] = array(
       'og_uid_titles' => array (
           'left_field' => 'uid',
           'field' => 'uid',
           'type' => 'INNER',
       ),
  );  
  return $data;
}