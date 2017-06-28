<?php

/**
 * Query the database for the monthly events and current members
 * for the monthly email that is sent to the emailing list.  Email
 * is sent on the 3rd day of each month.
 */
function tuscblackchamber_generate_monthly_email($testing = FALSE) {
  global $base_url;
  global $user;
  $base_url_www = str_replace("mychamber.", "", $base_url);
  $emailName = "TABCC eNewsletter-" . date("F Y");
  $htmlEmail = "";
  $lastEmailGenDate = variable_get('tuscblackchamber_monthly_news_last_gen', 0);
  // $hasEmailSent = variable_get('tuscblackchamber_monthly_email_created', 0);
  $mailReturn = FALSE;
  $params = array();
  $transaction = db_transaction();
  $teaserLngth = variable_get('tuscblackchamber_teaser_length', 200);

  // if today is the first Thursday of the month, it is the afternoon, and email hasn't already been sent today  
  if (($lastEmailGenDate < (REQUEST_TIME-86400) && date("j") <= 7 && date("D") == "Thu" && date("A") == "PM") || $testing == TRUE){
    
    // log activity to database
    watchdog('tuscblackchamber', 'Monthly Newsletter: Mailing list script started', array(), WATCHDOG_INFO, NULL);

    // query news events since last newsletter time
    $newsQuery = db_select('node', 'n');
    $newsQuery->join('field_data_body', 'b', 'b.entity_id = n.nid', array());
    $newsQuery->fields('n', array('nid', 'title', 'created'));
    $newsQuery->fields('b', array('body_value'));
    $newsQuery->condition('n.type', 'article', '='); // node type is article
    $newsQuery->condition('n.created', $lastEmailGenDate, '>='); // created after last newsletter run
    $newsQuery->condition('n.status', 1, '='); // status is published
    $newsQuery->condition('b.deleted', 0, '='); // where body is not deleted
    $newsQuery->orderBy('n.created', 'desc');
    $newsQuery->range(0, 20);
    $newsResult = $newsQuery->execute();

    // query special articles since last newsletter time
    $specQuery = db_select('node', 'n');
    $specQuery->join('field_data_body', 'b', 'b.entity_id = n.nid', array());
    $specQuery->fields('n', array('nid', 'title', 'created'));
    $specQuery->fields('b', array('body_value'));
    $specQuery->condition('n.type', 'tabcc_spec_feature', '='); // node type is article
    $specQuery->condition('n.created', $lastEmailGenDate, '>='); // created after last newsletter run
    $specQuery->condition('n.status', 1, '='); // status is published
    $specQuery->condition('b.deleted', 0, '='); // where body is not deleted
    $specQuery->orderBy('n.created', 'desc');
    $specFeatResults = $specQuery->execute();

    // query upcoming events within the next 45 days
    $eventQuery = "select evnt.id, evnt.title, evnt.summary, evnt.start_date,
    evnt.end_date, opval.label, is_online_registration, evnt.description,
    unix_timestamp(registration_end_date) as registration_end_date
    from civicrm_event evnt, civicrm_option_value opval
    where opval.value = evnt.event_type_id and opval.option_group_id = 14 and
    evnt.is_active = 1 and evnt.is_public = 1 and
    (evnt.start_date >= now() and
    evnt.start_date <= now() + interval 45 day) and
    evnt.is_template = 0 order by start_date limit 20";
    $eventResults = db_query($eventQuery); // $eventQuery->execute();

    // job opportunties section since last newsletter time
    $jobQuery = db_select('node', 'n');
    $jobQuery->join('field_data_body', 'b', 'b.entity_id = n.nid', array());
    $jobQuery->fields('n', array('nid', 'title', 'created'));
    $jobQuery->fields('b', array('body_value'));
    $jobQuery->condition('n.type', 'job_listing', '='); // node type is article
    $jobQuery->condition('n.created', $lastEmailGenDate, '>='); // created after last newsletter run
    $jobQuery->condition('n.status', 1, '='); // status is published
    $jobQuery->condition('b.deleted', 0, '='); // where body is not deleted
    $jobQuery->orderBy('n.created', 'desc');
    $jobsResults = $jobQuery->execute();

    // if there is something to include in the newsletter, create an email
    if (($newsResult->rowCount() != 0 || $eventResults->rowCount() != 0 || $jobsResults->rowCount() != 0 || $specFeatResults->rowCount() != 0) || $testing == TRUE) {

      watchdog('tuscblackchamber', 'Monthly Newsletter: There is information to report. Generating email.', array(), WATCHDOG_INFO, NULL);

      // opening section
      $htmlEmail .= "<img src=\"" . $base_url . "/sites/default/files/pictures/tabccLogo.jpg\">";
      $htmlEmail .= "<h1 style=\"text-align: center;\">Tuscaloosa Area Black Chamber of Commerce eNewsletter, " . date("F Y") . "</h1>";
      $htmlEmail .= "<p>The mission of the Tuscaloosa Area Black Chamber of Commerce, ";
      $htmlEmail .= "Inc. is to promote economic development in the Tuscaloosa County, AL ";
      $htmlEmail .= "African-American community and surrounding under-represented communities of ";
      $htmlEmail .= "West Alabama by developing economic empowerment strategies that will assist ";
      $htmlEmail .= "current and future Black or under-represented business owners in becoming ";
      $htmlEmail .= "sustainable.</p>";
      $htmlEmail .= "<p> </p>";
      $htmlEmail .= "<p> </p>";

      // if there is news to report, then add section
      if ($newsResult->rowCount() > 0) {

        // add news header
        $htmlEmail .= "<p>&nbsp;</p>";
        $htmlEmail .= "<h2 style='color: #f00; text-align: center;'>TABCC News</h2>";

        foreach($newsResult as $newsArticle) {
          // build to HTML email
          $htmlEmail .= "<p>";
          $htmlEmail .= "<strong style=\"color: #f00;\">" . $newsArticle->title . "</strong><br />";
          $htmlEmail .= "<strong>Posted:</strong> " . format_date($newsArticle->created, 'medium', '', NULL, NULL) . "<br />";
          $htmlEmail .= substr(strip_tags($newsArticle->body_value), 0, $teaserLngth) . "...<br />";
          $htmlEmail .= "<strong>Read Full Article:</strong> " . l(t($base_url_www . "/node/" . $newsArticle->nid),
            $base_url_www . "/node/" . $newsArticle->nid, array()) . "<br />";
          $htmlEmail .= "</p>";
        } // end foreach($newsQuery as $newsArticle)
      } // end if ($newsResult->rowCount > 0)

      // if there are events to report, the add them to the email
      if($eventResults->rowCount() > 0){

        // add events header
        $htmlEmail .= "<p>&nbsp;</p>";
        $htmlEmail .= "<h2 style='color: #f00; text-align: center;'>Upcoming Events</h2>";
        $htmlEmail .= "<p>Please make an effort to attend these upcoming events hosted " .
        "by TABCC and TABCC Members. If you have questions about a particular event, " .
        "please visit the event page or contact TABCC.</p>";

        foreach($eventResults as $eventDetail) {
          // build to HTML email
          $htmlEmail .= "<p>";
          $htmlEmail .= "<strong style=\"color: #f00;\">" . $eventDetail->title . "</strong><br />";
          $htmlEmail .= "<strong>Start Date</strong>: " . $eventDetail->start_date . "<br />";
          $htmlEmail .= "<strong>Event Type:</strong> " . $eventDetail->label . "<br />";
          $htmlEmail .= "<strong>Registration Required:</strong> ";

          if ($eventDetail->is_online_registration == 0) {
            $htmlEmail .= "No<br />";
          } // end if
          else if ($eventDetail->is_online_registration == 1) {
            $htmlEmail .= "Yes<br />";
            $htmlEmail .= "<strong>Registration Deadline:</strong> ";
            $htmlEmail .= format_date($eventDetail->registration_end_date, 'medium', '', NULL, NULL) . "<br />";
          } // end else if 
          else{
            // should never meet this condition
            throw new Exception("Invalid registration code detected.");
          } // end else

          if ($eventDetail->summary != "") {
            $htmlEmail .= "<strong>Summary:</strong> " . $eventDetail->summary . "...<br />";
          } // end if
          else{
            $htmlEmail .= "<strong>Summary:</strong> ";
            $htmlEmail .= substr(strip_tags($eventDetail->description), 0, $teaserLngth) . "...<br />";
          } // end else

          // $htmlEmail .= "More Info: " . $base_url . "civicrm/event/info?reset=1&id=" . $eventDetail->id . "<br />";
          $htmlEmail .= "<strong>More Info:</strong> " . l(t($base_url . "/civicrm/event/info?reset=1&id=" . $eventDetail->id),
            $base_url . "/civicrm/event/info?reset=1&id=" . $eventDetail->id, array()) . "<br />";
          $htmlEmail .= "</p>";
        } // end foreach
      } // end if

      // if there is special content that has been created, add it to the email
      if($specFeatResults->rowCount() > 0) {

        // add special feature content header
        $htmlEmail .= "<p>&nbsp;</p>";
        $htmlEmail .= "<h2 style='color: #f00; text-align: center;'>Special Featured Content</h2>";

        foreach($specFeatResults as $specDetail) {
          //build to HTML email
          $htmlEmail .= "<p>";
          $htmlEmail .= "<strong style=\"color: #f00;\">" . $specDetail->title . "</strong><br />";
          $htmlEmail .= "<strong>Posted:</strong> " . format_date($specDetail->created, 'medium', '', NULL, NULL) . "<br />";
          $htmlEmail .= substr(strip_tags($specDetail->body_value), 0, $teaserLngth) . "...<br />";
          $htmlEmail .= "<strong>Read Full Article:</strong> " . l(t($base_url_www . "/node/" . $specDetail->nid),
            $base_url_www . "/node/" . $specDetail->nid, array()) . "<br />";
          $htmlEmail .= "</p>";
        } // end foreach
      } // end if

      // add membership recommendation
      $htmlEmail .= "<p>&nbsp;</p>";
      $htmlEmail .= "<h2 style='color: #f00; text-align: center;'>TABCC Membership</h2>";
      $htmlEmail .= "<p>TABCC is currently looking for new members!!! Not currenlty a " .
      "member? Then please visit the Membership section section of our website at " .
      l(t($base_url_www . "/membership"), $base_url_www . "/membership", array()) .
      " for our member benefits and application.</p>";

      // if there are new jobs to report, include them in the email
      if ($jobsResults->rowCount() > 0) {

        // html header for job opportunities
        $htmlEmail .= "<p>&nbsp;</p>";
        $htmlEmail .= "<h2 style='color: #f00; text-align: center;'>Job Opportunities</h2>";
        $htmlEmail .= "<p>Looking for a job or know someone in need of employment? Check out " .
        "our recent job listings posted by TABCC Members.</p>";

        foreach($jobsResults as $jobItem) {
          // build to HTML email
          $htmlEmail .= "<p>";
          $htmlEmail .= "<strong style='color: #f00;'>" . $jobItem->title . "</strong><br />";
          $htmlEmail .= "<strong>Posted:</strong> " . format_date($jobItem->created, 'medium', '', NULL, NULL) . "<br />";
          $htmlEmail .= substr(strip_tags($jobItem->body_value), 0, $teaserLngth) . "...<br />";
          // $htmlEmail .= "<a href='" . $base_url . "node/" . $jobItem->nid . "'>Read Full Listing</a><br />";
          $htmlEmail .= "<strong>See Full Job Posting:</strong> " .
            l(t($base_url_www . "/node/" . $jobItem->nid), $base_url_www . "/node/" . $jobItem->nid, array()) . "<br />";
          $htmlEmail .= "</p>";
        } // end foreach
      } // end if

      // save email as node for public viewing 
      tuscblackchamber_save_email_as_node($emailName, $htmlEmail, $emailName . " has been released. The latest going on with TABCC can be viewed here");

      // add the footer to the email
      $htmlEmail .= tuscblackchamber_get_mailing_list_footer();

      // send email to those in the mailing list
      $params['!htmlBody'] = $htmlEmail;
      
      watchdog('tuscblackchamber', 'Monthly Newsletter: Email has been generated.', array(), WATCHDOG_INFO, NULL);

      // update the time that the email was last sent
      variable_set('tuscblackchamber_monthly_news_last_gen', REQUEST_TIME);
          
      if ($testing == FALSE){
        // send the email to all mailing list subscribers
        $params['!bcc'] = tuscblackchamber_get_mailing_list_receipents();
        // $mailReturn = drupal_mail('tabccmychamber', 'newsletter_monthly', 'undisclosed-recipients:;', 'en', $params, 'noreply@tuscblackchamber.org', TRUE);

        if ($mailReturn == TRUE) {
          watchdog('tuscblackchamber', 'Monthly Newsletter: Email was sent to subscribers.', array(), WATCHDOG_INFO, NULL);
          
          // update the time that the email was last sent
          // variable_set('tuscblackchamber_monthly_news_last_gen', REQUEST_TIME);
        } // end if
        else{
          watchdog('tuscblackchamber', 'Monthly Newsletter: A failure occurred when trying to send the email.', array(), WATCHDOG_INFO, NULL);
        } // end else

        // variable_set('tuscblackchamber_last_newsletter_gen', REQUEST_TIME);
        // variable_set('tuscblackchamber_monthly_email_created', 1);
      } // end if
      else{
        // send email only to user that requested the test
        $params['!bcc'] = $user->mail;
        // drupal_mail('tabccmychamber', 'newsletter', '', 'en', $params, 'noreply@tuscblackchamber.org', TRUE);
        watchdog('tuscblackchamber', 'Monthly Newsletter: Email was sent to tester.', array(), WATCHDOG_INFO, NULL);
        drupal_set_message('Monthly newsletter testing email was sent. Check your email.', 'status', FALSE);
      } // end else

    } // end if
    else{
      watchdog('tuscblackchamber', 'Monthly Newsletter: Nothing to report.', array(), WATCHDOG_INFO, NULL);
    } // end else
  } // end else
} // end function

/**
 *
 * Send weekly email to mailing list if events are occurring within the next 7
 * days. Email is sent on Sunday of each week with the full list of upcoming
 * events.
 */
function tuscblackchamber_generate_weekly_events_email($testing = FALSE) {
  global $base_url;
  global $user;
  $base_url_www = str_replace("mychamber.", "", $base_url);
  // $hasEmailSent = variable_get('tuscblackchamber_last_event_newsletter_gen', 0);
  $lastEmailGenDate = variable_get('tuscblackchamber_weekly_news_last_gen', 0);
  $htmlEmail = "";
  $params = array();
  $teaserLngth = variable_get('tuscblackchamber_teaser_length', 200);

  // if today is Monday, there are events scheduled this week
  if((date("D") == "Mon" && $lastEmailGenDate < (REQUEST_TIME-86400) && date("A") == "PM") || 
  $testing == TRUE) {

    // query upcoming events within the next 8 days
    $eventQuery = "select evnt.id, evnt.title, evnt.summary, evnt.start_date,
    evnt.end_date, opval.label, is_online_registration, evnt.description,
    unix_timestamp(registration_end_date) as registration_end_date
    from civicrm_event evnt, civicrm_option_value opval
    where opval.value = evnt.event_type_id and opval.option_group_id = 14 and
    evnt.is_active = 1 and evnt.is_public = 1 and
    (evnt.start_date >= now() and
    evnt.start_date <= now() + interval 8 day) and
    evnt.is_template = 0 order by start_date";
    $eventResults = db_query($eventQuery); // $eventQuery->execute();

    // if there is something to report, then generate the email
    if ($eventResults->rowCount() != 0 || $testing == TRUE) {

      // $htmlEmail .= "<p>Hello,</p>";
      $htmlEmail .= "<img src=\"" . $base_url . "/sites/default/files/pictures/tabccLogo.jpg\">";
      $htmlEmail .= "<h1 style=\"color: #f00; text-align: center;\">Upcoming TABCC Events Between<br />";
      $htmlEmail .= date("F d, Y", REQUEST_TIME) . " and " . date("F d, Y", (REQUEST_TIME + 604800)) . "</h1>";

      $htmlEmail .= "<p>Please make an effort to attend these upcoming event(s) hosted " .
      "by the Tuscaloosa Area Black Chamber of Commerce (TABCC) and TABCC Members. " .
      "If you have questions about a particular event, " .
      "please click the More Info link to go to the event page or " .
      "contact the Tuscaloosa Area Black Chamber of Commerce.</p>";

      foreach($eventResults as $eventDetail) {
        // build to HTML email
        $htmlEmail .= "<p>";
        $htmlEmail .= "<strong style=\"color: #f00;\">" . $eventDetail->title . "</strong><br />";
        $htmlEmail .= "<strong>Start Date</strong>: " . $eventDetail->start_date . "<br />";
        $htmlEmail .= "<strong>Event Type:</strong> " . $eventDetail->label . "<br />";
        $htmlEmail .= "<strong>Registration Required:</strong> ";
 
        // display if registration is required or not
        if ($eventDetail->is_online_registration == 0) {
          $htmlEmail .= "No<br />";
        } // end if
        else if ($eventDetail->is_online_registration == 1) {
          $htmlEmail .= "Yes<br />";
          $htmlEmail .= "<strong>Registration Deadline:</strong> ";
          $htmlEmail .= format_date($eventDetail->registration_end_date, 'medium', '', NULL, NULL) . "<br />";
        } // end else if 
        else{
          // should never meet this condition
          throw new Exception("Invalid registration code detected.");
        } // end else

        if ($eventDetail->summary != "") {
          $htmlEmail .= "<strong>Summary:</strong> " . $eventDetail->summary . "...<br />";
        } // end if
        else{
          $htmlEmail .= "<strong>Summary:</strong> ";
          $htmlEmail .= substr(strip_tags($eventDetail->description), 0, $teaserLngth) . "...<br />";
        } // end else

        // $htmlEmail .= "More Info: " . $base_url . "civicrm/event/info?reset=1&id=" . $eventDetail->id . "<br />";
        $htmlEmail .= "<strong>More Info:</strong> " . l(t($base_url . "/civicrm/event/info?reset=1&id=" . $eventDetail->id),
          $base_url . "/civicrm/event/info?reset=1&id=" . $eventDetail->id, array()) . "<br />";
        $htmlEmail .= "</p>";
      } // end foreach

      // add mailing list footer to email
      $htmlEmail .= tuscblackchamber_get_mailing_list_footer();
      
      // send email to those in the mailing list
      $params['!htmlBody'] = $htmlEmail;

      if ($testing == FALSE){
        // if not testing, send the email to all on the mailing list
        $params['!bcc'] = tuscblackchamber_get_mailing_list_receipents();
        drupal_mail('tabccmychamber', 'newsletter_events', 'undisclosed-recipients:;', 'en', $params, 'noreply@tuscblackchamber.org', TRUE);
        watchdog('tabccmychamber', 'Event Newsletter: Successfully generated and sent Event Newsletter.', array(), WATCHDOG_INFO, NULL);

        // update the flag that the email was last sent
        variable_set('tuscblackchamber_weekly_news_last_gen', REQUEST_TIME);
        // variable_set('tuscblackchamber_last_event_newsletter_gen', 1);
        // watchdog('tuscblackchamber', 'Event Newsletter: Set generate value to 1.', array(), WATCHDOG_INFO, NULL);
      } // end if
      else{
        // if testing, send the email only to the user that requested the test to be ran
        $params['!bcc'] = $user->mail;
        drupal_mail('tabccmychamber', 'newsletter_events', 'undisclosed-recipients:;', 'en', $params, 'noreply@tuscblackchamber.org', TRUE);
        drupal_set_message('Weekly event newsletter testing email was sent. Check your email.', 'status', FALSE);
        watchdog('tuscblackchamber', 'Event Newsletter: Successfully generated and sent Event Newsletter to tester.', array(), WATCHDOG_INFO, NULL);
      } // end else
    } // end if
  } // end if
  
  /*
  else if (date("D") == "Sat") {
    // reset variable if end of week
    variable_set('tuscblackchamber_last_event_newsletter_gen', 0);
    watchdog('tuscblackchamber', 'Event Newsletter: Set generate value to 0.', array(), WATCHDOG_INFO, NULL);
  } // end else if
  else{
    // do nothing
  } // end else
  */
} // end function


/**
 * footer text of the email newsletters that are sent out.
 */
function tuscblackchamber_get_mailing_list_footer() {
  global $base_url; 
  $outputString = "";
  
  // additional spaces
  $outputString .= "<p>&nbsp;</p>";
  $outputString .= "<p>&nbsp;</p>";

  // add html footer
  $outputString .= "<p style='text-align: left;'>";
  $outputString .= "Tuscaloosa Area Black Chamber of Commerce<br />";
  $outputString .= tuscblackchamber_getPhysicalAddress() . " | 205-614-8585<br />";
  $outputString .= l(t("http://www.tuscblackchamber.org"), "http://www.tuscblackchamber.org", array()) . "<br />";
  $outputString .= "Facebook: " . l(t("http://www.facebook.com/tuscblackcc"), "http://www.facebook.com/tuscblackcc", array()) . "<br />";
  $outputString .= "Twitter: " . l(t("http://twitter.com/tuscblackcc"), "http://twitter.com/tuscblackcc", array()) . "<br />";
  $outputString .= "</p>";

  // add unsubscribe links to email
  $outputString .= "<p>To unsubscribe, please login to your MyChamber account at ";
  $outputString .= l(t($base_url), $base_url, array()) . " and update your email settings. ";
  $outputString .= "To ensure that you continue to receieve emails ";
  $outputString .= "from TABCC in your Inbox, please add noreply@tuscblackchamber.org ";
  $outputString .= "and tuscblackchamber@gmail.com to your email address book ";
  $outputString .= "or safe senders list.</p>";
      
  return $outputString;
} // end function

/**
 * Gets the email receipents that are on the mailing list.
 *
 * @return string List of email addresses that have subscribed to the mailing list
 */
function tuscblackchamber_get_mailing_list_receipents() {
  $emails = "";

  $emailQuery = "SELECT email
  FROM civicrm_group_contact cgc, civicrm_email ce
  WHERE cgc.group_id = 2 AND cgc.status = 'Added' AND
  ce.contact_id = cgc.contact_id AND ce.is_primary = 1";
  $emailList = db_query($emailQuery);

  foreach($emailList as $emailAddr) {
    // add the email address to the string
    $emails .= $emailAddr->email . ', ';
  } // end foreach
  watchdog("tuscblackchamber", "Email sent to: " . $emails, array(), WATCHDOG_DEBUG, NULL);

  // $emails = "tharam04@trlubuntu.att.net, blackboxitech@trlubuntu.att.net";
  return $emails;
} // end function

function tuscblackchamber_queue_mailing_list_email($subject, $body) {
  db_insert('tuscblackchamber_ml_pndng')
    ->fields(array(
      // 'mail_to' => $to,
      'mail_to' => 'tharam04@ramwebservices.att.net',
      'mail_sbjct' => $subject,
      'mail_bdy' => $body,
      'mail_sent' => 0,
    ))
    ->execute();
} // end function

function tuscblackchamber_save_email_as_node($title, $body, $summary = "") {  
  global $user;
  $node = new stdClass();
  
  $node->title = $title;
  $node->type = "article";
      
  // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
  node_object_prepare($node); 
  
  $node->language = LANGUAGE_NONE; // 'en'; // Or e.g. 'en' if locale is enabled
  $node->uid = 1;
  $node->is_new = TRUE;
  $node->sticky = 0;
  $node->status = 1; //(1 or 0): published or not
  $node->promote = 1; //(1 or 0): promoted to front page
  $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
  
  // configure the body field and add the text
  $node->body[$node->language][0]['value'] = $body;
  $node->body[$node->language][0]['summary'] = $summary;
  $node->body[$node->language][0]['format'] = 'full_html';
  
  $path = strtolower(str_replace(" ", "_", $node->title));
  $node->path = array('alias' => $path);
  
  // Term reference (taxonomy) field
  // $node->field_product_tid[$node->language][]['tid'] = $form_state['values']['a taxonomy term id'];
  
  // Entity reference field
  /*
  $node->field_customer_nid[$node->language][] = array(
    'target_id' => $form_state['values']['entity id'],
    'target_type' => 'node',
  );
  */
  // 'node' is default,
  
  // Other possible values are "user" and  "taxonomy_term" 
  $node = node_submit($node); // Prepare node for saving
  node_save($node);
  // drupal_set_message( "Node with nid " . $node->nid . " saved!\n");
  // $form_state['redirect']  = 'SOME WHERE';
} // end function

/**
 * send test email to the user to confirm functionality.
 */
function tuscblackchamber_test_email() {
  global $base_url;

  // send a testing email all mailing functions
  tuscblackchamber_monthly_email(TRUE);
  tuscblackchamber_weekly_events_email(TRUE);

  // redirect to the madule admin page
  drupal_goto('admin/config/system/tuscblackchamber', array(), 302);
} // end function

function tuscblackchamber_send_mailing_list_email() {
  $result = db_select('tuscblackchamber_ml_pndng', 'm')
    ->fields('m', array('mid', 'mail_to', 'mail_sbjct', 'mail_bdy'))
    ->condition('mail_sent', 0, '=')
    ->range(0, 50)
    ->execute();
    
  foreach($result as $email) {
    $params = array();
    $params['!mail_sbjct'] = $email->mail_sbjct;
    $params['!mail_bdy'] = $email->mail_bdy;
    
    // send the email 
    drupal_mail('tabccmychamber', 'mail_list_msg', $email->mail_to, 'en', $params, NULL, TRUE);
    
    // update the status of the mail row
    db_update('tuscblackchamber_ml_pndng')
      ->fields(array(
        'mail_sent' => 1,
        // 'mail_snt_dt' => REQUEST_TIME,
      ))
      ->condition('mid', $email->mid, '=')
      ->execute();
  } // end foreach
} // end function

