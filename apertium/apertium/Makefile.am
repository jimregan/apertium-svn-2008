h_sources = Collection.H HMM.H MorphoStream.H TaggerUtils.H TaggerWord.H \
            ConstantManager.H TTag.H ApertiumConfig.H \
            TransferWord.H Tagger.H TransferWordList.H TaggerData.H TransferData.H\
            TSXReader.H TRXReader.H Transfer.H \
            TransferToken.H TransferInstr.H \
            LexTorWord.H LexTorData.H StringUtils.H LexTor.H T2XReader.H \
            Interchunk.H InterchunkWord.H Postchunk.H LexTorEval.H
cc_sources = Collection.C ConstantManager.C HMM.C MorphoStream.C \
             TaggerUtils.C TaggerWord.C TransferWord.C \
             Tagger.C TransferWordList.C TaggerData.C TransferData.C\
             TSXReader.C TRXReader.C Transfer.C \
             TransferToken.C TransferInstr.C \
             LexTorWord.C LexTorData.C StringUtils.C LexTor.C T2XReader.C \
             Interchunk.C InterchunkWord.C Postchunk.C LexTorEval.C

library_includedir = $(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)


GENERATEDSCRIPTS = apertium-gen-deformat apertium-gen-reformat \
                   apertium-translator apertium-validate-tagger \
                   apertium-validate-transfer apertium-validate-dictionary \
                   apertium-validate-modes \
                   apertium-gen-oldbil apertium-gen-lextorbil \
                   apertium-gen-lextormono apertium-gen-wlist-lextor \
                   apertium-preprocess-corpus-lextor \
                   apertium-gen-stopwords-lextor \
                   apertium-translator-lextor apertium-validate-interchunk \
                   apertium-validate-postchunk apertium \
                   apertium-gen-modes

bin_PROGRAMS = apertium-pretransfer apertium-destxt apertium-retxt \
               apertium-deshtml apertium-rehtml \
               apertium-desrtf apertium-rertf apertium-tagger \
               apertium-preprocess-transfer apertium-transfer \
               apertium-filter-ambiguity apertium-lextor \
               apertium-gen-wlist-lextor-translation \
               apertium-unchunker apertium-interchunk apertium-postchunk \
               apertium-lextor-eval ###pruebas-lextor apertium-lextor-search

bin_SCRIPTS =  $(GENERATEDSCRIPTS) apertium-createmodes.awk

instdir = apertium

lib_LTLIBRARIES = libapertium-2.0.la
libapertium_2_0_la_SOURCES = $(h_sources) $(cc_sources)
libapertium_2_0_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

apertiumdir = $(prefix)/apertium-$(GENERIC_API_VERSION)
apertiuminclude = $(prefix)/include/apertium-$(GENERIC_API_VERSION)
apertiumlib = $(prefix)/lib 

apertium_DATA = deformat.xsl reformat.xsl new2old.xsl lexchoice.xsl \
                lexchoicebil.xsl tagger.dtd interchunk.dtd format.dtd \
                transfer.dtd dix.dtd postchunk.dtd modes.dtd modes2bash.xsl

apertium_pretransfer_SOURCES = apertium-pretransfer.C
apertium_destxt_SOURCES = apertium-destxt.C
apertium_retxt_SOURCES = apertium-retxt.C
apertium_deshtml_SOURCES = apertium-deshtml.C
apertium_rehtml_SOURCES = apertium-rehtml.C
apertium_desrtf_SOURCES = apertium-desrtf.C
apertium_rertf_SOURCES = apertium-rertf.C

apertium_tagger_SOURCES = apertium-tagger.C
apertium_tagger_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_preprocess_transfer_SOURCES = transferpp.C
apertium_preprocess_transfer_LDADD = $(APERTIUM_LIBS) \
                                     -lapertium-$(GENERIC_API_VERSION)

apertium_filter_ambiguity_SOURCES = apertium-filter-ambiguity.C
apertium_filter_ambiguity_LDADD = $(APERTIUM_LIBS) \
                                  -lapertium-$(GENERIC_API_VERSION)

apertium_transfer_SOURCES = apertium-transfer.C
apertium_transfer_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_interchunk_SOURCES = apertium-interchunk.C
apertium_interchunk_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_postchunk_SOURCES = apertium-postchunk.C
apertium_postchunk_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_lextor_SOURCES = apertium-lextor.C
apertium_lextor_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_lextor_eval_SOURCES = apertium-lextor-eval.C
apertium_lextor_eval_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

###apertium_lextor_search_SOURCES = apertium-lextor-search.C
###apertium_lextor_search_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

###pruebas_lextor_SOURCES = pruebas-lextor.C
###pruebas_lextor_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_gen_wlist_lextor_translation_SOURCES = apertium-gen-wlist-lextor-translation.C
apertium_gen_wlist_lextor_translation_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

apertium_unchunker_SOURCES = unchunker.C
apertium_unchunker_LDADD = $(APERTIUM_LIBS) -lapertium-$(GENERIC_API_VERSION)

INCLUDES = -I$(top_srcdir) $(APERTIUM_CFLAGS)
CLEANFILES = *~ apertium-destxt.C apertium-retxt.C apertium-deshtml.C \
             apertium-rehtml.C apertium-desrtf.C apertium-rertf.C \
             $(GENERATEDSCRIPTS)

apertium-destxt.C: txt-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) deformat.xsl txt-format.xml |$(FLEX) -Cfer -t >$@

apertium-retxt.C: txt-format.xml Makefile.am reformat.xsl
	$(XSLTPROC) reformat.xsl txt-format.xml |$(FLEX) -Cfer -t >$@

apertium-deshtml.C: html-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) deformat.xsl html-format.xml |$(FLEX) -Cfer -t >$@

apertium-rehtml.C: html-format.xml Makefile.am reformat.xsl
	$(XSLTPROC) reformat.xsl html-format.xml |$(FLEX) -Cfer -t >$@

apertium-desrtf.C: rtf-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) deformat.xsl rtf-format.xml |$(FLEX) -Cfer -t >$@

apertium-rertf.C: rtf-format.xml Makefile.am reformat.xsl
	$(XSLTPROC) reformat.xsl rtf-format.xml |$(FLEX) -Cfer -t >$@

apertium-validate-tagger: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-tagger script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/tagger.dtd --noout \$$FILE1 && exit 0;" >>$@
	@echo "exit 1;" >> $@
	@chmod a+x $@

apertium-validate-transfer: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-transfer script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/transfer.dtd --noout \$$FILE1 && exit 0;" >>$@
	@echo "exit 1;" >> $@
	@chmod a+x $@

apertium-validate-interchunk: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-interchunk script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/interchunk.dtd --noout \$$FILE1 && exit 0;" >>$@
	@echo "exit 1;" >> $@
	@chmod a+x $@

apertium-validate-postchunk: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-postchunk script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/postchunk.dtd --noout \$$FILE1 && exit 0;" >>$@
	@echo "exit 1;" >> $@
	@chmod a+x $@

apertium-validate-modes: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-modes script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/modes.dtd --noout \$$FILE1 && exit 0;" >>$@
	@echo "exit 1;" >> $@
	@chmod a+x $@


apertium-validate-dictionary: Makefile.am validate-header.sh
	@echo "Creating apertium-validate-dictionary script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/dix.dtd --noout \$$FILE1 && exit 0;" >> $@
	@echo "exit 1;" >> $@
	@chmod a+x $@

apertium-gen-deformat: Makefile.am deformat-header.sh
	@echo "Creating apertium-gen-deformat script"
	@echo "#!$(BASH)" > $@
	@cat deformat-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/format.dtd --noout \$$FILE1 && \\" >> $@
	@if [ `basename $(XSLTPROC)` == xsltproc ]; \
	  then echo "$(XSLTPROC) --stringparam mode \$$MODE $(apertiumdir)/deformat.xsl \$$FILE1 >/tmp/\$$\$$.deformat.l && \\"; \
          else echo "$(XSLTPROC) $(apertiumdir)/deformat.xsl \$$FILE1 \"\\\$$mode=\$$MODE\" >/tmp/\$$\$$.deformat.l && \\"; \
          fi >> $@ 
	@echo "$(FLEX) \$$FLEXOPTS -o/tmp/\$$\$$.lex.cc /tmp/\$$\$$.deformat.l && \\" >> $@
	@echo "$(CXX) $(CXXFLAGS) -w $(APERTIUM_CFLAGS) -I $(apertiuminclude) -o \$$FILE2 /tmp/\$$\$$.lex.cc && \\" >> $@
	@echo "rm /tmp/\$$\$$.deformat.l /tmp/\$$\$$.lex.cc" >> $@
	@chmod a+x $@

apertium-gen-reformat: Makefile.am gen-header.sh
	@echo "Creating apertium-gen-reformat script"
	@echo "#!$(BASH)" > $@
	@cat gen-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/format.dtd --noout \$$FILE1 && \\" >> $@
	@echo "$(XSLTPROC) $(apertiumdir)/reformat.xsl \$$FILE1 >/tmp/\$$\$$.reformat.l && \\" >> $@
	@echo "$(FLEX) \$$FLEXOPTS -o/tmp/\$$\$$.lex.cc /tmp/\$$\$$.reformat.l && \\" >> $@
	@echo "$(CXX) $(CXXFLAGS) -w $(APERTIUM_CFLAGS) -I $(apertiuminclude) -o \$$FILE2 /tmp/\$$\$$.lex.cc &&\\" >> $@
	@echo "rm /tmp/\$$\$$.reformat.l /tmp/\$$\$$.lex.cc" >> $@
	@chmod a+x $@

apertium-gen-modes: Makefile.am modes-header.sh
	@echo "Creating apertium-gen-modes script"
	@echo "#!$(BASH)" > $@
	@cat modes-header.sh >> $@
	@echo "$(XMLLINT) --dtdvalid $(apertiumdir)/modes.dtd --noout \$$FILE1 && \\" >> $@
	@if [ `basename $(XSLTPROC)` == xsltproc ]; \
	  then echo "$(XSLTPROC) --stringparam prefix $(prefix)/bin --stringparam dataprefix \$$FULLDIRNAME  $(apertiumdir)/modes2bash.xsl \$$FILE1 | $(prefix)/bin/apertium-createmodes.awk PARAM=\$$FULLDIRNAME"; \
          else echo "$(XSLTPROC) $(apertiumdir)/modes2bash.xsl \$$FILE1 \\\$$prefix=$(prefix)/bin \\\$$dataprefix=\$$FULLDIRNAME| $(prefix)/bin/apertium-createmodes.awk PARAM=\$$FULLDIRNAME"; \
          fi >> $@ 
	@chmod a+x $@

apertium-translator: Makefile.am trans-header.sh
	@echo "Creating apertium-translator script"
	@echo "#!$(BASH)" > $@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@cat trans-header.sh >>$@
	@chmod a+x $@

apertium: Makefile.am apertium-header.sh
	@echo "Creating apertium  script"
	@echo "#!$(BASH)" > $@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@cat apertium-header.sh >>$@
	@chmod a+x $@

apertium-translator-lextor: Makefile.am trans-lextor-header.sh
	@echo "Creating apertium-translator-lextor script"
	@echo "#!$(BASH)" > $@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@cat trans-lextor-header.sh >>$@
	@chmod a+x $@

apertium-gen-oldbil: Makefile.am transformdicbil-header.sh
	@echo "Creating apertium-gen-oldbil script"
	@echo "#!$(BASH)" >$@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@echo "XSLTPROC_OPTIONS=\"\"">>$@
	@echo "STYLESHEET=\"$(apertiumdir)/new2old.xsl\"">>$@
	@cat transformdicbil-header.sh >>$@
	@chmod a+x $@

apertium-gen-lextorbil: Makefile.am transformdic-header.sh
	@echo "Creating apertium-gen-lextorbil script"
	@echo "#!$(BASH)" >$@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@echo "XSLTPROC_OPTIONS_LR=\"\"">>$@
	@echo "XSLTPROC_OPTIONS_RL=\"--stringparam r2l yes\"">>$@
	@echo "STYLESHEET=\"$(apertiumdir)/lexchoicebil.xsl\"">>$@
	@cat transformdic-header.sh >>$@
	@chmod a+x $@

apertium-gen-lextormono: Makefile.am transformdic-header.sh
	@echo "Creating apertium-gen-lextormono script"
	@echo "#!$(BASH)" >$@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@echo "XSLTPROC_OPTIONS_LR=\"\"">>$@
	@echo "XSLTPROC_OPTIONS_RL=\"--stringparam r2l yes\"">>$@
	@echo "STYLESHEET=\"$(apertiumdir)/lexchoice.xsl\"">>$@
	@cat transformdic-header.sh >>$@
	@chmod a+x $@

apertium-gen-wlist-lextor: Makefile.am gen-wlist-lextor-header.sh
	@echo "Creating apertium-gen-wlist-lextor script"
	@echo "#!$(BASH)" >$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@cat gen-wlist-lextor-header.sh >>$@
	@chmod a+x $@

apertium-preprocess-corpus-lextor: Makefile.am preprocess-corpus-lextor.sh
	@echo "Creating apertium-preprocess-corpus-lextor script"
	@echo "#!$(BASH)" >$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@echo "APERTIUM_PATH=\"$(prefix)/bin\"" >>$@
	@cat preprocess-corpus-lextor.sh >>$@
	@chmod a+x $@

apertium-gen-stopwords-lextor: Makefile.am gen-stopwords-lextor.sh
	@echo "Creating apertium-gen-stopwords-lextor script"
	@echo "#!$(BASH)" >$@
	@cat gen-stopwords-lextor.sh >>$@
	@chmod a+x $@

man_MANS=apertium.1 apertium-deshtml.1 apertium-desrtf.1 apertium-destxt.1 \
         apertium-filter-ambiguity.1 apertium-gen-deformat.1 apertium-gen-lextorbil.1 \
         apertium-gen-lextormono.1 apertium-gen-reformat.1 apertium-gen-stopwords-lextor.1 \
         apertium-gen-wlist-lextor.1 apertium-gen-wlist-lextor-translation.1 \
         apertium-lextor.1 apertium-preprocess-corpus-lextor.1 \
         apertium-preprocess-transfer.1 apertium-pretransfer.1 apertium-rehtml.1 \
         apertium-rertf.1 apertium-retxt.1 apertium-tagger.1 apertium-transfer.1 \
         apertium-translator.1 apertium-validate-dictionary.1 apertium-validate-tagger.1 \
         apertium-validate-transfer.1 ###apertium-lextor-eval.1 

EXTRA_DIST = gen-header.sh deformat-header.sh \
             reformat.xsl deformat.xsl new2old.xsl lexchoice.xsl lexchoicebil.xsl \
             tagger.dtd transfer.dtd format.dtd dix.dtd txt-format.xml \
             html-format.xml rtf-format.xml trans-header.sh apertium-header.sh $(man_MANS) \
             validate-header.sh transformdic-header.sh transformdicbil-header.sh \
             gen-wlist-lextor-header.sh gen-stopwords-lextor.sh preprocess-corpus-lextor.sh \
             trans-lextor-header.sh interchunk.dtd postchunk.dtd modes.dtd \
             modes-header.sh apertium-createmodes.awk modes2bash.xsl
