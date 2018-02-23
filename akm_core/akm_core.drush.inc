<?php

/**
 * Implements hook_drush_command().
 */
function akm_core_drush_command() {

  $items['drush-export-prepare'] = array(
    'description' => 'Prepare progect to export',
    'aliases' => array('dep'),
  );

  return $items;
}

/**
 * Callback for the drush-demo-command command
 */
function drush_akm_core_drush_export_prepare() {
  global $databases;

  // Set active GCE adminim theme.
  variable_set('admin_theme', 'gce');
  // Clear sitemap xml variable.
  if (variable_get('xmlsitemap_base_url')) {
    variable_set('xmlsitemap_base_url', '');
  }

  $tables = drupal_get_schema();
  // Turncate all cache tables.
  foreach ($tables as $name => $fields) {
    if (substr($name, 0, 6) === 'cache_') {
      $result = db_truncate($name)->execute();
    }
  }
  // Turncate watchdog table.
  $result = db_truncate('watchdog')->execute();

  // Get db connection info.
  $database = isset($databases['default']['default']['database']) ? $databases['default']['default']['database'] : '';
  $username = isset($databases['default']['default']['username']) ? $databases['default']['default']['username'] : '';
  $password = isset($databases['default']['default']['password']) ? $databases['default']['default']['password'] : '';
  // Mysql dump.
  drush_shell_exec('mysqldump -u ' . $username . ' -p' . $password . ' ' . $database . ' > ../db/latest.sql');
}
