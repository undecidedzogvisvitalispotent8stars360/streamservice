<?php
require_once "class_sql.php";
use StreamAPI as API;
	$ApiSQL = new API\Users();
	$StreamSQLApi = new API\Streams();
	$url = $_GET['swfurl'];
	//... split url, if its not private stream url, and not nickname of owner -> 403? or allow multiple streams...
	
	if ( !isset($_GET['username']) || !isset($_GET['password'])   ) return http_response_code(403);
	if( $ApiSQL -> checkUserByNamePassword($_GET['username'], $_GET['password']) ){
		 return header("HTTP/1.1 202 Accepted");;//
	}
	return http_response_code(403);  
?>
