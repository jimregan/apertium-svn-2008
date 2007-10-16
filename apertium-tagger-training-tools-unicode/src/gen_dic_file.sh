
if [[ -z $1 || -z $2 || -z $3 ]]
then
  echo "USAGE: $(basename $0) dictionary.dix dictionary.bin tagger.tsx" 1>&2
  exit 1;
fi

DIXFILE=$1
BINFILE=$2
TSXFILE=$3

echo "Generating dic file" 1>&2
echo "This may take some time. Please, take a cup of coffee and come back later." 1>&2

apertium-validate-dictionary $DIXFILE
apertium-validate-tagger $TSXFILE


lt-expand $DIXFILE | grep -v "__REGEXP__" | grep -v ":<:" |\
awk 'BEGIN{FS=":>:|:"}{print $1 ".";}' > /tmp/dic.expanded
echo "." >> /tmp/dic.expanded
echo "?" >> /tmp/dic.expanded
echo ";" >> /tmp/dic.expanded
echo ":" >> /tmp/dic.expanded
echo "!" >> /tmp/dic.expanded
echo "42" >> /tmp/dic.expanded
echo "," >> /tmp/dic.expanded
echo "(" >> /tmp/dic.expanded
echo "[" >> /tmp/dic.expanded
echo ")" >> /tmp/dic.expanded
echo "]" >> /tmp/dic.expanded
echo "¿" >> /tmp/dic.expanded
echo "¡" >> /tmp/dic.expanded

cat /tmp/dic.expanded | apertium-destxt | lt-proc -a $BINFILE | \
apertium-filter-ambiguity $TSXFILE

rm /tmp/dic.expanded
