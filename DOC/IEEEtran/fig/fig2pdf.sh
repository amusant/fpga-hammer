#!/bin/sh

#-------------------------------------------------------------------------------
# Filename:    fig2pdf.sh
#               - It is usefull to have the following alias for xfig:
#                 xfig -specialtext -latexfonts -startlatexFont default
#-------------------------------------------------------------------------------

# Usage:
if [ -z $1 ] ; then
  echo "Usage: $0 <file>"
  echo "$0 produces <file>.pdf from <file>.fig, where <file>.fig contains arbitrary drawings and LaTeX mathematical formulas";
  exit ;
fi ;

# Creation of a working copy:
cp $1.fig temp.fig
# Creation of the postscript layer:
fig2dev -L pstex temp.fig > temp.pstex
# Creation of the LaTeX layer (adjusted over the PS):
fig2dev -L pstex_t -p temp.pstex temp.fig > temp.pstex_t
# Generating a blank (apart from the graphic) DVI document:
latex driver.tex #>/dev/null
# Copying the "\label" defined in ../ in the driver auxiliary file:
#grep "newlabel" ../ci_get_2004.aux >> driver.aux
#grep "citation" ../ci_get_2004.aux >> driver.aux
#grep "bibcite" ../ci_get_2004.aux >> driver.aux
# Recompiling with the adequate cross-references:
latex driver.tex #>/dev/null
# Converting the DVI to EPS
dvips -E driver.dvi -o $1.eps 2>&1 > /dev/null
# Converting the DVI to PDF
epstopdf $1.eps --outfile $1.pdf
# Removing temporay files
rm -f temp.{pstex,pstex_t} driver.dvi driver.aux driver.log
