<?php


function tuscblackchamber_contact_site_form_submit($form, &$form_state) {
  
} // end function

/**
 *
 * implemments hook_form()
 */
function tuscblackchamber_contact_form() {
  $form = array();
  $address = tuscblackchamber_getPhysicalAddress();
  $mapsAddress = str_replace(' ', '+', $address);
  $mapsUrl = "http://maps.google.com/maps?q=" . $mapsAddress . "&oe=utf-8&hnear=" . $mapsAddress . "&gl=us&t=m&z=14";

  $form['#method'] = 'post';
  $form['#action'] = '?';
  $introParagraph = "TABCC would love to hear from you. If you have any questions or " .
      "comments about TABCC benefits or services, please feel free to contact us.<br /><br />" .
      "<strong>Physical Address</strong><br />" . $address . "<br />" .
      l(t("View on Google Maps"), $mapsUrl, array('attributes' => array('target' => '_blank'))) . "<br /><br />" .
      "<strong>Mailing Address</strong><br />" . tuscblackchamber_getMailingAddress() . "<br /><br />" .
      "<strong>Phone Number</strong><br />" . variable_get('tabcc_phone', '205-614-8585');
  $form['tuscblackchamber_contact_intro'] = array(
      '#prefix' => '<p>',
      '#markup' => t($introParagraph, array(), array()),
      '#suffix' => '</p>',
  );
  $form['tuscblackchamber_contact_name'] = array(
      '#title' => t('Contact Name', array(), array()),
      '#type' => 'textfield',
      '#maxlength' => 50,
      '#required' => TRUE,
  );
  $form['tuscblackchamber_contact_email'] = array(
      '#title' => t('Email Address', array(), array()),
      '#type' => 'textfield',
      '#maxlength' => 50,
      '#required' => TRUE,
  );
  $form['tuscblackchamber_contact_phone'] = array(
      '#title' => t('Phone Number', array(), array()),
      '#type' => 'textfield',
      '#maxlength' => 15,
      '#size' => 15,
      '#required' => FALSE,
      '#description' => t('Please enter phone number in XXX-XXX-XXXX format.', array(), array()),
  );
  $form['tuscblackchamber_contact_message'] = array(
      '#title' => t('Mesage, comment, or question', array(), array()),
      '#type' => 'textarea',
      '#required' => TRUE,
  );
  $form['tuscblackchamber_contact_visitor_ip'] = array(
      '#type' => 'hidden',
      '#value' => t($_SERVER['REMOTE_ADDR'], array(), array()),
  );
  $form['tuscblackchamber_spam_checker'] = array(
      '#title' => t('Spam Checker', array(), array()),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#maxlength' => 10,
      '#size' => 10,
      '#prefix' => t('The "T" in "TABCC" stands for what?', array(), array()),
  );
  $form['tuscblackchamber_contact_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Send Email', array(), array()),
  );
  return $form;
} // end function tuscblackchamber_contact_form()

/**
 *
 * validates the contact form.
 */
function tuscblackchamber_contact_form_validate($form, &$form_state) {
  // validate phone number
  if( $form_state['values']['tuscblackchamber_contact_phone'] != "" ){
    $phoneNumber = trim($form_state['values']['tuscblackchamber_contact_phone']);
    $strippedPhoneNum = preg_replace('/(\W*)/', '', $phoneNumber); //remove all non numeric characters from string
    if( strlen($strippedPhoneNum) != 10 ){
      form_set_error('tuscblackchamber_contact_phone', t('Please enter a valid 10-digit phone number in XXX-XXX-XXXX format.', array(), array()));
    } // end if
  } // end if

  // validate email address
  if( valid_email_address($form_state['values']['tuscblackchamber_contact_email']) == FALSE ){
    form_set_error('tuscblackchamber_contact_email', t('Please enter a valid email address.', array(), array()));
  } // end if

  // validate spam checker
  if( strtoupper($form_state['values']['tuscblackchamber_spam_checker']) != "TUSCALOOSA" ){
    form_set_error('tuscblackchamber_spam_checker', t('Please enter the correct value for the spam checker.', array(), array()));
  } // end if
} // end function tuscblackchamber_contact_form_validate($form, &$form_state)

/**
 *
 * submits the contact form
 * @form
 * @form_state  vaiues from the form are in this variable in an array format.
 */
function tuscblackchamber_contact_form_submit($form, &$form_state) {
  $params['!name'] = $form_state['values']['tuscblackchamber_contact_name'];
  $params['!email'] = $form_state['values']['tuscblackchamber_contact_email'];
  $params['!phone'] = $form_state['values']['tuscblackchamber_contact_phone'];
  $params['!messageComment'] = $form_state['values']['tuscblackchamber_contact_message'];
  $params['!ipAddress'] = $form_state['values']['tuscblackchamber_contact_visitor_ip'];

  // send to applicant
  $emailSent = drupal_mail('tuscblackchamber', 'contact_tabcc', 'tuscblackchamber@gmail.com', 'en', $params, $params['!email'], TRUE);

  if( $emailSent == TRUE ){
    // show success message
    drupal_set_message("Your message was sent successfully!", 'status', TRUE);
  } // end if
  else{
    // show error message
    drupal_set_message("An error occurred when trying to send your message. Please try again.", 'error', TRUE);
  } // end else
} // end function tuscblackchamber_contact_form_submit($form, &$form_state)
