<?
    include("config.php"); //archivo de configuración

	//Set data needed 
    if($tipo=="n")
    {
	$cadFich="noun";
	$show=": nouns";
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
	
	
	

	//Path for temporary dictionnaries:
	$path="../dics";
	
	
	//professional dictionaries
	$fich_LR="$path/apertium-$nomtrad.$LR.webform";
	$fich_RL="$path/apertium-$nomtrad.$RL.webform";
    	$fich_LRRL="$path/apertium-$nomtrad.$LR-$RL.webform";
	//and semi-professional
	$fich_semi="$path/$LR-$RL.semi.dic";

	
	
	//Path for paradigms
	
	$path_paradigmasLR="./paradigmas.$LR-$RL/paradig".$cadFich."_".$LR;
    	$path_paradigmasRL="./paradigmas.$LR-$RL/paradig".$cadFich."_".$RL;
	

	//Path for the morphological binaries
    	$aut_LRRL=$dicos_path."/apertium-$nomtrad/$LR-$RL.automorf.bin";
    	$aut_RLLR=$dicos_path."/apertium-$nomtrad/$RL-$LR.automorf.bin";
	

	if($valida!=1 && $user=="")
	{
		$user=$REDIRECT_REMOTE_USER;
		//$user="validator";	
	}
	
	

if($acc!="Delete")
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
    <title>OpenTrad apertium insertion form. Insert new<?= $show ?> </title>

   <script language="JavaScript">
    <!-- 
   	function cierra() 
   	{
   		window.close();
   	}
	
   //Comprueba que se han seleccionado los paradigmas
	function   compruebaParad(form)
	{
	
	   if((form.paradigmaLR.options[form.paradigmaLR.selectedIndex].value)=="None"
	   ||(form.paradigmaRL.options[form.paradigmaRL.selectedIndex].value)=="None" )
	   {
		alert("Select the inflection paradigm");
		return false;
	   }else if (form.LRlem.value=="" || form.RLlem.value=="")
	    {
		alert("Empty lemma");
		return false;
	    }
	    else	
	    {
		return true;	
	    }
	
	}
	
      -->
    </script>
  </head>


<?php

} //end if (acc=="Delete")


if($acc=="" || $acc==null)  //selecc.php is shown again
{

?>


 <body>
    <!-- For non-visual user agents: -->
      <div id="top"><a href="#nrc-main-copy" class="doNotDisplay doNotPrint">Skip to main content.</a></div>

    <!-- ##### Header ##### -->

    <div id="header">
      <h1 class="headerTitle">
        <a href="../" title="Browse to homepage"><span>Opentrad apertium</span> insertion form</a>
      </h1>
    </div>  
    
	<div id="side-bar">

      <!-- ##### Left Sidebar ##### -->

      <div class="leftSideBar">
        <br/>
        <p class="sideBarTitle"><a href="http://apertium.sourceforge.net/">More information</a></p>
	<br>
        <p class="sideBarTitle"><a href="http://sourceforge.net/projects/apertium/">Downloads</a></p>
      </div>

    </div>
    <!-- ##### Main Copy ##### -->

    <div id="nrc-main-copy">
      <h1 id="text" style="border-top:none; padding-top: 0;">Insert new<?= $show ?></h1>

<?


if($valida==1)
	echo "User: $user";
	
	if($valida!=1)
		$accion="selecc.php";
	else	
		$accion="valida.php";
//		$accion="selecc.php";
		
		

?>
	<form action=<?= $accion ?> method="post">	
	  <fieldset>
	       
		<div>
		  <label style="margin-right: 8em; text-align:left">Lemma (<?=$LR?>)</label>
		  <label style="margin-left: 7em; text-align:right">Lemma (<?=$RL?>)</label>
		</div>	  
<?php 

//se eliminan las barras '\' de las cadenas con los lemas
$LRlem=str_replace("\\","",$LRlem);      
$RLlem=str_replace("\\","",$RLlem);       

// COMPRUEBA QUE LOS LEMAS NO EXISTEN 

 //in temporary professional dictionary lang 1
	
system("grep 'lm=\"$LRlem\"' $fich_LR > /dev/null", $retval);
     
if ($retval==0)
{
	$warningLR.="This word is already in the temporary dictionaries ($LR):<br/>".$LRlem  ; 
	$showErrLR=1;
}
            
 //in temporary  semi-professional dictionary lang 1
if($valida!=1) 
{ 
	//Se comprueban  los temporales semiprofesionales
		
	$mirasemi="awk '$1==\"$LRlem\" {print $1\" \"$2}' $fich_semi";		
	$salSemi=shell_exec($mirasemi);
	if($salSemi !="")
	{
		$showErrLR=1;
		if (strlen($warning)>0)	//crea la cadena de advertencia a mostrar
	    		$warningRL.="<br/>";
			
		$salSemi=htmlspecialchars($salSemi);
		$salSemi=str_replace("\n","<br/>",$salSemi);
		$warningLR.="This word is already in the dictionaries to be validated ($LR):<br/>".$LRlem ;
	}
}
 
 //in final dictionary lang 1
//ARANES!!
if($LR == "oc"){
	system("grep 'lm=\"$LRlem\"' ".$dicos_path."/apertium-$nomtrad/apertium-$nomtrad.$LR.metadix > /dev/null", $retval);
}else{
	system("grep 'lm=\"$LRlem\"' ".$dicos_path."/apertium-$nomtrad/apertium-$nomtrad.$LR.dix > /dev/null", $retval);
}
//fi ARANES

	  
if ($retval==0)
{  
    $warningLR.="This word is already in the dictionary ($LR):<br/>".$LRlem ; 
    $showErrLR=1;
}


 //in temporary professional dictionary lang 2  
system("grep 'lm=\"$RLlem\"' $fich_RL > /dev/null", $retval);
      
if ($retval==0)
{
    $warningRL.="This word is already in the temporary dictionaries ($RL):<br/>".$RLlem  ;
    $showErrRL=1;
}
  
 //in temporary  semi-professional dictionary lang 2
if($valida!=1) 
{ 
	
	$mirasemi2="awk '$2==\"$RLlem\" {print $1\" \"$2}' $fich_semi";
	$salSemi=shell_exec($mirasemi2);
		
	if($salSemi !="")
	{
		$showErrRL=1;
		if (strlen($warning)>0)	//crea la cadena de advertencia a mostrar
	    		$warningRL.="<br/>";
		
		$salSemi=htmlspecialchars($salSemi);
		$salSemi=str_replace("\n","<br/>",$salSemi);
		$warningRL.="This word is already in the dictionaries to be validated ($RL):<br/>".$RLlem ;
	}
}
  
 //final dictionary lang 2 
 
system("grep 'lm=\"$RLlem\"' ".$dicos_path."/apertium-$nomtrad/apertium-$nomtrad.$RL.dix > /dev/null", $retval);
	  
if ($retval==0)
{  
    $warningRL.="This word is already in the dictionary($RL):<br/>".$RLlem ;
    $showErrRL=1;		
}
     


//if($valida!=1)  //normal insertion
{ 
      
?>
		<div>
		<label style="margin-right: 1em">
	  	  <input type="text" 
		         name="LRlem" 
			 value="<?php print("$LRlem"); ?>" >
		</label>	
<?php

}
// else  //Data validation. Shows the words to be validated.
//    {
//    	
//   	echo "\t\t\t<input type=\"hidden\" name=\"valida\" value=\"1\">\n";
//	
//	
	//to select a word to be validated
//	echo "<div>\n";
//	echo "  <label style=\"margin-right: 1em\">\n";
//	echo "    <select style=\"width:12em;\" name=\"LRlem\" onchange=\"document.forms[0].submit()\">\n";
//	for($cont=0;$cont<count($LRlemV);$cont++)
//	{
//		if(($LRlemV[$cont])==$LRlem)
//		{
//			$selected="selected";
//		}
//		else
//		{
//			$selected="";
//		}
//		echo  "\t\t\t<option value=\"".$LRlemV[$cont]."\" $selected>".$LRlemV[$cont]."</option>\n";
//	}
//	echo "    </select>\n";
//	echo "  </label>\n";
//	
//     }



if($showErrLR==1 && $showErrRL==0)
	$izda="selected";
else	if($showErrLR==0 && $showErrRL==1)
	$drcha="selected";
else	if($direccion==":<:")
	$izda="selected";
else	if($direccion==":>:")
	$drcha="selected";
else	$centro="selected";
?>
		  <label style="margin-right: 1em">
		    <select name="direccion">
			  <option value=":>:" <?=$drcha?> >---></option>
			  <option value=":" <?=$centro?>  ><---></option>
			  <option value=":<:" <?=$izda?>  ><---</option>
	  	    </select>
		  </label>
		  <label>
		    <input type="text" 
		    	   name="RLlem" 
			   value="<?php print("$RLlem"); ?>" >
		  </label>
		</div>
		
		
			
<?php

if($showErrLR==1) 
	print("\t\t<div>\n  \t\t<left><b style=\"margin-right: 2em; color:red\">$warningLR</b></left>\n\t\t</div>\n");

if($showErrRL==1) 
	print("\t\t<div>\n  \t\t<right><b style=\"color:red\">$warningRL</b></right>\n\t\t</div>\n");

?>
		<div>
		  <label style="margin-right: 5em; text-align:left">Word "<?=$LRlem?>" is as...</label>
		  <label>Word "<?=$RLlem?>" is as...</label>
	        </div>
		
	        <div>
		  <label>
		    <select name="paradigmaLR" onchange="document.forms[0].submit()">
			   
<?php 
	
//ARANES!!
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

function PosicioVocalTall($lema, $vocal, $infinitiu){
// torna la posició dins del $lema on es troba la $vocal que separa l'arrel de la desinencia.
// És necessari la terminació d'infinitiu del verb $lema i la longitud de la part fixa del paradigma
// (el que va entre []). Busquem la vocal del final cap al principi, tornem la primera ocurrència de $vocal
        $i=strlen($lema)-strlen($infinitiu)-1;

	$posicio=false;
	for( ; $i >= 0; $i--){
		if($lema[$i] == $vocal) { $posicio=$i; $i=-1; }
	}	

//	echo "lema(".strlen($lema).")-infinitiu(".strlen($infinitiu).")-long fixa(".$long_fixa_par."posicio= ".$posicio;

	return $posicio;
}
//FI ARANES!!

  $fde=fopen($path_paradigmasLR,"r");
  
  $primero=1;
  while (!feof($fde)) {
  
      $buffere = fgets($fde, 4096);
      $cadenae = explode("\t",trim($buffere));
	
	      
      if($cadenae[0]!="")
      {
      	if($primero==1 && ($paradigmaLR=="" || $paradigmaLR=="None"))//Mira si no se le pasa ningun paradigma
	{
		echo("\t\t\t<option value=\"None\" selected>Select a paradigm</option>\n");
		$primero=0;
	}
	
	if(strcmp($paradigmaLR,$cadenae[0])==0) //comprueba si es el paradigma seleccionado
      	 $selected="selected ";
	else
	 $selected="";
		 
		
	if (count($cadenae)>1)
	{
	  	//Añade el género y número al final del paradigma
	      	$gener=strtoupper($cadenae[count($cadenae)-1]);
	      	$numLR=$cadenae[count($cadenae)-2];    	  
	}
	else
	   {
		$gener="";
	   }


	if($tipo!="n" && $tipo!="adj" && $tipo!="acr")	// en los verbos no se pone género
	{
		$gener="";
		$numLR="";
	}


		
	//Se comprueba si el paradigma puede aplicarse al lema que se ha introducido
	$vectParLR=explode(" ",$cadenae[0]);
      	$arr=explode("/",$vectParLR[0]);

//**ARANES!!!:la terminació de l'infinitiu s'arreplega dels paradigmes asociats, no dels examples
	if($tipo=="vb")
	{
		$arr=explode("/",$cadenae[2]);                    //cadenae2 te el primer paradigma associat
		$inicio_inf=strpos($arr[1],"]");
		if($inicio_inf === false){ $inicio_inf=0; }
		else{ $inicio_inf=$inicio_inf+1; }
		$arr[1]= substr($arr[1],$inicio_inf,strpos($arr[1],"__")-$inicio_inf ); //agafem la terminacio de l'infinitiu
      	}
//*FI ARANES			
//**ARANES!!	
// $cadenaeParaMostrar=strtolower($cadenae[0]);	
	$cadenaeParaMostrar=$cadenae[0];	


      	if(sizeof($arr)==2 && $selected=="")	//si el lema se corta    ej:"alumn/o" y no es el paradigma seleccionado
	{
		if($arr[1]==substr($LRlem,strlen($LRlem)-strlen($arr[1])))   //si coinciden los finales del paradigma y del lema introducido
			echo("\t\t\t<option value=\"$cadenae[0]\" $selected>$cadenaeParaMostrar ".$gener." ".$numLR."</option>\n");
	}
      	else
                	echo("\t\t\t<option value=\"$cadenae[0]\" $selected>$cadenaeParaMostrar ".$gener." ".$numLR."</option>\n");
		
			
      }
      
    }
    if($tipo=="n" || $tipo=="adj" || tipo=="acr")
    {
    if($paradigmaLR=="Altrecas")
  	$selected="selected";
    else
  	$selected="";	
    print("\t\t\t<option value=\"Altrecas\" $selected >None of these</option>\n");
    }
	  	  

  fclose ($fde);


    
?>  		 
		    </select>
	      	  </label>
			
<?
if($tipo=="n" || $tipo=="adj" || $tipo=="acr")
{
 	//echo "<center>"; 
	
	//se obtienen los datos del paradigma de español actual
      $cadparadigma=shell_exec("grep '^$paradigmaLR' $path_paradigmasLR");
      
        //La cadena devuelta por el grep la metemos en un array separando las pal
      $array_paradigma_LR=explode("\t",$cadparadigma);
      $generosLR=$array_paradigma_LR[count($array_paradigma_LR)-1];

	//se obtienen los datos del paradigma de catalan actual
      $cadparadigma=shell_exec("grep '^$paradigmaRL' $path_paradigmasRL}");
      
      //La cadena devuelta por el grep la metemos en un array separando las pal
      $array_paradigma_RL=explode("\t",$cadparadigma);
      $generosRL=$array_paradigma_RL[count($array_paradigma_RL)-1];
      
 	
      	
}
?>
		  <label style="margin-left: 6em">
		    <select name="paradigmaRL" onchange="document.forms[0].submit()">
<?php
		
		
  $fdp=fopen($path_paradigmasRL,"r");
  $primero=1;
  while (!feof($fdp)) 
  {
      $bufferp = fgets($fdp, 4096);
      $cadenap = explode("\t",trim($bufferp));
	      
      if($cadenap[0]!="")	
      {
      	if($primero==1 && ($paradigmaRL=="" || $paradigmaRL=="None")) //Mira si no se le pasa ningun paradigma
	{
           echo("\t\t\t<option value=\"None\" selected>Select a paradigm</option>\n");
		//$paradigmaRL="Ninguno";	//se le asigna el primero
		$primero=0;
	}
	if(strcmp($paradigmaRL,$cadenap[0])==0)
	      	 $selected="selected ";
	else
		 $selected="";
        if (count($cadenap)>1)
        {
	      	$gener=strtoupper($cadenap[count($cadenap)-1]);
	      	$numRL=$cadenap[count($cadenap)-2];
    	   
	}      	
	else
	{
	      	$gener="";
	}


	if($tipo!="n" && $tipo!="adj" && $tipo!="acr")	// en los verbos no se pone género
	{
		$gener="";
		$numRL="";
        }
			
	//Se comprueba si el paradigma puede aplicarse al lema que se ha introducido
	$vectParRL=explode(" ",$cadenap[0]);
      	$arr=explode("/",$vectParRL[0]);
      	if(sizeof($arr)==2 && $selected=="")	
	{
		if($arr[1]==substr($RLlem,strlen($RLlem)-strlen($arr[1])))
			echo("\t\t\t<option value=\"$cadenap[0]\" $selected >$cadenap[0] ".$gener." ".$numRL."</option>\n");
	}
      	else
                	echo("\t\t\t<option value=\"$cadenap[0]\" $selected >$cadenap[0] ".$gener." ".$numRL."</option>\n");
      }
  }
  if($tipo=="n" || $tipo=="adj" || tipo=="acr")
  {
    if($paradigmaRL=="Altrecas")
  	  $selected="selected";
    else
  	  $selected="";	
          print("\t\t\t<option value=\"Altrecas\" $selected >None of these</option>\n");
  }	  	 
 fclose ($fdp);
?>  
		    </select>
		  </label>
	        </div>
	        <div>
				
<?php
	
//Flexiones del lema de castellano
	
      
      $vectParLR=explode(" ",$paradigmaLR);
      $arr=explode("/",$vectParLR[0]);
      if(sizeof($arr)==2)
	      $vectParLR[0]="/".$arr[1];
      else
              $vectParLR[0]=" ";
	
      $partmp=strtok($paradigmaLR," ");			//coge el nombre del paradigma hasta el primer espacio
      $paradigmaLR=$partmp;				// ya q este puede tener información adicional
      
    
	      
      $token=strtok($paradigmaLR,"/");
      $raiz_LR=substr($LRlem, 0, strlen($LRlem)-(strlen($paradigmaLR)-strlen($token)-1));
      					
      //genera los ejemplos
      $ejemplosLR="";
      if(sizeof($vectParLR)>0)
      {
        for($cont=0; $cont < sizeof($vectParLR) ;$cont++)
	{
	
		
		$flexion=substr ($vectParLR[$cont],1);


//ARANES!!
	     	if($LR == "oc" && $paradigmaLR != ""){ //si es occita i ja em seleccionat algún paradigma
	     	        //se obtienen los datos del paradigma de LR actual
	     	        $cadparadigma=shell_exec("grep ^$paradigmaLR $path_paradigmasLR");
     	                //La cadena devuelta por el grep la metemos en un array separando las pal
                        $cadenae=explode("\t",$cadparadigma);  //en cadenae[2] tenim el primer paradigma associat
                  
                        $token=$cadenae[2]; //agafem el paradigma

//$token=strtok($token,"/");
//$token=strtok("/");	//agafem el que hi ha després de l'arrel
	     	
			$inicio_inf=strpos($token,"]");
			if($inicio_inf === false){ 
				$inicio_inf=strpos($token,"/")+1; 
				$parametritzat=0;
			}
			else{ 
				$inicio_inf=$inicio_inf+1; 
				$parametritzat=1;
			}				//mira on comença l'infinitiu


//if($inicio_inf==0){$long_fixa_par=0;}
//else{
//	$long_fixa_par= strpos($token,"]") - strpos($token,"[") - 1;   //longitud de la part fixa del paradigma (el parametre, si en te)
//}

			if($parametritzat == 1){
				$long_fixa_par= strpos($token,"]") - strpos($token,"[") - 1;   //longitud de la part fixa del paradigma (el parametre, si en te)
				if($long_fixa_par==-1){$long_fixa_par=0;}
	     		
				$infinitiu=substr($token,$inicio_inf,strpos($token,"__")-$inicio_inf );  //terminació de l'infinitiu;

//				$vocal=$flexion[0];
				$vocal_de_flexion=$flexion[0];

				$vocal=substr( $token,strpos($token,"/")+1,1 );
				$inicio_flexion=1+$long_fixa_par;
				$flexion=substr($flexion,$inicio_flexion,strlen($flexion)-$inicio_flexion);

				$long_fixa_lema= (strlen($LRlem)-strlen($infinitiu)-1) - PosicioVocalTall($LRlem,$vocal,$infinitiu);

				$raiz_LR=substr($LRlem, 0, strlen($LRlem)-(1 + $long_fixa_lema + strlen($infinitiu) ));
				//CALCULEM l'ARREL del lema a introduir (el que hi haurà davant de \ )
				//pendent de millorar !!!
				$fixa=substr($LRlem,strlen($raiz_LR)+1,$long_fixa_lema); 
				//CALCULEM la PART FIXA del lema (el que es posaria entre [])      
				//pendent de millorar !!!
			}
			else{
				$vocal=$fixa="";
			}
//			$flexion=$vocal.$fixa.$flexion;		
			$flexion=$vocal_de_flexion.$fixa.$flexion;		

		}
//FI ARANES!!



		$ejemplosLR.="\t\t  <label style=\"margin-right: 1em\">".$raiz_LR."<b style=\"color: red\">".$flexion."</b></label>\n";
	}
      }
      echo($ejemplosLR );
?>	 	 
		  <label style="margin-left: 8em"></label>
<?php
	
		//Flexiones del lema de catalán
	 // coge las posibles terminaciones
	
      $toktmp=strtok($paradigmaRL,"/");
      $vectParRL=explode(" ",$paradigmaRL);
      $arr=explode("/",$vectParRL[0]);
      if(sizeof($arr)==2)
	      $vectParRL[0]="/".$arr[1];
      else
              $vectParRL[0]=" ";
	
      $partmp=strtok($paradigmaRL," ");			//coge el nombre del paradigma hasta el primer espacio
      $paradigmaRL=$partmp;				// ya q este puede tener información adicional
      							//ej;  alt/o /a /os /as -> solo quiero "alt/o"
     	      
	      
      $token=strtok($paradigmaRL,"/");
      $raiz_RL=substr($RLlem, 0, strlen($RLlem)-(strlen($paradigmaRL)-strlen($token)-1));
      					
      //genera los ejemplos
      $ejemplosRL="";
      if(sizeof($vectParRL)>0)
      {
      	for($cont=0; $cont < sizeof($vectParRL) ;$cont++)
	{
		$flexion=substr ($vectParRL[$cont],1);
		$ejemplosRL.="\t\t  <label style=\"margin-right: 1em\">".$raiz_RL."<b style=\"color:red\">".$flexion."</b></label>\n";
	}
      }
      echo($ejemplosRL);
?>	 
		</div>
		<div>
		
		
<?php 
//echo "num es ahora es=".$numLR.".";
if($tipo!="vb" && $tipo!="prep")
{

	 if($paradigmaLR=="Altrecas")
	 {
		if($geneLR=="")	$geneLR="m";
		if($numeLR=="")	$numeLR="sg";		
?>			
			<input type="radio" name="geneLR" value="m" <?if ($geneLR=="m") echo "checked ";  echo $disabled; ?> >m &nbsp;
			<input type="radio" name="geneLR" value="f" <?if ($geneLR=="f") echo "checked ";  echo $disabled; ?> >f &nbsp;
			<input type="radio" name="geneLR" value="mf" <?if ($geneLR=="mf") echo "checked ";  echo $disabled; ?> >mf &nbsp;
                        <input type="radio" name="geneLR" value="nt" <?if ($geneLR=="nt") echo "checked ";  echo $disabled; ?> >nt &nbsp;
			<input type="radio" name="numeLR" value="sg" <?if ($numeLR=="sg") echo "checked ";  echo $disabled; ?> >sg &nbsp;
			<input type="radio" name="numeLR" value="pl" <?if ($numeLR=="pl") echo "checked ";  echo $disabled; ?> >pl &nbsp;
			<input type="radio" name="numeLR" value="sp" <?if ($numeLR=="sp") echo "checked ";  echo $disabled; ?> >sp &nbsp;
<?
	}
?>		
		<label style="margin-left: 10em"> </label>
<?php 
	 if($paradigmaRL=="Altrecas")
	 {
	 	if($geneRL=="")	$geneRL="m";
		if($numeRL=="")	$numeRL="sg";
?>
			<input type="radio" name="geneRL" value="m" <?if ($geneRL=="m") echo "checked ";  echo $disabled; ?> >m &nbsp;
			<input type="radio" name="geneRL" value="f" <?if ($geneRL=="f") echo "checked ";  echo $disabled; ?> >f &nbsp;
			<input type="radio" name="geneRL" value="mf" <?if ($geneRL=="mf") echo "checked ";  echo $disabled; ?> >mf &nbsp;
                        <input type="radio" name="geneRL" value="nt" <?if ($geneRL=="nt") echo "checked ";  echo $disabled; ?> >nt &nbsp;
			<input type="radio" name="numeRL" value="sg" <?if ($numeRL=="sg") echo "checked ";  echo $disabled; ?> >sg &nbsp;
			<input type="radio" name="numeRL" value="pl" <?if ($numeRL=="pl") echo "checked ";  echo $disabled; ?> >pl &nbsp;
			<input type="radio" name="numeRL" value="sp" <?if ($numeRL=="sp") echo "checked ";  echo $disabled; ?> >sp &nbsp;
<?
	 }
}

?>

		</div>

		<div>
	          <label>Your comments: 
		    <input type="text" name="comentarios" value="<?= htmlspecialchars($comentarios)?>" size="40">
		  </label>
		 </div>

			<input type="hidden" name="LR" value="<?= $LR ?>"/>
			<input type="hidden" name="RL" value="<?= $RL ?>"/>
			<input type="hidden" name="lemLR" value="<?= $LRlem ?>"/>
			<input type="hidden" name="lemRL" value="<?= $RLlem ?>"/>
			<input type="hidden" name="mostrar" value="<?= $show ?>"/>
			<input type="hidden" name="antParadLR" value="<?= $paradigmaLR ?>"/>
			<input type="hidden" name="antParadRL" value="<?= $paradigmaRL ?>"/>
			<input type="hidden" name="user" value="<?= $user ?>"/>
			<input type="hidden" name="tipo" value="<?= $tipo ?>"/>
                                 
					
			
		<div>
		  <a href="verSemi.php?LR=<?= $LR ?>&RL=<?= $RL ?>" target="_blank">See pairs to be validated</a>
		    <br/>
		</div>
		<div>	

<?		
	if ($valida==1)  //si se esta validando se pone un botón para borrar la palabra actual
	{
?>
		  <label>
		    <input type="submit" 
		           name="acc" 
			   value="Delete" />
	          <label>
<?
	}
?>		
		  <label>
		    <input type="button" 
		    	   name="acc" 
			   value="- Close -"  
			   onclick="cierra()" />
		  </label>
		  <label>
		    <input type="submit" 
		       	   name="acc" 
			   value="Go on >>"  
			   onclick=" return compruebaParad(this.form)" />
		  </label>
		</div>
		
    	    </form>
<?
$userAct=$user;

if (in_array($userAct, $usuaris_professionals))
{
	if($valida!=1)
	{
		echo "\t\t<div>";
		echo "<a href=\"valida.php?RL=$RL&LR=$LR\">Validate pairs</a>";
		echo "</div>\n";
        }
}
?>
          </fieldset>
	</div>

   <!-- ##### Footer ##### -->

     <div id="footer">
   <!--   <div class="doNotPrint">
        <a href="./index.html">Tell a Friend</a> |
        <a href="./index.html">Privacy Policy</a> |
        <a href="./index.html">Site Map</a> |
        <a href="./index.html">Feedback</a> |
        <a href="./index.html">Help</a>
      </div>
   -->
      <div><a href="http://www.dlsi.ua.es">
        Departament de Llenguatges i Sistemes Informàtics</a>, Universitat
        d'Alacant, 2006 | Last modified 20.02.2006<!-- by <a
        href="mailto:sortiz@dlsi.ua.es" title="Envíame un correo">Sergio
        Ortiz</a> -->
      </div>
    </div>

</body>
</html>

<?php
} //Final si $acc es vacio      
else
{

	if($acc=="Delete")
	{
		$tmpfname = tempnam("/tmp", "semi");
      		if(file_exists($tmpfname))
		{
			
			copy($fich_semi,$tmpfname);	
			$creasemi="awk '{FS=\"\\t\"} $1!=\"$LRlem\" && $2!=\"$RLlem\"{print $0}' $tmpfname > $fich_semi";	
			$salida=shell_exec($creasemi);
		} 
		
		$url = "./valida.php?RL=$RL&LR=$LR";
		header("Location: $url");

		
	}
	else
	{
	   
                $userAct=$REDIRECT_REMOTE_USER;		
	
		if (in_array($userAct, $usuaris_professionals))
		{
			include("insertarPro.php"); //se inserta en el fichero validado
		}
		else
	
			include("insertar.php");  //se inserta en el fichero sin validar.
	}

}// final else

?>


