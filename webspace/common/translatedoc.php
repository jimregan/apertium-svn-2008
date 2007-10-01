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
function process_form_doc() {
	$dir = $_POST["direction"];
  $mark = $_POST["mark"];
  $doctype = $_POST["doctype"];


	if($mark == 1) {
		$markUnknown = "";
	} else {
		$markUnknown = "-u";
	}

	show_form("");
		
	$trad = translate($doctype, $dir, $markUnknown);
	
	print "<h3>Translation</h3>";
	
	print '<p>' . $trad . '</p>';
}



/*
  **************************
	   SHOW FORM
	**************************
*/
function show_form($text) {
print<<<_HTML_
<form class="translation" enctype="multipart/form-data" action="$_SERVER[PHP_SELF]?id=translatedoc" method="post">
	<fieldset>
	<legend></legend>
	
	<label for="direction">Translation:
	<select id="direction" name="direction" title="Select the translation type">
		<option value="es-ca">Spanish-Catalan</option>
		<option value="ca-es">Catalan-Spanish</option>
		<option value="es-gl">Spanish-Galician</option>
		<option value="gl-es">Galician-Spanish</option>
		<option value="es-pt">Spanish-Portuguese(pt)</option>
		<option value="pt-es">Portuguese-Spanish</option>
		<option value="es-br">Spanish-Portuguese(br)</option>
		<option value="oc-ca">Catalan-Aranese</option>
		<option value="ca-oc">Aranese-Catalan</option>
		<option value="fr-ca">Catalan-French</option>
		<option value="ca-fr">French-Catalan</option>
		<option value="ca-en">Catalan-English</option>
		<option value="en-ca">English-Catalan</option>
	</select>
	</label>
	
	<label for="doctype">Document type:
	<select id="doctype" name="doctype" title="Select the document type to be translated">
		<option value="txt" title="Text">Text</option>
		<option value="html" title="HyperText Markup Language">HTML</option>
		<option value="rtf" title="Rich Text Format">RTF</option>
		<option value="odt" title="OpenDocument">ODT</option>
		<option value="sxw" title="OpenOffice.org Writer">SXW</option>
	</select>
	</label>
	<br/><br/>
	
	<label>Document:
		<input type="hidden" name="MAX_FILE_SIZE" value="10000000"/>
		<input title="Select the file to be translated" name="userfile" type="file"/>
	</label>
	
	<br/>
	<label for="mark">Mark unknown words:
		<input  name="mark" value="1" type="checkbox" title="Select the box to mark unknown words"/>
	</label>
	</fieldset>
	
	<div>
		<input type="submit" value="Translate" class="submit" title="Clic here to translate"/>
		<input type="reset" value="Reset" class="reset" title="Clic here to reset"/>
	</div>
	
	</form>
_HTML_;
}

/*
  **************************
	   TRANSLATE DOCUMENT
	**************************
*/
function translate($doctype, $dir, $markUnknown) {
	global $APERTIUM_TRANSLATOR;
	
	$content_type = getContentType($doctype);
	$download_filetype = getDownloadFileType($doctype);

  if($markUnknown != 1) {
    $doctype = $doctype."u";
  }

  if($_FILES['userfile']['size']>16384*4*4*4*4){
    echo "FILE IS TOO BIG";
    exit;
  }

	$tempfile = tempnam("/tmp","translate");
	$tempfile = $tempfile . "." . $doctype;

	$datapair = getDataPair($dir);

  if($datapair == "en-ca") {
  	$program="/home/fran/svnroot/local/stable/bin/apertium";
  	# /home/sortiz/apertium-$nomtrad $direccion $tipo $userfile $archivo2";
  } else {
  	$program="/home/fran/svnroot/local/stable/bin/apertium-translator";
  	# /home/sortiz/apertium-$nomtrad $direccion $tipo $userfile $archivo2";
  }

	//$program = $APERTIUM_TRANSLATOR;  

  if($doctype == "sxw" || $doctype == "odt" || $doctype == "odtu" || $doctype == "sxwu" ||
     $doctype == "doc" || $doctype == "docu")
  {
    $program = $program."-ooffice";
  }

  $cmd = $program." /home/fran/svnroot/archive/apertium-$datapair $dir $type ".$_FILES['userfile']['tmp_name']." ".$tempfile;
  //$cmd = $program." $dir $type ".$_FILES['userfile']['tmp_name']." ".$tempfile;
  
  $str = shell_exec($cmd);

  header('Content-Type: '.$content_type);
  header('Content-Disposition: attachment; filename="downloaded.'.$download_filetype.'"');
  header('Content-length: '.filesize($tempfile));
  readfile($tempfile);
  unlink ($tempfile);
}

/*
  **************************
	   GET CONTENT_TYPE
	**************************
*/
function getContentType($doctype) {
  switch($doctype)
  {
    case "txt":
      $content_type = "text/plain";
      break;
    case "rtf":
      $content_type = "text/rtf";
      break;
    case "html":
      $content_type = "text/html";
      break;
    case "odt":
      $content_type = "application/vnd.oasis.opendocument.text";
      break;
    case "sxw":
      $content_type = "application/vnd.sun.xml.writer";
      break;
    case "doc":
      $content_type = "application/rtf";
      break;
  }
  return $content_type;
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
		default: $datapair="es-ca"; break;
 	}
 	return $datapair;
}

/*
  **************************
	   GET DOWNLOAD FILE TYPE
	**************************
*/
function getDownloadFileType($doctype) {
  if($doctype != "doc") {
  	$download_filetype = $doctype;
  } else {
  	$download_filetype = "rtf";
  }
  return $download_filetype;
}

?>
