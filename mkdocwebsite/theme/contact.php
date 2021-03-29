<?php
$HOMEPATH = str_replace("public_html", "", $_SERVER['DOCUMENT_ROOT']);
$HOMEPATH .= "phpenv.php";
require($HOMEPATH);
?>
{% extends "base.html" %}
{% block content %}
<h1 id="contact">Contact</h1>
<?php
if (isset($_POST['emailaddress']) && isset($HELPDESK_EMAIL)) {
    date_default_timezone_set('America/Chicago');
    $new_line = "\r\n";
    $current_time = date("Y-m-d H:i:s");
    $message = print_r($_POST, true);
    $message .= "Submitted " . $current_time . $new_line;
    $message .= "IP Address " . $_SERVER['REMOTE_ADDR'];
    $subject = "Website Submission at " . $current_time;
    $headers = implode("", array('From' => $_POST['emailaddress']));

    if ($_POST['emailaddress'] == "tester@tuscblackchamber.org") {
        $mail_result = true;
    } else {
        $mail_result = mail($HELPDESK_EMAIL, $subject, $message, $headers);
    }

    if ($mail_result) {
?>
        <div class="bg-success text-light container py-2 my-5" id="successmessage">
            Your request has been submitted successfully!
        </div>
    <?php
    } else {
    ?>
        <div class="bg-danger text-light container py-2 my-5" id="failuremessage">
            An error occurred when attempting to process your request.
        </div>
    <?php
    }
} else {
    ?>
    <div class="text-center pb-3">
        <img src="/images/headerlogo.jpg" alt="TABCC logo and motto - TABCC, Connecting the Disconnected">
    </div>
    <p>
        TABCC would love to hear from you. If you have any questions or comments about TABCC benefits
        or services, please feel free to contact us.
    </p>
    <h2>Mailing Address</h2>
    <p>
        2247 Fikes Lane<br />
        Tuscaloosa, AL 35401
    </p>
    <h2>Phone Number</h2>
    <p><a href="tel:2056148585">205-614-8585</a></p>
    <h2>Email</h2>
    <p>Please allow up to 72 business hours to get back to you.</p>
    <form method="POST" action="/contact.php" id="contactform">
        <p>
            <label for="customerfirst" class="required">First Name</label>
            <input class="form-control" id="customerfirst" name="customerfirst" type="text" placeholder="First Name" minlength="3" required="required">
        </p>
        <p>
            <label for="customerlast" class="required">Last Name</label>
            <input class="form-control" id="customerlast" name="customerlast" type="text" placeholder="Last Name" minlength="3" required="required">
        </p>
        <p>
            <label for="emailaddress" class="required">Email Address</label>
            <input class="form-control" id="emailaddress" name="emailaddress" type="email" placeholder="Email Address" minlength="10" required="required">
        </p>
        <p>
            <label for="phonenumber" class="required">Phone Number</label>
            <input class="form-control" type="tel" placeholder="Phone Number" minlength="10" id="phonenumber" name="phonenumber" maxlength="12" required="required" pattern="[0-9]{3}-[0-9]{3}-[0-9]{4}">
            <div class="text-muted font-italic">Enter number in 555-555-5555 format.</div>
        </p>
        <p>
            <label for="cresponse" class="required">Your Comment or Question</label>
            <textarea class="form-control" rows="4" placeholder="How can we help you?" id="cresponse" name="cresponse" minlength="100"></textarea>
            <div class="text-muted font-italic">Minimum 100 characters. The more details, the better</div>
        </p>
        <p>
            <div class="g-recaptcha" data-sitekey="6Lfxf4IaAAAAABcuXGqO3MnunwQq7uiqaLAzojgT"></div>
        </p>
        <p>
            <button id="submit" type="submit" class="form-control btn btn-dark-gray">Submit</button>
        </p>
    </form>
<?php
}
?>
{% endblock %}