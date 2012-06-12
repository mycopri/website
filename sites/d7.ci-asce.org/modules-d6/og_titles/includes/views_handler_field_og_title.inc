<?php
class views_handler_field_og_title extends views_handler_field
{
  function options_form(&$form, &$form_state)
  {
      parent::options_form($form, $form_state);
      $form['og_titles_manage_link'] = array(
         '#type' => 'radios',
         '#title' => t('Manage Own Title Link'),
         '#description' => t('Display a link for the user to manage their own title. This is only useful for overriding views such as og_my. This option should only be enabled for user operations.'),
         '#default_value' => isset($this->options['og_titles_manage_link']) ? $this->options['og_titles_manage_link'] : 0,
         '#options' => array(0 => t('No'), 1 => t('Yes')),
      );
  }

  function render($values)
  {
    global $user;

    //FIXME: will $values->users_uid break? It seems to be handling gracefully so far..
    if ($user->uid == $values->users_uid && $this->options['og_titles_manage_link'] && user_access('manage own og title'))
        return l($values->{$this->field_alias}, "og/manage/$values->nid/title", array('query' => drupal_get_destination()));
    return $values->{$this->field_alias};

  }
}
