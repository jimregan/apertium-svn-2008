<?php

/*
	DEFAULT LANGUAGE PAIR FOR EACH LANGUAGE
*/
function getPair($lang) {
	if( $lang == "es" ) {
		return "es-ca";
	}
	if( $lang == "ca" ) {
		return "ca-es";
	}
	if( $lang == "pt" ) {
		return "pt-es";
	}
	if( $lang == "en" ) {
		return "en-ca";
	}
	if( $lang == "fr" ) {
		return "fr-ca";
	}
	
	/*
	if( $lang == "af" ) {
		return "af-en";
	}
	*/
	/*
	if( $lang == "oc" ) {
		return "oc-ca";
	}
	*/
	if( $lang == "gl" ) {
		return "gl-es";
	}	
	/*
	if( $lang == "ro" ) {
		return "ro-es";
	}
	*/
}
?>