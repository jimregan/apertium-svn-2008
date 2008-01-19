<?php
include_once("config/apertium-config.php");
?>

<?php
// if content from textbox was sent for translation
if (array_key_exists('textbox',$_POST)) {
	process_form();
} else {
// otherwise: show the translation textbox
	show_form("","","0","0");
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
	$val = $_POST['val'];

	if($mark == 1) {
		$markUnknown = "";
	} else {
		$markUnknown = "-u";
	}

	show_form($text, $dir, $mark, $val);
		
	if($val == 1) {
		if ($dir == "es-ca") {
			$dir = "es-va";		
		}
	}

		
	$trad = translate($text, $dir, $markUnknown);
	
	print "<h3>" . _("Translation") . "</h3>";
	
	print '<p>' . $trad . '</p>';
	
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox, $dir, $mark, $val) {
print<<<_HTML_
<form class="translation" action="$_SERVER[PHP_SELF]?id=internostrum" method="post">

<fieldset>
	<legend></legend>
_HTML_;
		print '<label for="direction">' . _("Translation type") . ': ';
		print '<select id="direction" name="direction" title="Select the translation type">';
		print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("Catalan") . "</option>";
		print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("Spanish") . "</option>";
print<<<_HTML_
			</select>
		</label>
		<br/><br/>
		<label for="textbox">
_HTML_;
			print '<textarea id="textbox" name="textbox" cols="50" rows="10" title="' . _("Insert plain text to translate here") . '">';
$text = stripslashes($textbox);
print $text;
print<<<_HTML_
</textarea>
		</label>
		<br/>
		<label for="val">
_HTML_;
		print '<input id="val" value="1" ' . ($val == '1' ? 'checked="yes"' : '') . ' name="val" type="checkbox" title="Dóna prioritat a les formes valencianes">';
		print _("Give priority to forms in Valencian") . ' <b><a href="?id=formesvalencianes">[+]</a></b>';
print<<<_HTML_
					
		</label>
		<br/>
		<label for="mark">
_HTML_;
		print '<input id="mark" value="1" ' . ($mark == "1" ? 'checked="yes"' : '') . ' name="mark" type="checkbox" title="Marca les paraules desconegudes"/>';
		print _("Mark unknown words");
print<<<_HTML_
		</label>
		<div>
_HTML_;
		print '<input type="submit" value="' . _("Translate") . '" class="submit" title="Tradueix "/>';
		print '<input type="reset" value="' . _("Reset") . '" class="reset" title="Reestableix"/>';
print<<<_HTML_
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
	global $INTERNOSTRUM_LING_DATA;
	
	$text = stripslashes($text);
	$tempfile = tempnam("tmp","tradtext");
	
  $fd = fopen($tempfile,"w");
  fputs($fd, $text);
  fclose($fd);
  
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -d " . $INTERNOSTRUM_LING_DATA . " -f txt $markUnknown $dir $tempfile";
	
	$trad = shell_exec($cmd);
	$trad = replaceUnknown($trad);
	$trad = str_replace("\n","<br/>\n", $trad);
	
	unlink($tempfile);
	
	return $trad;
}

?>
