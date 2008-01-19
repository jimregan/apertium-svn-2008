# author Felipe Sánchez Martínez

DATADIR=~/apertium-es-pt
GIZADIR=~/GIZA++-v2

L1=es
L2=pt

#Alignment symmetrizartion method to use: union|intersection|refined
SIMMETRIZATION_METHOD=refined

#Minimum and maximum length of the alignment templates
MIN=1
MAX=7

#Criterion to use whe interpreting the count of an AT count|log|prod
CRITERION=log

#Minimum number of occurrences for an AT to be taken into account
TH_COUNT=10

################################################################

TRAINGIZA_PREFIX=train-giza++

TRAINGIZA_L1_L2=$(TRAINGIZA_PREFIX)-$(L1)-$(L2)
TRAINGIZA_L2_L1=$(TRAINGIZA_PREFIX)-$(L2)-$(L1)

PROB_L1=$(DATADIR)/$(L1)-$(L2).prob
PROB_L2=$(DATADIR)/$(L2)-$(L1).prob

MORF_L1=$(DATADIR)/$(L1)-$(L2).automorf.bin
MORF_L2=$(DATADIR)/$(L2)-$(L1).automorf.bin

BIL_L1_L2=$(DATADIR)/$(L1)-$(L2).autobil.bin
BIL_L2_L1=$(DATADIR)/$(L2)-$(L1).autobil.bin

GIZA_ALIGNMENTS_L1_L2_LR=$(TRAINGIZA_L1_L2)/LR/GIZA++.A3.final
GIZA_ALIGNMENTS_L1_L2_RL=$(TRAINGIZA_L1_L2)/RL/GIZA++.A3.final

GIZA_ALIGNMENTS_L2_L1_LR=$(TRAINGIZA_L2_L1)/LR/GIZA++.A3.final
GIZA_ALIGNMENTS_L2_L1_RL=$(TRAINGIZA_L2_L1)/RL/GIZA++.A3.final

ATXFILE_L1_L2=apertium-es-br.transfer-at.$(L1)-$(L2).atx
ATXFILE_L2_L1=apertium-es-br.transfer-at.$(L2)-$(L1).atx

CLEANFILES=alignments.$(L1)-$(L2).LR.gz alignments.$(L1)-$(L2).RL.gz \
           alignments.$(L2)-$(L1).LR.gz alignments.$(L2)-$(L1).RL.gz \
           alignments.$(L1)-$(L2).gz alignments.$(L2)-$(L1).gz \
           alignment-templates.uniq.$(L1)-$(L2).gz alignment-templates.uniq.$(L2)-$(L1).gz \
           bilphrases.$(L1)-$(L2).gz bilphrases.$(L2)-$(L1).gz \
           alignment-templates.$(L1)-$(L2).gz alignment-templates.$(L2)-$(L1).gz log.* \
           $(TRAINGIZA_L1_L2)/*.vcb $(TRAINGIZA_L1_L2)/*.snt $(TRAINGIZA_L1_L2)/*.txt \
           $(TRAINGIZA_L1_L2)/*.classes $(TRAINGIZA_L1_L2)/*.cats $(TRAINGIZA_L1_L2)/*log* \
           $(TRAINGIZA_L2_L1)/*.vcb $(TRAINGIZA_L2_L1)/*.snt $(TRAINGIZA_L2_L1)/*.txt \
           $(TRAINGIZA_L2_L1)/*.classes $(TRAINGIZA_L2_L1)/*.cats $(TRAINGIZA_L2_L1)/*log* \
           $(TRAINGIZA_L1_L2)/LR/* $(TRAINGIZA_L1_L2)/RL/* \
           $(TRAINGIZA_L2_L1)/LR/* $(TRAINGIZA_L2_L1)/RL/*\

all: trules.$(L1)-$(L2).xml trules.$(L2)-$(L1).xml

$(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos.txt: $(L1).txt.gz $(MORF_L1) $(PROB_L1) preprocess_$(L1)_$(L1)-$(L2).sh
	@echo "Analysing "$(L1)".txt.gz to train '"$(L1)"-"$(L2)"' alignments ... "
	zcat $(L1).txt.gz | awk '{print $$0 " ."}' | apertium-destxt | lt-proc -a $(MORF_L1) | \
	apertium-tagger -g $(PROB_L1) | apertium-pretransfer | apertium-retxt | \
	sed -re "s/ \^[.]<sent>\\$$\$$//g"  | apertium-preprocess-corpus-transfer-at |\
	./preprocess_$(L1)_$(L1)-$(L2).sh > $@
	@echo ""

$(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos.txt: $(L2).txt.gz $(MORF_L2) $(PROB_L2) preprocess_$(L2)_$(L1)-$(L2).sh 
	@echo "Analysing "$(L2)".txt.gz to train '"$(L1)"-"$(L2)"' alignments ... "
	zcat $(L2).txt.gz | awk '{print $$0 " ."}' | apertium-destxt | lt-proc -a $(MORF_L2) |\
	apertium-tagger -g $(PROB_L2) | apertium-pretransfer | apertium-retxt |\
	sed -re "s/ \^[.]<sent>\\$$\$$//g"  | apertium-preprocess-corpus-transfer-at |\
	./preprocess_$(L2)_$(L1)-$(L2).sh > $@
	@echo ""

$(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos.txt: $(L2).txt.gz $(MORF_L2) $(PROB_L2) preprocess_$(L2)_$(L2)-$(L1).sh
	@echo "Analysing "$(L2)".txt.gz to train '"$(L2)"-"$(L1)"' alignments ... "
	zcat $(L2).txt.gz | awk '{print $$0 " ."}' | apertium-destxt | lt-proc -a $(MORF_L2) |\
        apertium-tagger -g $(PROB_L2) | apertium-pretransfer | apertium-retxt |\
        sed -re "s/ \^[.]<sent>\\$$\$$//g"  | apertium-preprocess-corpus-transfer-at |\
	./preprocess_$(L2)_$(L2)-$(L1).sh > $@
	@echo ""

$(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos.txt: $(L1).txt.gz $(MORF_L1) $(PROB_L1) preprocess_$(L1)_$(L2)-$(L1).sh
	@echo "Analysing "$(L1)".txt.gz to train '"$(L2)"-"$(L1)"' alignments ... "
	zcat $(L1).txt.gz | awk '{print $$0 " ."}' | apertium-destxt | lt-proc -a $(MORF_L1) |\
        apertium-tagger -g $(PROB_L1) | apertium-pretransfer | apertium-retxt |\
        sed -re "s/ \^[.]<sent>\\$$\$$//g"  | apertium-preprocess-corpus-transfer-at |\
	./preprocess_$(L1)_$(L2)-$(L1).sh > $@
	@echo ""

$(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos_$(L2).$(L1)-$(L2).pos.snt: $(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos.txt  $(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos.txt
	@echo "Preparing corpora to train ("$(L1)"-"$(L2)") with GIZA++ ..."
	$(GIZADIR)/plain2snt.out $(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos.txt  $(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos.txt 2> $(TRAINGIZA_L1_L2)/log.plain2snt
	@echo ""

$(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos_$(L1).$(L1)-$(L2).pos.snt: $(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos.txt  $(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos.txt
	@echo "Preparing corpora to train ("$(L1)"-"$(L2)") with GIZA++ ..."
	$(GIZADIR)/plain2snt.out $(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos.txt  $(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos.txt 2> $(TRAINGIZA_L1_L2)/log.plain2snt
	@echo ""

$(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos_$(L1).$(L2)-$(L1).pos.snt: $(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos.txt  $(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos.txt
	@echo "Preparing corpora to train ("$(L2)"-"$(L1)") with GIZA++ ..."
	$(GIZADIR)/plain2snt.out $(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos.txt  $(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos.txt 2> $(TRAINGIZA_L2_L1)/log.plain2snt
	@echo ""

$(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos_$(L2).$(L2)-$(L1).pos.snt: $(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos.txt  $(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos.txt
	@echo "Preparing corpora to train ("$(L2)"-"$(L1)") with GIZA++ ..."
	$(GIZADIR)/plain2snt.out $(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos.txt  $(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos.txt 2> $(TRAINGIZA_L2_L1)/log.plain2snt
	@echo ""

$(GIZA_ALIGNMENTS_L1_L2_LR): $(TRAINGIZA_L1_L2)/$(L1).$(L1)-$(L2).pos_$(L2).$(L1)-$(L2).pos.snt
	@echo "Performing GIZA++ training ("$(L1)"-"$(L2)") from left to right ..."
	cd $(TRAINGIZA_L1_L2) && $(GIZADIR)/trainGIZA++.sh $(L1).$(L1)-$(L2).pos.vcb  $(L2).$(L1)-$(L2).pos.vcb $(L1).$(L1)-$(L2).pos_$(L2).$(L1)-$(L2).pos.snt &&\
	mv GIZA++.* *.vcb.classes.cats *.vcb.classes mkcls1.log mkcls2.log LR
	@echo ""

$(GIZA_ALIGNMENTS_L1_L2_RL): $(TRAINGIZA_L1_L2)/$(L2).$(L1)-$(L2).pos_$(L1).$(L1)-$(L2).pos.snt
	@echo "Performing GIZA++ training ("$(L1)"-"$(L2)") from right to left ..."
	cd $(TRAINGIZA_L1_L2) && $(GIZADIR)/trainGIZA++.sh $(L2).$(L1)-$(L2).pos.vcb  $(L1).$(L1)-$(L2).pos.vcb $(L2).$(L1)-$(L2).pos_$(L1).$(L1)-$(L2).pos.snt &&\
	mv GIZA++.* *.vcb.classes.cats *.vcb.classes mkcls1.log mkcls2.log RL
	@echo ""

$(GIZA_ALIGNMENTS_L2_L1_LR): $(TRAINGIZA_L2_L1)/$(L2).$(L2)-$(L1).pos_$(L1).$(L2)-$(L1).pos.snt
	@echo "Performing GIZA++ training ("$(L2)"-"$(L1)") from left to right ..."
	cd $(TRAINGIZA_L2_L1) && $(GIZADIR)/trainGIZA++.sh $(L2).$(L2)-$(L1).pos.vcb  $(L1).$(L2)-$(L1).pos.vcb $(L2).$(L2)-$(L1).pos_$(L1).$(L2)-$(L1).pos.snt &&\
	mv GIZA++.* *.vcb.classes.cats *.vcb.classes mkcls1.log mkcls2.log LR
	@echo ""

$(GIZA_ALIGNMENTS_L2_L1_RL): $(TRAINGIZA_L2_L1)/$(L1).$(L2)-$(L1).pos_$(L2).$(L2)-$(L1).pos.snt
	@echo "Performing GIZA++ training ("$(L2)"-"$(L1)") from right to left ..."
	cd $(TRAINGIZA_L2_L1) && $(GIZADIR)/trainGIZA++.sh $(L1).$(L2)-$(L1).pos.vcb  $(L2).$(L2)-$(L1).pos.vcb $(L1).$(L2)-$(L1).pos_$(L2).$(L2)-$(L1).pos.snt &&\
	mv GIZA++.* *.vcb.classes.cats *.vcb.classes mkcls1.log mkcls2.log RL
	@echo ""

alignments.$(L1)-$(L2).LR.gz: $(GIZA_ALIGNMENTS_L1_L2_LR) 
	@echo "Converting GIZA++ LR alignments into apertium format ("$(L1)"-"$(L2)") ..."
	cat $(GIZA_ALIGNMENTS_L1_L2_LR) | giza++A3-to-apertium | gzip > $@
	@echo ""

alignments.$(L1)-$(L2).RL.gz: $(GIZA_ALIGNMENTS_L1_L2_RL) 
	@echo "Converting GIZA++ RL alignments into apertium format ("$(L1)"-"$(L2)") ..."
	cat $(GIZA_ALIGNMENTS_L1_L2_RL) | giza++A3-to-apertium | apertium-switch-alignments | gzip > $@
	@echo ""

alignments.$(L2)-$(L1).LR.gz: $(GIZA_ALIGNMENTS_L2_L1_LR) 
	@echo "Converting GIZA++ LR alignments into apertium format ("$(L2)"-"$(L1)") ..."
	cat $(GIZA_ALIGNMENTS_L2_L1_LR) | giza++A3-to-apertium | gzip > $@
	@echo ""

alignments.$(L2)-$(L1).RL.gz: $(GIZA_ALIGNMENTS_L2_L1_RL) 
	@echo "Converting GIZA++ RL alignments into apertium format ("$(L2)"-"$(L1)") ..."
	cat $(GIZA_ALIGNMENTS_L2_L1_RL) | giza++A3-to-apertium | apertium-switch-alignments | gzip > $@
	@echo ""

alignments.$(L1)-$(L2).gz: alignments.$(L1)-$(L2).LR.gz alignments.$(L1)-$(L2).RL.gz
	@echo "Symmetrizing alignment ("$(L1)"-"$(L1)") by "$(SIMMETRIZATION_METHOD)
	apertium-symmetrize-alignments -i alignments.$(L1)-$(L2).LR.gz -j alignments.$(L1)-$(L2).RL.gz -o $@ -p $(SIMMETRIZATION_METHOD) -z 2> log.symm.$(L1)-$(L2)

alignments.$(L2)-$(L1).gz: alignments.$(L2)-$(L1).LR.gz alignments.$(L2)-$(L1).RL.gz
	@echo "Symmetrizing alignment ("$(L2)"-"$(L1)") by "$(SIMMETRIZATION_METHOD)
	apertium-symmetrize-alignments -i alignments.$(L2)-$(L1).LR.gz -j alignments.$(L2)-$(L1).RL.gz -o $@ -p $(SIMMETRIZATION_METHOD) -z 2> log.symm.$(L2)-$(L1)

bilphrases.$(L1)-$(L2).gz: alignments.$(L1)-$(L2).gz
	@echo "Extracting bilingual phrases from alignments."$(L1)"-"$(L2)".gz ..."
	apertium-xtract-bilingual-phrases -i alignments.$(L1)-$(L2).gz -o $@ -n $(MIN) -m $(MAX) -z 2> log.bilph.$(L1)-$(L2)
	gzip log.bilph.$(L1)-$(L2)
	@echo ""

bilphrases.$(L2)-$(L1).gz: alignments.$(L2)-$(L1).gz
	@echo "Extracting bilingual phrases from alignments."$(L2)"-"$(L1)".gz ..."
	apertium-xtract-bilingual-phrases -i alignments.$(L2)-$(L1).gz -o $@ -n $(MIN) -m $(MAX) -z 2> log.bilph.$(L2)-$(L1)
	gzip log.bilph.$(L2)-$(L1)
	@echo ""

alignment-templates.$(L1)-$(L2).gz: $(ATXFILE_L1_L2) bilphrases.$(L1)-$(L2).gz $(BIL_L1_L2)
	@echo "Extracing alignment templates from bilphrases."$(L1)"-"$(L2)".gz ..."
	apertium-validate-transfer-at $(ATXFILE_L1_L2)
	apertium-xtract-alignment-templates -x $(ATXFILE_L1_L2) -b $(BIL_L1_L2) -i bilphrases.$(L1)-$(L2).gz -o $@ -z -e 2> log.aligtem.$(L1)-$(L2)
	gzip log.aligtem.$(L1)-$(L2)
	@echo ""

alignment-templates.$(L2)-$(L1).gz: $(ATXFILE_L2_L1) bilphrases.$(L2)-$(L1).gz $(BIL_L2_L1)
	@echo "Extracing alignment templates from bilphrases."$(L2)"-"$(L1)".gz ..."
	apertium-validate-transfer-at $(ATXFILE_L2_L1)
	apertium-xtract-alignment-templates -x $(ATXFILE_L2_L1) -b $(BIL_L2_L1) -i bilphrases.$(L2)-$(L1).gz -o $@ -z -e 2> log.aligtem.$(L2)-$(L1)
	gzip log.aligtem.$(L2)-$(L1)
	@echo ""

alignment-templates.uniq.$(L1)-$(L2).gz: alignment-templates.$(L1)-$(L2).gz
	@echo "Unifying alignment templates from file alignment-templates."$(L1)"-"$(L2)".gz ..."
	zcat alignment-templates.$(L1)-$(L2).gz | ./filter_alignment_templates.awk |\
	cut -d' ' -f1 --complement | sort | uniq -c | sed -re "s/^[ ]+//g" | sort -t '|' -k 2,2 | gzip > $@
	@echo ""

alignment-templates.uniq.$(L2)-$(L1).gz: alignment-templates.$(L2)-$(L1).gz
	@echo "Unifying alignment templates from file alignment-templates."$(L2)"-"$(L1)".gz ..."
	zcat alignment-templates.$(L2)-$(L1).gz | ./filter_alignment_templates.awk |\
	cut -d' ' -f1 --complement | sort | uniq -c | sed -re "s/^[ ]+//g" | sort -t '|' -k 2,2 | gzip > $@
	@echo ""

trules.$(L1)-$(L2).xml: alignment-templates.uniq.$(L1)-$(L2).gz
	@echo "Converting alignment templates from file alignment-templates.uniq."$(L1)"-"$(L2)".gz into apertium transfer rules ... "
	apertium-gen-transfer-from-aligment-templates -i alignment-templates.uniq.$(L1)-$(L2).gz -m $(TH_COUNT) -c $(CRITERION) -z > $@ 2> log.trules.$(L1)-$(L2)
	@echo ""

trules.$(L2)-$(L1).xml: alignment-templates.uniq.$(L2)-$(L1).gz
	@echo "Converting alignment templates from file alignment-templates.uniq."$(L2)"-"$(L1)".gz into apertium transfer rules ... "
	apertium-gen-transfer-from-aligment-templates -i alignment-templates.uniq.$(L2)-$(L1).gz -m $(TH_COUNT) -c $(CRITERION) -z > $@ 2> log.trules.$(L2)-$(L1)
	@echo ""

clean: 
	rm -f $(CLEANFILES) *~
