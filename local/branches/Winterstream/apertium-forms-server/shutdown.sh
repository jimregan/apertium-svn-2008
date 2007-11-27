#!/bin/sh

PID=`ps a | grep forms | head -1 | cut -f1 -d' '`
echo $PID
kill -9 $PID
PID=`ps a | grep forms | head -1 | cut -f2 -d' '`
echo $PID
kill -9 $PID
