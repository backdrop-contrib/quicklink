# Quicklink

## What is Quicklink?

[Quicklink library](https://github.com/GoogleChromeLabs/quicklink) attempts to make navigations to subsequent pages load faster. This module loads the library and provides a Backdrop administrative interface to configure it.

Quicklink:

* Detects links within the viewport.
* Waits until the browser is idle.
* Checks if the user isn't on a slow connection or has data-saver enabled.
* Prefetches URLs to the links (using <link rel=prefetch> or XHR). Provides some control over the request priority (can switch to fetch() if supported).

## Installation

1. Download the Backdrop module and extract it to your modules folder.

By default this module will load the Quicklink JavaScript library from a CDN at
`https://unpkg.com/quicklink@1.0.0/dist/quicklink.umd.js`.

## How do I access the Quicklink admin interface?

The Quicklink module admin interface is located at
`admin/config/development/performance/quicklink`, and can be accessed from a tab on the development
/ performance settings page.


## What browsers does this support?

Without polyfills:
Chrome, Firefox.

With [Intersection Observer polyfill](https://github.com/w3c/IntersectionObserver/tree/master/polyfill):
Safari, Edge


## How to ignore certain links.

You can tell Quicklink to ignore certain links by adding them into the ignore
list at `admin/config/development/performance/quicklink`.

In addition, you can add a `noprefetch` attribute onto the `<a>` tag to tell
the library not to prefetch this link.

## Issues

To submit bug reports and feature suggestions, or to track changes:
  http://github.com/backdrop-contrib/quicklink/issues

## Current Maintainers

- Herb v/d Dool (https://github.com/herbdool/)
- Seeking additional maintainers.

## Credits

- Ported to Backdrop by Herb v/d Dool (https://github.com/herbdool/)
- Originally developed for Drupal by [mherchel](https://www.drupal.org/u/mherchel)

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
