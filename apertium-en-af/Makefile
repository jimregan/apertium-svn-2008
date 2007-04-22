LANG1=en
LANG2=af
PREFIX1=$(LANG1)-$(LANG2)
PREFIX2=$(LANG2)-$(LANG1)
LTCOMP=/home/spectre/local/bin/lt-comp

all: clean
	apertium-validate-dictionary apertium-en-af.en.dix
	$(LTCOMP) lr apertium-en-af.en.dix en-af.automorf.bin

	apertium-validate-dictionary apertium-en-af.af.dix
	$(LTCOMP) lr apertium-en-af.af.dix af-en.automorf.bin

	apertium-validate-dictionary apertium-en-af.en-af.dix
	$(LTCOMP) lr apertium-en-af.en-af.dix en-af.autobil.bin
	apertium-validate-dictionary apertium-en-af.en-af.dix
	$(LTCOMP) rl apertium-en-af.en-af.dix af-en.autobil.bin

	apertium-validate-dictionary apertium-en-af.en.dix
	$(LTCOMP) rl apertium-en-af.af.dix en-af.autogen.bin
	apertium-validate-dictionary apertium-en-af.af.dix
	$(LTCOMP) rl apertium-en-af.en.dix af-en.autogen.bin

	apertium-validate-transfer apertium-en-af.trules-en-af.xml
	apertium-preprocess-transfer apertium-en-af.trules-en-af.xml trules-en-af.bin
	ln -s apertium-en-af.trules-en-af.xml trules-en-af.xml

	apertium-validate-transfer apertium-en-af.trules-af-en.xml
	apertium-preprocess-transfer apertium-en-af.trules-af-en.xml trules-af-en.bin
	ln -s apertium-en-af.trules-af-en.xml trules-af-en.xml

clean:
	rm -rf *.bin trules-af-en.xml trules-en-af.xml

test:
	echo "I have three beers" | apertium-destxt | lt-proc en-af.automorf.bin | apertium-pretransfer  | perl -ne 's,^([^/]*/)(.*)$,\^\2,; s,^(.*\$\s\^)[^/]+/(.*)$,\1\2,; print' | apertium-transfer trules-en-af.xml trules-en-af.bin en-af.autobil.bin | lt-proc -g en-af.autogen.bin | apertium-retxt
