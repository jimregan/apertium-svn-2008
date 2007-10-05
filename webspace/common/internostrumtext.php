<?php
include_once("config/apertium-config.php");
?>

<?php
// if content from textbox was sent for translation
if (array_key_exists('textbox',$_POST)) {
	process_form();
} else {
// otherwise: show the translation textbox
	show_form("","");
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

	show_form($text, $dir);
		
	if($val == 1) {
		if ($dir == "es-ca") {
			$dir = "es-va";		
		}
	}

		
	$trad = translate($text, $dir, $markUnknown);
	
	print "<h3>Traducció</h3>";
	
	print '<p>' . $trad . '</p>';
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox, $dir) {
print<<<_HTML_
<form class="translation" action="$_SERVER[PHP_SELF]?id=internostrum" method="post">

<fieldset>
	<legend></legend>
		<label for="direction">Sentit&nbsp;
			<select id="direction" name="direction" title="Select the translation type">
_HTML_;
print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">Espanyol &rarr; Català</option>";
			print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">Català &rarr; Español</option>";
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
		<label for="val">
		<input id="val" value="1" name="val" type="checkbox" title="Dóna prioritat a les formes valencianes">
		Dóna prioritat a les <b>formes valencianes</b>			
		</label>
		<br/>
		<label for="mark">
		<input id="mark" value="1" name="mark" type="checkbox" title="Marca les paraules desconegudes"/>
		Marca les paraules desconegudes			
		</label>
	</fieldset>
	<div>
		<input type="submit" value="Tradueix" class="submit" title="Tradueix "/>
		<input type="reset" value="Reestableix" class="reset" title="Reestableix"/>
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
  
	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -d /home/ebenimeli/archive/internostrum -f txt $markUnknown $dir $tempfile";

	$trad = shell_exec($cmd);
	$trad = replaceUnknown($trad);
	$trad = str_replace("\n","<br/>\n", $trad);
	
	unlink($tempfile);
	
	return $trad;
}

?>
