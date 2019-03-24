<?php

/**
 *
 * Update the selected variables in MyChamber to the TABCC website.
 */
function tuscblackchamber_cron_update_with_mychamber_variables() {

  $transaction = db_transaction();

  try{
    $query = "SELECT name, value FROM myc_variable";
    $query .= "WHERE name LIKE 'user_mail%' ";
    $query .= "OR name LIKE 'tabcc%' ";
    $query .= "OR name LIKE 'maintenance_mode%' ";
    $query .= "OR name LIKE 'tuscblackchamber%' ";
    $result = db_query($query);

    foreach($result as $variable) {
      // update values
      $queryWww = "UPDATE variable ";
      $queryWww .= "SET value = :value ";
      $queryWww .= "WHERE name = :name";
      $resultWww = db_query($queryWww, array(':name' => $variable->name, ':value' => $variable->value));
    } // end foreach
  } // end try
  catch(Exception $e) {

    // roll back update and log error if problem occurs
    $transaction->rollback();
    watchdog_exception('tuscblackchamber', $e, $e, array(), WATCHDOG_CRITICAL, NULL);
  } // end catch
} // end function tuscblackchamber_cron_update_with_mychamber_variables()

/**
 *
 * Updates the site variables with the latest contact data entered into CiviCRM.
 */
function tuscblackchamber_refresh_website_contact_info() {
  civicrm_initialize();
    $params = array(
    'version' => 3,
    'sequential' => 1,
    'id' => 1,
  );
  $result = civicrm_api('Contact', 'get', $params);

  variable_set('tabcc_mail_addr_street1', $result['supplemental_address_1']);
  variable_set('tabcc_mail_addr_street2', $result['supplemental_address_2']);
  variable_set('tabcc_mail_addr_city', $result['city']);
  variable_set('tabcc_mail_addr_state', $result['state_province_name']);
  variable_set('tabcc_mail_addr_zip', $result['postal_code']);
  variable_set('tabcc_phy_addr_street1', $result['supplemental_address_1']);
  variable_set('tabcc_phy_addr_street2', $result['supplemental_address_2']);
  variable_set('tabcc_phy_addr_city', $result['city']);
  variable_set('tabcc_phy_addr_state', $result['state_province_name']);
  variable_set('tabcc_phy_addr_zip', $result['postal_code']);
  variable_set('tabcc_phone', $result['phone']);
} // end function tuscblackchamber_refresh_website_contact_info()




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// EVENT LISTING PAGE
////////////////////////////////////////////////////////////////////////////////

/**
 *
 * implements hook_form()
 */
function tuscblackchamber_event_list_form() {
  $myChamberUrl = tuscblackchamber_get_mychamber_url();
  $form = array();
  $rows = array();

  $header = array(
    array('data' => 'Event Title', 'field' => 'title'),
    array('data' => 'Start Time', 'field' => 'start_date', 'sort' => 'asc'),
    array('data' => 'End Time', 'field' => 'end_date'),
    array('data' => 'Event Type', 'field' => 'label'),
  );

  /*
  $query = db_select('civicrm_event', 'evnt');
  $query->join('civicrm_option_value', 'opval', 'opval.value = evnt.event_type_id', array());
  $query->fields('evnt', array('id', 'title', 'summary', 'start_date', 'end_date'));
  $query->fields('opval', array('label'));
  $query->condition('opval.option_group_id', 14, '=');
  $query->condition('evnt.is_active', 1, '=');
  $query->condition('evnt.is_public', 1, '=');
  $query->condition('evnt.start_date', 'now()', '>');
  */

  $dbQuery = "select evnt.id, evnt.title, evnt.summary, evnt.start_date,
  evnt.end_date, opval.label
  from civicrm_event evnt, civicrm_option_value opval
  where opval.value = evnt.event_type_id and opval.option_group_id = 14 and
  evnt.is_active = 1 and evnt.is_public = 1 and
  (evnt.start_date > (now() - interval 1 day) OR
  (evnt.start_date < now() AND evnt.end_date > now())) and
  evnt.is_template = 0 order by start_date";
  $result = db_query($dbQuery, array());

  foreach($result as $row) {
    $rows[] = array(
      l(t($row->title, array(), array()),
        "http://" . $myChamberUrl . "/civicrm/event/info?reset=1&id=" . $row->id,
        array('attributes' => array('target' => '_blank'))) .
        "<br />" . $row->summary,
      format_date(strtotime($row->start_date), 'short', '', NULL, 'en-us'),
      format_date(strtotime($row->end_date), 'short', '', NULL, 'en-us'),
      $row->label,
    );
  } // end foreach($result as $row)

  $form['contracts'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No TABCC events have been planned at this time.', array(), array()),
    '#caption' => '<p>Below are the planned and upcoming events for TABCC and
    its affiliates. Please make plans to attend these events. For more
    information about each event, click the event title.</p>',
  );
  return $form;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// CONTACT FORM
////////////////////////////////////////////////////////////////////////////////

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
    }
  }

  // validate email address
  if( valid_email_address($form_state['values']['tuscblackchamber_contact_email']) == FALSE ){
    form_set_error('tuscblackchamber_contact_email', t('Please enter a valid email address.', array(), array()));
  }

  // validate spam checker
  if( strtoupper($form_state['values']['tuscblackchamber_spam_checker']) != "TUSCALOOSA" ){
    form_set_error('tuscblackchamber_spam_checker', t('Please enter the correct value for the spam checker.', array(), array()));
  }
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


