<?php
  include("header.php");

  $dir = $_POST["dir"];
  $mark = $_POST["mark"];
 

  if($dir=="") {
          $dir = $_POST["dir"];
  }
  if($dir=="") {
          $dir = $_GET["dir"];
  }
  if($mark=="") {
          $mark = $_GET["mark"];
  }
  if($inurl=="") {
          $inurl = $_POST["inurl"];
  }
  if($inurl=="") {
          $inurl = $_GET["inurl"];
  }

  $inurl = urldecode($inurl);

	if($inurl=="") {
    error(_("URL not found"));
  }

  if(substr($inurl, 0, 7) != "http://") {
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
  <!--    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"> -->
    <title>Apertium</title>
  </head>

<?php 
	$variante = $_POST["variante"];
	
   if($variante!=2){
  ?>
  
  <frameset rows="50,*" frameborder="no">
  <?php
   }
   else{
  ?>

  <frameset rows="*" frameborder="no">
    <?php
    }
    
	echo("<frame src=\"margen.php?dir=$dir&inurl=$inurl\" name=\"_margen\" border=\"0\" frameborder=\"no\" framespacing=\"0\" marginwidth=\"1\" marginheight=\"1\" scrolling=\"no\">");
	echo("<frame src=\"tradurl.php?mark=$mark&dir=$dir&inurl=$inurl\" name=\"_cos\" border=\"0\" frameborder=\"no\" framespacing=\"0\" marginwidth=\"1\" marginheight=\"1\" scrolling=\"auto\">");
       
  ?>
  </frameset>
  <noframes>
    <?php print _("Update your browser") ?>
  </noframes>
</html>
