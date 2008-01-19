<?php
    include("config.php"); //archivo de configuración

 if($tipo=="mw")
    {
	$cadFich="verb";
	$show=": multiwords";
	$tag="<s n=\"vblex\"/>";
	$tagout="<vblex>";
	$nota="";
    }
	

	//Rutas de los diccionarios temporales
	$path="../dics";
	
	
	$fich_LR="$path/apertium-$LR-$RL.$LR.webform";
	$fich_RL="$path/apertium-$LR-$RL.$RL.webform";
    	$fich_LRRL="$path/apertium-$LR-$RL.$LR-$RL.webform";
	$fich_semi="$path/$LR-$RL.semi.dic";

	
	$path_paradigmasLR="./paradigmas.$LR-$RL/paradig".$cadFich."_".$LR;
    	$path_paradigmasRL="./paradigmas.$LR-$RL/paradig".$cadFich."_".$RL;
	

	//Rutas del analizador morfológico y los dic. compilados
    	$aut_LRRL=$dicos_path."/apertium-$nomtrad/$LR-$RL.automorf.bin";
    	$aut_RLLR=$dicos_path."/apertium-$nomtrad/$RL-$LR.automorf.bin";
      
        $LRcua=str_replace(" ","<b/>",$LRcua);      
        $RLcua=str_replace(" ","<b/>",$RLcua);   


  	

//Funciones para acentuar en caso de q la palabra sea un verbo
    
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
      
      



   if(!isset($accion)) {
   

      echo "<form action=\"ins_multip.php\">";
      echo "<input type=\"hidden\" name=\"accion\" value=\"Insert >>\">";

     
      
      //Debemos calcular la raiz de la nueva palabra, lo hacemos a partir del paradigma
      $token=strtok($paradigmaLR,"/");
      $raiz_LR=substr($LRverb, 0, strlen($LRverb)-(strlen($paradigmaLR)-strlen($token)-1));
	
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

                      $long_fixa_lema= (strlen($LRverb)-strlen($infinitiu)-1) - PosicioVocalTall($LRverb,$vocal,$infinitiu);

                      $raiz_LR=substr($LRverb, 0, strlen($LRverb)-(1 + $long_fixa_lema + strlen($infinitiu) ));
                      //CALCULEM l'ARREL del lema a introduir (el que hi haurà davant de \ )
                      //pendent de millorar
                      $fixa=substr($LRverb,strlen($raiz_LR)+1,$long_fixa_lema);
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
      
     
     //Debemos calcular la raiz de la nueva palabra, lo hacemos a partir del paradigma
      $token=strtok($paradigmaRL,"/");
      $raiz_RL=substr($RLverb, 0, strlen($RLverb)-(strlen($paradigmaRL)-strlen($token)-1));
      
      //genera los ejemplos
      $ejemplosRL="";
      if(sizeof($vectParRL)>0)
      {
      	for($cont=0; $cont < sizeof($vectParRL) ;$cont++)
		$ejemplosRL.="<br/>".$raiz_RL.$vectParRL[$cont];
      }
   
      //Montamos la cadena a insertar en el diccionario es
      $cadparadigma=shell_exec("grep ^$paradigmaLR $path_paradigmasLR");
      //La cadena devuelta por el grep la metemos en un array separando las palabras
      $array_paradigma_LR=explode("\t",$cadparadigma);
      $cad_LR="";
      //Calculamos la dirección para es
      $direccion_LR=$direccion;
      
      //Calculamos direccion ca      
      if ($direccion==":<:")
         $direccion_RL=":>:";
      else if ($direccion==":>:")
         $direccion_RL=":<:";	 
      else
         $direccion_RL=$direccion;	 

      $LRlm=str_replace("<b/>"," ",$LRlem);      
      $RLlm=str_replace("<b/>"," ",$RLlem);   

      $raiz_vb=$raiz_LR;	
      for($i=0; $i<$array_paradigma_LR[1]; $i++) 
      { // Recorremos todos los paradigmas de esta palabra

//ARANES!!: calculamos la raiz de la nueva palabra, lo hacemos a partir del siguiente paradigma asociado
        $token=strtok($array_paradigma_LR[$i+2],"/");            
        $token=strtok("/");
        $token=substr($token,0,strpos($token,"__") );        // agafem la terminació de l'infinitiu (està entre "/" i"__" )
        $raiz_LR=substr($LRverb, 0, strlen($LRverb)- strlen($token) );   //calculem l'arrel

//MILLORARRRRRRRRRRRR
        if( $tipo == "mw" && ($LR == "oc" || $LR == "fr") && $parametritzat == 1) {
          $raiz_LR=substr($LRverb, 0, strlen($LRverb)-(1 + $long_fixa_lema + strlen($infinitiu) ));

        
//          $raiz_LR=substr($LRverb, 0, strlen($LRverb)-(1 + $long_fixa_par + strlen($infinitiu) ));
        }
//Açó de calcular l'arrel s'ha de fer millloooooorrr
//FI ARANES!!




	 if($i==1 && $tipo=="mw")
	 {
	 	$pos=strpos($paradigmaLR,"/");  //se calcula cual es la primera letra a partir del corte en el paradigma
	 	$primera_letra=$paradigmaLR[$pos+1];
		$raiz_LR=acentuar($raiz_LR,$primera_letra); //se obtiene la raiz del verbo acentuada
	
	 }
	 
	      if (strpos($array_paradigma_LR[$i+2],"/")==0)
	      {
                  $raiz_LR="";
              }
	 
	 $array_paradigma_LR[$i+2]=str_replace ("\n","", $array_paradigma_LR[$i+2]);


//ARANES!!!
         if( $tipo == "mw" && ($LR == "oc" || $LR == "fr") && $parametritzat == 1){
               $par_parametre=" p1=\"".$fixa."\"";
         }
         else{
               $par_parametre="";
         }
//FI ARANES!!!
                                                              

	 
	if($LRcua!="")
	{
	
          if ($raiz_LR!=$raiz_vb)
          {
              if($direccion==":>:")
               {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n  <p>\n    <l>$LRcua</l>\n    <r><g>$LRcua</g></r>\n  </p>\n</e>\n";
	       }	
	       else
	       {
	         if($direccion==":<:")
	         {	
		  $printdir="RL";
		  $cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n  <p>\n    <l>$LRcua</l>\n    <r><g>$LRcua</g></r>\n  </p>\n</e>\n";
	         }
	         else
	         {
	          $cad_LR.="<e lm=\"$LRlm\" a=\"$user\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n  <p>\n    <l>$LRcua</l>\n    <r><g>$LRcua</g></r>\n  </p>\n</e>\n";
	         } 
	       }
          }
          else
          {
	     if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n  <p>\n    <l>$LRcua</l>\n    <r><g>$LRcua</g></r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		 $printdir="RL";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n  <p>\n    <l>$LRcua</l>\n    <r><g>$LRcua</g></r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	       
	         $cad_LR.="<e lm=\"$LRlm\" a=\"$user\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n  <p>\n    <l>$LRcua</l>\n    <r><g>$LRcua</g></r>\n  </p>\n</e>\n";
	       } 
	    } 
          }		
	}
	else	
	{
            
          if ($raiz_LR!=$raiz_vb)
          {
             if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		 $printdir="RL";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       }
	      
	       else
	       {
	       
	         $cad_LR.="<e lm=\"$LRlm\" a=\"$user\">\n  <p>\n    <l>$raiz_LR</l>\n    <r>$raiz_vb</r>\n  </p>\n<par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       } 
	    
	     } 
          }
          else
          {	
	    if($direccion==":>:")
	     {
	        $printdir="LR";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		 $printdir="RL";
		$cad_LR.="<e lm=\"$LRlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       }
	      
	       else
	       {
	       
	         $cad_LR.="<e lm=\"$LRlm\" a=\"$user\">\n  <i>$raiz_LR</i>\n  <par n=\"".$array_paradigma_LR[$i+2]."\"".$par_parametre."/>\n</e>\n";
	       } 
	    }
	  }	
	 
	} 	 
      $cad_LR=str_replace ("<i></i>\n","", $cad_LR);
      
      }	
       
       //Montamos la cadena a insertar en el diccionario ca
      $cadparadigma=shell_exec("grep ^$paradigmaRL $path_paradigmasRL");
      //La cadena devuelta por el grep la metemos en un array separando las palabras
      $array_paradigma_RL=explode("\t",$cadparadigma);
      $cad_RL="";
      
      for($i=0; $i<$array_paradigma_RL[1]; $i++) { // Recorremos todos los paradigmas de esta palabra
//afegit per Gema per a acccentuar els verbs en es del fr-es      
	 if($i==1 && $tipo=="mw" && $RL=="es")
	 {
	 	$pos=strpos($paradigmaRL,"/");  //se calcula cual es la primera letra a partir del corte en el paradigma
	 	$primera_letra=$paradigmaRL[$pos+1];
		$raiz_RL=acentuar($raiz_RL,$primera_letra); //se obtiene la raiz del verbo acentuada en es
	 }
//fi afegit per Gema    
          $array_paradigma_RL[$i+2]=str_replace ("\n","", $array_paradigma_RL[$i+2]);

	      if (strpos($array_paradigma_RL[$i+2],"/")==0){
                  $raiz_RL="";
              }
	      
	  if($RLcua!=""){
	  
	     if($direccion==":>:")
	     {
	        $printdir="RL";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n  <p>\n    <l>$RLcua</l>\n    <r><g>$RLcua</g></r>\n  </p>\n</e>\n";
	     }	
	     else
	     {
	       if($direccion==":<:")
	       {	
		 $printdir="LR";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n  <p>\n    <l>$RLcua</l>\n    <r><g>$RLcua</g></r>\n  </p>\n</e>\n";
	       }
	      
	       else
	       {
	        $cad_RL.="<e lm=\"$RLlm\" a=\"$user\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n  <p>\n    <l>$RLcua</l>\n    <r><g>$RLcua</g></r>\n  </p>\n</e>\n";
	       }
	     }
	   }
	   else
	   {
	      if($direccion==":>:")
	      {
	        $printdir="RL";
		$cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n</e>\n";
	      }	
	      else
	      {
	        if($direccion==":<:")
	        {	
		  $printdir="LR";
	 	  $cad_RL.="<e lm=\"$RLlm\" r=\"$printdir\" a=\"$user\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n</e>\n";
	        }
	      
	        else
	        {
	          $cad_RL.="<e lm=\"$RLlm\" a=\"$user\">\n  <i>$raiz_RL</i>\n  <par n=\"".$array_paradigma_RL[$i+2]."\"/>\n</e>\n";
	        }
	      }
	    } 
	    
	    
	     $cad_RL=str_replace ("<i></i>\n","", $cad_RL);
	} 	     
    
     //BILINGUE////////////////////////////////////////////
     //Se calcula la cadena a insertar en el bilingue
     
     
  if($LRcua!="" && $RLcua!="")
  {
    if($direccion==":>:")
    {
	        $printdir="LR";
		$cad_bil="<e r=\"".$printdir."\" a=\"$user\">\n  <p>\n    <l>".$LRverb."<g>".$LRcua."</g>".$tag."</l>\n    <r>".$RLverb."<g>".$RLcua."</g>".$tag."</r>\n  </p>\n</e>\n";
    }	
    else
    {
       if($direccion==":<:")
       {	
	  $printdir="RL";
	  $cad_bil="<e r=\"".$printdir."\" a=\"$user\">\n  <p>\n    <l>".$LRverb."<g>".$LRcua."</g>".$tag."</l>\n    <r>".$RLverb."<g>".$RLcua."</g>".$tag."</r>\n  </p>\n</e>\n";
       }	
       else
       {
          $cad_bil="<e a=\"$user\">\n  <p>\n    <l>".$LRverb."<g>".$LRcua."</g>".$tag."</l>\n    <r>".$RLverb."<g>".$RLcua."</g>".$tag."</r>\n  </p>\n</e>\n";
       }	
    }	   

  } 
  else
  {
   
     if($LRcua=="" && $RLcua!="")
     {
     
     	if($direccion==":>:")
	{
	        $printdir="LR";
		$cad_bil="<e r=\"".$printdir."\" a=\"$user\">\n  <p>\n    <l>".$LRverb.$tag."</l>\n    <r>".$RLverb."<g>".$RLcua."</g>".$tag."</r>\n  </p>\n</e>\n";
	}	
	else
	{
	    
	    if($direccion==":<:")
	    {	
	
		$printdir="RL";
		$cad_bil="<e r=\"".$printdir."\" a=\"$user\">\n  <p>\n    <l>".$LRverb.$tag."</l>\n    <r>".$RLverb."<g>".$RLcua."</g>".$tag."</r>\n  </p>\n</e>\n";
	    }	
	    else
	    {
	       
	         $cad_bil="<e a=\"$user\">\n  <p>\n    <l>".$LRverb.$tag."</l>\n    <r>".$RLverb."<g>".$RLcua."</g>".$tag."</r>\n  </p>\n</e>\n";
	    }	
         }	   

     }
     else 
     {
     
       if($LRcua!="" && $RLcua=="")
       {
       
          if($direccion==":>:")
	  {
	        $printdir="LR";
		$cad_bil="<e r=\"".$printdir."\" a=\"$user\">\n  <p>\n    <l>".$LRverb."<g>".$LRcua."</g>".$tag."</l>\n    <r>".$RLverb.$tag."</r>\n  </p>\n</e>\n";
	  }	
	  else
	  {
	     if($direccion==":<:")
	     {	
		$printdir="RL";
		$cad_bil="<e r=\"".$printdir."\" a=\"$user\">\n  <p>\n    <l>".$LRverb."<g>".$LRcua."</g>".$tag."</l>\n    <r>".$RLverb.$tag."</r>\n  </p>\n</e>\n";
	     }	
             else
	     {  
	         $cad_bil="<e a=\"$user\">\n  <p>\n    <l>".$LRverb."<g>".$LRcua."</g>".$tag."</l>\n    <r>".$RLverb.$tag."</r>\n  </p>\n</e>\n";
	     }	
          }	    
        }  
      }
    }      
    
    
       //Si la traducción es unidireccional y ya existe el lema entonces no se inserta nada en el dic.
      if($direccion==":>:" && strlen($warning)>0)
      {
      		$warning_LR="It is not necessary to put it in the dictionary (".$LR.").<br/>";
		$cad_RL="";
      }
      if($direccion==":<:" && strlen($warning)>0)
      {
      		$warning_RL="It is not necessary to put it in the dictionary (".$RL.").<br/>";
		$cad_LR="";
      }
        
      
      echo "<br/><br/>\n";
      echo "<div class=\"area\">\n";
      echo "<p style=\"font-weight: bold\">Dictionary entries (".$LR.")</p>\n";      
      echo "<textarea name=\"cad_LR\"  cols=\"70\" rows=\"7\">".$comentarios.$cad_LR."</textarea>\n";  
      if (strlen($warning)>0) {
         echo "<br/><b class=\"adv\">ADVERTENCIA: </b><br/>".$warningLR;
      }
      echo "</div>\n";
      echo "<div class=\"area\">\n";
      echo "<p style=\"font-weight: bold\">Dictionary entries (".$LR."-".$RL.")</p>\n";
      echo "<textarea name=\"cad_bil\" cols=\"70\" rows=\"6\">".$comentarios.$cad_bil."</textarea>\n";
      echo "</div>\n";     
      echo "<div class=\"area\">\n";
      echo "<p style=\"font-weight: bold\">Dictionary entries (".$RL.")</p>\n";      
      echo "<textarea name=\"cad_RL\"  cols=\"70\" rows=\"7\">".$comentarios.$cad_RL."</textarea>\n"; 
      if (strlen($warning)>0) {
         echo "<br/><b class=\"adv\">ADVERTENCIA: </b><br/>".$warning_RL;
      } 
      echo "</div>\n";

      echo "<input type=\"hidden\" name=\"LR\"value=\"$LR\" >";
      echo "<input type=\"hidden\" name=\"RL\"value=\"$RL\" >";


      echo "<div class=\"area\">\n";      
      echo "<label><input class=\"button\" type=\"button\" value=\"<< Go back\" onclick=\"history.go(-1)\"></label>\n";
     
        if ($valida==1){  //si se esta validando se pone un botón para borrar la palabra actual
      		echo "<label><input class=\"submit\" type=\"submit\" name=\"accion\" value=\"Delete\" /></label>\n";
        }
   
      echo "<label><input class=\"submit\" type=\"submit\"  name=\"accion\" value=\"Insert >>\"></label>\n";
      echo "</div>\n"; 
      echo "</form>\n";

   
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
   else if ($accion=="Insert >>") {
   
   
      $cad_LR=str_replace("\\'","'",$cad_LR);  
      $cad_bil=str_replace("\\'","'",$cad_bil);      
      $cad_RL=str_replace("\\'","'",$cad_RL); 
   
      
      system("echo -n \"$cad_LR\" >>$fich_LR", $retval); 
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!</h2><br><br>
                <h2>La entrada no ha podido ser insertada en el fichero '$fich_LRRL'.</h2><br>
	        <h3>Probablemente se trate de un problema de permisos.</h3><br><br>
                </center>");      	     
      }	

      system("echo -n \"$cad_bil\" >>$fich_LRRL", $retval);
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!</h2><br><br>
                <h2>La entrada no ha podido ser insertada en el fichero '$fich_bil'.</h2><br>
	        <h3>Probablemente se trate de un problema de permisos.</h3><br><br>
                </center>");      	     
      }
      system("grep $cad_bil $fich_LRRL >/dev/null", $retval);
      if ($retval==0)
	 print("<center><h2>ERROR de tipo desconocido</h2><br><br>
         <h2>La entrada no ha podido ser insertada en el fichero '$fich_bil'.</h2><br>
	 <br><br>
         </center>");
	 
       
      system("echo -n \"$cad_RL\" >>$fich_RL", $retval); 
      if($retval!=0) { // Se produjo un error en la inserción
         print("<center><h2>ERROR!<br>
                <h2>La entrada no ha podido ser insertada en el fichero '$fich_RLLR'.</h2><br>
	        <h3>Probablemente se trate de un problema de permisos.</h3><br><br>
                </center>");  	    	     
      }	


	echo "<div class=\"area\">\n";
	echo "<br/><a style=\"text-align: center\" href=\"/prototype/webform\" >New word</a><br/>\n"; 
        echo "<h3>Thanks and see you soon!</h3>\n";      
        echo "<input type=\"button\" value=\"Close\" onclick=\"cierra()\"><br/><br/>\n";
	echo "</div>\n";
      
  	echo "</form>\n"; 
	
	
        echo "<!-- ##### Footer ##### -->\n"; 

        echo "<div id=\"footer\">\n"; 
  
        echo "<div><a href=\"http://www.dlsi.ua.es\">\n"; 
        echo "Departament de Llenguatges i Sistemes Informàtics</a>, Universitat\n"; 
        echo " d'Alacant, 2005 | Last modified 30.09.2005.<!-- by <a\n"; 
        echo "href=\"mailto:sortiz@dlsi.ua.es\" title=\"Envíame un correo\">Sergio\n"; 
        echo "Ortiz</a> -->\n"; 
        echo "</div>\n"; 
      
       echo "</div>\n"; 

   }	            
?>
</body>
</html>
