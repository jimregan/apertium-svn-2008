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

	$resultado = join("",file($inurl));
	$resultado = str_replace("<head>", "<head>\n<base href='".$inurl."' target='_top'/>", $resultado);
   
   $encoding = detect_encoding($resultado);
	// Only applied for ro-es
	$resultado = replace_characters($dir,$resultado);
	write_file($resultado, $tempfile);
	$cmd = build_translation_cmd($dir, $inurl, $mark, $tempfile,$encoding[1]);
	//print "cmd = " . $ejecutable;
	echo("<base href='".$inurl."' target='_top'>");
	header("Content-Type: text/html; charset=\"". $encoding[0] . "\"");
	$str = shell_exec($cmd);
	$str = special_process($dir,$str);
 	
 	if($mark==1){
    if($dir=="es-ca") {
      $mot="mot";
    } else {
    	$mot="mot2";
    }
    $str = ereg_replace("HREF=\"http://[^\"\']+/cgi\-bin/diccionaris/paraula2\.cgi\%3f?","target=\"_top\" href=\"http://".getenv("SERVER_NAME").$puerto."/diccionaris/index.php?".$mot."=",$str);
    $str = ereg_replace("\%26(#[0-9]{2,3})\%3b","&\\1;",$str);
  }

  unlink($tempfile);

  // parsear $str buscando frames para no poner el marco de navegacion
  // repetido

  $str = eregi_replace("<frame[ \n\t\r]+src[ \n\t\r]*=[ \n\t\r]*","<frame src=",$str);

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

// Replace characters
function replace_characters($dir, $resultado) {
	// Solución no muy elegante pero que me resuelve 
	// por ahora el problemita de ro-es
	if ($dir == "ro-es") {
		$resultado = utf8_encode($resultado);
		$resultado = str_replace("ã","ă",$resultado);
		$resultado = str_replace("º","ș",$resultado);
		$resultado = str_replace("ş","ș",$resultado);  
		$resultado = str_replace("ª","Ș",$resultado);    	 
		$resultado = str_replace("þ","ț",$resultado);
		$resultado = str_replace("ţ","ț",$resultado);
	}
	return $resultado;
}

// Special processing
function special_process($dir, $str) {
	if( $dir == "ro-es" ) {
		$str = utf8_decode($str);
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
	// for ro-es
	//	$ejecutable = "LANG=en_GB.UTF-8 cat $tempfile | LANG=en_GB.UTF-8 sed 's/ţ/ț/g' | LANG=en_GB.UTF-8 sed 's/ş/ș/g' | LANG=en_GB.UTF-8 $APERTIUM_TRANSLATOR -f $type $markUnknown $dir | $TURL_PATH \"$dirbase\" \"$inurl\"";
	// any other language pair
	if ( $dir == "ro-es") {
		$ejecutable = "LC_ALL=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f $type $dir $tempfile | $TURL_PATH \"$dirbase\" \"$inurl\"";
	} else {
   $ejecutable = "LC_ALL=es_ES.$encoding $APERTIUM_TRANSLATOR -f $type $dir $tempfile | $TURL_PATH \"$dirbase\" \"$inurl\"";
   }
   return $ejecutable;
}


function detect_encoding($text) {
	$enc = mb_detect_encoding($text, "UTF-8, ISO-8859-15");
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


