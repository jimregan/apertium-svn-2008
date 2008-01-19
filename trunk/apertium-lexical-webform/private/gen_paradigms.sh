#for i in es-ca es-gl es-pt es-ro fr-ca oc-ca
for i in $(ls -d paradigmas.??-??)
do
	lo=${i:11:2}
	lm=${i:14:2}
	echo "Generant paradigmes per a '${i}'..."
	$(
	  cd ${i}
	  rm paradig*_$lo
	  rm paradig*_$lm
	  xsltproc ../paradigmas.xsl paradig.$lo-$lm.$lo.xml | ../creaparadig.awk
	  xsltproc ../paradigmas.xsl paradig.$lo-$lm.$lm.xml | ../creaparadig.awk
	)
	echo "ok"
done
