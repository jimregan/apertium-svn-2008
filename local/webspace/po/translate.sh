#!/bin/bash

function translateLang() {
	echo "Language: $1"
	lang=$1
	msgfmt $lang.po
	mv messages.mo ../locale/$lang/LC_MESSAGES/
}                                                                          

translateLang "en_GB"
translateLang "ca_ES"
translateLang "es_ES"

