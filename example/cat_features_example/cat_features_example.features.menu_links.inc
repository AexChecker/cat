<?php
/**
 * @file
 * cat_features_example.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function cat_features_example_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: navigation:cat-menu
  $menu_links['navigation:cat-menu'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'cat-menu',
    'router_path' => 'cat-menu',
    'link_title' => 'CAT menu example',
    'options' => array(
      'attributes' => array(
        'title' => 'CAT menu example.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '10',
  );
  // Exported menu link: navigation:cat-menu/example-02
  $menu_links['navigation:cat-menu/example-02'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'cat-menu/example-02',
    'router_path' => 'cat-menu/example-02',
    'link_title' => 'Example #2',
    'options' => array(
      'attributes' => array(
        'title' => 'CAT menu example #02.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'cat-menu',
  );
  // Exported menu link: navigation:cat-menu/example-03
  $menu_links['navigation:cat-menu/example-03'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'cat-menu/example-03',
    'router_path' => 'cat-menu/example-03',
    'link_title' => 'Example #3',
    'options' => array(
      'attributes' => array(
        'title' => 'CAT menu example #03.',
      ),
    ),
    'module' => 'system',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'cat-menu',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('CAT menu example');
  t('Example #2');
  t('Example #3');


  return $menu_links;
}
