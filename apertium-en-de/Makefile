LANG1=en
LANG2=de
PREFIX1=$(LANG1)-$(LANG2)
PREFIX2=$(LANG2)-$(LANG1)

all: clean
	apertium-validate-dictionary apertium-en-de.en.dix
	lt-comp lr apertium-en-de.en.dix en-de.automorf.bin

	apertium-validate-dictionary apertium-en-de.de.dix
	lt-comp lr apertium-en-de.de.dix de-en.automorf.bin

	apertium-validate-dictionary apertium-en-de.en-de.dix
	lt-comp lr apertium-en-de.en-de.dix en-de.autobil.bin
	apertium-validate-dictionary apertium-en-de.en-de.dix
	lt-comp rl apertium-en-de.en-de.dix de-en.autobil.bin

	apertium-validate-dictionary apertium-en-de.en.dix
	lt-comp rl apertium-en-de.de.dix en-de.autogen.bin
	apertium-validate-dictionary apertium-en-de.de.dix
	lt-comp rl apertium-en-de.en.dix de-en.autogen.bin


	apertium-validate-transfer apertium-en-de.trules-de-en.xml
	apertium-preprocess-transfer apertium-en-de.trules-de-en.xml trules-de-en.bin
	ln -s apertium-en-de.trules-de-en.xml trules-de-en.xml

	apertium-validate-transfer apertium-en-de.trules-en-de.xml
	apertium-preprocess-transfer apertium-en-de.trules-en-de.xml trules-en-de.bin
	ln -s apertium-en-de.trules-en-de.xml trules-en-de.xml

	lt-comp lr apertium-en-de.post-en.dix de-en.autopgen.bin

clean:
	rm -rf *.bin trules-de-en.xml trules-en-de.xml wordmirror/*.pyc


test:
	cat TODO | grep '\#\*' | cut -f2 -d'*' > /tmp/testrun
	cat /tmp/testrun | apertium-destxt | lt-proc de-en.automorf.bin | apertium-pretransfer  | apertium-tagger --tagger de-en.prob | apertium-transfer trules-de-en.xml trules-de-en.bin de-en.autobil.bin | lt-proc -g de-en.autogen.bin | lt-proc -p de-en.autopgen.bin | apertium-retxt
	
