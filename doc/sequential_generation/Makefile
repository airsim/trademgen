#
DOCREF = sequential_event_generation

# Targets
all: ${DOCREF}.pdf

pdf: ${DOCREF}.pdf

#makeindex ${DOCREF}.idx

${DOCREF}.pdf: clean ${DOCREF}.tex
	pdflatex ${DOCREF}.tex
	bibtex ${DOCREF}
	pdflatex ${DOCREF}.tex
	latex_count=5 ; \
	while egrep -s 'Rerun (LaTeX|to get cross-references right)' ${DOCREF}.log && [ $$latex_count -gt 0 ] ;\
	    do \
	      echo "Rerunning latex...." ;\
	      pdflatex ${DOCREF}.tex ;\
	      latex_count=`expr $$latex_count - 1` ;\
	    done


clean:
	rm -f ${DOCREF}.ps ${DOCREF}.dvi ${DOCREF}.aux ${DOCREF}.bbl ${DOCREF}.blg ${DOCREF}.toc ${DOCREF}.idx ${DOCREF}.ind ${DOCREF}.ilg ${DOCREF}.log ${DOCREF}.out ${DOCREF}.pdf
