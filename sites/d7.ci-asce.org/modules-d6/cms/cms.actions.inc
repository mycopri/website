<?php

/**
 *  Assign a topic to an abstract. Drupal action.
 */
function assign_topic_action($object, $context) {
	$query = 'op=' . $context['op'];
	//$conference = cms_get_conference($object->event, TRUE);
	drupal_goto('conferences/' . $conference->url . '/abstract/' . $object->nid . '/topic', 'op=' . $context['op']);
}

