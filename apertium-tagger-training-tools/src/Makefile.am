# author: Felipe Sánchez Martínez

bin_PROGRAMS = apertium-tagger-tl-trainer apertium-tagger-eval apertium-tagger-baum-welch \
               apertium-tagger-check-parameters apertium-tagger-readwords  \
               apertium-SGT apertium-tagger-apply-rules apertium-tagger-prob2txt \
               apertium-tagger-supervised
# apertium-bestTLtagger apertium-hmm-language-model

bin_SCRIPTS = apertium-xtract-regex-trules apertium-tagger-gen-crp-file apertium-tagger-gen-dic-file \
              apertium-trigrams-langmodel apertium-trigrams-langmodel-perline

apertium_tagger_training_toolsdir=$(prefix)/apertium-tagger-training-tools-$(GENERIC_API_VERSION)

EXTRA_DIST = xtract-trules.xsl xtract-regex-trules.sh gen_crp_file.sh gen_dic_file.sh \
             apertium-trigrams-langmodel.pl apertium-trigrams-langmodel-perline.pl 

apertium_tagger_tl_trainer_SOURCES =  HMM_TL_driven_trainer.H  Segment.H \
            StringCollection.H  TransferRules.H  Translations.H  Utils.H PathsPruner.H \
            SmoothUtils.H apertium-tagger-tl-trainer.C  \
            HMM_TL_driven_trainer.C Segment.C  StringCollection.C  TransferRules.C \
            Translations.C  Utils.C PathsPruner.C SmoothUtils.C
apertium_tagger_tl_trainer_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_eval_SOURCES = Utils.H Utils.C TaggerEval.H TaggerEval.C apertium-tagger-eval.C
apertium_tagger_eval_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_baum_welch_SOURCES = apertium-tagger-baum-welch.C SmoothUtils.H SmoothUtils.C
apertium_tagger_baum_welch_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_supervised_SOURCES = apertium-tagger-supervised.C SmoothUtils.H SmoothUtils.C
apertium_tagger_supervised_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_check_parameters_SOURCES = apertium-tagger-check-parameters.C
apertium_tagger_check_parameters_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_readwords_SOURCES = apertium-tagger-readwords.C
apertium_tagger_readwords_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_apply_rules_SOURCES = apertium-tagger-apply-rules.C
apertium_tagger_apply_rules_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_tagger_prob2txt_SOURCES = apertium-tagger-prob2txt.C
apertium_tagger_prob2txt_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

#apertium_hmm_language_model_SOURCES = apertium-hmm-language-model.C Utils.H Utils.C
#apertium_hmm_language_model_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

#apertium_bestTLtagger_SOURCES = apertium-bestTLtagger.C Segment.H TransferRules.H Utils.H Segment.C TransferRules.C Utils.C                                                
#apertium_bestTLtagger_LDADD = $(LIBXML2_LIBS) $(LTTOOLBOX_LIBS) $(APERTIUM_LIBS)

apertium_SGT_SOURCES = apertium-SGT.C 

apertium_tagger_training_tools_DATA = xtract-trules.xsl

INCLUDES = $(LIBXML2_CFLAGS) $(LTTOOLBOX_CFLAGS) $(APERTIUM_CFLAGS)

CLEANFILES = *~ $(bin_SCRIPTS)

apertium-xtract-regex-trules: xtract-regex-trules.sh Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(BASH)" > $@
	@echo "XSLFILE="$(apertium_tagger_training_toolsdir)/xtract-trules.xsl >> $@
	@echo "XSLTPROC="$(XSLTPROC) >> $@
	@cat   xtract-regex-trules.sh >> $@
	@chmod a+x $@

apertium-tagger-gen-crp-file:  gen_crp_file.sh Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(BASH)" > $@
	@echo "APERTIUM_DESTXT="$(APERTIUM_DESTXT) >> $@
	@echo "LTPROC="$(LTPROC) >> $@
	@cat   gen_crp_file.sh >> $@
	@chmod a+x $@

apertium-tagger-gen-dic-file: gen_dic_file.sh Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(BASH)" > $@
	@cat   gen_dic_file.sh >> $@
	@chmod a+x $@

apertium-trigrams-langmodel: apertium-trigrams-langmodel.pl Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(PERL) -w" > $@
	@cat   apertium-trigrams-langmodel.pl >> $@
	@chmod a+x $@

apertium-trigrams-langmodel-perline: apertium-trigrams-langmodel-perline.pl Makefile.am
	@echo "Creating $@ script"
	@echo "#!$(PERL) -w" > $@
	@cat   apertium-trigrams-langmodel-perline.pl >> $@
	@chmod a+x $@
