# Webauth Add-Ons


### What this module does:
* Creates a new option under _Who can register accounts_ at https://example.stanford.edu/admin/config/people/accounts#edit-registration-cancellation for Stanford Only
* Uses <a href="https://api.drupal.org/api/drupal/modules!search!search.pages.inc/function/template_preprocess_search_results/7" title="template_preprocess_search_results">template_preprocess_search_results</a> to prevent intranet search results from being displayed to non webauth users.
* Removes forms from /user/register and /user/password while rewriting /user to force login with webauth.
* Adds a function to check for defined sunet role :
 
```php
if(!function_exists('sunet_role')):
  function sunet_role() {
    global $user;
    return array_key_exists(variable_get('webauth_addons_role', '1'), $user->roles);
  }
endif;```

### What this module does not do:

* Implement webauth security

## Requirments

* Stanford webauth drupal module (https://drupalfeatures.stanford.edu/project/webauth)
* Global Redirect (https://www.drupal.org/project/globalredirect)
