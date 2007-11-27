<?php
		$sections = array("whatisapertium", "whodevelopsit", "downloading", "testdrive", "documentation", "interact", "latestnews", "software", "funding", "users", "contact");
		$text = array("qu'est Apertium?", "qui développe Apertium?", "téléchargement", "testez-le", "documentation", "interagissez!", "dernières nouvelles", "logiciels liés", "financement", "utilisateurs", "contact");
		
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
   	"translatetext"=>"Traduction de texte",
   	"translatedoc"=>"Traduction de documents"  	   	
   ),
"documentation" =>array
   (
   /*"documents"=>"Documents",*/
   "publications"=>"Publications",
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
