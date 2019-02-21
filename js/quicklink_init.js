'use strict';

(function () {
  Backdrop.behaviors.quicklink = {
    'attach': function attachQuicklink(context, settings) {

      var debug = Backdrop.settings.quicklink.debug;

      function hydrateQuicklinkConfig() {
        Backdrop.settings.quicklink.quicklinkConfig = Backdrop.settings.quicklink.quicklinkConfig || {};
        Backdrop.settings.quicklink.ignoredSelectorsLog = Backdrop.settings.quicklink.ignoredSelectorsLog || [];

        var quicklinkConfig = Backdrop.settings.quicklink.quicklinkConfig;
        var ignoredSelectorsLog = Backdrop.settings.quicklink.ignoredSelectorsLog;

        quicklinkConfig.ignores = [];

        // Loop through all the patterns to ignore, and generate rules to ignore URL patterns.
        for (var i = 0; i < Backdrop.settings.quicklink.url_patterns_to_ignore.length; i++) {
          var pattern = Backdrop.settings.quicklink.url_patterns_to_ignore[i];

          (function(i, pattern) {
            if (pattern.length) {
              quicklinkConfig.ignores.push(function(uri, elem) {
                var ruleName = 'Pattern found in href. See ignored selectors log.';
                var ruleFunc = uri.includes(pattern);

                outputDebugInfo(ruleFunc, ruleName, uri, elem, pattern);

                return ruleFunc;
              });
            }
          })(i, pattern);
        }

        if (Backdrop.settings.quicklink.ignore_admin_paths) {
          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Exists in admin element container.';
            var ruleFunc = elem.matches('#admin-bar a, .contextual-links-wrapper a');

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });
        }

        if (Backdrop.settings.quicklink.ignore_ajax_links) {
          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Link has "use-ajax" CSS class.';
            var ruleFunc = elem.classList.contains('use-ajax');

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });

          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Link has "/ajax" in url.';
            var ruleFunc = uri.includes('/ajax');

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });
        }

        if (Backdrop.settings.quicklink.ignore_file_ext) {
          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Contains file extension at end of href.';
            var ruleFunc = uri.match(/\.[^\/]{1,4}$/);

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });
        }

        quicklinkConfig.ignores.push(function(uri, elem) {
          var ruleName = 'Contains noprefetch attribute.';
          var ruleFunc = elem.hasAttribute('noprefetch');

          outputDebugInfo(ruleFunc, ruleName, uri, elem);

          return ruleFunc;
        });

        quicklinkConfig.ignores.push(function(uri, elem) {
          var ruleName = 'Contains download attribute.';
          var ruleFunc = elem.hasAttribute('download');

          outputDebugInfo(ruleFunc, ruleName, uri, elem);

          return ruleFunc;
        });

        quicklinkConfig.origins = (Backdrop.settings.quicklink.allowed_domains) ? Backdrop.settings.quicklink.allowed_domains : false;
        quicklinkConfig.urls = (Backdrop.settings.quicklink.prefetch_only_paths) ? Backdrop.settings.quicklink.prefetch_only_paths : false;
      }

      function outputDebugInfo(ruleFunc, ruleName, uri, elem, pattern) {
        if (debug && ruleFunc) {
          var debugMessage = ruleName + ' Link ignored.';
          var thisLog = {};
          var pattern = pattern || false;

          elem.classList.add('quicklink-ignore');
          elem.textContent += '🚫';
          elem.dataset.quicklinkMatch = debugMessage;

          thisLog.ruleName = ruleName;
          thisLog.uri = uri;
          thisLog.elem = elem;
          thisLog.message = debugMessage;

          if (pattern) thisLog.pattern = pattern;

          (function(thisLog) {
            Backdrop.settings.quicklink.ignoredSelectorsLog.push(thisLog);
          })(thisLog);
        }
      }

      function loadQuicklink() {
        var urlParams = new URLSearchParams(window.location.search);
        var noprefetch = urlParams.get('noprefetch') !== null;

        if (noprefetch && debug) {
          console.info('The "noprefetch" parameter exists in the URL querystring. Quicklink library not loaded.');
        }

        return window.quicklink && !noprefetch;
      }

      if (!Backdrop.settings.quicklink.quicklinkConfig) hydrateQuicklinkConfig();

      Backdrop.settings.quicklink.quicklinkConfig.el = (Backdrop.settings.quicklink.selector) ? context.querySelector(Backdrop.settings.quicklink.selector) : context;

      if (debug) {
        console.info('Quicklink config object', Backdrop.settings.quicklink.quicklinkConfig);
        console.info('Quicklink module debug log', Backdrop.settings.quicklink.debug_log);
        console.info('Quicklink ignored selectors log', Backdrop.settings.quicklink.ignoredSelectorsLog);
      }

      if (loadQuicklink()) quicklink(Backdrop.settings.quicklink.quicklinkConfig);
    },
  };
})();
