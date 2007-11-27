<?php $inurl = $_GET["inurl"]; $inurl=urldecode($inurl); ?>

  <base target="_top">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    <title>apertium</title>
    <link rel="stylesheet" href="/styles/gila-screen.css" type="text/css">
  </head>
  <body text="#000000" bgcolor="#FFFFFF" link="#990000" vlink="#550000" alink="#FFCC00">
  <table border="1">
    <table width="100%" border="0" cellpadding="0" cellspacing="0">
          <tbody>
            <tr>
              <td width="1%"></td>
              <td align="center">
              <b>
<?php
  $inurl = $_GET["inurl"];
  $direccion = $_GET["direccion"];
  if(strstr($inurl,"http://")==false)
    $inurl="http://$inurl";
  else if(strstr($inurl,"?inurl")!=false){
    $inurl=str_replace("direccion=es-ca","direccion=null",$inurl);
    $inurl=str_replace("direccion=ca-es","direccion=null",$inurl);
  }

echo("<a href='$inurl' target='_blank'>");
echo("Original page");
?></a></b></font></td>
            </tr>
<!--            <tr>
              <td colspan="2" bgcolor="#000000" valign="top" height="5">
          </td>
            </tr> -->
  </table>
  </body>
</html>

