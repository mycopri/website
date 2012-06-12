<?php
/**
 * Fetch an array of conferences
 *
 * @return
 *   an array of conferences
 *   FALSE if no topics entered
 */
function cms_get_conferences() {
  $sql = "SELECT * FROM cms_event";
  $results = db_query($sql);
  if(mysqli_num_rows($results)) {
    while ($row = db_fetch_object($results)) {
      $conferences[$row->event_id] = $row;
			$sql_node = "SELECT nid FROM cms_nodes WHERE eid = %d";
			$home_nid = db_result(db_query($sql_node, $row->event_id));
			$conferences[$row->event_id]->home_node = node_load($home_nid);
    }
		return $conferences;
  }
  else {
    return FALSE;
  }
}

/**
 * Get a cms conference object
 * 
 * $object->event_id
 * $object->event
 * $object->url
 * $object->override_url
 * $object->publish
 * 
 * @param $criteria
 * 		By default this is the url fragment as defined in the
 * 		cms_event table i.e. $object->url
 * 
 * @param $by_id
 * 		TRUE or FALSE if TRUE then the criteria will be $object->event_id
 * 
 * @return
 * 		A fully populated object as shown above from the cms_event table
 */
function cms_get_conference($criteria, $by_id = FALSE) {
  if ($by_id) {
	  $sql = "SELECT * FROM cms_event WHERE event_id=$criteria";
  }
  else {
  	$sql = "SELECT * FROM cms_event WHERE url='$criteria'";
  }
  $results = db_query($sql);
  if(mysqli_num_rows($results)) {
  	return db_fetch_object($results);
  }
  else {
    return FALSE;
  }
}

/**
 * Fetch an array of topics and subtopics in the conference
 *
 * @return
 *   an array of the topics and subtopics
 *   FALSE if no topics entered
 */
function cms_get_topics() {
	$sql = "SELECT * FROM cms_topic";
	$results = db_query($sql);
	if(mysqli_num_rows($results)) {
		while ($row = db_fetch_object($results)) {
			$topics[$row->topic_id]->title = $row->title;
			if(cms_get_subtopics($row->topic_id)) {
				$topics[$row->topic_id]->subtopics = cms_get_subtopics($row->topic_id);
			}
		}
		return $topics;
	}
	else {
		return FALSE;
	}
}

/*
 * Fetch an array of topics and subtopics in the conference
 *
 * @return
 *   an array of the topics and subtopics
 *   FALSE if no topics entered
 */
function cms_get_topic($topic_id) {
	$sql = "SELECT * FROM cms_topic WHERE topic_id=$topic_id";
	$results = db_query($sql);
	if(mysqli_num_rows($results)) {
		return db_fetch_object($results);
	}
	else {
		return FALSE;
	}
}

function cms_get_subtopic($sub_id = NULL) {
  if (!$sub_id) {
  	$row->sub_id = 0;
  	$row->title = '';
  	$row->topic_id = 0;
  	$row->event_id = 0;
  	return $row;
  }
	$sql = "SELECT * FROM cms_subtopic WHERE sub_id=$sub_id";
  $results = db_query($sql);
  if(mysqli_num_rows($results)) {
    return db_fetch_object($results);
  }
  else {
    return FALSE;
  }
}

/*
 * Fetch an array of sub-topics in the conference
 *
 * @param topic_id
 * @return
 *   an array of subtopics for the given topic_id
 *   FALSE if no topics entered
 */
function cms_get_subtopics($topic_id = NULL) {
	if($topic_id) {
		$sql = "SELECT * FROM cms_subtopic WHERE topic_id = $topic_id";
		$results = db_query($sql);
		if (mysqli_num_rows($results)) {
			while ($row = db_fetch_object($results)) {
				$subtopics[$row->sub_id] = $row->title;
			}
		}
		return $subtopics;
	}
	else {
		return FALSE;
	}
}

/**
 * 
 * @param $event_id
 *   the event id of the conference
 * &return
 *   an array of abstracts (full node objects)
 */
function cms_get_abstracts($event_id) {
	$sql = "SELECT cms.* FROM cms_abstract cms 
	         WHERE cms.event_id = $event_id";
	$results = db_query($sql);
	
	while($row = db_fetch_object($results)) {
		$abstracts[$row->abstract_nid] = node_load($row->abstract_nid);
	}
	return $abstracts;
}

/**
 * Perform several queries and provide abstract submission
 * statistics for a specific conference
 * 
 * @param $event_id
 */
function cms_get_abstracts_summary($event_id) {
	$abstracts = cms_get_abstracts($event_id);
	$info->total_abstracts = count($abstracts);
	
	// Get count of topics
	$sql = "SELECT count(topic) as topic_count, topic " .
	         "FROM `cms_abstract` " .
	         "WHERE event_id = $event_id GROUP BY topic";
  $results = db_query($sql);
  while ($row = db_fetch_object($results)) {
  	$topic = cms_get_topic($row->topic);
  	if ($row->topic == '0') {
  		$topic->title = 'Unspecified';
  	}
  	$counts[$topic->title] = array(
  	  'count' => $row->topic_count,
  	  'id' => $row->topic,
  	);
  }
	$info->topics = $counts;
	
	$counts = array();
	// Get count of subtopics
	$sql = "SELECT count(subtopic) as subtopic_count, subtopic " .
	         "FROM `cms_abstract` " .
	         "WHERE event_id = $event_id GROUP BY subtopic";
  $results = db_query($sql);
  while ($row = db_fetch_object($results)) {
    $topic = cms_get_subtopic($row->subtopic);
    if ($row->subtopic == '0') {
      $topic->title = 'Unspecified';
    }
    $counts[$topic->title] = array(
      'count' => $row->subtopic_count,
      'id' => $row->subtopic,
    );
  }
  $info->subtopics = $counts;
	return $info;
}

function cms_get_abstracts_by($event_id, $value, $field = 'topic') {
	$sql = "SELECT * FROM cms_abstract WHERE event_id = $event_id AND $field = $value";
  $results = db_query($sql);
  while ($row = db_fetch_object($results)) {
  	$abstracts[$row->abstract_nid] = node_load($row->abstract_nid);
  }
  return $abstracts;
}

function cms_get_author($aid) {
	$sql = "SELECT * FROM cms_authors WHERE aid = $aid";
  $results = db_query($sql);
  return db_fetch_object($results);
}

function cms_get_authors($id) {
	$abstract = node_load($id);
  $submitter = user_load($abstract->uid);
  $author->first = $submitter->profile_first_name;
  $author->last = $submitter->profile_last_name;
  $author->credentials = $submitter->profile_credentials;
  $authors[$submitter->uid] = $author;
	$sql = "SELECT * FROM cms_abstract_authors WHERE abstract_nid = $id";
  $results = db_query($sql);
  while($row = db_fetch_object($results)) {
  	$author = cms_get_author($row->author_id);
  	$authors[$row->author_id] = $author;
  }
  return $authors;
}

function cms_get_submissions($uid) {
	$sql = "SELECT * FROM cms_abstract
	         JOIN node ON node.nid = cms_abstract.abstract_nid
	         WHERE node.uid = $uid";
	$results = db_query($sql);
	while($row = db_fetch_object($results)) {
		$submissions[$row->abstract_nid] = $row;
	}
	return $submissions;
}

function cms_get_tabs($eid) {
	$sql = "SELECT * FROM cms_tabs WHERE eid = %d ORDER BY weight ASC";
	$results = db_query($sql, $eid);
	while($row = db_fetch_object($results)) {
		$tabs[$row->tid] = $row;
	}
	return $tabs;	
}

/**
 *  Get a node object for a conference page
 * 
 * @param $url
 * 		The defined fragment of the conference url e.g.
 * 		ci-asce.org/conferences/{fragment}/welcome. Fragments
 * 		are defined in the cms_events table
 * 
 * @param $tab
 * 		The type of page that is to be retrieved i.e.
 * 		welcome, call, venue. This is defined in the cms_nodes
 * 		table.
 * 
 * @return
 * 		A fully populated node object representing the page
 */
function cms_get_node_page($url, $tab = NULL) {
	$conference = cms_get_conference($url);
	$sql = "SELECT * FROM cms_nodes WHERE eid = %d AND type = '%s'";
	$row = db_fetch_object(db_query($sql, $conference->event_id, $tab));
	return node_load($row->nid);
}
