<?php

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
	print "<option value='oc-ca' " . ($dir == 'oc-ca' ? ' selected=true' : '') . ">" . _("Occitan") . " &rarr; " . _("Catalan") . "</option>";
	print "<option value='ca-oc' " . ($dir == 'ca-oc' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("Occitan") . "</option>";
	print "<option disabled='true'></option>";
	print "<option value='oc_aran-ca' " . ($dir == 'oc_aran-ca' ? ' selected=true' : '') . ">" . _("Aranese") . " &rarr; " . _("Catalan") . "</option>";
	print "<option value='ca-oc_aran' " . ($dir == 'ca-oc_aran' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("Aranese") . "</option>";
	print "<option disabled='true'></option>";
	print "<option value='en-ca' " . ($dir == 'en-ca' ? ' selected=true' : '') . ">" . _("English") . " &rarr; " . _("Catalan") . "</option>";
	print "<option value='ca-en' " . ($dir == 'ca-en' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("English") . "</option>";
	print "<option disabled='true'></option>";
	print "<option value='fr-ca' " . ($dir == 'fr-ca' ? ' selected=true' : '') . ">" . _("French") . " &rarr; " . _("Catalan") . "</option>";
	print "<option value='ca-fr' " . ($dir == 'ca-fr' ? ' selected=true' : '') . ">" . _("Catalan") . " &rarr; " . _("French") . "</option>";
	print "<option disabled='true'></option>";
	//print "<option value='es-ro' " . ($dir == 'es-ro' ? ' selected=true' : '') . ">" . _("Spanish") . " &rarr; " . _("Romanian") . "</option>";
	print "<option value='ro-es' " . ($dir == 'ro-es' ? ' selected=true' : '') . ">" . _("Romanian") . " &rarr; " . _("Spanish") . "</option>";
?>