<?php
/**
 * @file
 * living_shoreline_case_study.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function living_shoreline_case_study_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_additional_info_group|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_additional_info_group';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Additional Information',
    'weight' => '9',
    'children' => array(
      0 => 'field_ls_attachments',
      1 => 'field_ls_pictures',
      2 => 'field_website',
      3 => 'group_audience',
      4 => 'field_ls_other_comments',
      5 => 'group_access',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_additional_info_group|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_additional_info|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_additional_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Additional Information',
    'weight' => '15',
    'children' => array(
      0 => 'field_ls_attachments',
      1 => 'field_ls_pictures',
      2 => 'field_website',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'formatter' => 'no-start',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_additional_info|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_additional_info|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_additional_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional Information',
    'weight' => '8',
    'children' => array(
      0 => 'field_ls_pictures',
      1 => 'field_website',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_additional_info|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_administrative|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_administrative';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Administrative',
    'weight' => '9',
    'children' => array(
      0 => 'field_ls_attachments',
      1 => 'group_audience',
      2 => 'group_access',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_administrative|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_coastal_env_conditions|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_coastal_env_conditions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Coastal and Environmental Considerations',
    'weight' => '4',
    'children' => array(
      0 => 'field_ls_waterbody',
      1 => 'field_ls__expsd_fetch_dist',
      2 => 'field_ls_site_classification',
      3 => 'field_ls_fetch_orientation',
      4 => 'field_ls_tidal_range',
      5 => 'field_ls_nearby_struct_desc',
      6 => 'field_ls_salinity',
      7 => 'field_ls_boat_wake_exp',
      8 => 'field_ls_adj_to_nav_chan',
      9 => 'field_ls_no_wake_desig',
      10 => 'field_ls_100_yr_flood_elev',
      11 => 'field_ls_100_yr_flood_elev_datum',
      12 => 'field_ls_other_site_classificat',
      13 => 'field_ls_other_vertical_da',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_coastal_env_conditions|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_construction|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_construction';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_general';
  $field_group->data = array(
    'label' => 'Construction',
    'weight' => '1',
    'children' => array(
      0 => 'field_ls_constr_dates_est',
      1 => 'field_ls_construction_dates',
      2 => 'field_ls_const_dates_actual',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_construction|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_design_considerations|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_design_considerations';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Design Elements',
    'weight' => '10',
    'children' => array(
      0 => 'group_ls_design',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'label' => 'Design Elements',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'no-start',
    ),
  );
  $export['group_ls_design_considerations|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_design_considerations|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_design_considerations';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Design Considerations',
    'weight' => '4',
    'children' => array(
      0 => 'field_ls_length',
      1 => 'field_ls_fill',
      2 => 'field_ls_fill_type',
      3 => 'field_ls_other_fill_type_desc',
      4 => 'field_ls_fill_amount',
      5 => 'field_ls_nearshore_slope_design',
      6 => 'field_ls_area_shlw_bottom_conv',
      7 => 'field_ls_area_uplnd_conv',
      8 => 'field_ls_project_width',
      9 => 'field_ls_width_above_mhw',
      10 => 'field_ls_width_below_mhw',
      11 => 'field_ls_design_wave_height',
      12 => 'field_ls_design_criteria',
      13 => 'field_ls_source_of_fill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_design_considerations|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_design_group|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_design_group';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Design Considerations',
    'weight' => '5',
    'children' => array(
      0 => 'field_ls_length',
      1 => 'field_ls_fill',
      2 => 'field_ls_fill_type',
      3 => 'field_ls_other_fill_type_desc',
      4 => 'field_ls_fill_amount',
      5 => 'field_ls_nearshore_slope_design',
      6 => 'field_ls_area_shlw_bottom_conv',
      7 => 'field_ls_area_uplnd_conv',
      8 => 'field_ls_project_width',
      9 => 'field_ls_width_above_mhw',
      10 => 'field_ls_width_below_mhw',
      11 => 'field_ls_design_wave_height',
      12 => 'field_ls_design_criteria',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_design_group|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_design|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_design';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_design_considerations';
  $field_group->data = array(
    'label' => 'Design',
    'weight' => '122',
    'children' => array(
      0 => 'field_ls_length',
      1 => 'field_ls_fill',
      2 => 'field_ls_fill_type',
      3 => 'field_ls_other_fill_type_desc',
      4 => 'field_ls_fill_amount',
      5 => 'field_ls_nearshore_slope_design',
      6 => 'field_ls_area_shlw_bottom_conv',
      7 => 'field_ls_area_uplnd_conv',
      8 => 'field_ls_project_width',
      9 => 'field_ls_width_above_mhw',
      10 => 'field_ls_width_below_mhw',
      11 => 'field_ls_design_wave_height',
      12 => 'field_ls_design_criteria',
      13 => 'field_ls_source_of_fill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_design|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_display_group|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_display_group';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Living Shorelines Case Study',
    'weight' => '0',
    'children' => array(
      0 => 'field_ls_location',
      1 => 'group_ls_dsply_pre_project_cond',
      2 => 'group_ls_coastal_env_conditions',
      3 => 'group_ls_design_group',
      4 => 'group_ls_structures_group',
      5 => 'group_ls_non_struct_group',
      6 => 'group_ls_project_perform_group',
      7 => 'group_ls_additional_info_group',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_display_group|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_dsply_pre_project_cond|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_dsply_pre_project_cond';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Pre-Project Conditions',
    'weight' => '3',
    'children' => array(
      0 => 'field_ls_pre_submrgd_veg',
      1 => 'field_ls_bank_elevation',
      2 => 'field_ls_pre_nearshore_slope',
      3 => 'field_ls_pre_structures_debris',
      4 => 'field_ls_pre_struc_debr_descr',
      5 => 'field_ls_pre_constr_erosion',
      6 => 'field_ls_pre_constr_adj_erosion',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_dsply_pre_project_cond|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_env_conditions|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_env_conditions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Coastal and Environmental Conditions',
    'weight' => '9',
    'children' => array(
      0 => 'group_ls_environmental_cond',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'label' => 'Coastal and Environmental Conditions',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'no-start',
    ),
  );
  $export['group_ls_env_conditions|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_env_conditions|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_env_conditions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Coastal and Environmental Considerations',
    'weight' => '3',
    'children' => array(
      0 => 'field_ls_waterbody',
      1 => 'field_ls_site_classification',
      2 => 'field_ls_fetch_orientation',
      3 => 'field_ls_tidal_range',
      4 => 'field_ls_nearby_struct_desc',
      5 => 'field_ls_salinity',
      6 => 'field_ls_boat_wake_exp',
      7 => 'field_ls_adj_to_nav_chan',
      8 => 'field_ls_no_wake_desig',
      9 => 'field_ls_100_yr_flood_elev',
      10 => 'field_ls_100_yr_flood_elev_datum',
      11 => 'field_ls_other_site_classificat',
      12 => 'field_ls_other_vertical_da',
      13 => 'field_ls_expsd_fetch_dist',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_env_conditions|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_environmental_cond|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_environmental_cond';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_env_conditions';
  $field_group->data = array(
    'label' => 'Environmental Conditions',
    'weight' => '105',
    'children' => array(
      0 => 'field_ls_waterbody',
      1 => 'field_ls_site_classification',
      2 => 'field_ls_fetch_orientation',
      3 => 'field_ls_tidal_range',
      4 => 'field_ls_nearby_struct_desc',
      5 => 'field_ls_salinity',
      6 => 'field_ls_boat_wake_exp',
      7 => 'field_ls_adj_to_nav_chan',
      8 => 'field_ls_no_wake_desig',
      9 => 'field_ls_100_yr_flood_elev',
      10 => 'field_ls_100_yr_flood_elev_datum',
      11 => 'field_ls_other_site_classificat',
      12 => 'field_ls_other_vertical_da',
      13 => 'field_ls_expsd_fetch_dist',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_environmental_cond|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_general|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_general';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'General',
    'weight' => '7',
    'children' => array(
      0 => 'field_ls_location',
      1 => 'title',
      2 => 'group_ls_construction',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'formatter' => 'no-start',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_general|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_general|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_general';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'General',
    'weight' => '1',
    'children' => array(
      0 => 'field_ls_location',
      1 => 'field_ls_constr_dates_est',
      2 => 'field_ls_const_dates_actual',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_general|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_non_struct_group|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_non_struct_group';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Non-Structural Elements',
    'weight' => '7',
    'children' => array(
      0 => 'field_ls_percentage_of_vegetativ',
      1 => 'field_ls_was_erosion_fabric_used',
      2 => 'field_ls_root_mat_developed_by_v',
      3 => 'field_ls_cross_section_width_od_',
      4 => 'field_ls_additional_ecological_b',
      5 => 'field_ls_type_of_substrate_below',
      6 => 'field_ls_prim_wetland_species_be',
      7 => 'field_ls_prim_wetland_species_ab',
      8 => 'field_ls_type_of_substrate_above',
      9 => 'field_ls_ot_wetlands_species_bel',
      10 => 'field_ls_ot_wetlands_species_abo',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_non_struct_group|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_non_structural_elements|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_non_structural_elements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Non-Structural Elements',
    'weight' => '13',
    'children' => array(
      0 => 'group_ls_non_structural',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'formatter' => 'no-start',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_non_structural_elements|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_non_structural_elements|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_non_structural_elements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Non-Structural Elements',
    'weight' => '6',
    'children' => array(
      0 => 'field_ls_percentage_of_vegetativ',
      1 => 'field_ls_was_erosion_fabric_used',
      2 => 'field_ls_root_mat_developed_by_v',
      3 => 'field_ls_cross_section_width_od_',
      4 => 'field_ls_additional_ecological_b',
      5 => 'field_ls_type_of_substrate_below',
      6 => 'field_ls_prim_wetland_species_be',
      7 => 'field_ls_prim_wetland_species_ab',
      8 => 'field_ls_type_of_substrate_above',
      9 => 'field_ls_ot_wetlands_species_bel',
      10 => 'field_ls_ot_wetlands_species_abo',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_non_structural_elements|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_non_structural|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_non_structural';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_non_structural_elements';
  $field_group->data = array(
    'label' => 'Non-Structural',
    'weight' => '163',
    'children' => array(
      0 => 'field_ls_percentage_of_vegetativ',
      1 => 'field_ls_was_erosion_fabric_used',
      2 => 'field_ls_cross_section_width_od_',
      3 => 'field_ls_additional_ecological_b',
      4 => 'field_ls_type_of_substrate_below',
      5 => 'field_ls_prim_wetland_species_be',
      6 => 'field_ls_prim_wetland_species_ab',
      7 => 'field_ls_type_of_substrate_above',
      8 => 'field_ls_ot_wetlands_species_bel',
      9 => 'field_ls_ot_wetlands_species_abo',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_non_structural|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_pre_conditions|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_pre_conditions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Pre-Project Conditions',
    'weight' => '8',
    'children' => array(
      0 => 'group_ls_preproj',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'label' => 'Pre-Project Conditions',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'no-start',
    ),
  );
  $export['group_ls_pre_conditions|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_pre_conditions|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_pre_conditions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Pre-Project Conditions',
    'weight' => '2',
    'children' => array(
      0 => 'field_ls_pre_submrgd_veg',
      1 => 'field_ls_bank_elevation',
      2 => 'field_ls_pre_nearshore_slope',
      3 => 'field_ls_pre_structures_debris',
      4 => 'field_ls_pre_struc_debr_descr',
      5 => 'field_ls_pre_constr_erosion',
      6 => 'field_ls_pre_constr_adj_erosion',
      7 => 'field_ls_maj_plant_species_pres',
      8 => 'field_ls_ot_major_plant_species_',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_pre_conditions|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_preproj|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_preproj';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_pre_conditions';
  $field_group->data = array(
    'label' => 'Pre-Project',
    'weight' => '93',
    'children' => array(
      0 => 'field_ls_pre_submrgd_veg',
      1 => 'field_ls_bank_elevation',
      2 => 'field_ls_pre_nearshore_slope',
      3 => 'field_ls_pre_structures_debris',
      4 => 'field_ls_pre_struc_debr_descr',
      5 => 'field_ls_pre_constr_erosion',
      6 => 'field_ls_pre_constr_adj_erosion',
      7 => 'field_ls_maj_plant_species_pres',
      8 => 'field_ls_ot_major_plant_species_',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_preproj|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_project_perform_group|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_project_perform_group';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Project Performance',
    'weight' => '8',
    'children' => array(
      0 => 'field_ls_erosion_performance',
      1 => 'field_ls_habitat_performance',
      2 => 'field_ls_unforeseen_benefits',
      3 => 'field_ls_storm_exposure',
      4 => 'field_ls_storm_date',
      5 => 'field_ls_storm_intensity',
      6 => 'field_ls_descr_proj_storm_perfor',
      7 => 'field_ls_des_overall_project_p',
      8 => 'field_ls_overall_perfom_of_the',
      9 => 'field_ls_des_overall_project_p_s',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_project_perform_group|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_project_performance_2|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_project_performance_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_project_performance';
  $field_group->data = array(
    'label' => 'Project Performance',
    'weight' => '72',
    'children' => array(
      0 => 'field_ls_erosion_performance',
      1 => 'field_ls_habitat_performance',
      2 => 'field_ls_root_mat_developed_by_v',
      3 => 'field_ls_des_overall_project_p',
      4 => 'field_ls_overall_perfom_of_the',
      5 => 'field_ls_des_overall_project_p_s',
      6 => 'field_ls_desc_overall_performan',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_project_performance_2|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_project_performance|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_project_performance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Project Performance',
    'weight' => '14',
    'children' => array(
      0 => 'field_ls_unforeseen_benefits',
      1 => 'field_ls_other_comments',
      2 => 'group_storms',
      3 => 'group_ls_project_performance_2',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'formatter' => 'no-start',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_project_performance|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_project_performance|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_project_performance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Project Performance',
    'weight' => '7',
    'children' => array(
      0 => 'field_ls_erosion_performance',
      1 => 'field_ls_habitat_performance',
      2 => 'field_ls_unforeseen_benefits',
      3 => 'field_ls_other_comments',
      4 => 'field_ls_storm_exposure',
      5 => 'field_ls_storm_date',
      6 => 'field_ls_storm_intensity',
      7 => 'field_ls_descr_proj_storm_perfor',
      8 => 'field_ls_des_overall_project_p',
      9 => 'field_ls_overall_perfom_of_the',
      10 => 'field_ls_des_overall_project_p_s',
      11 => 'field_ls_desc_overall_performan',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_project_performance|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_structural_elements|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_structural_elements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls';
  $field_group->data = array(
    'label' => 'Structural Elements',
    'weight' => '11',
    'children' => array(
      0 => 'group_ls_structural',
    ),
    'format_type' => 'multipage',
    'format_settings' => array(
      'label' => 'Structural Elements',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'no-start',
    ),
  );
  $export['group_ls_structural_elements|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_structural_elements|node|living_shoreline_case_study|full';
  $field_group->group_name = 'group_ls_structural_elements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Structural Elements',
    'weight' => '5',
    'children' => array(
      0 => 'field_ls_structure_yes_no',
      1 => 'field_ls_area_shlw_btm_2_struc',
      2 => 'field_ls_width_open_water',
      3 => 'field_ls_type_of_structure',
      4 => 'field_ls_struc_materials_desc',
      5 => 'field_ls_dist_of_struct_mhw',
      6 => 'field_ls_dist_of_struct_mlw',
      7 => 'field_ls_struct_base_width',
      8 => 'field_ls_length_of_struct',
      9 => 'field_ls_struct_crest_width',
      10 => 'field_ls_struct_side_slope',
      11 => 'field_ls_structure_monolithic_or',
      12 => 'field_ls_height_of_struct_above',
      13 => 'field_ls_max_depth_at_seaward_e',
      14 => 'field_ls_max_depth_at_landward_',
      15 => 'field_ls_drop_downs_or_openings_',
      16 => 'field_ls_width_of_drop_downs_or',
      17 => 'field_ls_spacing_between_gaps_',
      18 => 'field_ls_type_of_foundation',
      19 => 'field_ls_geotextile_used_in_fou',
      20 => 'field_ls_porosity_of_structure',
      21 => 'field_ls_porosity_of_struct_perc',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_structural_elements|node|living_shoreline_case_study|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_structural|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls_structural';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_structural_elements';
  $field_group->data = array(
    'label' => 'Structural',
    'weight' => '138',
    'children' => array(
      0 => 'field_ls_structure_yes_no',
      1 => 'field_ls_area_shlw_btm_2_struc',
      2 => 'field_ls_width_open_water',
      3 => 'field_ls_type_of_structure',
      4 => 'field_ls_struc_materials_desc',
      5 => 'field_ls_dist_of_struct_mhw',
      6 => 'field_ls_dist_of_struct_mlw',
      7 => 'field_ls_struct_base_width',
      8 => 'field_ls_length_of_struct',
      9 => 'field_ls_struct_crest_width',
      10 => 'field_ls_struct_side_slope',
      11 => 'field_ls_structure_monolithic_or',
      12 => 'field_ls_height_of_struct_above',
      13 => 'field_ls_max_depth_at_seaward_e',
      14 => 'field_ls_max_depth_at_landward_',
      15 => 'field_ls_drop_downs_or_openings_',
      16 => 'field_ls_width_of_drop_downs_or',
      17 => 'field_ls_spacing_between_gaps_',
      18 => 'field_ls_type_of_foundation',
      19 => 'field_ls_geotextile_used_in_fou',
      20 => 'field_ls_porosity_of_structure',
      21 => 'field_ls_porosity_of_struct_perc',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_ls_structural|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls_structures_group|node|living_shoreline_case_study|default';
  $field_group->group_name = 'group_ls_structures_group';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_ls_display_group';
  $field_group->data = array(
    'label' => 'Structural Elements',
    'weight' => '6',
    'children' => array(
      0 => 'field_ls_structure_yes_no',
      1 => 'field_ls_area_shlw_btm_2_struc',
      2 => 'field_ls_width_open_water',
      3 => 'field_ls_type_of_structure',
      4 => 'field_ls_struc_materials_desc',
      5 => 'field_ls_dist_of_struct_mhw',
      6 => 'field_ls_dist_of_struct_mlw',
      7 => 'field_ls_struct_base_width',
      8 => 'field_ls_length_of_struct',
      9 => 'field_ls_struct_crest_width',
      10 => 'field_ls_struct_side_slope',
      11 => 'field_ls_structure_monolithic_or',
      12 => 'field_ls_height_of_struct_above',
      13 => 'field_ls_max_depth_at_seaward_e',
      14 => 'field_ls_max_depth_at_landward_',
      15 => 'field_ls_drop_downs_or_openings_',
      16 => 'field_ls_width_of_drop_downs_or',
      17 => 'field_ls_spacing_between_gaps_',
      18 => 'field_ls_type_of_foundation',
      19 => 'field_ls_geotextile_used_in_fou',
      20 => 'field_ls_porosity_of_structure',
      21 => 'field_ls_porosity_of_struct_perc',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_ls_structures_group|node|living_shoreline_case_study|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ls|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_ls';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Living Shorelines Case Study',
    'weight' => '0',
    'children' => array(
      0 => 'group_ls_pre_conditions',
      1 => 'group_ls_env_conditions',
      2 => 'group_ls_design_considerations',
      3 => 'group_ls_structural_elements',
      4 => 'group_ls_general',
      5 => 'group_ls_project_performance',
      6 => 'group_ls_additional_info',
      7 => 'group_ls_non_structural_elements',
    ),
    'format_type' => 'multipage-group',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
        'page_header' => 3,
        'move_additional' => 1,
        'page_counter' => 1,
        'move_button' => 0,
      ),
    ),
  );
  $export['group_ls|node|living_shoreline_case_study|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_storms|node|living_shoreline_case_study|form';
  $field_group->group_name = 'group_storms';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'living_shoreline_case_study';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_ls_project_performance';
  $field_group->data = array(
    'label' => 'Storm Performance',
    'weight' => '79',
    'children' => array(
      0 => 'field_ls_storm_exposure',
      1 => 'field_ls_storm_date',
      2 => 'field_ls_storm_intensity',
      3 => 'field_ls_descr_proj_storm_perfor',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_storms|node|living_shoreline_case_study|form'] = $field_group;

  return $export;
}
