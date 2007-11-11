<?
	require_once 'config.php';
	require_once 'pair.php';

	/*
         *
         *	Set up variables 
	 *
	 */

//	session_start();
	
	/* If not already loaded for this session, load the config, 
	   otherwise set the $config variable to the existing config. */

	if(!isset($_SESSION['config'])) {
		$config = new Config('config/config.xml');	
		$_SESSION['config'] = $config;
	} else {
		$config = $_SESSION['config'];
	}

	$pairs = $config->pairs();
	if(!isset($_POST['selected_pair'])) {
		$pair_names = array_keys($pairs);
		$current_pair = $pair_names[0];
	} else {
		$current_pair = $_POST['selected_pair'];
	}
	$pair = $config->get_pair($current_pair);

	if(!isset($_POST['selected_tag'])) {
		$tags = $pair->tags();
		$tag_names = array_keys($tags);
		$current_tag  = $tag_names[0];
	} else {
		$current_tag  = $_POST['selected_tag'];
	}

	$template = "none";
	if(isset($_POST['selected_template'])) {
		$template = $_POST['selected_template'];
	}

	$left_lemma = $_POST['left_lemma'];
	$left_paradigm = $_POST['left_paradigm'];
	$left_comment = $_POST['left_comment'];

	$right_lemma = $_POST['right_lemma'];
	$right_paradigm = $_POST['right_paradigm'];
	$right_comment = $_POST['right_comment'];

	$left_lemma_exists = "false";
	$right_lemma_exists = "false";

	$left_lemma_exists = $_POST['left_lemma_exists'];
	$right_lemma_exists = $_POST['right_lemma_exists'];

	$dictionary_left = $pair->dictionary('left');
	$left_lemma_exists = $dictionary_left->lemma_exists($left_lemma, $current_tag);

	$dictionary_right = $pair->dictionary('right');
	$right_lemma_exists = $dictionary_right->lemma_exists($right_lemma, $current_tag);

	$restriction = $_POST['restriction'];

	if(isset($_POST['preview_box'])) {
		if($_POST['preview_box'] == _('Preview')) {
			print $_POST['preview_box'];
			print ' (' . $template . ')';
			$previewing = 'on';
		}
	}

	if(isset($_POST['commit_box'])) {
		if($_POST['commit_box'] == _('Commit')) {
			print $_POST['commit_box'];
			print ' (' . $template . ')';
			$committing = 'on';
		}
	}

	if(isset($_POST['clear_box'])) {
		if($_POST['clear_box'] == _('Clear')) {
			$right_lemma = '';
			$left_lemma = '';
			$right_paradigm = '';
			$left_paradigm = '';
			$restriction = '';
		}
	}

?>

<html>
<head>
  <title><? print _('Dictionary management'); ?></title>
  <link rel="stylesheet" type="text/css" href="styles/default.css"/>
  <script src="js/boxover.js"></script> 
</head>
<body>
<form action="index.php" method="POST" name="dixform">
<div id="language_bar">
ca · en · es
</div>
<div>
  <div width="100%"> 								<!-- Header -->
    <? print _('Language pair:'); ?> <select name="selected_pair" onChange="dixform.submit();">
    <?
       $pairs = $config->pairs();

       foreach($pairs as $pair) {
           $pair = trim($pair->name);
           print '  <option value="' . $pair . '"' . ($pair ==  $current_pair  ? ' selected' : '') . '>' . $pair . '</option>' . "\n    ";
       } 

       print "\n";
    ?>
    </select>
    <? print _('Part-of-speech:'); ?> <select name="selected_tag" onChange="dixform.submit();">
    <?
       $pair = $config->get_pair($current_pair);
       $tags = $pair->tags();

       foreach($tags as $tag) {
           $tag = trim($tag);
           print '<option value="' . $tag . '"' . ($tag ==  $current_tag  ? ' selected' : '') . '>' . $tag . '</option>' . "\n";
       } 
    ?>
    </select>
  </div>

  <hr/>
  <div>
    <!-- Left side -->
    <div id="left">
    <?
        $pair = $config->get_pair($current_pair);
	$dictionary = $pair->dictionary('left');
	
	print _($dictionary->language) . ' <br /><small>(';
	print $dictionary->file . ' )</small>';
    ?>
    <p>
      Lemma:<sup><span class="tooltip" title="header=[Lemma] body=[Type in the lemma, or citation form of the word you wish to add.]">?</span></sup>
      <input type="text" name="left_lemma" value="<? print $left_lemma; ?>" <? if($left_lemma_exists == "true") { print 'id="known_word"'; } ?>>

      <? if($left_lemma_exists == "true") { print _("Known lemma"); } ?>
    </p>
    <p>
      <? print _('Paradigm'); ?>:
      <sup><span class="tooltip" title="header=[Paradigm] body=[Paradigms define how a word inflects, select the one that fits the lemma you added.]">?</span></sup>
      <select name="left_paradigm">
      <?
          $pair = $config->get_pair($current_pair);
	  $dictionary = $pair->dictionary('left');
	  $paradigms =  $dictionary->paradigms($current_tag);
	  $display_mode = $dictionary->get_display_mode($current_tag);

	  $glosses = $dictionary->glosses();

	  foreach($paradigms as $paradigm) {
              // if we're in "list mode and there is no gloss for this
              // paradigm, don't display it
              if($display_mode == "list" &&  $glosses[$paradigm->name] == "") {
                  continue;
              }
              print '<option value="' . $paradigm->name . '"';
	      if($paradigm->name == $left_paradigm) {
	          print ' selected';
	      }
              if($glosses[$paradigm->name] != "") {
                  print '>' . $glosses[$paradigm->name] . '</option>'. "\n";
              } else {
                  print '>' . $paradigm->name . '</option>'. "\n";
              }
	  }
      ?>
      </select>
    </p>
    <p>
    <? print _('Comment'); ?>:  
    <sup><span class="tooltip" title="header=[Comment] body=[You can leave a comment here.]">?</span></sup>
    <input type="text" name="left_comment" value="<? print $left_comment; ?>" /> 
    </p>
    <p>
    <?
        if($previewing == 'on') {
            $pair = $config->get_pair($current_pair);
            $dictionary = $pair->dictionary('left');

            $par = $dictionary->get_paradigm($left_paradigm, $current_tag);
	    $stems = $par->stems();
	    $show_stems = $pair->shows($current_tag);
            $templates = $pair->templates("l");

	    foreach(array_keys($stems) as $stem) {
                if(sizeof($show_stems) > 1) {
                    // if there is a tag whitelist, use it.
                    foreach($show_stems as $show) {
                        if($show == $stems[$stem][0]) {
                            print $dictionary->incondicional($left_lemma, $left_paradigm) . $stem . '<br />';
                            print '<span id="symbol_list">' . $stems[$stem][0] . '</span>';
                            print '<br/>';
                        }
                    }
                } else {
                    // if not, display all stem-tag pairs.
                    print $dictionary->incondicional($left_lemma, $left_paradigm) . $stem . '<br />';
                    print '<span id="symbol_list">' . $stems[$stem][0] . '</span>';
                    print '<br/>';
                }
           
                // if some templates have been retrieved for this pair
                // check the stem tags of the current paradigm against 
                // the ones stored for each template. if any match, set
                // the current template.
		if(sizeof($templates) > 0) {
                    foreach(array_keys($templates) as $tmpl_tag) {
                        if(strstr($stems[$stem][0], $tmpl_tag)) {
                            $template = $templates[$tmpl_tag];       
                        }
                    }

                } else {
                        $template = "none"; 
	        }
	    }

            $dictionary = $pair->dictionary('left');
            $left_entrada = $dictionary->generate_monodix_entrada($left_lemma, $left_paradigm, $left_comment, "webform");

	    print '<p>';
            print '<span id="monodix_entrada"><pre>';
            print str_replace('>', '&gt;', str_replace('<', '&lt;', $left_entrada));
            print '</pre></span>';
	} 
    ?>
    </p>  
    </div>
    <!-- Bidix side -->
    <div id="centre">
      Direction restriction:<sup><span class="tooltip" title="header=[Direction restriction] body=[Please indicate the direction in which this word pair should be translated.]">?</span></sup>
      <br />
      <input type="radio" name="restriction" value="none" <? if($restriction == 'none' || $restriction == '') print  ' checked'; ?>>←→<br />
      <input type="radio" name="restriction" value="LR" <? if($restriction == 'LR')   print  ' checked'; ?>>→→<br />
      <input type="radio" name="restriction" value="RL" <? if($restriction == 'RL')   print ' checked'; ?>>←←
      <p>
        <input type="submit" name="clear_box" value="<? print _('Clear'); ?>">
        <input type="submit" name="preview_box" value="<? print _('Preview'); ?>">
        <input type="submit" name="commit_box" value="<? print _('Commit'); ?>">
      </p> 
      <p>
      <?
          if($previewing == "on") {
              $pair = $config->get_pair($current_pair);

              $dictionary = $pair->dictionary('bilingual');

          }

      ?> 
      </p>
    </div>
    <!-- Right side -->
    <div id="right">
    <?
        $pair = $config->get_pair($current_pair);
	$dictionary = $pair->dictionary('right');
	
	print $dictionary->language . ' <br /><small>(';
	print $dictionary->file . ' )</small>';
    ?>
    <p>
      Lemma:<sup>&nbsp;</sup><input type="text" name="right_lemma" value="<? print $right_lemma; ?>" <? if($right_lemma_exists == "true") { print 'id="known_word"'; } ?>>
      <? if($right_lemma_exists == "true") { print "Known lemma"; } ?>
    </p>
    <p>
      Paradigm:<sup>&nbsp;</sup><select name="right_paradigm">
      <?
          $pair = $config->get_pair($current_pair);
	  $dictionary = $pair->dictionary('right');
	  $paradigms =  $dictionary->paradigms($current_tag);
	  $display_mode = $dictionary->get_display_mode($current_tag);

	  foreach($paradigms as $paradigm) {
              // if we're in "list mode and there is no gloss for this
              // paradigm, don't display it
              if($display_mode == "list" &&  $glosses[$paradigm->name] == "") {
                  continue;
              }
              print '<option value="' . $paradigm->name . '"';
	      if($paradigm->name == $right_paradigm) {
	          print ' selected';
	      }
	      print '>' . $paradigm->name . '</option>'. "\n";
	  }
      ?>
      </select>
    </p>
    <p>
      <? print _('Comment'); ?>:  
      <input type="text" name="right_comment" value="<? print $right_comment; ?>" /> 
    </p>
    <p>
    <?
        if($previewing == 'on') {
            $pair = $config->get_pair($current_pair);
            $dictionary = $pair->dictionary('right');

            $par = $dictionary->get_paradigm($right_paradigm, $current_tag);
	    $stems = $par->stems();
	    $show_stems = $pair->shows($current_tag);
            $templates = $pair->templates("r");

	    foreach(array_keys($stems) as $stem) {
                if(sizeof($show_stems) > 1) {
                    foreach($show_stems as $show) {
                        if($show == $stems[$stem][0]) {
                            print $dictionary->incondicional($right_lemma, $right_paradigm) . $stem . '<br />';
                            print '<span id="symbol_list">' . $stems[$stem][0] . '</span>';
                            print '<br/>';
                        }
                    }
                } else {
                    print $dictionary->incondicional($right_lemma, $right_paradigm) . $stem . '<br />';
                    print '<span id="symbol_list">' . $stems[$stem][0] . '</span>';
                    print '<br/>';
                }
 
		if(sizeof($templates) > 0) {
                    foreach(array_keys($templates) as $tmpl_tag) {
                        if(strstr($stems[$stem][0], $tmpl_tag)) {
                            $template = $templates[$tmpl_tag];       
                        }
                    }
                } else {
                        $template = "none"; 
	        }
	    }

            $dictionary = $pair->dictionary('right');
            $right_entrada = $dictionary->generate_monodix_entrada($right_lemma, $right_paradigm, $right_comment, "webform");

	    print '<p>';
            print '<span id="monodix_entrada"><pre>';
            print str_replace('>', '&gt;', str_replace('<', '&lt;', $right_entrada));
            print '</pre></span>';
	} 
    ?>
    </p>
    </div>
  </div>
  <div>
  <?
          $pair = $config->get_pair($current_pair);
          $dictionary = $pair->dictionary('bilingual');

          if($template != "none") {
              $bidix_entrada = $dictionary->generate_bidix_entrada_from_template($pair->template_dir(), $template, $left_lemma, $right_lemma, "", "webform"); 
              print '<span id="bidix_entrada"><pre>';
              $bidix_entrada = str_replace('><', '>\n<', str_replace('>', '&gt;', str_replace('<', '&lt;', $bidix_entrada)));
	      $bidix_entrada = str_replace("&lt;/e&gt;\n", "&lt;/e&gt;<br />", $bidix_entrada);
	      print $bidix_entrada;
              print '</pre></span>';
          } else { 
              $bidix_entrada = $dictionary->generate_bidix_entrada($left_lemma, $right_lemma, $current_tag, $restriction, "", "webform");
              print '<span id="bidix_entrada"><pre>';
              print str_replace('><', '>\n<', str_replace('>', '&gt;', str_replace('<', '&lt;', $bidix_entrada)));
              print '</span></pre>';
          }

          print '<input type="hidden" name="selected_template" value="' . $template . '">';
  ?>
  </div>
  <div width="100%">

  <?
      if($committing == "on") {
          $pair = $config->get_pair($current_pair);

          $dictionary = $pair->dictionary('left');
          $left_entrada = $dictionary->generate_monodix_entrada($left_lemma, $left_paradigm, $left_comment, "webform");

          $dictionary = $pair->dictionary('bilingual');
          if($template != "none") {
              $bidix_entrada = $dictionary->generate_bidix_entrada_from_template($pair->template_dir(), $template, $left_lemma, $right_lemma, "", "webform"); 
          } else { 
              $bidix_entrada = $dictionary->generate_bidix_entrada($left_lemma, $right_lemma, $current_tag, $restriction, "", "webform");
          }

          $dictionary = $pair->dictionary('right');
          $right_entrada = $dictionary->generate_monodix_entrada($right_lemma, $right_paradigm, $right_comment, "webform");

          print '<span id="monodix_entrada">';
          print str_replace('>', '&gt;', str_replace('<', '&lt;', $left_entrada));
          print '</span>';
          print '<p>';
          print '<span id="bidix_entrada">';
          print str_replace('>', '&gt;', str_replace('<', '&lt;', $bidix_entrada));
          print '</span>';
          print '<p>';
          print '<span id="monodix_entrada">';
          print str_replace('>', '&gt;', str_replace('<', '&lt;', $right_entrada));
          print '</span>';

          $pair->commit($left_entrada, $bidix_entrada, $right_entrada);
      }
  ?>
  </div>
</div>
</form>
</body>
</html>
