<?php
/**
 * @file Views include file with views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function newsml_views_plugins() {
  $path = drupal_get_path('module', 'newsml');
  return array(
    'style' => array(
      'newsml' => array(
        'title' => t('NewsML'),
        'help' => t('Generates an NewsML feed from a view.'),
        'handler' => 'views_plugin_style_newsml',
        'theme' => 'views_view_newsml',
        'theme path' => $path . '/theme',
        'theme file' => 'newsml.theme.inc',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'theme file' => 'newsml.theme.inc',
      ),
    ),
    'row' => array(
      'node_newsml' => array(
        'title' => t('NewsML Content'),
        'help' => t('Display the content with standard node view.'),
        'handler' => 'views_plugin_row_node_newsml',
        'theme' => 'views_view_row_newsml',
        'theme path' => $path . '/theme',
        'theme file' => 'newsml.theme.inc',
        'base' => array('node'),
        'uses options' => FALSE,
        'type' => 'feed',
      ),
    ),
  );
}
