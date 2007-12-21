export LC_CTYPE=$(locale -a|grep -i "utf[.]*8"|head -1);
if [[ LC_CTYPE == "" ]]
then echo "Error: install an UTF-8 locale in your system";
     exit 1;
fi

awk '{gsub("Ĉ","Cx"); gsub("Ĝ","Gx"); gsub("Ĥ","Hx"); gsub("Ĵ","Jx");\
      gsub("Ŝ","Sx"); gsub("ĉ","cx"); gsub("ĝ","gx"); gsub("ĵ","jx");\
      gsub("ĥ","hx"); gsub("ŝ","sx"); gsub("ŭ","ux"); gsub("Ŭ","Ux");\
      print;}' $1 
