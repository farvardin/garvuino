# Makefile for TeXtallion
# Tiny almost-Kiss Word Processor
# http://anamnese.online.fr/site2/textallion/docs/presentation.html
# License: http://en.wikipedia.org/wiki/BSD_licenses
#
# get newest txt2tags   at http://txt2tags.org (might not be fully compatible with textallion)
# get newest textallion at https://bitbucket.org/farvardin/textallion
#
# This is a Makefile for *GNU make*, the default for Linux.
# On *BSD systems, you'll need to use "gmake" instead of "make"

# Makefile initially generated on 2017-09-13


TEXTALLIONFOLDER = /usr/share/textallion//
# <!> keep /usr/share/textallion if it is written so!

ifeq ($(wildcard /usr/bin/python2),)
 PYTHONVER = python 
else 
 PYTHONVER = python2
endif

TXT2TAGS = $(PYTHONVER) $(TEXTALLIONFOLDER)/contrib/txt2tags/txt2tags
#TXT2TAGS = txt2tags

ifdef TEXTALLIONDOC
  DOCUMENT = $(TEXTALLIONDOC)
else
  DOCUMENT = garvuino_manual
endif

DOCUMENT_TITLE = Garvuino Manual
DOCUMENT_AUTHOR = Garvalf
DOCUMENT_TAGS = arduino, 8bit, 1bit, chiptune, ym2149, ay38910
DOCUMENT_INFO = textallion - https://bitbucket.org/farvardin/textallion

ifdef DOCLANG
  DOCUMENT_LANGUAGE = $(DOCLANG)
else
  DOCUMENT_LANGUAGE = en 
endif

DOCUMENT_COVER = $(DOCUMENT).jpg

PDFREADER = xdg-open
EPUBREADER = ebook-viewer 
HTMLREADER = firefox
DIFFTOOL = meld
RENPY = /opt/renpy/renpy.sh

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	EDITTOOL = open
else
#	EDITTOOL = gvim
	EDITTOOL = geany
endif




info:
	printf "make: +  \n all \n beamer \n booklet \n clean \n cleanall \n clean-everything \n configuration-update \n cover \n cyoa-epub \n cyoa-gbl \n cyoa-graph \n cyoa-html \n cyoa-hyena \n cyoa-inform7 \n cyoa-pdf \n cyoa-play \n cyoa-ramus \n cyoa-renpy \n cyoa-togbl \n cyoa-twee \n cyoa-txt \n edit \n epub \n html \n htmlhandhelds \n info \n makefile \n pdf \n pdfsmall \n read \n readepub \n readhtml \n readindex \n readpdf \n slidy \n split \n tidy \n txt \n vignettes \n website \n xetex \n \n" 
	
all:    pdf md clean



#distrib: html clean
#	-rm $(DOCUMENT).zip
#	-rm -fr $(DOCUMENT)
#	-mkdir $(DOCUMENT)
#	-cp * $(DOCUMENT)
#	zip $(DOCUMENT).zip -r $(DOCUMENT) -x $(DOCUMENT).zip


md: 	
	$(TXT2TAGS) -t md -o garvuino_manual.md garvuino_manual.t2t


html:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/xhtml.html -t xhtml --css-inside --css-sugar --toc --outfile $(DOCUMENT).html $(DOCUMENT).t2t

htmlhandhelds:
	$(TXT2TAGS) -t xhtml --no-style  --style=$(TEXTALLIONFOLDER)/includes/sample_handheld.css --toc --outfile $(DOCUMENT).html $(DOCUMENT).t2t

edit:
	$(EDITTOOL) $(DOCUMENT).t2t &
txt:
	$(TXT2TAGS) --no-headers -t txt $(DOCUMENT).t2t

pdf:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/latex.tex -t tex --toc --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
	sed -i -e "s/,frenchb,english//g" $(DOCUMENT).tex
	-pdflatex -interaction batchmode $(DOCUMENT).tex
	-makeindex $(DOCUMENT).idx
	-pdflatex -interaction batchmode $(DOCUMENT).tex
	# (the compilation of the latex document is duplicated so the TOC generated the first time will be included the second time)
	-pdflatex -interaction batchmode $(DOCUMENT).tex
	# and one again because of makeindex
	#mv $(DOCUMENT).pdf ../
	

pdfweb: 
	make html
	wkhtmltopdf --page-size A4 --margin-top 1.0cm --margin-bottom 1.0cm --margin-left 0.5cm --margin-right 0.5cm $(DOCUMENT).html $(DOCUMENT).pdf

pdfwk:
	make pdfweb

pdfnolatex:
	make pdfweb


xetex:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/xetex.tex -t tex --toc --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
ifeq ($(DOCUMENT_LANGUAGE),en)
	sed -i -e "s/,frenchb,english//g" $(DOCUMENT).tex
endif
	-xelatex -interaction batchmode $(DOCUMENT).tex
	-xelatex -interaction batchmode $(DOCUMENT).tex

latex:
	make pdf
	

lettre:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/lettre.tex -t tex --no-toc --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
	-pdflatex -interaction batchmode $(DOCUMENT).tex
	-make clean

lettre-1page:
	pdfjam $(DOCUMENT).pdf '1' --outfile $(DOCUMENT)b.pdf 
	mv $(DOCUMENT)b.pdf $(DOCUMENT).pdf 
	
readpdf:	
	$(PDFREADER) $(DOCUMENT).pdf &

readhtml:	
	$(HTMLREADER) $(DOCUMENT).html &

readindex:	
	$(HTMLREADER) index_$(DOCUMENT).html &

readepub:	
	$(EPUBREADER) $(DOCUMENT).epub &

read:
	make readpdf &

pdfsmall: pdf
	-pdfnup $(DOCUMENT).pdf --nup 2x1

booklet: pdf
	-pdf2ps $(DOCUMENT).pdf
	-psbook $(DOCUMENT).ps | psnup -2 > $(DOCUMENT)_booklet.ps
	-ps2pdf $(DOCUMENT)_booklet.ps


slidy: 
	$(TXT2TAGS) -C $(TEXTALLIONFOLDER)/templates/slidy.conf.t2t -T $(TEXTALLIONFOLDER)/templates/slidy -t xhtml --css-inside -o $(DOCUMENT)_slide.html $(DOCUMENT).t2t

beamer: 
	$(TXT2TAGS) --no-style --no-infile -C $(TEXTALLIONFOLDER)/templates/beamer.conf.t2t -C $(TEXTALLIONFOLDER)/core/textallion_beamer.t2t -T $(TEXTALLIONFOLDER)/templates/beamer -t tex -o $(DOCUMENT)_slide.tex $(DOCUMENT).t2t
	pdflatex $(DOCUMENT)_slide.tex
	
	
# uses html tidy

tidy:
	-tidy -asxhtml --tidy-mark 1 --wrap 0 --clean 1 --output-xhtml 1 --input-encoding utf8 --doctype strict --new-inline-tags video,audio,canvas  $(DOCUMENT).html > $(DOCUMENT)2.html
	cat $(DOCUMENT)2.html | sed -e "s/<meta name=\"generator\" content=\"http:\/\/txt2tags.org\" \/>//g" | sed -e "s/border=\"0\"//g" | sed -e "s/lang=\"fr\"//g" | sed -e "s/border=\"0\"//g" | sed -e "s/<\?xml version=\"1.0\" encoding=\"utf8\"\?>//g" | sed -e "s/name=\"toc[0-9]\"//g"> $(DOCUMENT).html

# uses calibre

	
epub:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/epub.html -t xhtml --no-style --no-toc --outfile $(DOCUMENT).html $(DOCUMENT).t2t
	cat $(DOCUMENT).html | sed -e "s/<audio\(.*\)audio>//g" > $(DOCUMENT)2.html
	mv $(DOCUMENT)2.html $(DOCUMENT).html
	-make tidy
	cat $(DOCUMENT).html | sed -e "s/&#10086;/*/g" | sed -e "s/&#10087;/*/g" | sed -e "s/&#10037;/*/g" | sed -e "s/&#9788;/*/g" | sed -e "s/&#9789;/*/g" | sed -e "s/&#9790;/*/g" | sed -e "s/&#9675;/*/g" | sed -e "s/html\#ftn/html/g" > $(DOCUMENT)2.html
	mv $(DOCUMENT)2.html $(DOCUMENT).html
	#
	ebook-convert $(DOCUMENT).html $(DOCUMENT).epub --max-levels 0 --pretty-print --level1-toc //h:h1 --level2-toc //h:h2 --level3-toc //h:h3 --max-toc-links 0 --toc-threshold 3  --chapter-mark pagebreak  --cover $(DOCUMENT_COVER) --extra-css $(TEXTALLIONFOLDER)/includes/epub.css --no-default-epub-cover  --preserve-cover-aspect-ratio --no-chapters-in-toc --disable-font-rescaling
	# --filter-css font-family
	#
	ebook-meta  $(DOCUMENT).epub --title "$(DOCUMENT_TITLE)" --authors "$(DOCUMENT_AUTHOR)" --tags "$(DOCUMENT_TAGS)" --language $(DOCUMENT_LANGUAGE) --book-producer 'textallion - https://bitbucket.org/farvardin/textallion' --comments "$(DOCUMENT_INFO)" --cover $(DOCUMENT_COVER) 

# split (not working well...)

split:
	htmldoc --no-links --charset utf8 --strict --book -t htmlsep -d ebook $(DOCUMENT)2.html
	#$(TXT2TAGS) -t html --split 2 --no-toc --outfile $(DOCUMENT).html $(DOCUMENT).t2t 

website:
	cat $(TEXTALLIONFOLDER)/templates/website.html | sed -e "s|%%DOCUMENT AUTHOR%%|$(DOCUMENT_AUTHOR)|g" | sed -e "s|%%DOCUMENT TITLE%%|$(DOCUMENT_TITLE)|g" | sed -e "s|%%DOCUMENT%%|$(DOCUMENT)|g"  | sed -e "s|%%DOCUMENT COVER%%|$(DOCUMENT_COVER)|g" > index_$(DOCUMENT).html


cover:
	convert $(DOCUMENT).svg $(DOCUMENT).jpg
	convert $(DOCUMENT).svg $(DOCUMENT).png
	
	
configuration-update:
	$(DIFFTOOL) makefile $(TEXTALLIONFOLDER)/samples/makefile
	$(DIFFTOOL) $(DOCUMENT).sty $(TEXTALLIONFOLDER)/includes/sample.sty
	$(DIFFTOOL) $(DOCUMENT).css $(TEXTALLIONFOLDER)/includes/sample.css

# CYOA part 

cyoa-play:
	$(BROWSER) $(DOCUMENT).html

cyoa-html:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/cyoa.html --config-file $(TEXTALLIONFOLDER)/core/txt2cyoa.t2t -t xhtml --css-inside --outfile $(DOCUMENT).html $(DOCUMENT).t2t
	
	
cyoa-ramus:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/ramus.html  --config-file $(TEXTALLIONFOLDER)/core/txt2cyoa.t2t  -t xhtml --no-css-inside --outfile $(DOCUMENT)_ramus.html $(DOCUMENT).t2t
	sed -i -e "s/href=\"#/rel=\"/g" $(DOCUMENT)_ramus.html
	sed -i -e "s/style=\"display:none\"//g" $(DOCUMENT)_ramus.html
	#sed -i -e "s/onclick\(.*\)rel/rel/" $(DOCUMENT)_ramus.html
	sed -i -e "s/<p><br\/><br\/><br\/><\/p><\/div>/xxCLEARLINKSxx\n<\/div>/g" $(DOCUMENT)_ramus.html
	# remove the 1st occurence only 
	sed -i -e "0,/\xxCLEARLINKSxx/s/\xxCLEARLINKSxx//" $(DOCUMENT)_ramus.html
	# Create the do clear links
	# If you don't like it this way, uncomment the next line first, to remove everything
	#sed -i -e "s/xxCLEARLINKSxx//g" $(DOCUMENT)_ramus.html
	sed -i -e "s/\xxCLEARLINKSxx/\[\?do clear_links\(\)\; \?\]/g" $(DOCUMENT)_ramus.html
	sed -i -e "s/xxRAMUS_INITxx/<div id=\"story\" style=\"Display: none;\">\n<div id=\"start\">\n<li>Start: <b><a rel=\"page1\">1<\/a><\/b>/g" $(DOCUMENT)_ramus.html
	


cyoa-txt:
	$(TXT2TAGS) --no-headers -t txt $(DOCUMENT).t2t

cyoa-pdf:
	#$(TXT2TAGS) -t tex --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
	#-pdflatex $(DOCUMENT).tex
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/latex.tex --config-file $(TEXTALLIONFOLDER)/core/txt2cyoa.t2t -t tex --no-toc --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
	-pdflatex -interaction batchmode $(DOCUMENT).tex

cyoa-gamebook: 
# doesnt work yet!
# http://www.ctan.org/tex-archive/macros/latex/contrib/gamebook
	#$(TXT2TAGS) -t tex --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
	#-pdflatex $(DOCUMENT).tex
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/gamebook.tex --config-file $(TEXTALLIONFOLDER)/core/txt2cyoa.t2t -t tex --no-toc --outfile $(DOCUMENT).tex $(DOCUMENT).t2t
	#sed -i -e "s/\\textbf{\\begin{center}\\subsection\*{\\Huge{([^ ].*?)}}\\end{center}\\vskip-2em}/\gbsection{\1}/g" $(DOCUMENT).tex
	sed -i -e "s/\\textbf{\\begin{itemize}/\begin{gbturnoptions}/g" $(DOCUMENT).tex
	sed -i -e "s/\\item/\gbitem/g" $(DOCUMENT).tex
	-pdflatex -interaction batchmode $(DOCUMENT).tex
	
cyoa-epub:
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/epub.html --config-file $(TEXTALLIONFOLDER)/core/txt2cyoa.t2t -t xhtml --no-style --no-toc --outfile $(DOCUMENT).html $(DOCUMENT).t2t
	cat $(DOCUMENT).html | sed -e "s/<audio\(.*\)audio>//g" > $(DOCUMENT)2.html
	mv $(DOCUMENT)2.html $(DOCUMENT).html
	-make tidy
	cat $(DOCUMENT).html | sed -e "s/&#10086;/*/g" | sed -e "s/&#10087;/*/g" | sed -e "s/&#10037;/*/g" | sed -e "s/&#9788;/*/g" | sed -e "s/&#9789;/*/g" | sed -e "s/&#9790;/*/g" | sed -e "s/&#9675;/*/g" |\
	sed -e "s/el[1-9].style.visibility = 'hidden';//g" |\
	sed -e "s/span.c[1-9] {display:none}//g" |\
	sed -e "s/span.c[1-9] {display: none}//g" |\
	sed -e "s/div.c[1-9] {display:none}//g" |\
	sed -e "s/span.c[1-9] {visibility:hidden}//g" > $(DOCUMENT)2.html
	mv $(DOCUMENT)2.html $(DOCUMENT).html
	#
	ebook-convert $(DOCUMENT).html $(DOCUMENT).epub --max-levels 0 --pretty-print --level1-toc //h:h1 --level2-toc //h:h2 --level3-toc //h:h3 --max-toc-links 0 --toc-threshold 3  --chapter-mark pagebreak --page-breaks-before //h:div[@id] --cover $(DOCUMENT_COVER) --extra-css $(TEXTALLIONFOLDER)/includes/epub.css --no-default-epub-cover --no-svg-cover --preserve-cover-aspect-ratio --no-chapters-in-toc
	#
	ebook-meta  $(DOCUMENT).epub --title "$(DOCUMENT_TITLE)" --authors "$(DOCUMENT_AUTHOR)" --tags "$(DOCUMENT_TAGS)" --language $(DOCUMENT_LANGUAGE) --book-producer 'textallion & txt2cyoa - https://bitbucket.org/farvardin/textallion' --comments 'textallion - https://bitbucket.org/farvardin/textallion' --cover $(DOCUMENT_COVER) 


cyoa-graph:	
	printf "\n \n \n \n"  > graph.txt
	-cat $(TEXTALLIONFOLDER)/core/txt2cyoa.t2t $(DOCUMENT).t2t |grep -E "^#|^>|^==|^-|rdv|preproc|postproc" >> graph.txt
	-$(TXT2TAGS) --no-headers -o graph2.txt -t txt graph.txt 
	-echo "digraph G { " > graph.txt
	-echo "bgcolor=beige; "                                          >> graph.txt
	-echo "node [shape=egg, fillcolor=antiquewhite2, style=filled];" >> graph.txt
	-echo "edge [arrowsize=1, fillcolor=gold];"                      >> graph.txt
	-cat graph2.txt | grep -E "\->|;" | sed -e "s/'//g" >> graph.txt
	-echo "} " >> graph.txt
	-cp graph.txt graph2.txt
	#gawk '/->/ { z=$0} /[0-9]+[^->]/ {print z$0}' fichier.txt
	# we convert to graphviz using the python script, and remove the extra "0->" before the conf
	-cat graph2.txt | $(PYTHONVER) $(TEXTALLIONFOLDER)/includes/lines.py  | sed -e "s/0->bgcolor/bgcolor/g" | sed -e "s/0->node/node/g" | sed -e "s/0->edge/edge/g" > graph.txt
	-rm graph2.txt
	-dot graph.txt  -Tpng > $(DOCUMENT)_graph.png
	-dot graph.txt  -Tsvg > $(DOCUMENT)_graph.svg
	-mv graph.txt  $(DOCUMENT)_graph.txt
	
cyoa-gbl:
	printf "$(DOCUMENT)\nPar YourName\n%%date(%Y-%m-%d)\n" > $(DOCUMENT)_gbl.t2t 
	echo "%!includeconf: txt2cyoa.t2t" >> $(DOCUMENT)_gbl.t2t 
	cat $(DOCUMENT).gbl | sed -e "s/= \(.*\) =/\n=== \1 ===\n/g" |\
	# sed -e "s/=\(.*\)=/\n=== \1 ===\n/g" | sed -e "s/= \(.*\)=/\n=== \1 ===\n/g" | sed -e "s/=\(.*\) =/\n=== \1 ===\n/g" |\
	 sed -e "s/^#\([0-9]*\)# \(.*\)/\n\n== \1 ==\n\n=== \2 ===\n\n/g" | sed -e "s/^>\(.*\)=\([0-9]*\)/- \1, rdv au \2/g" >> $(DOCUMENT)_gbl.t2t 

cyoa-togbl:
	cat $(DOCUMENT).t2t | sed -e "s/=== \1 ===/ \(.*\) =/g" |\
	 sed -e "s/^#\([0-9]*\)# \(.*\)/\n\n== \1 ==\n\n=== \2 ===\n\n/g" | sed -e "s/^>\(.*\)=\([0-9]*\)/- \1, rdv au \2/g"  >> $(DOCUMENT)_export.gbl

cyoa-twine:	cyoa-twee

cyoa-twee:
	# for use with http://gimcrackd.com/etc/src/
	# corrected twee parser at https://github.com/mcdemarco/twee
	#echo "du texte et puis à la fin de la ligne, un numéro par exemple 42"|sed -e "s/\([0-9]\+\)$/\[\[\1 \1\]\]/g"
	# try to use Unix line feed if possible
	echo ":: StoryTitle" > $(DOCUMENT)_export.tw 
	cat $(DOCUMENT).t2t  | perl -pe 's/^\%(.*)\n//' |\
	# remove extra textallion syntax |\
	 perl -pe 's/TESTLUCK/\/\/Am I lucky today? (throwing a dice, 5 or 6 means luck)\/\//'|\
	 # abbreviations |\
	 perl -pe 's/tt (\d+)/turn to [[\1]]/g' |\
	 perl -pe 's/rdv au (\d+)/rendez-vous au [[\1]]/g' |\
	 perl -pe 's/rendez-vous au (\d+)/rendez-vous au [[\1]]/g' |\
	 # replace any ="introduction" by 0 |\
	 perl -pe "s/[I-i]ntro["duction"]*/0/g" |\
	 perl -pe "s/\=\"[I-i]ntro["duction"]*/0/g" |\
	 # italic and underline are the same. Only bold is different |\
	 perl -pe "s/\*\*(.*?)\*\*/''\1''/g" |\
	 perl -pe "s/\`\`(.*?)\`\`/{{{\1}}}/g" |\
	 perl -pe 's/¯/ /g' |\
	 perl -pe 's/[ ]*\{.{4}\}[ ]*//g' |\
	 perl -pe 's/[ ]*\{.{3}\}[ ]*//g' |\
	 perl -pe 's/rdv/rendez-vous/' |\
	 # images / [[bla.jpg] 1] est pour image sur premiere page |\
	 perl -pe 's/\[\[(.*).jpg] 1\]//' |\
	 perl -pe 's/\[\[(.*).png] 1\]//' |\
	 perl -pe 's/\[(.*).jpg\]/\[img\[\1.jpg\]\]/' |\
	 perl -pe 's/\[(.*).png\]/\[img\[\1.png\]\]/' |\
	 # why did we remove media? |\
	 #perl -pe 's/..\/media\///' |\
	 perl -pe 's/\[(.*).ogg\]//' |\
	 # choices and links (\r is for windows newline) \
	 perl -pe 's/- (.*?) (\d+?)( *)\n/- \1 [[\2]]\n/g' |\
	 perl -pe 's/- (.*?) (\d+?)( *)\r/- \1 [[\2]]/g' |\
	 # deprecated: perl -pe 's/\[(\d+) \#(.*?)\]/[[\2|\1]]/g' |\
	 perl -pe 's/\[([^\#].*?) \| \#(.*?)\]/[[\1|\2]]/g' |\
	 perl -pe 's/\[([^\#].*?)\|\#(.*?)\]/[[\1|\2]]/g' |\
	 perl -pe 's/\[([^\#].*?) \#(.*?)\]/[[\1|\2]]/g' |\
	 #perl -pe 's/\[(\d+) \#(\d+)\]/[[\1]]/g' |\
	 #perl -pe 's/ \#(\d+?) / [[\1]] /g' |\
	 #perl -pe 's/ \#(.*?) / [[\1]] /g' |\
	 #perl -pe 's/ \#([^ ].*) / [[\1]]/g' |\
	 perl -pe 's/\[\#(\d+?)\]/[[\1]]/g' |\
	 perl -pe 's/\[\#(.*?)\]/[[\1]]/g' |\
	 # notes \
	 perl -pe 's/°°(.*?)°°(.*?)°°/ \/\/\2\/\/ /' |\
	 # chapters \
	 perl -pe 's/== (\d+) ==/:: \1/' |\
	 perl -pe 's/==(\d+)==\[(.*)\]/:: \1/' |\
	 perl -pe 's/==(\d+)==/:: \2/' |\
	 perl -pe 's/== (.*?) ==\[(.*?)\]/:: \2/' |\
	 perl -pe 's/== (.*?) ==/:: \1/' |\
	 # make twee lists \
	 perl -pe 's/^- /* /' |\
	 perl -pe 's/^+ /# /' |\
	 # \
	 perl -pe 's/:: 0/:: Start/' >> $(DOCUMENT)_export.tw 
	 -iconv -f UTF-8 -t ISO-8859-15 $(DOCUMENT)_export.tw -o $(DOCUMENT)_export_iso8859.tw
	# export to html
	$(PYTHONVER) $(TEXTALLIONFOLDER)/templates/twee/twee -t jonah $(DOCUMENT)_export.tw > $(DOCUMENT)_twee_jonah.html
	$(PYTHONVER) $(TEXTALLIONFOLDER)/templates/twee/twee -t sugarcane $(DOCUMENT)_export.tw > $(DOCUMENT)_twee_sugarcane.html
	$(PYTHONVER) $(TEXTALLIONFOLDER)/templates/twee/twee -t mobile $(DOCUMENT)_export.tw > $(DOCUMENT)_twee_mobile.html
	$(PYTHONVER) $(TEXTALLIONFOLDER)/templates/twee/twee -t sugarcube $(DOCUMENT)_export.tw > $(DOCUMENT)_twee_sugarcube.html

cyoa-hyena:
	# for use with http://www.projectaon.org/staff/jens/
	# specifications: http://www.collectingsmiles.com/wiki/index.php?title=Hyena_AudioGame_specifications
	# implementation (player): http://www.freegameengines.org/gamebook-engine/
	printf "#start \nStart the game: Click #page0\n" > $(DOCUMENT)_export.gamebook
	cat $(DOCUMENT).t2t  | perl -pe 's/^\%(.*)\n//' | perl -pe 's/(\d+)\n/Click #page\1\n/' | perl -pe 's/== (\d+) ==/#page\1/' |  perl -pe 's/==(\d+)==\[(.*)\]/page\1/' >> $(DOCUMENT)_export.gamebook 
	printf "\n#script\n\n" >> $(DOCUMENT)_export.gamebook 

cyoa-choicescript:
	# for use with http://www.choiceofgames.com/
	# specifications: http://www.choiceofgames.com/make-your-own-games/choicescript-intro/
	-rm -fr $(DOCUMENT)_choicescript
	-mkdir -p $(DOCUMENT)_choicescript/media/
	-cp -fr $(TEXTALLIONFOLDER)/templates/choicescript/* $(DOCUMENT)_choicescript
	sed "s/ChoiceScript Game/`sed q $(DOCUMENT).t2t`/" $(TEXTALLIONFOLDER)/templates/choicescript/mygame/index.html > $(DOCUMENT)_choicescript/mygame/index.html
	sed "s/ChoiceScript Game/`sed q $(DOCUMENT).t2t`/" $(TEXTALLIONFOLDER)/templates/choicescript/mygame/index_fr.html > $(DOCUMENT)_choicescript/mygame/index_fr.html
	printf "*comment Made using ChoiceScript and Textallion.\n" > $(DOCUMENT)_choicescript/mygame/scenes/textallion.txt 
	#printf "Welcome \n*page_break\n" > choicescript/mygame/scenes/textallion.txt \
	cat $(DOCUMENT).t2t  | perl -pe 's/^\%(.*)\n//' | perl -pe 's/^\%(.*)\n//' |\
	#sed can't replace multiple newlines. So we remove them, and add them back later. \
	 perl -pe  's/\n/NEWLINE/g' |\
	 perl -pe 's/NEWLINENEWLINE- /\n\n*choice\n- /g' |\
	 perl -pe 's/NEWLINE/\n/g' |\
	 perl -pe 's/(\d+)\n/\1\n\t\t*goto \1\n/' |\
	 perl -pe 's/- /\t# \1/' |\
	 perl -pe 's/== (\d+) ==/\n*label \1\n\[b\]\1\[\/b\]/g' |\
	 perl -pe 's/==(\d+)==\[(.*)\]/*label \1\n\[b\]\1\[\/b\]/g' |\
	 perl -pe 's/==(\d+)==/*label \1\n\[b\]\1\[\/b\]/g' |\
	# end of game |\
	 perl -pe 's/FIN/FIN\n*ending/' |\
	 perl -pe 's/THE END/THE END\n*ending/' |\
	 # remove extra textallion syntax |\
	 perl -pe 's/TESTLUCK/\[i\]Am I lucky today? (throwing a dice, 5 or 6 means luck)\[\/i\]/'|\
	 perl -pe 's/¯/ /g' |\
	 perl -pe 's/[ ]*\{.{4}\}[ ]*//g' |\
	 perl -pe 's/[ ]*\{.{3}\}[ ]*//g' |\
	 perl -pe 's|\/\/(.*)\/\/|\[i\]\1\[/i\]|' |\
	 perl -pe 's/\[\[(.*).jpg] 1\]/*image \1.jpg/' |\
	 perl -pe 's/\[\[(.*).png] 1\]/*image \1.jpg/' |\
	 perl -pe 's/\[(.*).jpg\]/*image \1.jpg/' |\
	 perl -pe 's/\[(.*).jpg\]/*image \1.jpg/' |\
	 perl -pe 's/..\/media\///' |\
	 perl -pe 's/\[(.*).ogg\]//' |\
	 perl -pe 's/rdv/rendez-vous/'   >> $(DOCUMENT)_choicescript/mygame/scenes/textallion.txt
	 -cp *.jpg $(DOCUMENT)_choicescript/mygame/
	 -cp *.png $(DOCUMENT)_choicescript/mygame/
	 -cp ../media/*.jpg $(DOCUMENT)_choicescript/mygame/
	 -cp ../media/*.png $(DOCUMENT)_choicescript/mygame/
	 

cyoa-cs: cyoa-choicescript


cyoa-undum:
	# for use with http://www.undum.com/
	# pb with undum if 1 extra line break in the code
	-rm -fr $(DOCUMENT)_undum
	-mkdir -p $(DOCUMENT)_undum/media/
	-cp -fr $(TEXTALLIONFOLDER)/templates/undum_media/* $(DOCUMENT)_undum/media/
	-cat $(TEXTALLIONFOLDER)/templates/undum.html > $(DOCUMENT)_undum/$(DOCUMENT)_undum.html
	-cat $(DOCUMENT).t2t  | perl -pe 's/^\%(.*)\n//' |\
	# remove 1 empty useless image \
	perl -pe 's/\{->--\}\[\[(.*)\] 1\]\{-<--\}//' |\
	# remove 3 first lines of the t2t doc \
	     sed '1,4d' |\
	 perl -pe 's/^\%(.*)\n//' |\
	# remove textallion specific syntax (and later also) \
         perl -pe 's/[ ]*\{.{4}\}[ ]*//g' |\
	 perl -pe 's/[ ]*\{.{3}\}[ ]*//g' |\
	perl -pe  's/\{/NEWLINE/g' |\
	 #sed can't replace multiple newlines. So we remove them, and add them back later. |\
	 perl -pe  's/\n/NEWLINE/g' |\
	 perl -pe  "s/\'/APOSTROPHE/g" |\
	 perl -pe 's/NEWLINENEWLINE- (.*?)(\d+)NEWLINE/<\/p>APOSTROPH2\n\+ APOSTROPH2<ul class=GUILLEMEToptionsGUILLEMET><li><a href=GUILLEMETnode\2GUILLEMET>\1 \2<\/a><\/li>APOSTROPH2NEWLINE/g' |\
	 perl -pe 's/NEWLINENEWLINENEWLINE//g' |\
	 perl -pe 's/NEWLINENEWLINE//g' |\
	 perl -pe 's/\.NEWLINE/. /g' |\
	 perl -pe 's/\. NEWLINE/. /g' |\
	 perl -pe 's/\!NEWLINE/! /g' |\
	 perl -pe 's/\! NEWLINE/! /g' |\
	 perl -pe 's/\?NEWLINE/? /g' |\
	 perl -pe 's/\? NEWLINE/? /g' |\
	 perl -pe 's/»NEWLINE/» /g' |\
	 perl -pe 's/NEWLINE/\n/g' |\
	 perl -pe 's/- (.*?)(\d+)/\+ APOSTROPH2<li><a href=GUILLEMETnode\2GUILLEMET>\1 \2<\/a><\/li>APOSTROPH2/g' |\
	 perl -pe 's/== (\d+) ==/ \n \);\n\n undum.game.situations.node\1 = new undum.SimpleSituation\(\nAPOSTROPH2<p><br\/><h2>\1<\/h2>/g' |\
	 # end of game |\
	 perl -pe 's/FIN/FIN APOSTROPH2/' |\
	 perl -pe 's/THE END/THE END APOSTROPH2/' |\
	 # remove extra textallion syntax |\
	 perl -pe 's/TESTLUCK/\[i\]Am I lucky today? (throwing a dice, 5 or 6 means luck)\[\/i\]/'|\
	 perl -pe 's/¯/ /g' |\
	 perl -pe "s/APOSTROPHE/\\\'/g" |\
	 perl -pe "s/APOSTROPH2/\'/g" |\
	 perl -pe 's/GUILLEMET/\"/g' |\
	 perl -pe 's/[ ]*\{.{4}\}[ ]*//g' |\
	 perl -pe 's/[ ]*\{.{3}\}[ ]*//g' |\
	 perl -pe 's|\/\/(.*)\/\/|\[i\]\1\[/i\]|' |\
	 perl -pe 's/\[\[(.*).jpg] 1\]/*image \1.jpg/' |\
	 perl -pe 's/\[\[(.*).png] 1\]/*image \1.jpg/' |\
	 perl -pe 's/\[(.*).jpg\]/*image \1.jpg/' |\
	 perl -pe 's/\[(.*).jpg\]/*image \1.jpg/' |\
	 perl -pe 's/..\/media\///' |\
	 perl -pe 's/\[(.*).ogg\]//' |\
	 perl -pe 's/rdv/rendez-vous/'  >> $(DOCUMENT)_undum/$(DOCUMENT)_undum.html
	 -echo "); </script></body></html>" >> $(DOCUMENT)_undum/$(DOCUMENT)_undum.html

play-cyoa-renpy:
	#
	$(RENPY) $(DOCUMENT)_renpy


cyoa-renpy:
	# for use with http://renpy.org/
	rm -fr $(DOCUMENT)_renpy
	mkdir $(DOCUMENT)_renpy
	# sed can't replace multiple newlines. So we remove them, and add them back later.
	cp -fr $(TEXTALLIONFOLDER)/templates/renpy/* $(DOCUMENT)_renpy
	touch  $(DOCUMENT)_renpy/.nomedia
	cat $(DOCUMENT).t2t   |\
     sed '1,4d' |\
	 perl -pe 's/^\%(.*)\n//' |\
	 perl -pe 's/^(.*): *\n/    "\1:"\n/'  |\
	 perl -pe 's/\n/NEWLINE/' |\
	 perl -pe 's/NEWLINENEWLINE-/\n\n    menu:\n-/g' |\
	 perl -pe 's/NEWLINE/\n/g' |\
	 perl -pe 's/"    menu:\"/menu:\n/'  |\
	 perl -pe 's/tt (\d+)/continue \1/g' |\
	 perl -pe 's/\(rdv au (\d+)\)/ \1/g' |\
	 perl -pe 's/rdv au (\d+)/ \1/g' |\
	 perl -pe 's/continue[r|z] au (\d+)/continuer \1/g' |\
	 perl -pe 's/rendez-vous au (\d+)/ \1/g' |\
	 perl -pe 's/[A|a]lle[z|r] au (\d+)/: \1/g' |\
	 perl -pe 's/ au (\d+)/ \1/g' |\
	 perl -pe 's/(.*) (\d+)[ ]*\n/        "\1":\n            jump page\2\n/' |\
	 perl -pe 's/(.*) \[(.*) #(.*)\][ ]*\n/        "\1":\n            jump page\2\n/' |\
	 perl -pe 's/== (\d+) ==[ ]*\n/\nlabel page\1:\n    scene bg\n    with None\n/' |\
	 perl -pe 's/==(\d+)==\[(.*)\]\n/\nlabel page\1:\n    scene bg\n    with None\n/' |\
	 perl -pe 's/== (.*) ==[ ]*\n/\nlabel page\1:\n    scene bg\n    with None\n/' |\
	 perl -pe 's/\[\[(.*).jpg] 1\]/    image \1 = "\1.jpg"\n    \x{0024} showmypic("\1")/' |\
	 perl -pe 's/\[\[(.*).png] 1\]/    image \1 = "\1.png"\n    \x{0024} showmypic("\1")/' |\
	 perl -pe 's/\[(.*).jpg\]/    image \1 = "\1.jpg"\n    \x{0024} showmypic("\1")/' |\
	 perl -pe 's/\[(.*).png\]/    image \1 = "\1.png"\n    \x{0024} showmypic("\1")/' |\
	 perl -pe 's/\[(.*).ogg\]/    \x{0024} renpy.music.play("\1.ogg", loop=False)\n/' |\
	 perl -pe 's/\[(.*).mid\]/    \x{0024} renpy.music.play("\1.mid", loop=False)\n/' |\
	 perl -pe 's/..\/images\///g' |\
	 perl -pe 's/..\/media\///g' |\
	 perl -pe 's/¯/ /g' |\
	 perl -pe 's/\[(.*)\]//' |\
	 perl -pe 's/^(.*)\.[ ]*\n/    "\1."/' |\
	 perl -pe 's/^(.*),[ ]*\n/    "\1,"/' |\
	 perl -pe 's/^(.*)”[ ]*\n/    "\1”"/' |\
	 perl -pe 's/^(.*)»[ ]*\n/    "\1»"/' |\
	 perl -pe 's/^(.*)![ ]*\n/    "\1!"/'  |\
	 perl -pe 's/^(.*)\?[ ]*\n/    "\1?"/' |\
	 perl -pe 's/^\/\/(.*)\/\/\n/    "\{i\}\1\{\/i\}"/' |\
	 perl -pe 's/^\*\*(.*)\*\*\n/    "\{b\}\1\{\/b\}"/' |\
	 perl -pe 's/^-(.*)/        "~~ missing line (please review your source code)~~"/' |\
	 perl -pe 's/        \"- /        " /' |\
	 #italic |\
	 #perl -pe 's/\{ \/\/ \}/\{i\}/' |\
	 #perl -pe 's/\{\/\/\/ \}/\{\/i\}/' |\
	 perl -pe 's/\{ \/\/ \}//' |\
	 perl -pe 's/\{\/\/\/ \}//' |\
	 perl -pe 's/\/\/(.*)\/\//\{i\}\1\{\/i\}/' |\
	 perl -pe 's/\{.{3}\}(.*){(.*)\}/    "\2"/' |\
	 perl -pe 's/\{{3}\}//' |\
	 perl -pe 's/TESTLUCK/    "Am I lucky today? (throwing a dice, 5 or 6 means luck)"/'|\
	 perl -pe 's/:\":/\":/'|\
	 perl -pe 's/\" THE END \"/jump end/' |\
	 perl -pe 's/THE END/    jump end/'	 >> $(DOCUMENT)_renpy/game/script.rpy 
	 printf "\nlabel end:\n    show expression Text(\"THE END.\", size=50, yalign=0.5, xalign=0.5, drop_shadow=(2, 2)) as text\n    with dissolve\n    \" \" \n" >> $(DOCUMENT)_renpy/game/script.rpy 
	 

cyoa-inform7-temp:

	# before : remove comments
		 # change " |\
	 # TODO # perl -pe 's/\"(.*)\"/\[\"\]\1\[\"\]/g' |\  

cyoa-inform7:
	 sed 's/^$$/ /' $(DOCUMENT).t2t > $(DOCUMENT).tmp  
	 printf  "== 00 ==\n" >> $(DOCUMENT).tmp  
	 cat $(DOCUMENT).tmp |\
	 perl -pe 's/==(\d+)==\[(.*)\]/== \1 ==/' |\
	 perl -pe 's/== (\d+) ==/\n== \1 ==\n/' |\
	 perl -pe 's/TESTLUCK/Am I lucky today? (throwing a dice, 5 or 6 means luck)/'|\
	 # remove comments |\
	 perl -pe 's/^\%(.*)\n//' |\
	 # remove syntax such as [11 #nord] |\
	 perl -pe 's/\[(\d+) #([^ ].*?)\]/\2/g' |\
	 perl -pe 's/- (.*) \[(.*)\]\n/- \1 \2\n/g' |\
	 perl -pe 's/- (.*) \((.*)\)\n/- \1 \2\n/g' |\
	 perl -pe 's/\[(.*)\]{(.*)\}\n//g' |\
	 perl -pe 's/\[(.*)\]\n//g' |\
	 perl -pe 's/\[(.*)\]//g' |\
	 perl -pe 's/{~~~~}/ /g' |\
	 perl -pe 's/¯/ /g' |\
	 perl -pe 's/\?/\? /g' 	> $(DOCUMENT).tmp2
	 printf "\"$(DOCUMENT_TITLE)\" by $(DOCUMENT_AUTHOR)\n\nInclude Adventure Book by Edward Griffiths.\n\n\n" > $(DOCUMENT).i7
	 printf "The first Page is a page. \"Starting the game...\" It is followed by Page1.\n\n" >> $(DOCUMENT).i7
	 perl $(TEXTALLIONFOLDER)/core/adventurebook.pl $(DOCUMENT).tmp2 >> $(DOCUMENT).i7
	 cat $(DOCUMENT).i7 |\
	 # remove extra spaces |\
	 perl -pe 's/  [ ]*"./ "./g' |\
	 perl -pe 's/  [ ]*/ /g' |\
	 perl -pe 's/\{->--\} FIN \{-<--\}[ ]*"./". \nIt is followed by GameEnd./' |\
	 perl -pe 's/\{->--\} THE END \{-<--\}[ ]*"./". \nIt is followed by GameEnd./' |\
	 # remove extra textallion syntax |\
	 perl -pe 's/{(.*)}\n//g' |\
	 perl -pe 's/{(.*)}//g' > $(DOCUMENT).tmp
	 printf "\nGameEnd is a page. \"** THE END **\".\n\n" >> $(DOCUMENT).tmp
	 #  convert French apostrophes for inform7 source
	 cat $(DOCUMENT).tmp | sed -e "s/S'/S[\']/g" | sed -e "s/N'/N[\']/g" | sed -e "s/L'/L[\']/g" | sed -e "s/C'/C[\']/g" | sed -e "s/D'/D[\']/g" | sed -e "s/J'/J[\']/g" | sed -e "s/M'/M[\']/g"  | sed -e "s/U'/U[\']/g" | sed -e "s/s'/s[\']/g" | sed -e "s/ n'/ n[\']/g" | sed -e "s/l'/l[\']/g" | sed -e "s/ c'/ c[\']/g" | sed -e "s/d'/d[\']/g" | sed -e "s/j'/j[\']/g" | sed -e "s/m'/m[\']/g" | sed -e "s/u'/u[\']/g" > $(DOCUMENT).i7
	 -rm $(DOCUMENT).tmp
	 -rm $(DOCUMENT).tmp2
	 cp -fr $(TEXTALLIONFOLDER)/templates/inform7 ./
	 cp $(DOCUMENT).i7 inform7/cyoa.inform/Source/story.ni
	 cd inform7
	 make z8

cyoa-inform6:
	inform +source_path=$(TEXTALLIONFOLDER)/templates/inform6/ +include_path=./,$(TEXTALLIONFOLDER)/templates/inform6/,/usr/share/inform/include +code_path=./ "$1".inf 

# images (thumbnails)

vignettes:
	sh $(TEXTALLIONFOLDER)/core/vignettes.sh
	mv vignettes_doc.t2t vignettes_$(DOCUMENT).t2t
	sed -i -e "s/@@DOCUMENT_TITLE@@/$(DOCUMENT_TITLE)/"  vignettes_$(DOCUMENT).t2t
	sed -i -e "s/@@DOCUMENT_AUTHOR@@/$(DOCUMENT_AUTHOR)/"  vignettes_$(DOCUMENT).t2t
	$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/xhtml.html -t xhtml --style=$(TEXTALLIONFOLDER)/includes/sample.css --css-inside --no-toc --outfile vignettes_$(DOCUMENT).html vignettes_$(DOCUMENT).t2t


	#$(TXT2TAGS) -T $(TEXTALLIONFOLDER)/templates/xhtml.html -t xhtml --css-inside --toc --outfile #$(DOCUMENT).html $(DOCUMENT).t2t


# CLEAN part 


clean: clean-temp

clean-temp:
	-rm  *~ 
	-rm  .*~
	-rm  $(DOCUMENT).toc
	-rm  $(DOCUMENT).tex
	-rm  $(DOCUMENT)_slide.tex
	-rm  $(DOCUMENT)2.html
	-rm  $(DOCUMENT).ps
	-rm  $(DOCUMENT)_booklet.ps
	-rm  *.log 
	-rm  *.out
	-rm  *.aux
	-rm  *.toc
	-rm  *.nav
	-rm  *.snm
	-rm  *.ilg
	-rm  *.idx
	-rm  *.ind
	-rm  *.fax
	-rm  *.tns



clean-docs:
	-rm -i $(DOCUMENT).pdf
	-rm -i $(DOCUMENT)*.pdf
	-rm -i $(DOCUMENT)*.html
	-rm -i $(DOCUMENT).epub
	-rm -i vignettes_*
	-rm -fri tb
	-rm -i *graph.svg
	-rm -i *graph.png
	-rm -i *graph.txt
	-rm -i *.pdf
	-rm -i *.ps
	-rm -i *.tex
	-rm -i *.html
	-rm -i *.epub

	


cleanall: clean-temp clean-docs

clean-everything: cleanall


DOCUMENTNEWNAME = newname

rename:
	@echo "Edit the DOCUMENTNEWNAME variable at the end of the makefile (no space please)"
	@echo "(current document name to replace is $(DOCUMENT))"
	@echo "(current document new name is $(DOCUMENTNEWNAME))"
	@echo "(We will keep the older $(DOCUMENT).t2t as a reference)"
	@echo "press a key to start"
	@read PAUSE
	-cp -i $(DOCUMENT).t2t $(DOCUMENTNEWNAME).t2t
	-mv $(DOCUMENT).sty $(DOCUMENTNEWNAME).sty
	-mv $(DOCUMENT).css $(DOCUMENTNEWNAME).css
	-mv $(DOCUMENT).jpg $(DOCUMENTNEWNAME).jpg
	-mv $(DOCUMENT).png $(DOCUMENTNEWNAME).png
	-mv $(DOCUMENT).svg $(DOCUMENTNEWNAME).svg
	-sed -i -e "s/$(DOCUMENT)/$(DOCUMENTNEWNAME)/" $(DOCUMENTNEWNAME).t2t
	-sed -i -e "s/$(DOCUMENT)/$(DOCUMENTNEWNAME)/" makefile



