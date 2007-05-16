VERSION=1.0.2
LANG1=es
LANG2=pt
LANG2b=pt_BR
PREFIX1=$(LANG1)-$(LANG2)
PREFIX1b=$(LANG1)-$(LANG2b)
PREFIX2=$(LANG2)-$(LANG1)

BASENAME=apertium-es-pt

TARGETS_COMMON = $(PREFIX1).automorf.bin $(PREFIX1).autobil.bin $(PREFIX1).autogen.bin \
     $(PREFIX1).autopgen.bin $(PREFIX1b).autobil.bin $(PREFIX1b).autogen.bin\
     $(PREFIX2).automorf.bin $(PREFIX2).autobil.bin \
     $(PREFIX2).autogen.bin $(PREFIX2).autopgen.bin modes 

TARGETS = $(TARGETS_COMMON) trules-$(PREFIX1).bin trules-$(PREFIX2).bin trules-$(PREFIX1b).bin \
          trules-$(PREFIX1).xml trules-$(PREFIX2).xml trules-$(PREFIX1b).xml

$(PREFIX1).automorf.bin: $(BASENAME).$(LANG1).dix
	apertium-validate-dictionary $(BASENAME).$(LANG1).dix
	lt-comp lr $(BASENAME).$(LANG1).dix $@

$(PREFIX1).autobil.bin: es-pt.dix
	apertium-validate-dictionary es-pt.dix
	lt-comp lr es-pt.dix $@

$(PREFIX1).autogen.bin: pt.dix
	apertium-validate-dictionary pt.dix
	lt-comp rl pt.dix $@

$(PREFIX1b).autobil.bin: es-pt_BR.dix
	apertium-validate-dictionary es-pt_BR.dix
	lt-comp lr es-pt_BR.dix $@
                
$(PREFIX1b).autogen.bin: pt_BR.dix
	apertium-validate-dictionary pt_BR.dix
	lt-comp rl pt_BR.dix $@
                                
                                
$(PREFIX1).autopgen.bin: $(BASENAME).post-$(LANG2).dix
	apertium-validate-dictionary $(BASENAME).post-$(LANG2).dix
	lt-comp lr $(BASENAME).post-$(LANG2).dix $@

$(PREFIX2).automorf.bin: $(BASENAME).$(LANG2).dix
#	apertium-validate-dictionary $(BASENAME).$(LANG2).dix
	lt-comp lr $(BASENAME).$(LANG2).dix $@

$(PREFIX2).autobil.bin: $(BASENAME).$(PREFIX1).dix
#	apertium-validate-dictionary $(BASENAME).$(PREFIX1).dix
	lt-comp rl $(BASENAME).$(PREFIX1).dix $@

$(PREFIX2).autogen.bin: $(BASENAME).$(LANG1).dix
#	apertium-validate-dictionary $(BASENAME).$(LANG1).dix
	lt-comp rl $(BASENAME).$(LANG1).dix $@

$(PREFIX2).autopgen.bin: $(BASENAME).post-$(LANG1).dix
#	apertium-validate-dictionary $(BASENAME).post-$(LANG1).dix
	lt-comp lr $(BASENAME).post-$(LANG1).dix $@

trules-$(PREFIX2).bin: $(BASENAME).trules-$(PREFIX2).xml
	apertium-validate-transfer $(BASENAME).trules-$(PREFIX2).xml
	apertium-preprocess-transfer $(BASENAME).trules-$(PREFIX2).xml \
	                             trules-$(PREFIX2).bin
	                         
trules-$(PREFIX1).xml: $(BASENAME).trules-$(PREFIX1).xml
	./filterrules.sh pt_PT $(BASENAME).trules-$(PREFIX1).xml >$@

trules-$(PREFIX1b).xml: $(BASENAME).trules-$(PREFIX1).xml
	./filterrules.sh pt_BR $(BASENAME).trules-$(PREFIX1).xml >$@	                      
	                             
trules-$(PREFIX1).bin: trules-$(PREFIX1).xml
	apertium-validate-transfer trules-$(PREFIX1).xml
	apertium-preprocess-transfer trules-$(PREFIX1).xml $@

trules-$(PREFIX1b).bin: trules-$(PREFIX1b).xml
	apertium-validate-transfer trules-$(PREFIX1b).xml
	apertium-preprocess-transfer trules-$(PREFIX1b).xml $@

trules-$(PREFIX2).xml: $(BASENAME).trules-$(PREFIX2).xml
	ln -s $(BASENAME).trules-$(PREFIX2).xml trules-$(PREFIX2).xml

EXTRA_DIST=$(BASENAME).$(PREFIX1).dix \
           $(BASENAME).post-$(LANG1).dix $(BASENAME).post-$(LANG2).dix \
           $(BASENAME).$(LANG2).dix $(BASENAME).$(LANG1).tsx \
           $(BASENAME).$(LANG2).tsx $(BASENAME).trules-$(PREFIX1).xml \
           $(BASENAME).trules-$(PREFIX2).xml $(PREFIX1).prob $(PREFIX2).prob \
           $(PREFIX1)-supervised.make $(PREFIX1)-unsupervised.make \
           $(PREFIX2)-supervised.make $(PREFIX2)-unsupervised.make \
           $(LANG1)-tagger-data/README $(LANG2)-tagger-data/README \
           filter.sh filter.xsl filterrules.sh filterrules.xsl
                                  
modes: modes.xml
	apertium-gen-modes modes.xml
	           
apertium_es_ptdir=$(prefix)/share/apertium-1.0/$(PREFIX1)/
apertium_es_pt_DATA=$(PREFIX1).automorf.bin $(PREFIX2).automorf.bin \
                    $(PREFIX1).autobil.bin $(PREFIX2).autobil.bin \
                    $(PREFIX1).autogen.bin $(PREFIX2).autogen.bin \
                    $(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin \
                    $(PREFIX1).prob $(PREFIX2).prob trules-$(PREFIX1).xml \
                    trules-$(PREFIX2).xml trules-$(PREFIX1).bin \
                    $(PREFIX1b).autogen.bin $(PREFIX1b).autobil.bin \
                    trules-$(PREFIX2).bin trules-$(PREFIX1b).bin \
                    trules-$(PREFIX1b).xml modes

CLEANFILES = $(TARGETS) pt.dix pt_BR.dix es-pt.dix es-pt_BR.dix 

pt.dix: $(BASENAME).$(LANG2).dix
	./filter.sh pt left $(BASENAME).$(LANG2).dix >$@
        
pt_BR.dix: $(BASENAME).$(LANG2).dix
	./filter.sh br left $(BASENAME).$(LANG2).dix >$@
                 
es-pt.dix: $(BASENAME).$(PREFIX1).dix
	./filter.sh pt right $(BASENAME).$(PREFIX1).dix >$@
                         
es-pt_BR.dix: $(BASENAME).$(PREFIX1).dix
	./filter.sh br right $(BASENAME).$(PREFIX1).dix >$@
                                 