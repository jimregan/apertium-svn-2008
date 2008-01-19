
if [[ -z $1 || -z $2 ]]
then
  echo "USAGE: $(basename $0) corpus.txt dictionary.bin" 1>&2
  exit 1;
fi

echo "Generating crp file" 1>&2
echo "This may take some time. Please, take a cup of coffee and come back later." 1>&2

cat $1 | $APERTIUM_DESTXT | $LTPROC $2
