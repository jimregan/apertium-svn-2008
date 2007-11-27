<?php 
	$xml = simplexml_load_file("news.xml");
	
	foreach( $xml->channel->item as $item) {
		print '<h3>' . $item->title  . '</h3>';
		print '<p>' . $item->description . '<br/></p>';
		if( $item->link ) {
			print '<div align="right" class="rsslink">&nbsp;&raquo; <a href="' . $item->link . '">www</a></div>';
		}
	}
?>
