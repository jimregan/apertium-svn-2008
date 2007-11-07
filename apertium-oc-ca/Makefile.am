VERSION=1.0.2
LANG1=oc
LANG2=ca
LANG1b=oc@aran
LANG2b=ca@aran
PREFIX1=$(LANG1)-$(LANG2)
PREFIX1b=$(LANG1b)-$(LANG2)
PREFIX2=$(LANG2)-$(LANG1)
PREFIX2b=$(LANG2)-$(LANG1b)

BASENAME=apertium-$(PREFIX1)

TARGETS_COMMON = $(PREFIX1).automorf.bin $(PREFIX1b).automorf.bin \
                 $(PREFIX2).automorf.bin $(PREFIX2b).automorf.bin \
                 $(PREFIX1).autobil.bin $(PREFIX1b).autobil.bin \
                 $(PREFIX2).autobil.bin $(PREFIX2b).autobil.bin \
                 $(PREFIX1).autogen.bin $(PREFIX1b).autogen.bin \
                 $(PREFIX2).autogen.bin $(PREFIX2b).autogen.bin \
                 $(PREFIX1).autopgen.bin $(PREFIX1b).autopgen.bin \
                 $(PREFIX2).autopgen.bin $(PREFIX2b).autopgen.bin 
     
TARGETS = $(TARGETS_COMMON) trules-$(PREFIX1).bin trules-$(PREFIX2).bin \
          trules-$(PREFIX1).xml trules-$(PREFIX2).xml $(PREFIX1).t1x.bin $(PREFIX1).t2x.bin $(PREFIX1).t3x.bin \
          $(PREFIX1b).t1x.bin $(PREFIX1b).t2x.bin $(PREFIX1b).t3x.bin \
          $(PREFIX2).t1x.bin $(PREFIX2).t2x.bin $(PREFIX2).t3x.bin \
          $(PREFIX2b).t1x.bin $(PREFIX2b).t2x.bin $(PREFIX2b).t3x.bin \
          $(PREFIX1).t1x. $(PREFIX1).t2x $(PREFIX1).t3x \
          $(PREFIX1b).t1x $(PREFIX1b).t2x $(PREFIX1b).t3x \
          $(PREFIX2).t1x $(PREFIX2).t2x $(PREFIX2).t3x \
          $(PREFIX2b).t1x $(PREFIX2b).t2x $(PREFIX2b).t3x 

.deps:
	mkdir .deps
	
.deps/$(LANG1).metadix: $(BASENAME).$(LANG1).metadix .deps
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(LANG1).metadix >$@

.deps/$(LANG1).dix: .deps/$(LANG1).metadix
	xsltproc buscaPar.xsl .deps/$(LANG1).metadix | uniq >$$$$tmp1 && \
	xsltproc $$$$tmp1 .deps/$(LANG1).metadix > $@ && rm $$$$tmp1

.deps/$(LANG1b).metadix: $(BASENAME).$(LANG1).metadix .deps
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(LANG1).metadix >$@

.deps/$(LANG1b).dix: .deps/$(LANG1b).metadix
	xsltproc buscaPar.xsl .deps/$(LANG1b).metadix | uniq >$$$$tmp1 && \
	xsltproc $$$$tmp1 .deps/$(LANG1b).metadix > $@ && rm $$$$tmp1

.deps/$(LANG2).dix: $(BASENAME).$(LANG2).dix .deps
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(LANG2).dix >$@

.deps/$(LANG2b).dix: $(BASENAME).$(LANG2).dix .deps
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(LANG2).dix >$@

.deps/$(PREFIX1).dix: $(BASENAME).$(PREFIX1).dix .deps
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX1).dix >$@

.deps/$(PREFIX1b).dix: $(BASENAME).$(PREFIX1).dix .deps
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX1).dix >$@

.deps/$(PREFIX1).post.dix: $(BASENAME).post-$(LANG2).dix .deps
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).post-$(LANG2).dix >$@

.deps/$(PREFIX1b).post.dix: $(BASENAME).post-$(LANG2).dix .deps
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).post-$(LANG2).dix >$@

.deps/$(LANG1).post.dix: $(BASENAME).post-$(LANG1).dix .deps
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).post-$(LANG1).dix >$@

.deps/$(LANG1b).post.dix: $(BASENAME).post-$(LANG1).dix .deps
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).post-$(LANG1).dix >$@

.deps/$(LANG2).post.dix: $(BASENAME).post-$(LANG2).dix .deps
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).post-$(LANG2).dix >$@

.deps/$(LANG2b).post.dix: $(BASENAME).post-$(LANG2).dix .deps
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).post-$(LANG2).dix >$@
		             
$(PREFIX1).automorf.bin: .deps/$(LANG1).dix
	apertium-validate-dictionary .deps/$(LANG1).dix
	lt-comp lr .deps/$(LANG1).dix $@

$(PREFIX1b).automorf.bin: .deps/$(LANG1b).dix
	apertium-validate-dictionary .deps/$(LANG1b).dix
	lt-comp lr .deps/$(LANG1b).dix $@

$(PREFIX2).automorf.bin: .deps/$(LANG2).dix
	apertium-validate-dictionary .deps/$(LANG2).dix
	lt-comp lr .deps/$(LANG2).dix $@

$(PREFIX2b).automorf.bin: .deps/$(LANG2b).dix
	apertium-validate-dictionary .deps/$(LANG2b).dix
	lt-comp lr .deps/$(LANG2b).dix $@

$(PREFIX1).autobil.bin: .deps/$(PREFIX1).dix
	apertium-validate-dictionary .deps/$(PREFIX1).dix
	lt-comp lr .deps/$(PREFIX1).dix $@

$(PREFIX1b).autobil.bin: .deps/$(PREFIX1b).dix
	apertium-validate-dictionary .deps/$(PREFIX1b).dix
	lt-comp lr .deps/$(PREFIX1b).dix $@

$(PREFIX2).autobil.bin: .deps/$(PREFIX1).dix
	apertium-validate-dictionary .deps/$(PREFIX1).dix
	lt-comp rl .deps/$(PREFIX1).dix $@

$(PREFIX2b).autobil.bin: .deps/$(PREFIX1b).dix
	apertium-validate-dictionary .deps/$(PREFIX1b).dix
	lt-comp rl .deps/$(PREFIX1b).dix $@

$(PREFIX1).autogen.bin: .deps/$(LANG2).dix
	apertium-validate-dictionary .deps/$(LANG2).dix
	lt-comp rl .deps/$(LANG2).dix $@

$(PREFIX1b).autogen.bin: .deps/$(LANG2b).dix
	apertium-validate-dictionary .deps/$(LANG2b).dix
	lt-comp rl .deps/$(LANG2b).dix $@

$(PREFIX2).autogen.bin: .deps/$(LANG1).dix
	apertium-validate-dictionary .deps/$(LANG1).dix
	lt-comp rl .deps/$(LANG1).dix $@

$(PREFIX2b).autogen.bin: .deps/$(LANG1b).dix
	apertium-validate-dictionary .deps/$(LANG1b).dix
	lt-comp rl .deps/$(LANG1b).dix $@

$(PREFIX1).autopgen.bin: .deps/$(LANG2).post.dix
	apertium-validate-dictionary .deps/$(LANG2).post.dix
	lt-comp lr .deps/$(LANG2).post.dix $@

$(PREFIX1b).autopgen.bin: .deps/$(LANG2b).post.dix
	apertium-validate-dictionary .deps/$(LANG2b).post.dix
	lt-comp lr .deps/$(LANG2b).post.dix $@

$(PREFIX2).autopgen.bin: .deps/$(LANG1).post.dix
	apertium-validate-dictionary .deps/$(LANG1).post.dix
	lt-comp lr .deps/$(LANG1).post.dix $@

$(PREFIX2b).autopgen.bin: .deps/$(LANG1b).post.dix
	apertium-validate-dictionary .deps/$(LANG1b).post.dix
	lt-comp lr .deps/$(LANG1b).post.dix $@
	
$(PREFIX1).t1x: $(BASENAME).$(PREFIX1).t1x
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX1).t1x > $@
	apertium-validate-transfer $@

$(PREFIX1b).t1x: $(BASENAME).$(PREFIX1).t1x
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX1).t1x > $@
	apertium-validate-transfer $@

$(PREFIX2).t1x: $(BASENAME).$(PREFIX2).t1x
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX2).t1x > $@
	apertium-validate-transfer $@

$(PREFIX2b).t1x: $(BASENAME).$(PREFIX2).t1x
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX2).t1x > $@
	apertium-validate-transfer $@

$(PREFIX1).t2x: $(BASENAME).$(PREFIX1).t2x
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX1).t2x > $@
	apertium-validate-interchunk $@

$(PREFIX1b).t2x: $(BASENAME).$(PREFIX1).t2x
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX1).t2x > $@
	apertium-validate-interchunk $@

$(PREFIX2).t2x: $(BASENAME).$(PREFIX2).t2x
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX2).t2x > $@
	apertium-validate-interchunk $@

$(PREFIX2b).t2x: $(BASENAME).$(PREFIX2).t2x
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX2).t2x > $@
	apertium-validate-interchunk $@
	
$(PREFIX1).t3x: $(BASENAME).$(PREFIX1).t3x
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX1).t3x > $@
	apertium-validate-postchunk $@

$(PREFIX1b).t3x: $(BASENAME).$(PREFIX1).t3x
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX1).t3x > $@
	apertium-validate-postchunk $@

$(PREFIX2).t3x: $(BASENAME).$(PREFIX2).t3x
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX2).t3x > $@
	apertium-validate-postchunk $@

$(PREFIX2b).t3x: $(BASENAME).$(PREFIX2).t3x
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX2).t3x > $@
	apertium-validate-postchunk $@

$(PREFIX1).t1x.bin: $(PREFIX1).t1x
	apertium-preprocess-transfer $(PREFIX1).t1x $@

$(PREFIX1b).t1x.bin: $(PREFIX1b).t1x
	apertium-preprocess-transfer $(PREFIX1b).t1x $@

$(PREFIX2).t1x.bin: $(PREFIX2).t1x
	apertium-preprocess-transfer $(PREFIX2).t1x $@

$(PREFIX2b).t1x.bin: $(PREFIX2b).t1x
	apertium-preprocess-transfer $(PREFIX2b).t1x $@

$(PREFIX1).t2x.bin: $(PREFIX1).t2x
	apertium-preprocess-transfer $(PREFIX1).t2x $@

$(PREFIX1b).t2x.bin: $(PREFIX1b).t2x
	apertium-preprocess-transfer $(PREFIX1b).t2x $@

$(PREFIX2).t2x.bin: $(PREFIX2).t2x
	apertium-preprocess-transfer $(PREFIX2).t2x $@

$(PREFIX2b).t2x.bin: $(PREFIX2b).t2x
	apertium-preprocess-transfer $(PREFIX2b).t2x $@

$(PREFIX1).t3x.bin: $(PREFIX1).t3x
	apertium-preprocess-transfer $(PREFIX1).t3x $@

$(PREFIX1b).t3x.bin: $(PREFIX1b).t3x
	apertium-preprocess-transfer $(PREFIX1b).t3x $@

$(PREFIX2).t3x.bin: $(PREFIX2).t3x
	apertium-preprocess-transfer $(PREFIX2).t3x $@

$(PREFIX2b).t3x.bin: $(PREFIX2b).t3x
	apertium-preprocess-transfer $(PREFIX2b).t3x $@

trules-$(PREFIX1).bin: $(BASENAME).trules-$(PREFIX1).xml
	apertium-validate-transfer $(BASENAME).trules-$(PREFIX1).xml
	apertium-preprocess-transfer $(BASENAME).trules-$(PREFIX1).xml \
	                             trules-$(PREFIX1).bin
	                             
trules-$(PREFIX2).bin: $(BASENAME).trules-$(PREFIX2).xml
	apertium-validate-transfer $(BASENAME).trules-$(PREFIX2).xml
	apertium-preprocess-transfer $(BASENAME).trules-$(PREFIX2).xml \
	                             trules-$(PREFIX2).bin

trules-$(PREFIX1).xml: $(BASENAME).trules-$(PREFIX1).xml
	ln -s $(BASENAME).trules-$(PREFIX1).xml trules-$(PREFIX1).xml

trules-$(PREFIX2).xml: $(BASENAME).trules-$(PREFIX2).xml
	ln -s $(BASENAME).trules-$(PREFIX2).xml trules-$(PREFIX2).xml

EXTRA_DIST=$(BASENAME).$(PREFIX1).dix \
           $(BASENAME).post-$(LANG1).dix $(BASENAME).post-$(LANG2).dix \
           $(BASENAME).$(LANG2).dix $(BASENAME).$(LANG1).tsx \
           $(BASENAME).$(LANG2).tsx $(BASENAME).trules-$(PREFIX1).xml \
           $(BASENAME).trules-$(PREFIX2).xml $(PREFIX1).prob $(PREFIX2).prob \
           buscaPar.xsl principal.xsl apertium-oc-ca.oc.metadix modes.xml \
           $(PREFIX1)-supervised.make $(PREFIX1)-unsupervised.make \
           $(PREFIX2)-supervised.make $(PREFIX2)-unsupervised.make \
           $(LANG1)-tagger-data/README $(LANG2)-tagger-data/README 
                                  
modes: modes.xml
	apertium-gen-modes modes.xml
	
$(PREFIX1).mode $(PREFIX2).mode $(PREFIX1b).mode $(PREFIX2b).mode: modes.xml
	apertium-gen-modes modes.xml

apertium_oc_cadir=$(prefix)/share/apertium/apertium-$(PREFIX1)/
apertium_oc_camodesdir=$(prefix)/share/apertium/modes/
	           
apertium_oc_ca_DATA=$(PREFIX1).automorf.bin $(PREFIX2).automorf.bin \
                    $(PREFIX1).autobil.bin $(PREFIX2).autobil.bin \
                    $(PREFIX1).autogen.bin $(PREFIX2).autogen.bin \
                    $(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin \
                    $(PREFIX1b).automorf.bin $(PREFIX2b).automorf.bin \
                    $(PREFIX1b).autobil.bin $(PREFIX2b).autobil.bin \
                    $(PREFIX1b).autogen.bin $(PREFIX2b).autogen.bin \
                    $(PREFIX1b).autopgen.bin $(PREFIX2b).autopgen.bin \
                    $(PREFIX1).prob $(PREFIX2).prob trules-$(PREFIX1).xml \
                    trules-$(PREFIX2).xml trules-$(PREFIX1).bin \
                    trules-$(PREFIX2).bin $(MODES) \
                    $(PREFIX1).t1x.bin $(PREFIX1).t2x.bin $(PREFIX1).t3x.bin \
                    $(PREFIX1b).t1x.bin $(PREFIX1b).t2x.bin $(PREFIX1b).t3x.bin \
                    $(PREFIX2).t1x.bin $(PREFIX2).t2x.bin $(PREFIX2).t3x.bin \
                    $(PREFIX2b).t1x.bin $(PREFIX2b).t2x.bin $(PREFIX2b).t3x.bin \
                    $(PREFIX1).t1x $(PREFIX1).t2x $(PREFIX1).t3x \
                    $(PREFIX1b).t1x $(PREFIX1b).t2x $(PREFIX1b).t3x \
                    $(PREFIX2).t1x $(PREFIX2).t2x $(PREFIX2).t3x \
                    $(PREFIX2b).t1x $(PREFIX2b).t2x $(PREFIX2b).t3x \
                    $(PREFIX1).mode $(PREFIX2).mode $(PREFIX1b).mode $(PREFIX2b).mode

install-data-local:
	apertium-gen-modes modes.xml apertium-$(PREFIX1)
	$(INSTALL_DATA) $(PREFIX1).mode $(apertium_oc_camodesdir)
	$(INSTALL_DATA) $(PREFIX1b).mode $(apertium_oc_camodesdir)
	$(INSTALL_DATA) $(PREFIX2).mode $(apertium_oc_camodesdir)
	$(INSTALL_DATA) $(PREFIX2).mode $(apertium_oc_camodesdir)
	
CLEANFILES = -r $(TARGETS) .deps
