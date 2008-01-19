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
	$trace = $_POST['trace'];	

	if($mark == 1) {
		$markUnknown = "";
	} else {
		$markUnknown = "-u";
	}

	show_form($text, $dir);
	
	$trad = translate($text, $dir, $markUnknown, $trace);
	
	print "<h3>" . _("Translation") . "</h3>";
	
	print '<p class="transresult">' . $trad . '</p>';
}

/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($textbox, $dir) {
	print '<form class="translation" action="' . $_SERVER[PHP_SELF] . '?id=es-en-alpha" method="post">';
	print '<fieldset><legend></legend><label for="direction">';
	print _("Translation type:");
	
	print ' <select id="direction" name="direction" title="' . _("Select the translation type") . '">';

	//include_once("available_pairs.php");
	print "<option value='es-en' " . ($dir == 'es-en' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("English") . "</option>";
	print "<option value='en-es' " . ($dir == 'en-es' ? ' selected=true' : '') . ">" . _("English") . " &rarr; " . _("Spanish") . "</option>";	
	
	print '</select></label><br/><br/><label for="textbox">';
	print '<textarea id="textbox" name="textbox" cols="50" rows="10" title="' . _("Insert plain text to translate here") . '">';

	$text = stripslashes($textbox);
	print $text;

	print '</textarea></label><br/>';
	print '<label for="mark">';
	print _("Mark unknown words") . " ";
	print '<input id="mark" disabled="true" value="1" name="mark" type="checkbox" title="Check the box to mark unknown words"/></label>';
	print '<br/>';

	print '<label for="trace">';
	print _("Print trace") . " ";
	print '<input id="mark" value="1" name="trace" type="checkbox" title="Check the box to print a trace"/></label>';
	print '<br/><br/>';
	
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
function translate($text, $dir, $markUnknown, $trace) {
	// Imports global vars (from config/apertium-config.php)
	//global $APERTIUM_TRANSLATOR;
	//global $LING_DATA_DIR;
	
	$text = stripslashes($text);
	//$tempfile = tempnam("/home/ebenimeli/transducens/apertium-es-ca-en/dix","tradtext");
	//$tempfile = "/home/ebenimeli/transducens/apertium-es-ca-en/dix/trad.tmp";
	
	//$fd = fopen($tempfile,"w");
	//fputs($fd, $text);
	//fclose($fd);

	$DIRECTORY = "/home/ebenimeli/transducens/apertium-es-ca-en/dix";
	
	if ($trace == 1) {
		print_trace($text, $dir, $DIRECTORY);
	}

	//$cmd = "LANG=es_ES.UTF-8 echo \"" . $text . "\" | " . $DIRECTORY . "/translate-pipeline-" . $dir. "-nodesamb.sh";
	//$trad = shell_exec($cmd);
	//print "Trad (no desambig.):";
	//print $trad;	  	
	
  	$cmd = "LANG=es_ES.UTF-8 echo \"" . $text . "\" | " . $DIRECTORY . "/translate-pipeline-" . $dir. ".sh";
  	//print_HTML($cmd);
	$trad = shell_exec($cmd);
	
	  	
	//$trad = replaceUnknown($trad);
	//$trad = str_replace("\n","<br/>\n", $trad);
	//unlink($tempfile);
	return $trad;
}

function print_HTML($text) {
	print '<div class="trace">';
	$text_mod = utf8_decode($text);
	$text_mod = htmlentities($text_mod);
	print $text_mod;
	print "<br/><br/>";		
	print '</div>';
}

function print_trace($text, $dir, $DIRECTORY) {
	if( $dir == "es-en" ) {
		$morf_cmd = "LANG=es_ES.UTF-8 echo \"" . $text . "\" | LANG=es_ES.UTF-8 lt-proc " . $DIRECTORY . "/es-en.automorf.bin";
		$res_morf = shell_exec($morf_cmd);
		print_HTML($res_morf);
	
		$desambig_cmd = "LANG=es_ES.UTF-8 echo \"" . $res_morf . "\" | LANG=es_ES.UTF-8 apertium-tagger -g " . $DIRECTORY ."/es-ca.prob";
		$res_desambig = shell_exec($desambig_cmd);
		print_HTML($res_desambig);
						
		//$gawk_cmd = "LANG=es_ES.UTF-8 gawk 'BEGIN{RS=\"$\"; FS=\"/\";}{nf=split(\$1,COMPONENTS,\"^\"); for(i = 1; i<nf; i++) printf COMPONENTS[i]; if(\$2 != \"\") printf(\"^%s\$\",\$2);}'";
		//$select_first_cmd = "LANG=es_ES.UTF-8 echo \"" . $res_morf . "\" | " . $gawk_cmd;
		//$res_select_first = shell_exec($select_first_cmd);
		//print_HTML($res_select_first);

		$transfer_cmd = "LANG=es_ES.UTF-8 echo \"" . $res_desambig . "\" | LANG=es_ES.UTF-8 apertium-transfer " . $DIRECTORY . "/trules-es-en.xml " . $DIRECTORY . "/trules-es-en.bin " . $DIRECTORY . "/es-en.autobil.bin";
		$res_transfer = shell_exec($transfer_cmd);
		print_HTML($res_transfer);

		$gen_cmd = "LANG=es_ES.UTF-8 echo \"" . $res_transfer . "\" | LANG=es_ES.UTF-8 lt-proc -g " . $DIRECTORY . "/en-es.autogen.bin";
		$res_gen = shell_exec($gen_cmd);
		//print_HTML($res_gen);
	}

}


?>
