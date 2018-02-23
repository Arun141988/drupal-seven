<?php

/**
 * @file
 * Provides administrative gse popups functionality.
 */

/**
 * Administrative settings.
 */
function akm_popup_settings() {

  $form['age_gate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Age gate popup settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['age_gate']['akm_popup_show_popup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show popup'),
    '#default_value' => variable_get('akm_popup_show_popup', FALSE),
  );
  $form['age_gate']['akm_popup_time_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Time offset in seconds before show the popup'),
    '#default_value' => variable_get('akm_popup_time_offset', ''),
    '#maxlength' => 3,
    '#size' => 3,
  );
  $form['age_gate']['akm_popup_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of popup'),
    '#default_value' => variable_get('akm_popup_title', ''),
    '#maxlength' => 60,
  );
  $akm_popup_age_gate_message = variable_get('akm_popup_message', array('value' => '', 'format' => 'filtered_html '));
  $form['age_gate']['akm_popup_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Age gate message'),
    '#default_value' => $akm_popup_age_gate_message['value'],
    '#rows' => 8,
  );
  $form['age_gate']['akm_popup_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Agree button text'),
    '#default_value' => variable_get('akm_popup_button_text', 'I Agree'),
    '#maxlength' => 60,
  );

  $form['geolocation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Geolocation settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['geolocation']['akm_popup_ask_geolocation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use geolocation'),
    '#default_value' => variable_get('akm_popup_ask_geolocation', FALSE),
  );

  $options = array();
  // Get locations from vocabulary!
  if ($locations = taxonomy_get_tree(2)) {
    foreach ($locations as $location) {
      $options[$location->tid] = $location->name;
    }
  }
  $form['geolocation']['akm_popup_top_locations'] = array(
    '#type' => 'select',
    '#title' => 'Top locations',
    '#options' => $options,
    '#description' => t('Select a top locations where user will be redirected'),
    '#multiple' => TRUE,
    '#default_value' => variable_get('akm_popup_top_locations', FALSE),
  );

  $form['video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video page popup settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['video']['akm_popup_stop_videos'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use stop video popup'),
    '#default_value' => variable_get('akm_popup_stop_videos', FALSE),
  );
  $form['video']['akm_popup_video_time_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Time offset in seconds before stop a video'),
    '#default_value' => variable_get('akm_popup_video_time_offset', ''),
    '#maxlength' => 3,
    '#size' => 3,
  );
  $form['video']['akm_popup_video_popup_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of popup'),
    '#default_value' => variable_get('akm_popup_video_popup_title', ''),
    '#maxlength' => 60,
  );
  $akm_popup_video_popup_message = variable_get('akm_popup_video_popup_message', array(
    'value' => '',
    'format' => 'filtered_html '
  ));
  $form['video']['akm_popup_video_popup_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Age gate message'),
    '#default_value' => $akm_popup_video_popup_message['value'],
    '#rows' => 8,
  );
  $form['video']['akm_popup_video_popup_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#default_value' => variable_get('akm_popup_video_popup_button_text', ''),
    '#maxlength' => 60,
  );
  $form['video']['akm_popup_video_popup_button_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Button link'),
    '#default_value' => variable_get('akm_popup_video_popup_button_link', ''),
    '#maxlength' => 60,
  );

  return system_settings_form($form);
}
