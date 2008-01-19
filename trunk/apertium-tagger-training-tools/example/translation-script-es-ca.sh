#!/bin/sh

#Change to use the desired data and translation direction
DATA=$HOME/apertium/apertium-es-ca-1.0
DIRECTION=es-ca

FORMAT=txt
AUTOMORF=$DATA/$DIRECTION.automorf.bin
AUTOBIL=$DATA/$DIRECTION.autobil.bin
AUTOGEN=$DATA/$DIRECTION.autogen.bin
AUTOPGEN=$DATA/$DIRECTION.autopgen.bin

awk '{printf $0 "^.<sent>$[]" }' |\
apertium-pretransfer |\
apertium-transfer $DATA/trules-$DIRECTION.xml $DATA/trules-$DIRECTION.bin $AUTOBIL |\
lt-proc -d $AUTOGEN  |\
lt-proc -p $AUTOPGEN |\
apertium-re$FORMAT |\
sed -e "s/*\(\\w\+\)/\1/g"

