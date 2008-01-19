VERSION=1.0

FILES=documentacio.tex documentacio.bib fig4.eps fig2.eps fig3.eps fig4.eps diagram.eps fdatos.eps
all: documentacio.pdf

documentacio.pdf: $(FILES)
	latex documentacio
	bibtex documentacio
	latex documentacio
	latex documentacio
	latex documentacio
	dvipdfm documentacio

dist: $(FILES) documentacio.pdf Makefile
	rm -Rf apertium-documentation-es-$(VERSION)
	mkdir apertium-documentation-es-$(VERSION)
	cp $(FILES) documentacio.pdf Makefile apertium-documentation-es-$(VERSION)
	tar cvf apertium-documentation-es-$(VERSION).tar apertium-documentation-es-$(VERSION)
	rm -Rf apertium-documentation-es-$(VERSION)
	gzip -9 -f apertium-documentation-es-$(VERSION).tar
	
