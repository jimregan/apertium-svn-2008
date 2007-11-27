<?
	require_once 'pair.php';

	class Config {
		
		public $wd = '';
		public $cd = '';
		public $pairs;
		public $doc;

		function Config($_file) {
			$this->doc = new DOMDocument;
			$this->doc->load($_file);
			$this->parse_config();
		}

		function parse_config() {
			$working_dir = $this->doc->getElementsByTagName('wd');
			$cache_dir = $this->doc->getElementsByTagName('cache');
	
			$this->wd = $working_dir->item(0)->firstChild->wholeText;
			$this->cd = $cache_dir->item(0)->firstChild->wholeText . '/';

			$enabled_pairs = $this->doc->getElementsByTagName('pair');	

			foreach($enabled_pairs as $pair) {
				$pair_name = $pair->getAttribute('n');

				$this->pairs[$pair_name] = new Pair($this->wd, $pair_name, $pair);
				$this->pairs[$pair_name]->cachedir = $this->cd . $pair_name . '/';

				$enabled_tags = $pair->getElementsByTagName('tag');	
	
				foreach($enabled_tags as $tag) {
					$tag_name = $tag->getAttribute('n');
					$show_list = array();

					$shows = $tag->getElementsByTagName('show');

					foreach($shows as $show) {
						$syms = $show->getAttribute('syms');
						array_push($show_list, $syms);
					}

					$sides = $tag->getElementsByTagName('side');

					foreach($sides as $side) {
						$paradigm_display_mode = "";
						$current_side = $side->getAttribute('n');
						$templates = $side->getElementsByTagName('use-template');

						foreach($templates as $template) {
							$tag  = $template->getAttribute('tags');
							$file = $template->getAttribute('file');
							$this->pairs[$pair_name]->add_template($file, $tag, $current_side);
						}

						$gloss_sections = $side->getElementsByTagName('paradigms');

						foreach($gloss_sections as $gloss_section) {
							$paradigm_display_mode = $gloss_section->getAttribute('display');
							$glosses = $gloss_section->getElementsByTagName('paradigm');

							foreach($glosses as $gloss) {
								$name  = $gloss->getAttribute('n');
								$gloss = $gloss->getAttribute('c');
								//print $name . ' ~ ' . $gloss . "\n";
								$this->pairs[$pair_name]->add_gloss($name, $gloss, $current_side);
							}
						}

						$this->pairs[$pair_name]->set_display_mode($tag_name, $paradigm_display_mode, $current_side);
					}

					$this->pairs[$pair_name]->add_tag($tag_name, $show_list);
				}

				$this->pairs[$pair_name]->populate();
			}
		}

		function pairs() {
			return $this->pairs;
		}

		function get_pair($_name) {
			return $this->pairs[$_name];
		}
	}
	
?>
