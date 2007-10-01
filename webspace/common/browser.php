<?php
  include("header.php");

  $direccion = $_POST["direccion"];
  $marcar = $_POST["marcar"];

  if($direccion=="") {
          $direccion = $_POST["direccion"];
  }
  if($direccion=="") {
          $direccion = $_GET["direccion"];
  }
  if($inurl=="") {
          $inurl = $_POST["inurl"];
  }
  if($inurl=="") {
          $inurl = $_GET["inurl"];
  }

  $inurl = urldecode($inurl);

  if($inurl=="")
    error("URL no encontrada");

  if(substr($inurl, 0, 7) != "http://")
  {
    $inurl = "http://".$inurl;
  }

  $newurl = parse_url($inurl, PHP_URL_SCHEME)."://";
  if(parse_url($inurl, PHP_URL_USER)!=""){
    $usuario=1;
    $newurl.=parse_url($inurl, PHP_URL_USER);
  }
  if(parse_url($inurl, PHP_URL_PASS) !=""){
    $usuario=1;
    $newurl.=":".parse_url($inurl, PHP_URL_PASS);
  }
  if($usuario)
    $newurl.="@";
  $newurl.=parse_url($inurl, PHP_URL_HOST).parse_url($inurl, PHP_URL_PATH);
  if(parse_url($inurl, PHP_URL_PATH)=="")
    $newurl.="/";
  if(parse_url($inurl, PHP_URL_QUERY)!=""){
    $newurl.="?".parse_url($inurl, PHP_URL_QUERY);
  }
  $inurl=urlencode($newurl);
?>

<!DOCTYPE html public "-//w3c//dtd html 4.0 transitional//en">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    <title>apertium</title>
  </head>

  <?php
  $variante = $_POST["variante"];
   if($variante!=2){?>
  <frameset rows="50,*" frameborder="no">
  <?php
   }
   else{
  ?>

  <frameset rows="*" frameborder="no">
    <?php
    }
/*       if($variante!=2)*/
         echo("<frame src=\"margen.php?direccion=$direccion&inurl=$inurl\" name=\"_margen\" border=\"0\" frameborder=\"no\" framespacing=\"0\" marginwidth=\"1\" marginheight=\"1\" scrolling=\"no\">");
       echo("<frame src=\"tradurl.php?marcar=$marcar&direccion=$direccion&inurl=$inurl\" name=\"_cos\" border=\"0\" frameborder=\"no\" framespacing=\"0\" marginwidth=\"1\" marginheight=\"1\" scrolling=\"auto\">");
       
  ?>
  </frameset>
  <noframes>
    Actualitze el seu navegador
  </noframes>
</html>
