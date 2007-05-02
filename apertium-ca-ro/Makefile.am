VERSION=0.1
LANG1=ca
LANG2=ro
PREFIX1=$(LANG1)-$(LANG2)
PREFIX2=$(LANG2)-$(LANG1)

BASENAME=apertium-$(PREFIX1)

TARGETS_COMMON = $(PREFIX1).automorf.bin $(PREFIX1).autobil.bin $(PREFIX1).autogen.bin \
     $(PREFIX2).automorf.bin $(PREFIX2).autobil.bin \
     $(PREFIX2).autogen.bin \
     trules-$(PREFIX1).bin \
     trules-$(PREFIX2).bin \
     modes


#     $(PREFIX2).autopgen.bin \
#     $(PREFIX1).t1x.bin $(PREFIX1).t2x.bin $(PREFIX1).t3x.bin
#     $(PREFIX2).t1x.bin $(PREFIX2).t2x.bin $(PREFIX2).t3x.bin

     #$(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin


     
$(PREFIX1).automorf.bin: $(BASENAME).$(LANG1).dix
	apertium-validate-dictionary $(BASENAME).$(LANG1).dix
	lt-comp lr $(BASENAME).$(LANG1).dix $@

$(PREFIX1).autobil.bin: $(BASENAME).$(PREFIX1).dix
	apertium-validate-dictionary $(BASENAME).$(PREFIX1).dix
	lt-comp lr $(BASENAME).$(PREFIX1).dix $@

trules-$(PREFIX1).bin: $(BASENAME).trules-$(PREFIX1).xml
	apertium-validate-transfer $(BASENAME).trules-$(PREFIX1).xml
	apertium-preprocess-transfer $(BASENAME).trules-$(PREFIX1).xml $@
	ln -s $(BASENAME).trules-$(PREFIX1).xml trules-$(PREFIX1).xml

$(PREFIX1).autogen.bin: $(BASENAME).$(LANG2).dix
	apertium-validate-dictionary $(BASENAME).$(LANG2).dix
	lt-comp rl $(BASENAME).$(LANG2).dix $@

#$(PREFIX1).autopgen.bin: $(BASENAME).post-$(LANG2).dix
#	apertium-validate-dictionary $(BASENAME).post-$(LANG2).dix
#	lt-comp lr $(BASENAME).post-$(LANG2).dix $@

$(PREFIX2).automorf.bin: $(BASENAME).$(LANG2).dix
	apertium-validate-dictionary $(BASENAME).$(LANG2).dix
	lt-comp lr $(BASENAME).$(LANG2).dix $@

$(PREFIX2).autobil.bin: $(BASENAME).$(PREFIX1).dix
	apertium-validate-dictionary $(BASENAME).$(PREFIX1).dix
	lt-comp rl $(BASENAME).$(PREFIX1).dix $@

trules-$(PREFIX2).bin: $(BASENAME).trules-$(PREFIX2).xml
	apertium-validate-transfer $(BASENAME).trules-$(PREFIX2).xml
	apertium-preprocess-transfer $(BASENAME).trules-$(PREFIX2).xml $@
	ln -s $(BASENAME).trules-$(PREFIX2).xml trules-$(PREFIX2).xml

$(PREFIX2).autogen.bin: $(BASENAME).$(LANG1).dix
	apertium-validate-dictionary $(BASENAME).$(LANG1).dix
	lt-comp rl $(BASENAME).$(LANG1).dix $@

#$(PREFIX2).autopgen.bin: $(BASENAME).post-$(LANG1).dix
#	apertium-validate-dictionary $(BASENAME).post-$(LANG1).dix
#	lt-comp lr $(BASENAME).post-$(LANG1).dix $@


EXTRA_DIST=$(BASENAME).$(LANG1).dix $(BASENAME).$(PREFIX1).dix \
           $(BASENAME).$(LANG2).dix  \
           $(PREFIX1)-supervised.make $(PREFIX1)-unsupervised.make \
           $(PREFIX2)-supervised.make $(PREFIX2)-unsupervised.make \
           $(LANG1)-tagger-data/README $(LANG2)-tagger-data/README \
           $(BASENAME).$(PREFIX2).t2x $(BASENAME).$(PREFIX2).t3x \
           modes.xml

#           $(PREFIX1).prob $(PREFIX2).prob 
#          $(BASENAME).post-$(LANG1).dix $(BASENAME).post-$(LANG2).dix 

modes: modes.xml
	apertium-gen-modes modes.xml
	
apertium_en_afdir=$(prefix)/share/apertium-1.0/$(PREFIX1)/
apertium_en_af_DATA=$(PREFIX1).automorf.bin $(PREFIX2).automorf.bin \
                    $(PREFIX1).autobil.bin $(PREFIX2).autobil.bin \
                    trules-$(PREFIX1).bin \
                    trules-$(PREFIX2).bin \
		    modes
		  
#		    $(PREFIX2).autopgen.bin \
#                    $(PREFIX1).autogen.bin $(PREFIX2).autogen.bin \
#                    $(PREFIX1).autopgen.bin $(PREFIX2).autopgen.bin 
#                    $(PREFIX1).prob $(PREFIX2).prob 

CLEANFILES = -r $(TARGETS_COMMON)  trules-af-en.xml trules-en-af.xml
