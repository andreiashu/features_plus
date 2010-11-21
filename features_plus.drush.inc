<?php

/**
 * @file
 *   Features Plus module drush integration.
 */

/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function features_plus_drush_command() {
  $items = array();

  $items['features-component-export'] = array(
    'description' => "Export a component feature from your site into a module.",
    'arguments' => array(
      'component' => 'Component name to export.',
    ),
    'drupal dependencies' => array('features', 'features_plus'),
    'aliases' => array('fce'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function features_plus_drush_help($section) {
  switch ($section) {
    case 'drush:features-component-export':
      return dt("Export a whole component (ie. variable, permission, cck) from your site into a module.");
  }
}

/**
 * Create a feature module from a component.
 */
function drush_features_component_export() {
  $args = func_get_args();

  if (count($args) == 1) {
    // Assume that the user intends to create a module with the same name as the
    // "value" of the component.
    $component = $args[0];
    $component_skip_keys = features_plus_component_skip_keys($component);
    $stub = features_plus_get_component($component, $component_skip_keys);
    _drush_features_export($stub, $component);
  }
  elseif (count($args) > 1) {
    // Assume that the user intends to create a new module based on a list of
    // components. First argument is assumed to be the name.
    $name = array_shift($args);
    $stub = array();
    foreach ($args as $component) {
      $component_skip_keys = features_plus_component_skip_keys($component);
      $stub = array_merge($stub, features_plus_get_component($component, $component_skip_keys));
    }
    _drush_features_export($stub, array(), $name);
  }
  else {
    $rows = array(array(dt('Available components')));
    foreach (features_get_components(TRUE) as $component => $info) {
      $rows[] = array($component);
    }
    drush_print_table($rows, TRUE);
  }
}
