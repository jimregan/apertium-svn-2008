<?php
global $langtext;
function printLanguageLink($code, $text, $url, $newLang) {
	if ($newLang==$code) {
		$code = '&nbsp;<b>' . $text . '</b>&nbsp;';
		$GLOBALS["langtext"] = $code;
	} else {
	print '<a title="' . $text . '" href="' . $url . 'lang=' . $code . '">' . $code . '</a>&nbsp;&nbsp;';
	}
}

?>

<div id="langselect">
	<?php
		//printLanguageLink("af","afrikaans (afrikaans)", $url, $newLang, $currentLang);
		printLanguageLink("ca","català", $url, $newLang, $currentLang);
		//printLanguageLink("cy","cymraeg", $url, $newLang, $currentLang);
		//printLanguageLink("da","dansk", $url, $newLang, $currentLang);
		printLanguageLink("de","deutsch", $url, $newLang, $currentLang);		
		printLanguageLink("en","english", $url, $newLang, $currentLang);
		printLanguageLink("es","español", $url, $newLang, $currentLang);
		printLanguageLink("eu","euskara", $url, $newLang, $currentLang);		
		printLanguageLink("fr","français", $url, $newLang, $currentLang);
		printLanguageLink("gl","galego", $url, $newLang, $currentLang);
		//printLanguageLink("id","indonesian", $url, $newLang, $currentLang);
		//printLanguageLink("mk","македонски јазик (macedonian)", $url, $newLang, $currentLang);		
		//printLanguageLink("ms","bahasa Melayu (malay)", $url, $newLang, $currentLang);		
		//printLanguageLink("nl","nederlands (	dutch)", $url, $newLang, $currentLang);
		printLanguageLink("oc","occità", $url, $newLang, $currentLang);
		printLanguageLink("pt","português", $url, $newLang, $currentLang);
		printLanguageLink("ro","română", $url, $newLang, $currentLang);
		//printLanguageLink("sh","cрпскохрватски / srpskohrvatski (serbo-croatian)", $url, $newLang, $currentLang);
		//printLanguageLink("sv","svenska (swedish)", $url, $newLang, $currentLang);
		//printLanguageLink("tg","тоҷикӣ (tajik)", $url, $newLang, $currentLang);		
		print '<a title="" href="' . $url . 'lang=' . $newLang . '">-&nbsp;<b>'. $langtext .'</b>&nbsp;</a>';
	?>
</div>

