<?php

  function error($mensaje){
?>
   <html>
    <head><title>apertium - Error</title>
    </head>
    <body><center>
<p><h1><font face="Verdana,Arial,Helvetica,Sans-serif" size="6"> Error:
<?php echo($mensaje);?></font></h1></p>
</center>
</body>
</html>
<?php
  }

  function mensaje($mensaje){
?>
   <html>
    <head><title>apertium - Error</title>
    </head>
    <body><center><p><h1><font face="Verdana,Arial,Helvetica,Sans-serif" size="6">
<?php echo($mensaje);?></font></h1></p>
</center>
</body>
</html>
<?php
  }

  function peticion_correcta($mensaje_error){
    if(getenv("REQUEST_METHOD")!="POST"){
      error($mensaje_error);
      exit;
    }
  }




  function vacio($cadena, $mensaje_error){
    if(strlen($cadena)==0){
      error($mensaje_error);
      exit;
    }
  }

function es_email($correu, $mensaje_error){
  //comprobar que efectivamente es una direccion de correo electronico
  if(preg_match("/^([\w\d-\.]+\@[\w\d-\.]+[ ]*,[ ]*)*[\w\d-\.]+\@[\w\d-\.]+$/",$correu));
  else{
    error($mensaje_error);
    exit;
  }
}

function limite($cad,$lim,$mensaje_error){
  if(strlen($cad)>$lim){
    error($mensaje_error);
    exit;
  }
}

?>
