<?php
	include_once("config/langpairs.php");
?>

<?php
	$dir = $HTTP_GET_VARS["dir"];
	if ($dir == "") {
		$dir = getPair($lang);
	}
	show_form($dir);

/*
  **************************
	   SHOW FORM
  **************************
*/
function show_form($dir) {
	print '<form class="translation" action="common/browser.php" method="post">';
	print '<table><fieldset><legend></legend>';
	print '<tr><label for="dir">';
	print '<td>' . _("Translation") . ':</td>';
	print '<td><select id="dir" name="dir" title="' . _("Select the translation type") . '">';

	include_once("available_pairs.php");

	print '</select></td></label></tr>';

	print '<tr><label for="inurl"><td>URL:</td>';
	print '<td><input size="40" title="' . _("URL to be translated") . '" name="inurl" type="text" value="http://"/></td>';
	print '</label></tr>';

	print '<tr><label for="mark"><td>' . _("Mark unknown words") . ':</td>';
	print '<td><input name="mark" value="1" type="checkbox"/></td>';
	print '</label></tr></fieldset>';

	print '<tr><div><td><input type="submit" value="' . _("Translate") . '" class="submit" title="' . _("Click here to translate") . '"/>';
	print '<input type="reset" value="' . _("Reset") . '" class="reset" title="' . _("Click here to reset") . '"/></td></div></tr>';
	print '</table></form>';
}
