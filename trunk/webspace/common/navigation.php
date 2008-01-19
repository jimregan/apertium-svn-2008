<div id="links">
	<p>
		<?php 
			//include_once("sections/sections_ids.php");
			$file = "content/" . $lang . "/sections.php";
			if (file_exists($file)) {
				include_once("content/" . $lang . "/sections.php");
			} else {
				include_once("content/en/sections.php");
			}
			
                 		
			$num = count($sections);
			
			for ($i=0; $i < $num; $i++) {
				echo '<br/>';
				$numSub = count($subsections[$sections[$i]]);
				$thisSection = false;	
				if ($numSub > 0) {
					foreach ($subsections[$sections[$i]] as $key=>$value) {
						if ( $key == $id ) {
							$thisSection = true;
							print '<a class="selected" title="' . $description[$i] . '" href="?id=' . $sections[$i] . '&amp;lang=' . $lang . '">' . $text[$i] . '</a>';
							$numSub = count($subsections[$sections[$i]]);
							if ($numSub > 0) {
								foreach ($subsections[$sections[$i]] as $key=>$value) {
									$islink = strstr($key, '@');
									$islink = substr($islink,1, strlen($islink)-1);
									if( strcmp($islink,"") != 0 ) {
										print '<a class="submenu" href="' . $value . '">' . $islink . '</a>';
									} else {
										print '<a class="submenu" href="?id=' . $key . '">' . $value . '</a>';
									}
								}
							}		
						} else {
							//print '<a title="' . $description[$i] . '" href="?id=' . $sections[$i] . '&amp;lang=' . $lang . '">' . $text[$i] . '</a>';
							//echo $description[$i];
						}
					}
				}
				
				if (!$thisSection) {
				if ($sections[$i] == $id) {
					print '<a class="selected" title="' . $description[$i] . '" href="?id=' . $sections[$i] . '&amp;lang=' . $lang . '">' . $text[$i] . '</a>';
					$numSub = count($subsections[$id]);
					if ($numSub > 0) {
						foreach ($subsections[$id] as $key=>$value) {
									$islink = strstr($key, '@');
									$islink = substr($islink,1, strlen($islink)-1);
									if( strcmp($islink,"") != 0 ) {
										print '<a class="submenu" href="' . $value . '">' . $islink . '</a>';
									} else {
										print '<a class="submenu" href="?id=' . $key . '">' . $value . '</a>';
									}
							
						}
					}		
				} else {
					print '<a title="' . $description[$i] . '" href="?id=' . $sections[$i] . '&amp;lang=' . $lang . '">' . $text[$i] . '</a>';
					echo $description[$i];
				}
				}
			}
		?>
	</p>

<!--
	<br/>
	<div id="rss">
	<a href="news.xml">
	<img src="images/rss20.gif" border="0px"/>
	</a>
	</div>
-->	

</div>



