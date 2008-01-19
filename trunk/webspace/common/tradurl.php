<?php
	include("header.php");
	require("../config/apertium-config.php");
	
	$dir = $_GET["dir"];
	$mark = $_GET["mark"];
	if ($mark=="")
		$mark = $_POST["mark"];	
	$inurl = $_GET["inurl"];
	$inurl = urldecode($inurl);
	
	// la variable $autorizado si es distinto de 0 se puede usar sin límites
	if(strlen($inurl)>=500){
		echo("Error");
		exit;
	}
	$autorizado = 0;
	error_reporting(E_USER_ERROR);

	$tempfile = tempnam("/tmp","URL");
	error_reporting(0);  

	//$resultado = join("",file($inurl));
	$resultado = file_get_contents($inurl);
	$resultado = str_replace("<head>", "<head>\n<base href='".$inurl."' target='_top'/>", $resultado);
   $encoding = detect_encoding($resultado);
	// Only applied for ro-es
	$resultado = replace_characters($dir, $resultado, $encoding[0]);
  	write_file($resultado, $tempfile);
  	
	$cmd = build_translation_cmd($dir, $inurl, $mark, $tempfile, $encoding[1]);
	//print "cmd = " . $cmd;
	echo("<base href='".$inurl."' target='_top'>");
	
	header("Content-Type: text/html; charset=\"". $encoding[0] . "\"");
	
	$str = shell_exec($cmd);
	$str = special_process($dir,$str,$encoding[0]);
		
	print $str;
	//print "Translation: $str";
	
 	
 	/*
 	if($mark==1){
    if($dir=="es-ca") {
      $mot="mot";
    } else {
    	$mot="mot2";
    }
    $str = ereg_replace("HREF=\"http://[^\"\']+/cgi\-bin/diccionaris/paraula2\.cgi\%3f?","target=\"_top\" href=\"http://".getenv("SERVER_NAME").$puerto."/diccionaris/index.php?".$mot."=",$str);
    $str = ereg_replace("\%26(#[0-9]{2,3})\%3b","&\\1;",$str);
  }
  */

  //unlink($tempfile);

  // parsear $str buscando frames para no poner el marco de navegacion
  // repetido

  //$str = eregi_replace("<frame[ \n\t\r]+src[ \n\t\r]*=[ \n\t\r]*","<frame src=",$str);

/*
  $strToSearch = "src=\"http://".getenv("SERVER_NAME").$puerto."/".$reltype."/browser.php?";
  $replaceWith = $strToSearch."variante=2&";
  $str = str_replace($strToSearch,$replaceWith,$str);

	for($i = 0, $limit = strlen($str); $i != $limit; $i++) {
    if(ord($str[$i]) >= 128) {
      $myvar = ord($str[$i]);
      echo "&#$myvar;";
    } else {
      echo $str[$i];
    }
  }
  
  */

// Replace characters
function replace_characters($dir, $text, $encoding) {
	if ($dir == "ro-es") {
		if( $encoding != "UTF-8" ) {
			$text = utf8_encode($text);
			$text = str_replace("þ","ț",$text);
			$text = str_replace("Þ","Ț",$text);			
			$text = str_replace("ã","ă",$text);
			$text = str_replace("Ã","Ă",$text);
			$text = str_replace("ª","Ș",$text);
			$text = str_replace("º","ș",$text);			
		} else {
		$text = str_replace("ş","ș",$text); 
		$text = str_replace("Ş","Ș",$text); 
		$text = str_replace("ţ","ț",$text); 
		$text = str_replace("Ţ","Ț",$text);
		}

	}
	return $text;
}

// Special processing
function special_process($dir, $str, $encoding) {
	if( $dir == "ro-es" ) {
		if( $encoding != "UTF-8" ) { 
			// Solución temporal
			$str = str_replace("ă","ã",$str);
			$str = str_replace("Ă","Ã",$str);			
			$str = str_replace("ț","þ",$str);
			$str = str_replace("Ț","Þ",$str);		
			$str = str_replace("Ș","ª",$str);
			$str = str_replace("ș","º",$str);			
			$str = utf8_decode($str);
		} else {
			$str = str_replace("ș","ş",$str); 
			$str = str_replace("Ș","Ş",$str); 
			$str = str_replace("ț","ţ",$str); 
			$str = str_replace("Ț","Ţ",$str);
		}
	}
	return $str;
}

// Build translation command
function build_translation_cmd($dir, $inurl, $mark, $tempfile, $encoding) {
	global $APERTIUM_TRANSLATOR;
	global $TURL_PATH;
	global $WWW_ROOT_DIR;
	
	$puerto = getenv("SERVER_PORT")==80?"":":".getenv("SERVER_PORT");
  $dirbase = "http://".getenv("SERVER_NAME")."$puerto/$WWW_ROOT_DIR/common/browser.php?mark=$mark&dir=$dir&inurl=";
 	$type = "html -u";
 	if ($mark==1 ) {
 		$type = "html";
 	}
   //$ejecutable = "LC_ALL=es_ES.$encoding $APERTIUM_TRANSLATOR -f $type $dir $tempfile | $TURL_PATH \"$dirbase\" \"$inurl\"";
	$ejecutable = "LC_ALL=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f $type $dir $tempfile | $TURL_PATH \"$dirbase\" \"$inurl\"";
   return $ejecutable;
}

// Detect encoding
function detect_encoding($text) {
	$enc = mb_detect_encoding($text, "UTF-8, ISO-8859-15", "ISO-8859-2");
	$e[0] = $enc;
	if($enc == "UTF-8") {
		$encoding = "utf8";
  } else {
    $encoding = "iso885915@euro";
  }
  $e[1] = $encoding;
  return $e;

}

function write_file($resultado, $tempfile) {
  vacio($resultado,"Page not found");
  limite($resultado,16384*4*4*4, "Maximum size excedeed");
  	
  $fd = fopen($tempfile,"w");
  fputs($fd, $resultado);
  fclose($fd);
}

?>


