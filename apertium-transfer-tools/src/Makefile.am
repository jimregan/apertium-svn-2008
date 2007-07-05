# author: Felipe Sánchez Martínez

bin_PROGRAMS = apertium-xtract-bilingual-phrases apertium-xtract-alignment-templates apertium-gen-transfer-from-aligment-templates \
               apertium-symmetrize-alignments apertium-posttransfer

bin_SCRIPTS = giza++A3-to-apertium apertium-validate-transfer-at apertium-preprocess-corpus-transfer-at apertium-switch-alignments \
              apertium-validate-posttransfer

apertium_transfer_toolsdir=$(prefix)/apertium-transfer-tools-$(GENERIC_API_VERSION)
apertium_transfer_tools_DATA = apertium-transfer-at.dtd apertium-posttransfer.dtd

EXTRA_DIST = giza++A3-to-apertium.awk apertium-transfer-at.dtd validate-xml-dtd.sh preprocess-corpus.awk switch-alignments.awk \
             apertium-posttransfer.dtd

apertium_xtract_bilingual_phrases_SOURCES =  apertium-xtract-bilingual-phrases.C Alignment.H Alignment.C \
       Utils.H Utils.C zfstream.H zfstream.C
apertium_xtract_bilingual_phrases_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS) $(Z_LIBS)

apertium_xtract_alignment_templates_SOURCES = apertium-xtract-alignment-templates.C Alignment.H Alignment.C \
       Utils.H Utils.C LexicalizedWords.H LexicalizedWords.C ATXReader.H ATXReader.C zfstream.H zfstream.C \
       AlignmentTemplate.H AlignmentTemplate.C
apertium_xtract_alignment_templates_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS) $(Z_LIBS)

apertium_gen_transfer_from_aligment_templates_SOURCES = apertium-gen-transfer-from-aligment-templates.C \
       AlignmentTemplate.H AlignmentTemplate.C Alignment.H Alignment.C LexicalizedWords.H LexicalizedWords.C \
       Utils.H Utils.C zfstream.H zfstream.C TransferRule.H TransferRule.C
apertium_gen_transfer_from_aligment_templates_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS) $(Z_LIBS) -lm

apertium_symmetrize_alignments_SOURCES = apertium-symmetrize-alignments.C Alignment.H Alignment.C Utils.H Utils.C \
       zfstream.H zfstream.C 
apertium_symmetrize_alignments_LDADD = $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS) $(Z_LIBS)

apertium_posttransfer_SOURCES = apertium-posttransfer.C PTXReader.H PTXReader.C Utils.H Utils.C
apertium_posttransfer_LDADD = $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS) $(Z_LIBS)

INCLUDES = $(LIBXML2_CFLAGS) $(LTTOOLBOX_CFLAGS) $(APERTIUM_CFLAGS) $(Z_CFLAGS)

CLEANFILES = *~ $(bin_SCRIPTS)

giza++A3-to-apertium:  giza++A3-to-apertium.awk Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(AWK) -f" > $@
	@cat  giza++A3-to-apertium.awk >> $@
	@chmod a+x $@

apertium-switch-alignments: switch-alignments.awk Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(AWK) -f" > $@
	@cat switch-alignments.awk >> $@
	@chmod a+x $@

apertium-analyse-alignments: apertium-analyse-alignments.sh Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(BASH) " > $@
	@cat  apertium-analyse-alignments.sh >> $@
	@chmod a+x $@

apertium-validate-transfer-at: validate-xml-dtd.sh Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(BASH)" > $@
	@echo "XMLLINT=$(XMLLINT)" >> $@
	@echo "DTD=$(apertium_transfer_toolsdir)/apertium-transfer-at.dtd" >> $@
	@cat validate-xml-dtd.sh >> $@
	@chmod a+x $@

apertium-preprocess-corpus-transfer-at:  preprocess-corpus.awk Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(AWK) -f" > $@
	@cat  preprocess-corpus.awk >> $@
	@chmod a+x $@

apertium-validate-posttransfer: validate-xml-dtd.sh Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(BASH)" > $@
	@echo "XMLLINT=$(XMLLINT)" >> $@
	@echo "DTD=$(apertium_transfer_toolsdir)/apertium-posttransfer.dtd" >> $@
	@cat validate-xml-dtd.sh >> $@
	@chmod a+x $@
