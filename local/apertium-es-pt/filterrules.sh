case $# in
  2)
    LANG=$1
    ;;
  *)
    echo "USAGE: filter.sh LANG FILE";
    echo "With SIDE one of 'left' or 'right'";
    exit 1;
esac

xsltproc --stringparam lang $LANG filterrules.xsl $2

