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
#include <lttoolbox/FSTProcessor.H>
#include <lttoolbox/Compression.H>
#include <lttoolbox/MyStdio.H>

#include <iostream>

using namespace std;

FSTProcessor::FSTProcessor()
{
  // escaped_chars chars
  escaped_chars.insert(static_cast<unsigned short>('['));
  escaped_chars.insert(static_cast<unsigned short>(']'));
  escaped_chars.insert(static_cast<unsigned short>('^'));
  escaped_chars.insert(static_cast<unsigned short>('$'));
  escaped_chars.insert(static_cast<unsigned short>('/'));
  escaped_chars.insert(static_cast<unsigned short>('\\'));
  escaped_chars.insert(static_cast<unsigned short>('@'));
  escaped_chars.insert(static_cast<unsigned short>('<'));
  escaped_chars.insert(static_cast<unsigned short>('>'));
}

FSTProcessor::~FSTProcessor()
{
}

void
FSTProcessor::streamError()
{
  cerr << "Error: Malformed input stream." << endl;
  exit(EXIT_FAILURE);
}

unsigned short
FSTProcessor::readEscaped(FILE *input)
{
  if(feof(input))
  {
    streamError();
  }

  unsigned short val = static_cast<unsigned short>(fgetc_unlocked(input));

  if(feof(input) || escaped_chars.find(val) == escaped_chars.end())
  {
    streamError();
  }
  
  return val;
}

string 
FSTProcessor::readFullBlock(FILE *input, char const delim1, char const delim2)
{
  string result = "";
  result += delim1;
  char c = delim1;

  while(!feof(input) && c != delim2)
  {
    c = static_cast<char>(fgetc_unlocked(input));
    result += c;
    if(c != '\\')
    {
      continue;
    }
    else
    {
      result += static_cast<char>(readEscaped(input));
    }
  }   

  if(c != delim2)
  {
    streamError();
  }

  return result;
}

unsigned short
FSTProcessor::readAnalysis(FILE *input)
{
  if(!input_buffer.isEmpty())
  {
    return input_buffer.next();
  }

  unsigned short val = static_cast<unsigned short>(fgetc_unlocked(input));
  if(feof(input))
  {
    return 0;
  }

  if(escaped_chars.find(val) != escaped_chars.end())
  {
    switch(val)
    {
      case '<':
        val = static_cast<unsigned short>(alphabet(readFullBlock(input, '<', '>')));
	input_buffer.add(val);
        return val;

      case '[':
        blankqueue.push(readFullBlock(input, '[', ']'));
        input_buffer.add(' ');
        return static_cast<unsigned short>(' ');
        
      case '\\':
        val = static_cast<unsigned short>(fgetc_unlocked(input));
        if(escaped_chars.find(val) == escaped_chars.end())
        {
          streamError();
        }
        input_buffer.add(val);
        return val;

      default:
        streamError();
    }                   
  }

  input_buffer.add(val);
  return val;
}


void
FSTProcessor::skipUntil(FILE *input, FILE *output, int const character)
{
  while(true)
  {
    int val = fgetc_unlocked(input);
    if(feof(input))
    {
      return;
    }

    if(val == '\\')
    {
      val = fgetc_unlocked(input);
      if(feof(input))
      {
	return;
      }
      fputc_unlocked('\\', output);
      fputc_unlocked(val, output);
    }
    else if(val == character)
    {
      return;
    }
    else
    {
      fputc_unlocked(val, output);
    }
  }
}

unsigned short
FSTProcessor::readGeneration(FILE *input, FILE *output)
{
  int val = fgetc_unlocked(input);

  if(feof(input))
  {
    return 0;
  }
  
  if(outOfWord)
  {
    if(val == '^')
    {
      val = fgetc_unlocked(input);
      if(feof(input))
      {
        return 0;
      }
    }
    else if(val == '\\')
    {
      fputc_unlocked(val,output);
      val = fgetc_unlocked(input);
      if(feof(input))
      {
        return 0;
      }
      fputc_unlocked(val,output);
      skipUntil(input,output, '^');
      val = fgetc_unlocked(input);
      if(feof(input))
      {
        return 0;
      }
    }
    else
    {
      fputc_unlocked(val, output);
      skipUntil(input, output, '^');
      val = fgetc_unlocked(input);
      if(feof(input))
      {
        return 0;
      }
    }
    outOfWord = false;
  }

  if(val == '\\')
  {
    val = fgetc_unlocked(input);
    return static_cast<unsigned short>(val);
  }
  else if(val == '$')
  {
    outOfWord = true;
    return static_cast<unsigned short>('$');
  }
  else if(val == '<')
  {
    string cad = "";
    cad += char(val);
    while((val = fgetc_unlocked(input)) != '>')
    {
      if(feof(input))
      {
	streamError();
      }
      cad += char(val);
    }
    cad += char(val);

    return alphabet(cad);
  }
  else if(val == '[')
  {
    fputs_unlocked(readFullBlock(input, '[', ']').c_str(), output);
    return readGeneration(input, output);
  }
  else
  {
    return static_cast<unsigned short>(val);
  }

  return 0;
}

void
FSTProcessor::flushBlanks(FILE *output)
{  
  for(unsigned int i = blankqueue.size(); i > 0; i--)
  {
    fputs_unlocked(blankqueue.front().c_str(), output);
    blankqueue.pop();
  }
}

void
FSTProcessor::calcInitial()
{
  for(map<string, TransExe, Ltstr>::iterator it = transducers.begin(),
                                             limit = transducers.end();
      it != limit; it++)
  {
    root.addTransition(0, 0, it->second.getInitial());
  }

  initial_state.init(&root);
}

bool
FSTProcessor::endsWith(string const &a, string const &b)
{
  if(a.size() < b.size())
  {
    return false;
  }
  else
  {
    return a.substr(a.size()-b.size()) == b;
  }
}

void
FSTProcessor::classifyFinals()
{
  for(map<string, TransExe, Ltstr>::iterator it = transducers.begin(),
                                             limit = transducers.end();
      it != limit; it++)
  {
    if(endsWith(it->first, "@inconditional"))
    {
      inconditional.insert(it->second.getFinals().begin(),
                           it->second.getFinals().end());
    }
    else if(endsWith(it->first, "@standard"))
    {
      standard.insert(it->second.getFinals().begin(),
                      it->second.getFinals().end());
    }
    else if(endsWith(it->first, "@postblank"))
    {
      postblank.insert(it->second.getFinals().begin(),
                       it->second.getFinals().end());
    }
    else
    {
      cerr << "Error: Unsupported transducer type for '";
      cerr << it->first << "'." << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void
FSTProcessor::writeEscaped(string const &str, FILE *output)
{
  for(unsigned int i = 0, limit = str.size(); i < limit; i++)
  {
    if(escaped_chars.find(str[i]) != escaped_chars.end())
    {
      fputc_unlocked('\\', output);
    }
    fputc_unlocked(str[i], output);
  } 
}


void
FSTProcessor::printWord(string const &sf, string const &lf, FILE *output)
{
  fputc_unlocked('^', output);
  writeEscaped(sf, output);
  fwrite_unlocked(lf.c_str(), lf.size(), sizeof(char), output);
  fputc_unlocked('$', output);
}

void
FSTProcessor::printUnknownWord(string const &sf, FILE *output)
{
  fputc_unlocked('^', output);
  writeEscaped(sf, output);
  fputc_unlocked('/', output);
  fputc_unlocked('*', output);
  writeEscaped(sf, output);
  fputc_unlocked('$', output);  
}

unsigned int
FSTProcessor::lastBlank(string const &str)
{
  for(int i = static_cast<int>(str.size())-1; i >= 0; i--)
  {
    if(alphabetic_chars.find(str[i]) == alphabetic_chars.end())
    {
      return static_cast<unsigned int>(i);
    }
  }

  return 0;
}

void
FSTProcessor::printSpace(unsigned short const val, FILE *output)
{
  if(blankqueue.size() > 0)
  {
    flushBlanks(output);
  }
  else
  {
    fputc_unlocked(val, output);
  }
}

bool
FSTProcessor::isEscaped(unsigned short const c) const
{
  return escaped_chars.find(c) != escaped_chars.end();
}

bool
FSTProcessor::isAlphabetic(unsigned short const c) const
{
  return alphabetic_chars.find(c) != alphabetic_chars.end();
}

void
FSTProcessor::load(FILE *input)
{
  int len = Compression::multibyte_read(input);

  while(len > 0)
  {
    alphabetic_chars.insert(static_cast<unsigned short>(fgetc_unlocked(input)));
    len--;
  }  
  
  alphabet.read(input);

  len = Compression::multibyte_read(input);

  while(len > 0)
  {
    int len2 = Compression::multibyte_read(input);
    string name = "";
    while(len2 > 0)
    {
      name += static_cast<char>(fgetc_unlocked(input));
      len2--;
    }
    transducers[name].read(input, alphabet.size());
    len--;
  } 

}

void
FSTProcessor::initAnalysis()
{
  calcInitial();
  classifyFinals();
  all_finals = standard;
  all_finals.insert(inconditional.begin(), inconditional.end());
  all_finals.insert(postblank.begin(), postblank.end());
}

void
FSTProcessor::initGeneration()
{
  calcInitial();  
  for(map<string, TransExe, Ltstr>::iterator it = transducers.begin(),
                                             limit = transducers.end();
      it != limit; it++)
  {
    all_finals.insert(it->second.getFinals().begin(),
                      it->second.getFinals().end());
  }
}

void
FSTProcessor::initPostgeneration()
{
  initGeneration();
}

void
FSTProcessor::initBiltrans()
{
  initGeneration();
}

void
FSTProcessor::analysis(FILE *input, FILE *output)
{
  bool last_incond = false;
  bool last_postblank = false;
  State current_state = initial_state;
  string lf = "";
  string sf = "";
  int last = 0;

  while(unsigned short val = readAnalysis(input))
  {
    // test for final states
    if(current_state.isFinal(all_finals))
    {
      if(current_state.isFinal(inconditional))
      {
        bool firstupper = isupper(sf[0]);
        bool uppercase = firstupper && isupper(sf[sf.size()-1]);

        lf = current_state.filterFinals(all_finals, alphabet,
                                        escaped_chars,
                                        uppercase, firstupper);
        last_incond = true;
        last = input_buffer.getPos();
      }
      else if(current_state.isFinal(postblank))
      {
        bool firstupper = isupper(sf[0]);
        bool uppercase = firstupper && isupper(sf[sf.size()-1]);

        lf = current_state.filterFinals(all_finals, alphabet,
                                        escaped_chars,
                                        uppercase, firstupper);
        last_postblank = true;
        last = input_buffer.getPos();      
      }
      else if(!isAlphabetic(val))
      {
        bool firstupper = isupper(sf[0]);
        bool uppercase = firstupper && isupper(sf[sf.size()-1]);

        lf = current_state.filterFinals(all_finals, alphabet, 
                                        escaped_chars, 
                                        uppercase, firstupper);
        last_postblank = false;
        last_incond = false;
        last = input_buffer.getPos();
      }
    }
    else if(sf == "" && isspace(val)) 
    {
      lf = "/*";
      lf.append(sf);
      last_postblank = false;
      last_incond = false;
      last = input_buffer.getPos();
    }

    if(!isupper(val))
    {
      current_state.step(val);
    }
    else
    {
      current_state.step(val, tolower(val));
    }
      
    if(current_state.size() != 0)
    {
      alphabet.getSymbol(sf, val);
    }
    else
    {
      if(!isAlphabetic(val) && sf == "")
      {
        if(isspace(val))
        {
          printSpace(val, output);
        }
        else
        {
          if(isEscaped(val))
          {
            fputc_unlocked('\\', output);
          }
          fputc_unlocked(val, output);
        }
      }
      else if(last_incond)
      {
        printWord(sf.substr(0, sf.size()-input_buffer.diffPrevPos(last)),
		  lf, output);
        input_buffer.setPos(last);
        input_buffer.back(1);
      }
      else if(last_postblank)
      {
        printWord(sf.substr(0, sf.size()-input_buffer.diffPrevPos(last)),
		  lf, output);
	fputc_unlocked(' ', output);
        input_buffer.setPos(last);
        input_buffer.back(1);
      }
      else if(isAlphabetic(val) && 
              ((sf.size()-input_buffer.diffPrevPos(last)) > lastBlank(sf) || 
               lf == ""))
      {
        do
        {
          alphabet.getSymbol(sf, val);
        }         
        while((val = readAnalysis(input)) && isAlphabetic(val));

        unsigned int limit = sf.find(' ');
        unsigned int size = sf.size();
        limit = (limit == string::npos?size:limit);
        input_buffer.back(1+(size-limit));
        printUnknownWord(sf.substr(0, limit), output);
      }
      else if(lf == "")
      {
        unsigned int limit = sf.find(' ');
        unsigned int size = sf.size();
        limit = (limit == string::npos?size:limit);
        input_buffer.back(1+(size-limit));
        printUnknownWord(sf.substr(0, limit), output);
      }
      else
      {
        printWord(sf.substr(0, sf.size()-input_buffer.diffPrevPos(last)), 
                  lf, output);
        input_buffer.setPos(last);
        input_buffer.back(1);
      }
	
      current_state = initial_state;
      lf = "";
      sf = "";
      last_incond = false;
      last_postblank = false;
    }
  }
  
  // print remaining blanks
  flushBlanks(output);
}

void
FSTProcessor::generation(FILE *input, FILE *output, bool unknown_words)
{
  State current_state = initial_state;
  string sf = "";
 
  outOfWord = false;
 
  skipUntil(input, output, '^');
  while(unsigned short val = readGeneration(input, output))
  {
    if(val == '$')
    {
      if(sf[0] == '*')
      {
	if(unknown_words)
        {
	  writeEscaped(sf, output);
	}
	else
	{
	  writeEscaped(sf.substr(1), output);
	}
      }
      else if(sf[0] == '@')
      {
        writeEscaped(sf, output);
      }
      else if(current_state.isFinal(all_finals))
      {
        bool uppercase = sf.size() > 1 && isupper(sf[1]);
        bool firstupper= isupper(sf[0]);

        fputs_unlocked(current_state.filterFinals(all_finals, alphabet,
                                                  escaped_chars,
                                                  uppercase, firstupper).substr(1).c_str(),
						  output);
      }
      else
      {
	fputc_unlocked('#', output);
	writeEscaped(sf.c_str(), output);
      }
  
      current_state = initial_state;
      sf = "";
    }
    else if(isspace(val) && sf.size() == 0)
    {
      // do nothing
    }
    else if(sf.size() > 0 && sf[0] == '*')
    {
      alphabet.getSymbol(sf, val);
    }
    else
    {
      alphabet.getSymbol(sf,val);
      if(current_state.size() > 0)
      {
	if(val < 256 && isupper(val))
	{
	  current_state.step(val, tolower(val));
	}
	else
	{
	  current_state.step(val);
	}
      }
    }
  }
}

void
FSTProcessor::postgeneration(FILE *input, FILE *output)
{
  bool skip_mode = true;
  State current_state = initial_state;
  string lf = "";
  string sf = "";
  int last = 0;

  while(unsigned short val = readAnalysis(input))
  {
    if(val == '~')
    {
      skip_mode = false;
    }

    if(skip_mode)
    {
      if(isspace(val))
      {
	printSpace(val, output);
      }
      else
      {
        if(isEscaped(val))
        {
          fputc_unlocked('\\', output);
        }
      	fputc_unlocked(val, output);
      }
    }
    else      
    {
      // test for final states
      if(current_state.isFinal(all_finals))
      {
        bool firstupper = isupper(sf[1]);
        bool uppercase = sf.size() > 1 && firstupper && isupper(sf[2]);
        lf = current_state.filterFinals(all_finals, alphabet,
					escaped_chars,
					uppercase, firstupper, 0);

        // case of the beggining of the next word
        	
        string mybuf = "";				
        for(unsigned int i = sf.size()-1; i >= 0; i--)
        {
          if(!isalpha(sf[i]))
          {
            break;
          }
          else
          {
            mybuf = sf[i] + mybuf;
          }
        }
        
        if(mybuf.size() > 0)
        {
          bool myfirstupper = isupper(mybuf[0]);
          bool myuppercase = mybuf.size() > 1 && isupper(mybuf[1]);
        
          for(unsigned int i = lf.size()-1; i >= 0; i--)
          {
            if(!isalpha(lf[i]))
            {
              if(myfirstupper && i != lf.size()-1)
              {
                lf[i+1] = toupper(lf[i+1]);
              }
              else
              {
                lf[i+1] = tolower(lf[i+1]);
              }
              break;
            }
            else
            {
              if(myuppercase)
              {
                lf[i] = toupper(lf[i]);
              }
              else
              {
                lf[i] = tolower(lf[i]);
              }
            }
          }
        }
        
        last = input_buffer.getPos();
      }

      if(!isupper(val))
      {
        current_state.step(val);
      }
      else
      {
        current_state.step(val, tolower(val));
      }
      
      if(current_state.size() != 0)
      {
        alphabet.getSymbol(sf, val);
      }
      else
      {	
        if(lf == "")
	{
          unsigned int mark = sf.size();
          for(unsigned int i = 1, limit = sf.size(); i < limit; i++)
          {
            if(sf[i] == '~')
            { 
              mark = i;
              break;
            }
          }
	  fputs_unlocked(sf.substr(1, mark-1).c_str(), output);
          if(mark == sf.size())
          {
	    input_buffer.back(1);
          }
          else
          {
            input_buffer.back(sf.size()-mark);
	  }
	}
	else
	{
	  fputs_unlocked(lf.substr(1,lf.size()-3).c_str(), output);
	  input_buffer.setPos(last);
          input_buffer.back(2);
          val = lf[lf.size()-2];
	  if(isspace(val))
	  {
	    printSpace(val, output);
	  }
	  else
	  {
	    if(isEscaped(val))
	    {
	      fputc_unlocked('\\', output);
	    }
	    fputc_unlocked(val, output);
	  }	  
	}

	current_state = initial_state;
	lf = "";
	sf = "";
	skip_mode = true;
      }
    }
  }
  
  // print remaining blanks
  flushBlanks(output);  
}

string
FSTProcessor::biltrans(string const &input_word, bool with_delim)
{
  State current_state = initial_state;
  string result="";
  unsigned int start_point = 1;
  unsigned int end_point = input_word.size()-2;
  string queue = "";
  
  if(with_delim == false)
  {
    start_point = 0;
    end_point = input_word.size()-1;
  }

  if(input_word[start_point] == '*')
  {
    return input_word;
  }
  
  bool firstupper = isupper(input_word[start_point]);
  bool uppercase = firstupper && isupper(input_word[start_point+1]);

  for(unsigned int i = start_point; i <= end_point; i++)
  {
    unsigned short val;
    string symbol ="";
 
    if(input_word[i] == '\\')
    {
      i++;
      val = static_cast<unsigned short>((unsigned char) input_word[i]);
    }
    else if(input_word[i] == '<')
    {
      symbol = '<';
      for(unsigned int j = i + 1; j <= end_point; j++)
      {
	symbol += input_word[j];
	if(input_word[j] == '>')
	{
	  i = j;
	  break;
	}
      }
      val = alphabet(symbol);
    }
    else
    {
      val = static_cast<unsigned short>((unsigned char) input_word[i]);
    }
    if(current_state.size() != 0)
    {
      if(val < 256 && isupper(val))
      {
	current_state.step(val, tolower(val));
      }
      else
      {
	current_state.step(val);
      }
    }
    if(current_state.isFinal(all_finals))
    {
      result = current_state.filterFinals(all_finals, alphabet,
                                         escaped_chars,
                                         uppercase, firstupper, 0);
      if(with_delim)
      {      
        result[0] = '^';
      }
      else
      {
        result = result.substr(1);
      }
    }
    
    if(current_state.size() == 0)
    { 
      if(symbol != "")
      {
        queue.append(symbol);
      }
      else
      {
	// word is not present
        if(with_delim)
	{
          result = "^@" + input_word.substr(1);  
	}
        else
	{
          result = "@" + input_word;
	}
        return result;  
      }      
    }
  }

  // attach unmatched queue automatically

  if(queue != "")
  {
    string result_with_queue = "";    
    bool multiple_translation = false;
    for(unsigned int i = 0, limit = result.size(); i != limit; i++)
    {
      switch(result[i])
      {
        case '\\':
          result_with_queue += '\\';
	  i++;
          break;
     
        case '/':
          result_with_queue.append(queue);
	  multiple_translation = true;
	  break;
    
        default:
          break;
      }
      result_with_queue += result[i];
    }
    result_with_queue.append(queue);

    if(with_delim)
    {
      result_with_queue += '$';
 //     if(multiple_translation)
 //     {
 //       result_with_queue[0] = '@';
 //       result = "^" + result_with_queue;
 //       return result;
 //     }
 //     else
 //     {
        return result_with_queue;
 //     }
    }
 //   if(multiple_translation)
 //   {
 //     return  "@"+result_with_queue;
 //   }
 //   else
 //   {
      return result_with_queue;
 //   }
  }
  else
  {
    if(with_delim)
    {
      result += '$';
    }
    return result;
  }
}

pair<string, int>
FSTProcessor::biltransWithQueue(string const &input_word, bool with_delim)
{
  State current_state = initial_state;
  string result="";
  unsigned int start_point = 1;
  unsigned int end_point = input_word.size()-2;
  string queue = "";
  
  if(with_delim == false)
  {
    start_point = 0;
    end_point = input_word.size()-1;
  }

  if(input_word[start_point] == '*')
  {
    return pair<string, int>(input_word, 0);
  }
  
  bool firstupper = isupper(input_word[start_point]);
  bool uppercase = firstupper && isupper(input_word[start_point+1]);

  for(unsigned int i = start_point; i <= end_point; i++)
  {
    unsigned short val;
    string symbol ="";
 
    if(input_word[i] == '\\')
    {
      i++;
      val = static_cast<unsigned short>((unsigned char) input_word[i]);
    }
    else if(input_word[i] == '<')
    {
      symbol = '<';
      for(unsigned int j = i + 1; j <= end_point; j++)
      {
	symbol += input_word[j];
	if(input_word[j] == '>')
	{
	  i = j;
	  break;
	}
      }
      val = alphabet(symbol);
    }
    else
    {
      val = static_cast<unsigned short>((unsigned char) input_word[i]);
    }
    if(current_state.size() != 0)
    {
      if(val < 256 && isupper(val))
      {
	current_state.step(val, tolower(val));
      }
      else
      {
	current_state.step(val);
      }
    }
    if(current_state.isFinal(all_finals))
    {
      result = current_state.filterFinals(all_finals, alphabet,
                                         escaped_chars,
                                         uppercase, firstupper, 0);
      if(with_delim)
      {      
        result[0] = '^';
      }
      else
      {
        result = result.substr(1);
      }
    }
    
    if(current_state.size() == 0)
    { 
      if(symbol != "")
      {
        queue.append(symbol);
      }
      else
      {
	// word is not present
        if(with_delim)
	{
          result = "^@" + input_word.substr(1);  
	}
        else
	{
          result = "@" + input_word;
	}
        return pair<string, int>(result, 0);  
      }      
    }
  }

  // attach unmatched queue automatically

  if(queue != "")
  {
    string result_with_queue = "";    
    bool multiple_translation = false;
    for(unsigned int i = 0, limit = result.size(); i != limit; i++)
    {
      switch(result[i])
      {
        case '\\':
          result_with_queue += '\\';
	  i++;
          break;
     
        case '/':
          result_with_queue.append(queue);
	  multiple_translation = true;
	  break;
    
        default:
          break;
      }
      result_with_queue += result[i];
    }
    result_with_queue.append(queue);

    if(with_delim)
    {
      result_with_queue += '$';
    }
    return pair<string, int>(result_with_queue, queue.size());
  }
  else
  {
    if(with_delim)
    {
      result += '$';
    }
    return pair<string, int>(result, 0);
  }
}

string
FSTProcessor::biltransWithoutQueue(string const &input_word, bool with_delim)
{
  State current_state = initial_state;
  string result="";
  unsigned int start_point = 1;
  unsigned int end_point = input_word.size()-2;
  
  if(with_delim == false)
  {
    start_point = 0;
    end_point = input_word.size()-1;
  }

  if(input_word[start_point] == '*')
  {
    return input_word;
  }
  
  bool firstupper = isupper(input_word[start_point]);
  bool uppercase = firstupper && isupper(input_word[start_point+1]);

  for(unsigned int i = start_point; i <= end_point; i++)
  {
    unsigned short val;
    string symbol ="";
 
    if(input_word[i] == '\\')
    {
      i++;
      val = static_cast<unsigned short>((unsigned char) input_word[i]);
    }
    else if(input_word[i] == '<')
    {
      symbol = '<';
      for(unsigned int j = i + 1; j <= end_point; j++)
      {
	symbol += input_word[j];
	if(input_word[j] == '>')
	{
	  i = j;
	  break;
	}
      }
      val = alphabet(symbol);
    }
    else
    {
      val = static_cast<unsigned short>((unsigned char) input_word[i]);
    }
    if(current_state.size() != 0)
    {
      if(val < 256 && isupper(val))
      {
	current_state.step(val, tolower(val));
      }
      else
      {
	current_state.step(val);
      }
    }
    if(current_state.isFinal(all_finals))
    {
      result = current_state.filterFinals(all_finals, alphabet,
                                         escaped_chars,
                                         uppercase, firstupper, 0);
      if(with_delim)
      {      
        result[0] = '^';
      }
      else
      {
        result = result.substr(1);
      }
    }
    
    if(current_state.size() == 0)
    { 
      if(symbol == "")
      {
	// word is not present
        if(with_delim)
	{
          result = "^@" + input_word.substr(1);  
	}
        else
	{
          result = "@" + input_word;
	}
        return result;  
      }      
    }
  }

  if(with_delim)
  {
    result += '$';
  }
  return result;
}


bool
FSTProcessor::valid() const
{
  return !initial_state.isFinal(all_finals);
}

unsigned short
FSTProcessor::readSAO(FILE *input)
{
  if(!input_buffer.isEmpty())
  {
    return input_buffer.next();
  }

  unsigned short val = static_cast<unsigned short>(fgetc_unlocked(input));
  if(feof(input))
  {
    return 0;
  }

  if(escaped_chars.find(val) != escaped_chars.end())
  {
    if(val == '<')
    {
      string str = readFullBlock(input, '<', '>');
      if(str.substr(0, 9) == "<![CDATA[")
      {
        while(str.substr(str.size()-3) != "]]>")
        {
          str.append(readFullBlock(input, '<', '>').substr(1));
        }
        blankqueue.push(str);
        input_buffer.add(' ');
        return static_cast<unsigned short>(' ');
      }
      else
      {
        streamError();
      }
    }
    else if (val == '\\') {
      val = static_cast<unsigned short>(fgetc_unlocked(input));
      if(isEscaped(val))
      {
        input_buffer.add(val);
        return val;
      }
      else
        streamError();
    }
    else
    {
      streamError();
    }                   
  }

  input_buffer.add(val);
  return val;
}

void
FSTProcessor::printSAOWord(string const &lf, FILE *output)
{
  for(unsigned int i = 1, limit = lf.size(); i != limit; i++)
  {
    if(lf[i] == '/')
    {
      break;
    }
    fputc_unlocked(lf[i], output);
  }
}

void
FSTProcessor::SAO(FILE *input, FILE *output)
{
  bool last_incond = false;
  bool last_postblank = false;
  State current_state = initial_state;
  string lf = "";
  string sf = "";
  int last = 0;

  escaped_chars.clear();
  escaped_chars.insert(static_cast<unsigned short>('\\'));
  escaped_chars.insert(static_cast<unsigned short>('<'));
  escaped_chars.insert(static_cast<unsigned short>('>'));

  while(unsigned short val = readSAO(input))
  {
    // test for final states
    if(current_state.isFinal(all_finals))
    {
      if(current_state.isFinal(inconditional))
      {
        bool firstupper = isupper(sf[0]);
        bool uppercase = firstupper && isupper(sf[sf.size()-1]);

        lf = current_state.filterFinalsSAO(all_finals, alphabet,
                                        escaped_chars,
                                        uppercase, firstupper);
        last_incond = true;
        last = input_buffer.getPos();
      }
      else if(current_state.isFinal(postblank))
      {
        bool firstupper = isupper(sf[0]);
        bool uppercase = firstupper && isupper(sf[sf.size()-1]);

        lf = current_state.filterFinalsSAO(all_finals, alphabet,
                                        escaped_chars,
                                        uppercase, firstupper);
        last_postblank = true;
        last = input_buffer.getPos();      
      }
      else if(!isAlphabetic(val))
      {
        bool firstupper = isupper(sf[0]);
        bool uppercase = firstupper && isupper(sf[sf.size()-1]);

        lf = current_state.filterFinalsSAO(all_finals, alphabet, 
                                        escaped_chars, 
                                        uppercase, firstupper);
        last_postblank = false;
        last_incond = false;
        last = input_buffer.getPos();
      }
    }
    else if(sf == "" && isspace(val)) // "hay incidencias" problem
    {
      lf = "/*";
      lf.append(sf);
      last_postblank = false;
      last_incond = false;
      last = input_buffer.getPos();
    }

    if(!isupper(val))
    {
      current_state.step(val);
    }
    else
    {
      current_state.step(val, tolower(val));
    }
      
    if(current_state.size() != 0)
    {
      alphabet.getSymbol(sf, val);
    }
    else
    {
      if(!isAlphabetic(val) && sf == "")
      {
        if(isspace(val))
        {
          printSpace(val, output);
        }
        else
        {
          if(isEscaped(val))
          {
            fputc_unlocked('\\', output);
          }
          fputc_unlocked(val, output);
        }
      }
      else if(last_incond)
      {
        printSAOWord(lf, output);
        input_buffer.setPos(last);
        input_buffer.back(1);
      }
      else if(last_postblank)
      {
        printSAOWord(lf, output);
	fputc_unlocked(' ', output);
        input_buffer.setPos(last);
        input_buffer.back(1);
      }
      else if(isAlphabetic(val) && 
              ((sf.size()-input_buffer.diffPrevPos(last)) > lastBlank(sf) || 
               lf == ""))
      {
        do
        {
          alphabet.getSymbol(sf, val);
        }         
        while((val = readSAO(input)) && isAlphabetic(val));

        unsigned int limit = sf.find(' ');
        unsigned int size = sf.size();
        limit = (limit == string::npos?size:limit);
        input_buffer.back(1+(size-limit));
        fwrite_unlocked("<d>", 3, sizeof(char), output);
        fwrite_unlocked(sf.c_str(), limit, sizeof(char), output);
        fwrite_unlocked("</d>", 4, sizeof(char), output);
      }
      else if(lf == "")
      {

        unsigned int limit = sf.find(' ');
        unsigned int size = sf.size();
        limit = (limit == string::npos?size:limit);
        input_buffer.back(1+(size-limit));
        fwrite_unlocked("<d>", 3, sizeof(char), output);
        fwrite_unlocked(sf.c_str(), limit, sizeof(char), output);
        fwrite_unlocked("</d>", 4, sizeof(char), output);
      }
      else
      {
        printSAOWord(lf, output);
        input_buffer.setPos(last);
        input_buffer.back(1);
      }
	
      current_state = initial_state;
      lf = "";
      sf = "";
      last_incond = false;
      last_postblank = false;
    }
  }
  
  // print remaining blanks
  flushBlanks(output);
}

