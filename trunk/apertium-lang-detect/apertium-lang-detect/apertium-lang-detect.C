/*
 * Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <apertium-lang-detect/apertium-lang-detect-config.h>
#include <dirent.h>
#include <list>
#include <string>

using namespace std;




#define NR_FIX_PARAM		2	//numero de parametros fijos en la llamada
#define NR_MAX_LANG		300	//numero maximo de lenguas con que puede trabajar

#define LONG_LANG_CODE		8	//tamaño maximo del código de lengua
#define LONG_BUFFER		200000	//tamaño del buffer para almacenar parrafos extranjeros
#define CORRELATION_MIN		0.43	//correlacion mínima para aceptar la decisión del identificador de idiomas
#define CORRELATION_DIFF	0.007	//diferencia de correlacion entre 1º y 2º para considerarse relevante	

struct resultado{
	float quality;
	char language[LONG_LANG_CODE];
};


extern "C" {
	int mguesser_init(int argc, char *argv[], int nfiles, char *files[]);
	int mguesser(FILE *buffer, int num, struct resultado *resultados);
	int mguesser_close();
} //para poder enlazar C con C++, las funciones en C deben ir aquí.




struct Ltstr
{
  bool operator()(string const & s1, string const &s2)
    {
      //return strcmp(s1.c_str(), s2.c_str()) > 0;
      return s1 > s2;
    }  
};


enum TokenType
{
  blank,
  word,
  eof
};

struct Token
{
  TokenType type;
  string value;
};

bool endsWith(string const &s1, string const &s2)
{
  if(s1.size() < s2.size())
  {
    return false;
  }
  else if(s1.substr(s1.size() - s2.size()) != s2)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool beginsWith(string const &s1, string const &s2)
{
  if(s1.size() < s2.size())
  {
    return false;
  }
  else if(s1.substr(0, s2.size()) != s2)
  {
    return false;
  }
  else
  {
    return true;
  }
}

list<string> readFiles(string const &directory_name)
{
  struct dirent **namelist;
  int nfiles = scandir(directory_name.c_str(), &namelist, NULL, alphasort);
  if(nfiles < 0)
  {
    cerr << "Error: Cannot scan " << directory_name << " directory to read data." << endl;
    exit(EXIT_FAILURE);
  }

  list<string> result;
  while(nfiles--)
  {
    result.push_front(namelist[nfiles]->d_name);
    free(namelist[nfiles]);
  }
  free(namelist);

  return result;
}

list<string> filterFilesEndingWith(list<string> const &files, string const &ending)
{
  list<string> retval;

  for(list<string>::const_iterator it = files.begin(), limit = files.end();
      it != limit; it++)
  {
    if(endsWith(*it, ending))
    {
      retval.push_back(*it);
    }
  }
  
  return retval;
}

list<string> filterFilesBeginningWith(list<string> const &files, string const &ending)
{
  list<string> retval;

  for(list<string>::const_iterator it = files.begin(), limit = files.end();
      it != limit; it++)
  {
    if(beginsWith(*it, ending))
    {
      retval.push_back(*it);
    }
  }
  
  return retval;
}


void readText(FILE *input, Token &t)
{
  t.type = word;
  while(true)
  {
    int value = fgetc_unlocked(input);
    if(feof(input))
    {
      return;
    }
    else if(value == '\\')
    {
      t.value += '\\';
      value = fgetc_unlocked(input);
      if(feof(input))
      {
        return;
      }
      else
      {
        t.value += static_cast<char>(value);
      }
    }
    else if(value == '[')
    {
      ungetc(value, input);
      return;
    }
    else if(value == ' ')
    {
      ungetc(value, input);
      return;
    }
    else
    {
      t.value += static_cast<char>(value);
    }
  }
}

void readBlock(FILE *input, Token &t)
{
  t.type = blank;
  t.value = "[";
  while(true)
  {
    int value = fgetc_unlocked(input);
    if(feof(input))
    {
      t.type = eof;
      return;
    }
        
    if(value == '\\')
    {
      t.value += '\\';
      value = fgetc_unlocked(input);
      t.value += static_cast<char>(value);
    }
    else if(value == ']')
    {
      t.value += ']';
      return;
    }
    else
    {
      t.value += static_cast<char>(value);
    }
  }
}

void readToken(FILE *input, Token &t)
{
  if(feof(input))
  {
    t.type = eof;
    t.value = "";
    return;
  }
  int value = fgetc_unlocked(input);
  if(feof(input))
  {
    t.type = eof;
    t.value = "";
    return;
  }


  switch(value)
  {
    case ' ':
      t.type = blank;
      t.value = " ";
      return;
      
    case '[':
      t.value = "[";
      readBlock(input, t);
      return;
      
    default:
      t.value = string("") + static_cast<char>(value);
      readText(input, t);
      return;
  } 
}

bool good(Token const &t)
{
  if(t.type == eof && t.value != "")
  {
    return false;
  }
  else
  {
    return true;
  }
}


list<string> checkCommand(int argc, char *argv[], FILE **input, FILE **output,
                  char* lang, map<string, float, Ltstr> *mapa_words)
{
  int nr_lg=0;

  FILE *f;
  char word[100];

  char system_call[100];
  char lenguas[NR_MAX_LANG][LONG_LANG_CODE];

  *input = stdin;
  *output = stdout;
  

  if(argc<NR_FIX_PARAM){
    cerr << "USAGE: " << basename(argv[0]);
    cerr << " source_language_code (other_languages_codes)" << endl;
    exit(EXIT_FAILURE);
  }


  list<string> files = filterFilesEndingWith(readFiles(ALL_MAPS_DIR), ".lm");

  if(argc==NR_FIX_PARAM)
  {
     files = filterFilesEndingWith(files, ".lm");
  }
  else{
    while( nr_lg < argc-NR_FIX_PARAM){
	strcpy(lenguas[nr_lg], argv[nr_lg+NR_FIX_PARAM]);
	nr_lg++;
    }

    list<string> accum;
    for(int i=0;i<nr_lg;i++){
        list<string> tmp = filterFilesBeginningWith(files, lenguas[i]);
        
        accum.insert(accum.end(), tmp.begin(), tmp.end());
    }
  }

  //lee el fichero con las palabras de clases reservadas y crea un mapa con ellas
  strcpy(system_call,INDICADORES_DIR);
  strcat(system_call,"/indicador.word_");
  strcat(system_call,lang);
  f=fopen(system_call,"r");
  if (f!=NULL){
     while(!feof(f)){
 	fscanf(f,"%s", word);
	for(int i = 0; word[ i ]; i++)  word[i] = tolower(word[ i ]);  //convert to lower case
	(*mapa_words)[word] = 1;
     }
	fclose(f);
  }
  else{
  	cerr << "Error: Permission denied for reading the file '";
  	cerr << system_call << "'." << endl;
  }

  return files;
}


//***** Funciones para Evaluación de la Lengua ****************************
//*************************************************************************

float EvaluaPalabras(FILE *f, map<string, float, Ltstr> *mapa_words){
	char word[1000];
	unsigned long int cont=1;
	float acum=0;
	map<string, float>::iterator it;

//	scanf(buffer,"%s",&word);


	// recoje las palabras del buffer y
	// comprueba una a una si están en el mapa de palabras
	if (f!=NULL){
	    while(!feof(f)){
		fscanf(f,"%s", word);
		if(strlen(word)<20){
			it = (*mapa_words).find(word);
//        	cout << word << (it == (*mapa_words).end() ? "not " : " ") << "found\n";
			acum += it -> second;
			cont++;
		}

	    }
    	}

	//devuelve la suma entre el numero de palabras
	return acum/cont;
}

int Evalua(char *buffer, map<string, float, Ltstr> *mapa_words, char *evaluated_lang, char *lang, int argc, char *argv[], int nfiles, char *files[]){
	float eval_words;
	FILE *f;
//	char name[10];
//  	char system_call[100];
	struct resultado resultados[200];

	string buff = buffer;

	//elimina [] del buffer a analizar
        string::size_type idx = 0;
        while (true)
        {
             idx = buff.find("[", idx); 
             if (idx == string::npos)
                 break;
             buff.replace(idx, 1, " ");
             idx += 1;     // don't change the replacement
         }
        while (true)
        {
             idx = buff.find("]", idx); 
             if (idx == string::npos)
                 break;
             buff.replace(idx, 1, " ");
             idx += 1;     // don't change the replacement
         }

	//pasa el buffer al identificador de idioma para detecta el idioma del parrafo
//	strcpy(name,"XXXXXX");
//	f=fdopen(mkstemp(name),"rw+");
	f=tmpfile();
	fprintf(f,"%s",buff.c_str());
	fseek(f,0,SEEK_SET);
	mguesser_init(argc, argv, nfiles, files);
	mguesser(f, 4, resultados);
	fseek(f,0,SEEK_SET);
	eval_words=EvaluaPalabras(f,mapa_words);
	fclose(f);



strcpy(evaluated_lang,resultados[0].language);
//cerr<<"Buffer= "<<buff<<" lang="<<evaluated_lang<<" correl= "<<resultados[0].quality<<"("<<CORRELATION_MIN<<")" <<" EP="<<eval_words<<endl;

// ---   HEURISTICA para la selección del idioma del buffer  ----

//	if(evaluated_lang==lang) correlation=0.3;
//	ALFA*correlation - BETA*eval_words > X   extranjero; sino propio;

	if(  resultados[0].quality < CORRELATION_MIN)  strcpy(evaluated_lang, lang);	// si no llega al mínimo, mantiene la leng. original

	if(   ( resultados[0].quality < CORRELATION_MIN || (resultados[0].quality - resultados[1].quality) < CORRELATION_DIFF ) 
		&& eval_words > 0.05 )  
		strcpy(evaluated_lang, lang);
	
	return EXIT_SUCCESS;
}


int MarcadorInicio(Token t, int activated_eval){
	int x=0;

    	if (	(t.value.c_str())[0]=='«'  || (t.value.c_str())[t.value.length()-1]=='«'  ||
    		(t.value.c_str())[1]=='«'  || (t.value.c_str())[t.value.length()-2]=='«'  )
		x=1;
	if(  (	(t.value.c_str())[0]=='\"' || (t.value.c_str())[t.value.length()-1]=='\"' || 
    		(t.value.c_str())[1]=='\"'  || (t.value.c_str())[t.value.length()-2]=='\"'  
     	     )  && !activated_eval)	
		x=1;

	return x;
}

int MarcadorFinal(Token t, int activated_eval, Token t_anterior){
	int x=0;


	if( t.type == blank && t.value.length()>30){
		x=1;
		cerr<<"blank="<<t.value;
	}
	if( t.type == blank && /* t.value.length()<3 && */ t_anterior.value==".")
		x=1;
	if( (t.value.c_str())[0]=='.' || (t.value.c_str())[t.value.length()-1]=='.') 
		x=1;
	if( (t.value.c_str())[0]==';' || (t.value.c_str())[t.value.length()-1]==';') 
		x=1;
	if( (t.value.c_str())[0]==':' || (t.value.c_str())[t.value.length()-1]==':') 
		x=1;
   	if (	(t.value.c_str())[0]=='»'  || (t.value.c_str())[t.value.length()-1]=='»'  ||
    		(t.value.c_str())[1]=='»'  || (t.value.c_str())[t.value.length()-2]=='»'  )	
		x=1;
	if(  ( 	(t.value.c_str())[0]=='\"' || (t.value.c_str())[t.value.length()-1]=='\"' || 
    		(t.value.c_str())[1]=='\"'  || (t.value.c_str())[t.value.length()-2]=='\"'  
	     )	&& activated_eval)
		x=1;
	
	return x;
}





//*************************************************************************

int main(int argc, char *argv[])
{
  FILE *input, *output;
  char *lang = argv[1];
  char* evaluated_lang = new char [LONG_LANG_CODE];

  map<string, float, Ltstr> mapa_words;

  int activated_eval=0;
  char *buffer=(char*)malloc(sizeof(char)*LONG_BUFFER);

  list<string> file_list =  checkCommand(argc, argv, &input, &output, argv[1], &mapa_words);
  
  Token t, t_anterior;

  buffer[0]='\0';

  char **files = (char **) malloc(sizeof(char*)*file_list.size());
  int idx = 0;
  for(list<string>::iterator it = file_list.begin(); it != file_list.end(); it++, idx++)
  {
    files[idx] = strdup(it->c_str());
  }

// Inicializamos el mguesser
  mguesser_init(argc, argv, file_list.size(), files);
  int const nfiles = file_list.size();  

  while(true)
  {
    t_anterior=t;

    readToken(input, t);
    if(t.type == eof)
    {
      if(!good(t))
      {
        cerr << "Warning: the file ends unexpectedly." << endl;
      }
      break;
    }



    // ----- comprueba si hay algun posible cambio de idioma -------------
      if ( MarcadorInicio(t,activated_eval) ){
	Evalua(buffer,&mapa_words,evaluated_lang,lang, argc, argv, nfiles, files);
	if(strcmp(lang,evaluated_lang)) cout <<"["<<buffer<<"]";
	else				cout <<buffer;
	activated_eval=1;
	buffer[0]='\0';
	strcat(buffer,t.value.c_str());
      }
      else if ( MarcadorFinal(t,activated_eval,t_anterior) ){
	if(t.type == blank){
		Evalua(buffer, &mapa_words,evaluated_lang,lang, argc, argv, nfiles, files);				//evalua el buffer
      		if(strcmp(lang,evaluated_lang)) cout <<"["<<buffer<<"]";	//si es lengua distinta
                else				cout <<buffer;			//si es misma lengua
		cout << t.value;
      		buffer[0]='\0';
	}
	else{
		activated_eval=0;						//desactiva la evaluacion y almacnamiento en buffer
		buffer=strcat(buffer,t.value.c_str());     			//añade el item actual al buffer
		Evalua(buffer,  &mapa_words, evaluated_lang,lang, argc, argv, nfiles, files);				//evalua el buffer
		if(strcmp(lang,evaluated_lang)) cout <<"["<<buffer<<"]";	//si es lengua distinta
		else				cout <<buffer;			//si es misma lengua
		buffer[0]='\0';
	}
      }
      else{ 
		strcat(buffer,t.value.c_str());
      }
      // -------------------------------------------------------------------

  }//end while

/* liberamos los mapas para el mguesser*/
mguesser_close();
  
  for(unsigned int i = 0; i < file_list.size(); i++)
  {
    free(files[i]);
  }
  
  free(files);


  return EXIT_SUCCESS;
}

