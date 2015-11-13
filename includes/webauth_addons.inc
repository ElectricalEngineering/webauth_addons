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
    $user = \Drupal::currentUser();
    return (bool)array_key_exists(\Drupal::config('webauth_addons.settings')->get('webauth_addons_role'), $user->roles);
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

    $user = \Drupal::currentUser();

    $function = function_exists('dpm') ? dpm : waa_r;

    if( (defined('WAA_DEBUG') &&  WAA_DEBUG) && ( $user->hasPermission('access devel information') || $user->hasPermission('view webauth addons debug') ) ):

     $function($obj, $name, $type);

    endif;
  }

endif;

/**
 * Implements hook_preprocess_page()
 */
function webauth_addons_preprocess_page(&$vars, $hook) {

  $vars['clean_url'] = drupal_get_path_alias( implode( '/', arg() ) );

  // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// if( arg(0) == 'user' && (variable_get('user_register') && variable_get('user_register') == '9505') && !sunet_role() ):
//     unset($vars['tabs']);
// #    drupal_goto( url( variable_get('webauth_path', '/sites/default/webauth/login') ) );
//      // @FIXME
// // url() expects a route name or an external URI.
// // drupal_goto( url('sites/default/webauth/login') );
// 
//   endif;


  // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// if(arg(0) == 'user' && arg(1) == 'register' && (variable_get('user_register') && variable_get('user_register') == '9505')):
//     // @FIXME
// // drupal_set_title() has been removed. There are now a few ways to set the title
// // dynamically, depending on the situation.
// //
// //
// // @see https://www.drupal.org/node/2067859
// // drupal_set_title(t('Local registration is closed'));
// 
//     $vars['page']['content'] = t('<p>Local registration is closed.  Please login or create an account by clicking the "Log in" tab above.</p>');
//   endif;


  // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// if(arg(0) == 'user' && arg(1) == 'password' && (variable_get('user_register') && variable_get('user_register') == '9505')):
//     // @FIXME
// // drupal_set_title() has been removed. There are now a few ways to set the title
// // dynamically, depending on the situation.
// //
// //
// // @see https://www.drupal.org/node/2067859
// // drupal_set_title(t('Reset your SUNet ID Password'));
// 
//     $vars['page']['content'] = t('<p>To request a new SUNet ID password, please follow this <a href="@link" rel="nofollow">link</a>.</p>', array('@link' => \Drupal\Core\Url::fromUri('https://accounts.stanford.edu/resetpw?execution=e1s1') ) );
//     if( !sunet_role() ):
//       $vars['page']['content'] .= t('<p>To login, click the "Log in" tab above.</p>');
//     endif;
//   endif;


//debug

waa_debug($vars, waa_default_string() );

}
// To-Do : use Drupal cache instead
// of drupal variable for intranet nodes.
function _webauth_addons_cache( $gen = false )  {

  $nids = &drupal_static(__FUNCTION__);

  if(!$nids):

    $cache_id = 'webauth_addons:intranet_cache';
    $cache = \Drupal::cache('cache')->get($cache_id);

    if($cache && !empty($cache->data)):
      $nids = $cache->data;
    else:
      $nids = _webauth_addons_get_intranet_nids(true);
      \Drupal::cache('cache')->set($cache_id, $nids, ( REQUEST_TIME + 60 * 60 )); //1 hour
    endif; // $cache !empty($cache->data)

  endif; // !$nids

  return $nids;
}

function _webauth_addons_get_intranet_nids( $gen = false ) {

  // @FIXME
// Could not extract the default value because it is either indeterminate, or
// not scalar. You'll need to provide a default value in
// config/install/webauth_addons.settings.yml and config/schema/webauth_addons.schema.yml.
$vars = \Drupal::config('webauth_addons.settings')->get('webauth_addons_intranet_nids');

  if(!$vars || $gen ):

    if($gen):
      $message = 'Forced update of intranet pages cache from @file on line @line in @function function.';
      \Drupal::logger('webauth_addons')->notice(\Drupal\Component\Utility\SafeMarkup::checkPlain($message), array('@line' => __LINE__, '@file' => __FILE__, '@function' => __FUNCTION__));
    endif;

    $vars['ts'] = REQUEST_TIME;
    $vars['updated'] = date();

    // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $aliases = db_select('url_alias', 'a')
//       ->fields('a', array('source', 'alias'))
//       ->condition('a.alias', '%' . db_like( variable_get('intranet_base','intranet') ) . '%', 'LIKE' )
//       ->addTag('node_access')
//       ->execute()
//       ->fetchAll();


    foreach($aliases as $alias):
      $nids[] = end( explode('/', $alias->source) );

    endforeach;


    $aliases = NULL;
    $vars['nids'] = $nids;

    \Drupal::configFactory()->getEditable('webauth_addons.settings')->set('webauth_addons_intranet_nids', $vars)->save();

  endif;

  return $vars;

}
function webauth_addons_cron() {

  $frequency = \Drupal::config('webauth_addons.settings')->get('webauth_addons_intranet_frequency');
  $interval = 60 * 60 * 24 * $frequency;

  if ((REQUEST_TIME - \Drupal::config('webauth_addons.settings')->get('webauth_addons_last_cron')) > $interval):

    _webauth_addons_get_intranet_nids(true);

    $message = 'Updated intranet pages cache from @file on line @line in @function function.';
    \Drupal::logger('webauth_addons')->info(\Drupal\Component\Utility\SafeMarkup::checkPlain($message), array('@line' => __LINE__, '@file' => __FILE__, '@function' => __FUNCTION__));
    \Drupal::configFactory()->getEditable('webauth_addons.settings')->set('webauth_addons_last_cron', REQUEST_TIME)->save();
  endif;


}
/**
 * Implements hook_query_TAG_alter()
 */
function webauth_addons_query_search_node_alter(&$query) {

  if( (defined('WAA_DEBUG') &&  WAA_DEBUG) && ( $user->hasPermission('access devel information') || $user->hasPermission('view webauth addons debug') ) ):
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
  $elapsed = \Drupal::service("date.formatter")->formatInterval((time() - $vars['result']['node']->changed) , 3);

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
