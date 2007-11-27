<?php
	include_once("config/apertium-config.php");
	
	$dir = $HTTP_GET_VARS["dir"];
	if ($dir == "") {
		$dir = getPair($lang);
	}
	show_form("", $dir);
?>

<?php
/*
  **************************
	   SHOW FORM
  **************************
*/
function show_form($text, $dir) {
	print '<form class="translation" enctype="multipart/form-data" action="common/traddoc.php" method="post">';
	print '<fieldset><legend></legend><table border="0px"><tr><label for="direction">';
	print '<td>' . _("Translation") . ':</td>';
	print '<td><select id="direction" name="direction" title="">';
	if ($dir=="") {
				print "<option disabled='true' selected='true'>" . _("Select direction...") . "</option>";
			}
			
			print "<option value='es-ca' " . ($dir == 'es-ca' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("Catalan") . "</option>";
			print "<option value='ca-es' " . ($dir == 'ca-es' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("Spanish") . "</option>";
			print "<option disabled='true'></option>";
			print "<option value='es-gl' " . ($dir == 'es-gl' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("Galician") . "</option>";
			print "<option value='gl-es' " . ($dir == 'gl-es' ? ' selected=true' : '') . ">" . _("Galician") . " &rarr; " . _("Spanish") . "</option>";
			print "<option disabled='true'></option>";
			print "<option value='es-pt' " . ($dir == 'es-pt' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("Portuguese") . "</option>";
			print "<option value='pt-es' " . ($dir == 'pt-es' ? ' selected=true' : '') . ">" . _("Portuguese") . " &rarr; " . _("Spanish") . "</option>";
			print "<option value='es-pt_BR' " . ($dir == 'es-pt_BR' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("Brazilian Portuguese") . "</option>";
			print "<option disabled='true'></option>";
			print "<option value='en-ca' " . ($dir == 'en-ca' ? ' selected=true' : '') . ">" . _("English") . " &rarr; " . _("Catalan") . "</option>";
			print "<option value='ca-en' " . ($dir == 'ca-en' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("English") . "</option>";
			print "<option disabled='true'></option>";
			print "<option value='fr-ca' " . ($dir == 'fr-ca' ? ' selected=true' : '') . ">" . _("French") . " &rarr; " . _("Catalan") . "</option>";
			print "<option value='ca-fr' " . ($dir == 'ca-fr' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("French") . "</option>";	
	print '</select></td></label></tr>';

	// Document type	
	print	'<tr><label for="doctype"><td>' . _("Document type") . ':</td>';
	print '<td>';		
	print '<select id="doctype" name="doctype" title="">';
	print '<option value="txt" title="Text">' . _("Text - Plain text") . '</option>';
	print '<option value="html" title="HyperText Markup Language">' . _("HTML - HyperText Markup Language") . '</option>';
	print '<option value="rtf" title="Rich Text Format">' . _("RTF - Rich Text Format") . '</option>';
	print '<option value="odt" title="OpenDocument">' . _("ODT - OpenDocument") . '</option>';
	//print '<option value="sxw" title="OpenOffice.org Writer">' . _("SXW - OpenOffice.org Writer") . '</option>';
	print '</select></td></label></tr>';
	
	// Document file
	print '<tr><label for="userfile">';	
	print '<td>' . _("Document") . ':</td>';
	print '<td><input type="hidden" name="MAX_FILE_SIZE" value="10000000"/>';
	print '<input title="' . _("Select the file to be translated") . '" name="userfile" type="file"/>';
	print '</td></label></tr><tr><label for="mark">';		
	print '<td>' . _("Mark unknown words") . ':</td>';
	print '<td><input  name="mark" value="1" type="checkbox" title="' . _("Select the box to mark unknown words") . '"/></td>';
	print '</label></tr>';
	
	// Translate & Reset
	print '<tr><td><div>';
	print '<input type="submit" value="' . _("Translate") . '" class="submit" title="' . _("Clic here to translate") . '"/>';
	print '<input type="reset" value="' . _("Reset") . '" class="reset" title="' . _("Clic here to reset") . '"/>';
	print '</div></td></tr></table></fieldset></form>';
	
}

?>
