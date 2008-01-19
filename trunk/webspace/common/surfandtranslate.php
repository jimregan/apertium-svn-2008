<?php
	include_once("config/langpairs.php");
?>

<?php
	$dir = $HTTP_GET_VARS["dir"];
	if ($dir == "") {
		$dir = getPair($lang);
	}
	show_form($dir, $lang);

/*
  **************************
	   SHOW FORM
  **************************
*/
function show_form($dir, $lang) {
	print '<script src="common/js/surfandtranslate.js" type="text/javascript"></script>';
	print '<form class="translation" action="common/browser.php" method="post">';
	print '<table><fieldset><legend></legend>';
	print '<tr><label for="dir">';
	print '<td>' . _("Translation") . ':</td>';
	print '<td><select onchange="loadURLExamples();" id="dir" name="dir" title="' . _("Select the translation type") . '">';

	include_once("available_pairs.php");

	print '</select></td></label></tr>';

	print '<tr><label for="inurl"><td>URL:</td>';
	print '<td><input size="40" title="' . _("URL to be translated") . '" name="inurl" id="inurl" type="text" value="http://"/></td>';
	print '</label></tr>';

	// An example of URL is given for ro-es
	if( $dir == "ro-es" ) {
	print '<tr>';
	print '<td></td><td>- ' . _("or") . ' -</td>';
	print '</tr>';
	print '<tr><label for="urlexample"><td></td>';
	print '<td>';
	include_once("urlexamples.php");
	print '</td>';
	print '</label></tr>';
	}
	

	print '<tr><label for="mark"><td colspan="2">' . _("Mark unknown words") . ': ';
	print '<input name="mark" value="1" type="checkbox"/></td>';
	print '</label></tr></fieldset>';

	print '<tr><div><td><input type="submit" value="' . _("Translate") . '" class="submit" title="' . _("Click here to translate") . '"/>';
	print '<input type="reset" value="' . _("Reset") . '" class="reset" title="' . _("Click here to reset") . '"/></td></div></tr>';
	print '</table></form>';
}
