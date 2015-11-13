<?php /**
 * @file
 * Contains \Drupal\webauth_addons\EventSubscriber\InitSubscriber.
 */

namespace Drupal\webauth_addons\EventSubscriber;

use Symfony\Component\HttpKernel\KernelEvents;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

class InitSubscriber implements EventSubscriberInterface {

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    return [KernelEvents::REQUEST => ['onEvent', 0]];
  }

  public function onEvent() {

    if (!module_exists('webauth')): // Check for webauth module
      drupal_set_message(t('The Stanford WebAuth module must be enabled to use WebAuth Add-Ons.'), 'error');
      return FALSE;
    endif;

    // Load module includes
    module_load_include('inc', 'webauth_addons', 'includes/webauth_addons');
    module_load_include('inc', 'webauth_addons', 'includes/webauth_addons.form');
  }

}
