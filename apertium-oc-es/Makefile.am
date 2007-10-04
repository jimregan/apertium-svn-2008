VERSION=1.0.2
LANG1=oc
LANG2=es
LANG1b=oc@aran
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
          
$(PREFIX1).automorf.bin: $(BASENAME).$(LANG1).metadix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(LANG1).metadix >$$$$tmp1 && \
	xsltproc buscaPar.xsl $$$$tmp1  | uniq > $$$$tmp2 && \
	xsltproc $$$$tmp2 $$$$tmp1 >$$$$tmp3 && \
	apertium-validate-dictionary $$$$tmp3 && \
	lt-comp lr $$$$tmp3 $@ && rm $$$$tmp1 $$$$tmp2 $$$$tmp3

$(PREFIX1b).automorf.bin: $(BASENAME).$(LANG1).metadix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(LANG1).metadix >$$$$tmp1 && \
	xsltproc buscaPar.xsl $$$$tmp1  | uniq > $$$$tmp2 && \
	xsltproc $$$$tmp2 $$$$tmp1 >$$$$tmp3 && \
	apertium-validate-dictionary $$$$tmp3 && \
	lt-comp lr $$$$tmp3 $@ && rm $$$$tmp1 $$$$tmp2 $$$$tmp3

$(PREFIX2).automorf.bin: $(BASENAME).$(LANG2).dix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(LANG2).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX2b).automorf.bin: $(BASENAME).$(LANG2).dix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(LANG2).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX1).autobil.bin: $(BASENAME).$(PREFIX1).dix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX1).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX1b).autobil.bin: $(BASENAME).$(PREFIX1).dix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX1).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX2).autobil.bin: $(BASENAME).$(PREFIX1).dix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(PREFIX1).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp rl $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX2b).autobil.bin: $(BASENAME).$(PREFIX1).dix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(PREFIX1).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp rl $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX1).autogen.bin: $(BASENAME).$(LANG2).dix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(LANG2).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp rl $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX1b).autogen.bin: $(BASENAME).$(LANG2).dix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(LANG2).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp rl $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX2).autogen.bin: $(BASENAME).$(LANG1).metadix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).$(LANG1).metadix >$$$$tmp1 && \
	xsltproc buscaPar.xsl $$$$tmp1  | uniq > $$$$tmp2 && \
	xsltproc $$$$tmp2 $$$$tmp1 >$$$$tmp3 && \
	apertium-validate-dictionary $$$$tmp3 && \
	lt-comp rl $$$$tmp3 $@ && rm $$$$tmp1 $$$$tmp2 $$$$tmp3

$(PREFIX2b).autogen.bin: $(BASENAME).$(LANG1).metadix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).$(LANG1).metadix >$$$$tmp1 && \
	xsltproc buscaPar.xsl $$$$tmp1  | uniq > $$$$tmp2 && \
	xsltproc $$$$tmp2 $$$$tmp1 >$$$$tmp3 && \
	apertium-validate-dictionary $$$$tmp3 && \
	lt-comp rl $$$$tmp3 $@ && rm $$$$tmp1 $$$$tmp2 $$$$tmp3

$(PREFIX1).autopgen.bin: $(BASENAME).post-$(LANG2).dix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).post-$(LANG2).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX1b).autopgen.bin: $(BASENAME).post-$(LANG2).dix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).post-$(LANG2).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX2).autopgen.bin: $(BASENAME).post-$(LANG1).dix
	xsltproc --stringparam aversion $(PREFIX1) aversion.xsl $(BASENAME).post-$(LANG1).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

$(PREFIX2b).autopgen.bin: $(BASENAME).post-$(LANG1).dix
	xsltproc --stringparam aversion $(PREFIX1b) aversion.xsl $(BASENAME).post-$(LANG1).dix >$$$$tmp1 && \
	apertium-validate-dictionary $$$$tmp1 && \
	lt-comp lr $$$$tmp1 $@ && rm $$$$tmp1

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

EXTRA_DIST=$(BASENAME).$(PREFIX1).dix \
           $(BASENAME).post-$(LANG1).dix $(BASENAME).post-$(LANG2).dix \
           $(BASENAME).$(LANG2).dix $(BASENAME).$(LANG1).tsx \
           $(BASENAME).$(LANG2).tsx \
           $(PREFIX1).prob $(PREFIX2).prob \
           buscaPar.xsl principal.xsl apertium-oc-es.oc.metadix modes.xml \
           $(PREFIX1)-supervised.make $(PREFIX1)-unsupervised.make \
           $(PREFIX2)-supervised.make $(PREFIX2)-unsupervised.make \
           $(LANG1)-tagger-data/README $(LANG2)-tagger-data/README 
                                  
           
apertium-oc-es.oc.dix: apertium-oc-es.oc.metadix
	@xsltproc buscaPar.xsl apertium-oc-es.oc.metadix | uniq > $$gen.xsl
	@xsltproc $$gen.xsl apertium-oc-es.oc.metadix >$@
	@rm $$gen.xsl
	
modes: modes.xml
	apertium-gen-modes modes.xml
	           
apertium_oc_esdir=$(prefix)/share/apertium-1.0/$(PREFIX1)/
apertium_oc_es_DATA=$(PREFIX1).automorf.bin $(PREFIX2).automorf.bin \
                    $(PREFIX1).autobil.bin $(PREFIX2).autobil.bin \
                    $(PREFIX1).autogen.bin $(PREFIX2).autogen.bin \
                    $(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin \
                    $(PREFIX1b).automorf.bin $(PREFIX2b).automorf.bin \
                    $(PREFIX1b).autobil.bin $(PREFIX2b).autobil.bin \
                    $(PREFIX1b).autogen.bin $(PREFIX2b).autogen.bin \
                    $(PREFIX1b).autopgen.bin $(PREFIX2b).autopgen.bin \
                    $(PREFIX1).prob $(PREFIX2).prob \
                    $(PREFIX1).t1x.bin $(PREFIX1).t2x.bin $(PREFIX1).t3x.bin \
                    $(PREFIX1b).t1x.bin $(PREFIX1b).t2x.bin $(PREFIX1b).t3x.bin \
                    $(PREFIX2).t1x.bin $(PREFIX2).t2x.bin $(PREFIX2).t3x.bin \
                    $(PREFIX2b).t1x.bin $(PREFIX2b).t2x.bin $(PREFIX2b).t3x.bin \
                    $(PREFIX1).t1x $(PREFIX1).t2x $(PREFIX1).t3x \
                    $(PREFIX1b).t1x $(PREFIX1b).t2x $(PREFIX1b).t3x \
                    $(PREFIX2).t1x $(PREFIX2).t2x $(PREFIX2).t3x \
                    $(PREFIX2b).t1x $(PREFIX2b).t2x $(PREFIX2b).t3x  modes

CLEANFILES = -r $(TARGETS) apertium-oc-es.oc.dix
