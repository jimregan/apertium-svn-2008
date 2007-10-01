<?php
include_once("config/apertium-config.php");
/*
$file = "content/" . $lang . "/translatedoc.php";
if( file_exists($file) ) {
	include_once("content/" . $lang . "/translatedoc.php");
} else {
	if (file_exists("content/en/translatedoc.php") ) {
		include_once("content/en/translatedoc.php");
	}
}
*/
?>

<?php
	show_form("");

/*
  **************************
	   SHOW FORM
  **************************
*/
function show_form($text) {
print<<<_HTML_
<form class="translation" enctype="multipart/form-data" action="common/traddoc.php" method="post">
	<fieldset>
	<legend></legend>
	<table border="0px">		
	<tr>
	<label for="direction">
	<td>Translation:</td>
	<td>
	<select id="direction" name="direction" title="">
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
	</td>
	</label>
	</tr>	
	<tr>
	<label for="doctype">
	<td>Document type:</td>
	<td>		
	<select id="doctype" name="doctype" title="$DOCUMENT">
		<option value="txt" title="Text">Text - Plain text</option>
		<option value="html" title="HyperText Markup Language">HTML - HyperText Markup Language</option>
		<option value="rtf" title="Rich Text Format">RTF - Rich Text Format</option>
		<option value="odt" title="OpenDocument">ODT - OpenDocument</option>
		<option value="sxw" title="OpenOffice.org Writer">SXW - OpenOffice.org Writer</option>
	</select>
	</td>
	</label>
	</tr>
	<tr>
	<label for="userfile">
	<td>Document:</td>
	<td>
		<input type="hidden" name="MAX_FILE_SIZE" value="10000000"/>
		<input title="Select the file to be translated" name="userfile" type="file"/>
		</td>
	</label>		
	</tr>
	<tr>
	<label for="mark">
	<td>Mark unknown words:</td>
	<td>
		<input  name="mark" value="1" type="checkbox" title="Select the box to mark unknown words"/>
		</td>
	</label>
	</tr>
	</fieldset>
	<tr>
	<td>
	<div>
		<input type="submit" value="Translate" class="submit" title="Clic here to translate"/>
		<input type="reset" value="Reset" class="reset" title="Clic here to reset"/>

	</div>
	</td>
	</tr>
	</table>	
	</form>

_HTML_;
}

?>
