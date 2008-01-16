all:
	xmllint --xinclude apertium-en-nl.en.dix.xml > apertium-en-nl.en.dix
	xmllint --xinclude apertium-en-nl.nl.dix.xml > apertium-en-nl.nl.dix
	xmllint --xinclude apertium-en-nl.en-nl.dix.xml > apertium-en-nl.en-nl.dix

	lt-comp lr apertium-en-nl.en.dix en-nl.automorf.bin
	lt-comp lr apertium-en-nl.nl.dix nl-en.automorf.bin

	lt-comp lr apertium-en-nl.en-nl.dix en-nl.autobil.bin
	lt-comp rl apertium-en-nl.en-nl.dix nl-en.autobil.bin

	lt-comp rl apertium-en-nl.nl.dix en-nl.autogen.bin
	lt-comp rl apertium-en-nl.en.dix nl-en.autogen.bin

	lt-comp rl apertium-en-nl.post-nl.dix en-nl.autopgen.bin
	
	apertium-validate-transfer apertium-en-nl.en-nl.t1x
	apertium-preprocess-transfer apertium-en-nl.en-nl.t1x en-nl.t1x.bin
	apertium-validate-interchunk apertium-en-nl.en-nl.t2x
	apertium-preprocess-transfer apertium-en-nl.en-nl.t2x en-nl.t2x.bin
	apertium-validate-postchunk apertium-en-nl.en-nl.t3x
	apertium-preprocess-transfer apertium-en-nl.en-nl.t3x en-nl.t3x.bin

	apertium-validate-transfer apertium-en-nl.nl-en.t1x
	apertium-preprocess-transfer apertium-en-nl.nl-en.t1x nl-en.t1x.bin
	apertium-validate-interchunk apertium-en-nl.nl-en.t2x
	apertium-preprocess-transfer apertium-en-nl.nl-en.t2x nl-en.t2x.bin
	apertium-validate-postchunk apertium-en-nl.nl-en.t3x
	apertium-preprocess-transfer apertium-en-nl.nl-en.t3x nl-en.t3x.bin

	#clean up temp files
	rm apertium-en-nl.en.dix apertium-en-nl.nl.dix apertium-en-nl.en-nl.dix
clean:
	rm -f *.bin apertium-en-nl.en.dix apertium-en-nl.nl.dix apertium-en-nl.en-nl.dix

test:
	echo "I have two beers" | lt-proc en-nl.automorf.bin | apertium-tagger -g en-nl.prob | apertium-pretransfer | apertium-transfer apertium-en-nl.en-nl.t1x en-nl.t1x.bin en-nl.autobil.bin | apertium-interchunk apertium-en-nl.en-nl.t2x en-nl.t2x.bin | apertium-postchunk apertium-en-nl.en-nl.t3x en-nl.t3x.bin | lt-proc -g en-nl.autognl.bin

