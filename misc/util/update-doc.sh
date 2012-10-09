#!/bin/sh

cd trunk/build
make doc-all
cd doc

ftp -v -n -i david.osborn.name <<EOF
user habitual heidiness
binary
cd public_html/page/doc
mput page.dvi page.html page.info page.pdf page.ps page.txt
quit
EOF
