<?
	require_once 'pair.php';

	class Config {
		
		public $wd = '';
		public $pairs;
		public $doc;

		function Config($_file) {
			$this->doc = new DOMDocument;
			$this->doc->load($_file);
			$this->parse_config();
		}

		function parse_config() {
			$working_dir = $this->doc->getElementsByTagName('wd');
	
			$this->wd = $working_dir->item(0)->firstChild->wholeText;

			$enabled_pairs = $this->doc->getElementsByTagName('pair');	

			foreach($enabled_pairs as $pair) {
				$pair_name = $pair->getAttribute('n');

				$this->pairs[$pair_name] = new Pair($this->wd, $pair_name, $pair);

				$enabled_tags = $pair->getElementsByTagName('tag');	
	
				foreach($enabled_tags as $tag) {
					$tag_name = $tag->getAttribute('n');
					$show_list = array();

					$shows = $tag->getElementsByTagName('show');

					foreach($shows as $show) {
						$syms = $show->getAttribute('syms');
						array_push($show_list, $syms);
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
