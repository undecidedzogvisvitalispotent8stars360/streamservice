<?php
namespace StreamAPI{
require_once "config.php";
	class SQL{
		const sqls = array(
			"getCaptches"=>"SELECT * from Captcha;",
			"getCaptchaBySession"=>"SELECT * from Captcha where hash_session='%s';",
			"addCaptcha"=>"INSERT INTO Captcha VALUES( '%s', '%s' );",
			
			"addUser"=>'INSERT INTO users(name, password) VALUES("%s", "%s" );',
			"checkUserByName"=>"SELECT * FROM users WHERE name='%s';",
			"checkUserByNamePassword"=>"SELECT * FROM users WHERE name='%s' and password='%s';",

			"removeUserByName"=>"DELETE FROM users where name='%s'",


			"truncateCaptcha"=>"truncate Captcha;",
			"deleteCaptcha"=>"delete from Captcha where hash_session='%s';",
			"checkCaptcha"=>"SELECT * from Captcha where hash_session='%s' and hash_value='%s'; ",
			"getCountOfTB"=>"select COUNT(*) AS count FROM %s",
			"getCountOfTBWhere"=>"select COUNT(*) AS count FROM %s WHERE %s='%s'"
		);
		function doSQL($sprintf, ...$arguments){
			$string = vsprintf($sprintf, $arguments );
			//printf("Debug info: %s\n\r<br>", $string);
			return $result = mysqli_query($this->con, $string ); 
		}
		function getSQLCon(){
				return $this->con;
		}
		function __construct(){
			$this->con = mysqli_connect(MYSQL_ADDR, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB); 
			if(!$this->con) die("Can't connect to DB");
		}
		function __destruct(){
			mysqli_close($this->con);
		}
		function getLastSQLError(){
			return mysqli_error($this->con);
		}
		function getCountOfTB($table){
			$r= $this->doSQL( sql::sqls['getCountOfTB'], $table );
			return mysqli_fetch_assoc($r)['count'];
		}
		function getCountOfTBWhere($table,$a,$b){
			$r= $this->doSQL( sql::sqls['getCountOfTBWhere'], $table,$a,$b );
			return mysqli_fetch_assoc($r)['count'];
		}
		function real_escape($string){
			return mysqli_real_escape_string($this->con,$string);
		}

		};
	class Streams extends SQL{
		function __construct(){
			parent::__construct();
		}
		function addStream(){}
		function delStream(){}
		function checkStream(){}
		function getStreamList(){}		
	}
	class Users extends Captcha{
		function __construct(){
			parent::__construct();
		}
		function registerUser($username, $password){
			if(!$this->checkCaptchaHTTP()) die("Uncorrect captcha");
			if( strlen($username) < 3 || strlen($password) < 3) die("your username or password is short");
			$password=hash(DEFHASHALGO,$password);

			if($this->getCountOfTBWhere("users", "name",$this->real_escape($username) ) ) die("Name is used");
			$this->doSQL( sql::sqls['addUser'], $this->real_escape($username),
				$this->real_escape($password) );
			return true;
		}
		function checkUserByNamePassword($username, $password){
			$password=hash(DEFHASHALGO,$password);
			$rv=$this->doSQL( sql::sqls['checkUserByNamePassword'], $this->real_escape($username),
				$this->real_escape($password)  );	
			if( strlen(mysqli_fetch_assoc($rv)['name']) > 0 ) return true;
			return False;
		}
	}
	class Captcha extends SQL{
		function __construct($max_captchas=300){
			parent::__construct();
			$this->max_captchas=$max_captchas;
		}
		function checkHashSize($hash){
			if( strlen($hash) == DEFSIZEOFHASH ) return True;
			return False;
		}
		function addCaptcha( $hash_session, $hash_value ){
			if( strlen($hash_session) != 64 ) die("Uncorrect hash size");
			if( $this->getCountOfTBWhere( "Captcha", "hash_session", $this->real_escape( $hash_session ) )>0 )
				die("This HASH EXIST");
			if( $this->getCountOfTB("Captcha") > $this->max_captchas ) $this->doSQL( sql::sqls['truncateCaptcha'] );
			$this->doSQL( sql::sqls['addCaptcha'],
				 $this->real_escape( $hash_session ), $this->real_escape( $hash_value ));	
			return True;		
		}
		function deleteCaptcha( $session ){
			$this->doSQL(sql::sqls['deleteCaptcha'], $this->real_escape($session));
		}
		function checkCaptcha( $hash_session, $hash_value ){

			if( !$this->checkHashSize($hash_session) ) die("Uncorrect hash size");
			$rv=$this->doSQL(sql::sqls['checkCaptcha'], $this->real_escape($hash_session),
				 $this->real_escape($hash_value));
			$rv=mysqli_fetch_assoc($rv);
			if($rv) $this->deleteCaptcha($hash_session);
			return $rv;
		}
		function checkCaptchaHTTP(){
			$need_values=array("hash_session", "hash_value");
			foreach( $need_values as $n )
				if(!isset($_GET[$n])) die("Not exist values");
			$rv=$this->checkCaptcha($_GET["hash_session"], hash(DEFHASHALGO,$_GET["hash_value"]) );
			return $rv;
		}
	};
}
