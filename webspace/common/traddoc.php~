<?php
	process_form();
?>

<?php
/*
	**************************
	   PROCESS FORM
	**************************
*/
function process_form() {
	$dir = $_POST["direction"];
  $mark = $_POST["mark"];
  $doctype = $_POST["doctype"];


	if($mark == 1) {
		$markUnknown = "";
	} else {
		$markUnknown = "-u";
	}

	translate($doctype, $dir, $markUnknown);
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
