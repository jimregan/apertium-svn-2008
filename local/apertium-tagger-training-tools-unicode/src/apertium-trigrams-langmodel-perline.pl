######################################################################
# Copyright (C) 2007 Felipe Sánchez-Martínez
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
# Trigram language model used to estimate the likelihood of each
# translation peroduce when training a part-of-speech tagger using
# information from the target language.
#
# For a deeper description on how the method works read the paper:
#
# Exploring the use of target-language information to train the
# part-of-speech tagger of machine translation systems.  By Felipe
# Sánchez-Martínez, Juan Antonio Pérez-Ortiz and Mikel L. Forcada.
# In Lecture Notes in Computer Science 3230 (Advances in Natural
# Language Processing, Proceedings of EsTAL - España for Natural
# Language Processing), p. 137-148, October 20-22, 2004, Alicante,
# Spain.  
# © Springer-Verlag Berling Heidelberg 2004
# http://www.dlsi.ua.es/~fsanchez/pub/pdf/sanchez04b.pdf
#
#  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
######################################################################


use strict;
use warnings;

# Getting command line arguments:
use Getopt::Long;
# Documentation:
use Pod::Usage;
# I/O Handler
use IO::Handle;

use locale;
use POSIX qw(locale_h);
 
print STDERR "LOCALE: ", setlocale(LC_ALL,""), "\n";

# Get a reference to a hash of locale-dependent info
#my $locale_values = localeconv();
#      
# Output sorted list of the values
#for (sort keys %$locale_values) {
#  printf "%-20s = %s\n", $_, $locale_values->{$_}
#}

#Vars for the command line options
my($train, $eval, $help, $input, $man, $debug, $fastload, $fifoin, $fifout);

#Global vars
my %word2id;
my @id2word;

my(%trigrams, %bigrams, %monograms);
my $count;
my %resSGT; #Hash to store the result of applicating the Simple Good-Turing method, 
            #the key will be the frequency and the value the ptobabvlity


# Command line arguments
GetOptions( 'train|t'              => \$train,
            'eval|e'               => \$eval,
            'help|h'               => \$help,
            'man|m'                => \$man,            
            'input|i=s'            => \$input,
            'debug|d'              => \$debug,
            'fastload|f'           => \$fastload,
            'fifoin|fi=s'          => \$fifoin,
            'fifout|fo=s'          => \$fifout,            
          ) || pod2usage(2);

pod2usage(2) if $help;
pod2usage(1) if $man;
pod2usage(2) unless ($train or $eval);
pod2usage(2) if ($train and $eval);
pod2usage(2) unless $input;
if((($fifoin)&&(!$fifout))||((!$fifoin)&&($fifout))) {
   print STDERR "Error: -fifout and -fifoin must be used in conjunction.\n";
   print STDERR "Read the manual (trigrams.pl -m) before using these options.\n";
   pod2usage(2);
}

binmode STDOUT, ":utf8";
binmode STDIN, ":utf8";
binmode STDERR, ":utf8";
binmode DATA, ":utf8";


unless (open(INPUT, "<:utf8", $input)) {
   print STDERR "Error: Cannot open input file \'$input\': $!\n"; 
   exit 1;
}

&do_train if $train;

if ($eval) {
   &cargar_modelo;
   close INPUT;   

   if ($fifoin) {
      while(1) { #Input(output will be done through named pipes, this process 
                 #will be killed from outside 
         unless (open(TXTINPUT, "<:utf8", $fifoin)) {
            print STDERR "Error: Cannot open input file \'$fifoin\': $!\n"; 
            exit 1;
         }   
         unless (open(TXTOUTPUT, ">:utf8", $fifout)) {
            print STDERR "Error: Cannot open output file \'$fifout\': $!\n"; 
            exit 1;
         }   
         &do_eval;
         close TXTINPUT;
         close TXTOUTPUT;
      }
   } else {
      *TXTINPUT=*STDIN;
      *TXTOUTPUT=*STDOUT;
       &do_eval;
   }
}

sub get_word_id {
   my($palabra) = @_;
   
   if (defined($word2id{$palabra})) {
      #print STDERR "Id of word '$palabra' is " . $word2id{$palabra} . "\n";
      return $word2id{$palabra};
   } else {
     push @id2word, $palabra;
     $word2id{$palabra}=$#id2word;
     #print STDERR "Registering word '$palabra' id is " . $word2id{$palabra} . "\n";
     return $word2id{$palabra};
   }
}

sub do_train {
   print STDERR "Training";
   
   my(@palabras, $palabra, $npalabras);
   
   my(@un_trigrama, $cad_trigrama);
   my(@un_bigrama, $cad_bigrama);
   
   my($ntrigrams, $nbigrams, $nmonograms);

   my $eos=get_word_id(".");
   #We start seen end-of-sentence marks (points)
   @un_bigrama=($eos);
   @un_trigrama=($eos,$eos);
   $palabra="";
      
   $monograms{$eos}++;;
   $bigrams{"$eos $eos"}++;
   $count++;
   
   while(<INPUT>) {
      &prepara_entrada;
      
      @palabras = split /\s+/;
      foreach $palabra (@palabras) {                  
         next unless(length($palabra)); 
        
	 $palabra = &get_word_id($palabra);
	 
         print STDERR "." if ((($npalabras++)%5000)==0); 
         
         push @un_trigrama, $palabra;         
         push @un_bigrama, $palabra;
         
         if (@un_trigrama>3) {         
            shift @un_trigrama;
         } 
         
         if (@un_bigrama>2) {
            shift @un_bigrama;
         }
                  
         $cad_trigrama="@un_trigrama";
         $cad_bigrama="@un_bigrama";
         
         $trigrams{$cad_trigrama}++;
         $bigrams{$cad_bigrama}++;
         $monograms{$palabra}++;
         $count++;         
      }
   }
        
   $ntrigrams = keys %trigrams;
   $nbigrams = keys %bigrams;
   $nmonograms = keys %monograms;
   
   print STDERR "\n$npalabras processed words.\n";
   print STDERR "$ntrigrams distinct trigrams found.\n";
   print STDERR "$nbigrams distinct bigrams found.\n";
   print STDERR "$nmonograms distinct monograms found.\n";

   &SGT; #Aplicar SGT sobre la distribución de 
         #probabilidad de los unigramas (para tratar bien las desconocidas)
   &volcar_modelo;   
}

sub do_eval {
   print STDERR "Listo. (Ctr-D/Ctr-C para terminar)\n"; 
   print STDERR "\$ ";
   
   #Procedemos al cálculo de la verosimilitud, procesamos la entrada estándar
   #Evaluamos toda la entrada hasta el final de fichero, tras los cual
   #volcamos la verosimilitud calculada.
   
   my($prob_bigrama, $prob_trigrama, $prob_acumulada);
   my(@palabras, $palabra, @un_trigrama, @un_bigrama, $cad_trigrama);
   my($val_lambda2, $val_lambda3, $frec_bigrama, $frec_trigrama, $frec_ungrama);
   my(@aux_trigrama, $auxcad_bigrama);

   
   while(<TXTINPUT>) {
      &prepara_entrada; 
      
      if ($debug) {
         print STDERR "TEXTO: $_\n";
      }

      #No esperamos a tener un trigrama completo
      #Primero contemplamos el unigram, luego el bigrama y así hasta el final

      #Inicializar varibles
      $prob_acumulada=1;
      @un_bigrama=();
      @un_trigrama=(); 
      $palabra="";
   
      my($cuentaletras);
      $cuentaletras=0;
      
      @palabras = split /\s+/;
      foreach $palabra (@palabras) {
         
         next unless(length($palabra)); #Por si la cadena está vacía (pasa, no debiera por el split, pero...)
         
	 $cuentaletras+=length($palabra);
	 
	 $palabra = &get_word_id($palabra);
	          
         push @un_trigrama, $palabra;         
         push @un_bigrama, $palabra;

         if (@un_trigrama>3) {         
            shift @un_trigrama;
         } 
         
         if (@un_bigrama>2) {
            shift @un_bigrama;
         }

         if (@un_trigrama==1) {
	    $prob_acumulada=$prob_acumulada*&prob1($palabra);
            if ($debug) {
               print STDERR "   prob1($palabra)=", &prob1($palabra), "\n";	    
	    }
	 } elsif (@un_trigrama==2) {
	    $prob_acumulada=$prob_acumulada*&prob2($palabra, @un_bigrama);
	 } else {                                   
            $cad_trigrama="@un_trigrama";
   
            $prob_bigrama = &prob2($palabra, @un_bigrama); ##$val_lambda2*$frec_bigrama + (1-$val_lambda2)*$frec_ungrama;
                         
            @aux_trigrama=@un_trigrama; 
            pop @aux_trigrama;
            $auxcad_bigrama="@aux_trigrama"; #(w1 w2)
 
            $val_lambda3=&lambda(&datos_ngrama($auxcad_bigrama,2)); #lambda 3 (se basa en w1 w2)
            $frec_trigrama=&frec3(@un_trigrama);
                                   
            $prob_trigrama = $val_lambda3*$frec_trigrama + (1-$val_lambda3)*$prob_bigrama;

            $prob_acumulada=$prob_acumulada*$prob_trigrama;
         
            if ($debug) {
               print STDERR "   frec3($cad_trigrama)=$frec_trigrama\n";
               print STDERR "   lambda3($auxcad_bigrama)=$val_lambda3\n";
               print STDERR "   P($cad_trigrama)=$prob_trigrama\n";         
            }         
         }
      }      
      print TXTOUTPUT $prob_acumulada, "\n"; #|$cuentaletras";
   } 
   
   
   print STDERR "\$ ";
   print STDERR "Fin.\n";
}   

sub volcar_modelo {
   my($key, $value);

   print "$count\n";
   
   print STDERR "Writing word2id information ...\n";
   printf "-//--WORD2ID--//-\n";
   while (($key, $value)= each %word2id) {
      print "$key\t$value\n";
   }
   print STDERR "Writing Simple Good-Turing model for 1-grams ...\n";
   printf "-//--SGT--//-\n";
   while (($key, $value)= each %resSGT) {
      print "$key\t$value\n";
   }

   print STDERR "Writing 1-grams ...\n";
   print "-//--MONOGRAMAS--//-\n";
   while (($key, $value)= each %monograms) {
      print "$key\t$value\n";
   }

   print STDERR "Writing 2-grams...\n";      
   print "-//--BIGRAMS--//-\n";
   while (($key, $value)= each %bigrams) {   
      print "$key\t$value\n";
   }

   print STDERR "Writing 3-grams...\n";   
   print "-//--TRIGRAMS--//-\n";
   while (($key, $value)= each %trigrams) {      
      print "$key\t$value\n";
   }   
}

sub cargar_modelo {
   my ($longgrama, @line);
      
   while(<INPUT>) {
      chomp;
      
      if ($_ eq "-//--MONOGRAMAS--//-") {
         print STDERR "Reading 1-grams ...\n";
         $longgrama=1;
         next;
      } elsif ($_ eq "-//--BIGRAMS--//-") {
         print STDERR "Reading 2-grams ...\n";      
         $longgrama=2;
         next;
      } elsif ($_ eq "-//--TRIGRAMS--//-") {
         print STDERR "Reading 3-grams ...\n";
         $longgrama=3;
         next;
      } elsif ($_ eq "-//--SGT--//-") {
         print STDERR "Reading Simple Good-Turing model for 1-grams ...\n";
         $longgrama=-99;
	 next;
      } elsif ($_ eq "-//--WORD2ID--//-") {
         print STDERR "Reading word2id information ...\n";
	 $longgrama=-199;
	 next;
      } elsif (!defined($count)) {
         $count=$_;
         next;
      }
      
      return if (($fastload) and ($longgrama>0)); #Ya se ha leido la cuenta y el modelo SGT
               
      @line = split /\t/;
      
      if ($longgrama==1) {
         $monograms{$line[0]}=$line[1];         
      } elsif ($longgrama==2) {
         $bigrams{$line[0]}=$line[1];
      } elsif ($longgrama==3) {
         $trigrams{$line[0]}=$line[1];
      } elsif ($longgrama==-99) {
         $resSGT{$line[0]}=$line[1];
      } elsif ($longgrama==-199) {
         $word2id{$line[0]}=$line[1];
	 $id2word[$line[1]]=$line[0];
      }
   }
   print STDERR $count, "\n";
}

sub prepara_entrada { 
   chomp;

   #Eliminamos comillas dobles
   s/["»«]//g;

   #Sustituimos el apostrofo por un espacio (me cargo las apostrofaciones, pero tb los entrecomillados).
   s/[']/ /g;
   
   #Introducimos espacios antes de los signos de puntuación a principio y/o final de palabra
   s/([.,;:%¿?¡!()\[\]{}<>])(\p{IsWord}+)/$1 $2/g;
   s/(\p{IsWord}+)([.,;:%¿?¡!()\[\]{}<>])/$1 $2/g;

   #Pasamos a misnusculas 
   $_= lc;
   
   #Sustituir los número por un token que los represente
   s/\p{IsDigit}+/ --NUM-- /g;
   s/\p{IsDigit}+[.,]\p{IsDigit}+/ --NUM-- /g;

   #Eliminar espacios a principio y final de la cadena
   #s/^[ \t]+//g;
   #s/[ \t]+$//g;      
}

sub prob2 { #Probabilidad de un bigrama
   my($palabra, @un_bigrama) = @_;
   
   my($val_lambda2, $frec_bigrama, $prob_ungrama, $prob_bigrama);
      
   $val_lambda2=&lambda(&datos_ngrama($un_bigrama[0],1)); #lambda 2 (se basa en la primera palabra del bigrama)
   $frec_bigrama=&frec2(@un_bigrama);         
   $prob_ungrama=&prob1($palabra);
   
   $prob_bigrama=$val_lambda2*$frec_bigrama + (1-$val_lambda2)*$prob_ungrama;
   
   if ($debug) {
      my $cad_bigrama="@un_bigrama";
      print STDERR "   prob1($palabra)=$prob_ungrama\n";
      print STDERR "   frec2($cad_bigrama)=$frec_bigrama\n";
      print STDERR "   lambda2(", $un_bigrama[0], ")=$val_lambda2\n";
      print STDERR "   P($cad_bigrama)=$prob_bigrama\n";
   }         
                        
   return $prob_bigrama;
}

sub lambda {
   my($valor)=@_;
     
   my($val_sqrt);
   $val_sqrt=sqrt($valor);
   
   return $val_sqrt/($val_sqrt+1);   
}

sub prob1 { #Probabilidad de una palabra, hace uso del modelo SGT
   my($palabra)=@_;
   
   return $resSGT{&datos_ngrama($palabra,1)};
}

sub frec2 {
   my @un_bigrama=@_;
   
   my($cuenta_bigrama, $cuenta_palabra, $cad_bigrama);
   
   $cad_bigrama="@un_bigrama";
   
   $cuenta_bigrama=&datos_ngrama($cad_bigrama, 2); #w2, w3
   $cuenta_palabra=&datos_ngrama($un_bigrama[0], 1); #w2
   
   if ($cuenta_palabra>0) {
      return $cuenta_bigrama/$cuenta_palabra;   
   } else {
      return 0;
   }
}

sub frec3 {
   my @un_trigrama=@_;
   
   my($cuenta_trigrama, $cuenta_bigrama, $cad_bigrama, $cad_trigrama);
   
   $cad_trigrama="@un_trigrama";
   pop @un_trigrama;
   $cad_bigrama="@un_trigrama";
   
   $cuenta_trigrama=&datos_ngrama($cad_trigrama, 3); #w1, w2, w3
   $cuenta_bigrama=&datos_ngrama($cad_bigrama, 2); #w1, w2
   
   if ($cuenta_bigrama>0) {
      return $cuenta_trigrama/$cuenta_bigrama;
   } else {
      return 0;
   }
}

sub datos_ngrama {
   my($ngrama, $tipograma)=@_;
   my $valor_retorno;
   my @aux;

   if (!$fastload) {
      if ($tipograma==1) { #Monogramas
         $valor_retorno=$monograms{$ngrama};
      } elsif ($tipograma==2) { #Bigrams
         $valor_retorno=$bigrams{$ngrama};
      } else { #Trigrams
         $valor_retorno=$trigrams{$ngrama};
      }
      $valor_retorno=0 unless(defined($valor_retorno));
      return $valor_retorno;
   } else {   
      #Vemos si ya los hemos recuperado con anterioridad
      if (($tipograma==1) and (defined($monograms{$ngrama}))) { #Monogramas
         return $monograms{$ngrama};
      } elsif (($tipograma==2) and (defined($bigrams{$ngrama}))) { #Bigrams
         return $bigrams{$ngrama};
      } elsif (defined($trigrams{$ngrama})) { #Trigrams
         return $trigrams{$ngrama};
      }
   
      #Vamos a por el valor al fichero de entrada
      $_=$ngrama;
      s/([^\s]+)/\\\\$1/g;
      $_=`egrep "^($_)\t[0-9]+" $input 2>/dev/null`;
   
      if (length($_)==0) {
         $valor_retorno=0;
      } else {
         @aux = split/\s+/;
         $valor_retorno=pop @aux;
      }
      
      #Los guardamos por si los volvemos a necesitar
      if ($tipograma==1) { #Monogramas
         $monograms{$ngrama}=$valor_retorno;
      } elsif ($tipograma==2) { #Bigrams
         $bigrams{$ngrama}=$valor_retorno;
      } else { #Trigrams
         $trigrams{$ngrama}=$valor_retorno;
      }
      
      return $valor_retorno;
   }
}

sub SGT { #Realiza la estimación Simple Good-Turing (usa un programa externo);

   #Calcula r y N_r sobre las palabra para después aplicar Simple Good-Turing
   my %frec_frecs; #Indice del has: r; valor N_r
   my($palabra, $frecuencia);
   my $data=""; #Entrada para el programa que calcula SGT
      
   while (($palabra, $frecuencia)= each %monograms) {
      $frec_frecs{$frecuencia}++;
   }
     
   foreach $frecuencia (sort {$a <=> $b} keys %frec_frecs) {
      $data = $data.$frecuencia." ".$frec_frecs{$frecuencia}."\n";
   }
   
   my $sal=`echo \"$data\" | apertium-SGT`;
   
   my @SGT = split(/\n/,$sal);
   
   my ($linSGT,@aux);
   foreach $linSGT (@SGT) {
      @aux = split(/\t/, $linSGT);
      $resSGT{$aux[0]}=$aux[1];
   }
   
   #Al estimar la verosimilitud de una secuencia de texto a las palabras desconocidas se 
   #le asignará la probabilidad de todas las palabras desconocidas (no estimamos el número de
   #desconocidas). La probabilidad de una palabra desconocida será equiparable a la de una
   #palabra vista un unica vez
   $resSGT{0}=$resSGT{1};
}

__END__


=head1 NAME

trigrams - Trigram language model

=head1 SYNOPSIS

trigrams [options] 

 Options:
   -train|-t        train the language model from a raw input corpus 
                    (incompatible with -eval)
   -eval|-e         estimate the likelihood of an input sentence 
                    (incompatible with -train)
   -input|-i        specify the input file
   -help|-h         show this help
   -man|-m          show the manual page
   -debug|-d        show debug information while operating
   -fastload|-f     Do not load all the parameter at once
   -fifoin|-fi      Specify a named pipe for input
   -fifout|-fo      Specify a named pipe for output

Copyright (C) 2006 Felipe Sánchez-Martínez

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

=head1 OPTIONS

=over 8

=item B<-train>

Para entrenar el modelo a partir del fichero de entrada que deberá contener 
un corpus de la lengua a modelar. Tras calcular el modelo se vuelca por
la salida estándar. Esta opción es incompatible con -eval.

=item B<-eval>

Evaluar la verosimilitud del texto que se presente por la entrada estándar.
Se evaluará la verosimilitud de cada línea.
Esta opción es incompatible con -train.

=item B<-input>

Si -train contiene el fichero con el corpus a partir del cual debe realizarse
el entrenamiento.
Si -eval contiene el fichero con el modelo previamente calculado.

=item B<-help>

Mostrar mensaje de ayuda.

=item B<-man>

Mostrar este manual.

=item B<-debug>

Mostrar información de depuración

=item B<-fastload>

No cargar lo parámetros del modelo. Se recuperaran del fichero conforme se
vayan necesitando. Se eliminan los 15 segundo que puede tardar el modelo en
cargarse y se reduce drásticamente el consumo de memoria. 
A cambio la evaluación es un poco más lenta. 

=item B<-fifoin>

Esta opción permite especificar una tubería con nombre de la que leerá 
(en lugar de la entrada estandar). 
Además cuando se ejecute en este modo el programa permanecerá 
en un bucle indefinido leyendo de este fichero, de modo que cuando llegue el <EOF>
lo volverá abrir. Es util para acelerar su uso desde un programa externo.
Solo tiene sentido si -eval y -fifout. Obligatorio si -fifout.

=item B<-fifout>

Esta opción permite especificar una tubería con nombre sobre la que escribir
(en lugar de la salida estandar). 
Además cuando se ejecute en este modo el programa permanecerá en
en un bucle indefinido escribiendo en este fichero.
Es util para acelerar su uso desde un programa externo.
Solo tiene sentido si -eval  y -fifoin. Obligatorio si -fifoin.


=back
=head1 DESCRIPTION

Este programa se emplea para el cálculo de la verosimilitud de un fragmento
de texto y para el entrenamiento del modelo que sirve para el cálculo de 
dicha verosimilitud. 

ADVERTENCIA: El programa usa las 'locales' definidas por el usuario, éstas deben
ser acordes con la lengua de la que se desea estimar el modelo. En caso contrario
la conversión de mayúsculas a minúsculas no funcionará convenientemente.


Copyright (C) 2007 Felipe Sánchez-Martínez

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

Trigram language model used to estimate the likelihood of each translation 
peroduce when training a part-of-speech tagger using information from
the target language.

For a deeper description on how the method works read the paper:

Exploring the use of target-language information to train the
part-of-speech tagger of machine translation systems.  By Felipe
Sánchez-Martínez, Juan Antonio Pérez-Ortiz and Mikel L. Forcada.
In Lecture Notes in Computer Science 3230 (Advances in Natural
Language Processing, Proceedings of EsTAL - España for Natural
Language Processing), p. 137-148, October 20-22, 2004, Alicante,
Spain.  
© Springer-Verlag Berling Heidelberg 2004
http://www.dlsi.ua.es/~fsanchez/pub/pdf/sanchez04b.pdf

=cut
