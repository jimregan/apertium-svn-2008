PROB=/home/spectre/source/apertium-en-de/de-en.prob

all: clean
	xmllint --xinclude apertium-cy-en.cy.dix.in > apertium-cy-en.cy.dix
	xmllint --xinclude apertium-cy-en.en.dix.in > apertium-cy-en.en.dix
	xmllint --xinclude apertium-cy-en.cy-en.dix.in > apertium-cy-en.cy-en.dix

	apertium-validate-dictionary apertium-cy-en.cy.dix
	apertium-validate-dictionary apertium-cy-en.en.dix
	apertium-validate-dictionary apertium-cy-en.cy-en.dix

	lt-comp lr apertium-cy-en.cy.dix cy-en.automorf.bin
	lt-comp lr apertium-cy-en.en.dix en-cy.automorf.bin

	lt-comp rl apertium-cy-en.en.dix cy-en.autogen.bin
	lt-comp rl apertium-cy-en.cy.dix en-cy.autogen.bin

	lt-comp lr apertium-cy-en.cy-en.dix cy-en.autobil.bin
	lt-comp rl apertium-cy-en.cy-en.dix en-cy.autobil.bin

	apertium-validate-transfer   apertium-cy-en.cy-en.t1x
	apertium-validate-interchunk apertium-cy-en.cy-en.t2x
	apertium-validate-postchunk  apertium-cy-en.cy-en.t3x

	apertium-preprocess-transfer apertium-cy-en.cy-en.t1x cy-en.t1x.bin
	apertium-preprocess-transfer apertium-cy-en.cy-en.t2x cy-en.t2x.bin
	apertium-preprocess-transfer apertium-cy-en.cy-en.t3x cy-en.t3x.bin

clean:
	rm -f *.bin apertium-cy-en.cy.dix apertium-cy-en.en.dix apertium-cy-en.cy-en.dix

test:
	echo "Genir pawb yn rhydd ac yn gydradd â'i gilydd mewn urddas a hawliau." | lt-proc cy-en.automorf.bin | apertium-tagger -g $(PROB) | apertium-pretransfer | apertium-transfer apertium-cy-en.cy-en.t1x cy-en.t1x.bin cy-en.autobil.bin | apertium-interchunk apertium-cy-en.cy-en.t2x cy-en.t2x.bin | apertium-postchunk apertium-cy-en.cy-en.t3x cy-en.t3x.bin | lt-proc -g cy-en.autogen.bin 
