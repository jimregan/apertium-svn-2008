<?php 
//requires("config/apertium-config.php");


// apertium-config.php: $APERTIUM_TRANSLATOR, $LING_DATA_DIR, etc. 

$lang="en";
$apertiumtranslator=$APERTIUM_TRANSLATOR;
$lingdata="$LING_DATA_DIR/apertium-$datapair/";
$dir="es-pt";
$tipo="txt";
$cuadrotexto = $HTTP_GET_VARS["textbox"];

$cuadrotexto=stripslashes($cuadrotexto);
$myfile=tempnam("tmp/","tradtext");
$fd=fopen($myfile,"w");
fputs($fd, $cuadrotexto);
fclose($fd);


$cmd="$apertiumtranslator $lingdata $dir $tipo $myfile";
//echo "Ejecuta: $cmd";

$trad=shell_exec($cmd);
$file = "content/" . $lang . "/translation.html";



//  $trad = $result;
  
// fin sustituciXn  


//  echo $trad;


// unlink($myfile);
// include("indparam.php");
?>
