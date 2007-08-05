<?php

    if($tipo=="n")
    {
	$cadFich="noun";
	$show=": nouns";
	$tag="<s n=\"n\"/>";
	$tagout="<n>";
	
    }
    if($tipo=="adj")
    {
	$cadFich="adj";
	$show=": adjectivs";
	$tag="<s n=\"adj\"/>";
	$tagout="<adj>";
	
    }
    if($tipo=="vb")
    {
	$cadFich="verb";
	$show=": verbs";
	$tag="<s n=\"vblex\"/>";
	$tagout="<vblex>";
	$nota="";
    }
    
    if($tipo=="prep")
    {
	$cadFich="pr";
	$show=": prepositions";
	$tag="<s n=\"pr\"/>";
	$tagout="<pr>";
    }
    
    if($tipo=="adv")
    {
	$cadFich="adv";
	$show=": adverbs";
	$tag="<s n=\"adv\"/>";
	$tagout="<adv>";
	$nota="";
    }
    
    if($tipo=="cnjadv")
    {
	$cadFich="cnjadv";
	$show=": adverbial conjunctions";
	$tag="<s n=\"cnjadv\"/>";
	$tagout="<cnjadv>";
	$nota="";
    }
    
    if($tipo=="pname")
    {
	$cadFich="pname";
	$show=": proper names";
	$tag="<s n=\"np\"/>";
	$tagout="<np>";
	$nota="";
    }
       
     if($tipo=="acr")
    {
	$cadFich="acr";
	$show=": acronyms";
	$tag="<s n=\"n\"/><s n=\"acr\"/>";
	$tagout="<n><acr>";
	$nota="";
    }
	

    $path="../dics";
    $fichSemi="$path/$LR-$RL.semi.dic";

       

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

    <link rel="stylesheet" type="text/css" href="/styles/gila-screen.css" media="screen" title="Gila (screen)" />
    <link rel="stylesheet" type="text/css" href="/styles/gila-print.css" media="print" />
    <title>OpenTrad apertium insertion form. Insert new<?= $show ?> </title>

<title>Insert new<?= $show ?></title>

<script language="Javascript">
function cierra() {
   window.close();
}
</script>
</head>
<body>

<?php
        //se eliminan las barras '\' de las cadenas con los lemas
        $LRlem=str_replace("\\","",$LRlem);      
        $RLlem=str_replace("\\","",$RLlem);              
//	$LRlem=str_replace(" ","<b/>",$LRlem);      
//      $RLlem=str_replace(" ","<b/>",$RLlem); 


$cadenaFichero=$LRlem."\t".$RLlem."\t".$paradigmaLR."\t".$direccion."\t".$paradigmaRL."\t".$tipo."\t".$comentarios."\t ".$user."\t".$geneLR."\t".$geneRL."\t".$numLR."\t".$numRL."\n";
	      
    
     
      system("echo -n \"$cadenaFichero\" >> $fichSemi", $retval);
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!</h2><br>
                <h2>This entry has not been inserted in '$fichSemi'.</h2><br>
	        <h3>It is probably due to a permission problem.</h3><br><br>
                </center>");      	     
      }
      system("grep \"$cadenaFichero\" $fichSemi >/dev/null", $retval);
      if ($retval!=0)
	 print("<center><h2>ERROR de tipo desconocido</h2><br><br>
         <h2>Probablement es tracti d'un problema de permisos. '$fich_bil'.</h2><br>
	 <br><br>
         </center>");
	 
     	echo "<div class=\"area\">\n";
	echo "<br/><a style=\"text-align: center\" href=\"/prototype/webform\" >New word</a><br/>\n"; 
        echo "<h3>Thanks and see you soon!</h3>\n";      
        echo "<p>Your entries will be validated and added to Apertium as soon as possible</p>\n";
        echo "<input type=\"button\" value=\"Close\" onclick=\"cierra()\"><br/><br/>\n";
	echo "</div>\n";
     
	
	
        echo "<!-- ##### Footer ##### -->\n"; 

        echo "<div id=\"footer\">\n"; 
  
        echo "<div><a href=\"http://www.dlsi.ua.es\">\n"; 
        echo "Departament de Llenguatges i Sistemes Informàtics</a>, Universitat\n"; 
        echo " d'Alacant, 2006 | Last modified 30.02.2006<!-- by <a\n"; 
        echo "href=\"mailto:sortiz@dlsi.ua.es\" title=\"Envíame un correo\">Sergio\n"; 
        echo "Ortiz</a> -->\n"; 
        echo "</div>\n"; 
      
       echo "</div>\n";     
      
 
   
?>
</body>
</html>
