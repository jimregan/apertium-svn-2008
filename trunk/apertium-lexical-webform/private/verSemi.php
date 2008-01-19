<?php
    include("config.php"); //archivo de configuración

    if($tipo=="n")
    {
	$cadFich="noun";
	$show=": nouns";
	$tag="<s n=\"n\"/>";
	
    }
    if($tipo=="adj")
    {
	$cadFich="adj";
	$show=": adjectives";
	$tag="<s n=\"adj\"/>";
    }
    if($tipo=="vb")
    {
	$cadFich="verb";
	$show=": verbs";
	$tag="<s n=\"vblex\"/>";
	$nota="";
    }
    
    if($tipo=="prep")
    {
	$cadFich="pr";
	$show=": prepositions";
	$tag="<s n=\"pr\"/>";
    }
    
    if($tipo=="adv")
    {
	$cadFich="adv";
	$show=": adverbs";
	$tag="<s n=\"adv\"/>";
	$nota="";
    }
    
    if($tipo=="cnjadv")
    {
	$cadFich="cnjadv";
	$show=": adverbial conjunctions";
	$tag="<s n=\"cnjadv\"/>";
	$nota="";
    }
    
    if($tipo=="pname")
    {
	$cadFich="pname";
	$show=": proper names";
	$tag="<s n=\"np\"/>";
	$nota="";
    }
 
     if($tipo=="acr")
    {
	$cadFich="acrònims";
	$show=": acronyms";
	$tag="<s n=\"n\"/><s n=\"acr\"/>";
	$nota="";
    }


 
    //Rutas de los diccionarios temporales
	$path="../dics";
	$fichSemi="$path/$LR-$RL.semi.dic";

  
    //Rutas de los dic. compilados
    	$aut_LRRL=$dicos_path."/apertium-$nomtrad/$LR-$RL.automorf.bin";
    	$aut_RLLR=$dicos_path."/apertium-$nomtrad/$RL-$LR.automorf.bin";
	
    
    
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
    <title>Words to be validated</title>

<script language="Javascript">
function cierra() {
   window.close();
}
</script>
</head>
  <body>
    <fieldset> 
<?php

	if($user=="")
	      	$user=$REDIRECT_REMOTE_USER;


$entradas=file($fichSemi);
?>
      <table cellspacing="3em" cols="7" align="center">
	<tr>
    	  <td>Lemma (<?=$LR?>)</td>
    	  <td>Is as</td>
    	  <td>Direction</td>   
    	  <td>Lemma (<?=$RL?>)</td>
    	  <td>Is as</td>
      	  <td>Part-of-Speech</td>
    	  <td>Comments</td>
  	</tr>
<?php


for($cont=0;$cont<count($entradas);$cont++)
{

	$entradas[$cont]=str_replace("\n","",$entradas[$cont]); 	// quita los \n
	$trozos=split("\t",$entradas[$cont]);

	$LRlem=$trozos[0];
	if($LRlem=="$LRlem")
	{	
		if($selecc!=$cont)
		{
			$selecc=$cont;
			$cambio=1;
		}else
			$cambio=0;
	}	
	$RLlem=$trozos[1];
	$paradigmaLR=$trozos[2];
	$direccion=$trozos[3];
	$paradigmaRL=$trozos[4];
	$tipo=$trozos[5];
	$comentarios=$trozos[6];
	$user=$trozos[7];
	$geneLR=$trozos[8];
	$geneRL=$trozos[9];
	$numLR=$trozos[10];
	$numRL=$trozos[11];
	
	if($tipo=="n")
    	{
		$tipo="noun";
        }
	if($tipo=="adj")
	{
		$tipo="adjective";
        }
	if($tipo=="vb")
        {
		$tipo="verb";
	}
	if($tipo=="adv")
	{
		$tipo="adverb";
	}
    
     	if($tipo=="cnjadv")
	{
		$tipo="cnjadv";
	}
	if($tipo=="pname")
	{
		$tipo="pname";
	}
	
	
	if($user==$user){
?>

	<tr>
	  <td><?=$LRlem?></td>
	  <td><?=$paradigmaLR?></td>
	  <td><?=$direccion?></td>
	  <td><?=$RLlem?></td>
	  <td><?=$paradigmaRL?></td>
	  <td><?=$tipo?></td>
	  <td><?=$comentarios?></td>
	</tr>
<?php
              }	     
} 
?>
      </table>
    </fieldset>  

     <div class="area">
       <br/><br/><input type="button" value="Close" onclick="cierra()"><br/><br/>
     </div>
    
    </form>
  </body>
</html>
