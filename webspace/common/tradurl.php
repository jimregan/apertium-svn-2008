<?php
 include("header.php");

 $direccion = $_GET["direccion"];
 $marcar = $_GET["marcar"];
 $inurl = $_GET["inurl"];

 $inurl=urldecode($inurl);

 $basedata = "/home/fran/svnroot/archive/apertium-";
 $translator = "/home/fran/svnroot/local/stable/bin/apertium-translator";
 $relpath = "apertium";

  switch ($direccion){

  case "es-ca": case "ca-es": $nomtrad="es-ca"; break;
  case "es-gl": case "gl-es": $nomtrad="es-gl"; break;
  case "es-pt": case "pt-es": $nomtrad="es-pt-br"; break;
  case "es-br": case "br-es": $nomtrad="es-pt-br"; break;
  case "oc-ca": case "ca-oc": $nomtrad="oc-ca"; break;
  case "fr-ca": case "ca-fr": $nomtrad="fr-ca"; break;
  case "en-ca": case "ca-en": $nomtrad="en-ca"; break;
  default: $nomtrad="es-ca"; break;
}

  if($nomtrad == "en-ca")
  {
    $translator = "/home/fran/svnroot/local/stable/bin/apertium";
  }

 $datadir = $basedata.$nomtrad.'*/';

// la variable $autorizado si es distinto de 0 se puede usar sin límites
  if(strlen($inurl)>=500){
    echo("Error");
    exit;
  }
  $autorizado = 0;
  error_reporting(E_USER_ERROR);


  $archivo=tempnam("/tmp","URL");

  error_reporting(0);
  $resultado=join("",file($inurl));
  vacio($resultado,"Page not found");
  limite($resultado,16384*4*4*4, "Maximum size excedeed");

  $fd=fopen($archivo,"w");
  if(!$fd){
    echo("Error");
    exit;
  }                                           
  
  fputs($fd,$resultado);

  fclose($fd);

  echo("<base href='".$inurl."' target='_top'>");
  $puerto=getenv("SERVER_PORT")==80?"":":".getenv("SERVER_PORT");
  $dirbase="http://".getenv("SERVER_NAME")."$puerto/$relpath/navegador.php?marcar=$marcar&direccion=$direccion&inurl=";
  if($marcar==1) {
    //$traductor="$direccio.trad.webd";
    //$traductor="trad.sh $direccio webd";
    $ejecutable = "$translator $datadir $direccion html $archivo | turl \"$dirbase\" \"$inurl\"";

//    $ejecutable = "$translator $datadir $direccion html $archivo | turl \"". $dirbase . $inurl ."\"";
  }
  else {
    //$traductor="$direccio.trad.web";
    //$traductor="trad.sh $direccio web";
    $ejecutable = "$translator $datadir $direccion htmlu $archivo | turl \"$dirbase\" \"$inurl\"";
#    $ejecutable = "cat $archivo";

//    $ejecutable = "$translator $datadir $direccion htmlu $archivo | turl \"". $dirbase . $inurl ."\"";
  }

//  print $ejecutable;

  $micad=shell_exec($ejecutable);
  if($marcar==1){
    if($direccion=="es-ca")
      $mot="mot";
    else $mot="mot2";
    $micad=ereg_replace("HREF=\"http://[^\"\']+/cgi\-bin/diccionaris/paraula2\.cgi\%3f?","target=\"_top\" href=\"http://".getenv("SERVER_NAME").$puerto."/diccionaris/index.php?".$mot."=",$micad);
    $micad=ereg_replace("\%26(#[0-9]{2,3})\%3b","&\\1;",$micad);
  }

  unlink($archivo);

  // parsear $micad buscando frames para no poner el marco de navegacion
  // repetido


  $micad=eregi_replace("<frame[ \n\t\r]+src[ \n\t\r]*=[ \n\t\r]*","<frame src=",$micad);

  $cadabuscar="src=\"http://".getenv("SERVER_NAME").$puerto."/".$reltype."/navegador.php?";
  $sustituyepor=$cadabuscar."variante=2&";
  $micad=str_replace($cadabuscar,$sustituyepor,$micad);

 for($i = 0, $mylimit = strlen($micad); $i != $mylimit; $i++)
  {
    if(ord($micad[$i]) >= 128)
    {
      $myvar = ord($micad[$i]);
      echo "&#$myvar;";
    }
    else
    {
      echo $micad[$i];
    }
  }
?>


