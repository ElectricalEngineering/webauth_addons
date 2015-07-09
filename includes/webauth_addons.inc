<?php

/**
 * @file includes/webauth_addons.inc
 * Helper functions provided by the Webauth Add-Ons module.
 */

define('WAA_DEBUG', 0);

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

function waa_r($obj, $name = NULL, $type = 'debug') {
  if($name == NULL):
     $name = t('<br />File : @file<br />Function : @func<br />Line : @line<br />', array('@file' => __FILE__, '@func' => __FUNCTION__, '@line' => __LINE__) );
  endif;

  $message =
  '<pre>'
    . $name . ' => '
    . print_r($obj, true)
    . '</pre>';

  drupal_set_message($message, $type);
}

function waa_default_string() {

  $debug = debug_backtrace();
  $debug = array(
    'file' => $debug[0]['file'],
    'line' => $debug[0]['line'],
    'func' => $debug[1]['function']
  );

  return t( 'WAA_DEBUG : @function, line @line<br />@file<br />', array('@line' => $debug['line'], '@file' => $debug['file'], '@function' => $debug['func']) );
}

if(!function_exists('waa_debug')):
  function waa_debug($obj, $name = 'WAA_DEBUG : ', $type = 'debug') {

    global $user;

    $function = function_exists('dpm') ? dpm : waa_r;

    if( (defined('WAA_DEBUG') &&  WAA_DEBUG) && ( user_access('access devel information', $user) || user_access('view webauth addons debug', $user) ) ):

     $function($obj, $name, $type);

    endif;
  }

endif;

/**
 * Implements hook_preprocess_page()
 */
function webauth_addons_preprocess_page(&$vars, $hook) {

  $vars['clean_url'] = drupal_get_path_alias( implode( '/', arg() ) );

  if( arg(0) == 'user' && (variable_get('user_register') && variable_get('user_register') == '9505') && !sunet_role() ):
    unset($vars['tabs']);
#    drupal_goto( url( variable_get('webauth_path', '/sites/default/webauth/login') ) );
     drupal_goto( url('sites/default/webauth/login') );
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

//debug

waa_debug($vars, waa_default_string() );

}
// To-Do : use Drupal cache instead
// of drupal variable for intranet nodes.
function _webauth_addons_cache( $gen = false )  {

  $nids = &drupal_static(__FUNCTION__);

  if(!$nids):

    $cache_id = 'webauth_addons:intranet_cache';
    $cache = cache_get($cache_id, 'cache');

    if($cache && !empty($cache->data)):
      $nids = $cache->data;
    else:
      $nids = _webauth_addons_get_intranet_nids(true);
      cache_set($cache_id, $nids, 'cache', ( REQUEST_TIME + 60 * 60 ) ); //1 hour
    endif; // $cache !empty($cache->data)

  endif; // !$nids

  return $nids;
}

function _webauth_addons_get_intranet_nids( $gen = false ) {

  $vars = variable_get('webauth_addons_intranet_nids');

  if(!$vars || $gen ):

    if($gen):
      $message = 'Forced update of intranet pages cache from @file on line @line in @function function.';
      watchdog(
        'webauth_addons',
        check_plain($message),
        array('@line' => __LINE__, '@file' => __FILE__, '@function' => __FUNCTION__),
        WATCHDOG_NOTICE
      );
    endif;

    $vars['ts'] = REQUEST_TIME;
    $vars['updated'] = date();

    $aliases = db_select('url_alias', 'a')
      ->fields('a', array('source', 'alias'))
      ->condition('a.alias', '%' . db_like( variable_get('intranet_base','intranet') ) . '%', 'LIKE' )
      ->addTag('node_access')
      ->execute()
      ->fetchAll();

    foreach($aliases as $alias):
      $nids[] = end( explode('/', $alias->source) );

    endforeach;


    $aliases = NULL;
    $vars['nids'] = $nids;

    variable_set('webauth_addons_intranet_nids', $vars);

  endif;

  return $vars;

}
function webauth_addons_cron() {

  $frequency = variable_get('webauth_addons_intranet_frequency', 1);
  $interval = 60 * 60 * 24 * $frequency;

  if ((REQUEST_TIME - variable_get('webauth_addons_last_cron', 0)) > $interval):

    _webauth_addons_get_intranet_nids(true);

    $message = 'Updated intranet pages cache from @file on line @line in @function function.';
    watchdog(
        'webauth_addons',
        check_plain($message),
        array('@line' => __LINE__, '@file' => __FILE__, '@function' => __FUNCTION__),
        WATCHDOG_INFO
      );
    variable_set('webauth_addons_last_cron', REQUEST_TIME);
  endif;


}
/**
 * Implements hook_query_TAG_alter()
 */
function webauth_addons_query_search_node_alter(&$query) {

  if( (defined('WAA_DEBUG') &&  WAA_DEBUG) && ( user_access('access devel information', $user) || user_access('view webauth addons debug', $user) ) ):
    $intranet_nodes = _webauth_addons_get_intranet_nids();
  endif;

  if( !sunet_role() ):
    $intranet_nodes = _webauth_addons_get_intranet_nids();
    $nids = $intranet_nodes['nids'];

    foreach($nids as $nid):
      $query->condition( db_and()->condition('n.nid', $nid, '!=') );
    endforeach;

  endif;

  //debug
  $debug = array(
    'nids' => $intranet_nodes,
    'qts' => $query->__toString()
  );
  waa_debug( $debug, waa_default_string() );
}

/**
 * Implements hook_preprocess_search_result()
 */
function webauth_addons_preprocess_search_result(&$vars) {

  // Get the elapsed time since the last update of the node
  $elapsed = format_interval((time() - $vars['result']['node']->changed) , 3);

  // Change the style of the search match to italics and cardinal red
  $vars['snippet'] = preg_replace('/<strong>/', '<strong style="font-style:oblique; color:#990009;">', $vars['snippet']);

  // If we have a time, add it to the snippet
  if($elapsed):
    $vars['snippet'] .= t(
      '<br /><small style="font-style:oblique; font-weight:lighter;"> Updated : %s ago</small>',
      array('%s' => $elapsed)
    );
  endif;

}
