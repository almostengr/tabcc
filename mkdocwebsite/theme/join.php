<?php
$HOMEPATH = str_replace("public_html", "", $_SERVER['DOCUMENT_ROOT']);
$HOMEPATH .= "phpenv.php";
require($HOMEPATH);
?>

{% extends "base.html" %}

{% block content %}
<h1 id="contact">Online Membership Application</h1>

<?php
$showApplicationForm = false;
$showPaymentForm = false;

if ($_SERVER['REQUEST_METHOD'] == "POST") {
    $mail_result = false;

    date_default_timezone_set('America/Chicago');
    $new_line = "\r\n";
    $message = print_r($_POST, true);
    $message .= "Submitted " . date("Y-m-d H:i:s") . $new_line;
    $message .= "IP Address " . $_SERVER['REMOTE_ADDR'];
    $subject = "TABCC Membership Application Submission";
    $headers = implode("", array('From' => $_POST['emailAddress']));

    if ($_POST['emailAddress'] == "tester@tuscblackchamber.org") {
        $mail_result = true;
    } else {
        $mail_result = mail($HELPDESK_EMAIL, $subject, $message, $headers);
    }

    if ($mail_result) {
        $showPaymentForm = true;
        ?>
        <div class="bg-success text-light container py-2 my-5" id="successmessage">
            Your application has been submitted successfully.
        </div>
        <?php
    } else {
        $showApplicationForm = true;
        ?>
        <div class="bg-danger text-light container py-2 my-5" id="failuremessage">
            An error occurred when attempting to process your request.
        </div>
        <?php
    }
}

if ($showPaymentForm == true) {
    ?>
    <form action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">
        <input type="hidden" name="cmd" value="_s-xclick">
        <input type="hidden" name="hosted_button_id" value="5QAN9KT26KK34">
        <p>
            To complete the membership application process, you will need to make your payment. You can make
            your payment by doing either: <br />
            <ol>
                <li>Send a check or money order to: <br />
                    TABCC<br />c/o Patricia Cade<br />6712 2nd Avenue<br />Thomaston, AL 36783</li>
                <li>PayPal by selecting your membership level and clicking the Buy Now button below</li>
            </ol>
        </p>
        <table>
            <tr>
                <td><input type="hidden" name="on0" value="Membership Type">Membership Type</td>
            </tr>
            <tr>
                <td>
                    <select name="os0">
                        <option value="Individual (Non-Proprietor)">Individual (Non-Proprietor) $100.00 USD</option>
                        <option value="Associate (Nonprofit Organizations, Churches)">Associate (Nonprofit Organizations, Churches) $125.00 USD</option>
                        <option value="Business I (Gross Sales < $250,000)">Business I (Gross Sales < $250,000) $150.00 USD</option>
                        <option value="Business II (Gross Sales > $250,000)">Business II (Gross Sales > $250,000) $250.00 USD</option>
                        <option value="Corporate Partner, Bronze">Corporate Partner, Bronze $500.00 USD</option>
                        <option value="Corporate Partner, Silver">Corporate Partner, Silver $1,000.00 USD</option>
                        <option value="Corporate Partner, Gold">Corporate Partner, Gold $1,500.00 USD</option>
                        <option value="Corporate Partner, Platinum">Corporate Partner, Platinum $2,500.00 USD</option>
                    </select>
                </td>
            </tr>
        </table>
        <input type="hidden" name="currency_code" value="USD">
        <input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_buynowCC_LG.gif" name="submit" alt="Buy Now">
        <img alt="Major credit cards accepted" src="https://www.paypalobjects.com/en_US/i/scr/pixel.gif" width="1" height="1">
    </form>
<?php
}

if ($_SERVER['REQUEST_METHOD'] == "GET" || $showApplicationForm == true) {
?>
    <div>
        <ul>
            <li>Advocacy</li>
            <li>Business Management and Counseling Assistance</li>
            <li>Business Marketing Support</li>
            <li>Educational Seminars and Workshops</li>
            <li>Annual Business</li>
            <li>Opportunity Conference and Expo </li>
            <li>Monthly Members Meetings </li>
            <li>Bi-Monthly Networking Opportunities</li>
            <li>Bid Opportunities Notifications</li>
            <li>Job Announcements</li>
            <li>Referrals</li>
            <li>Connection to Chambers Across State of Alabama</li>
        </ul>
    </div>
    <form method="POST" action="/join.php" id="membershipform">
        <p>
            <label for="membershipType">Membership Type</label>
            <select value="membershipType" required="required" class="form-control">
                <option value="">-Select-</option>
                <option value="Individual (Non-Proprietor)">Individual (Non-Proprietor) $100.00 USD</option>
                <option value="Associate (Nonprofit Organizations, Churches)">Associate (Nonprofit Organizations, Churches) $125.00 USD</option>
                <option value="Business I (Gross Sales < $250,000)">Business I (Gross Sales < $250,000) $150.00 USD</option>
                <option value="Business II (Gross Sales > $250,000)">Business II (Gross Sales > $250,000) $250.00 USD</option>
                <option value="Corporate Partner, Bronze">Corporate Partner, Bronze $500.00 USD</option>
                <option value="Corporate Partner, Silver">Corporate Partner, Silver $1,000.00 USD</option>
                <option value="Corporate Partner, Gold">Corporate Partner, Gold $1,500.00 USD</option>
                <option value="Corporate Partner, Platinum">Corporate Partner, Platinum $2,500.00 USD</option>
            </select>
        </p>
        <fieldset>
            <legend>Personal Information</legend>
            <p>
                <label for="memberFirstName" class="required">First Name</label>
                <input class="form-control" id="memberFirstName" name="memberFirstName" type="text"
                    minlength="3" required="required" value="<?php echo $_POST["memberFirstName"]; ?>">
            </p>
            <p>
                <label for="memberLastName" class="required">Last Name</label>
                <input class="form-control" id="memberLastName" name="memberLastName" type="text"
                    minlength="3" required="required" value="<?php echo $_POST["memberLastName"]; ?>">
            </p>
            <p>
                <label for="memberTitle" class="">Title</label>
                <input class="form-control" id="memberTitle" name="memberTitle" type="text"
                    value="<?php echo $_POST["memberTitle"]; ?>">
            </p>
        </fieldset>
        <fieldset>
            <legend>Business Information</legend>
            <p>
                <label for="businessName">Business Name</label>
                <input class="form-control" id="businessName" name="businessName" type="text"
                    value="<?php echo $_POST["businessName"]; ?>">
            </p>
            <p>
                <label for="businessCategory">Business Category</label>
                <select class="form-control" id="businessCategory" name="businessCategory">
                    <option value="">-Select-</option>
                    <option value="Automotive">Automotive</option>
                    <option value="Business Support and Supplies">Business Support and Supplies</option>
                    <option value="Computers and Electronics">Computers and Electronics</option>
                    <option value="Construction and Contractors">Construction and Contractors</option>
                    <option value="Education">Education</option>
                    <option value="Entertainment">Entertainment</option>
                    <option value="Food and Dining">Food and Dining</option>
                    <option value="Health and Medicine">Health and Medicine</option>
                    <option value="Home and Garden">Home and Garden</option>
                    <option value="Legal and Financial">Legal and Financial</option>
                    <option value="Manufacturing, Wholesale, and Distribution">Manufacturing, Wholesale, and Distribution</option>
                    <option value="Merchants (Retail)">Merchants (Retail)</option>
                    <option value="Miscellaneous">Miscellaneous</option>
                    <option value="Personal Care and Services">Personal Care and Services</option>
                    <option value="Real Estate">Real Estate</option>
                    <option value="Travel and Transportation">Travel and Transportation</option>
                </select>
            </p>
            <p>
                <label for="numberOfEmployees">Number of Employees</label>
                <input type="number" id="numberOfEmployees" name="numberOfEmployees"
                    value="<?php echo $_POST["numberOfEmployees"]; ?>">
            </p>
        </fieldset>
        <fieldset>
            <legend>Contact Information</legend>
            <p>
                <label for="phoneNumber" class="required">Phone Number</label>
                <input class="form-control" type="tel" placeholder="555-555-5555" minlength="10"
                    id="phoneNumber" name="phoneNumber" maxlength="12" required="required"
                    pattern="[0-9]{3}-[0-9]{3}-[0-9]{4}" value="<?php echo $_POST["phoneNumber"]; ?>">
            </p>
            <p>
                <label for="faxNumber" class="">Fax Number</label>
                <input class="form-control" type="tel" placeholder="555-555-5555" id="faxNumber"
                    name="faxNumber" maxlength="12" pattern="[0-9]{3}-[0-9]{3}-[0-9]{4}"
                    value="<?php echo $_POST["faxNumber"]; ?>">
            </p>
            <p>
                <label for="emailAddress" class="required">Email Address</label>
                <input class="form-control" id="emailAddress" name="emailAddress" type="email"
                    minlength="10" value="<?php echo $_POST["emailAddress"]; ?>">
            </p>
            <p>
                <label for="website" class="">Website</label>
                <input class="form-control" type="tel" id="website" name="website"
                    value="<?php echo $_POST["website"]; ?>">
            </p>
        </fieldset>
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