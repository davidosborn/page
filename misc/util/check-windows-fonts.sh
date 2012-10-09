#!/bin/bash

# NOTE: run on Cygwin; requires process substitution

expected='ahronbd.ttf andlso.ttf angsa.ttf angsab.ttf angsai.ttf angsau.ttf angsaub.ttf angsaui.ttf angsauz.ttf angsaz.ttf arial.ttf arialbd.ttf arialbi.ttf ariali.ttf ariblk.ttf artrbdo.ttf artro.ttf batang.ttc browa.ttf browab.ttf browai.ttf browau.ttf browaub.ttf browaui.ttf browauz.ttf browaz.ttf comic.ttf comicbd.ttf cordia.ttf cordiab.ttf cordiai.ttf cordiau.ttf cordiaub.ttf cordiaui.ttf cordiauz.ttf cordiaz.ttf cour.ttf courbd.ttf courbi.ttf coure.fon couri.ttf david.ttf davidbd.ttf davidtr.ttf estre.ttf framd.ttf framdit.ttf frank.ttf gautami.ttf georgia.ttf georgiab.ttf georgiai.ttf georgiaz.ttf gulim.ttc impact.ttf kaiu.ttf l_10646.ttf latha.ttf lucon.ttf lvnm.ttf lvnmbd.ttf mangal.ttf marlett.ttf micross.ttf mingliu.ttc modern.fon mriam.ttf mriamc.ttf mriamfx.ttf mriamtr.ttf msgothic.ttc msmincho.ttc mvboli.ttf nrkis.ttf pala.ttf palab.ttf palabi.ttf palai.ttf raavi.ttf rod.ttf rodtr.ttf roman.fon script.fon serife.fon shruti.ttf simfang.ttf simhei.ttf simkai.ttf simpbdo.ttf simpfxo.ttf simpo.ttf simsun.ttc smalle.fon sserife.fon sylfaen.ttf symbol.ttf symbole.fon tahoma.ttf tahomabd.ttf times.ttf timesbd.ttf timesbi.ttf timesi.ttf tradbdo.ttf trado.ttf trebuc.ttf trebucbd.ttf trebucbi.ttf trebucit.ttf tunga.ttf upcdb.ttf upcdbi.ttf upcdi.ttf upcdl.ttf upceb.ttf upcebi.ttf upcei.ttf upcel.ttf upcfb.ttf upcfbi.ttf upcfi.ttf upcfl.ttf upcib.ttf upcibi.ttf upcii.ttf upcil.ttf upcjb.ttf upcjbi.ttf upcji.ttf upcjl.ttf upckb.ttf upckbi.ttf upcki.ttf upckl.ttf upclb.ttf upclbi.ttf upcli.ttf upcll.ttf verdana.ttf verdanab.ttf verdanai.ttf verdanaz.ttf webdings.ttf wingding.ttf wst_czech.fon wst_engl.fon wst_fren.fon wst_germ.fon wst_ital.fon wst_span.fon wst_swed.fon'
expected=`echo "$expected" | tr ' ' "\n"`
installed=`cd 'c:\windows\fonts' && ls`

diff=`diff -u <(echo "$expected") <(echo "$installed")`

missing=`grep '^-[^-]' <(echo "$diff") | sed 's/^-//'`
extraneous=`grep '^\+[^+]' <(echo "$diff") | sed 's/^\+//'`

echo 'Fonts that should be installed, but are not:'
echo '--------------------------------------------'
echo "$missing"
echo

echo 'Fonts that are installed, but should not be:'
echo '--------------------------------------------'
echo "$extraneous"
echo
