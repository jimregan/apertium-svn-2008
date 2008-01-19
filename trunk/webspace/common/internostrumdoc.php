<?php
	include_once("config/apertium-config.php");
	show_form("","","");
?>

<?php
/*
  **************************
	   SHOW FORM
  **************************
*/
function show_form($text, $mark, $val) {
print<<<_HTML_
<form class="translation" enctype="multipart/form-data" action="common/traddoc-internostrum.php" method="post">
	<fieldset>
	<legend></legend>

	<table border="0px">
	<!-- Direction -->
	<tr>
	<label for="direction">
	<td>Sentit:</td>
	<td>
	<select id="direction" name="direction" title="">
_HTML_;
	print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">Castellà &rarr; Català</option>";
	print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">Català &rarr; Castellà</option>";
print<<<_HTML_
	</select>
	</td>
	</label>
	</tr>

	<!-- Doctype -->
	<tr>
	<label for="doctype">
	<td>Tipus de fitxer:</td>
	<td>		
	<select id="doctype" name="doctype" title="">
		<option value="txt" title="Text net">Text - Text net</option>
		<option value="html" title="llenguatge de marcat d'hipertext">HTML - HyperText Markup Language</option>
		<option value="rtf" title="format de text enriquit">RTF - Rich Text Format</option>
		<option value="odt" title="format de document obert">ODT - OpenDocument</option>
		<!--
		<option value="sxw" title="OpenOffice.org Writer">SXW - OpenOffice.org Writer</option>
		-->
	</select>
	</td>
	</label>
	</tr>

	<!-- User file -->
	<tr>
	<label for="userfile">
	<td>Document:</td>
	<td>
		<input type="hidden" name="MAX_FILE_SIZE" value="10000000"/>
		<input title="Select the file to be translated" name="userfile" type="file"/>
	</td>
	</label>		
	</tr>
	
	<!-- Valencià -->
	<tr>
	<label for="val">
	<td colspan="2">
_HTML_;
		print '<input id="val" value="1" ' . ($val == '1' ? 'checked="yes"' : '') . ' name="val" type="checkbox" title="Dóna prioritat a les formes valencianes">';
print<<<_HTML_
		Dóna prioritat a les <b><a href="?id=formesvalencianes">formes valencianes</a></b>			
	</td>
	</label>
	</tr>
	
	<!-- Mark unknown words-->
	<tr>
	<label for="mark">
	<td colspan="2">
_HTML_;
		print '<input id="mark" value="1" ' . ($mark == "1" ? 'checked="yes"' : '') . ' name="mark" type="checkbox" title="Marca les paraules desconegudes"/>';
print<<<_HTML_
	Marca les paraules desconegudes
	</td>
	</label>
	</tr>
	
	<!-- Buttons -->
	<tr><td colspan="2">
	<div>
		<input type="submit" value="Tradueix" class="submit" title="Tradueix "/>
		<input type="reset" value="Reestableix" class="reset" title="Reestableix"/>
	</div>
	</td></tr>
	</table>
			</fieldset>	
	</form>
_HTML_;
}

?>
