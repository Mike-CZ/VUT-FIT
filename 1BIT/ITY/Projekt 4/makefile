#
# Projekt: Typografie a publikování  4. projekt
# Autor:   Lukáš Pelánek
# Datum:   22. 4. 2014
#

name=proj4


proj: proj4.tex
	latex $(name).tex
	bibtex $(name).aux
	latex $(name).tex
	latex $(name).tex
	dvips -t a4 $(name).dvi
	ps2pdf $(name).ps
