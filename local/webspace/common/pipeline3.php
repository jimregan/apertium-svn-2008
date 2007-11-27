<?php
include_once("config/apertium-config.php");
?>

<?php
// if content from textbox was sent for translation
if (array_key_exists('textbox',$_POST)) {
	process_form();
} else {
// otherwise: show the translation textbox
	show_form("","","","");
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
	$dir1 = $_POST['direction1'];
	$dir2 = $_POST['direction2'];
	$dir3 = $_POST['direction3'];
			
	$mark = $_POST['mark'];

	if($mark == 1) {
		$markUnknown = "";
	} else {
		$markUnknown = "-u";
	}

	show_form($text,$dir1,$dir2,$dir3);
		
	$trad = translate3($text, $dir1, $dir2, $dir3, $markUnknown);
	
	print "<h3>Translation</h3>";
	
	print '<p>' . $trad . '</p>';
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox, $dir1, $dir2, $dir3) {
print<<<_HTML_
<form class="translation" action="$_SERVER[PHP_SELF]?id=pipeline3" method="post">

<fieldset>
	<legend></legend>
		<label for="direction1">A &rarr; B&nbsp;
			<select id="direction1" name="direction1" title="Select the translation type for A-B">
_HTML_;
print_language_pairs($dir1);
print<<<_HTML_
			</select>
		</label>

		<br/>
		<label for="direction2">B &rarr; C&nbsp;
			<select id="direction2" name="direction2" title="Select the translation type for B-C">
_HTML_;
print_language_pairs($dir2);
print<<<_HTML_
			</select>
		</label>

		<br/>
		<label for="direction3">C &rarr; D&nbsp;
			<select id="direction3" name="direction3" title="Select the translation type for C-D">
_HTML_;
print_language_pairs($dir3);
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
	</fieldset>
	<div>
		<input type="submit" value="Translate" class="submit" title="Press button to translate"/>
		<input type="reset" value="Reset" class="reset" title="Press button to reset form"/>
	</div>
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


function translate3($text, $dir1, $dir2, $dir3, $markUnknown) {
	// Imports global vars (from config/apertium-config.php)
	global $APERTIUM_TRANSLATOR;
	global $LING_DATA_DIR;
	
	$text = stripslashes($text);
	$tempfile = tempnam("tmp","tradtext");
	
  $fd = fopen($tempfile,"w");
  fputs($fd, $text);
  fclose($fd);
  
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $dir1 $tempfile";

	$trad1 = shell_exec($cmd);
	
	$text = stripslashes($trad1);
	$tempfile = tempnam("tmp","tradtext");
	
  $fd = fopen($tempfile,"w");
  fputs($fd, $text);
  fclose($fd);
  
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $dir2 $tempfile";

	$trad2 = shell_exec($cmd);

	$text = stripslashes($trad2);
	$tempfile = tempnam("tmp","tradtext");
	
  $fd = fopen($tempfile,"w");
  fputs($fd, $text);
  fclose($fd);
  
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $dir3 $tempfile";

	$trad3 = shell_exec($cmd);
 	
	$trad3 = replaceUnknown($trad3);
	$trad3 = str_replace("\n","<br/>\n", $trad3);
	
	unlink($tempfile);
	
	return $trad3;
}

/*
  **************************
	   PRINT LANGUAGE PAIRS
	**************************
*/
function print_language_pairs($dir) {
print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">Spanish - Catalan</option>";
			print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">Catalan - Spanish</option>";
			print "<option value='es-pt' " . ($dir == 'es-pt' ? ' selected=true' : '') . ">Spanish - Portuguese</option>";
			print "<option value='pt-es' " . ($dir == 'pt-es' ? ' selected=true' : '') . ">Portuguese - Spanish</option>";
			print "<option value='es-pt_BR' " . ($dir == 'es-pt_BR' ? ' selected=true' : '') . ">Spanish - Brazilian Portuguese</option>";
			print "<option value='en-ca' " . ($dir == 'en-ca' ? ' selected=true' : '') . ">English - Catalan</option>";
			print "<option value='ca-en' " . ($dir == 'ca-en' ? ' selected=true' : '') . ">Catalan - English</option>";
			//print "<option disabled='true' value='fr-ca' " . ($dir == 'fr-ca' ? ' selected=true' : '') . ">French - Catalan</option>";
			//print "<option disabled='true' value='ca-fr' " . ($dir == 'ca-fr' ? ' selected=true' : '') . ">Catalan - French</option>";		
}

?>
