VERSION=1.0

FILES=apertium2doc.tex
all: apertium2doc.pdf

apertium2doc.pdf: $(FILES)
	latex apertium2doc
	latex apertium2doc
	latex apertium2doc
	dvipdfm apertium2doc

dist: $(FILES) apertium2doc.pdf Makefile
	rm -Rf apertium2-documentation-ca-$(VERSION)
	mkdir apertium2-documentation-ca-$(VERSION)
	cp $(FILES) apertium2doc.pdf Makefile apertium2-documentation-ca-$(VERSION)
	tar cvf apertium2-documentation-ca-$(VERSION).tar apertium2-documentation-ca-$(VERSION)
	rm -Rf apertium2-documentation-ca-$(VERSION)
	gzip -9 -f apertium2-documentation-ca-$(VERSION).tar
	
clean: 
	rm -f *.aux *.bbl *.blg *.dvi *.log *.toc *~