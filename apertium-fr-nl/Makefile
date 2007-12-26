all:
	xmllint --xinclude apertium-fr-nl.fr.dix.xml > apertium-fr-nl.fr.dix
	xmllint --xinclude apertium-fr-nl.nl.dix.xml > apertium-fr-nl.nl.dix
	xmllint --xinclude apertium-fr-nl.fr-nl.dix.xml > apertium-fr-nl.fr-nl.dix

	lt-comp lr apertium-fr-nl.fr.dix fr-nl.automorf.bin
	lt-comp lr apertium-fr-nl.nl.dix nl-fr.automorf.bin

	lt-comp lr apertium-fr-nl.fr-nl.dix fr-nl.autobil.bin
	lt-comp rl apertium-fr-nl.fr-nl.dix nl-fr.autobil.bin

	lt-comp rl apertium-fr-nl.nl.dix fr-nl.autogen.bin
	lt-comp rl apertium-fr-nl.fr.dix nl-fr.autogen.bin
	
#	apertium-validate-transfer apertium-fr-nl.fr-nl.t1x
#	apertium-preprocess-transfer apertium-fr-nl.fr-nl.t1x fr-nl.t1x.bin
#	apertium-validate-interchunk apertium-fr-nl.fr-nl.t2x
#	apertium-preprocess-transfer apertium-fr-nl.fr-nl.t2x fr-nl.t2x.bin
#	apertium-validate-postchunk apertium-fr-nl.fr-nl.t3x
#	apertium-preprocess-transfer apertium-fr-nl.fr-nl.t3x fr-nl.t3x.bin
#
	apertium-preprocess-transfer apertium-fr-nl.trules-fr-nl.xml trules-fr-nl.bin
	apertium-preprocess-transfer apertium-fr-nl.trules-nl-fr.xml trules-nl-fr.bin

	apertium-validate-dictionary apertium-fr-nl.post-fr.dix
	lt-comp lr apertium-fr-nl.post-fr.dix nl-fr.autopgen.bin


	#clean up temp files
	rm apertium-fr-nl.fr.dix apertium-fr-nl.nl.dix apertium-fr-nl.fr-nl.dix

clean:
	rm -f *.bin apertium-fr-nl.fr.dix apertium-fr-nl.nl.dix apertium-fr-nl.fr-nl.dix trules-fr-nl.bin

test:
	echo "J'ai deux bières" | lt-proc fr-nl.automorf.bin | apertium-tagger -g fr-nl.prob | apertium-pretransfer | apertium-transfer apertium-fr-nl.fr-nl.t1x fr-nl.t1x.bin fr-nl.autobil.bin | apertium-interchunk apertium-fr-nl.fr-nl.t2x fr-nl.t2x.bin | apertium-postchunk apertium-fr-nl.fr-nl.t3x fr-nl.t3x.bin | lt-proc -g fr-nl.autognl.bin

