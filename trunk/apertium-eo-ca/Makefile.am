VERSION=0.8
LANG1=eo
LANG2=ca
PREFIX1=$(LANG2)-$(LANG1)
PREFIX2=$(LANG2)-$(LANG1)-translit

BASENAME=apertium-eo-ca
BILNAME=apertium-eo-ca.eo-ca.dix

TARGETS_COMMON = $(PREFIX1).automorf.bin $(PREFIX1).autobil.bin $(PREFIX1).autogen.bin \
     $(PREFIX2).autobil.bin $(PREFIX2).autogen.bin $(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin \
     $(PREFIX1).t1x.bin $(PREFIX1).t2x.bin $(PREFIX1).t3x.bin \
     $(PREFIX2).t1x.bin $(PREFIX2).t2x.bin $(PREFIX2).t3x.bin modes $(PREFIX1).mode $(PREFIX2).mode

TARGETS = $(TARGETS_COMMON)

$(PREFIX1).automorf.bin: $(BASENAME).$(LANG2).dix
	apertium-validate-dictionary $(BASENAME).$(LANG2).dix
	lt-comp lr $(BASENAME).$(LANG2).dix $@

$(PREFIX1).autobil.bin: $(BILNAME)
	apertium-validate-dictionary $(BILNAME)
	lt-comp rl $(BILNAME) $@

$(BILNAME).translit: $(BILNAME)
	./transliterate.sh $(BILNAME) >$@

$(PREFIX2).autobil.bin: $(BILNAME).translit
	apertium-validate-dictionary $(BILNAME).translit
	lt-comp rl $(BILNAME).translit $@

$(PREFIX1).autogen.bin: $(BASENAME).$(LANG1).dix
	apertium-validate-dictionary $(BASENAME).$(LANG1).dix
	lt-comp rl $(BASENAME).$(LANG1).dix $@
	
$(BASENAME).$(LANG1).dix.translit: $(BASENAME).$(LANG1).dix
	./transliterate.sh $(BASENAME).$(LANG1).dix >$@

$(PREFIX2).autogen.bin: $(BASENAME).$(LANG1).dix.translit
	apertium-validate-dictionary $(BASENAME).$(LANG1).dix.translit
	lt-comp rl $(BASENAME).$(LANG1).dix.translit $@

$(PREFIX1).autopgen.bin: $(BASENAME).post-$(LANG1).dix
	apertium-validate-dictionary $(BASENAME).post-$(LANG1).dix
	lt-comp lr $(BASENAME).post-$(LANG1).dix $@
	
$(BASENAME).post-$(LANG1).dix.translit: $(BASENAME).post-$(LANG1).dix
	./transliterate.sh $(BASENAME).post-$(LANG1).dix >$@

$(PREFIX2).autopgen.bin: $(BASENAME).post-$(LANG1).dix.translit
	apertium-validate-dictionary $(BASENAME).post-$(LANG1).dix.translit
	lt-comp lr $(BASENAME).post-$(LANG1).dix.translit $@

$(PREFIX1).t1x.bin: $(BASENAME).$(PREFIX1).t1x
	apertium-validate-transfer $(BASENAME).$(PREFIX1).t1x
	apertium-preprocess-transfer $(BASENAME).$(PREFIX1).t1x $@

$(BASENAME).$(PREFIX1).t1x.translit: $(BASENAME).$(PREFIX1).t1x
	./transliterate.sh $(BASENAME).$(PREFIX1).t1x >$@

$(PREFIX2).t1x.bin: $(BASENAME).$(PREFIX1).t1x.translit
	apertium-validate-transfer $(BASENAME).$(PREFIX1).t1x.translit
	apertium-preprocess-transfer $(BASENAME).$(PREFIX1).t1x.translit  $@


$(PREFIX1).t2x.bin: $(BASENAME).$(PREFIX1).t2x
	apertium-validate-interchunk $(BASENAME).$(PREFIX1).t2x
	apertium-preprocess-transfer $(BASENAME).$(PREFIX1).t2x $@

$(BASENAME).$(PREFIX1).t2x.translit: $(BASENAME).$(PREFIX1).t2x
	./transliterate.sh $(BASENAME).$(PREFIX1).t2x >$@

$(PREFIX2).t2x.bin: $(BASENAME).$(PREFIX1).t2x.translit
	apertium-validate-interchunk $(BASENAME).$(PREFIX1).t2x.translit
	apertium-preprocess-transfer $(BASENAME).$(PREFIX1).t2x.translit $@


$(PREFIX1).t3x.bin: $(BASENAME).$(PREFIX1).t3x
	apertium-validate-postchunk $(BASENAME).$(PREFIX1).t3x
	apertium-preprocess-transfer $(BASENAME).$(PREFIX1).t3x $@

$(BASENAME).$(PREFIX1).t3x.translit: $(BASENAME).$(PREFIX1).t3x
	./transliterate.sh $(BASENAME).$(PREFIX1).t3x >$@

$(PREFIX2).t3x.bin: $(BASENAME).$(PREFIX1).t3x.translit
	apertium-validate-postchunk $(BASENAME).$(PREFIX1).t3x.translit
	apertium-preprocess-transfer $(BASENAME).$(PREFIX1).t3x.translit $@

EXTRA_DIST=$(BASENAME).$(LANG1).dix $(BILNAME) \
	   $(BASENAME).post-$(LANG1).dix \
	   $(BASENAME).$(LANG2).dix \
	   $(PREFIX1).prob \
	   $(BASENAME).$(PREFIX1).t1x $(BASENAME).$(PREFIX1).t2x \
	   $(BASENAME).$(PREFIX1).t3x \
	   transliterate.sh modes.xml

apertium_eo_ca_DATA=$(PREFIX1).automorf.bin \
		    $(PREFIX1).autobil.bin $(PREFIX2).autobil.bin \
		    $(PREFIX1).autogen.bin $(PREFIX2).autogen.bin \
		    $(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin \
		    $(PREFIX1).prob \
		    $(PREFIX1).t1x.bin $(PREFIX1).t2x.bin $(PREFIX1).t3x.bin \
		    $(PREFIX2).t1x.bin $(PREFIX2).t2x.bin $(PREFIX2).t3x.bin \
		    $(BASENAME).$(PREFIX1).t1x $(BASENAME).$(PREFIX1).t2x $(BASENAME).$(PREFIX1).t3x \
		    $(BASENAME).$(PREFIX1).t1x.translit $(BASENAME).$(PREFIX1).t2x.translit $(BASENAME).$(PREFIX1).t3x.translit \
		    $(PREFIX1).mode $(PREFIX2).mode

apertium_eo_cadir=$(prefix)/share/apertium/apertium-$(PREFIX1)/
apertium_eo_camodesdir=$(prefix)/share/apertium/modes/

$(PREFIX1).mode: modes.xml
	apertium-gen-modes modes.xml
	cp *.mode modes/

$(PREFIX2).mode: modes.xml
	apertium-gen-modes modes.xml
	cp *.mode modes/

modes: modes.xml
	apertium-gen-modes modes.xml

install-data-local:
	apertium-gen-modes modes.xml apertium-$(PREFIX1)
	$(INSTALL_DATA) $(PREFIX1).mode $(apertium_eo_camodesdir)
	$(INSTALL_DATA) $(PREFIX2).mode $(apertium_eo_camodesdir)
	$(INSTALL_DATA) $(BASENAME).$(PREFIX1).t1x $(BASENAME).$(PREFIX1).t2x $(apertium_eo_cadir)
	$(INSTALL_DATA) $(BASENAME).$(PREFIX1).t3x $(BASENAME).$(PREFIX1).t1x.translit $(apertium_eo_cadir)
	$(INSTALL_DATA) $(BASENAME).$(PREFIX1).t2x.translit $(BASENAME).$(PREFIX1).t3x.translit $(apertium_eo_cadir)


CLEANFILES = -rf $(TARGETS) *.translit modes 

