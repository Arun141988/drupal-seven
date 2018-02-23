<?php
/**
 * @file
 * Administer settings
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Modify Account settings.
 */
function akm_users_form_user_admin_settings_alter(&$form, &$form_state) {

  $form['email_approve_payments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment approved'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Enable and edit e-mail messages sent to users when their payment are approved. The list of available tokens that can be used in e-mails is provided below.'),
    '#group' => 'email',
  );
  $form['email_approve_payments']['akm_users_approve_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify user when account is approved.'),
    '#default_value' => variable_get('akm_users_approve_notification', FALSE),
  );
  $form['email_approve_payments']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="akm_users_approve_notification"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['email_approve_payments']['settings']['akm_users_approve_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _akm_users_notification_text('akm_users_approve_notification_subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['email_approve_payments']['settings']['akm_users_approve_notification_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _akm_users_notification_text('akm_users_approve_notification_body', NULL, array(), FALSE),
    '#rows' => 5,
  );

  $form['email_reject_payments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment rejected'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Enable and edit e-mail messages sent to users when their payment are rejected. The list of available tokens that can be used in e-mails is provided below.'),
    '#group' => 'email',
  );
  $form['email_reject_payments']['akm_users_reject_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify user when account is approved.'),
    '#default_value' => variable_get('akm_users_reject_notification', FALSE),
  );
  $form['email_reject_payments']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="akm_users_reject_notification"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['email_reject_payments']['settings']['akm_users_reject_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _akm_users_notification_text('akm_users_reject_notification_subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['email_reject_payments']['settings']['akm_users_reject_notification_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _akm_users_notification_text('akm_users_reject_notification_body', NULL, array(), FALSE),
    '#rows' => 5,
  );

  $form['email_update_bill'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add new bill'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Enable and edit e-mail messages sent to admin when user update bill. The list of available tokens that can be used in e-mails is provided below.'),
    '#group' => 'email',
  );
  $form['email_update_bill']['akm_users_update_bill_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify admin when user update bill.'),
    '#default_value' => variable_get('akm_users_update_bill_notification', FALSE),
  );
  $form['email_update_bill']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="akm_users_update_bill_notification"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['email_update_bill']['settings']['akm_users_update_bill_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _akm_users_notification_text('akm_users_update_bill_notification_subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['email_update_bill']['settings']['akm_users_update_bill_notification_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _akm_users_notification_text('akm_users_update_bill_notification_body', NULL, array(), FALSE),
    '#rows' => 5,
  );

  $form['email_account_unpublish'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unpublish account'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Enable and edit e-mail messages sent to user when account unpublish. The list of available tokens that can be used in e-mails is provided below.'),
    '#group' => 'email',
  );
  $form['email_account_unpublish']['akm_users_unpublish_account_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify user when account unpublish.'),
    '#default_value' => variable_get('akm_users_unpublish_account_notification', FALSE),
  );
  $form['email_account_unpublish']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="akm_users_unpublish_account_notification"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['email_account_unpublish']['settings']['akm_users_unpublish_account_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _akm_users_notification_text('akm_users_unpublish_account_notification_subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['email_account_unpublish']['settings']['akm_users_unpublish_account_notification_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _akm_users_notification_text('akm_users_unpublish_account_notification_body', NULL, array(), FALSE),
    '#rows' => 5,
  );

  $form['email_new_user_registred'] = array(
    '#type' => 'fieldset',
    '#title' => t('Have new user'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Enable and edit e-mail messages sent to admin when new user registered. The list of available tokens that can be used in e-mails is provided below.'),
    '#group' => 'email',
  );
  $form['email_new_user_registred']['akm_users_new_user_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify admin when new user registered.'),
    '#default_value' => variable_get('akm_users_new_user_notification', FALSE),
  );
  $form['email_new_user_registred']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="akm_users_new_user_notification"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['email_new_user_registred']['settings']['akm_users_new_user_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _akm_users_notification_text('akm_users_new_user_notification_subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['email_new_user_registred']['settings']['akm_users_new_user_notification_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _akm_users_notification_text('akm_users_new_user_notification_body', NULL, array(), FALSE),
    '#rows' => 5,
  );
}

function _akm_users_notification_text($key, $language = NULL, $variables = array(), $replace = TRUE) {
  $langcode = isset($language) ? $language->language : NULL;

  if ($admin_setting = variable_get($key, FALSE)) {
    // An admin setting overrides the default string.
    $text = $admin_setting;
  }
  else {
    // To do default text to mails.
    switch ($key) {
      case 'akm_users_approve_notification_subject':
        $text = t('Account details for [user:name] at [site:name] (payment accepted)', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_approve_notification_body':
        $text = t('[user:name],

Your payment on [site:name] has been accepted.

--  [site:name] team', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_reject_notification_subject':
        $text = t('Account details for [user:name] at [site:name] (payment reject)', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_reject_notification_body':
        $text = t('[user:name],

Your payment on [site:name] has been reject.

--  [site:name] team', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_update_bill_notification_subject':
        $text = t('Add new bill at [site:name]', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_update_bill_notification_body':
        $text = t('User [user:name] update a bill.

Your should approve or reject payment on <a href="[site:url]admin/content/invoices">[site:url]admin/content/invoices</a>.

--  [site:name] team', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_unpublish_account_notification_subject':
        $text = t('Account details for [user:name] at [site:name] (account unpublish)', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_unpublish_account_notification_body':
        $text = t('[user:name],

Your advertising was unpublish on [site:name].

--  [site:name] team', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_new_user_notification_subject':
        $text = t('User with name [user:name] registered at [site:name]', array(), array('langcode' => $langcode));
        break;

      case 'akm_users_new_user_notification_body':
        $text = t('User with [user:name] registred at [site:name].

You can view user profile follow by [user:url].

--  [site:name] team', array(), array('langcode' => $langcode));
        break;

      default:
        // No override, return default string.
        $text = '';
        break;
    }
  }
  if ($replace) {
    // We do not sanitize the token replacement, since the output of this
    // replacement is intended for an e-mail message, not a web browser.
    return token_replace($text, $variables, array(
      'language' => $language,
      'callback' => 'user_mail_tokens',
      'sanitize' => FALSE,
      'clear' => TRUE,
    ));
  }

  return $text;
}
