<?php
	include("header.php");
	require("../config/apertium-config.php");
	global $APERTIUM_TRANSLATOR;
	global $TURL_PATH;
	
	$dir = $_GET["dir"];
	$mark = $_GET["mark"];
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
  
  shell_exec("cat $resultado | LANG=es_ES.UTF-8 sed 's/ţ/ț/g' | LANG=es_ES.UTF-8 sed 's/ş/ș/g'");
  
  $resultado = utf8_encode($resultado);
  
  vacio($resultado,"Page not found");
  limite($resultado,16384*4*4*4, "Maximum size excedeed");

  $fd = fopen($tempfile,"w");
  if(!$fd){
    echo("Error");
    exit;
  }                                           
  
  fputs($fd,$resultado);
  fclose($fd);

  echo("<base href='".$inurl."' target='_top'>");
  
  $puerto = getenv("SERVER_PORT")==80?"":":".getenv("SERVER_PORT");
  $dirbase = "http://".getenv("SERVER_NAME")."$puerto/$WWW_ROOT_DIR/common/browser.php?mark=$mark&dir=$dir&inurl=";
 
 	$type = "html -u";
 	if ($mark == 1 ) {
 		$type = "html";
 	}

	if ($dir == "ro-es" ) {
		$ejecutable = "cat $tempfile | LANG=es_ES.UTF-8 sed 's/ţ/ț/g' | LANG=es_ES.UTF-8 sed 's/ş/ș/g' | LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f $type $markUnknown $dir | $TURL_PATH \"$dirbase\" \"$inurl\"";
  } else {
   $ejecutable = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f $type $dir $tempfile | $TURL_PATH \"$dirbase\" \"$inurl\"";
  }
  
  //echo "CMD: $ejecutable";

  $str = shell_exec($ejecutable);
 	$str = utf8_decode($str);
  
  if($mark==1){
    if($dir=="es-ca") {
      $mot="mot";
    } else {
    	$mot="mot2";
    }
    
    $str = ereg_replace("HREF=\"http://[^\"\']+/cgi\-bin/diccionaris/paraula2\.cgi\%3f?","target=\"_top\" href=\"http://".getenv("SERVER_NAME").$puerto."/diccionaris/index.php?".$mot."=",$str);
    $str = ereg_replace("\%26(#[0-9]{2,3})\%3b","&\\1;",$str);
  }

  unlink($archivo);

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
  
?>


