<?php
    include("config.php"); //archivo de configuración

    if($tipo=="n")
    {
	$cadFich="noun";
	$show=": noun";
	$tag="<s n=\"n\"/>";
	$tagout="<n>";
	
    }
    if($tipo=="adj")
    {
	$cadFich="adj";
	$show=": adjectives";
	$tag="<s n=\"adj\"/>";
	$tagout="<adj>";
	
    }
    if($tipo=="vb")
    {
	$cadFich="verb";
	$show=": verbs";
	$tag="<s n=\"vblex\"/>";
	$tagout="<vblex>";
	$nota="";
    }
    
    if($tipo=="prep")
    {
	$cadFich="pr";
	$show=": prepositions";
	$tag="<s n=\"pr\"/>";
	$tagout="<pr>";
    }
    
    if($tipo=="adv")
    {
	$cadFich="adv";
	$show=": adverbs";
	$tag="<s n=\"adv\"/>";
	$tagout="<adv>";
	$nota="";
    }
    
    if($tipo=="cnjadv")
    {
	$cadFich="cnjadv";
	$show=": adverbial conjunctions";
	$tag="<s n=\"cnjadv\"/>";
	$tagout="<cnjadv>";
	$nota="";
    }
    
    if($tipo=="pname")
    {
	$cadFich="pname";
	$show=": proper names";
	$tag="<s n=\"np\"/>";
	$tagout="<np>";
	$nota="";
    }
       
     if($tipo=="acr")
    {
	$cadFich="acr";
	$show=": acronyms";
	$tag="<s n=\"n\"/><s n=\"acr\"/>";
	$tagout="<n><acr>";
	$nota="";
    }


  
	//Rutas de los diccionarios temporales
	$path="../dics";
	

	$fich_LR="$path/apertium-$LR-$RL.$LR.webform";
	$fich_RL="$path/apertium-$LR-$RL.$RL.webform";
    	$fich_LRRL="$path/apertium-$LR-$RL.$LR-$RL.webform";
	
	$fich_semi="$path/$LR-$RL.semi.dic";

        //Rutas de los paradigmas de flexión
	
 

	$path_paradigmasLR="./paradigmas.$LR-$RL/paradig".$cadFich."_".$LR;
    	$path_paradigmasRL="./paradigmas.$LR-$RL/paradig".$cadFich."_".$RL;

	//Rutas de los dic. compilados
    	$aut_LRRL=$dicos_path."/apertium-$LR-$RL/$LR-$RL.automorf.bin";
    	$aut_RLLR=$dicos_path."/apertium-$LR-$RL/$RL-$LR.automorf.bin";
	
	
	function esVocalFuerte($letra){
	  if($letra=="a"||$letra=="e"||$letra=="o") return true;
	  else return false;
	}

	function esVocalDebil($letra){
	  if($letra=="i"||$letra=="u") return true;
	  else return false;
	}
	
	function esVocal($letra){
	  return esVocalDebil($letra)||esVocalFuerte($letra);
	}
	
	function diptongo($vocal1, $vocal2){
	  if((esVocalDebil($vocal1)&&esVocalDebil($vocal2))|| 
	     (esVocalFuerte($vocal1)&&esVocalDebil($vocal2))||
	     (esVocalDebil($vocal1)&&esVocalFuerte($vocal2)))
	    return true;
	  else return false;
	}

	//acentúa una cadena para ciertas formas verbales del castellano
	function acentuar($cadena,$siguienteletra){
	  //quitar todos los acentos
	  $cadena=strtr($cadena,"áéíóúÁÉÍÓÚ","aeiouAEIOU");
	  $comienzo=strlen($cadena)-1;
	  if(diptongo($cadena[$comienzo],$siguienteletra))
	    $comienzo--;
	  for(;$comienzo>=0;$comienzo--){
	    if(esVocal($cadena[$comienzo])){
	      if(esVocalFuerte($cadena[$comienzo])){
	        $cadena[$comienzo]=strtr($cadena[$comienzo],"aeoAEO","áéóÁÉÓ");
	        return $cadena;
	      }
	      else if(esVocal($cadena[$comienzo-1])){
	        $comienzo--;
	        $cadena[$comienzo]=strtr($cadena[$comienzo],"aeiouAEIOU","áéíóúÁÉÍÓÚ");
	        return $cadena;
	      }
	      else{
	        $cadena[$comienzo]=strtr($cadena[$comienzo],"aeiouAEIOU","áéíóúÁÉÍÓÚ");
	        return $cadena;
	      }
	    }  
	  }    
	  return $cadena;
	}

//ARANES!!
          function esMayuscula($letra){
            $letra=strtr($letra,"ABCDEFGHIJKLMNOPQRSTUVWXYZ","###########################");
            if($letra=="#") return true;
            else return false;
          }
                        

        //devuelve true si la cadena tiene acento, false si no hay acento
        function TieneAcento($cadena){
          $acento=strrpos($cadena,"á");
          $acento=$acento || strrpos($cadena,"é");
          $acento=$acento || strrpos($cadena,"í");
          $acento=$acento || strrpos($cadena,"ó");
          $acento=$acento || strrpos($cadena,"ú");
          $acento=$acento || strrpos($cadena,"Á");
          $acento=$acento || strrpos($cadena,"É");
          $acento=$acento || strrpos($cadena,"Í");
          $acento=$acento || strrpos($cadena,"Ó");
          $acento=$acento || strrpos($cadena,"Ú");
          $acento=$acento || strrpos($cadena,"à");
          $acento=$acento || strrpos($cadena,"è");
          $acento=$acento || strrpos($cadena,"ì");
          $acento=$acento || strrpos($cadena,"ò");
          $acento=$acento || strrpos($cadena,"ù");
          $acento=$acento || strrpos($cadena,"À");
          $acento=$acento || strrpos($cadena,"È");
          $acento=$acento || strrpos($cadena,"Ì");
          $acento=$acento || strrpos($cadena,"Ò");
          $acento=$acento || strrpos($cadena,"Ù");
          //acento es false si no te accent

//          echo "cadena ".$cadena." tiene acento en: ".$acento.".";

          return $acento;        
        }

function accentua($paraula,$sentit){
//accentua l'ultima vocal accentuable d'una paraula amb un accent tancat/obert depenent del $sentit indicat
        $comienzo=strlen($paraula)-1;
        for($comienzo;$comienzo>=0;$comienzo--){
          if(esVocal($paraula[$comienzo])){
            if($sentit == 1) //accent tancat
            {
              $paraula[$comienzo]=strtr($paraula[$comienzo],"aeiouAEIOU","àéíóúÀÉÍÓÚ");
            }
            else //accent obert
            {
              $paraula[$comienzo]=strtr($paraula[$comienzo],"aeiouAEIOU","àèíòúÀÈÍÒÚ");
            }
            $comienzo=-1;
          }
        }
        return $paraula;
}
                                                                                                                                                                                        

	//acentúa una cadena para ciertas formas verbales del castellano
	function PonQuitaAcento($cadena,$cadena_paradigma){
	  //mira l'arrel de cadena_paradigma i si està accentuat posa accent en cadena (si no el te)
	  //si no està accentuat lleva l'accent en cadena (si el te)

          $token=strtok($cadena_paradigma,"/");            //agafem l'arrel del paradigma

          $acento_lem=TieneAcento($cadena);       //mira si te accent la cadena del lema
          $acento_par=TieneAcento($token);	  //mira si te accent la cadena del paradigma

          if($acento_par !== false){ 	//si te accent el paradigma....
            if($acento_lem === false){	//y el lema no te accent, accentua l'arrel del lema
              $comienzo=strlen($cadena)-1;
              for($comienzo;$comienzo>=0;$comienzo--){
	        if(esVocal($cadena[$comienzo])){
	          $cadena[$comienzo]=strtr($cadena[$comienzo],"aeiouAEIOU","àéíóúÀÉÍÓÚ");
	          $comienzo=-1;
                }
              }
            }	// si ya esta acentuado lema no hace nada.
          }
          else{  //y si no te accent el paradigma...
            if($acento_lem !== false){	//y el lema te accent, lleva l'acent de l'arrel del lema
              //quitar todos los acentos de la cadena del lema
              $cadena=strtr($cadena,"áéíóúÁÉÍÓÚàèìòùÀÈÌÒÙ","aeiouAEIOUaeiouAEIOU");
            }
          }

	  return $cadena;
	}
	
	
	function PosicioVocalTall($lema, $vocal, $infinitiu){
	// torna la posició dins del $lema on es troba la $vocal que separa l'arrel de la desinencia.
	// És necessari la terminació d'infinitiu del verb $lema. Busquem la vocal del final cap al principi
	// tornem la primera ocurrència de $vocal
	  $i=strlen($lema)-strlen($infinitiu)-1;
	  $posicio=false;
	  for( ; $i >= 0; $i--){
		if($lema[$i] == $vocal) { $posicio=$i; $i=-1; }
          }	
          return $posicio;
	}

//FI ARANES!!

    
 //Se comprueban los generos de los paradigmas para ver si es GD
	
if($tipo=="n" || $tipo=="adj" || $tipo=="acr")
{
	//se obtienen los datos del paradigma de LR actual
      $cadparadigma=shell_exec("grep ^$paradigmaLR $path_paradigmasLR");
      //La cadena devuelta por el grep la metemos en un array separando las pal
      $array_paradigma_LR=explode("\t",$cadparadigma);
      $generosLR=$array_paradigma_LR[count($array_paradigma_LR)-1]; 	
      $generosLR=substr($generosLR,0,-1); // se quita el \n final
      $numLR=$array_paradigma_LR[count($array_paradigma_LR)-2]; 	


	//se obtienen los datos del paradigma de RL actual
      $cadparadigma=shell_exec("grep ^$paradigmaRL $path_paradigmasRL");
      //La cadena devuelta por el grep la metemos en un array separando las pal
      $array_paradigma_RL=explode("\t",$cadparadigma);
      $generosRL=$array_paradigma_RL[count($array_paradigma_RL)-1]; 
      $generosRL=substr($generosRL,0,-1); // se quita el \n final	
      $numRL=$array_paradigma_RL[count($array_paradigma_RL)-2]; 	

      
      if($paradigmaLR!="Altrecas")
      	      $genLR=explode(" ",$generosLR);
	
      else
       	       $genLR[0]=$geneLR;
      	       
      if($paradigmaRL!="Altrecas")           
            $genRL=explode(" ",$generosRL);
      else	    
	    $genRL[0]=$geneRL;	  
	    
 
      
      if($generosLR!=$generosRL || $geneLR!=$geneRL)	// si no coinciden ambos generos
      {
  
	   $GDLR=0;
	   $GDRL=0;
	   $cambGen=0;
	   if($genLR[0]=="mf" && $genRL[0]!="mf" && count($genRL)>1)
	   {
		$GDRL=1;	   	  	
		//echo "GD RL";
	   }else if($genLR[0]!="mf" && $genRL[0]=="mf" && count($genLR)>1)
	   {
		$GDLR=1;		
		//echo "GD LR";
	   }else if(count($genLR)==1 && count($genRL)>1)
	   {
	   	$GDRL=1;
	   }else if(count($genLR)>1 && count($genRL)==1)
	   {
	   	$GDLR=1;
		//echo "GDlr";
	   }else if($genLR[0]!=$genRL[0] && count($genLR)==1 && count($genRL)==1)  //cambia el género 
	   {
	   	$cambGen=1;			
	   	//echo "cb gen";
	   }

	   //echo "<br>Comprovar gènere";
      }
      
}

	

   if(!isset($accion)) {
   
          
	//se eliminan las barras '\' de las cadenas con los lemas
        $LRlem=str_replace("\\","",$LRlem);      
        $RLlem=str_replace("\\","",$RLlem);  
      	$LRlem=str_replace(" ","<b/>",$LRlem);      
        $RLlem=str_replace(" ","<b/>",$RLlem); 
   
      $warning="";
      
      // Buscamos a ver si estan las entradas en los diccionarios temporales
      //LR

     $cadMay="\">".$LRlem."<\"";

      system("grep $cadMay $fich_LRRL > /dev/null", $retval);

        
      if ($retval==0)	//si existe una cadena igual
      {
         
	 $encontr1=shell_exec("grep  $cadMay $fich_LRRL ");	//se recorre cada uno de las lineas con la palabra..
	 $cadenas=explode("\n",$encontr1);			 // ..y se genera el texto con el error
	 $showErrLR=0;
	 $cad1="";
	 for($i=0 ; $i<sizeof($cadenas) ; $i++)
	 {
	 	if( strstr( $cadenas[$i],$tagout)!=FALSE )
		{
			 $cad1.="<br/>".htmlspecialchars($cadenas[$i]);         
			 $showErrLR=1;
		}
	 }
	 
	 
	 if($showErrLR!=1)     //muestra advertencia
                $cad1="";
	
         if(strlen($cad1)>0)
                 $warning.="This word is already on the temporary dictionary:".$cad1 ; 

    
      }//fin if retval==0
      
	
      
	//RL

	  $cadMay="\">".$RLlem."<\"";
  
      system("grep $cadMay $fich_LRRL > /dev/null", $retval);
      if ($retval==0)
      {
        
	 
	
	 $encontr1=shell_exec("grep  $cadMay $fich_LRRL ");
	 $cadenas=explode("\n",$encontr1);
	 $showErrRL=0;
	 $cad1="";
	 for($i=0 ; $i<sizeof($cadenas) ; $i++)
	 {
	 	if( strstr( $cadenas[$i],$tagout)!=FALSE )
		{
			 $cad1.="<br/>".htmlspecialchars($cadenas[$i]);         
			 $showErrRL=1;
		}
	 }
	     
	 if($showErrRL!=1)     //muestra advertencia
                $cad1="";

	
                 
         if(strlen($cad1)>0)
                 $warning.="This word is already on the temporary dictionary:".$cad1 ; 

        
	

      }//fin if retval==0 
      
      
      //miramos si ya se encuentra en los diccionarios compilados
      
	 $miracomp= "echo '$LRlem' | $anmor $aut_LRRL";
         $sal_anmor_LR = shell_exec($miracomp);
  	if ($sal_anmor_LR[0]!='*') {
	  if (strlen($warning)>0)	//crea la cadena de advertencia a mostrar
	    $warning.="<br/>";
         $cad="This word is already on the dictionary ($LR)."; 

	 $cadenas=explode("/",$sal_anmor_LR);
	 $showErr=0;
	 
	 for($i=0 ; $i<sizeof($cadenas) ; $i++)
	 {
	 	if( strstr( $cadenas[$i],$tagout)!=FALSE )
		{
			 $cad.="<br/>".htmlspecialchars($cadenas[$i]);         
			 $showErr=1;
		}
	 }
	 if($showErr==1)     //muestra advertencia
         {
                //print("<b class=\"adv\">*$LR</b>");
                $warning.=$cad."</center>";
         }

	   	
	 
        }
	
	
	
	$miracomp2= "echo '$RLlem' | $anmor $aut_RLLR";
  	$sal_anmor_RL = shell_exec($miracomp2);
 	if ($sal_anmor_RL[0]!='*') {
	  if (strlen($warning)>0)	//crea la cadena de advertencia a mostrar
	    $warning.="<br/>";
         $cad="This word is already on the dictionary ($RL)."; 

	 $cadenas=explode("/",$sal_anmor_RL);
	 $showErr=0;
	 
	 for($i=0 ; $i<sizeof($cadenas) ; $i++)
	 {
	 	if( strstr( $cadenas[$i],$tagout)!=FALSE )
		{
			 $cad.="<br/>".htmlspecialchars($cadenas[$i]);         
			 $showErr=1;
		}
	 }
	 if($showErr==1)     //muestra advertencia
         {
                //print("<b class=\"adv\">*$RL</b>");
                $warning.=$cad."</center>";
         }
	 
        }
	

      
      // coge las posibles terminaciones
      $toktmp=strtok($paradigmaLR,"/");
      $vectParLR=explode(" ",$paradigmaLR);
      $arr=explode("/",$vectParLR[0]);
      if(sizeof($arr)==2)
	      $vectParLR[0]="/".$arr[1];
      else
              $vectParLR[0]=" ";
	     
       

      
      $toktmp=strtok($paradigmaRL,"/");
      $vectParRL=explode(" ",$paradigmaRL);
      $arr=explode("/",$vectParRL[0]);
      if(sizeof($arr)==2)
	      $vectParRL[0]="/".$arr[1];
      else
              $vectParRL[0]=" ";
            
      $partmp=strtok($paradigmaLR," ");			//coge el nombre del paradigma hasta el primer espacio
      $paradigmaLR=$partmp;				// ya q este puede tener información adicional
      							//ej;  alt/o /a /os /as -> solo quiero "alt/o"
     
      $partmp=strtok($paradigmaRL," ");
      $paradigmaRL=$partmp;
      
      
 
      if($valida!=1)
		$ac="insertarPro.php";
      else	
		$ac="valida.php"; 
		
      echo "<body>\n"; 
     

	echo "<form action=\"./$ac\">\n"; 
     
      
      //Debemos calcular la raiz de la nueva palabra, lo hacemos a partir del paradigma
      $token=strtok($paradigmaLR,"/");
      $raiz_LR=substr($LRlem, 0, strlen($LRlem)-(strlen($paradigmaLR)-strlen($token)-1));
	
      //genera los ejemplos
      $ejemplosLR="";
      if(sizeof($vectParLR)>0)
      {
      	for($cont=0; $cont < sizeof($vectParLR) ;$cont++){

            $flexion=$vectParLR[$cont];

//ARANES!!
            if($LR == "oc" || $LR == "fr"){            
                  $flexion=substr($flexion,1,strlen($flexion)-1);
                  
                  //se obtienen los datos del paradigma de LR actual
                  $cadparadigma=shell_exec("grep ^$paradigmaLR $path_paradigmasLR");
                  //La cadena devuelta por el grep la metemos en un array separando las pal
                  $cadenae=explode("\t",$cadparadigma);  //en cadenae[2] tenim el primer paradigma associat

                  $token=$cadenae[2]; //agafem el paradigma
        
                  $inicio_inf=strpos($token,"]");
                  if($inicio_inf === false){
                      $inicio_inf=strpos($token,"/")+1;
                      $parametritzat=0;
                  }
                  else{
                      $inicio_inf=$inicio_inf+1;
                      $parametritzat=1;
                  }                               //mira on comença l'infinitiu
                                                                                                                                                                                                                                 
                               
//                  $inicio_inf=strpos($token,"]");
//                  if($inicio_inf === false){ $inicio_inf=0; }
//                  else{ $inicio_inf=$inicio_inf+1; }                              //mira on comença l'infinitiu

                  if( $parametritzat == 1){
                  
                      $long_fixa_par= strpos($token,"]") - strpos($token,"[") - 1;   //longitud de la part fixa del paradigm
                      if($long_fixa_par==-1){$long_fixa_par=0;}

                      $infinitiu=substr($token,$inicio_inf,strpos($token,"__")-$inicio_inf );  //terminació de l'infinitiu;

//                  $vocal=$flexion[0];
                      $vocal_de_flexion=$flexion[0];
                         

                      $vocal=substr( $token,strpos($token,"/")+1,1 );
                      $inicio_flexion=1+$long_fixa_par;
                      $flexion=substr($flexion,$inicio_flexion,strlen($flexion)-$inicio_flexion);

                      $long_fixa_lema= (strlen($LRlem)-strlen($infinitiu)-1) - PosicioVocalTall($LRlem,$vocal,$infinitiu);

                      $raiz_LR=substr($LRlem, 0, strlen($LRlem)-(1 + $long_fixa_lema + strlen($infinitiu) ));
                      //CALCULEM l'ARREL del lema a introduir (el que hi haurà davant de \ )
                      //pendent de millorar
                      $fixa=substr($LRlem,strlen($raiz_LR)+1,$long_fixa_lema);
                      //CALCULEM la PART FIXA del lema (el que es posaria entre [])
                      //pendent de millorar
                  }
                  else{
                      $vocal=$fixa="";
                      $long_fixa_lema=0;
                  }                                                                                                                                                                                                                                                                                                                                                              
//                  $flexion="/".$vocal.$fixa.$flexion;
                  $flexion="/".$vocal_de_flexion.$fixa.$flexion;

            }
//FI ARANES!!
                      
          $ejemplosLR.=" ".$raiz_LR."<b style=\"color:red\">".$flexion."</b>,";
        }
      }

      
     
     
      $token=strtok($paradigmaRL,"/");
      $raiz_RL=substr($RLlem, 0, strlen($RLlem)-(strlen($paradigmaRL)-strlen($token)-1));
      
  
      
      //genera los ejemplos
      $ejemplosRL="";
      if(sizeof($vectParRL)>0)
      {
      	for($cont=0; $cont < sizeof($vectParRL) ;$cont++)
		$ejemplosRL.=" ".$raiz_RL."<b style=\"color:red\">".$vectParRL[$cont]."</b>,";
      }
   

      //Montamos la cadena a insertar en el diccionario LR
      $cadparadigma=shell_exec("grep ^$paradigmaLR $path_paradigmasLR");
      //La cadena devuelta por el grep la metemos en un array separando las palabras
      $array_paradigma_LR=explode("\t",$cadparadigma);
      $cad_LR="";
      //Calculamos la dirección para LR
      $direccion_LR=$direccion;

      //Calculamos direccion RL      
      if ($direccion==":<:")
         $direccion_RL=":>:";
      else if ($direccion==":>:")
         $direccion_RL=":<:";	 
      else
         $direccion_RL=$direccion;	 

      $LR_dic=$LRlem;	
      $LR_bil=$LRlem;	
      $LRlm=str_replace("<b/>"," ",$LRlem);      
      $RLlm=str_replace("<b/>"," ",$RLlem);   
      $raiz_vb=$raiz_LR;
	
      for($i=0; $i<$array_paradigma_LR[1]; $i++) 
      { // Recorremos todos los paradigmas de esta palabra para ver si tiene más de un paradima asociado
      
//ARANES!!: calculamos la raiz de la nueva palabra, lo hacemos a partir del siguiente paradigma asociado
        $token=strtok($array_paradigma_LR[$i+2],"/");            
        $token=strtok("/");
        $token=substr($token,0,strpos($token,"__") );        // agafem la terminació de l'infinitiu (està entre "/" i"__" )
        $raiz_LR=substr($LRlem, 0, strlen($LRlem)- strlen($token) );   //calculem l'arrel

//MILLORARRRRRRRRRRRR
        if( $tipo == "vb" && ($LR == "oc" || $LR == "fr") && $parametritzat == 1) {
          $raiz_LR=substr($LRlem, 0, strlen($LRlem)-(1 + $long_fixa_lema + strlen($infinitiu) ));

        
//          $raiz_LR=substr($LRlem, 0, strlen($LRlem)-(1 + $long_fixa_par + strlen($infinitiu) ));
        }
//Açó de calcular l'arrel s'ha de fer millloooooorrr
//FI ARANES!!
      
      
	 if($i==1 && $tipo=="vb" && $LR=="es")
	 {
	 	$pos=strpos($paradigmaLR,"/");  //se calcula cual es la primera letra a partir del corte en el paradigma
	 	$primera_letra=$paradigmaLR[$pos+1];
		$raiz_LR=acentuar($raiz_LR,$primera_letra); //se obtiene la raiz del verbo acentuada en es
	 }

        $array_paradigma_LR[$i+2]=str_replace ("\n","", $array_paradigma_LR[$i+2]);
	 
	// $pos=strpos($paradigmaLR,"/");
	$pos=strpos($array_paradigma_LR[$i+2],"/");
	
   	 if ($pos===0){
                  $raiz_LR="";
              }

//ARANES!!!
         if( $tipo == "vb" && ($LR == "oc" || $LR == "fr") && $parametritzat == 1){
             $par_parametre=" prm=\"".$fixa."\"";    
         }
         else{
             $par_parametre="";        
         }
         
//FI ARANES!!!
	    
	if($tipo=="vb" && $raiz_LR!=$raiz_vb)
        {
             if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	     }	
	     else if($direccion==":<:")
	       {	
		 $printdir="RL";
                 $cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       }
	      
	       else
	       {
                  $cad_LR.="<e lm=\"$LRlm\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";	       
	       } 
	 

        }
        else  if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	     }	
	     else if($direccion==":<:")
	       {	
		 $printdir="RL";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       }
	      
	       else
	       {
	       
	         $cad_LR.="<e lm=\"$LRlm\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       } 
	    
	 		
	
	  $cad_LR=str_replace ("<i></i>\n","", $cad_LR);		
	  
      }
      
      
      if($paradigmaLR=="Altrecas" && ($tipo=="n" || $tipo=="adj" || $tipo=="acr"))
      {
        
             if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$LR_dic$tag<s n=\"$geneLR\"/><s n=\"$numLR\"/></r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		$printdir="RL";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$LR_dic$tag<s n=\"$geneLR\"/><s n=\"$numLR\"/></r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	       
	        $cad_LR.="<e lm=\"$LRlm\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$LR_dic$tag<s n=\"$geneLR\"/><s n=\"$numLR\"/></r>\n  </p>\n</e>\n";
	       } 
	    
	     }
	
    
      }else if ($paradigmaLR=="Altrecas" )
          
     	
             if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$LR_dic$tag</r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		$printdir="RL";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$LR_dic$tag</r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	        $cad_LR.="<e lm=\"$LRlm\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$LR_dic$tag</r>\n  </p>\n</e>\n";
	       } 
	    
	     }
	     
         
    
    
       //Montamos la cadena a insertar en el diccionario RL
      $cadparadigma=shell_exec("grep ^$paradigmaRL $path_paradigmasRL");
      //La cadena devuelta por el grep la metemos en un array separando las palabras
      $array_paradigma_RL=explode("\t",$cadparadigma);
      $cad_RL="";


      
      if($tipo=="vb" ) //si es verbo se pone solo la raiz a la drcha
      {
	  	$RL_dic=$raiz_RL;
		if($tipo=="vb")
			$RL_bil=$RLlem;
		else
			$RL_bil=$raiz_RL;
      }
      else
      {
		$RL_dic=$RLlem;	
		$RL_bil=$RLlem;
      }
		
      
      for($i=0; $i<$array_paradigma_RL[1]; $i++) { // Recorremos todos los paradigmas de esta palabra
       
 //afegit per Gema per a acccentuar els verbs en es del fr-es
      
	 if($i==1 && $tipo=="vb" && $RL=="es")
	 {
	 	$pos=strpos($paradigmaRL,"/");  //se calcula cual es la primera letra a partir del corte en el paradigma
	 	$primera_letra=$paradigmaRL[$pos+1];
		$raiz_RL=acentuar($raiz_RL,$primera_letra); //se obtiene la raiz del verbo acentuada en es
	 }

        $array_paradigma_RL[$i+2]=str_replace ("\n","", $array_paradigma_RL[$i+2]);
	 
	// $pos=strpos($paradigmaRL,"/");
	$pos=strpos($array_paradigma_RL[$i+2],"/");
	
   	 if ($pos===0){
                  $raiz_RL="";
              }
	    
	if($tipo=="vb" && $raiz_RL!=$raiz_vb2)
        {
             if($direccion==":>:")
	     {
	        $printdir="RL";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <p>\n    <l>$raiz_RL</l>\n    <r>$raiz_vb2</r>\n  </p>\n<par n=\"".$array_paradigma_RL[$i+2]."\"".$par_parametre."/>\n</e>\n";
	     }	
	     else if($direccion==":<:")
	       {	
		 $printdir="LR";
                 $cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <p>\n    <l>$raiz_RL</l>\n    <r>$raiz_vb2</r>\n  </p>\n<par n=\"".$array_paradigma_RL[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       }
	      
	       else
	       {
                  $cad_RL.="<e lm=\"$RLlm\">\n  <p>\n    <l>$raiz_RL</l>\n    <r>$raiz_vb2</r>\n  </p>\n<par n=\"".$array_paradigma_RL[$i+2]."\"".$par_parametre."/>\n</e>\n";	       
	       } 
	 

        }
        else 
//fi afegit per Gema      
             if($direccion==":>:")
	     {
	        $printdir="RL";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		 $printdir="LR";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n</e>\n";
	       }
	      
	       else
	       {
	        $cad_RL.="<e lm=\"$RLlm\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n</e>\n";
	       } 
	    
	     }
	 	 

	 $cad_RL=str_replace ("<i></i>\n","", $cad_RL); 
      }
      if($paradigmaRL=="Altrecas" && ($tipo=="n" || $tipo=="adj" || $tipo=="acr"))
      	 if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <p>\n    <l>$RL_dic</l>\n    <r>$RL_dic$tag<s n=\"$geneRL\"/><s n=\"$numRL\"/></r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		$printdir="RL";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <p>\n    <l>$RL_dic</l>\n    <r>$RL_dic$tag<s n=\"$geneRL\"/><s n=\"$numRL\"/></r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	        $cad_RL.="<e lm=\"$RLlm\">\n  <p>\n    <l>$RL_dic</l>\n    <r>$RL_dic$tag<s n=\"$geneRL\"/><s n=\"$numRL\"/></r>\n  </p>\n</e>\n";
	       } 
	    
	     }
	     
	
      else if ($paradigmaLR=="Altrecas" )
      	  if($direccion_RL==":>:")
	     {
	        $printdir="LR";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <p>\n    <l>$RL_dic</l>\n    <r>$RL_dic$tag</r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion_RL==":<:")
	       {	
		$printdir="RL";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\">\n  <p>\n    <l>$RL_dic</l>\n    <r>$RL_dic$tag</r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	        $cad_RL.="<e lm=\"$RLlm\">\n  <p>\n    <l>$LR_dic</l>\n    <r>$RL_dic$tag</r>\n  </p>\n</e>\n";
	       } 
	    
	     }
	
	
       //Se genera la cadena bilingüe
        $tagLR=$tagRL=$tag;
	
	$cad_bil="";
	$direccionBil=$direccion;
	$muestra=1;
	$noND=1;
	
//      if($paradigmaLR=="Altrecas" || $paradigmaRL=="Altrecas")
/*       { 	
	      
      	      if($numLR=="sp" && $numRL!="sp" )  // si la entrada de castellano es <sp> y la de catala no
	      {
		        $tagLR.="<s n=\"".$genLR[0]."\"/><s n=\"sp\"/>";  
		  	$tagRL.="<s n=\"".$genRL[0]."\"/>";  
		
			
			if($direccion!=":>:")
			{
				if($GDRL==1)
				{
					for($ge=0;$ge<count($genRL);$ge++)
					{
						$tagRL=$tag."<s n=\"".$genRL[$ge]."\"/>";  
						$cad_bil.="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tagLR."</l>\n    <r>".$RL_bil.$tagRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
						$cad_bil.="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tagLR."</l>\n    <r>".$RL_bil.$tagRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
					}
					$GDRL=0;
					$tagGenRL="<s n=\"GD\">";
				}
				else
				{
				
					$cad_bil.="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tagLR."</l>\n    <r>".$RL_bil.$tagRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
	
					$cad_bil.="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tagLR."</l>\n    <r>".$RL_bil.$tagRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
				
					$tagGenRL="<s n=\"".$genRL[0]."\"/>";
				}
				 
				if($direccion==":<:") $muestra=0;   //no hay que poner la entrada genérica 
			}
	       		$direccionBil=":>:";
			$tagNumRL.="<s n=\"ND\"/>";
			$tagNumLR.="<s n=\"sp\"/>";
			
			$tagGenLR="<s n=\"".$genLR[0]."\">";	
			$noND=0;   		
	      }	
	      
	      if($numLR!="sp" && $numRL=="sp" )  // si la entrada de catala es <sp> y la de castellano no
	      {
		       
			$tagLR.="<s n=\"".$genLR[0]."\"/>";  
		  	$tagRL.="<s n=\"".$genRL[0]."\"/><s n=\"sp\"/>";  
			
			if($direccion!=":<:")
			{
				if($GDLR==1)
				{
					for($ge=0;$ge<count($genLR);$ge++)
					{
						$tagLR=$tag."<s n=\"".$genLR[$ge]."\"/>";
						$cad_bil.="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tagLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tagRL."</r>\n  </p>\n</e>\n";
						
					}	$cad_bil.="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tagLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tagRL."</r>\n  </p>\n</e>\n";
						
					$GDLR=0;
					$tagGenLR="<s n=\"GD\"/>";
				
				}
				else
				{
					$cad_bil.="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tagLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tagRL."</r>\n  </p>\n</e>\n";
						
					$cad_bil.="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tagLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tagRL."</r>\n  </p>\n</e>\n";
						
					$tagGenLR="<s n=\"".$genLR[0]."\"/>";
				}
				 
				if($direccion==":>:") $muestra=0;   //no hay que poner la entrada genérica 
			}
       			$direccionBil=":<:";
			$tagNumLR="<s n=\"ND\"/>";
			$tagNumRL="<s n=\"sp\"/>";
			
			$tagGenRL="<s n=\"".$genRL[0]."\"/>";	
			$noND=0;   		
	      }	
	      
	}
*/   	


if($genLR[0]==$genRL[0]){      
      $tagGenLR="<s n=\"".$genLR[0]."\"/>";	
      $tagGenRL="<s n=\"".$genRL[0]."\"/>";	
      $tagNumLR="<s n=\"".$numLR."\"/>";
      $tagNumRL="<s n=\"".$numRL."\"/>";


       if($numLR==$numRL){ 
           $muestra=1;
           $tagNumLR="";
           $tagNumRL="";
           $tagGenLR="";
           $tagGenRL="";
       }
       else{
           switch($numLR){
               case "sg":
                   $tagNumLR="<s n=\"sg\"/>";
                   switch($numRL){
                          case "pl":
                              $tagGenLR="";
                              $tagGenRL="";
                              $muestra=1;
                              break;
                          case "sg_pl":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                              } 
                              $muestra=0;
                              break;
                          case "sp":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }
                              $muestra=0;
                              break;
                  }                                     
                  break;            
              case "pl":
                   $tagNumLR="<s n=\"pl\"/>";
                   switch($numRL){
                          case "sg":
                              $tagGenLR="";
                              $tagGenRL="";
                              $muestra=1;
                              break;
                          case "sg_pl":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                              } 
                              $muestra=0;
                              break;
                          case "sp":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }
                              $muestra=0;
                              break;
                  }                                     
                  break;            



              case "sg_pl":
                   $tagNumLR="<s n=\"sg_pl\"/>";
                   switch($numRL){
                          case "sg":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              } 
                              $muestra=0;
                              break;

                          case "pl":

                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";

                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              } 
                              $muestra=0;
                              break;

                          case "sp":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"ND\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"ND\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                                $printdir="LR";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sg\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"pl\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sp\"/></r>\n  </p>\n</e>\n";
                              } 
                              $muestra=0;
                              break;
                  }                                     
                  break;            


              case "sp":
                   $tagNumLR="<s n=\"sp\"/>";
                   switch($numRL){
                          case "sg":
                          case "pl":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
                              }
                              $muestra=0;
                              break;
                          case "sg_pl":
                              if($direccion==":>:"){
                                $printdir="LR";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"ND\"/></r>\n  </p>\n</e>\n";
                              }else if($direccion==":<:"){
                                $printdir="RL";
                                $cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                              }else{   
                                $cad_bil="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"ND\"/></r>\n  </p>\n</e>\n";
                                $printdir="RL";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"sg\"/></r>\n  </p>\n</e>\n";
                                $cad_bil=$cad_bil."<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR."<s n=\"sp\"/></l>\n    <r>".$RL_bil.$tag.$tagGenRL."<s n=\"pl\"/></r>\n  </p>\n</e>\n";
                              } 

                              $muestra=0;
                              break;
                  }                                     
                  break;            


          }
       

       }


}
else{
/**** aço potser sobra **/
//       $muestra=0;

       if($cambGen==1 && $noND==1)
       {
       		
       		$tagGenLR.="<s n=\"".$genLR[0]."\"/>";
       		$tagGenRL.="<s n=\"".$genRL[0]."\"/>";		
       }
  
  
       
    if($GDRL==1)
      
       {
       	
		$tagLR.="<s n=\"".$genLR[0]."\"/>";  // mf
		
			
		if($direccion!=":>:")
		{
			
			for($gene=0;$gene<count($genRL);$gene++)
			  $cad_bil.="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tagLR."</l>\n    <r>".$RL_bil.$tagRL."<s n=\"".$genRL[$gene]."\"/></r>\n  </p>\n</e>\n";
			
			if($direccion==":<:")   //no hay que poner la entrada genérica 
		        {
			    $muestra=0;
			}
			else
			{
	        	    $cad_bil="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tagLR.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tagRL."<s n=\"GD\"/>".$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n".$cad_bil;
 
			    $muestra=0;
			}
		}
       		$direccionBil=":>:";
		$tagGenRL.="<s n=\"GD\"/>";	
		$tagGenLR.="<s n=\"".$genLR[0]."/>";	
		
	}
       
         
       if($GDLR==1)
       
       {
       
       
       		$tagRL.="<s n=\"".$genRL[0]."\"/>";  // mf
		
		if($direccion!=":<:")
		{
			
			for($gene=0;$gene<count($genLR);$gene++)
			  
			  $cad_bil.="<e r=\"LR\">\n  <p>\n    <l>".$LR_bil.$tagLR."<s n=\"".$genLR[$gene]."\"/></l>\n    <r>".$RL_bil.$tagRL."</r>\n  </p>\n</e>\n";
			  
	 
			if($direccion==":>:") 
			{
			    $muestra=0;
			}
			else
			{   //no hay que poner la entrada genérica  
			  $cad_bil="<e r=\"RL\">\n  <p>\n    <l>".$LR_bil.$tagLR."<s n=\"GD\"/>".$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tagRL.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n".$cad_bil;
 
			    $muestra=0;
			}
			
		} 
		
		$direccion=":<:";
		$tagGenLR.="<s n=\"GD\"/>";
		$tagGenRL.="<s n=\"".$genRL[0]."\"/>";	
	
       }
/**** aço potser sobra **/
}       	
	
	
		
      if($muestra==1)	  
      {
     
       	     if($direccion==":>:")
	
	     {
	        $printdir="LR";
		$cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
	
		$printdir="RL";
		$cad_bil="<e r=\"".$printdir."\">\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	       
	         $cad_bil="<e>\n  <p>\n    <l>".$LR_bil.$tag.$tagGenLR.$tagNumLR."</l>\n    <r>".$RL_bil.$tag.$tagGenRL.$tagNumRL."</r>\n  </p>\n</e>\n";
	       } 
	    
	     }	   
  
      }
      
      //Si la traducción es unidireccional y ya existe el lema entonces no se inserta nada en el dic.
      if($direccion==":>:" && strlen($warning)>0)
      {
      		$warning="It is not necessary to put it in the dictionary.<br/>";
		$cad_RL="";
      }
      if($direccion==":<:" && strlen($warning)>0)
      {
      		$warning="It is not necessary to put it in the dictionary.<br/>";
		$cad_LR="";
      }
      
  
      //se muestra un desplegable para elegir la palabra a insertar
      if($valida==1)  //Validación de los datos. Muestra un desplegable con todos las palabras introducidas por los usuarios
      {
//	      echo "<br/><label style= \"margin-left: 6em\">Select a word to be validated: ";      	
	      echo "<br/><label style= \"margin-left: 6em\">";      	
   	      echo "<input type=\"hidden\" name=\"valida\" value=\"1\">\n";
	      echo "<input type=\"hidden\" name=\"selecc\" value=\"$selecc\">\n";
	      //se muestra un campo de seleccion para elegir la palabra a validar
//	      echo  "<select name=\"LRlem\" onchange=\"document.forms[0].submit()\">";
// 	      for($cont=0;$cont<count($LRlemV);$cont++)
//		{
//			if($LRlemV[$cont]==$LRlem)
//			$selected="selected";
//			else
//			$selected="";
			
//			echo  "    <option value=\"".$LRlemV[$cont]."\" $selected>".$LRlemV[$cont]."</option>\n";
//		}
//		echo "</select>\n";
		echo "</label>\n";
        }
        //---- no llevar
    

      if($comentarios != "")
      {
        $cad_LR = ereg_replace("(lm=\"[^\"]+\")", "\\1 c=\"$comentarios\"", $cad_LR);
        $cad_bil = ereg_replace("<e", "<e c=\"$comentarios\"", $cad_bil);
        $cad_RL = ereg_replace("(lm=\"[^\"]+\")", "\\1 c=\"$comentarios\"", $cad_RL);
      }        
      
      if($user != "")
      {
        $cad_LR = ereg_replace("(lm=\"[^\"]+\")", "\\1 a=\"$user\"", $cad_LR);
        $cad_bil = ereg_replace("<e", "<e a=\"$user\"", $cad_bil);
        $cad_RL = ereg_replace("(lm=\"[^\"]+\")", "\\1 a=\"$user\"", $cad_RL);
      }      

      echo "<br/><br/>\n";
      echo "<div class=\"area\">\n";
      echo "<p style=\"font-weight: bold\">Dictionary entries (".$LR."): ".$ejemplosLR."</p>\n";      
      echo "<textarea name=\"cad_LR\"  cols=\"70\" rows=\"5\">".$cad_LR."</textarea>\n";  
      echo "</div>\n";
      echo "<div class=\"area\">\n";
      echo "<p style=\"font-weight: bold\">Dictionary entries (".$LR."-".$RL.")</p>\n";
      echo "<textarea name=\"cad_bil\" cols=\"70\" rows=\"5\">".$cad_bil."</textarea>\n";
      echo "</div>\n";     
      echo "<div class=\"area\">\n";
      echo "<p style=\"font-weight: bold\">Dictionary entries (".$RL."): ".$ejemplosRL."</p>\n";      
      echo "<textarea name=\"cad_RL\"  cols=\"70\" rows=\"5\">".$cad_RL."</textarea>\n"; 
      echo "</div>\n";

      
      
        if (strlen($warning)>0) {
           echo "<br/><b class=\"adv\">ADVERTIMENT: </b><br/>\n".$warning;
        } 
     
 
      echo "<input type=\"hidden\" name=\"valida\"value=\"$valida\" >\n";
      echo "<input type=\"hidden\" name=\"LRlem\"value=\"$LRlem\" >\n";
      echo "<input type=\"hidden\" name=\"RLlem\"value=\"$RLlem\" >\n";
      echo "<input type=\"hidden\" name=\"LR\"value=\"$LR\" >\n";
      echo "<input type=\"hidden\" name=\"RL\"value=\"$RL\" >\n";
      echo "<input type=\"hidden\" name=\"direccion\" value=\"$direccion\"/>\n";
      echo "<input type=\"hidden\" name=\"mostrar\" value=\"$show\"/>\n";
    
      echo "<div class=\"area\">\n";      
      echo "<label><input class=\"button\" type=\"button\" value=\"<< Go back\" onclick=\"history.go(-1)\"></label>\n";
     
        if ($valida==1){  //si se esta validando se pone un botón para borrar la palabra actual
      		echo "<label><input class=\"submit\" type=\"submit\" name=\"accion\" value=\"Delete\" /></label>\n";
        }
   
      echo "<label><input class=\"submit\" type=\"submit\"  name=\"accion\" value=\"Insert >>\"></label>\n";
      echo "</div>\n"; 
      echo "</form>\n";
      echo "<br/><br/>\n";
   
      echo "<!-- ##### Footer ##### -->\n"; 

      echo "<div id=\"footer\">\n"; 
  
      echo "<div><a href=\"http://www.dlsi.ua.es\">\n"; 
      echo "Departament de Llenguatges i Sistemes Informàtics</a>, Universitat\n"; 
      echo "d'Alacant, 2006 | Last modified 20.02.2006<!-- by <a\n"; 
      echo "href=\"mailto:sortiz@dlsi.ua.es\" title=\"Envíame un correo\">Sergio\n"; 
      echo "Ortiz</a> -->\n"; 
      echo "</div>\n"; 
      
      echo "</div>\n"; 

      echo "</body>\n";
      echo "</html>\n";
      
      //Esperamos confirmación del usuario para la inserción.
   }
 else if ($accion=="Insert >>") 
  
   {
   
   
 echo "<?xml version=\"1.0\" encoding=\"ISO-8859-15\"?>\n";
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="es">
  <head>
    <meta http-equiv="content-type" content="application/xhtml+xml; charset=ISO-8859-15" />
    <meta name="author" content="haran" />
    <meta name="generator" content="haran" />

    <!-- Navigational metadata for large websites (an accessibility feature): -->
    <link rel="top"      href="./index.html" title="Homepage" />
    <link rel="up"       href="./index.html" title="Up" />
    <link rel="first"    href="./index.html" title="First page" />
    <link rel="previous" href="./index.html" title="Previous page" />
    <link rel="next"     href="./index.html" title="Next page" />
    <link rel="last"     href="./index.html" title="Last page" />
    <link rel="toc"      href="./index.html" title="Table of contents" />
    <link rel="index"    href="./index.html" title="Site map" />

    <link rel="stylesheet" type="text/css" href="/styles/gila-screen-form2.css" media="screen" title="Gila (screen)" />
    <link rel="stylesheet" type="text/css" href="/styles/gila-print.css" media="print" />
	<title>OpenTrad apertium insertion form. Thank you!</title>

<script language="Javascript">
function cierra() {
   window.close();
}
</script>
</head>
<body>

<?php   
   
      $cad_LR=str_replace("\\'","'",$cad_LR);  
      $cad_bil=str_replace("\\'","'",$cad_bil);      
      $cad_RL=str_replace("\\'","'",$cad_RL); 
   
      $error=0;
      
       system("echo -n \"$cad_LR\" >>$fich_LR", $retval);      
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!</h2><br><br>
                <h2>L'entrada no ha pogut ser inserida en el fitxer '$fich_LR'.</h2><br>
	        <h3>Probablement es tracti d'un problema de permisos.</h3><br><br>
                </center>");     
	$error=1; 	     
      }	

      system("echo -n \"$cad_bil\" >>$fich_LRRL", $retval);
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!</h2><br><br>
                <h2This entry has not been inserted in '$fich_LRRL'.</h2><br>
	        <h3>It is probably due to a permission problem.</h3><br><br>
                </center>");      	     
		$error=1;
      }
      system("grep $cad_bil $fich_LRRL >/dev/null", $retval);
      if ($retval==0)
      {
	 print("<center><h2>UNKNOWN ERROR</h2><br><br>
         <h2>It is probably due to a permission problem. '$fich_LRRL'.</h2><br>
	 <br><br>
         </center>");
 	$error=1;
       }
         
      system("echo -n \"$cad_RL\" >>$fich_RL", $retval); 
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!</h2><br><br>
                <h2This entry has not been inserted in '$fich_RL'.</h2><br>
	        <h3>It is probably due to a permission problem.</h3><br><br>
                </center>");      	     
		$error=1;		
      }	
            
      if($error==0)
      {
      		
      		$tmpfname = tempnam("/tmp", "semi");
      		if(file_exists($tmpfname))
		{
			
			copy($fich_semi,$tmpfname);
                        $LRlm=str_replace("<b/>"," ",$LRlem);      
                        $RLlm=str_replace("<b/>"," ",$RLlem);   
			$creasemi="awk 'BEGIN{FS=\"\\t\";} $1!=\"$LRlm\" && $2!=\"$RLlm\"{print $0}' $tmpfname > $fich_semi";	
			$salida=shell_exec($creasemi);			
			
		}
		
	}			     
	
	if($valida==1)
	{
		$oncl="document.location.href='./valida.php?RL=$RL&LR=$LR'";
		$mensaje="Continue";
	}
	else
	{
		$oncl="cierra()";
		$mensaje="Close";
		echo "<div class=\"area\">\n"; 
		echo "<br/>\n"; 
		echo "<a style=\"text-align: center\" href=\"/prototype/webform\" >New word</a>\n"; 
		echo "<br/>\n"; 
		echo "</div>\n";
	}
	
	echo "<div class=\"area\">\n";
        echo "<h3>Thanks and see you soon!</h3>\n";      
        echo "<input type=\"button\" value=\"$mensaje\" onclick=\"$oncl\"><br/><br/>\n";
	echo "</div>\n";
      
   
        echo "<!-- ##### Footer ##### -->\n"; 

        echo "<div id=\"footer\">\n"; 
  
        echo "<div><a href=\"http://www.dlsi.ua.es\">\n"; 
        echo "Departament de Llenguatges i Sistemes Informàtics</a>, Universitat\n"; 
        echo " d'Alacant, 2006 | Last modified 20.02.2006<!-- by <a\n"; 
        echo "href=\"mailto:sortiz@dlsi.ua.es\" title=\"Envíame un correo\">Sergio\n"; 
        echo "Ortiz</a> -->\n"; 
        echo "</div>\n"; 
      
       echo "</div>\n"; 
   
      
      
      echo "</body>\n";
      echo "</html>\n";


  } 
   else if($accion=="Delete")
      {
   		
		$tmpfname = tempnam("/tmp", "semi");
      		if(file_exists($tmpfname))
		{
			
			copy($fich_semi,$tmpfname);	
                        $LRlm=str_replace("<b/>"," ",$LRlem);      
                        $RLlm=str_replace("<b/>"," ",$RLlem);   
			$creasemi="awk 'BEGIN{FS=\"\\t\";} $1!=\"$LRlm\" && $2!=\"$RLlm\"{print $0}' $tmpfname > $fich_semi";	
			$salida=shell_exec($creasemi);	
		} 


	
		$url = "./valida.php?RL=$RL&LR=$LR";
		header("Location: $url");

		
    }
   
   
   
?>
