<?php


//Se leen los datos del fichero y se llama al formulario para validar los datos

$path="../dics";
$fichSemi="$path/$LR-$RL.semi.dic";

	
$entradas=file($fichSemi);


$cambio=0;
for($cont=0;$cont<count($entradas);$cont++)
{

	$entradas[$cont]=str_replace("\n","",$entradas[$cont]); 	// quita los \n
	$trozos=split("\t",$entradas[$cont]);
	


	$LRlemV[$cont]=$trozos[0];
	if($LRlemV[$cont]=="$LRlem")
	{	
		if($selecc!=$cont)
		{
			$selecc=$cont;
			$cambio=1;
		}else
			$cambio=0;
	}	
	
	
	$RLlemV[$cont]=$trozos[1];
	$paradigmaLRV[$cont]=$trozos[2];
	$direccionV[$cont]=$trozos[3];
	$paradigmaRLV[$cont]=$trozos[4];
	$tipoV[$cont]=$trozos[5];
	$comentariosV[$cont]=$trozos[6];
	$userV[$cont]=$trozos[7];
	$geneLRV[$cont]=$trozos[8];
	$geneRLV[$cont]=$trozos[9];
	$numLRV[$cont]=$trozos[10];
	$numRLV[$cont]=$trozos[11];
	$LR=$LR;
	$RL=$RL;
}


if($selecc=="" && $paradigmaLR!="Altrecas")
	$selecc=0;   //elemento seleccionado 	
	

if($valida==0 || $cambio==1 )
{
	$LRlem=$LRlemV[$selecc];
	$RLlem=$RLlemV[$selecc];
	$paradigmaLR=$paradigmaLRV[$selecc];
	$direccion=$direccionV[$selecc];
	$paradigmaRL=$paradigmaRLV[$selecc];
	$tipo=$tipoV[$selecc];
	$comentarios=$comentariosV[$selecc];
	$user=$userV[$selecc];
	$geneLR=$geneLRV[$selecc];
	$geneRL=$geneRLV[$selecc];
	$numLR=$numLRV[$selecc];
	$numRL=$numRLV[$selecc];
	$LR=$LR;
	$RL=$RL;
	
	
	$valida=1;  //le indica a selecc.php que va a validar palabras y debe mostrar todas
}



if($tipo=="n" || $tipo=="adj" || $tipo=="vb" || $tipo=="adv" || $tipo=="pname" || $tipo=="acr"){
//if($tipo !== "" && $selecc !== ""){
//if($tipo !== "" ){
//if($tipo=="n" || $tipo=="adj" || $tipo=="vb"){

	include("./selecc.php");
}	   
 else{
  
   if($selecc==""){
   echo "<?xml version=\"1.0\" encoding=\"ISO-8859-15\"?>\n";
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="es">
  <head>
    <meta http-equiv="content-type" content="application/xhtml+xml; charset=ISO-8859-15" />
    <meta name="author" content="haran" />
    <meta name="generator" content="haran" />

    <!-- Navigational metadata for large websites (an accessibility feature): -->
    <link rel="top"      href="./index.html" title="Homepage" />
    <link rel="up"       href="./index.html" title="Up" />
    <link rel="first"    href="./index.html" title="First page" />
    <link rel="previous" href="./index.html" title="Previous page" />
    <link rel="next"     href="./index.html" title="Next page" />
    <link rel="last"     href="./index.html" title="Last page" />
    <link rel="toc"      href="./index.html" title="Table of contents" />
    <link rel="index"    href="./index.html" title="Site map" />

    <link rel="stylesheet" type="text/css" href="/styles/gila-screen-form2.css" media="screen" title="Gila (screen)" />
    <link rel="stylesheet" type="text/css" href="/styles/gila-print.css" media="print" />
    <title>OpenTrad apertium insertion form. Validating words.</title>
   <script language="JavaScript">
    <!--  //Comprueba que se han seleccionado los paradigmas
   
        function cierra() 
   	{
   		window.close();
   	}
   
     -->
    </script>
</head>
<body>

<?php
	 
     	echo "<div class=\"area\">\n";
	echo "<h3>All words have already been validated!</h3>\n";      
        echo "<h3>Thanks and see you soon!</h3>\n";      
        echo "<input type=\"button\" value=\"<< Go back\" onclick=\"history.go(-1)\">\n";
        echo "<input type=\"button\" value=\"- Close -\" onclick=\"history.go(-1)\"><br/><br/>\n";
	echo "</div>\n";
	
        echo "<!-- ##### Footer ##### -->\n"; 

        echo "<div id=\"footer\">\n"; 
  
        echo "<div><a href=\"http://www.dlsi.ua.es\">\n"; 
        echo "Departament de Llenguatges i Sistemes Informàtics</a>, Universitat\n"; 
        echo " d'Alacant, 2006 | Last modified 20.02.2006<!-- by <a\n"; 
        echo "href=\"mailto:sortiz@dlsi.ua.es\" title=\"Envíame un correo\">Sergio\n"; 
        echo "Ortiz</a> -->\n"; 
        echo "</div>\n"; 
      
        echo "</div>\n"; 
   
        echo "</body>\n";
        echo "</html>\n";	
 
 }
 else
     include("./insertarPro.php");

}
?>

