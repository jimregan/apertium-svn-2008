<?php
	print '<select id="urlexample" onchange="copyURL(this.value);" onclick="loadURLExamples();">';
	print '<option value="-1" disabled="true" selected="true">Select example...</option>';
	//print '<option value="http://' . $lang . '.wikipedia.org">Wikipedia - ' . $lang . '.wikipedia.org</option>';	
	print '</select>';
?>
