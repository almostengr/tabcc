<?php

function tuscblackchamber_submit_event_form() {
  $form = array();
  global $user;
  $eventTypeOptions = array();
  
  $eventTypes = db_select('civicrm_option_value', 'o')
    ->fields('o', array('value', 'name'))
    ->condition('is_active', 1, '=')
    ->condition('option_group_id', 14, '=')
    ->orderBy('name', 'asc')
    ->execute();
  foreach($eventTypes as $type) {
    $eventTypeOptions[$type->value] = $type->name;
  } // end foreach

  $form['contact'] = array(
    '#title' => t('Contact Information', array(), array()),
    '#type' => 'fieldset',
    '#description' => t('Information entered here will be displayed as contact information for more information about the event.', array(), array()),
  );
  $form['contact']['full_name'] = array(
    '#title' => t('Full Name', array(), array()),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#description' => t('Enter the full name of the person to contact about questions for this event.', array(), array()),
  );
  $form['contact']['phone_number'] = array(
    '#title' => t('Phone Number', array(), array()),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#maxlength' => 15,
  );
  $form['contact']['email_address'] = array(
    '#title' => t('Email Address', array(), array()),
    '#default_value' => isset($user->mail) ? $user->mail : '',
    '#type' => 'textfield',
    '#maxlength' => 50,
    '#required' => TRUE,
  );
  $form['event'] = array(
    '#type' => 'fieldset',
    '#title' => t('Event Details', array(), array()),
    '#description' => t('Enter all the requested information about your event. Submissions that are not completed will not be posted on TABCC\'s website.', array(), array()),
  );
  $form['event']['title'] = array(
    '#title' => t('Event Title', array(), array()),
    '#required' => TRUE,
    '#maxlength' => 100,
    '#description' => t('Enter the title of the event.', array(), array()),
  );
  $form['event']['description'] = array(
    '#title' => t('Event Description', array(), array()),
    '#required' => TRUE,
    '#maxlength' => 500,
    '#description' => t('Enter a full and detailed description about the event.', array(), array()),
    '#type' => 'textarea',
  );
  $form['event']['event_type'] = array(
    '#title' => t('Type', array(), array()),
    '#required' => TRUE,
    '#type' => 'select',
    '#options' => $eventTypeOptions,
    '#description' => t('Select the type of event this is.', array(), array()),
  );
  $form['event']['start_date'] = array(
    '#title' => t('Start Date', array(), array()),
    '#required' => TRUE,
    '#type' => 'date_select',
    '#default_value' => date("Y-m-d H:i", REQUEST_TIME),
    '#date_format' => "Y-m-d H:i",
    '#date_timezone' => 'America/Chicago',
    '#date_increment' => 15,
    '#date_year_range' => '0:+3',
    '#description' => t('Enter the date and time the event will start.', array(), array()),
  );
  $form['event']['end_date'] = array(
    '#title' => t('End Date', array(), array()),
    '#required' => FALSE,
    '#type' => 'date_select',
    '#default_value' => date("Y-m-d H:i", REQUEST_TIME),
    '#date_format' => "Y-m-d H:i",
    '#date_timezone' => 'America/Chicago',
    '#date_increment' => 15,
    '#date_year_range' => '0:+3',
    '#description' => t('Enter the date and time the event will start.', array(), array()),
  );
  $form['event']['registration_required'] = array(
    '#title' => t('Registation Required', array(), array()),
    '#required' => TRUE,
    '#type' => 'select',
    '#options' => array(
      1 => 'Yes',
      0 => 'No',
    ),
    '#description' => t('Is registration required for this event? If so you will also need to enter the registration cut off date.', array(), array()),
  );
  $form['event']['registration_cut_off'] = array(
    '#title' => t('Registration Cut Off', array(), array()),
    '#required' => FALSE,
    '#type' => 'date',
    '#description' => t('If selected yes to the above question, please enter the date that all registrations should be submitted.', array(), array()),
  );
  $form['event']['cost'] = array(
    '#type' => 'textfield',
    '#title' => t('Cost', array(), array()),
    '#required' => TRUE,
    '#field_prefix' => '$',
    '#size' => 15,
    '#description' => t('Enter the cost per person to attend the event. If no cost, enter "0.00". If multiple prices, enter the minimum to attend.', array(), array()),
    '#maxlength' => 10,
  );
  $form['event']['location'] = array(
    '#type' => 'textarea',
    '#title' => t('Event Location', array(), array()),
    '#required' => TRUE,
    '#description' => t('Enter the location of where the event will take place. Be sure to specify the room if applicable.', array(), array()),
    '#maxlength' => 20,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Event', array(), array()),
  );
  
  return $form;
} // end function

function tuscblackchamber_submit_event_form_validate($form, &$form_state) {
  $start_date = mktime(
    $form_state['values']['start_date']['hour'], 
    $form_state['values']['start_date']['minute'], 
    59, // second
    $form_state['values']['start_date']['month'], 
    $form_state['values']['start_date']['day'], 
    $form_state['values']['start_date']['year']
  );
  
  $end_date = mktime(
    $form_state['values']['end_date']['hour'], 
    $form_state['values']['end_date']['minute'], 
    59, // second
    $form_state['values']['end_date']['month'], 
    $form_state['values']['end_date']['day'], 
    $form_state['values']['end_date']['year']
  );
  
  $reg_end_date = mktime(23, 59, 59,
    $form_state['values']['registration_cut_off']['month'], 
    $form_state['values']['registration_cut_off']['day'], 
    $form_state['values']['registration_cut_off']['year']
  );
  
  if( $form_state['values']['registration_required'] == 1 ){
    // validate date if registration is required
    
    if( $reg_end_date <= $start_date || $reg_end_date < $end_date ) {
      form_set_error('registration_cut_off', "Registration End Date must be before Event Start and End Dates.");
    } // end if
    
    // drupal_set_message(print_r($form_state['values']['registration_cut_off']), 'warning');
    // drupal_set_message(($form_state['values']['start_date']['month']), 'warning');
    // drupal_set_message(($form_state['values']['start_date']['day']), 'warning');
    // drupal_set_message(($form_state['values']['start_date']['year']), 'warning');
  } // end if
  
} // end function

function tuscblackchamber_submit_event_form_submit($form, &$form_state) {
  // initialize civicrm
  civicrm_initialize();
  
  $start_date = mktime(
    $form_state['values']['start_date']['hour'], 
    $form_state['values']['start_date']['minute'], 
    59, // second
    $form_state['values']['start_date']['month'], 
    $form_state['values']['start_date']['day'], 
    $form_state['values']['start_date']['year']
  );
  
  $end_date = mktime(
    $form_state['values']['end_date']['hour'], 
    $form_state['values']['end_date']['minute'], 
    59, // second
    $form_state['values']['end_date']['month'], 
    $form_state['values']['end_date']['day'], 
    $form_state['values']['end_date']['year']
  );
  
  $reg_end_date = mktime(23, 59, 59,
    $form_state['values']['registration_cut_off']['month'], 
    $form_state['values']['registration_cut_off']['day'], 
    $form_state['values']['registration_cut_off']['year']
  );
  
  // save to database 
  $params = array(
    // 'id' => $form_state['values'][''],
		'title' => $form_state['values']['title'],
		'summary' => NULL,
		'description' => $form_state['values']['description'],
		'event_type_id' => $form_state['values'][''],
		'participant_listing_id' => NULL,
    'is_public' => 1,
		'start_date' => $start_date,
		'end_date' => $start_date,
		'is_online_registration' => 0,
		'registration_link_text' => "Register Now",
		'registration_start_date' => date("Y-m-d h:m:d", $start_date), // $form_state['values'][''],
		'registration_end_date' => date("Y-m-d h:m:d", $end_date),
		'max_participants' => $form_state['values'][''],
		'event_full_text' => "This event is currently full.",
		'is_monetary' => $form_state['values'][''],
		'financial_type_id' => $form_state['values'][''],
		'payment_processor' => NULL,
		'is_map' => 1,
		'is_active' => 0, // by default, do not publically post events
		'fee_label' => "Event Fee(s)",
		'is_show_location' => 1,
		'loc_block_id' => $form_state['values'][''],
		'default_role_id' => 1,
		'intro_text' => $form_state['values'][''],
		'footer_text' => $form_state['values'][''],
		'confirm_title' => "Confirm Registration Information",
		'confirm_text' => "<p>Please confirm that your registration information is correct.&nbsp; If it is not, please go back and correct it.</p>",
		'confirm_footer_text' => NULL,
		'is_email_confirm' => 1,
		'confirm_email_text' => $form_state['values'][''],
		'confirm_from_name' => "TABCC",
		'confirm_from_email' => "noreply@tuscblackchamber.org",
		'cc_confirm' => "",
		'bcc_confirm' => "krobinson@tuscblackchamber.org",
		'default_fee_id' => NULL,
		'default_discount_fee_id' => NULL,
		'thankyou_title' => "Thank you for registering",
		'thankyou_text' => $form_state['values'][''],
		'thankyou_footer_text' => NULL,
		'is_pay_later' => 0,
		'pay_later_text' => NULL,
		'pay_later_receipt' => NULL,
		'is_partial_payment' => 0,
		'initial_amount_label' => NULL,
		'initial_amount_help_text' => NULL,
		'min_initial_amount' => NULL,
		'is_multiple_registrations' => NULL,
		'allow_same_participant_emails' => 0,
		'has_waitlist' => 0,
		'requires_approval' => 0,
		'expiration_time' => NULL,
		'waitlist_text' => NULL,
		'approval_req_text' => NULL,
		'is_template' => 0,
		'template_title' => NULL,
		'created_id' => $user->uid,
		'created_date' => format_date(REQUEST_TIME, 'short', '', NULL, NULL),
		'currency' => 'USD',
		'campaign_id' => NULL,
		'is_share' => 1,
		'parent_event_id' => NULL,
		'slot_label_id' => NULL,
  );
  $result = civicrm_api('Event', 'create', $params);
  
  if (TRUE) {
    // display success message
    drupal_set_message('Your event has been saved. If approved, your event will be listed on TABCC\'s website.', 'status', FALSE);
    
    // cc copy of email to user that submitted request
    drupal_mail('tuscblackchamber', 'event_submit', 'krobinson@tuscblackchamber.org', 'en', array(), NULL, TRUE);
  } // end if
} // end function
