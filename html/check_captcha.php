<?php
require_once "class_sql.php";
use StreamAPI as API;
$ApiSQL = new API\Captcha();
$rv=$ApiSQL->checkCaptchaHTTP();
if($rv == True){
	print "Accepted";
}else print "Deny";
