<?php
require_once "class_sql.php";
use StreamAPI as API;
$ApiSQL = new API\Users();
$need_values=array("nick", "password");
foreach( $need_values as $n )
	if(!isset($_GET[$n])) die("Not exist values");
$rv=$ApiSQL->registerUser($_GET['nick'], $_GET['password']);
if($rv == True){
	print "Accepted";
}else print "Deny";
