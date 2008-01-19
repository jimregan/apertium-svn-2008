#!/usr/bin/awk -f
BEGIN{
  FS=":";
}

{
  print $2 >> "paradig" $1;
}