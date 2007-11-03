<?
	class Paradigm {
		public $name;
		public $stems;

		function Paradigm($_name) {
			$this->name = $_name;
			$this->stems = array();
		}

		function add_stem($_stem, $_symbols) {
			$this->stems[$_stem] = array();
			array_push($this->stems[$_stem], $_symbols);
		}

		function stems() {
			return $this->stems;
		}
	}	

	class Dictionary {
		public $language;
		public $file;
		public $doc;
		public $paradigms;
		public $depth;

		function Dictionary($_language, $_file, $_doc, $_tags) {
			$this->language = $_language;
			$this->file = $_file;
			$this->doc = $_doc;

			foreach($_tags as $tag) {
				$this->paradigms[$tag] = array();
				$needle = "__" . $tag;
				$cmd = 'cat */*/' . $_file . ' | grep "pardef" | grep "' . $needle . '\"" | cut -f2 -d= | sed "s/<!--.*-->//g" ';
				$cmd = $cmd . '| sed "s/<e>//g"'; # hack for elements on the same line as pardef (mal) add further hacks below.
				$res = shell_exec($cmd);
				$res = str_replace('"', '', $res);
				$res = str_replace('>', '', $res);

				foreach(explode("\n", $res) as $paradigm_name) {
					if($paradigm_name != "") {
						$par = new Paradigm($paradigm_name);
						array_push($this->paradigms[$tag], $par);
					}
				}
			}

/*** this is more correct, but the above is substantially faster.
			$_paradigms = $this->doc->getElementsByTagName('pardef');

			foreach($_tags as $tag) {
				$this->paradigms[$tag] = array();
				$needle = "/__" . $tag . "$/";

				foreach($_paradigms as $paradigm) {
					$paradigm_name = $paradigm->getAttribute('n');
					
					if(preg_match($needle, $paradigm_name)) {
						$par = new Paradigm($paradigm_name);
						array_push($this->paradigms[$tag], $par);

						$entradas = $paradigm->getElementsByTagName('e');

						foreach($entradas as $entrada) {
							$slist = '';

							$pair = $entrada->getElementsByTagName('p')->item(0);
							$left = $pair->getElementsByTagName('l')->item(0)->nodeValue;
							$right = $pair->getElementsByTagName('r')->item(0);

							$symbols = $right->getElementsByTagName('s');

							foreach($symbols as $symbol) {
								if($slist != '') {
									$slist = $slist . '.' . $symbol->getAttribute('n');
								} else {
									$slist = $slist . $symbol->getAttribute('n');
								}
							}

							$par->add_stem($left, $slist);
						}
					}
				}
			}
 ***/
		}

		function paradigms($_tag) {
			return $this->paradigms[$_tag];	
		}

		function get_paradigm($_name, $_tag) {
			foreach($this->paradigms[$_tag] as $paradigm) {
				if($paradigm->name == $_name) {
					$xpath = new DOMXPath($this->doc);
					$path  = "//pardef[@n='" . $_name . "']"; 
					$res   = $xpath->evaluate($path);

					$par = $res->item(0);

					$entradas = $par->getElementsByTagName('e');

					foreach($entradas as $entrada) {
						$slist = '';

						$pair = $entrada->getElementsByTagName('p')->item(0);
						$left = $pair->getElementsByTagName('l')->item(0)->nodeValue;
						$right = $pair->getElementsByTagName('r')->item(0);

						$symbols = $right->getElementsByTagName('s');

						foreach($symbols as $symbol) {
							if($slist != '') {
								$slist = $slist . '.' . $symbol->getAttribute('n');
							} else {
								$slist = $slist . $symbol->getAttribute('n');
							}
						}

						$paradigm->add_stem($left, $slist);
					}
	
					return $paradigm;
				}
			}

			print "Couldn't find paradigm";
		}

/*** this is more correct, but the above it substantially faster.
		function get_paradigm($_name, $_tag) {
			foreach($this->paradigms[$_tag] as $paradigm) {
				if($paradigm->name == $_name) {
					return $paradigm;
				}
			}

			print "Couldn't find paradigm";
		}
 ***/

                function lemma_exists($_lemma, $_tag) {
                        $dictionary = 'cache/*/' . $this->file;
                        $command = 'cat ' . $dictionary . ' | grep "\"' . $_lemma . '\"" | wc -l ';  
                        $count = shell_exec($command);

                        if($count > 0) {
                                return TRUE;
                        } else {
                                return FALSE;
                        }
                }


/***
		function lemma_exists($_lemma, $_tag) {
			$sections = $this->doc->getElementsByTagName('section');
			foreach($sections as $section) {
				$entradas = $section->getElementsByTagName('e');

				foreach($entradas as $entrada) {
					$paradigm = $entrada->getElementsByTagName('par')->item(0);
					if(!($paradigm)) {
						return FALSE;
					} else if($entrada->getAttribute('lm') == $_lemma && strstr($paradigm->getAttribute('n'), $_tag)) {
						return TRUE;
					}
				}
			}

			return FALSE;
		}
 ***/
		function incondicional($_lemma, $_paradigm) {

			if(!strstr($_paradigm, '/')) {
				return $_lemma;
			}

			$bar_pos       = strpos($_paradigm, '/');
			$und_pos       = strpos($_paradigm, '_');
			$chr_str       = $und_pos - $bar_pos;
			$sub_str       = strlen($_lemma) - $chr_str;
			$incondicional = substr($_lemma, 0, $sub_str + 1);

			return $incondicional;
		}

		function generate_monodix_entrada($_lemma, $_paradigm, $_comment, $_author) {
			// <e lm="lemma"><i>lemm</i><par n="paradigm"/></e>

			$incondicional = $this->incondicional($_lemma, $_paradigm);

				if($_comment != "") {
					$entrada = '<e lm="' . $_lemma . '" a="' . $_author . '">' . "\n" .
						   '  <i>' . $incondicional . '</i>' . "\n" .
						   '  <par n="' . $_paradigm . '"/>' . "\n" . 
						   '</e>' . 
					   	   '<!-- ' . $_comment . ' -->' . "\n";
				} else {
					$entrada = '<e lm="' . $_lemma . '" a="' . $_author . '">' . "\n" .
						   '  <i>' . $incondicional . '</i>' . "\n" .
						   '  <par n="' . $_paradigm . '"/>' . "\n" . 
						   '</e>';
				}


			return $entrada; 
		}

		function generate_bidix_entrada($_lemma1, $_lemma2, $_tag, $_restriction, $_comment, $_author) {
			// <e><p><l>lemma1<s n="tag"/></l><r>lemma2<s n="tag"/></r></p></e>

			if($_restriction == "none") {
				if($_comment != "") {
	 				$entrada = '<e a="' . $_author . '">' . "\n" . 
					           '  <p>' . "\n" .
						   '    <l>' . $_lemma1 . '<s n="' . $_tag . '"/></l>' . "\n" . 
						   '    <r>' . $_lemma2 . '<s n="' . $_tag . '"/></r>' . "\n" .
						   '  </p>' . "\n" . 
						   '</e>' . "\n" .
					           '<!-- ' . $_comment . ' -->' . "\n";
				} else {
	 				$entrada = '<e a="' . $_author . '">' . "\n" . 
					           '  <p>' . "\n" .
						   '    <l>' . $_lemma1 . '<s n="' . $_tag . '"/></l>' . "\n" . 
						   '    <r>' . $_lemma2 . '<s n="' . $_tag . '"/></r>' . "\n" .
						   '  </p>' . "\n" . 
						   '</e>' . "\n";
				}
			} else {
				$entrada = '<e r="' . $_restriction .  '" a="' . $_author . '"><p><l>' . $_lemma1 . '<s n="' . $_tag . '"/></l><r>' . $_lemma2 . '<s n="' . $_tag . '"/></r></p></e>' . "\n";
			}

			return $entrada;
		}

		function generate_bidix_entrada_from_template($_dir, $_template, $_lemma1, $_lemma2, $_comment, $_author) {

			$text = file_get_contents($_dir . $_template);

			$entrada = str_replace('entry_author', $_author, $text);
			$entrada = str_replace('entry_lemma_left', $_lemma1, $entrada);
			$entrada = str_replace('entry_lemma_right', $_lemma2, $entrada);

			return $entrada;
		}

		function commit($cache_dir, $e) {
			$doc = $this->doc;
			$filepath = $cache_dir . '/' . $this->file;

			$sections = $doc->getElementsByTagName('section');
			$insertion_point = '';

			foreach($sections as $section) {
				$id = $section->getAttribute("id");
				if($id == "main") {
					$insertion_point = $section;	
				}
			}

			if($insertion_point != '') {

				$frag = $doc->createDocumentFragment();
				$frag->appendXML($e);
				$insertion_point->appendChild($frag);
				$doc->save($filepath);

			} else {
				print "Couldn't find the main section";
				return;
			}
		}
	}

	/*
	 *
	 *	This class represents a language pair, with the left dictionary,
	 * 	right dictionary and bilingual dictionary (bidix).
	 */
	class Pair {
		
		public $name = '';
		public $tags;
		public $show;
		public $left;
		public $right;
		public $bidix;
		public $templates_left;
		public $templates_right;
		public $parent;
		public $wd;
		public $cachedir;
		public $templatedir;

		function Pair($_wd, $_name, $_parent) {
			$this->name = $_name;
			$this->parent = $_parent;
			$this->wd = $_wd;
		}

		function populate() {

			$this->templatedir = $this->wd . '/templates/'; 
			
			$dicts = $this->parent->getElementsByTagName('dictionary');

			//       <dictionary n="Spanish" side="left" format="dix" file="apertium-es-gl.es.dix" />

			$cachedir = $this->cachedir;

			foreach($dicts as $dict) {
				$current = $dict->getAttribute('n');
				$side = $dict->getAttribute('side');
				$filename = $dict->getAttribute('file');
					
				if($side == 'l') {
					$doc = new DOMDocument;
					$doc->load($cachedir . $filename);
					$this->left = new Dictionary($current, $filename, $doc, $this->tags);	
				} else if($side == 'bilingual') {
					$doc = new DOMDocument;
					$doc->load($cachedir . $filename);
					$this->bidix = new Dictionary($current, $filename, $doc, $this->tags);	
				} else if($side == 'r') {
					$doc = new DOMDocument;
					$doc->load($cachedir . $filename);
					$this->right = new Dictionary($current, $filename, $doc, $this->tags);	
				}
			}
		}


		function template_dir() {
			return $this->templatedir;
		}

		function tags() {
			return $this->tags;
		}

		function shows($_tag) {
			return $this->show[$_tag];
		}
			
		function add_tag($_tag, $_list) {
			$this->tags[$_tag] = $_tag;
			$this->show[$_tag] = $_list;
		}

		function add_template($_template, $_tag, $_side) {
			if($_side == "l") {
				$this->templates_left[$_tag] = $_template;
			} else if($_side == "r") {
				$this->templates_right[$_tag] = $_template;
			}
			return FALSE;
		}

		function template($_tag, $_side) {
			if($_side == "l") {
				return $this->templates_left[$_tag];
			} else if($_side == "r") {
				return $this->templates_right[$_tag];
			}
			return FALSE;
		}

		function templates($_side) {
			if($_side == "l") {
				return $this->templates_left;
			} else if($_side == "r") {
				return $this->templates_right;
			}
			return FALSE;
		}

		function dictionary($_side) {
			if($_side == 'left') {
				return $this->left;
			} else if($_side == 'bilingual') {
				return $this->bidix;
			} else if($_side == 'right') {
				return $this->right;
			}
		}

		function commit($e_left, $e_bidix, $e_right) {
			$this->left->commit($this->cachedir, $e_left);
			$this->right->commit($this->cachedir, $e_right);
			$this->bidix->commit($this->cachedir, $e_bidix);
		}
	}
?>
