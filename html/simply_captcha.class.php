<?php
require_once "class_sql.php";
use StreamAPI as API;

class Simply_Captcha_Render{
	private $im = NULL;
	private $background = NULL;
	private $fontColor = NULL;
	protected $width;
	protected $height;
	protected function getRandomColor(){
		$r = rand(0, 255);
		$g = rand(0, 255);
		$b = rand(0, 255);
		$textBlack = True;
		if($r < 60 || $g<60 || $b<60) $textBlack=False; 
		return 
		array(
		"red"=>$r,
		"green"=>$g,
		"blue"=>$b,
		"textBlack"=>$textBlack
		);
		
	}
	
	public function initImage(/*integer*/ $width,/*integer*/ $height){
	 	$this->im = imagecreatetruecolor($width,$height) or False;
	 	$color = $this->getRandomColor();
	 	$this->background = imagecolorallocate($this->im, $color["red"], $color["green"] ,$color["blue"]);
		$this->fontColor = $color["textBlack"] ? imagecolorallocate($this->im, 0, 0 ,0) : imagecolorallocate($this->im, 255, 255 ,255);
		imagefill($this->im, 0, 0, $this->background);
		$this->width = $width;
		$this->height = $height;
	}
	public function generateRandString($length=10){
	 $string = "";
	 while($length--){
		 $type = rand(0,2);
		 if(!$type)
		  $string = $string.chr(rand(48,57));//0-9
		 else if($type == 1)
		   $string = $string.chr(rand(65,90)); // A-Z
		 else
		  $string = $string.chr(rand(97,122)); // a-z
	 }
	 	return $string;
	}
	protected function randomPixels($mincount=100,$maxcount=500){
	  $color = $this->getRandomColor();
	  $color = imagecolorresolvealpha($this->im,$color['red'],$color['green'],$color['blue'],10);
	  for($i = rand($mincount,$maxcount);$i--;){
		 $tmpx=rand(0,$this->width) % $this->width;
		 $tmpy=rand(0,$this->height) % $this->width;
		 imagesetpixel($this->im, $tmpx,$tmpy, $color); 
	  }
	  
	  
	}
	protected function blur(){
		imagefilter($this->im, IMG_FILTER_GAUSSIAN_BLUR);
	}
	protected function lines($countCurwes=5){
		//shitcode lines
	  $color = $this->getRandomColor();
	  $color = imagecolorresolvealpha($this->im,$color['red'],$color['green'],$color['blue'],20);
	  while($countCurwes--){
	  $sx=rand(0,$this->width*2) % $this->width;
	  $sy=rand(0,$this->height*2) % $this->width;
	  imageline($this->im, $sx, $sy, $this->width, $this->height,$color);
      }
	} 
	public function getim(){
	 return $this->im;	
	}
	
	public function rendImage(){
		imagepng($this->im);
	}
	
	public function initRandString($font="./arial.ttf",$colorText=NULL,$sizeFont=13,$angle=2){
	 	if(!$colorText) $colorText = $this->fontColor;

	 	$string = isset( $_GET['TIMESTAMP'] ) && strlen($_GET['TIMESTAMP']) == 64 ? $this->generateRandString() : "ERROR NO TIMESTAMP";
		imagettftext($this->im, $sizeFont, $angle, 2, 30, $colorText, $font, $string);
		return $string;
	}
	
	public function __destruct(){
		imagedestroy($this->im);
	}
}

class Simply_Captcha extends Simply_Captcha_Render{
	public function __construct($width = 105,$height = 50,$algo="sha256",$nameofsession="SCAPTCHA"){
     	session_start();
		$this->ApiSQL = new API\Captcha();
		$this->initImage($width,$height);
		$string = $this->initRandString();
		$this->lines();
		$this->randomPixels();
		$this->blur();
		if(isset( $_GET['TIMESTAMP'] )){
			//$_SESSION[$nameofsession] = hash($algo,$string);
			$this->ApiSQL->addCaptcha($_GET['TIMESTAMP'], hash($algo,$string));
		}
		$this->rendImage();
	}
}

?>
