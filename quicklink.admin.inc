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
      It will match strings ending with a period followed by 1-4 characters.'),
    '#default_value' => $config->get('ignore_file_ext'),
  );
  $form['ignore']['url_patterns_to_ignore'] = array(
    '#type' => 'textarea',
    '#title' => t('URL patterns to ignore (optional)'),
    '#description' => t('Quicklink will not fetch data if the URL contains any of these patterns. One per line.'),
    '#default_value' => $config->get('url_patterns_to_ignore'),
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
  );
  $form['overrides']['allowed_domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Override allowed domains (optional)'),
    '#description' => t('List of domains to prefetch from. If empty, Quicklink will only prefetch links from the origin domain.
      If you configure this, be sure to input the origin domain. Add <code>true</code> here to allow <em>every</em> origin.'),
    '#default_value' => $config->get('allowed_domains'),
  );
  $form['overrides']['prefetch_only_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Prefetch these paths only (overrides everything else)'),
    '#description' => t('If enabled, will override other settings. <strong>Only these paths will be prefetched.</strong> Include the forward slash at the beginning of the path.'),
    '#default_value' => $config->get('prefetch_only_paths'),
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
    '#description' => t('Recommended. Disables loading of the Quicklink library when a PHP session has been started. Useful for modules that use sessions (e.g. commerce shopping carts).'),
    '#default_value' => $config->get('no_load_when_session'),
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

  // Polyfill tab.
  $form['polyfill'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extended Browser Support'),
    '#description' => t('Include support of additional browsers via polyfill.'),
    '#group' => 'settings',
  );
  $form['polyfill']['load_polyfill'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load <em>Intersection Observer</em> polyfill'),
    '#description' => t('This checkbox will enable loading of the <a href="https://developer.mozilla.org/en-US/docs/Web/API/Intersection_Observer_API" target="_blank">
      Intersection Observer</a> polyfill from <a href="https://polyfill.io" target="_blank">polyfill.io</a>. This will enable usage of Quicklink in Safari and Microsoft Edge browsers.'),
    '#default_value' => $config->get('load_polyfill'),
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

  //return system_settings_form($form);
  // Add a submit button
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

  $config->set('no_load_content_types', $form_state['values']['no_load_content_types'])
    ->set('selector', trim($form_state['values']['selector']))
    ->set('url_patterns_to_ignore', trim($form_state['values']['url_patterns_to_ignore']))
    ->set('prefetch_only_paths', trim($form_state['values']['prefetch_only_paths']))
    ->set('no_load_when_authenticated', $form_state['values']['no_load_when_authenticated'])
    ->set('no_load_when_session', $form_state['values']['no_load_when_session'])
    ->set('ignore_admin_paths', $form_state['values']['ignore_admin_paths'])
    ->set('ignore_ajax_links', $form_state['values']['ignore_ajax_links'])
    ->set('ignore_hashes', $form_state['values']['ignore_hashes'])
    ->set('ignore_file_ext', $form_state['values']['ignore_file_ext'])
    ->set('allowed_domains', trim($form_state['values']['allowed_domains']))
    ->set('load_polyfill', $form_state['values']['load_polyfill'])
    ->set('enable_debug_mode', $form_state['values']['enable_debug_mode'])
    ->save();

  // Clear caches if debug mode has been enabled/disabled.
  if ($debug_mode != $form_state['values']['enable_debug_mode']) {
    cache_clear_all();
  }

}
