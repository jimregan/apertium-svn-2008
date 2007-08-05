<?
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
	
	
	$fich_LR="$path/apertium-$nomtrad.$LR.webform";
	$fich_RL="$path/apertium-$nomtrad.$RL.webform";
    	$fich_LRRL="$path/apertium-$nomtrad.$LR-$RL.webform";
	$fich_semi="$path/$LR-$RL.semi.dic";

	
	$path_paradigmasLR="./paradigmas.$LR-$RL/paradig".$cadFich."_".$LR;
    	$path_paradigmasRL="./paradigmas.$LR-$RL/paradig".$cadFich."_".$RL;
	

	//Rutas de los dic. compilados
    	$aut_LRRL=$dicos_path."/apertium-$nomtrad/$LR-$RL.automorf.bin";
    	$aut_RLLR=$dicos_path."/apertium-$nomtrad/$RL-$LR.automorf.bin";
	
	$palabrasLR=explode(" ",$LRlem);
	$palabrasRL=explode(" ",$RLlem);
	
	$LRverb=$palabrasLR[0];
	$RLverb=$palabrasRL[0];
	
	
	
	 if($palabrasLR[1]!="")
	 {
	   $posLR=strpos($LRlem," "); 
	   $LRcua=substr($LRlem,$posLR);
	 }
	 else
	 	
	   $LRcua="";
		
		
	 if($palabrasRL[1]!="")
	 {
	   
	  $posRL=strpos($RLlem," "); 
	  $RLcua=substr($RLlem,$posRL);
	  
	 }
	 else
	 	
	   $RLcua="";
		
	

	
	if($valida!=1 && $user=="")
	{
		$user=$REDIRECT_REMOTE_USER;
		
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
    <!--  //Comprueba que se han seleccionado los paradigmas
   
        function cierra() 
   	{
   		window.close();
   	}
   
	function   compruebaParad(form)
	{
	
	 if((form.paradigmaLR.options[form.paradigmaLR.selectedIndex].value)=="Ninguno"
	||(form.paradigmaRL.options[form.paradigmaRL.selectedIndex].value)=="Ninguno" )
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

} //final comprob accion

if($acc=="" || $acc==null)  //volvemos a mostrar la página de seleccion de paradigmas
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
	echo "Usuari: $user";
	
	if($valida!=1)
		$accion="multip.php";
	else	
		$accion="valida.php";
	   
?>		
	<form action=<?= $accion ?> method="post">	
	  <fieldset>
	       
	       
	        <br/><br/>
		<label style="margin-right: 8em; text-align:left">Lemma (<?=$LR?>)</label>
		<label style="margin-left: 7em; text-align:right">Lemma (<?=$RL?>)</label>
	        <br/><br/>	  
<?php 

//se eliminan las barras '\' de las cadenas con los lemas
$LRlem=str_replace("\\","",$LRlem);      
$RLlem=str_replace("\\","",$RLlem);       

// COMPRUEBA QUE LOS LEMAS NO EXISTEN 

 //temporary dictionary lang 1
	
system("grep 'lm=\"$LRlem\"' $fich_LR > /dev/null", $retval);
     
if ($retval==0)
  {
	$warningLR.="This word is already in the temporary dictionaries:($LR):<br/> ".$LRlem  ; 
	$showErrLR=1;
  }
      
      
 //final dictionary lang 1

system("grep 'lm=\"$LRlem\"' ".$dicos_path."/apertium-$nomtrad/apertium-$nomtrad.$LR.dix > /dev/null", $retval);
	  
if ($retval==0)
 {  
    $warningLR.="<br/>This word is already in the dictionary ($LR):<br/> ".$LRlem ; 
    $showErrLR=1;
 }


 //temporary dictionary lang 2  
system("grep 'lm=\"$RLlem\"' $fich_RL > /dev/null", $retval);
      
if ($retval==0)
  {
    $warningRL.="<br/>This word is already in the temporary dictionaries:($RL):<br/> ".$RLlem  ;
    $showErrRL=1;
  }
      
 //final dictionary lang 2 
 
system("grep 'lm=\"$RLlem\"' ".$dicos_path."/apertium-$nomtrad/apertium-$nomtrad.$RL.dix > /dev/null", $retval);
	  
if ($retval==0)
 {  
    $warningRL.="<br/>This word is already in the dictionary($RL):<br/> ".$RLlem ;
    $showErrRL=1;		
 }
     


if($valida!=1)  //normal insertion
 { 
      
?>
		<label style="margin-right: 1em">
	  	  <input type="text" name="LRverb" value="<?php print("$LRverb"); ?>" >
		</label>	
<?php

 }
 else  //Data validation. Shows the words to be validated.
    {
      	
   	echo "<input type=\"hidden\" name=\"valida\" value=\"1\">";
	echo "<input type=\"hidden\" name=\"multip\" value=\"$multip\">";
	
	//to select a word to be validated
	echo  "<select name=\"LRlem\" onchange=\"document.forms[0].submit()\">";
	for($cont=0;$cont<count($LRlemV);$cont++)
	{
		if(($LRlemV[$cont])==$LRlem)
		{
			$selected="selected";
		}
		else
		{
			$selected="";
		}
		echo  "    <option value=\"".$LRlemV[$cont]."\" $selected>".$LRlemV[$cont]."</option>\n";
	}
	echo "</select>";
	
     }



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
		  <input type="text" name="RLverb" value="<?php print("$RLverb"); ?>" >
		</label>
		<br/><br/>
		<br/><br/>
		
			
<?php

if($showErrLR==1) 
	print("<br/><left><b style=\"margin-right: 2em; color:red\">$warningLR</b></left>");

if($showErrRL==1) 
	print("<right><b style=\"color:red\">$warningRL</b></right><br/>");

?>
		<label style="margin-right: 5em; text-align:left">
		       Word "<?=$LRlem?>" is as...</label>
		<label>Word "<?=$RLlem?>" is as...</label>
		<br/><br/>
		
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
                                                                                                
	
  $fde=fopen($path_paradigmasLR,"r");
  
  $primero=1;
  while (!feof($fde)) {
  
      $buffere = fgets($fde, 4096);
      $cadenae = explode("\t",trim($buffere));
	
	      
      if($cadenae[0]!="")
      {
      	if($primero==1 && ($paradigmaLR=="" || $paradigmaLR=="Ninguno"))//Mira si no se le pasa ningun paradigma
	{
		echo("\t\t\t<option value=\"Ninguno\" selected>Selecciona paradigma</option>\n");
		$primero=0;
	}
	
	if(strcmp($paradigmaLR,$cadenae[0])==0) //comprueba si es el paradigma seleccionado
      	 $selected="selected ";
	else
	 $selected="";
		 
		
	if (count($cadenae)>1)
	{
	  	//Añade el género al final del paradigma
	      	$gener=strtoupper($cadenae[count($cadenae)-1]);
	}
	else
	   {
		$gener="";
	   }	
	if($tipo=="mw")		// en los verbos no se pone género
		$gener="";
		
		
	//Se comprueba si el paradigma puede aplicarse al lema que se ha introducido
	$vectParLR=explode(" ",$cadenae[0]);
      	$arr=explode("/",$vectParLR[0]);

//**ARANES!!!:la terminació de l'infinitiu s'arreplega dels paradigmes asociats, no dels examples
        if($tipo=="mw")
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
		if($arr[1]==substr($LRverb,strlen($LRverb)-strlen($arr[1])))   //si coinciden los finales del paradigma y del lema introducido
			echo("\t\t\t<option value=\"$cadenae[0]\" $selected>$cadenaeParaMostrar ".$gener."</option>\n");
	}
      	else
              	echo("\t\t\t<option value=\"$cadenae[0]\" $selected>$cadenaeParaMostrar ".$gener."</option>\n");
		
			
      }
    }
	  

  fclose ($fde);
    
?>  		 
	          </select>
	      	</label>
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
      	if($primero==1 && ($paradigmaRL=="" || $paradigmaRL=="Ninguno")) //Mira si no se le pasa ningun paradigma
	{
           echo("\t\t\t<option value=\"Ninguno\" selected>Selecciona paradigma</option>\n");
		//$paradigmaRL="Ninguno";	//se le asigna el primero
		$primero=0;
	}
	if(strcmp($paradigmaRL,$cadenap[0])==0)
	      	 $selected="selected ";
	else
		 $selected="";
        if (count($cadenap)>1)
	      	$gener=strtoupper($cadenap[count($cadenap)-1]);
	else
	      	$gener="";
	if($tipo=="mw")	// en los verbos no se pone género
		$gener="";
			
	//Se comprueba si el paradigma puede aplicarse al lema que se ha introducido
	$vectParRL=explode(" ",$cadenap[0]);
      	$arr=explode("/",$vectParRL[0]);
      	if(sizeof($arr)==2 && $selected=="")	
	{
		if($arr[1]==substr($RLverb,strlen($RLverb)-strlen($arr[1])))
			echo("\t\t\t<option value=\"$cadenap[0]\" $selected >$cadenap[0] ".$gener."</option>\n");
	}
      	else
                	echo("\t\t<option value=\"$cadenap[0]\" $selected >$cadenap[0] ".$gener."</option>\n");
      }
}
	  	 
          fclose ($fdp);
?>  
		  </select>
		</label>
		<br/><br/>		
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
      $raiz_LR=substr($LRverb, 0, strlen($LRverb)-(strlen($paradigmaLR)-strlen($token)-1));
      					
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
                                                                                                                                        
                                                                                                                                                                $inicio_inf=strpos($token,"]");
                           if($inicio_inf === false){
                                   $inicio_inf=strpos($token,"/")+1;
                                   $parametritzat=0;
                           }
                           else{
                                   $inicio_inf=$inicio_inf+1;
                                   $parametritzat=1;
                           }                               //mira on comença l'infinitiu
  
                           if($parametritzat == 1){
                                   $long_fixa_par= strpos($token,"]") - strpos($token,"[") - 1;   //longitud de la part fixa del paradigma (el parametre, si en
                                   if($long_fixa_par==-1){$long_fixa_par=0;}
                                                                                         
                                   $infinitiu=substr($token,$inicio_inf,strpos($token,"__")-$inicio_inf );  //terminació de l'infinitiu;

                                $vocal_de_flexion=$flexion[0];

                                $vocal=substr( $token,strpos($token,"/")+1,1 );
                                $inicio_flexion=1+$long_fixa_par;
                                $flexion=substr($flexion,$inicio_flexion,strlen($flexion)-$inicio_flexion);

                                $long_fixa_lema= (strlen($LRverb)-strlen($infinitiu)-1) - PosicioVocalTall($LRverb,$vocal,$infinitiu);

                                $raiz_LR=substr($LRverb, 0, strlen($LRverb)-(1 + $long_fixa_lema + strlen($infinitiu) ));
                                //CALCULEM l'ARREL del lema a introduir (el que hi haurà davant de \ )
                                //pendent de millorar !!!
                                $fixa=substr($LRverb,strlen($raiz_LR)+1,$long_fixa_lema);
                                //CALCULEM la PART FIXA del lema (el que es posaria entre [])
                                //pendent de millorar !!!
                        }
                        else{
                                $vocal=$fixa="";
                        }
//                      $flexion=$vocal.$fixa.$flexion;
                        $flexion=$vocal_de_flexion.$fixa.$flexion;

                }
//FI ARANES!!
                                                                                                                         
                                                                                                                         
		$ejemplosLR.="\t\t<label style=\"margin-right: 3em\">".$raiz_LR."<b style=\"color:red\">".$flexion."</b></label>";
	}
      }
      echo($ejemplosLR);

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
      $raiz_RL=substr($RLverb, 0, strlen($RLverb)-(strlen($paradigmaRL)-strlen($token)-1));
      					
      //genera los ejemplos
      $ejemplosRL="";
      if(sizeof($vectParRL)>0)
      {
      	for($cont=0; $cont < sizeof($vectParRL) ;$cont++)
	{
		$flexion=substr ($vectParRL[$cont],1);
		$ejemplosRL.="\n\t\t<label style=\"margin-left: 3em\">".$raiz_RL."<b style=\"color:red\">".$flexion."</b></label>\n";
	}
      }
      echo($ejemplosRL); 
      
?>
		<br/><br/>		 
		 
		
		
		

		<label>
	  	  <input type="text" 
		         name="LRcua" 
			 value="<?php print("$LRcua"); ?>" >
		</label>
		<label style="margin-left: 6em">
	  	<input type="text" 
		       name="RLcua" 
		       value="<?php print("$RLcua"); ?>" >
		</label>
		<br/><br/>
		<label><input type="hidden" name="LR" value="<?= $LR ?>"/></label>
		<label><input type="hidden" name="RL" value="<?= $RL ?>"/></label>
		<label><input type="hidden" name="LRverb" value="<?= $LRverb ?>"/></label>
		<label><input type="hidden" name="RLverb" value="<?= $RLverb ?>"/></label>
		<label><input type="hidden" name="LRlem" value="<?= $LRlem ?>"/></label>
		<label><input type="hidden" name="RLlem" value="<?= $RLlem ?>"/></label>
		<label><input type="hidden" name="mostrar" value="<?= $show ?>"/></label>
		<label><input type="hidden" name="antParadLR" value="<?= $paradigmaLR ?>"/></label>
		<label><input type="hidden" name="antParadRL" value="<?= $paradigmaRL ?>"/></label>
		<label><input type="hidden" name="usuario" value="<?= $user ?>"/></label>
		<label><input type="hidden" name="tipo" value="<?= $tipo ?>"/></label>	

<?
if ($valida==1)  //si se esta validando se pone un botón para borrar la palabra actual
{
?>
		<label>
		  <input type="submit" name="acc" value="Delete" />
		<label>
		<br/><br/>
<?
}
?>
		
		
		<label>
	  	  <input type="button" 
		  	 name="acc" 
			 value=" - Close - "  
			 onclick="cierra()" />
		</label>		
		<label>
	  	  <input type="submit" 
		  	 name="acc" 
			 value="Go on >>"  
			 onclick=" return compruebaParad(this.form)" />
		</label>
		<br/><br/>
		
<!--		
<?
$userAct=$REDIRECT_REMOTE_USER;
if (in_array($userAct, $usuaris_professionals))
{
if($valida!=1)
		echo "\t\t<label><a href=\"valida.php?RL=$RL&LR=$LR\">Validate pairs</a></label>\n";
		echo "\t\t<br/><br/>\n";
}
?>   
-->  	
	</fieldset>
      </form>
    </div>


 <!-- ##### Footer ##### -->

    <div id="footer">
 <!-- <div class="doNotPrint">
        <a href="./index.html">Tell a Friend</a> |
        <a href="./index.html">Privacy Policy</a> |
        <a href="./index.html">Site Map</a> |
        <a href="./index.html">Feedback</a> |
        <a href="./index.html">Help</a>
      </div>
   -->
      <div><a href="http://www.dlsi.ua.es">
        Departament de Llenguatges i Sistemes Informàtics</a>, Universitat
        d'Alacant, 2006 | Last modified 20.02.2006.<!-- by <a
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
			$creasemi="awk '$1!=\"$LRlem\" && $2!=\"$RLlem\"{print $0}' $tmpfname > $fich_semi";	
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
		
			include("ins_multip.php"); //se inserta en el fichero validado
		}
		else
	
			include("ins_multip.php");  //se inserta también en el fichero validado
	}

}// final else

?>


