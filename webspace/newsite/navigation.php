<div id="links">

	<p>
		<?php 
			include_once("sections/sections_ids.php");
			$file = "sections/sections_" . $lang . ".php";			
			if (file_exists($file)) {
				include_once("sections/sections_" . $lang . ".php");
			} else {
				include_once("sections/sections_en.php");
			}
			
                 		
			$num = count($sections);
			for ($i=0; $i < $num; $i++) {
				echo '<br/>';
				if ($sections[$i] == $id ) {
					print '<a class="selected" title="' . $description[$i] . '" href="?id=' . $sections[$i] . '&amp;lang=' . $lang . '">' . $text[$i] . '</a>';				
				} else {
					print '<a title="' . $description[$i] . '" href="?id=' . $sections[$i] . '&amp;lang=' . $lang . '">' . $text[$i] . '</a>';
				}
				echo $description[$i];
			}
		?>
	</p>
</div>
