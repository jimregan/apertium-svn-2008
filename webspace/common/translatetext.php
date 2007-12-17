<?php
	include_once("config/apertium-config.php");
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
	
	print "<h3>" . _("Translation") . "</h3>";
	
	print '<p class="transresult">' . $trad . '</p>';
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox, $dir) {
	print '<form class="translation" action="' . $_SERVER[PHP_SELF] . '?id=translatetext" method="post">';
	print '<fieldset><legend></legend><label for="direction">';
	print _("Translation type:");
	
	print ' <select id="direction" name="direction" title="' . _("Select the translation type") . '">';

	include_once("available_pairs.php");
	
	print '</select></label><br/><br/><label for="textbox">';
	print '<textarea id="textbox" name="textbox" cols="50" rows="10" title="' . _("Insert plain text to translate here") . '">';

	$text = stripslashes($textbox);
	print $text;

	print '</textarea></label><br/><label for="mark">';
	print _("Mark unknown words") . " ";
	print '<input id="mark" value="1" name="mark" type="checkbox" title="Check the box to mark unknown words"/></label>';
	print '<div><input type="submit" value="' . _("Translate") . '" class="submit" title="' . _("Press button to translate") . '"/>';
	print '<input type="reset" value="' . _("Reset") . '" class="reset" title="Press button to reset form"/>';
	print '</div></fieldset></form>';
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
  
  if ($dir == "ro-es" ) {
	$cmd = "cat $tempfile | LANG=en_GB.UTF-8 sed 's/ţ/ț/g' | LANG=en_GB.UTF-8 sed 's/ş/ș/g' | LANG=en_GB.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $dir";
  } else {
  	$cmd = "LANG=es_ES.UTF-8 $APERTIUM_TRANSLATOR -f txt $markUnknown $dir $tempfile";
  	}  
  
	$trad = shell_exec($cmd);
	$trad = replaceUnknown($trad);
	$trad = str_replace("\n","<br/>\n", $trad);
	
	unlink($tempfile);
	
	return $trad;
}


?>
