<?php
include_once("config/apertium-config.php");
// apertium-config.php: $APERTIUM_TRANSLATOR, $LING_DATA_DIR, etc.
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

?>
