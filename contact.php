<?php
$HOMEPATH = str_replace("public_html", "", $_SERVER['DOCUMENT_ROOT']);
$HOMEPATH .= "phpenv.php";
require($HOMEPATH);
?>
<!doctype html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
  <meta property="og:locale" content="en_US" />
  <meta property="og:site_name" content="Tuscaloosa Area Black Chamber of Commerce" />
  <meta name="twitter:card" content="summary" />

  
  <meta name="description" content="Connecting the disconnected.">
  <meta property="og:description" content="Connecting the disconnected." />
  <meta name="twitter:description" content="Connecting the disconnected." />
  

  
  <meta name="author" content="Tuscaloosa Area Black Chamber of Commerce (TABCC)">
  
  
  <title>Tuscaloosa Area Black Chamber of Commerce</title>
  <meta property="og:title" content="Tuscaloosa Area Black Chamber of Commerce" />
  <meta name="twitter:title" content="Tuscaloosa Area Black Chamber of Commerce" />
  

  

  

  
  <meta property="og:type" content="website" />
  

  
  <meta name="robots" content="index, follow" />
  

  

  

  <script src="https://www.google.com/recaptcha/api.js" async defer></script>

  <link href="//cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet"
    type="text/css">
  <link href="/css/bootstrap.min.css" rel="stylesheet">

  
  <link rel="shortcut icon" href="/images/SmallLogo250x151.png">
  

  <!-- Custom styles for this template -->
  <link href="/css/style.min.css" rel="stylesheet">

  
</head>

<body class="bg-black">
  <nav class="navbar navbar-expand-md navbar-dark fixed-top bg-yellow">
    <a class="navbar-brand" href="/"><img src="/images/SmallLogo250x151.png" style="height: auto; width: 50px;" alt="TABCC Home"></a>
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse"
      aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
      <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarCollapse">
      <ul class="navbar-nav mr-auto">
        
        
        <li class="nav-item">
          <a class="nav-link text-black" href="/about">ABOUT</a>
        </li>
        
        
        
        <li class="nav-item">
          <a class="nav-link text-black" href="/membership">MEMBERSHIP</a>
        </li>
        
        
        
        <li class="nav-item">
          <a class="nav-link text-black" href="/resources">RESOURCES</a>
        </li>
        
        
        
        <li class="nav-item">
          <a class="nav-link text-black" href="https://www.facebook.com/pg/tuscblackcc/events/">EVENTS</a>
        </li>
        
        
        
        <li class="nav-item">
          <a class="nav-link text-black" href="/jobs">JOBS</a>
        </li>
        
        
        
        <li class="nav-item">
          <a class="nav-link text-black" href="/contact.php">CONTACT</a>
        </li>
        
        
      </ul>
      
      
      
      
      
      
      
      
      
      
      
      
      
    </div>
  </nav>

  <main role="main" class="bg-white">

    

    

    <div class="bg-success text-light container py-2 d-none" id="successmessage">
      Your request has been submitted successfully!
    </div>

    <div class="bg-danger text-light container py-2 d-none" id="failuremessage">
      An error occurred when attempting to process your request.
    </div>

    <div class="container py-2" id="pagebody">
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

    </div>
  </main>

  

  
  <section class="bg-dark text-white subfooter">
    <div class="container text-center py-3">
      <div class="row">
        
        <div class="col-sm-12 col-md-4">
          <h4 class="subfooterheader text-yellow">Connect With TABCC!</h4>
          
          <p>
            
            <i class="fab fa-facebook"></i>
            
            
            <a href="https://www.facebook.com/tuscblackcc" class="text-white">
              Facebook</a>
            
          </p>
          
          <p>
            
            <i class="fab fa-twitter"></i>
            
            
            <a href="https://twitter.com/tuscblackcc" class="text-white">
              Twitter</a>
            
          </p>
          
        </div>
        
        <div class="col-sm-12 col-md-4">
          <h4 class="subfooterheader text-yellow">Affiliate Links</h4>
          
          <p>
            
            
            <a href="http://www.alblackcc.org/" class="text-white">
              Alabama State Black<br />Chamber of Commerce</a>
            
          </p>
          
          <p>
            
            
            <a href="https://www.birminghammetrobcc.com/" class="text-white">
              Birmingham Metro Black<br />Chamber of Commerce</a>
            
          </p>
          
          <p>
            
            
            <a href="https://mabcc.org/" class="text-white">
              Mobile Area Black<br />Chamber of Commerce</a>
            
          </p>
          
          <p>
            
            
            <a href="#" class="text-white">
              Northeast Alabama Black<br />Chamber of Commerce</a>
            
          </p>
          
          <p>
            
            
            <a href="http://naaachamber.org" class="text-white">
              North Alabama African American<br />Chamber of Commerce</a>
            
          </p>
          
          <p>
            
            
            <a href="#" class="text-white">
              River Region Black<br />Chamber of Commerce</a>
            
          </p>
          
        </div>
        
        <div class="col-sm-12 col-md-4">
          <h4 class="subfooterheader text-yellow">Other Links</h4>
          
          <p>
            
            <i class="fas fa-id-card-alt"></i>
            
            
            <a href="/contact.php" class="text-white">
              Contact</a>
            
          </p>
          
          <p>
            
            <i class="fas fa-phone"></i>
            
            
            <a href="tel:205-614-8585" class="text-white">
              205-614-8585</a>
            
          </p>
          
          <p>
            
            <i class="fas fa-photo-video"></i>
            
            
            <a href="/photos" class="text-white">
              Photos</a>
            
          </p>
          
          <p>
            
            <i class="fas fa-building"></i>
            
            
            <a href="/jobs" class="text-white">
              Job Opportunities</a>
            
          </p>
          
          <p>
            
            <i class="fas fa-globe-africa"></i>
            
            
            <a href="/" class="text-white">
              tuscblackchamber.org</a>
            
          </p>
          
          <p>
            
            <i class="fas fa-user-shield"></i>
            
            
            <a href="/privacy" class="text-white">
              Privacy Policy</a>
            
          </p>
          
        </div>
        
      </div>
    </div>
  </section>
  

  
  <footer class="bg-black text-yellow">
    <div class="container text-center py-3">
      <div class="col-sm-12">
        &copy; Copyright 2021 Tuscaloosa Area Black Chamber of Commerce (TABCC)
        
        <br />
        Website developed by <a href="https://rhtservices.net" target='_blank'>Robinson Handy and Technology Services
          LLC</a>
      </div>
      <div class="col-sm-12 pt-2">
        Cookies are used on this website to track your visits, provide advertisements specific to you, and preferences
        by a third-party. By continuing to use this site, you agree to the use of cookies unless you have disabled them.
        More information this is available in the Privacy Policy.
      </div>
      <!-- Last updated: 2021-05-02 19:09:12.471029+00:00 -->
    </div>
  </footer>
  

  <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js" async
    integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj"
    crossorigin="anonymous"></script>
  <!-- <script>window.jQuery || document.write('<script src="../assets/js/vendor/jquery.slim.min.js"><\/script>')</script> -->
  <script async src="/js/bootstrap.bundle.min.js"></script>
  <script async src="https://kit.fontawesome.com/a076d05399.js"></script>
  
  <script async src="/js/javascript.min.js"></script>
</body>

</html>