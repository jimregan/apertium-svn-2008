<?php

//to set the pair of languages in which new words are going to be added: es-ca, es-gl, etc.  

  switch ($nomtrad){
  
  case "es-ca": $LR="es"; $RL="ca"; break;
  case "es-pt": $LR="es"; $RL="pt"; break;
  case "oc-ca": $LR="oc"; $RL="ca"; break;
  case "fr-ca": $LR="fr"; $RL="ca"; break;
  case "es-ro": $LR="es"; $RL="ro"; break;
  case "fr-es": $LR="fr"; $RL="es"; break;  
  default: $LR="es"; $RL="ca"; break;
}

   
?>

<?php

//only registered users  (at the present moment) can write directly on the professional dictionaries;
//other users will wrote on a semi-professional ones to be validated 

$user=$REDIRECT_REMOTE_USER;

//depending on the PoS chosen by the user, the webform will show selecc.php or multip.php
//if no PoS has been set, it will be set "name"
//all these "if" can be turned into switch-case?

 if($_POST['cat']=="Verb")
  {
    $tipo=vb;
    include("selecc.php");
    exit;
  }

 if($_POST['cat']=="Noun")
  {
    $tipo=n;
    include("selecc.php");
    exit;
  }

 if($_POST['cat']=="Adjective")
  {
    $tipo=adj;
    include("selecc.php");
    exit;
  }
    

 if($_POST['cat']=="Prep")
  {
    $tipo=prep;
    include("selecc.php");
    exit;
  }
  
  
 if($_POST['cat']=="Adverb")
  {
    $tipo=adv;
    include("selecc.php");  

    exit;
  }
  
 if($_POST['cat']=="Cnjadv")
  {
    $tipo=cnjadv;
    include("selecc.php");  

    exit;
  }
    
  if($_POST['cat']=="Acr")
  {
    $tipo=acr;
    include("selecc.php");
    exit;
  }
  
   if($_POST['cat']=="Pname")
  {
    $tipo=pname;
    include("selecc.php");
    exit;
  }
  
  if($_POST['cat']=="MW")
  {
    $tipo=mw;
    include("multip.php");
    exit;
  }
  
   if($_POST['cat']=="")
  {
     $tipo=n;
     include("selecc.php");
     exit;
  }
  
 								     

?>
