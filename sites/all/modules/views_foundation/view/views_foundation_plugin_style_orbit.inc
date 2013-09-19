<?php
/**
 * @file
 * Views Foundation Orbit plugin.
 *
 * @see http://foundation.zurb.com/old-docs/f3/orbit.php
 */

/**
 * Style plugin to render rows in a Foundation Orbit slider.
 *
 * @ingroup views_style_plugins
 */
class views_foundation_plugin_style_orbit extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['animation'] = array('default' => 'horizontal-slide');
    $options['animation_speed'] = array('default' => 800);
    $options['advance_speed']   = array('default' => 4000);
    $options['directional_nav'] = array('default' => TRUE);
    $options['timer'] = array('default' => TRUE);
    $options['fluid'] = array('default' => 'true');
    $options['bullets'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Pre-build option list for the caption field.
    $fields = array('' => t('- None -'));
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      $fields[$field] = $handler->ui_name();
    }

    $form['animation'] = array(
      '#type' => 'select',
      '#title' => t('Animation type'),
      '#description' => t('Specify the Animation Style'),
      '#required' => TRUE,
      '#options' => array(
        'horizontal-slide' => t('horizontal-slide'),
        'vertical-slide' => t('vertical-slide'),
        'horizontal-push' => t('horizontal-push'),
        'fade' => t('fade'),
      ),
      '#default_value' => $this->options['animation'],
    );

    $form['animation_speed'] = array(
      '#type' => 'textfield',
      '#title' => t('Animation speed'),
      '#required' => TRUE,
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('Specify the Animation Speed'),
      '#default_value' => $this->options['animation_speed'],
      '#element_validate' => array('element_validate_integer_positive'),
    );

    $form['advance_speed'] = array(
      '#type' => 'textfield',
      '#title' => t('Advanced speed'),
      '#description' => t('If timer is enables, time between transitions'),
      '#required' => TRUE,
      '#size' => 10,
      '#maxlength' => 10,
      '#default_value' => $this->options['advance_speed'],
      '#element_validate' => array('element_validate_integer_positive'),
    );

    $form['directional_nav'] = array(
      '#type' => 'checkbox',
      '#title' => t('Directional Nav'),
      '#description' => t('Do you want directional nav?'),
      '#default_value' => $this->options['directional_nav'],
    );

    $form['timer'] = array(
      '#type' => 'checkbox',
      '#title' => t('Timer'),
      '#description' => t('Do you want a timer or not?'),
      '#default_value' => $this->options['timer'],
    );

    $form['caption_field'] = array(
      '#type' => 'select',
      '#title' => t('Caption field'),
      '#options' => $fields,
      '#default_value' => $this->options['caption_field'],
      '#description' => t('Select the field that will be used as the image caption.'),
    );

    $form['fluid'] = array(
      '#type' => 'textfield',
      '#title' => t('Fluid'),
      '#required' => TRUE,
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t("Control responsiveness of Orbit slides. Type true, or Aspect Ratio (eg. 16x6)"),
      '#default_value' => $this->options['fluid'],
    );
    $form['bullets'] = array(
      '#type' => 'checkbox',
      '#title' => t('Bullets'),
      '#description' => t('Do you want bullet navigation?'),
      '#default_value' => $this->options['bullets'],
    );
  }

  /**
   * Validating the inputs.
   */
  function options_validate($form, &$form_state) {

    parent::options_validate($form, $form_state);
    $options = &$form_state['values']['style_options'];

    if ($options['fluid'] != 'true') {
      if (!preg_match('/^[0-9]*[x][0-9]$/', $options['fluid'])) {
        form_error($form['fluid'], t('Please specify a correct aspect ratio.'));
      }
    }
  }

}
