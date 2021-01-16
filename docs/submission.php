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
    <title>Tuscaloosa Area Black Chamber of Commerce | Tuscaloosa, AL | Tuscaloosa Area Black Chamber of Commerce</title>
    <meta property="og:title" content="Tuscaloosa Area Black Chamber of Commerce | Tuscaloosa, AL" />
    <meta name="twitter:title" content="Tuscaloosa Area Black Chamber of Commerce | Tuscaloosa, AL" />
    <meta property="og:type" content="website" />
    <meta name="robots" content="index, follow" />
    <link rel="canonical" href="http://www.tuscblackchamber.org/">
    <meta property="og:url" content="http://www.tuscblackchamber.org/" />
    <meta property="twitter:url" content="http://www.tuscblackchamber.org/" />
    <!-- Global site tag (gtag.js) - Google Analytics -->
    <script async src="https://www.googletagmanager.com/gtag/js?id=G-1TEYPEZ2YY"></script>
    <script async>
        window.dataLayer = window.dataLayer || [];
        function gtag() {
            dataLayer.push(arguments);
        }
        gtag('js', new Date());
        gtag('config', 'G-1TEYPEZ2YY');
    </script>
    <link href="//cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="/css/bootstrap.min.css" rel="stylesheet">
    <link rel="shortcut icon" href="/images/SmallLogo250x151.png">
    <!-- Custom styles for this template -->
    <link href="/css/style.min.css" rel="stylesheet">
</head>
<body class="bg-black">
    <nav class="navbar navbar-expand-md navbar-dark fixed-top bg-yellow">
        <a class="navbar-brand" href="/"><img src="/images/SmallLogo250x151.png" style="height: auto; width: 50px;" alt="TABCC Home"></a>
        <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse" aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
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
                    <a class="nav-link text-black" href="/contact">CONTACTS</a>
                </li>
            </ul>
        </div>
    </nav>
    <main role="main" class="bg-white">
        <?php
        $env_vars = "../../phpenv.rs.php";
        require_once($env_vars);
        if (isset($_POST['emailer'])) {
            date_default_timezone_set('America/Chicago');
            unset($_POST['captcha']);
            $new_line = "\r\n";
            $current_time = date("Y-m-d H:i:s");
            $message = print_r($_POST, true);
            $message .= "Submitted " . $current_time . $new_line;
            $message .= "IP Address " . $_SERVER['REMOTE_ADDR'];
            $subject = $_POST['servicetype'] . " " . $current_time;
            $headers = array('From' => $_POST['emailer']);
            $mail_result = mail($HELPDESK_EMAIL, $subject, $message, $headers);
            if ($mail_result) {
        ?>
                <div class="bg-success text-light container py-2 d-none" id="successmessage">
                    Your request has been submitted successfully!
                </div>
            <?php
            } else {
            ?>
                <div class="bg-danger text-light container py-2 d-none" id="failuremessage">
                    An error occurred when attempting to process your request.
                </div>
        <?php
            }
        } else {
            header('Location: /500');
        }
        ?>
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
                        <a href="/contact" class="text-white">
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
                        <i class="fas fa-globe-africa"></i>
                        <a href="/" class="text-white">
                            tuscblackchamber.org</a>
                    </p>
                    <p>
                        <i class="fas fa-user-shield"></i>
                        <a href="/privacy-policy" class="text-white">
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
                Website developed by <a href="https://rhtservices.net" target='_blank'>Robinson Handy and Technology Services LLC</a>
            </div>
            <div class="col-sm-12 pt-2">
                Cookies are used on this website to track your visits, provide advertisements specific to you, and preferences
                by a third-party. By continuing to use this site, you agree to the use of cookies unless you have disabled them.
                More information this is available in the Privacy Policy.
            </div>
            <!-- Last updated: 2021-01-16 13:43:02.807170+00:00 -->
        </div>
    </footer>
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js" async integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj" crossorigin="anonymous"></script>
    <!-- <script>window.jQuery || document.write('<script src="../assets/js/vendor/jquery.slim.min.js"><\/script>')</script> -->
    <script async src="/js/bootstrap.bundle.min.js"></script>
    <script async src="https://kit.fontawesome.com/a076d05399.js"></script>
    <script async src="/js/javascript.min.js"></script>
</body>
</html>