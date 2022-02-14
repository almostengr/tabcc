<?php
$HOMEPATH = str_replace("public_html", "", $_SERVER['DOCUMENT_ROOT']);
$HOMEPATH .= "phpenv.php";
require($HOMEPATH);
?>
{% extends "base.html" %}
{% block content %}
<h1 id="contact">Contact</h1>
<?php

$customerFirst = $customerLast = $emailAddress = $phoneNumber = $cresponse = "";
$customerFirstError = $customerLastError = $emailAddressError = $phoneNumberError = $cresponseError = "";

function cleanInput($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    return htmlspecialchars($data);
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $customerName = cleanInput($_POST["customerName"]);
    if (strlen($customerName) < 4) {
        $customerNameError .= "Name is required. Please enter your full name. ";
    }
    if (!preg_match("/^[a-zA-Z ]*$/", $customerName)) {
        $customerNameError .= "Only letters and white space allowed. ";
    }

    $phoneNumber = cleanInput($_POST["phoneNumber"]);
    if (empty($phoneNumber)) {
        $phoneNumberError = "Phone number is required. ";
    }
    if (!preg_match("/^[0-9]{3}-[0-9]{3}-[0-9]{4}$/", $phoneNumber) ||  str_contains($phoneNumber, "555-")) {
        $phoneNumberError .= "Invalid phone number. ";
    }

    $emailAddress = cleanInput($_POST["emailAddress"]);
    if (empty($emailAddress)) {
        $emailAddressError = "Email address is required. ";
    }
    if (!filter_var($emailAddress, FILTER_VALIDATE_EMAIL)) {
        $emailAddressError .= "Invalid email address format. ";
    }

    $cresponse = cleanInput($_POST["cresponse"]);
    if (strlen($cresponse) < 100) {
        $cresponseError  = "Comment or Question field must be at least 100 characters long. ";
    }

    $cresponseNoSpaces = str_replace(" ", "", $cresponse);
    if (
        str_contains($cresponseNoSpaces, "http") || str_contains($cresponseNoSpaces, "www") ||
        str_contains($cresponseNoSpaces, ".com/") || str_contains($cresponseNoSpaces, ".org/") ||
        str_contains($cresponseNoSpaces, ".net/") || str_contains($cresponseNoSpaces, ".edu/")
    ) {
        $cresponseError .= "Links are not allowed. ";
    }

    if (
        empty($customerFirstError) && empty($customerLastError) && empty($emailAddressError)
        && empty($phoneNumberError) && empty($cresponseError)
    ) {
        date_default_timezone_set('America/Chicago');
        $new_line = "\r\n";
        $current_time = date("Y-m-d H:i:s");
        unset($_POST['submit']);
        $message = print_r($_POST, true);
        $message .= "Submitted " . $current_time . $new_line;
        $message .= "IP Address " . $_SERVER['REMOTE_ADDR'];
        $subject = "Website Submission at " . $current_time;
        $headers = implode("", array('From' => $_POST['emailaddress']));

        if ($emailAddress == "tester@tuscblackchamber.org") {
            $mail_result = true;
        } else {
            $mail_result = mail($HELPDESK_EMAIL, $subject, $message, $headers);
        }

        if ($mail_result) {
?>
            <div class="bg-success text-light container py-2 my-5" id="successmessage">
                Your request has been submitted successfully! Please allow 3 business days for us
                to respond.
            </div>
        <?php
        } else {
        ?>
            <div class="bg-danger text-light container py-2 my-5" id="failuremessage">
                An error occurred when attempting to process your request.
            </div>
<?php
        }
    }
}
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
    TABCC<br />
    c/o Patricia Cade<br />
    6712 2nd Avenue<br />
    Thomaston, AL 36783
</p>
<h2>Phone Number</h2>
<p><a href="tel:2056148585">205-614-8585</a></p>
<h2>Email</h2>
<p>Please allow up to 3 business days to get back to you.</p>
<form method="POST" action="/contact.php" id="contactform">
    <p>
        <label for="customerfirst" class="required">First Name</label>
        <input class="form-control" id="customerfirst" name="customerfirst" type="text" value="<?php echo $customerFirst; ?>" minlength="3" required="required">
    <div class="bg-danger text-light m-1 px-2" id="customerfirstError"><?php echo $customerfirstError; ?></div>
    </p>
    <p>
        <label for="customerlast" class="required">Last Name</label>
        <input class="form-control" id="customerlast" name="customerlast" type="text" value="<?php echo $customerlast; ?>" minlength="3" required="required">
    <div class="bg-danger text-light m-1 px-2" id="customerlastError"><?php echo $customerlastError; ?></div>
    </p>
    <p>
        <label for="emailAddress" class="required">Email Address</label>
        <input class="form-control" id="emailAddress" name="emailAddress" type="email" value="<?php echo $emailAddress; ?>" minlength="10" required="required">
    <div class="bg-danger text-light m-1 px-2" id="emailAddressError"><?php echo $emailAddressError; ?></div>
    </p>
    <p>
        <label for="phoneNumber" class="required">Phone Number</label>
        <input class="form-control" type="tel" value="<?php echo $phoneNumber; ?>" minlength="10" id="phoneNumber" name="phoneNumber" maxlength="12" required="required" pattern="[0-9]{3}-[0-9]{3}-[0-9]{4}">
    <div class="text-muted font-italic">Enter number in 555-555-5555 format.</div>
    <div class="bg-danger text-light m-1 px-2" id="phoneNumberError"><?php echo $phoneNumberError; ?></div>
    </p>
    <p>
        <label for="cresponse" class="required">Your Comment or Question</label>
        <textarea class="form-control" rows="4" placeholder="How can we help you?" id="cresponse" name="cresponse" minlength="100">
            <?php echo $cresponse; ?>
        </textarea>
    <div class="text-muted font-italic">Minimum 100 characters. The more details, the better</div>
    <div class="bg-danger text-light m-1 px-2" id="cresponseError"><?php echo $cresponseError; ?></div>
    </p>
    <p>
    <div class="g-recaptcha" data-sitekey="6Lfxf4IaAAAAABcuXGqO3MnunwQq7uiqaLAzojgT"></div>
    </p>
    <p>
        <button id="submit" type="submit" class="form-control btn btn-dark-gray">Submit</button>
    </p>
</form>
{% endblock %}