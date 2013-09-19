<?php
/**
 * @file
 * Views Foundation Navigation plugin.
 *
 * @see http://foundation.zurb.com/old-docs/f3/navigation.php
 */

/**
 * Style plugin to render rows in a Foundation Navigation.
 *
 * @ingroup views_style_plugins
 */
class views_foundation_plugin_style_navigation extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['nav_field'] = array('default' => '');
    $options['nav_vertical'] = array('default' => 0);
    $options['nav_large'] = array('default' => 0);
    $options['nav_count'] = array('default' => 3);
    $options['nav_link'] = array('default' => 0);

    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Pre-build option list for the navigation field.
    $fields = array('' => t('- None -'));
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      $fields[$field] = $handler->ui_name();
    }

    $form['nav_field'] = array(
      '#type' => 'select',
      '#title' => t('Navigation field'),
      '#options' => $fields,
      '#default_value' => $this->options['nav_field'],
      '#description' => t('Select the field that will be used as the top links.'),
    );

    $form['nav_vertical'] = array(
      '#type' => 'checkbox',
      '#title' => t('Vertical navigation'),
      '#description' => t('Select this checkbox if you want vertical navigation.'),
      '#default_value' => $this->options['nav_vertical'],
    );

    $form['nav_large'] = array(
      '#type' => 'checkbox',
      '#title' => t('Large flyouts'),
      '#description' => t('Select this checkbox if you want large flyouts.'),
      '#default_value' => $this->options['nav_large'],
    );

    $form['nav_count'] = array(
      '#type' => 'select',
      '#title' => t('Flyout "right" class'),
      '#options' => drupal_map_assoc(range(0, 10)),
      '#default_value' => $this->options['nav_count'],
      '#description' => t('Select after how many links add the class "right" to the flyout.'),
    );

    $form['nav_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Submenu links'),
      '#default_value' => $this->options['nav_link'],
      '#description' => t('Select this checkbox if you want to display all fields as links in the submenu'),
    );

  }

}
