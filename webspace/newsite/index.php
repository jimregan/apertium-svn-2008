<?php include_once("meta.php") ?>
<body>
<?php
	$id = $HTTP_GET_VARS["id"];
	if ($id == "") {
		$id = "whatisapertium";
	} 

?> 
        <div id="content">
        <!--
            <div id="search">
                <form action="" method="get">
                    <p>
                        search:&nbsp;<input type="text" />&nbsp;
                        <input type="image" src="images/zoom.jpg" value="Submit" alt="Search" title="Search" />
                    </p>
                </form>
            </div>
            -->
				<?php include_once("header.php") ?>
				
            <div id="body">
            	<!-- Navigation -->
            	<?php include_once("navigation.php") ?>
            	<?php $file = "content/" . $lang . "/" . $id . ".html"; ?>

					<?php
						if( file_exists($file) ) {
							include_once("content/" . $lang . "/" . $id . ".html");
						} else {
							if (file_exists("content/en/" . $id . ".html") ) {
							include_once("content/en/" . $id . ".html");
							
							//echo '<div id="translateit"><p>This page is not available in' . $langtext. '</p>';
							//echo '<p>Do you want to <a href="?id=howto&amp;src=' . $id . '&amp;lang=' . $lang . '">help us to translate it</a>?</p>';
							//echo '</div>';
							} else {
								print '<br/><br/>';
								print 'The page does not exist!';
								print '<br/><br/><br/><br/><br/><br/><br/><br/><br/><br/>';
								print '<br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/>';
							}
						}
					?>
            </div>
            <!-- footer -->
            <?php include_once("footer.php") ?>
        </div>
    
    </body>

</html>