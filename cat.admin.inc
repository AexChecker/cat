<?php

/**
 * @file
 * Administrative page callbacks for menu module.
 */

/**
 * Menu callback which shows an overview page of all the custom
 * "Context Actions" and "Tabs" (CAT).
 */
function cat_overview_page() {
  $result = db_query('
    SELECT
      mrc.*,
      mr.*
    FROM {menu_router_cat} mrc
    JOIN {menu_router} mr ON mr.path = mrc.path
    ORDER BY mrc.path
  ');

  $header = array(
    t('Path'),
    'access_callback',
    'page_callback',
    'tab_parent',
    t('Type'),
    t('Operations')
  );

  $rows = array();

  foreach ($result as $menu) {
    $row = array();
    $row[] = $menu->path;
    if (!empty($menu->access_callback)) {
      $row[] = $menu->access_callback;
    }
    else {
      $row[] = t('none');
    }
    $row[] = $menu->page_callback;
    if (!empty($menu->tab_parent)) {
      $row[] = $menu->tab_parent;
    }
    else {
      $row[] = t('none');
    }
    if (!isset($menu->hidden) || $menu->hidden == MENU_LOCAL_CAT_VISIBLE) {
      $row[] = t('Visible');
    }
    elseif ($menu->hidden == MENU_LOCAL_CAT_HIDDEN) {
      $row[] = t('Hidden');
    }
    elseif ($menu->hidden == MENU_LOCAL_CAT_DISABLED) {
      $row[] = t('Disabled');
    }
    elseif ($menu->hidden == MENU_LOCAL_CAT_DELATED) {
      $row[] = t('Delated');
    }
    else {
      $row[] = t('Unknown');
    }
    $row[] = l(t('edit'), 'admin/structure/menu/cat/edit/' . $menu->pid);
    $rows[] = $row;
  }

  if (!$rows) {
    $rows[] = array(
      array(
        'data' => t('No override menu item.'),
        'colspan' => '6',
      )
    );
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
  ));
}

/**
 * Menu callback.
 */
function cat_menu_hide_menu($form, &$form_state) {
  $form = array();

  $form['path'] = array(
    '#title' => t('Drupal parh'),
    '#type' => 'textfield',
    '#maxlength' => 256,
    '#required' => TRUE,
    '#autocomplete_path' => 'admin/structure/menu/cat/autocomplete/path'
  );

  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Type'),
    '#default_value' => MENU_LOCAL_CAT_HIDDEN,
    '#options' => array(
      MENU_LOCAL_CAT_HIDDEN => t('Hide'),
      MENU_LOCAL_CAT_DISABLED => t('Disable'),
      MENU_LOCAL_CAT_DELATED => t('Delete')
    )
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  return $form;
}

function cat_menu_hide_menu_validate($form, &$form_state) {
  $drupal_path = db_select('menu_router', 'mr')
    ->fields('mr', array('path'))
    ->condition('mr.path', $form_state['values']['path'], 'LIKE')
    ->execute()
    ->fetchField();

  if ($drupal_path != $form_state['values']['path']) {
    form_error($form['path'], t('Wrong path.'));
  }
  if (empty($form_state['clicked_button']['#value'])) {
    form_error($form['actions'], t('Please chose action.'));
  }
}

function cat_menu_hide_menu_submit($form, &$form_state) {
  $primary_keys = array();

  $record = array(
    'path' => $form_state['values']['path'],
    'hidden' => $form_state['values']['type']
  );

  $existed_record = db_select('menu_router_cat', 'm')
    ->fields('m')
    ->condition('m.path', $record['path'])
    ->execute()
    ->fetchAssoc();

  if ($existed_record) {
    $record = array_merge((array) $existed_record, $record);
    $primary_keys = array('pid');
  }

  $record = (object) $record;
  drupal_write_record('menu_router_cat', $record, $primary_keys);

  $form_state['redirect'] = array('admin/structure/menu/cat');
  menu_rebuild();
}

/**
 * Menu callback.
 */
function cat_menu_edit_menu($form, &$form_state, $pid) {
  $form = array();
  $form += cat_menu_hide_menu($form, $form_state);

  $path = db_select('menu_router_cat', 'm')
    ->fields('m')
    ->condition('m.pid', $pid)
    ->execute()
    ->fetchObject();

  $form['#path'] = $path;

  $form['type']['#default_value'] = $path->hidden;
  $form['type']['#options'] = array(
    MENU_LOCAL_CAT_VISIBLE => t('Visible/Revert'),
    MENU_LOCAL_CAT_HIDDEN => t('Hide'),
    MENU_LOCAL_CAT_DISABLED => t('Disable'),
    MENU_LOCAL_CAT_DELATED => t('Delete')
  );

  $form['path']['#default_value'] = $path->path;
  $form['path']['#disabled'] = TRUE;

  return $form;
}

function cat_menu_edit_menu_submit($form, &$form_state) {
  $path = $form['#path'];
  $path->hidden = $form_state['values']['type'];

  drupal_write_record('menu_router_cat', $path, array('pid'));

  $form_state['redirect'] = array('admin/structure/menu/cat');
  menu_rebuild();
}

/**
 * Menu callback.
 */
function cat_autocomplete_path() {
  $args = func_get_args();

  $matches = array();

  if (!empty($args)) {
    $string = trim(implode('/', $args), '/');

    if (!empty($_SERVER['REQUEST_URI'])) {
      if (substr($_SERVER['REQUEST_URI'], -1, 1) === '/') {
        $string .= '/';
      }
    }

    // Select menu paths
    $result = db_select('menu_router', 'mr')
      ->fields('mr', array('path'))
      ->condition('mr.path', db_like($string) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();

    // add matches to $matches
    foreach ($result as $path) {
      $matches[$path->path] = check_plain($path->path);
    }
  }

  drupal_json_output($matches);
}
