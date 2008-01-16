
if [[ -z $1 || -z $2 || -z $3 ]]
then
  echo "USAGE: $(basename $0) dictionary.dix tagset.tsx tagsetname" 1>&2
  exit 1;
fi

DIXFILE=$1
TSXFILE=$2
NAME=$3

echo "Generating tsx file" 1>&2
echo "This may take some time. Please, take a cup of coffee and come back later." 1>&2

lt-expand $DIXFILE | grep -v "__REGEXP__" | grep -v ":<:" |\
awk 'BEGIN{FS=":>:|:"}{print $2}' > dicexpanded

cat dicexpanded | grep -v "+" |\
sed -re "s/^[^<]+//g" |  sed -re "s/[^>]*$//g" |\
sort | uniq |\
awk '{gsub("><","."); gsub("<",""); gsub(">",""); print;}' > singletags

cat dicexpanded | grep "+" |\
sed -re "s/^[^<]+//g" | sed -re "s/[^>]*$//g" |\
sed -re "s/\+[^<]+/+/g" | sort | uniq |\
awk '{gsub("><","."); gsub("<",""); gsub(">",""); print;}' > multipletags

NSINGLE=$(cat singletags | wc -l)
NMULTIPLE=$(cat multipletags | wc -l)

echo "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>" > $TSXFILE
echo "<tagger name=\""$NAME"\">" >> $TSXFILE
echo "<tagset>" >> $TSXFILE

echo "<!--" >> $TSXFILE
echo "  "$NSINGLE" single tags" >> $TSXFILE
echo "  "$NMULTIPLE" multiple tags" >> $TSXFILE
echo "-->" >> $TSXFILE

cat singletags |\
while read tag ; do
  echo "  <def-label name=\""$tag"\" closed=\"true\">" >> $TSXFILE
  echo "    <tags-item tags=\""$tag"\"/>" >> $TSXFILE
  echo "  </def-label>" >> $TSXFILE
done

echo "  <def-label name=\"num\" closed=\"true\">" >> $TSXFILE
echo "    <tags-item tags=\"num\"/>" >> $TSXFILE
echo "  </def-label>" >> $TSXFILE

cat multipletags |\
while read tag ; do
  echo "  <def-mult name=\""$tag"\" closed=\"true\">" >> $TSXFILE
  echo "    <sequence>" >> $TSXFILE
  echo $tag | tr "+" "\n" |\
  while read subtag ; do
    echo "      <tags-item tags=\""$subtag"\"/>" >> $TSXFILE
  done
  echo "    </sequence>" >> $TSXFILE
  echo "  </def-mult>" >> $TSXFILE
done

echo "</tagset>" >> $TSXFILE
echo "</tagger>" >> $TSXFILE

apertium-validate-tagger $TSXFILE

echo "   "$NSINGLE" single tags"
echo "   "$NMULTIPLE" multiple tags"

echo "done."

rm -f dicexpanded singletags multipletags
