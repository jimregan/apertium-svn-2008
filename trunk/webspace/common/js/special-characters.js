function insertChar(oList){
	var iSelected = oList.selectedIndex;
	if(iSelected!=0){
		var tb = document.getElementById('thetextbox');
		tb.value = tb.value + oList.options[iSelected].value;
		oList.selectedIndex=0;
		tb.focus();
	}
}


function insertAtCursor(oList) {
	var iSelected = oList.selectedIndex;
	myValue = oList.options[iSelected].value;
	myField = document.getElementById('thetextbox');
	if(iSelected != 0) {
		oList.selectedIndex=0;
		myField.focus();
	}
	
	var isIE = document.all?true:false;
	var isNS = document.layers?true:false;
	
	//IE support 
	if(isIE) {
		//if (document.selection) {
			myField.focus();
			sel = document.selection.createRange();
			sel.text = myValue;
		//}
	} else {
		//MOZILLA/NETSCAPE support
		if (myField.selectionStart || myField.selectionStart == '0') {
			var startPos = myField.selectionStart;
			var endPos = myField.selectionEnd;
			myField.value = myField.value.substring(0, startPos) + myValue + myField.value.substring(endPos, myField.value.length);
		} else {
			myField.value += myValue;
		}
	}
}

document.write('<div id="special-chars">');
document.write('<select id="a" name="a" tabindex="-1" onchange="insertAtCursor(this);"><option value="a">a</option><option value="&Agrave;">&Agrave;</option><option value="&agrave;">&agrave;</option><option value="&Aacute;">&Aacute;</option><option value="&aacute;">&aacute;</option><option value="&Acirc;">&Acirc;</option><option value="&acirc;">&acirc;</option><option value="&Atilde;">&Atilde;</option><option value="&atilde;">&atilde;</option><option value="&Auml;">&Auml;</option><option value="&auml;">&auml;</option><option value="&Aring;">&Aring;</option><option value="&aring;">&aring;</option><option value="ă">ă<option value="Ă">Ă</select>');
document.write('<select id="e" name="e" tabindex="-1" onchange="insertAtCursor(this);"><option value="e">e</option><option value="&Egrave;">&Egrave;</option><option value="&egrave;">&egrave;</option><option value="&Eacute;">&Eacute;</option><option value="&eacute;">&eacute;</option><option value="&Ecirc;">&Ecirc;</option><option value="&ecirc;">&ecirc;</option><option value="&Euml;">&Euml;</option><option value="&euml;">&euml;</option></select>');
document.write('<select id="i" name="i" tabindex="-1" onchange="insertAtCursor(this);"><option value="i">i</option><option value="&Igrave;">&Igrave;</option><option value="&igrave;">&igrave;</option><option value="&Iacute;">&Iacute;</option><option value="&iacute;">&iacute;</option><option value="&Icirc;">&Icirc;</option><option value="&icirc;">&icirc;</option><option value="&Iuml;">&Iuml;</option><option value="&iuml;">&iuml;</option></select>');
document.write('<select id="o" name="o" tabindex="-1" onchange="insertAtCursor(this);"><option value="o">o</option><option value="&Ograve;">&Ograve;</option><option value="&ograve;">&ograve;</option><option value="&Oacute;">&Oacute;</option><option value="&oacute;">&oacute;</option><option value="&Ocirc;">&Ocirc;</option><option value="&ocirc;">&ocirc;</option><option value="&Otilde;">&Otilde;</option><option value="&otilde;">&otilde;</option><option value="&Ouml;">&Ouml;</option><option value="&ouml;">&ouml;</option></select>');
document.write('<select id="u" name="u" tabindex="-1" onchange="insertAtCursor(this);"><option value="u">u</option><option value="&Ugrave;">&Ugrave;</option><option value="&ugrave;">&ugrave;</option><option value="&Uacute;">&Uacute;</option><option value="&uacute;">&uacute;</option><option value="&Ucirc;">&Ucirc;</option><option value="&ucirc;">&ucirc;</option><option value="&Uuml;">&Uuml;</option><option value="&uuml;">&uuml;</option></select>');

document.write('<select id="s" name="s" tabindex="-1" onchange="insertAtCursor(this);"><option value="s">s</option><option value="ş">ş</option><option value="Ş">Ş</option></select>');
document.write('<select id="t" name="t" tabindex="-1" onchange="insertAtCursor(this);"><option value="t">t</option><option value="ţ">ţ</option><option value="Ţ">Ţ</option></select>');

document.write('<select id="misc" name="misc" tabindex="-1" onchange="insertAtCursor(this);"><option value="misc">'+s_Other+'</option><option value="&szlig;">&szlig;</option><option value="&Ccedil;">&Ccedil;</option><option value="&ccedil;">&ccedil;</option><option value="&Ntilde;">&Ntilde;</option><option value="&ntilde;">&ntilde;</option><option value="&yacute;">&yacute;</option><option value="&yuml;">&yuml;</option><option value="&iquest;">&iquest;</option><option value="&iexcl;">&iexcl;</option></select>');
document.write('</div>');
