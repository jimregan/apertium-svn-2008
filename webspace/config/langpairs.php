<?php

/*
	DEFAULT LANGUAGE PAIR FOR EACH LANGUAGE
*/
function getPair($lang) {
	switch($lang) {
		case "es":
			return "es-ca";
			break;
		case "ca":
			return "ca-es";
			break;
		case "pt":
			return "pt-es";
			break;
		case "en":
			return "en-ca";
			break;
		case "fr":
			return "fr-ca";
			break;
		//case "af":
			//return "af-en";
			//break;
		//case "oc":
			//return "oc-ca";
			//break;
		case "gl":
			return "gl-es";
			break;
		//case "ro":
			//return "ro-es";
			//break;
		default:
			return "";
			break;
		}			 
}

/*
	DEFAULT 'LOCALE' FOR EACH LANGUAGE
*/
function getLocale($lang) {
	switch($lang) {
		case "af":
			return "af_ZA";
			break;
		case "ca":
			return "ca_ES";
			break;
		case "es":
			return "es_ES";
			break;
		case "en":
			return "en_GB";
			break;
		case "fr":
			return "fr_FR";
			break;
		case "gl":
		  return "gl_ES";
		  break;
		case "oc":
			return "oc_FR";
			break;
		case "pt":
			return "pt_PT";
			break;
		case "ro":
			return "ro_RO";
			break;
		 default:
		 	return "en_GB";
		 	break;		 				
	}
}

?>