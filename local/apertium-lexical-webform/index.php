<?php
    include "private/config.php";
?>
<!-- -*-nxml-*- -->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="es">
  <head>
    <meta http-equiv="content-type" content="application/xhtml+xml; charset=ISO-8859-15" />
    <meta name="author" content="haran" />
    <meta name="generator" content="haran" />

    <!-- Navigational metadata for large websites (an accessibility feature): -->
    <link rel="top"      href="./index.php" title="Homepage" />
    <link rel="up"       href="./index.php" title="Up" />
    <link rel="first"    href="./index.php" title="First page" />
    <link rel="previous" href="./index.php" title="Previous page" />
    <link rel="next"     href="./index.php" title="Next page" />
    <link rel="last"     href="./index.php" title="Last page" />
    <link rel="toc"      href="./index.php" title="Table of contents" />
    <link rel="index"    href="./index.php" title="Site map" />

    <link rel="stylesheet" type="text/css" href="/styles/gila-screen-form.css" media="screen" title="Gila (screen)" />
    <link rel="stylesheet" type="text/css" href="/styles/gila-print.css" media="print" />

    <title>apertium insertion form</title> 
    
    <script type="text/javascript">
	<!--
	function inserta(area,valor) {
  
  	var contenido = area.value;
  	var resultado="";
  	var cadena="";
 
  	if(valor.charAt(0)==' ') {
    	cadena=valor.charAt(1);
  	}
  	else {
    	cadena=valor;
  	}
  	resultado=contenido.concat("",cadena);
  	area.value=resultado;
  	area.focus();
     }

	
      function submit_continuar(accion) {
	document.forms[0].action=accion;
	document.forms[0].submit;
	return true;
        	}
	// -->
		
	</script>
    
  </head>
 
  <body>
  
  
   <!-- For non-visual user agents: -->
      <div id="top"><a href="#nrc-main-copy" class="doNotDisplay doNotPrint">Skip to main content.</a></div>

    <!-- ##### Header ##### -->

     <div id="header">
       <h1 class="headerTitle">
         <a href="." title="Browse to homepage"><span>OpenTrad apertium</span> insertion form</a>
       </h1>
       
      <div class="subHeader">

<!--        <span class="doNotDisplay">Navigation: </span>
        <a href="./index.php">Text</a> |
        <a href="./index.php">Documents</a> |
        <a href="./index.php">Surf</a>
-->
      </div>
       
     </div>
     
     
      <div id="side-bar">

      <!-- ##### Left Sidebar ##### -->

      <div class="leftSideBar">
      
       <p class="sideBarTitle">In this page</p>
        <ul>
          <li><a href="#es-ca" title="Spanish/Catalan insertion form">Spanish/Catalan</a></li>
          <li><a href="#es-pt" title="Spanish/Portuguese insertion form">Spanish/Portuguese</a></li>
          <li><a href="#oc-ca" title="Aranese Occitan/Catalan insertion form">Aranese Occitan /Catalan</a></li>
          <li><a href="#es-ro" title="Spanish/Romanian insertion form">Spanish/Romanian</a></li>
          <li><a href="#fr-ca" title="French/Catalan insertion form">French/Catalan</a></li>
          <li><a href="#fr-es" title="French/Spanish insertion form">French/Spanish</a></li>
	</ul>   
<?php
    echo '<p class="sideBarTitle"><a href="mailto:'.$mail.'">If you want to help us adding words, mail us.</a></p><br/>';
?>
	  <p class="sideBarTitle"><a href="http://apertium.sourceforge.net/">More information</a></p><br/>
	  <p class="sideBarTitle"><a href="http://sourceforge.net/projects/apertium/">Downloads</a></p>
      </div>
      
     </div>
    <!-- ##### Main copy ##### -->
    
    
     <div id="nrc-main-copy">
     
          <div><a href="../terms.html">Terms of use</a></div>

       <h1 id="es-ca" style="border-top: 0; padding-top: 0;">Spanish/Catalan</h1>
         <form action="./private/resultado.php" target="_blank" method="post" onsubmit="return 0;">
	   <fieldset>
             <legend></legend>    	   
	   	<h3>Lemma equivalence</h3>
	   	

		<input type="hidden" name="user" id="user" value="<?php getenv('REMOTE_USER')?>" />
		<input type="hidden" name="nomtrad" id="nomtrad" value="es-ca"/>
		   <p>Spanish <?php
$myword = "";
if(isset($word) && $direccion == "es-ca")
{
  $myword = $word;
}
echo '<input type="text" name="LRlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
		   <p>Catalan  <?php
$myword = "";
if(isset($word) && $direccion == "ca-es")
{
  $myword = $word;
}
echo '<input type="text" name="RLlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
     	 	<h3>Part-of-speech tag</h3>
		   <ul>	 
	 		<li><input type="radio" name="cat" value="Noun"/> Noun</li>
	 		<li><input type="radio" name="cat" value="Adjective"/> Adjective</li>
	 		<li><input type="radio" name="cat" value="Acr"/> Acronym</li>
			<li><input type="radio" name="cat" value="Prep"/> Preposition</li>
	 		<li><input type="radio" name="cat" value="Verb"/> Verb</li>
	 		<li><input type="radio" name="cat" value="MW"/> Multiword verb</li>
	 		<li><input type="radio" name="cat" value="Pname"/> Proper name</li>
			<li><input type="radio" name="cat" value="Cnjadv"/> Adverbial conjunction</li>
	 		<li><input type="radio" name="cat" value="Adverb"/> Adverb </li>
		   </ul>
		
		<input type="submit" 
	               value="Go on" 
		       class="submit"
		       title="Press button to continue"
		       onclick="return submit_continuar('./private/resultado.php');"/>
		       
	  </fieldset>
       </form>
       
       <h1 id="es-pt" style="border-top: 0; padding-top: 0;">Spanish/Portuguese</h1>
         <form action="./private/resultado.php" target="_blank" method="post" onsubmit="return 0;">
	    <fieldset>
              <legend></legend> 
	        <input type="hidden" name="nomtrad" value="es-pt"/>
	   	<h3>Lemma equivalence</h3>	 

		   <p>Spanish <?php
$myword = "";
if(isset($word) && $direccion == "es-pt")
{
  $myword = $word;
}
echo '<input type="text" name="LRlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
		   <p>Portuguese  <?php
$myword = "";
if(isset($word) && $direccion == "pt-es")
{
  $myword = $word;
}
echo '<input type="text" name="RLlem" maxlength="250" value="'.$myword.'"/>';
?>
		</p>
		<h3>Part-of-speech tag</h3>
		   <ul>	 
	 	 	<li><input type="radio" name="cat" value="Noun"/> Noun</li>
	 		<li><input type="radio" name="cat" value="Adjective"/> Adjective</li>
	 		<li><input type="radio" name="cat" value="Acr"/> Acronym</li>
			<li><input type="radio" name="cat" value="Prep"/> Preposition</li>
	 		<li><input type="radio" name="cat" value="Verb"/> Verb</li>
	 		<li><input type="radio" name="cat" value="MW"/> Multiword verb</li>
			<li><input type="radio" name="cat" value="Pname"/> Proper name</li>
			<li><input type="radio" name="cat" value="Cnjadv"/> Adverbial conjunction</li>
	 		<li><input type="radio" name="cat" value="Adverb"/> Adverb </li>
		   </ul>
		
     	 	<input type="submit" 
	               value="Go on" 
		       class="submit"
		       title="Press button to continue"
		       onclick="return submit_continuar('./private/resultado.php');"/>
		       
	  </fieldset> 
       </form>

      
       <h1 id="oc-ca" style="border-top: 0; padding-top: 0;">Aranese Occitan/Catalan</h1>
         <form action="./private/resultado.php" target="_blank" method="post" onsubmit="return 0;">
	   <fieldset>
             <legend></legend>    	   
	   	<h3>Lemma equivalence</h3>
	   	

		<input type="hidden" name="user" id="user" value="<?php getenv('REMOTE_USER')?>" />
		<input type="hidden" name="nomtrad" id="nomtrad" value="oc-ca"/>
		   <p>Aranese Occitan <?php
$myword = "";
if(isset($word) && $direccion == "oc-ca")
{
  $myword = $word;
}
echo '<input type="text" name="LRlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
		   <p>Catalan  <?php
$myword = "";
if(isset($word) && $direccion == "ca-oc")
{
  $myword = $word;
}
echo '<input type="text" name="RLlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
     	 	<h3>Part-of-speech tag</h3>
		   <ul>	 
	 		<li><input type="radio" name="cat" value="Noun"/> Noun</li>
	 		<li><input type="radio" name="cat" value="Adjective"/> Adjective</li>
	 		<li><input type="radio" name="cat" value="Acr"/> Acronym</li>
			<li><input type="radio" name="cat" value="Prep"/> Preposition</li>
	 		<li><input type="radio" name="cat" value="Verb"/> Verb</li>
	 		<li><input type="radio" name="cat" value="MW"/> Multiword verb</li>
	 		<li><input type="radio" name="cat" value="Pname"/> Proper name</li>
			<li><input type="radio" name="cat" value="Cnjadv"/> Adverbial conjunction</li>
	 		<li><input type="radio" name="cat" value="Adverb"/> Adverb </li>
		   </ul>
		
		<input type="submit" 
	               value="Go on" 
		       class="submit"
		       title="Press button to continue"
		       onclick="return submit_continuar('./private/resultado.php');"/>
		       
	  </fieldset>
       </form>

       <h1 id="es-ro" style="border-top: 0; padding-top: 0;">Spanish/Romanian</h1>
         <form action="./private/resultado.php" target="_blank" method="post" onsubmit="return 0;">
	    <fieldset>
              <legend></legend> 
	        <input type="hidden" name="nomtrad" value="es-ro"/>
	   	<h3>Lemma equivalence</h3>	 

		   <p>Spanish <?php
$myword = "";
if(isset($word) && $direccion == "es-ro")
{
  $myword = $word;
}
echo '<input type="text" name="LRlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
		   <p>Romanian  <?php
$myword = "";
if(isset($word) && $direccion == "ro-es")
{
  $myword = $word;
}
echo '<input type="text" name="RLlem" maxlength="250" value="'.$myword.'"/>';
?>
		</p>
		<h3>Part-of-speech tag</h3>
		   <ul>	 
	 	 	<li><input type="radio" name="cat" value="Noun"/> Noun</li>
	 		<li><input type="radio" name="cat" value="Adjective"/> Adjective</li>
	 		<li><input type="radio" name="cat" value="Acr"/> Acronym</li>
			<li><input type="radio" name="cat" value="Prep"/> Preposition</li>
	 		<li><input type="radio" name="cat" value="Verb"/> Verb</li>
	 		<li><input type="radio" name="cat" value="MW"/> Multiword verb</li>
			<li><input type="radio" name="cat" value="Pname"/> Proper name</li>
			<li><input type="radio" name="cat" value="Cnjadv"/> Adverbial conjunction</li>
	 		<li><input type="radio" name="cat" value="Adverb"/> Adverb </li>
		   </ul>
		
     	 	<input type="submit" 
	               value="Go on" 
		       class="submit"
		       title="Press button to continue"
		       onclick="return submit_continuar('./private/resultado.php');"/>
		       
	  </fieldset> 
       </form>


     <h1 id="fr-ca" style="border-top: 0; padding-top: 0;">French/Catalan</h1>
         <form action="./private/resultado.php" target="_blank" method="post" onsubmit="return 0;">
	   <fieldset>
             <legend></legend>    	   
	   	<h3>Lemma equivalence</h3>
	   	

		<input type="hidden" name="user" id="user" value="<?php getenv('REMOTE_USER')?>" />
		<input type="hidden" name="nomtrad" id="nomtrad" value="fr-ca"/>
		   <p>French <?php
$myword = "";
if(isset($word) && $direccion == "fr-ca")
{
  $myword = $word;
}
echo '<input type="text" name="LRlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
		   <p>Catalan  <?php
$myword = "";
if(isset($word) && $direccion == "ca-fr")
{
  $myword = $word;
}
echo '<input type="text" name="RLlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
     	 	<h3>Part-of-speech tag</h3>
		   <ul>	 
	 		<li><input type="radio" name="cat" value="Noun"/> Noun</li>
	 		<li><input type="radio" name="cat" value="Adjective"/> Adjective</li>
	 		<li><input type="radio" name="cat" value="Acr"/> Acronym</li>
			<li><input type="radio" name="cat" value="Prep"/> Preposition</li>
	 		<li><input type="radio" name="cat" value="Verb"/> Verb</li>
	 		<li><input type="radio" name="cat" value="MW"/> Multiword verb</li>
	 		<li><input type="radio" name="cat" value="Pname"/> Proper name</li>
			<li><input type="radio" name="cat" value="Cnjadv"/> Adverbial conjunction</li>
	 		<li><input type="radio" name="cat" value="Adverb"/> Adverb </li>
		   </ul>
		
		<input type="submit" 
	               value="Go on" 
		       class="submit"
		       title="Press button to continue"
		       onclick="return submit_continuar('./private/resultado.php');"/>
		       
	  </fieldset>
       </form>
       
       
     <h1 id="fr-es" style="border-top: 0; padding-top: 0;">French/Spanish</h1>
         <form action="./private/resultado.php" target="_blank" method="post" onsubmit="return 0;">
	   <fieldset>
             <legend></legend>    	   
	   	<h3>Lemma equivalence</h3>
	   	

		<input type="hidden" name="user" id="user" value="<?php getenv('REMOTE_USER')?>" />
		<input type="hidden" name="nomtrad" id="nomtrad" value="fr-es"/>
		   <p>French <?php
$myword = "";
if(isset($word) && $direccion == "fr-es")
{
  $myword = $word;
}
echo '<input type="text" name="LRlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
		   <p>Spanish  <?php
$myword = "";
if(isset($word) && $direccion == "es-fr")
{
  $myword = $word;
}
echo '<input type="text" name="RLlem" maxlength="250" value="'.$myword.'"/>';
?>
		   </p>
     	 	<h3>Part-of-speech tag</h3>
		   <ul>	 
	 		<li><input type="radio" name="cat" value="Noun"/> Noun</li>
	 		<li><input type="radio" name="cat" value="Adjective"/> Adjective</li>
	 		<li><input type="radio" name="cat" value="Acr"/> Acronym</li>
			<li><input type="radio" name="cat" value="Prep"/> Preposition</li>
	 		<li><input type="radio" name="cat" value="Verb"/> Verb</li>
	 		<li><input type="radio" name="cat" value="MW"/> Multiword verb</li>
	 		<li><input type="radio" name="cat" value="Pname"/> Proper name</li>
			<li><input type="radio" name="cat" value="Cnjadv"/> Adverbial conjunction</li>
	 		<li><input type="radio" name="cat" value="Adverb"/> Adverb </li>
		   </ul>
		
		<input type="submit" 
	               value="Go on" 
		       class="submit"
		       title="Press button to continue"
		       onclick="return submit_continuar('./private/resultado.php');"/>
		       
	  </fieldset>
	  </form>
   
    </div>    
    
     <!-- ##### Footer ##### -->   
    <div id="footer">
      <div><a href="http://www.dlsi.ua.es">
        Departament de Llenguatges i Sistemes Informàtics</a>, Universitat
        d'Alacant, 2006 | Last modified 20.02.2006<!-- by <a
        href="mailto:sortiz@dlsi.ua.es" title="Envíame un correo">Sergio
        Ortiz</a> -->
	
      </div>
    </div>
    
  </body>
</html>
