<?php
/**
 * @file
 * Provides administrative gse invoices functionality.
 */

/**
 * Administrative settings.
 */
function akm_invoices_settings() {
  $form['akm_invoices_feature_invoice_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Premium invoice price ( apply only for Escorts)'),
    '#default_value' => variable_get('akm_invoices_feature_invoice_price', ''),
    '#maxlength' => 60,
  );
  $roles = akm_invoices_get_clear_roles();
  foreach ($roles as $rid => $role_name) {
    $slug = 'akm_invoices_rid_' . $rid . '_price';
    $form[$slug] = array(
      '#type' => 'textfield',
      '#title' => t('@role_name invoice price', array('@role_name' => $role_name)),
      '#default_value' => variable_get($slug, ''),
      '#maxlength' => 60,
    );
  }
  $form['akm_invoices_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date format for invoices'),
    '#default_value' => variable_get('akm_invoices_date_format', 'd/m/Y'),
    '#maxlength' => 60,
    '#description' => t('A PHP date format string suitable for input to !dateinfo.', array('!dateinfo' => l(t('date()'), 'http://php.net/date'))),
  );
  $form['akm_invoices_day_before_remind_letter'] = array(
    '#type' => 'textfield',
    '#title' => t('Remind to user X days before to invoice expiry date'),
    '#default_value' => variable_get('akm_invoices_day_before_remind_letter', 5),
    '#maxlength' => 60,
  );
  $form['akm_invoices_day_after_remind_letter'] = array(
    '#type' => 'textfield',
    '#title' => t('Remind to user X days after invoice due date expired'),
    '#default_value' => variable_get('akm_invoices_day_after_remind_letter', 5),
    '#maxlength' => 60,
  );
  $form['akm_invoices_block_user_after_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Block user X days after invoice due date expired'),
    '#default_value' => variable_get('akm_invoices_block_user_after_days', 21),
    '#maxlength' => 60,
    '#description' => t('Set empty for not block users.'),
  );

  return system_settings_form($form);
}