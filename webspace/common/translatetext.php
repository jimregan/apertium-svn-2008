<?php
include_once("config/apertium-config.php");
require("config/langpairs.php");
?>

<?php
// if content from textbox was sent for translation
if (array_key_exists('textbox',$_POST)) {
	process_form();
} else {
// otherwise: show the translation textbox
	$dir = $HTTP_GET_VARS["dir"];
	if ($dir == "") {
		$dir = getPair($lang);
	}
	show_form("", $dir);
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

	show_form($text, $dir);
		
	$trad = translate($text, $dir, $markUnknown);
	
	print "<h3>Translation</h3>";
	
	print '<p class="transresult">' . $trad . '</p>';
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox, $dir) {
print<<<_HTML_
<form class="translation" action="$_SERVER[PHP_SELF]?id=translatetext" method="post">

<fieldset>
	<legend></legend>
		<label for="direction">Translation type:
			<select id="direction" name="direction" title="Select the translation type">
_HTML_;
			if ($dir=="") {
				print "<option disabled='true' selected='true'>Select direction...</option>";
			}
			print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">Spanish &rarr; Catalan</option>";
			print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">Catalan &rarr; Spanish</option>";
			print "<option disabled='true'></option>";
			print "<option value='es-gl' " . ($dir == 'es-gl' ? ' selected=true' : '') . ">Spanish &rarr; Galician</option>";
			print "<option value='gl-es' " . ($dir == 'gl-es' ? ' selected=true' : '') . ">Galician &rarr; Spanish</option>";
			print "<option disabled='true'></option>";
			print "<option value='es-pt' " . ($dir == 'es-pt' ? ' selected=true' : '') . ">Spanish &rarr; Portuguese</option>";
			print "<option value='pt-es' " . ($dir == 'pt-es' ? ' selected=true' : '') . ">Portuguese &rarr; Spanish</option>";
			print "<option value='es-pt_BR' " . ($dir == 'es-pt_BR' ? ' selected=true' : '') . ">Spanish &rarr; Brazilian Portuguese</option>";
			print "<option disabled='true'></option>";
			print "<option value='en-ca' " . ($dir == 'en-ca' ? ' selected=true' : '') . ">English &rarr; Catalan</option>";
			print "<option value='ca-en' " . ($dir == 'ca-en' ? ' selected=true' : '') . ">Catalan &rarr; English</option>";
			print "<option disabled='true'></option>";
			print "<option value='fr-ca' " . ($dir == 'fr-ca' ? ' selected=true' : '') . ">French &rarr; Catalan</option>";
			print "<option value='ca-fr' " . ($dir == 'ca-fr' ? ' selected=true' : '') . ">Catalan &rarr; French</option>";		

print<<<_HTML_
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

	<div>
		<input type="submit" value="Translate" class="submit" title="Press button to translate"/>
		<input type="reset" value="Reset" class="reset" title="Press button to reset form"/>
	</div>
		</fieldset>
</form>
_HTML_;

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
	$tempfile = tempnam("tmp","tradtext");
	
  $fd = fopen($tempfile,"w");
  fputs($fd, $text);
  fclose($fd);
  
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $dir $tempfile";

	$trad = shell_exec($cmd);
	$trad = replaceUnknown($trad);
	$trad = str_replace("\n","<br/>\n", $trad);
	
	unlink($tempfile);
	
	return $trad;
}

?>
