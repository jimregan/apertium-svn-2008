<?php
	include_once("config/apertium-config.php");
	show_form("");
?>

<?php
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
_HTML_;

	print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">Spanish - Catalan</option>";
	print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">Catalan - Spanish</option>";
	print "<option value='es-pt' " . ($dir == 'es-pt' ? ' selected=true' : '') . ">Spanish - Portuguese</option>";
	print "<option value='pt-es' " . ($dir == 'pt-es' ? ' selected=true' : '') . ">Portuguese - Spanish</option>";
	print "<option value='es-pt_BR' " . ($dir == 'es-pt_BR' ? ' selected=true' : '') . ">Spanish - Brazilian Portuguese</option>";
	print "<option value='en-ca' " . ($dir == 'en-ca' ? ' selected=true' : '') . ">English - Catalan</option>";
	print "<option value='ca-en' " . ($dir == 'ca-en' ? ' selected=true' : '') . ">Catalan - English</option>";
	//print "<option disabled='true' value='fr-ca' " . ($dir == 'fr-ca' ? ' selected=true' : '') . ">French - Catalan</option>";
	//print "<option disabled='true' value='ca-fr' " . ($dir == 'ca-fr' ? ' selected=true' : '') . ">Catalan - French</option>";		

print<<<_HTML_
	</select>
	</td>
	</label>
	</tr>	
	<tr>
	<label for="doctype">
	<td>Document type:</td>
	<td>		
	<select id="doctype" name="doctype" title="">
		<option value="txt" title="Text">Text - Plain text</option>
		<option value="html" title="HyperText Markup Language">HTML - HyperText Markup Language</option>
		<option value="rtf" title="Rich Text Format">RTF - Rich Text Format</option>
		<option value="odt" title="OpenDocument">ODT - OpenDocument</option>
		<!--
		<option value="sxw" title="OpenOffice.org Writer">SXW - OpenOffice.org Writer</option>
		-->
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
