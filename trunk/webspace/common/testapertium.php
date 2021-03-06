<?php
include_once("config/apertium-config.php");

// apertium-config.php: $APERTIUM_TRANSLATOR, $LING_DATA_DIR, etc.
?>

<?php
// if content from textbox was sent for translation
if (array_key_exists('textbox',$_POST)) {
	process_form();
} else {
// otherwise: show the translation textbox
	show_form("");
}
?>

<?php
/*
	**************************
	   PROCESS FORM
	**************************
*/
function process_form() {
	$text = $_POST['textbox'];
	$dir = $_POST['direction'];
	$mark = $_POST['mark'];


	if($mark == 1) {
		$markUnknown = "";
	} else {
		$markUnknown = "-u";
	}

	show_form($text);

		
	$trad = translate($text, $dir, $markUnknown);
	
	print "<h3>Translation</h3>";
	
	print '<p>' . $trad . '</p>';
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox) {
print<<<_HTML_

<form class="translation" action="$_SERVER[PHP_SELF]?id=test" method="post">

<fieldset>
	<legend></legend>
		<label for="direction">Translation type:
			<select id="direction" name="direction" title="Select the translation type">
				<option value="es-ca">Spanish-Catalan</option>
				<option value="ca-es">Catalan-Spanish</option>
				<!--
				<option value="es-gl">Spanish-Galician</option>
				<option value="gl-es">Galician-Spanish</option>
				-->
				<option value="es-pt">Spanish-Portuguese(pt)</option>
				<option value="pt-es">Portuguese-Spanish</option>
				<option value="es-br">Spanish-Portuguese(br)</option>
				<!--
				<option value="oc-ca">Aranese-Catalan</option>
				<option value="ca-oc">Catalan-Aranese</option>
				-->
				<option value="fr-ca">French-Catalan</option>
				<option value="ca-fr">Catalan-French</option>
				<option value="en-ca">English-Catalan</option>
				<option value="ca-en">Catalan-English</option>
				<!--
				<option value="es-ro">Spanish-Romanian</option>
				<option value="ro-es">Romanian-Spanish</option>
				-->				
			</select>
		</label>
		<br/><br/>
		<label for="textbox">
			<textarea id="textbox" name="textbox" cols="50" rows="10" title="Insert plain text to translate here">
_HTML_;
	$text = stripslashes($textbox);
	print $text;
print<<<_HTML_
</textarea>
		</label>
		<br/>
		<label for="mark">Mark unknown words:
			<input id="mark" value="1" name="mark" type="checkbox" title="Check the box to mark unknown words"/>
		</label>

		<!--
		<br/><br/>
		<label>Output:</label>
		<div id="contenedor" style="font-weight:bold"/>
		-->
	</fieldset>
	<div>
		<input type="submit" value="Translate" class="submit" title="Press button to translate"/>
		<input type="reset" value="Reset" class="reset" title="Press button to reset form"/>
	</div>
</form>
_HTML_;
}

/*
  **************************
	   GET DATA PAIR
	**************************
*/
function getDataPair($dir) {
	switch ($dir){
  	case "es-ca": case "ca-es": $datapair="es-ca"; break;
  	case "es-gl": case "gl-es": $datapair="es-gl"; break;
  	case "es-pt": case "pt-es": $datapair="es-pt-br"; break;
  	case "es-br": case "br-es": $datapair="es-pt-br"; break;
  	case "oc-ca": case "ca-oc": $datapair="oc-ca"; break; 
  	case "fr-ca": case "ca-fr": $datapair="fr-ca"; break;
  	case "en-ca": case "ca-en": $datapair="en-ca"; break;
  	case "es-ro": case "ro-es": $datapair="es-ro"; break;   
  	default: $datapair="es-ca"; break;
 	}
 	return $datapair;
}

/*
  *************************************
	   REPLACE UNKNOWN WORDS WITH LINKS
  *************************************
*/
function replaceUnknown($trad) {
	$result = "";
	for($i = 0; $i < strlen($trad); $i++) {
	if($trad[$i]=='*') {
      $result = $result.'<span style="color:red;"><a href="http://xixona.dlsi.ua.es/apertium/webform/index.php?direccion='.$direccion.'&word=';
      $myword ="";
      for(; $trad[$i] != ' ' && $trad[$i] != '\n' && $i < strlen($trad); $i++)
      {
        $myword = $myword.$trad[$i];
      }
      $result .= substr($myword,1).'">'.$myword."</a></span>".$trad[$i];
    }
    else
    {
      $result.=$trad[$i];
    }
  }
  return $result;
}

/*
  **************************
	   TRANSLATE
	**************************
*/
function translate($text, $dir, $markUnknown) {
	// Imports global vars (from config/apertium-config.php)
	global $APERTIUM_TRANSLATOR;
	global $LING_DATA_DIR;
	
	$text = stripslashes($text);
	$datapair = getDataPair($dir);
	$tempfile = tempnam("tmp","tradtext");
	
  $fd = fopen($tempfile,"w");
  fputs($fd, $text);
  fclose($fd);
  
	//$cmd="$APERTIUM_TRANSLATOR $LING_DATA_DIR/apertium-$datapair $dir $type $tempfile";
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $type $dir $tempfile";

	$trad = shell_exec($cmd);
		$trad = replaceUnknown($trad);
	$trad = str_replace("\n","<br/>\n", $trad);
	
	unlink($tempfile);
	
	return $trad;
}

include_once("common/translatedoc.php");

?>
