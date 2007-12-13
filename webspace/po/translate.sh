#!/bin/bash

function translateLang() {
	echo "Language: $1"
	lang=$1
	msgfmt $lang.po
	mv messages.mo ../locale/$lang/LC_MESSAGES/
}                                                                          

translateLang "es_ES"
translateLang "en_GB"
translateLang "ca_ES"
translateLang "eu_ES"
translateLang "es_ES"
translateLang "fr_FR"
translateLang "ro_RO"
translateLang "af_ZA"
translateLang "pt_PT"
translateLang "oc_FR"
translateLang "gl_ES"


