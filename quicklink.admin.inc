<?php

/**
 * @file
 * Contains quicklink.admin.inc.
 */

/**
 * Quicklink settings form.
 *
 * @ingroup forms
 * @see quicklink_settings_submit
 */
function quicklink_settings($form, $form_state) {
  $config = config('quicklink.settings');

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#attributes' => array(
      'class' => array('quicklink'),
    ),
  );

  // Ignore tab.
  $form['ignore'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prefetch Ignore Settings'),
    '#description' => t('Specify what Quicklink should not prefetch.'),
    '#group' => 'settings',
  );
  $form['ignore']['ignore_admin_paths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not prefetch admin paths'),
    '#description' => t('Highly recommended. Ignore administrative paths.'),
    '#default_value' => $config->get('ignore_admin_paths'),
  );
  $form['ignore']['ignore_ajax_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not prefetch AJAX links'),
    '#description' => t('Highly recommended. Ignore links that trigger AJAX behavior.'),
    '#default_value' => $config->get('ignore_ajax_links'),
  );
  $form['ignore']['ignore_hashes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore paths with hashes (#) in them'),
    '#description' => t('Recommended. Prevents multiple prefetches of the same page.'),
    '#default_value' => $config->get('ignore_hashes'),
  );
  $form['ignore']['ignore_file_ext'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore paths with file extensions'),
    '#description' => t('Recommended. This will ignore links that end with a file extension.
      It will match paths ending with a period followed by 1-5 characters. Querystrings are supported.'),
    '#default_value' => $config->get('ignore_file_ext'),
  );
  $form['ignore']['url_patterns_to_ignore'] = array(
    '#type' => 'textarea',
    '#title' => t('URL patterns to ignore (optional)'),
    '#description' => t('Quicklink will not fetch data if the URL contains any of these patterns. One per line.'),
    '#default_value' => $config->get('url_patterns_to_ignore'),
    '#resizable' => FALSE,
    '#attributes' => array(
      'style' => 'max-width: 600px;',
    ),
  );
  $form['ignore']['ignore_selectors'] = array(
    '#type' => 'textarea',
    '#title' => t('Ignore these selectors (optional)'),
    '#description' => t('Selectors that will not be prefected. Enter one value per line. Example: <code>.footer a</code>'),
    '#default_value' => $config->get('ignore_selectors'),
    '#attributes' => array(
      'style' => 'max-width: 600px;',
    ),
  );

  // Overrides tab.
  $form['overrides'] = array(
    '#type' => 'fieldset',
    '#title' => t('Optional Overrides'),
    '#description' => t('Specify various overrides.'),
    '#group' => 'settings',
  );
  $form['overrides']['selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Override parent selector (optional)'),
    '#description' => t('Quicklink will search this CSS selector for URLs to prefetch (ex. <code>.body-inner</code>). Defaults to the whole document.'),
    '#maxlength' => 128,
    '#size' => 128,
    '#default_value' => $config->get('selector'),
    '#attributes' => array(
      'style' => 'max-width: 600px;',
    ),
  );
  $form['overrides']['allowed_domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Override allowed domains (optional)'),
    '#description' => t('List of domains to prefetch from. If empty, Quicklink will only prefetch links from the origin domain.
      If you configure this, be sure to input the origin domain. Add <code>true</code> here to allow <em>every</em> origin.'),
    '#default_value' => $config->get('allowed_domains'),
    '#resizable' => FALSE,
    '#attributes' => array(
      'style' => 'max-width: 600px;',
    ),
  );

  // When to Prefetch tab.
  $form['when_load_library'] = array(
    '#type' => 'fieldset',
    '#title' => t('When to Load Library'),
    '#description' => t('Specify when the Quicklink library will be loaded.'),
    '#group' => 'settings',
  );
  $form['when_load_library']['no_load_when_authenticated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefetch for anonymous users only'),
    '#description' => t('Highly recommended. Quicklink library will not be loaded for authenticated users.'),
    '#default_value' => $config->get('no_load_when_authenticated'),
  );
  $form['when_load_library']['no_load_when_session'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not prefetch during sessions'),
    '#description' => t('Recommended. Disables loading of the Quicklink library when a PHP session has been started. Useful for modules that use sessions (e.g. shopping carts).'),
    '#default_value' => $config->get('no_load_when_session'),
  );
  $form['when_load_library']['no_load_without_page_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not prefetch if page caching for anonymous users is disabled'),
    '#description' => t('Highly recommended. Disables loading of the Quicklink library when page caching for anonymous users is disabled.'),
    '#default_value' => $config->get('no_load_without_page_cache'),
  );

  $options = array();
  $types = node_type_get_types();

  foreach ($types as $type) {
    $options[$type->type] = $type->name;
  }

  $form['when_load_library']['no_load_content_types'] = array(
    '#title' => t('Do not load library on these content types:'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $config->get('no_load_content_types'),
  );

  // Parameters tab.
  $form['throttle_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Throttle Options'),
    '#description' => t('On this tab, set parameters to adjust how Quicklink loads content.'),
    '#group' => 'settings',
  );
  $form['throttle_options']['total_request_limit'] = array(
    '#type' => 'number',
    '#title' => t('Set request limit'),
    '#description' => t('Enter the total number of requests that can be
      prefetched for a given page. Set to 0 for unlimited. <br><em>Warning</em>: this may not
      work when Concurrency Throttle is also enabled (see
      <a href="https://github.com/GoogleChromeLabs/quicklink/issues/235">https://github.com/GoogleChromeLabs/quicklink/issues/235</a>.'),
    '#maxlength' => 10,
    '#size' => 10,
    '#default_value' => $config->get('total_request_limit'),
  );
  $form['throttle_options']['concurrency_throttle_limit'] = array(
    '#type' => 'number',
    '#title' => t('Set concurrency throttle'),
    '#description' => t('Enter a limit for the simultaneous requests that can be made while prefetching. Set to 0 for unlimited.'),
    '#maxlength' => 10,
    '#size' => 10,
    '#default_value' => $config->get('concurrency_throttle_limit'),
  );
  $form['throttle_options']['viewport_delay'] = array(
    '#type' => 'number',
    '#title' => t('Viewport Delay'),
    '#field_suffix' => t('ms'),
    '#description' => t('Amount of time each link needs to stay inside the viewport before being prefetched. Default is 0 ms.'),
    '#maxlength' => 10,
    '#size' => 10,
    '#default_value' => $config->get('viewport_delay'),
  );
  $form['throttle_options']['idle_wait_timeout'] = array(
    '#type' => 'number',
    '#title' => t('Set idle timeout value'),
    '#field_suffix' => t('ms'),
    '#description' => t('Amount of time the browser must be idle before prefetching, in milliseconds. Default is 2000 ms.'),
    '#maxlength' => 10,
    '#size' => 10,
    '#default_value' => $config->get('idle_wait_timeout'),
  );

  // Prefetch Paths Only Tab
  $form['paths_only'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prefetch Paths Only'),
    '#description' => t('On this tab, prefetch paths setting that will override all other settings.'),
    '#group' => 'settings',
  );
  $form['paths_only']['warning_message'] = array(
    '#type' => 'markup',
    '#prefix' => '<p>',
    '#markup' => t('Warning: This setting negates all other settings.'),
    '#suffix' => '</p>',
    '#attributes' => array(
      'class' => 'color-warning',
      'style' => 'max-width: 600px; padding: 1rem; font-weight: bold;',
    ),
  );
  $form['paths_only']['prefetch_only_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Prefetch these paths only (overrides everything else)'),
    '#description' => t('If enabled, this setting will override all other prefetch settings. <strong>Only these paths will be prefetched.</strong> Include the forward slash at the beginning of the path. Enter one value per line.'),
    '#default_value' => $config->get('prefetch_only_paths'),
    '#attributes' => array(
      'style' => 'max-width: 600px;',
    ),
  );

  // Debug tab.
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug'),
    '#description' => t('Enable debug logging.'),
    '#group' => 'settings',
  );
  $form['debug']['enable_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#description' => t("Log Quicklink development information to the HTML and JavaScript console."),
    '#default_value' => $config->get('enable_debug_mode'),
  );

  if ($config->get('enable_debug_mode')) {
    backdrop_set_message('Quicklink debug mode enabled. Be sure to disable this on production.', 'warning');
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler for quicklink_settings.
 */
function quicklink_settings_submit(&$form, $form_state) {
  $config = config('quicklink.settings');
  $debug_mode = $config->get('enable_debug_mode');

  $config->set('no_load_content_types', array_filter($form_state['values']['no_load_content_types']))
    ->set('selector', trim($form_state['values']['selector']))
    ->set('url_patterns_to_ignore', trim($form_state['values']['url_patterns_to_ignore']))
    ->set('ignore_selectors', trim($form_state['values']['ignore_selectors']))
    ->set('prefetch_only_paths', trim($form_state['values']['prefetch_only_paths']))
    ->set('no_load_when_authenticated', $form_state['values']['no_load_when_authenticated'])
    ->set('no_load_without_page_cache', $form_state['values']['no_load_without_page_cache'])
    ->set('no_load_when_session', $form_state['values']['no_load_when_session'])
    ->set('ignore_admin_paths', $form_state['values']['ignore_admin_paths'])
    ->set('ignore_ajax_links', $form_state['values']['ignore_ajax_links'])
    ->set('ignore_hashes', $form_state['values']['ignore_hashes'])
    ->set('ignore_file_ext', $form_state['values']['ignore_file_ext'])
    ->set('allowed_domains', trim($form_state['values']['allowed_domains']))
    ->set('total_request_limit', $form_state['values']['total_request_limit'])
    ->set('concurrency_throttle_limit', $form_state['values']['concurrency_throttle_limit'])
    ->set('viewport_delay', $form_state['values']['viewport_delay'])
    ->set('idle_wait_timeout', $form_state['values']['idle_wait_timeout'])
    ->set('enable_debug_mode', $form_state['values']['enable_debug_mode'])
    ->save();

  // Clear caches if debug mode has been enabled/disabled.
  if ($debug_mode != $form_state['values']['enable_debug_mode']) {
    cache_clear_all();
  }

}
