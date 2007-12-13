<?php
	$sections = array("whatisapertium", "whodevelopsit", "downloading", "testdrive", "documentation", "interact", "latestnews", "software", "funding", "users", "contact");
	$text = array("¿qué es Apertium?", "¿quién lo desarrolla?", "descargas", "pruébelo", "documentaci&oacute;n", "¡interactúe!", "&uacute;ltimas noticias", "software relacionado", "financiación", "usuarios", "contacto");

$subsections = array
(
/*
"whatisapertium" =>array
   (
   "langpairs"=>"Language pairs",
   ),
   */
   "testdrive"=>array
   (
   	"translatetext"=>"Traducción de texto",
   	"translatedoc"=>"Traducción de documentos",
   	"surfandtranslate"=>"Navegar y traducir",   	
   	"internostrum"=>"interNOSTRUM"
   ),
"documentation" =>array
   (
   /*"documents"=>"Documents",*/
   "publications"=>"Publicaciones",
   "link@Wiki" => "http://xixona.dlsi.ua.es/wiki"
   ),
"interact" =>array
	(
		"link@cgi::irc"=>"http://xixona.dlsi.ua.es/cgi-bin/cgiirc/irc.cgi",
		"link@Forum"=>"http://sourceforge.net/forum/forum.php?forum_id=481030",
		"link@Mailing list"=>"https://lists.sourceforge.net/lists/listinfo/apertium-stuff"
	),
"latestnews" =>array
	(
		"link@RSS"=>"http://www.sf.net/export/rss2_projnews.php?group_id=143781"
	),
"software" =>array
	(
		"apertium-tagger-training-tools"=>"Tagger training tools",
		"apertium-transfer-tools"=>"Transfer tools",
		"lttoolbox"=>"Lexical transformation",
		"apertium-eval-translator"=>"Evaluation"
	),
);
	
?>
