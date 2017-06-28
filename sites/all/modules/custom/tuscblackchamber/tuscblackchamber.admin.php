<?php

/**
 * adminster site settings
 */
function tuscblackchamber_admin_settings_form() {
  $form = array();

  $form['tuscblackchamber_teaser_length'] = array(
      '#title' => t('Email Teaser Length', array(), array()),
      '#type' => 'select',
      '#options' => array(
          50 => '50',
          75 => '75',
          100 => '100',
          150 => '150',
          200 => '200',
          250 => '250',
          300 => '300',
      ),
      '#required' => TRUE,
      '#description' => t('The length of text that should be included in the newsletter\'s summary', array(), array()),
      '#default_value' => variable_get('tuscblackchamber_teaser_length', ''),
  );
  $form['tuscblackchamber_webmaster_email'] = array(
      '#title' => t('Webmaster Email Address', array(), array()),
      '#type' => 'textfield',
      '#description' => t("Enter the email address of the website administrator.", array(), array()),
      '#required' => TRUE,
      '#default_value' => variable_get('tabcc_webmaster_email', ''),
  );
  return system_settings_form($form);
}// end function
