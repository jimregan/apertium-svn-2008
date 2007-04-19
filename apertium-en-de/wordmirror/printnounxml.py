# Francis Tyers

from WordMirror import WordMirrorNounInflection

def print_xml(inflection, noun): #{
	print '    <pardef n="' + noun +  '__n">';

	gender = inflection['Gender'];
	number = "";

	for key in inflection.keys(): #{
		if(key != 'Gender' and key != 'Word Type' and key != 'Plural'): #{
			for item in inflection[key].iteritems(): #{
				if(key == 'No Article' or key == 'Plural No Article'): #{	
					if(key == 'No Article'): #{
						number = 'sg'
					#}
					if(key == 'Plural No Article'): #{
						number = 'pl';
					#}
					print '      <e> <!-- ' + key + ' -->';
					print '        <p>';
					if(item[1] == noun): #{
						print '          <l/>';
					#}
					if(item[1] != noun): #{
						print '          <l>' + item[1].replace(noun, '') + '</l>';
					#}
					if(item[0] == 'Nominative'): #{
						print '          <r><s n="n"/><s n="' + gender + '"/><s n="nom"/><s n="' + number + '"/></r>';
					#}
					if(item[0] == 'Dative'): #{
						print '          <r><s n="n"/><s n="' + gender + '"/><s n="dat"/><s n="' + number + '"/></r>';
					#}
		
					if(item[0] == 'Accusative'): #{
						print '          <r><s n="n"/><s n="' + gender + '"/><s n="acc"/><s n="' + number + '"/></r>';
					#}

					if(item[0] == 'Genitive'): #{
						print '          <r><s n="n"/><s n="' + gender + '"/><s n="gen"/><s n="' + number + '"/></r>';
					#}
					print '        </p>';
					print '      </e>';
				#}
			#}
		#}
	#}
	print '    </pardef>'
#}

def usage(): #{
	print 'python printnounxml.py <noun>';
#}

if __name__ == "__main__":
	import sys;
	if(len(sys.argv) <= 1): #{
		usage();
		sys.exit();
	#}
	inflect = WordMirrorNounInflection(sys.argv[1]);
	print_xml(inflect, sys.argv[1]);
