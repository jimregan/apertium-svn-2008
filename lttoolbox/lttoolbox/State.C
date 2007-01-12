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
#include <lttoolbox/State.H>
#include <lttoolbox/Utility.H>

#include <cstring>

Pool<vector<unsigned short> > State::pool(4, vector<unsigned short>(50));

State::State()
{
}
 
State::~State()
{
  destroy();
}

State::State(State const &s)
{
  copy(s);
}

State &
State::operator =(State const &s)
{
  if(this != &s)
  {
    destroy();
    copy(s);
  }

  return *this;
}

void 
State::destroy()
{
  // release references
  for(multimap<Node *, vector<unsigned short> *>::iterator it = state.begin(),
                                                           limit = state.end();
      it != limit; it++)
  {
    pool.release(it->second);
  }

  state.clear();
}

void
State::copy(State const &s)
{
  // release references
  for(multimap<Node *, vector<unsigned short> *>::iterator it = state.begin(),
                                                           limit = state.end();
      it != limit; it++)
  {
    pool.release(it->second);
  }

  state = s.state;

  for(multimap<Node *, vector<unsigned short> *>::iterator it = state.begin(),
                                                           limit = state.end();
      it != state.end(); it++)
  {
    vector<unsigned short> *tmp = pool.get();
    *tmp = *(it->second);
    it->second = tmp;
  }
}

int 
State::size() const
{
  return state.size();
}

void
State::init(Node *initial)
{
  state.clear();
  multimap<Node *, vector<unsigned short> *>::iterator it;
  it = state.insert(pair<Node *, vector<unsigned short> * >(initial, pool.get()));
  it->second->clear();
  epsilonClosure();  
}  

void
State::apply(unsigned short const input)
{
  multimap<Node *, vector<unsigned short> *> new_state;

  multimap<Node *, vector<unsigned short> *>::iterator s_it;

  for(multimap<Node *, vector<unsigned short> *>::iterator s_it = state.begin(),
                                                           s_limit = state.end();
      s_it != s_limit;  s_it++)
  {
    map<unsigned short, Dest>::const_iterator it;
    it = s_it->first->transitions.find(input);
    if(it != s_it->first->transitions.end())
    {
      for(int i = 0; i != it->second.size; i++)
      {
        vector<unsigned short> *new_v = pool.get();
        *new_v = *(s_it->second);

        if(it->first != 0)
	{
          new_v->push_back(it->second.out_tag[i]);
        }

        new_state.insert(pair<Node *, vector<unsigned short> *>(it->second.dest[i], new_v));
      }
    }
    pool.release(s_it->second);
  }

  state = new_state;
}

void 
State::apply(unsigned short const input, unsigned short const alt)
{
  multimap<Node *, vector<unsigned short> *> new_state;

  for(multimap<Node *, vector<unsigned short> *>::iterator s_it = state.begin(),
                                                           s_limit = state.end();
      s_it != s_limit; s_it++)
  {
    map<unsigned short, Dest>::const_iterator it;
    it = s_it->first->transitions.find(input);
    if(it != s_it->first->transitions.end())
    {
      for(int i = 0; i != it->second.size; i++)
      {
        vector<unsigned short> *new_v = pool.get();

        *new_v = *(s_it->second);
        if(it->first != 0)
	{
          new_v->push_back(it->second.out_tag[i]);
        }
        new_state.insert(pair<Node *, vector<unsigned short> *>(it->second.dest[i], new_v));
      }
    }

    it = s_it->first->transitions.find(alt);
    if(it != s_it->first->transitions.end())
    {
      for(int i = 0; i != it->second.size; i++)
      {
        vector<unsigned short> *new_v = pool.get();

        *new_v = *(s_it->second);
        if(it->first != 0)
	{
          new_v->push_back(it->second.out_tag[i]);
        }
        new_state.insert(pair<Node *, vector<unsigned short> *>(it->second.dest[i], new_v));
      }
    }
    pool.release(s_it->second);
  }

  state = new_state;
}

void
State::epsilonClosure()
{
  list<multimap<Node *, vector<unsigned short> *>::iterator> alive;
  
  for(multimap<Node *, vector<unsigned short> *>::iterator s_it = state.begin(),
                                                           s_limit = state.end(); 
      s_it != s_limit; s_it++)
  {
    alive.push_back(s_it);
  }

  while(alive.size() != 0)
  {
    list<multimap<Node *, vector<unsigned short> *>::iterator>::iterator it = alive.begin();
    map<unsigned short, Dest>::iterator it2;
    it2 = (*it)->first->transitions.find(0);
    
    if(it2 != (*it)->first->transitions.end())
    {
      for(int i = 0 ; i != it2->second.size; i++)
      {
        vector<unsigned short> *tmp = pool.get();
        *tmp = *((*it)->second);
        if(it2->second.out_tag[i] != 0)
        {
	  tmp->push_back(it2->second.out_tag[i]);
        }
        alive.push_back(state.insert(pair<Node *, vector<unsigned short> *>(it2->second.dest[i], tmp)));
      }
    }
    alive.erase(it);
  }
}

void
State::step(unsigned short const input)
{
  apply(input);
  epsilonClosure();
}

void
State::step(unsigned short const input, unsigned short const alt)
{
  apply(input, alt);
  epsilonClosure();
}

bool
State::isFinal(set<Node *> const &finals) const
{  
  for(multimap<Node *, vector<unsigned short> *>::const_iterator it = state.begin(), 
                                                                 limit = state.end(); 
      it != limit; it++)
  {
    if(finals.find(it->first) != finals.end())
    {
      return true;
    }
  }
  
  return false;
}

string
State::filterFinals(set<Node *> const &finals, 
		    Alphabet const &alphabet,
		    set<unsigned short> const &escaped_chars,
		    bool uppercase, bool firstupper, int firstchar) const
{
  string result = "";

  for(multimap<Node *, vector<unsigned short> *>::const_iterator it = state.begin(),
                                                                 limit = state.end(); 
      it != limit; it++)
  {
    if(finals.find(it->first) != finals.end())
    {
      result += '/';
      unsigned int const first_char = result.size() + firstchar;
      for(unsigned int i = 0, limit = it->second->size(); i < limit; i++)
      {
        if(escaped_chars.find((*(it->second))[i]) != escaped_chars.end())
        {
          result += '\\';
        }
        alphabet.getSymbol(result, (*(it->second))[i], uppercase);
      }
      if(firstupper)
      {
	if(result[first_char] == '~')
	{
	  // skip post-generation mark
	  result[first_char+1] = toupper(result[first_char+1]);
	}
	else
	{
          result[first_char] = toupper(result[first_char]);
	}
      }
    }
  }
  
  return result;
}

string
State::filterFinalsSAO(set<Node *> const &finals, 
		       Alphabet const &alphabet,
		       set<unsigned short> const &escaped_chars,
		       bool uppercase, bool firstupper, int firstchar) const
{
  string result = "";

  for(multimap<Node *, vector<unsigned short> *>::const_iterator it = state.begin(),
                                                                 limit = state.end(); 
      it != limit; it++)
  {
    if(finals.find(it->first) != finals.end())
    {
      result += '/';
      unsigned int const first_char = result.size() + firstchar;
      for(unsigned int i = 0, limit = it->second->size(); i < limit; i++)
      {
        if(escaped_chars.find((*(it->second))[i]) != escaped_chars.end())
        {
          result += '\\';
        }
        if(alphabet.isTag((*(it->second))[i]))
        {
          result += '&';
          result.append(alphabet.getSymbol((*(it->second))[i]).substr(1));
          result[result.size()-1] = ';';
        }
        else
        {
          alphabet.getSymbol(result, (*(it->second))[i], uppercase);
        }
      }
      if(firstupper)
      {
	if(result[first_char] == '~')
	{
	  // skip post-generation mark
	  result[first_char+1] = toupper(result[first_char+1]);
	}
	else
	{
          result[first_char] = toupper(result[first_char]);
	}
      }
    }
  }
  
  return result;
}

