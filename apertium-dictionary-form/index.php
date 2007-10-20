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

	if(isset($_POST['selected_pair'])) {
		$_SESSION['current_pair'] = $_POST['selected_pair'];
	}
	$current_pair = $_SESSION['current_pair'];

	if(isset($_POST['selected_tag'])) {
		$_SESSION['current_tag'] = $_POST['selected_tag'];
	}
	$current_tag = $_SESSION['current_tag'];

	$left_lemma_exists = "false";
	$right_lemma_exists = "false";

	$left_lemma_exists = $_POST['left_lemma_exists'];
	$right_lemma_exists = $_POST['right_lemma_exists'];

	if(isset($_POST['left_lemma'])) {
		$_SESSION['left_lemma'] = $_POST['left_lemma'];
	}
	$left_lemma = $_POST['left_lemma'];

	if(isset($_POST['left_paradigm'])) {
		$_SESSION['left_paradigm'] = $_POST['left_paradigm'];
	}
	$left_paradigm = $_POST['left_paradigm'];

	if(isset($_POST['right_lemma'])) {
		$_SESSION['right_lemma'] = $_POST['right_lemma'];
	}
	$right_lemma = $_POST['right_lemma'];

	if(isset($_POST['right_paradigm'])) {
		$_SESSION['right_paradigm'] = $_POST['right_paradigm'];
	}
	$right_paradigm = $_POST['right_paradigm'];

	if(isset($_POST['restriction'])) {
		$_SESSION['restriction'] = $_POST['restriction'];
	}
	$restriction = $_POST['restriction'];

	if(isset($_POST['preview_box'])) {
		if($_POST['preview_box'] == _('Preview')) {
			print $_POST['preview_box'];
			$previewing = 'on';
		}
	}

	if(isset($_POST['commit_box'])) {
		if($_POST['commit_box'] == _('Commit')) {
			print $_POST['commit_box'];
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
<div>
  <div width="100%"> 								<!-- Header -->
    Language pair: <select name="selected_pair" onChange="dixform.submit();">
    <?
       $pairs = $config->pairs();

       foreach($pairs as $pair) {
           $pair = trim($pair->name);
           print '  <option value="' . $pair . '"' . ($pair ==  $current_pair  ? ' selected' : '') . '>' . $pair . '</option>' . "\n    ";
       } 

       print "\n";
    ?>
    </select>
    Part-of-speech: <select name="selected_tag" onChange="dixform.submit();">
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
    <div id="left">
    <?
        $pair = $config->get_pair($current_pair);
	$dictionary = $pair->dictionary('left');
	
	print $dictionary->language . ' <br /><small>(';
	print $dictionary->file . ' )</small>';
    ?>
    <p>
      Lemma:<sup><span title="header=[Lemma] body=[Type in the lemma, or citation form of the word you wish to add.]">?</span></sup>
      <input type="text" name="left_lemma" value="<? print $left_lemma; ?>" <? if($left_lemma_exists == "true") { print 'id="known_word"'; } ?>>

      <? if($left_lemma_exists == "true") { print "Known word"; } ?>
    </p>
    <p>
      Paradigm:<sup><span title="header=[Paradigm] body=[Paradigms define how a word inflects, select the one that fits the lemma you added.]">?</span></sup>
      <select name="left_paradigm">
      <?
          $pair = $config->get_pair($current_pair);
	  $dictionary = $pair->dictionary('left');
	  $paradigms =  $dictionary->paradigms($current_tag);

	  foreach($paradigms as $paradigm) {
              print '<option value="' . $paradigm->name . '"';
	      if($paradigm->name == $left_paradigm) {
	          print ' selected';
	      }
	      print '>' . $paradigm->name . '</option>'. "\n";
	  }
      ?>
      </select>
    </p>
    <p>
    <?
        if($previewing == 'on') {
            $pair = $config->get_pair($current_pair);
            $dictionary = $pair->dictionary('left');

            if($dictionary->lemma_exists($left_lemma, $current_tag)) {
		$left_lemma_exists = "true";
	        print '<input type="hidden" name="left_lemma_exists" value="true" />';
	    } else {
	        print '<input type="hidden" name="left_lemma_exists" value="false" />';
	    }

            $par = $dictionary->get_paradigm($left_paradigm, $current_tag);
	    $stems = $par->stems();
	    $show_stems = $pair->shows($current_tag);

	    foreach(array_keys($stems) as $stem) {
                if(sizeof($show_stems) > 1) {
                    foreach($show_stems as $show) {
                        if($show == $stems[$stem][0]) {
                            print $dictionary->incondicional($left_lemma, $left_paradigm) . $stem . '<br />';
                            print '<span id="symbol_list">' . $stems[$stem][0] . '</span>';
                            print '<br/>';
                        }
                    }
                } else {
                    print $dictionary->incondicional($left_lemma, $left_paradigm) . $stem . '<br />';
                    print '<span id="symbol_list">' . $stems[$stem][0] . '</span>';
                    print '<br/>';
                }

	    }
	} 
    ?>
    </p>  
    </div>
    <div id="centre">
      Direction restriction:<sup><span title="header=[Direction restriction] body=[Please indicate the direction in which this word pair should be translated.]">?</span></sup>
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
          }

      ?> 
      </p>
    </div>
    <div id="right">
    <?
        $pair = $config->get_pair($current_pair);
	$dictionary = $pair->dictionary('right');
	
	print $dictionary->language . ' <br /><small>(';
	print $dictionary->file . ' )</small>';
    ?>
    <p>
      Lemma:<sup>&nbsp;</sup><input type="text" name="right_lemma" value="<? print $right_lemma; ?>" <? if($right_lemma_exists == "true") { print 'id="known_word"'; } ?>>
      <? if($right_lemma_exists == "true") { print "Known word"; } ?>
    </p>
    <p>
      Paradigm:<sup>&nbsp;</sup><select name="right_paradigm">
      <?
          $pair = $config->get_pair($current_pair);
	  $dictionary = $pair->dictionary('right');
	  $paradigms =  $dictionary->paradigms($current_tag);

	  foreach($paradigms as $paradigm) {
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
    <?
        if($previewing == 'on') {
            $pair = $config->get_pair($current_pair);
            $dictionary = $pair->dictionary('right');

            if($dictionary->lemma_exists($right_lemma, $current_tag)) {
		$right_lemma_exists = "true";
	        print '<input type="hidden" name="right_lemma_exists" value="true" />';
	    } else {
	        print '<input type="hidden" name="right_lemma_exists" value="false" />';
	    }

            $par = $dictionary->get_paradigm($right_paradigm, $current_tag);
	    $stems = $par->stems();
	    $show_stems = $pair->shows($current_tag);

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
	    }
	} 
    ?>
    </p>
    </div>
  </div>
  <div width="100%">
  <?
      if($committing == "on") {
          $pair = $config->get_pair($current_pair);

          $dictionary = $pair->dictionary('left');
          $left_entrada = $dictionary->generate_monodix_entrada($left_lemma, $left_paradigm);

          $dictionary = $pair->dictionary('bilingual');
          $bidix_entrada = $dictionary->generate_bidix_entrada($left_lemma, $right_lemma, $current_tag);

          $dictionary = $pair->dictionary('right');
          $right_entrada = $dictionary->generate_monodix_entrada($right_lemma, $right_paradigm);

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
