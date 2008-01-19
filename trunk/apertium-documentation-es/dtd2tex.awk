#!/usr/bin/awk -f

BEGIN{
  print "\\begin{figure}[here]";
  print "\\begin{small}";
  print "\\begin{alltt}";
  FS=" ";
}

{
  anterior = 0;
  for(i = 1 ; i <= NF; i++)
  {
    if(anterior)
    {
      $i = "\\textbf{" $i "}";
    }
    anterior = gsub("ELEMENT", "\\textsl{ELEMENT}", $i);
    gsub("ATTLIST", "\\textsl{ATTLIST}", $i);
    gsub("#PCDATA", "\\textsl{#PCDATA}", $i);
    gsub("IDREF", "\\textsl{IDREF}", $i);
    gsub("#REQUIRED", "\\textsl{#REQUIRED}", $i);
    gsub("#IMPLIED", "\\textsl{#IMPLIED}", $i);
    gsub("EMPTY", "\\textsl{EMPTY}", $i);
    gsub(" ID", " \\textsl{ID}", $i);
  }

  print $0;
}
END{
  print "\\end{alltt}";
  print "\\end{small}";
  print "\\end{figure}";
}