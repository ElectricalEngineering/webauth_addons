<?php

/**
 * @file includes/webauth_addons.inc
 * Helper functions provided by the Webauth Add-Ons module.
 */

/**
 * Check that we are logged in via webauth
 * @return bool
 */
if(!function_exists('sunet_role')):
  function sunet_role() {
    global $user;
    return (bool)array_key_exists(variable_get('webauth_addons_role', 'webauth_default_role'), $user->roles);
  }
endif;

/**
 * Implements hook_preprocess_page()
 */
function webauth_addons_preprocess_page(&$vars, $hook) {

  $vars['clean_url'] = drupal_get_path_alias( implode( '/', arg() ) );

  if( arg(0) == 'user' && (variable_get('user_register') && variable_get('user_register') == '9505') && !sunet_role() ):
    unset($vars['tabs']);
    drupal_goto( url( variable_get('webauth_path', '/sites/default/webauth/login') ) );
  endif;

  if(arg(0) == 'user' && arg(1) == 'register' && (variable_get('user_register') && variable_get('user_register') == '9505')):
    drupal_set_title(t('Local registration is closed'));
    $vars['page']['content'] = t('<p>Local registration is closed.  Please login or create an account by clicking the "Log in" tab above.</p>');
  endif;

  if(arg(0) == 'user' && arg(1) == 'password' && (variable_get('user_register') && variable_get('user_register') == '9505')):
    drupal_set_title(t('Reset your SUNet ID Password'));
    $vars['page']['content'] = t('<p>To request a new SUNet ID password, please follow this <a href="@link" rel="nofollow">link</a>.</p>', array('@link' => url('https://accounts.stanford.edu/resetpw?execution=e1s1', array('external' => true) ) ) );
    if( !sunet_role() ):
      $vars['page']['content'] .= t('<p>To login, click the "Log in" tab above.</p>');
    endif;
  endif;

}

/**
 * Implements hook_preprocess_search_results()
 */
function webauth_addons_preprocess_search_results(&$vars) {

  $vars['search_results'] = '';
  $intranet = variable_get('intranet_base', 'intranet');

  foreach($vars['results'] as $key => $result):

    if(isset($vars['results'][$key]['user'])):
      unset($vars['results'][$key]['user']);
    endif;

    if( in_array( $intranet, explode('/', $vars['results'][$key]['link']), TRUE ) && !sunet_role() ):
      unset($vars['results'][$key]);
    else:
      $vars['results'][$key] = $result;
      $vars['search_results'] .= theme('search_result', array('result' => $result, 'module' => $vars['module']));
    endif;

  endforeach;

}
