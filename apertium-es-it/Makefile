LD_LIBRARY_PATH=/home/spectre/local/lib 
PATH=/home/spectre/local/bin:$PATH 

all:
	xmllint --xinclude apertium-es-it.es.dix.xml > apertium-es-it.es.dix
	xmllint --xinclude apertium-es-it.it.dix.xml > apertium-es-it.it.dix
	xmllint --xinclude apertium-es-it.es-it.dix.xml > apertium-es-it.es-it.dix

	lt-comp lr apertium-es-it.es.dix es-it.automorf.bin
	lt-comp lr apertium-es-it.it.dix it-es.automorf.bin

	lt-comp lr apertium-es-it.es-it.dix es-it.autobil.bin
	lt-comp rl apertium-es-it.es-it.dix it-es.autobil.bin

	lt-comp rl apertium-es-it.it.dix es-it.autogen.bin
	lt-comp rl apertium-es-it.es.dix it-es.autogen.bin
	
#	apertium-validate-transfer apertium-es-it.es-it.t1x
#	apertium-preprocess-transfer apertium-es-it.es-it.t1x es-it.t1x.bin
#	apertium-validate-interchunk apertium-es-it.es-it.t2x
#	apertium-preprocess-transfer apertium-es-it.es-it.t2x es-it.t2x.bin
#	apertium-validate-postchunk apertium-es-it.es-it.t3x
#	apertium-preprocess-transfer apertium-es-it.es-it.t3x es-it.t3x.bin

	#clean up temp files
	rm apertium-es-it.es.dix apertium-es-it.it.dix apertium-es-it.es-it.dix

clean:
	rm -f *.bin apertium-es-it.es.dix apertium-es-it.it.dix apertium-es-it.es-it.dix
