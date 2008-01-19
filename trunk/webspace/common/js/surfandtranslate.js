function copyURL(value){
	var tb = document.getElementById('inurl');
	tb.value = value;
}

function loadURLExamples() {
	var siteNames = new Array();
	var urls = new Array();	
	
	var dirObj = document.getElementById('dir');
	var dir = dirObj.value;
	
	if( dir == "es-ca" ) {
		siteNames[0] = "es.wikipedia.org";
		urls[0] = "http://es.wikipedia.org";	
	}

	if( dir == "ca-es" ) {
		siteNames[0] = "ca.wikipedia.org";
		urls[0] = "http://ca.wikipedia.org";	
		siteNames[1] = "elPeriódico.cat";
		urls[1] = "http://www.elperiodico.cat";
	}

	if( dir == "ro-es" ) {
		siteNames[0] = "Wikipedia - ro.wikipedia.org";
		urls[0] = "http://ro.wikipedia.org";	
	}
	

	var obj = document.getElementById('urlexample');
	obj.setAttribute("onchange", "copyURL(this.value);");
	//obj.setAttribute("onclick", "loadURLExamples();");
		
	if ( obj.hasChildNodes() ) {
    while ( obj.childNodes.length >= 1 ){
        obj.removeChild( obj.firstChild );       
    } 
	}	
	
	var option = document.createElement('option');
	option.setAttribute("value",-1);
	option.setAttribute("disabled","true");
	option.setAttribute("selected","true");	
	
	var text = document.createTextNode("Select example...");
	option.appendChild(text);
	obj.appendChild(option);
	
	
	for( i=0; i<urls.length; i++) {
		var option = document.createElement('option');
		option.setAttribute("value",urls[i]);
		var text = document.createTextNode(siteNames[i]);
		option.appendChild(text);
		obj.appendChild(option);
	}
	
}

