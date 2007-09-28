<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="es">
  <head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" /> 
        <link rel="stylesheet" type="text/css" href="style/style.css" />
        <title>Apertium: an open-source shallow-transfer machine translation engine and toolbox</title>
    </head>

<?php 
	$idiomas = split(",", $_SERVER['HTTP_ACCEPT_LANGUAGE']);
	foreach( $idiomas as $lg ) {
		$langBrowser = substr($lg,0,2);
		break;
	}
        $langParam = $HTTP_GET_VARS["lang"];
        $id = $HTTP_GET_VARS["id"];
        
        if( $langParam == null ) {
                if( isset($_SESSION['lang']) ) {
                        $lang = $_SESSION['lang'];
                } else {
                        if( $langBrowser == null ) {
                                $_SESSION['lang'] = 'en';
                                $lang = 'en';
                        } else {
                                $_SESSION['lang'] = $langBrowser;
                        }
                }
       } else {
       	$_SESSION['lang'] = $langParam;
        }
        $lang = $_SESSION['lang'];
        $newLang = $lang;
        
        $queryString = $_SERVER['QUERY_STRING'];

        $lon = strlen($queryString);

        if( $langParam != null ) {
                $newQueryString = substr($queryString,0,$long-8);
        } else {
                $newQueryString = $queryString;
        }
         
        if( $newQueryString!='' ) {
                $url = '?' . $newQueryString . '&amp;';
        } else {
                $url = '?' . $newQueryString;
        }
?>
