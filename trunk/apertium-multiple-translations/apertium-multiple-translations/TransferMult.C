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
#include <apertium-multiple-translations/TransferMult.H>
#include <apertium/TRXReader.H>
#include <lttoolbox/Compression.H>
#include <lttoolbox/XMLParseUtil.H>

#include <cctype>
#include <iostream>
#include <stack>

using namespace std;

void
TransferMult::copy(TransferMult const &o)
{
}

void
TransferMult::destroy()
{
  if(me)
  {
    delete me;
    me = NULL;
  }
}

TransferMult::TransferMult()
{
  me = NULL;
  isRule = false;
  defaultAttrs = lu;
  numwords = 0;
}

TransferMult::~TransferMult()
{
  destroy();
}

TransferMult::TransferMult(TransferMult const &o)
{
  copy(o);
}

TransferMult &
TransferMult::operator =(TransferMult const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

string
TransferMult::tolower(string const &str) const
{
  string result = str;
  for(unsigned int i = 0, limit = str.size(); i != limit; i++)
  {
    result[i] = ::tolower(result[i]);
  }

  return result;
}


void 
TransferMult::readData(FILE *in)
{
  alphabet.read(in);
  any_char = alphabet(TRXReader::ANY_CHAR);
  any_tag = alphabet(TRXReader::ANY_TAG);

  Transducer t;
  t.read(in);
  
  map<int, int> finals;  
  
  // finals
  for(int i = 0, limit = Compression::multibyte_read(in); i != limit; i++)
  {
    int key = Compression::multibyte_read(in);
    finals[key] = Compression::multibyte_read(in);
  }  
  
  me = new MatchExe(t, finals);
 
  // attr_items

  // fixed attr_items
  attr_items["lem"] = "(([^<]|\"\\<\")+)";
  attr_items["lemq"] = "(#[ _][^<]+)";
  attr_items["lemh"] = "(([^<#]|\"\\<\"|\"\\#\")+)";
  attr_items["whole"] = ".+";
  attr_items["tags"] = "((<[^>]+>)+)";
  
  for(int i = 0, limit = Compression::multibyte_read(in); i != limit; i++)
  {
    int size_k = Compression::multibyte_read(in);
    char cad_k[size_k+1];
    fread(cad_k, sizeof(char), size_k, in);
    cad_k[size_k] = 0;
    
    int size_v = Compression::multibyte_read(in);
    char cad_v[size_v+1];
    fread(cad_v, sizeof(char), size_v, in);
    cad_v[size_v] = 0;
    
    attr_items[cad_k] = cad_v;
  }

  // variables
  for(int i = 0, limit = Compression::multibyte_read(in); i != limit; i++)
  {
    int size_k = Compression::multibyte_read(in);
    char cad_k[size_k+1];
    fread(cad_k, sizeof(char), size_k, in);
    cad_k[size_k] = 0;
    
    int size_v = Compression::multibyte_read(in);
    char cad_v[size_v+1];
    fread(cad_v, sizeof(char), size_v, in);
    cad_v[size_v] = 0;
    
    variables[cad_k] = cad_v;
  }

  // macros
  for(int i = 0, limit = Compression::multibyte_read(in); i != limit; i++)
  {
    int size_k = Compression::multibyte_read(in);
    char cad_k[size_k+1];
    fread(cad_k, sizeof(char), size_k, in);
    cad_k[size_k] = 0;
    
    macros[cad_k] = Compression::multibyte_read(in);
  }


  // lists
  for(int i = 0, limit = Compression::multibyte_read(in); i != limit; i++)
  {
    int size_k = Compression::multibyte_read(in);
    char cad_k[size_k+1];
    fread(cad_k, sizeof(char), size_k, in);
    cad_k[size_k] = 0;
    for(int j = 0, limit2 = Compression::multibyte_read(in); j != limit2; j++)
    {
      int size_v = Compression::multibyte_read(in);
      char cad_v[size_v+1];
      fread(cad_v, sizeof(char), size_v, in);
      cad_v[size_v] = 0;
      lists[cad_k].insert(cad_v);
      listslow[cad_k].insert(tolower(cad_v));
    }  
  }
}

void
TransferMult::readBil(string const &fstfile)
{
  FILE *in = fopen(fstfile.c_str(), "r");
  if(!in)
  {
    cerr << "Error: Could not open file '" << fstfile << "'." << endl;
    exit(EXIT_FAILURE);
  }
  fstp.load(in);
  fstp.initBiltrans();
  fclose(in);
}

void
TransferMult::read(string const &datafile, string const &fstfile)
{
  // datafile
  FILE *in = fopen(datafile.c_str(), "r");
  if(!in)
  {
    cerr << "Error: Could not open file '" << datafile << "'." << endl;
    exit(EXIT_FAILURE);
  }
  readData(in);
  fclose(in);
  
  readBil(fstfile);
}

TransferToken &
TransferMult::readToken(FILE *in)
{
  if(!input_buffer.isEmpty())
  {
    return input_buffer.next();
  }

  string content = "";
  while(true)
  {
    int val = fgetc_unlocked(in);
    if(feof(in))
    {
      return input_buffer.add(TransferToken(content, tt_eof));
    }
    if(val == '\\')
    {  
      content += '\\';
      content += char(fgetc_unlocked(in));
    }
    else if(val == '[')
    {
      content += '[';
      while(true)
      {
	int val2 = fgetc_unlocked(in);
	if(val2 == '\\')
	{
	  content += '\\';
	  content += char(fgetc_unlocked(in));
	}
	else if(val2 == ']')
	{
	  content += ']';
	  break;
	}
	else
	{
	  content += char(val2);
	}
      }
    }
    else if(val == '$')
    {
      return input_buffer.add(TransferToken(content, tt_word));
    }
    else if(val == '^')
    {
      return input_buffer.add(TransferToken(content, tt_blank));
    }
    else
    {
      content += char(val);
    }
  }
}

void
TransferMult::transfer(FILE *in, FILE *out)
{
  int last = 0;

  output = out;
  ms.init(me->getInitial());
  
  while(true)
  {
    if(ms.size() == 0)
    {
      if(isRule)
      {
	applyRule();
	isRule = false;
	input_buffer.setPos(last);
      }
      else
      {
	if(tmpword.size() != 0)
	{
	  pair<string, int> tr = fstp.biltransWithQueue(*tmpword[0], false);
	  if(tr.first.size() != 0)
	  {
	    vector<string> multiword = acceptions(tr.first);	    
	    for(unsigned int i = 0, limit = multiword.size(); i != limit; i++)
	    {
	      if(i > 0)
	      {
	        fputs_unlocked("[ | ]", output);
	      }
	      fputc_unlocked('^', output);
	      fputs_unlocked(multiword[i].c_str(), output);
	      fputc_unlocked('$', output);
	    }
	  }
	  tmpword.clear();
	  isRule = false;
	  input_buffer.setPos(last);
	  input_buffer.next();       
	  last = input_buffer.getPos();
	  ms.init(me->getInitial());
	}
	else if(tmpblank.size() != 0)
	{
	  fputs_unlocked(tmpblank[0]->c_str(), output);
	  tmpblank.clear();
	  last = input_buffer.getPos();
	  ms.init(me->getInitial());
	}
      }
    }
    int val = ms.classifyFinals(me->getFinals());
    if(val != -1)
    {
      isRule = true;
      numwords = tmpword.size();
      last = input_buffer.getPos();
    }

    TransferToken &current = readToken(in);
   
    switch(current.getType())
    {
      case tt_word:
	applyWord(current.getContent());
        tmpword.push_back(&current.getContent());
	break;

      case tt_blank:
	ms.step(' ');
	tmpblank.push_back(&current.getContent());
	break;

      case tt_eof:
	if(tmpword.size() != 0)
	{
	  tmpblank.push_back(&current.getContent());
	  ms.clear();
	}
	else
	{
	  fputs_unlocked(current.getContent().c_str(), output);
	  return;
	}
	break;

      default:
	cerr << "Error: Unknown input token." << endl;
	return;
    }
  }
}

bool
TransferMult::isDefaultWord(string const &str)
{
  return str.find(" D<");
}

vector<string> 
TransferMult::acceptions(string const &str)
{
  vector<string> result;
  int low = 0;
  
  for(unsigned int i = 0, limit = str.size(); i != limit; i++)
  {
     if(str[i] == '\\')
     {
       i++;
     }
     else if(str[i] == '/')
     {
       string new_word = str.substr(low, i-low);
      
       if(result.size() > 1 && isDefaultWord(new_word))
       {
	 result.push_back(result[0]);
	 result[0] = new_word;
       }
       else
       {
         result.push_back(new_word);
       }
       low = i + 1;
     }
  }
  
  string otherword = str.substr(low);
  if(result.size() > 0 && isDefaultWord(otherword))
  {
    result.push_back(result[0]);
    result[0] = otherword;
  }
  else
  {
    result.push_back(otherword);
  }
  
  return result;
}

void 
TransferMult::writeMultiple(list<vector<string> >::iterator itwords,
                            list<string>::iterator itblanks, 
                            list<vector<string> >::const_iterator limitwords, 
                            string acum , bool multiple)
{
  if(itwords == limitwords)
  {
    if(multiple)
    {
      fputs_unlocked("[ | ]", output);
    }      
    fputs_unlocked(acum.c_str(), output);
  }
  else
  {
    vector<string> &refword = *itwords;

    itwords++;
  
    if(itwords == limitwords)
    {
      for(unsigned int i = 0, limit = refword.size(); i != limit; i++)
      {
        writeMultiple(itwords, itblanks, limitwords, 
                      acum + "^" + refword[i] + "$", multiple || (i > 0));
      }
    }
    else
    {
      string &refblank = *itblanks;
      itblanks++;
      
      for(unsigned int i = 0, limit = refword.size(); i != limit; i++)
      {
        writeMultiple(itwords, itblanks, limitwords, 
                      acum + "^" + refword[i] + "$" + refblank, 
                      multiple || (i > 0));
      }
    }
  }
}

void
TransferMult::applyRule()
{
  list<string> blanks;
  list<vector<string> > words;  

  words.push_back(acceptions(fstp.biltransWithQueue(*tmpword[0], false).first));
  
  for(unsigned int i = 1; i != numwords; i++)
  {
    blanks.push_back(*tmpblank[i-1]);    
    pair<string, int> tr = fstp.biltransWithQueue(*tmpword[i], false);
    words.push_back(acceptions(tr.first));
  }

  writeMultiple(words.begin(), blanks.begin(), words.end());

  ms.init(me->getInitial());
  
  tmpblank.clear();
  tmpword.clear();
  numwords = 0;
}

void
TransferMult::applyWord(string const &word_str)
{
  ms.step('^');
  for(unsigned int i = 0, limit = word_str.size(); i < limit; i++)
  {
    switch(word_str[i])
    {
      case '\\':
        i++;
	ms.step(::tolower(word_str[i]), any_char);
	break;

      case '<':
	for(unsigned int j = i+1; j != limit; j++)
	{
	  if(word_str[j] == '>')
	  {
	    int symbol = alphabet(word_str.substr(i, j-i+1));
	    if(symbol)
	    {
	      ms.step(symbol, any_tag);
	    }
	    else
	    {
	      ms.step(any_tag);
	    }
	    i = j;
	    break;
	  }
	}
	break;
	
      default:
	ms.step(::tolower(word_str[i]), any_char);
	break;
    }
  }
  ms.step('$');
}
