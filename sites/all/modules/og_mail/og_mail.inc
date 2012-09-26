<?php

/**
 * @file
 * Helper functions to add with the database operations when queuing events.
 * This include file is only called when og_mail_event is invoked.
 */

/**
 * Helper function to check and see if the node being updated
 * wasn't just recently created. We want to keep new posts
 * as new because sometimes the author will updated it right away.
 * 
 * @param type $node
 */
function _node_is_new($node) {
  $result = db_select('og_mail_events', 'events')
          -> fields('events', array('id', 'nid', 'event'))
          -> condition('nid', $node->nid, '=')
          -> condition('event', 'create', '=')
          ->execute()
          ->fetchAssoc();
  if($result) {
    return TRUE;
  }
}

function _node_in_group($node) {
  if (count($node->group_audience['und']) > 0) {
    return TRUE;
  }
}

/**
 * Save group event
 * 
 * @param type $node
 * @param type $event
 * @return the primary key or id of the event
 */
function _og_mail_save_event($object, $type, $event) {
  // Check to see if the node or object is already in the table
  $query = db_select('og_mail_events', 'events')
          -> fields('events', array('id', 'nid', 'event'))
          -> condition('nid', $object->nid)
          -> condition('type', $type)
          -> condition('event', $event);
  
  if($type == 'comment') {
    $query->condition('cid', $object->cid, '=');
  }
  $event_row = $query->execute()-> fetchAssoc();
  
  if ($type == 'node') {
    // We need to do this to make sure there is no confusion that this is not a
    // comment entity
    $cid = NULL; 
  }
  elseif ($type == 'comment') {
    $cid = $object->cid;
  }
  
  if (isset($event_row['id'])) {
    if ($event == 'update') {
      db_update('og_mail_events')
            ->fields(array(
              'created' => REQUEST_TIME,
            ))
            -> condition('id', $event_row['id'])
            -> execute();
      $event_id = $event_row['id'];
    }
  }
  else {
    $event_id = db_insert('og_mail_events')
          ->fields(array(
          'nid' => $object->nid,
          'cid' => $cid,
          'type' => $type,
          'event' => $event,
          'created' => REQUEST_TIME,
          ))
        ->execute();
  }
  return $event_id;
}

/**
 * 
 * @param type $node
 * @param type $event_id
 * @return type
 */
function _og_mail_queue($object, $event_id) {
  if (isset($object->cid)) {
    $node = node_load($object->nid);
  }
  else {
    $node = $object;
  }
  $users = _get_user_list($node);
  
  // Create an entry for every user in the og_mail
  // table
  foreach ($users as $uid => $mail) {
    // Check to see if this activity is already queued
    $queue_row = db_select('og_mail', 'mail')
          -> fields('mail', array('event_id', 'uid'))
          -> condition('event_id', $event_id, '=')
          -> condition('uid', $uid, '=')
          ->execute()
          ->fetchAssoc();
    if (!$queue_row) {
      db_insert('og_mail')
          ->fields(array(
          'event_id' => $event_id,
          'uid' => $uid,
          'sent' => 0,
          ))
        ->execute();
    }
  }
}

/**
 * Query the og_membership to get a list of users who belong
 * to the group/s that the node belongs to
 * 
 * @param type $node
 * @return an array of users
 */
function _get_user_list($node) {
  foreach($node->group_audience['und'] as $group) {
    $groups[$group['gid']]['state'] = $group['state'];
    $groups[$group['gid']]['created'] = $group['created'];
  }
  foreach ($groups as $gid => $group) {
    $result = db_select('og_membership', 'og')
            ->fields('og', array('etid'))
            ->condition('gid', $gid, '=')
            ->condition('entity_type', 'user', '=')
            ->execute();
    $users = array();
    foreach($result as $row) {
      $account = user_load($row->etid);
      if(!is_null($row->etid)) {
        $users[$account->uid] = $account->mail;
      }
    }
  }
  return $users;
}

function _og_mail_send() {
  // We will need to do this step by step, instead of sending an email
  // I'll concentrate first on just displaying a message and the flow
  // First step will be to check and see when was the last time an mail
  // was sent to the specific user.
  global $user;
  
  if (!user_access('og_mail admin')) {
    return;
  }
  
  $account = $user;
  $params['account'] = $account;  
  $params = array_merge($params, og_mail_prepare_message($account));
  $query = db_select('og_mail', 'mail')
          ->fields('mail', array('uid'))
          ->distinct()
          ->execute();
  $uids = $query->fetchAll();
  //dsm($uids);
  foreach($uids as $uid) {
    $account = user_load($uid->uid);
    $params['recipient'] = $account;

    $mail_success = drupal_mail('og_mail', 'og_mail', $account->mail, user_preferred_language($account), $params);
    if ($mail_success) {
      // Check to see if user exists
      $query = db_select('og_mail_user', 'user')
              -> fields('user', array('uid'))
              -> condition('uid', $account->uid)
              -> execute();
      $results = $query->fetchAssoc();
      // Let's get the timestamp and save it
      if(!$results) {
        db_insert('og_mail_user')
          ->fields(array(
            'uid' => $account->uid,
            'timestamp' => REQUEST_TIME,
            ))
        ->execute();
      }
      else {
        db_update('og_mail_user')
          -> fields(array(
             'timestamp' => REQUEST_TIME
             ))
          -> condition('uid', $account->uid)
          -> execute();
      }
      $num_deleted = db_delete('og_mail')
        ->condition('uid', $account->uid)
        ->execute();
    }
  }
}

function _og_mail_get_user_queue($account) {
  // Get the list of notifications for the user
  $query = db_select('og_mail', 'mail')
          -> fields('mail', array('event_id', 'id'))
          -> condition('uid', $account->uid)
          -> condition('sent', 0);
  $query->join('og_mail_events', 'events', 'mail.event_id = events.id');
  $query->addField('events', 'nid');
  $query->addField('events', 'cid');
  $query->addField('events', 'type');
  $query->addField('events','event');
  $query->addField('events','created');
  $query_object = $query->execute();
  $queue = $query_object->fetchAll();
  return $queue;
}

function og_mail_prepare_message($account) {
  global $base_url;
  
  $notifications = _og_mail_get_user_queue($account);
  dsm($notifications);
  
  $new_posts = array();
  $updated_posts = array();
  $comments = array();
  
  if($notifications) {
    foreach($notifications as $key => $notification) {
      $time_interval = '';
      if(!is_null($notification->cid)) {
        $object = comment_load($notification->cid);
      }
      else {
        $object = node_load($notification->nid);
      }
      $time_delta = time() - $object->changed;
      
      if ($time_delta < 1) {
        $time_interval = "just now.";
      }
      else {
        $time_interval = format_interval(time() - $object->changed, 2);
      }
      $author = user_load($object->uid);
      if ($notification->event == 'create') {
        if ($notification->type == 'node') {
          $new_posts[$key]['@title'] = l($object->title, $base_url . '/node/' . $object->nid, array('html' => TRUE));
          $new_posts[$key]['@author'] = $author->name;
          $new_posts[$key]['@time'] = $time_interval;
        }
        elseif ($notification->type == 'comment') {
          $node = node_load($object->nid);
          $new_comments[$key]['@node_title'] = l($node->title, $base_url . '/node/' . $node->nid, array('html' => TRUE));
          $new_comments[$key]['@comment_subject'] = l($object->subject, $base_url . '/node/' . $node->nid, array('html' => TRUE, 'fragment' => 'comment-' . $object->cid));
          $new_comments[$key]['@author'] = $author->name;
          $new_comments[$key]['@time'] = $time_interval;
        }
      }
      elseif ($notification->event == 'update') {
        $updated_posts[$key]['@title'] = l($object->title, $base_url . '/node/' . $object->nid, array('html' => TRUE));
        $updated_posts[$key]['@author'] = $author->name;
        $updated_posts[$key]['@time'] = $time_interval;
      }
      
    }
  }
  $params['new'] = $new_posts;
  $params['updated'] = $updated_posts;
  $params['comments'] = $comments;
  
  return $params;
}