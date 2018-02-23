<?php

/**
 * @file
 * Provides a controller for building an entity overview form.
 */
class InvoicesUIController extends EntityDefaultUIController {


  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    // Exclude add item.
    unset($items[$this->path . '/add']);
    return $items;
  }

  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    // By default just show a simple overview for all entities.
    $this->addEntityFilterForm($form, $form_state);

    $form['pager'] = array('#theme' => 'pager');

    $form['table'] = array(
      '#theme' => 'table',
      '#header' => $this->overviewTableHeaders(),
      '#rows' => $this->overviewTable(),
      '#empty' => t('There are no invoices.'),
    );

    return $form;
  }

  /**
   * Returns the entity administration filters form array.
   *
   * @ingroup forms
   */
  public function addEntityFilterForm(&$form, &$form_state) {
    $session = isset($_SESSION[$this->entityType . '_overview_filter']) ? $_SESSION[$this->entityType . '_overview_filter'] : array();
    $entity_fields = self::getInvoicesFields('for_filter');

    $form['filters'] = array(
      '#type' => 'fieldset',
      '#title' => t('Show only items where'),
      '#collapsible' => TRUE,
      '#collapsed' => count($session) ? FALSE : TRUE,
    );

    foreach ($entity_fields as $key => $field) {
      $form['filters'][$key] = array(
        '#title' => $field['description'],
        '#default_value' => isset($session[$key]['value']) ? $session[$key]['value'] : '',
        '#type' => 'textfield',
        '#maxlength' => 64,
        '#size' => 20,
      );
      // Add condition to filtering date.
      if ($field['is_expanded_filter']) {
        $form['filters']['condition_' . $key] = array(
          '#type' => 'select',
          '#options' => array(
            '=' => t('equal'),
            '>' => t('more than'),
            '<' => t('less than'),
            '!=' => t('not equal'),
          ),
          '#default_value' => isset($session[$key]['condition']) ? $session[$key]['condition'] : '',
        );
      }
    }

    $form['filters']['status']['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['filters']['status']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => count($session) ? t('Refine') : t('Filter'),
    );
    if (count($session)) {
      $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
    }

    return $form;
  }


  /**
   * Generates the render array for a overview table for arbitrary entities
   * matching the given conditions.
   *
   * @param $conditions
   *   An array of conditions as needed by entity_load().
   * @return Array
   *   A renderable array.
   */
  public function overviewTable($conditions = array()) {

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

    // Add all conditions to query.
    $conditions = isset($_SESSION[$this->entityType . '_overview_filter']) ? $_SESSION[$this->entityType . '_overview_filter'] : array();
    foreach ($conditions as $key => $value) {
      if (is_scalar($value)) {
        $query->propertyCondition($key, $value);
      }
      elseif (is_array($value) && isset($value['value'])) {
        // Convert date to timestamp.
        if (self::isDateField($key)) {
          $value['value'] = akm_invoices_formatted_date($value['value'], 'timestamp');
        }
        if (isset($value['condition']) && !empty($value['condition'])) {
          $query->propertyCondition($key, $value['value'], $value['condition']);
        }
        else {
          $query->propertyCondition($key, $value['value']);
        }
      }
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $headers = $this->filterTableHeaders();
    $query->tableSort($headers);

    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $entities = $ids ? entity_load($this->entityType, $ids) : array();
    $entity_fields = self::getInvoicesFields('for_table');
    $target_blank = array('attributes' => array('target' => '_blank'), 'html' => TRUE);
    $rows = array();
    $fids = array();

    foreach ($entities as $entity) {
      if (isset($entity->fid_of_paid_bill) && $entity->fid_of_paid_bill) {
        $fids[] = $entity->fid_of_paid_bill;
      }
    }
    $files = file_load_multiple($fids);

    foreach ($entities as $entity) {
      foreach ($entity_fields as $key => $field) {
        if ($key == 'fid_of_paid_bill') {
          if (isset($files[$entity->$key])) {
            $rows[$entity->iid][$key] = l($files[$entity->$key]->filename, file_create_url($files[$entity->$key]->uri), $target_blank);
          }
          else {
            $rows[$entity->iid][$key] = '';
          }
        }
        elseif ($key == 'user_name') {
          $rows[$entity->iid][$key] = l($entity->$key, 'user/' . $entity->uid);
        }
        elseif (self::isDateField($key)) {
          $rows[$entity->iid][$key] = akm_invoices_formatted_date($entity->$key);
        }
        else {
          $rows[$entity->iid][$key] = $entity->$key;
        }
      }

      //Add specify class to rows.
      if (!empty($rows[$entity->iid]['expiration_date']) && $entity->expiration_date > time()) {
        $rows[$entity->iid]['#attributes']['class'][] = 'not-expired';
      }

      // Add operations to invoice.
      $rows[$entity->iid]['operations'] = l(t('view'), '/user/invoices/view/' . $entity->hash, $target_blank) . ' ' .
        l(t('edit'), $this->path . '/manage/' . $entity->iid) . ' ' .
        l(t('delete'), $this->path . '/manage/' . $entity->iid . '/delete', array('query' => drupal_get_destination()));
    }

    return $rows;
  }

  /**
   * Generates the table headers for the overview table.
   */
  protected function overviewTableHeaders($conditions = '', $rows = '', $additional_header = array()) {
    $entity_fields = self::getInvoicesFields('for_table');

    foreach ($entity_fields as $key => $field) {
      $header[$key]['data'] = t('@title', array('@title' => $field['description']));
      // Make field sortable.
      if ($field['is_sortable']) {
        $header[$key]['field'] = $key;
        // Add sort by default.
        if ($field['is_sortable_by_default']) {
          $header[$key]['sort'] = $field['is_sortable_by_default'];
        }
      }
    }

    // Add title to opreration.
    $header['operations'] = array('data' => t('Operations'));

    return $header;
  }

  /**
   * Generates the table headers for the overview table.
   */
  protected function filterTableHeaders() {
    $entity_fields = self::getInvoicesFields('for_table');
    foreach ($entity_fields as $key => $field) {
      $header[$key] = array(
        'data' => $field['description'],
        'type' => 'property',
        'specifier' => $key,
      );
      // Add sort by default.
      if ($field['is_sortable_by_default']) {
        $header[$key]['sort'] = $field['is_sortable_by_default'];
      }
    }

    return $header;
  }

  public function overviewFormSubmit($form, &$form_state) {
    $entity_fields = self::getInvoicesFields('for_filter');

    switch ($form_state['values']['op']) {
      case t('Filter'):
      case t('Refine'):
        // Apply every filter that has a choice selected other than 'any'.
        foreach ($entity_fields as $filter => $options) {
          if (isset($form_state['values'][$filter]) && !empty($form_state['values'][$filter])) {
            $_SESSION[$this->entityType . '_overview_filter'][$filter]['value'] = $form_state['values'][$filter];
            if (isset($form_state['values']['condition_' . $filter]) && !empty($form_state['values']['condition_' . $filter])) {
              $_SESSION[$this->entityType . '_overview_filter'][$filter]['condition'] = $form_state['values']['condition_' . $filter];
            }
          }
        }
        break;

      case t('Reset'):
        $_SESSION[$this->entityType . '_overview_filter'] = array();
        break;
    }
  }

  /**
   * Check is this date fields of Entity.
   *
   * return bool
   */
  public static function isDateField($field) {
    $entity_fields_info = self::getInvoicesFieldsinfo();
    return $entity_fields_info[$field]['is_date'];
  }

  /**
   *  Give information about entity fields
   */
  public static function getInvoicesFieldsinfo($all = FALSE) {
    $clear_entity_fields = & drupal_static(__FUNCTION__, NULL);

    if (!isset($clear_entity_fields)) {
      $entity_fields = akm_invoices_get_table_fields();
      foreach ($entity_fields as $key => $value) {

        // Add default options
        $clear_entity_fields[$key] = array(
          'description' => $value['description'],
          'type' => $value['type'],
          'form_type' => 'textfield',
          'is_date' => FALSE,
          'is_sortable' => TRUE,
          'is_sortable_by_default' => FALSE,
          'is_expanded_filter' => TRUE,
          'is_link' => FALSE,

        );

        // Mark date fields
        if (in_array($key, array('start_date', 'stop_date', 'expiration_date'))) {
          $clear_entity_fields[$key]['is_date'] = TRUE;
        }
        // Make expiration_date sortable by default
        if ($key == 'expiration_date') {
          $clear_entity_fields[$key]['is_sortable_by_default'] = 'desc';
        }
        // Modify image field
        if ($key == 'fid_of_paid_bill') {
          $clear_entity_fields[$key]['form_type'] = 'managed_file';
        }
        // Modify subsripion_type field
        if ($key == 'subsripion_type') {
          $clear_entity_fields[$key]['is_expanded_filter'] = FALSE;
        }
        // Make user_name field link to uid
        if ($key == 'user_name') {
          $clear_entity_fields[$key]['is_link'] = 'uid';
          $clear_entity_fields[$key]['is_expanded_filter'] = FALSE;
        }
        // Modify invoice_price field
        if ($key == 'invoice_price') {
          $clear_entity_fields[$key]['is_expanded_filter'] = FALSE;
        }

      }
    }

    return $clear_entity_fields;
  }

  /**
   * Retrieve specific information about entity fields.
   */
  public static function getInvoicesFields($type = 'for_table') {
    $entity_fields_info = self::getInvoicesFieldsinfo();

    switch ($type) {
      case 'for_filter':
        unset($entity_fields_info['fid_of_paid_bill']);

      case 'for_table':
      case 'for_form':
        unset($entity_fields_info['iid']);
        unset($entity_fields_info['uid']);
        unset($entity_fields_info['hash']);
        break;

      default:
        break;
    }

    return $entity_fields_info;
  }
}

/**
 * Edit/update entity form.
 * 
 * Form id like ENTITY_TYPE_form.
 * @see entity_ui_get_form()
 */
function akm_invoices_form($form, &$form_state) {
  $entity = $form_state['build_info']['args'][0];
  $entity_fields = InvoicesUIController::getInvoicesFields('for_form');
  foreach ($entity_fields as $key => $field) {
    if ($field['form_type'] == 'textfield') {
      $form[$key] = array(
        '#title' => $field['description'],
        '#default_value' => $field['is_date'] ? akm_invoices_formatted_date($entity->$key) : $entity->$key,
        '#type' => $field['form_type'],
        '#maxlength' => 255,
        '#required' => empty($entity->$key) ? FALSE : TRUE,
      );
    }
    elseif ($field['form_type'] == 'managed_file') {
      $form[$key] = array(
        '#title' => $field['description'],
        '#type' => $field['form_type'],
        '#default_value' => $entity->$key ? $entity->$key : '',
        '#upload_location' => 'public://bills',
        '#upload_validators' => array(
          'file_validate_extensions' => array('gif png jpg jpeg'),
          'file_validate_size' => array(2 * 1024 * 1024),
        ),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Implements FORM_ID_validate.
 */
function akm_invoices_form_validate($form, &$form_state) {
  $entity = $form_state['build_info']['args'][0];
  $entity_fields = InvoicesUIController::getInvoicesFields('for_form');
  // Convert date to timestamp
  foreach ($entity_fields as $key => $value) {
    if (InvoicesUIController::isDateField($key)) {
      if ($timestamp = akm_invoices_formatted_date($form_state['values'][$key], 'timestamp')) {
        $form_state['values'][$key] = $timestamp;
      }
      else {
        $error = form_set_error($key);
      }
    }
  }
  if (isset($error)) {
    drupal_set_message(t('Can not convert date to timestamp! Maybe you set wrong date format, please check setting on !settingspage', array('!settingspage' => l(t('settings page'), 'admin/config/people/gce-invoices'))), 'error');
  }
}

/**
 * Implements FORM_ID_submit.
 */
function akm_invoices_form_submit($form, &$form_state) {
  $entity = entity_ui_controller($form_state['entity_type'])->entityFormSubmitBuildEntity($form, $form_state);
  $entity->save();
  $form_state['redirect'] = akm_invoices_PATH;
}
