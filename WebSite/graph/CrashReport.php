<?php
// ***************************************************************
//  bugRepMailer.php          version:  1.2   ·  date: 2012-05-06
//  -------------------------------------------------------------
//  exception handling
//  -------------------------------------------------------------
//  Copyright (C) 1999 - 2012 www.madshi.net, All Rights Reserved
// ***************************************************************

// 2012-05-06 1.2  (1) added "Basic" and "Digest" authentication scheme
//                 (2) added download link to recommended phpmailer version
// 2006-09-06 1.1  (1) added "MailFrom" field evaluation
//                 (2) added failure indication, if mailing failed
// 2006-09-02 1.0  initial version donated by David Perkins (thank you!)

// CAUTION: this script needs the file "class.phpmailer.php"
// the recommended version is 1.73 which you can download here:
// http://sourceforge.net/projects/phpmailer/files/phpmailer for php4/1.73/

// CAUTION: authentication can be troublesome, see here for tips:
// http://www.besthostratings.com/articles/http-auth-php-cgi.html

// ***************************************************************

// enter your email address here
// it's hard coded so the script can't be misused by anyone for spamming
$send_to = 'crash@padowan.dk';

// choose the authentication user/password
$user     = 'Graph';
$password = 'FuefTC4KnpwXLmnSAuLT';

// the following fields should be overwritten by madExcept
// but you can still initialize them, just in case
$subject    = 'bug report';
$body       = 'please find the bug report attached';
$from_email = 'sender@email.com';
$from_name  = 'MadExcept';

// ***************************************************************

require 'class.phpmailer.php';

// ***************************************************************

$realm = 'madExcept upload script';

// ***************************************************************
// here comes the real script code

if (($user != '') && (($_SERVER['PHP_AUTH_USER'] != $user) || ($_SERVER['PHP_AUTH_PW'] != $password)))
{
  // read the Digest response
  $needed_parts = array('nonce'=>1, 'nc'=>1, 'cnonce'=>1, 'qop'=>1, 'username'=>1, 'uri'=>1, 'response'=>1);
  $data = array();
  $keys = implode('|', array_keys($needed_parts));
  preg_match_all('@('.$keys.')=(?:([\'"])([^\2]+?)\2|([^\s,]+))@', $_SERVER['PHP_AUTH_DIGEST'], $matches, PREG_SET_ORDER);
  foreach ($matches as $m)
  {
    $data[$m[1]] = $m[3] ? $m[3] : $m[4];
    unset($needed_parts[$m[1]]);
  }
  // generate the valid response
  $A1 = md5($user.':'.$realm.':'.$password);
  $A2 = md5($_SERVER['REQUEST_METHOD'].':'.$data['uri']);
  $valid_response = md5($A1.':'.$data['nonce'].':'.$data['nc'].':'.$data['cnonce'].':'.$data['qop'].':'.$A2);
  // compare the response
  if (($needed_parts) || ($data['username'] != $user) || ($data['response'] != $valid_response))
  {
//    header('WWW-Authenticate: Basic realm="'.$realm.'"');
    header('WWW-Authenticate: Digest realm="'.$realm.'",qop="auth",nonce="'.uniqid().'",opaque="'.md5($realm).'"');
    header('HTTP/1.1 401 Unauthorized');
    exit;
  }
}

// setup a new PHPMailer instance and fill all needed fields
$mailer = new PHPMailer();
$mailer->AddAddress($send_to);
$mailer->Subject  = $subject;
$mailer->Body     = $body;
$mailer->From     = $from_email;
$mailer->FromName = $from_name;
$mailer->Mailer   = 'mail';

// let's parse a couple of fields which madExcept should have set
if (isset($_POST['MailFrom']))
{
  $mailFrom = $_POST['MailFrom'];
  $i1 = strpos($mailFrom, '<');
  $i2 = strrpos($mailFrom, '>');
  if (!(($i1 === false) || ($i2 === false) || ($i1 >= $i2)))
  {
    $mailer->From     = trim(substr($mailFrom, $i1 + 1, $i2 - $i1 - 1));
    $mailer->FromName = trim(substr($mailFrom, 0,       $i1          ));
  }
}
if (isset($_POST['MailSubject']))
  $mailer->Subject = $_POST['MailSubject'];
if (isset($_POST['MailBody']))
  $mailer->Body = $_POST['MailBody'];

// add all attachments to the mail
$found = false;
foreach ($_FILES as $arr)
{
  $mailer->AddAttachment($arr['tmp_name'], $arr['name']);
  $found = true;
};
if ($found)
  $mailer->AttachAll();
if (($found) || (isset($_POST['MailSubject'])) || (isset($_POST['MailBody'])))
{
  // we've found an attachment, or at least the mail subject or body was set
  // so we send the email
  if (!$mailer->Send())
    header('HTTP/1.0 500 Mailing failed');
}
?>
