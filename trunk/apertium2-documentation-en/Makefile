VERSION=1.0

FILES=documentation.tex documentation.bib fig4.eps fig2.eps fig3.eps fig4.eps diagram.eps fdatos.eps
all: documentation.pdf

documentation.pdf: $(FILES)
	latex documentation
	bibtex documentation
	latex documentation
	latex documentation
	latex documentation
	dvipdfm documentation

dist: $(FILES) documentation.pdf Makefile
	rm -Rf apertium-documentation-es-$(VERSION)
	mkdir apertium-documentation-es-$(VERSION)
	cp $(FILES) documentation.pdf Makefile apertium-documentation-es-$(VERSION)
	tar cvf apertium-documentation-es-$(VERSION).tar apertium-documentation-es-$(VERSION)
	rm -Rf apertium-documentation-es-$(VERSION)
	gzip -9 -f apertium-documentation-es-$(VERSION).tar
	

clean:
	rm *.pdf *.log *.blg *.dvi *.aux *.toc *.bbl
