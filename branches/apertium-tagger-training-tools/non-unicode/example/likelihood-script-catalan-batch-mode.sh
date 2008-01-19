#!/bin/sh

#Change to use the desired language model 
LMDATA=catalan.lm

apertium-trigrams-langmodel-perline -e -i $LMDATA  2>/dev/null
