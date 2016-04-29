TOP		?= trojan
VPATH		= fig
PDFLATEX	= pdflatex
PS2PDF		= ps2pdf14 
BIBTEX		= bibtex
TEXFILES  = $(wildcard *.tex)
BIBFILES	= $(wildcard *.bib)

FIGS       = $(wildcard fig/*.fig)
FIGS_PDF   = $(patsubst %.fig,%.pdftex,$(FIGS))
FIGS_PDF_T = $(patsubst %.fig,%.pdftex_t,$(FIGS))

NEEDED     = $(FIGS_PDF) $(FIGS_PDF_T)

all: $(TOP).pdf

dep : $(NEEDED)

ieee:$(TOP).pdf
	$(PS2PDF) -dPDFSETTINGS=/prepress $(TOP).pdf $(TOP)_ieee.pdf

$(TOP).pdf: $(TEXFILES) $(NEEDED) $(BIBFILES)
	$(PDFLATEX) $(TOP).tex
	$(BIBTEX)   $(TOP).aux
	$(PDFLATEX) $(TOP).tex
	$(PDFLATEX) $(TOP).tex
	$(PDFLATEX) $(TOP).tex



%.pdftex:%.fig
	fig2dev -L pdftex $< $@
%.pdftex_t:%.pdftex
	fig2dev -L pdftex_t -p $< $(patsubst %.pdftex_t,%.fig,$@) $@

check:
	aspell -c --lang="EN"  ${TOP}.tex
clean:
	rm -f *.aux *.log *.out *.lof *.toc *.bbl *.blg 
	rm -f $(TOP).pdf
	rm -f *~
	rm -f $(FIGS_PDF) $(FIGS_PDF_T)

