<?php
/**
 * Implementation of hook_views_data().
 *
 * Exposes all fields to the views system.
 */
function asce_cck_views_data() {
	$path = drupal_get_path('module', 'content') . '/includes/views/';
  include_once $path . 'content.views.inc';
  
	$data = array();
	$i = 0;
	$fields = asce_cck_fields();
	$custom_fields = array('ls', 'emi', 'ewri');
	foreach($fields as $key => $field) {
		$keyparts = explode('_', $key);
		if (!in_array($keyparts[1], $custom_fields)) {
			unset($fields[$key]);
		}
	}
	//drupal_set_message('$fields' . kpr($fields, TRUE));
	foreach ($fields as $key => $field) {
		$module = $field['module'];
		$result = (array) module_invoke($module, 'field_settings', 'views data', $field);
		drupal_alter('field_settings', $result, 'views data', $field);
		if (empty($result)) {
			//drupal_set_message($key . ' did not use field_settings...' . $module);
			$result = content_views_field_views_data($field);
		}
		if (is_array($result)) {
			$data = array_merge($data, $result);
		}
	}
	//dsm($data);
	foreach($data as $key => $value) {
		$keyparts = explode('_', $key);
		//dsm($keyparts);
		switch($keyparts[3]) {
			case 'ls':
				$group = 'Living Shorelines';
				break;
			case 'emi':
				$group = 'Engineering Mechanics Institute';
				break;
			case 'ewri':
				$group = '2012 EWRI Congress Session Proposals';	
		}
		$new_key = str_replace('node_data_', 'asce_cck_data_', $key);
		$new_data[$new_key] = $value;
		foreach ($new_data[$new_key] as $key_2 => $value) {
			$new_data[$new_key][$key_2]['group'] = $group;
		}
	}
	//dsm($new_data);
	return $new_data;
}

/**
 * Return a list of all fields.
 *
 * @param $field_name
 *   If not empty, return information on just this field.
 * @param $content_type_name
 *   If not empty, return information of the field within the context of this content
 *   type.
 *
 * Be sure to check empty() instead of isset() on field_name and
 * content_type_name to avoid bad results when the value is set
 * but empty, as sometimes happens in the formatter.
 */
function asce_cck_fields($field_name = NULL, $content_type_name = NULL) {
  $info = _asce_cck_type_info();
  //drupal_set_message('$info' . kpr($info, TRUE));
  if (isset($info['fields'])) {
    if (empty($field_name)) {
      return $info['fields'];
    }
    if (isset($info['fields'][$field_name])) {
      if (empty($content_type_name)) {
        return $info['fields'][$field_name];
      }
      if (isset($info['content types'][$content_type_name]['fields'][$field_name])) {
        return $info['content types'][$content_type_name]['fields'][$field_name];
      }
    }
  }
}

/**
 * Collate all information on content types, fields, and related structures.
 *
 * @param $reset
 *   If TRUE, clear the cache and fetch the information from the database again.
 */
function _asce_cck_type_info($reset = FALSE) {
  global $language;
  static $info;

  if ($reset || !isset($info)) {
    // Make sure this function doesn't run until the tables have been created,
    // For instance: when first enabled and called from content_menu(),
    // or when uninstalled and some subsequent field module uninstall
    // attempts to refresh the data.

    // Don't try this before content module's update is run
    // to add module and active columns to the table.
    if (variable_get('content_schema_version', -1) < 6007) {
      return array();
    }

    if (!$reset && $cached = cache_get('content_type_info:'. $language->language, content_cache_tablename())) {
      $info = $cached->data;
    }
    else {
      $info = array(
        'field types' => array(),
        'widget types' => array(),
        'fields' => array(),
        'content types' => array(),
      );

      // Populate field types.
      foreach (module_list() as $module) {
        $module_field_types = module_invoke($module, 'field_info');
        if ($module_field_types) {
          foreach ($module_field_types as $name => $field_info) {
            // Truncate names to match the value that is stored in the database.
            $db_name = substr($name, 0, 32);
            $info['field types'][$db_name] = $field_info;
            $info['field types'][$db_name]['module'] = $module;
            $info['field types'][$db_name]['formatters'] = array();
          }
        }
      }

      // Populate widget types and formatters for known field types.
      foreach (module_list() as $module) {
        if ($module_widgets = module_invoke($module, 'widget_info')) {
          foreach ($module_widgets as $name => $widget_info) {
            // Truncate names to match the value that is stored in the database.
            $db_name = substr($name, 0, 32);
            $info['widget types'][$db_name] = $widget_info;
            $info['widget types'][$db_name]['module'] = $module;
            // Replace field types with db_compatible version of known field types.
            $info['widget types'][$db_name]['field types'] = array();
            foreach ($widget_info['field types'] as $field_type) {
              $field_type_db_name = substr($field_type, 0, 32);
              if (isset($info['field types'][$field_type_db_name])) {
                $info['widget types'][$db_name]['field types'][] = $field_type_db_name;
              }
            }
          }
        }

        if ($module_formatters = module_invoke($module, 'field_formatter_info')) {
          foreach ($module_formatters as $name => $formatter_info) {
            foreach ($formatter_info['field types'] as $field_type) {
              // Truncate names to match the value that is stored in the database.
              $db_name = substr($field_type, 0, 32);
              if (isset($info['field types'][$db_name])) {
                $info['field types'][$db_name]['formatters'][$name] = $formatter_info;
                $info['field types'][$db_name]['formatters'][$name]['module'] = $module;
              }
            }
          }
        }
      }

      // Populate actual field instances.
      module_load_include('inc', 'content', 'includes/content.crud');
      foreach (node_get_types('types', NULL, TRUE) as $type_name => $data) {
        $type = (array) $data;
        $type['url_str'] = str_replace('_', '-', $type['type']);
        $type['fields'] = array();
        $type['tables'] = array();
        if ($fields = content_field_instance_read(array('type_name' => $type_name))) {
          foreach ($fields as $field) {
            $db_info = content_database_info($field);
            $type['tables'][$db_info['table']] = $db_info['table'];

            // Allow external modules to translate field strings.
            $field_strings = array(
              'widget_label' => $field['widget']['label'],
              'widget_description' => $field['widget']['description'],
            );
            drupal_alter('content_field_strings', $field_strings, $field['type_name'], $field['field_name']);
            $field['widget']['label'] = $field_strings['widget_label'];
            $field['widget']['description'] = $field_strings['widget_description'];

            $type['fields'][$field['field_name']] = $field;
            // This means that content_fields($field_name) (no type name)
            // returns the last instance loaded.
            $info['fields'][$field['field_name']] = $field;
          }
          // Make sure the per-type table is added, even if no field is actually
          // stored in it.
          $table = _content_tablename($type['type'], CONTENT_DB_STORAGE_PER_CONTENT_TYPE);
          $type['tables'][$table] = $table;
        }

        // Gather information about non-CCK 'fields'.
        $extra = module_invoke_all('content_extra_fields', $type_name);
        drupal_alter('content_extra_fields', $extra, $type_name);
        // Add saved weights.
        foreach (variable_get('content_extra_weights_'. $type_name, array()) as $key => $value) {
          // Some stored entries might not exist anymore, for instance if uploads
          // have been disabled, or vocabularies removed...
          if (isset($extra[$key])) {
            $extra[$key]['weight'] = $value;
          }
        }
        $type['extra'] = $extra;

        $info['content types'][$type_name] = $type;
      }

      cache_set('content_type_info:'. $language->language, $info, content_cache_tablename());
    }
  }
  return $info;
}