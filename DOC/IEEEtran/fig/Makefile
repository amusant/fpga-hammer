FIGFILES     = $(wildcard *.fig)
PDFFILES   = $(patsubst %.fig,%.pdf,$(FIGFILES))
PSTEXTFILES  = $(patsubst %.fig,%.pstex_t,$(FIGFILES))

.PRECIOUS: $(PDFFILES)

all : $(PDFFILES)

%.pdf: %.fig
	@#fig2dev -L pdf $< > $@
	./fig2pdf.sh $*

clean :
	rm -f *.fig.bak *.eps  *~ $(PDFFILES) 

ultraclean: clean
