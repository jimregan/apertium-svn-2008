#!/bin/sh

#Change to use the desired named pipes or another
#language model 
FIFO_IN=catalan_fifo_in
FIFO_OUT=catalan_fifo_out
LMDATA=catalan.lm

#IMPORTANT: Note that the language model program (apertium-trigrams-langmodel.pl)
#           is executed only once. Then it keep listeing to the named pipes. It reads
#           from FIFO_IN and writes to FIFO_OUT.
#           If there is no instance of the language model running, you need to delete
#           the named pipes, otherwise, this script will never execute another instance 
#           of the language model.

if [[ ! -p  $FIFO_IN  ]]
then
   # There is not any instance of the LM running, launch it
   mknod $FIFO_IN p
   mknod $FIFO_OUT p
   apertium-trigrams-langmodel -e -i $LMDATA -fi $FIFO_IN  -fo $FIFO_OUT  2>/dev/null &
fi

cat > $FIFO_IN
cat $FIFO_OUT
