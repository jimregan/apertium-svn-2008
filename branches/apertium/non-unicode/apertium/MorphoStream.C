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
/** 
 *  Word class and MorphoStream class definitions
 *
 *  @author	Felipe S�nchez-Mart�nez 
 */

#include <apertium/MorphoStream.H>
#include <apertium/ConstantManager.H>
#include <vector>

MorphoStream::MorphoStream(FILE *ftxt, bool d, TaggerData *t)
{
  foundEOF = false;
  debug=d;
  td = t;
  me = td->getPatternList().newMatchExe();
  alphabet = td->getPatternList().getAlphabet();
  input = ftxt;
  ca_any_char = alphabet(PatternList::ANY_CHAR);
  ca_any_tag = alphabet(PatternList::ANY_TAG);
  
  ConstantManager &constants = td->getConstants();
  ca_kignorar = constants.getConstant("kIGNORAR");
  ca_kbarra = constants.getConstant("kBARRA");
  ca_kdollar = constants.getConstant("kDOLLAR");
  ca_kbegin = constants.getConstant("kBEGIN");
  ca_kmot = constants.getConstant("kMOT");
  ca_kmas = constants.getConstant("kMAS");
  ca_kunknown = constants.getConstant("kUNKNOWN");
  
  map<string, int> &tag_index = td->getTagIndex();
  ca_tag_keof = tag_index["TAG_kEOF"];  
  ca_tag_kundef = tag_index["TAG_kUNDEF"]; 
}

MorphoStream::~MorphoStream() 
{
  delete me;
}

TaggerWord*
MorphoStream::get_next_word()
{
  if(vwords.size() != 0)
  {
    TaggerWord* word=vwords.front();
    vwords.erase(vwords.begin());
//    cout << *word << endl;
    return word;
  }

  if(feof(input))
  {
    return NULL;
  }
  
  int ivwords = 0;
  vwords.push_back(new TaggerWord());

  while(true)
  {
    int symbol = fgetc_unlocked(input);
    if(feof(input))
    {
      vwords[ivwords]->add_tag(ca_tag_keof, "", td->getPreferRules());
      return get_next_word();
    }
    if(symbol == '^')
    {
      readRestOfWord(ivwords);
      return get_next_word();
    }
    else
    {
      string str = "";
      str += static_cast<char>(symbol);

      while(symbol != '^')
      {
	symbol = fgetc_unlocked(input);
	if(feof(input))
	{
	  vwords[ivwords]->add_ignored_string(str);
          vwords[ivwords]->add_tag(ca_tag_keof, "", td->getPreferRules());
	  return get_next_word();
	}
	else if(symbol == '\\')
	{
	  str += '\\';
          symbol = fgetc_unlocked(input);
	  if(feof(input))
	  {
	    vwords[ivwords]->add_ignored_string(str);
            vwords[ivwords]->add_tag(ca_tag_keof, "", td->getPreferRules());
	    return get_next_word();
	  }
	  str += static_cast<char>(symbol);
	}
	else if(symbol == '^')
	{
	  if(str.size() > 0)
	  {
	    vwords[ivwords]->add_ignored_string(str);
          }
	  readRestOfWord(ivwords);
	  return get_next_word();
	}
        else
	{
	  str += static_cast<char>(symbol);
	}
      }
    }
  }
}

void
MorphoStream::lrlmClassify(string const &str, int &ivwords)
{
  int floor = 0;
  int last_type = -1;
  int last_pos = 0;

  ms.init(me->getInitial());
  for(int i = 0, limit = str.size(); i != limit; i++)
  {
    if(str[i] != '<')
    {
      if(str[i] == '+')
      {
        int val = ms.classifyFinals(me->getFinals());
        if(val != -1)
        {
          last_pos = i-1;
          last_type = val;
        }
      }
      ms.step(tolower(str[i]), ca_any_char);
    }
    else
    {
      string tag = "";
      for(int j = i+1; j != limit; j++)
      {
        if(str[j] == '\\')
        {
 	  j++;
        }
        else if(str[j] == '>')
        {
 	  tag = str.substr(i, j-i+1);
	  i = j;
          break;
        }
      }

      int symbol = alphabet(tag);
      if(symbol)
      {
        ms.step(symbol, ca_any_tag);
      }
      else
      {
        ms.step(ca_any_tag);
      }
    }

    if(ms.size() == 0)
    {
      if(last_pos != floor)
      {
        vwords[ivwords]->add_tag(last_type, 
                                 str.substr(floor, last_pos - floor + 1),
                                 td->getPreferRules());
	if(str[last_pos+1] == '+' && last_pos+1 < limit )
	{	
	  floor = last_pos + 1;
	  last_pos = floor;
          vwords[ivwords]->set_plus_cut(true); 
          if (((int)vwords.size())<=((int)(ivwords+1)))
            vwords.push_back(new TaggerWord(true));
          ivwords++;
	  ms.init(me->getInitial());
	}
	i = floor++;
      }
      else
      {
        if (debug)
        {
	  cerr<<"Warning: There is not coarse tag for the fine tag '"<< str.substr(floor) <<"'\n";
          cerr<<"         This is because of an incomplete tagset definition or a dictionary error\n";
	}
        vwords[ivwords]->add_tag(ca_tag_kundef, str.substr(floor) , td->getPreferRules());
	return;
      }
    }
    else if(i == limit - 1)
    {
      if(ms.classifyFinals(me->getFinals()) == -1)
      {
        if(last_pos != floor)
        {
          vwords[ivwords]->add_tag(last_type, 
                                   str.substr(floor, last_pos - floor + 1),
                                   td->getPreferRules());
          if(str[last_pos+1] == '+' && last_pos+1 < limit )
          {	
            floor = last_pos + 1;
	    last_pos = floor;
            vwords[ivwords]->set_plus_cut(true); 
            if (((int)vwords.size())<=((int)(ivwords+1)))
              vwords.push_back(new TaggerWord(true));
            ivwords++;
            ms.init(me->getInitial());
	  }
	  i = floor++;
        }
        else
        {
          if (debug)
          {
	    cerr<<"Warning: There is not coarse tag for the fine tag '"<< str.substr(floor) <<"'\n";
            cerr<<"         This is because of an incomplete tagset definition or a dictionary error\n";
	  }
          vwords[ivwords]->add_tag(ca_tag_kundef, str.substr(floor) , td->getPreferRules());
	  return;
        }
      }
    }
  }
  
  int val = ms.classifyFinals(me->getFinals());
  if(val == -1)
  {
    val = ca_tag_kundef;
    if (debug)
    {
      cerr<<"Warning: There is not coarse tag for the fine tag '"<< str.substr(floor) <<"'\n";
      cerr<<"         This is because of an incomplete tagset definition or a dictionary error\n";
    }

  }    
  vwords[ivwords]->add_tag(val, str.substr(floor), td->getPreferRules());
}

void
MorphoStream::readRestOfWord(int &ivwords)
{
  // first we have the superficial form
  string  str = "";
  
  while(true)
  {
    int symbol = fgetc_unlocked(input);
    if(feof(input))
    {
      if(str.size() > 0)
      {
        vwords[ivwords]->add_ignored_string(str);
        cerr<<"Warning (internal): kIGNORE was returned while reading a word\n";
        cerr<<"Word being read: "<<vwords[ivwords]->get_superficial_form()<<"\n";
        cerr<<"Debug: "<< str <<"\n";
      }
      vwords[ivwords]->add_tag(ca_tag_keof, "", td->getPreferRules());
      return;
    }
    else if(symbol == '\\')
    {
      symbol = fgetc_unlocked(input);
      str += '\\';
      str += static_cast<char>(symbol);
    }
    else if(symbol == '/')
    {
      vwords[ivwords]->set_superficial_form(str); 
      str = "";
      break;
    }
    else if(symbol == '$')
    {
      vwords[ivwords]->set_superficial_form(str);
      vwords[ivwords]->add_ignored_string("$");
      break;
    }
    else
    {
      str += static_cast<char>(symbol);
    }
  }

  // then we read the acceptions

  while(true)
  {
    int symbol = fgetc_unlocked(input);
    if(feof(input))
    {
      if(str.size() > 0)
      {
        vwords[ivwords]->add_ignored_string(str);
        cerr<<"Warning (internal): kIGNORE was returned while reading a word\n";
        cerr<<"Word being read: "<<vwords[ivwords]->get_superficial_form()<<"\n";
        cerr<<"Debug: "<< str <<"\n";
      }
      vwords[ivwords]->add_tag(ca_tag_keof, "", td->getPreferRules());
      return;
    }
    else if(symbol == '\\')
    {
      symbol = fgetc_unlocked(input);
      str += '\\';
      str += static_cast<char>(symbol);
      symbol = '\\';  // to prevent exiting with '\$'
    }
    else if(symbol == '/')
    {
      lrlmClassify(str, ivwords);
      str = "";
      ivwords = 0;
      continue;
    }
    else if(symbol == '$')
    {
      if(str[0] != '*')// do nothing with unknown words 
      {
	lrlmClassify(str, ivwords);
      }
      return;
    }
    else
    {
      str += static_cast<char>(symbol);
    }    
  }
}
