<?php
/**
 * @file includes/webauth_addons.form.inc
 * Form hooks provided by the Webauth Add-Ons module.
 */

/**
 * Function to turn off login/register form elements
 */
if(!function_exists('_turn_off_register_reset')):
  function _turn_off_register_reset(&$form, &$form_state){

    if( variable_get('user_register') && variable_get('user_register') == '9505' ):
      $form['name']['#access'] = FALSE;
      $form['actions']['#access'] = FALSE;
    endif;
  }
endif;
/**
 * Implements hook_form_FORM_ID_alter()
 * Add Stanford only option and set it to
 * EE's mailcode in case another module
 * alters this form
 */
function webauth_addons_form_user_admin_settings_alter(&$form, &$form_state) {

    $form['registration_cancellation']['user_register']['#options'][9505] = t('Stanford Only');
    $form['#rebuild'] = TRUE;

}

/**
 * Implements hook_form_FORM_ID_alter()
 * Deny access to register form
 */
function webauth_addons_form_user_register_form_alter(&$form, &$form_state) {

  _turn_off_register_reset($form, $form_state);

}

/**
 * Implements hook_form_FORM_ID_alter()
 * Deny access to user password reset form
 */
function webauth_addons_form_user_pass_alter(&$form, &$form_state) {

  _turn_off_register_reset($form, $form_state);

}

/**
 * Implements hook_form_FORM_ID_alter()
 * Deny access to local login form
 */
function webauth_addons_form_user_login_alter(&$form, &$form_state) {

  if( variable_get('user_register') && variable_get('user_register') == '3' && !sunet_role() ):
   _turn_off_register_reset($form, $form_state);
  endif;

}
