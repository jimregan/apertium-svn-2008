if [[ -z $1 ]]
then
  echo "USAGE: $(basename $0) transfer-rules.xml" 1>&2
  exit 1
fi

$XSLTPROC $XSLFILE $1
