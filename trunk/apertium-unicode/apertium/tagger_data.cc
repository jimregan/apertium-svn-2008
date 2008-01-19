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
#include <apertium/hmm.h>
#include <apertium/tagger_data.h>
#include <lttoolbox/compression.h>
#include <lttoolbox/endian_util.h>
#include <apertium/string_utils.h>

using namespace Apertium;

void
TaggerData::copy(TaggerData const &o)
{
  open_class = o.open_class;
  forbid_rules = o.forbid_rules;
  tag_index = o.tag_index;
  array_tags = o.array_tags;
  enforce_rules = o.enforce_rules;
  prefer_rules = o.prefer_rules;
  constants = o.constants;
  output = o.output;  
  this->setProbabilities(o.N, o.M, o.a, o.b);
  plist = o.plist;
}

void
TaggerData::destroy()
{
  if(a != NULL)
  {
    for(int i = 0; i != N; i++)
    {
      delete [] a[i];
    }
    delete [] a;
  }
  a = NULL;

  if(b != NULL)
  {
    for(int i = 0; i != N; i++)
    {
      delete [] b[i];
    }
    delete [] b;
  }
  b = NULL;
  N = 0;
  M = 0;
}

TaggerData::TaggerData()
{
  a = NULL;
  b = NULL;
  N = 0;
  M = 0;
}

TaggerData::~TaggerData()
{
  destroy();
}

TaggerData::TaggerData(TaggerData const &o)
{
  a = NULL;
  b = NULL;
  N = 0;
  M = 0;
  copy(o);
}

TaggerData &
TaggerData::operator =(TaggerData const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

set<TTag> &
TaggerData::getOpenClass()
{
  return open_class;
}

void
TaggerData::setOpenClass(set<TTag> const &oc)
{
  open_class = oc;
}

vector<TForbidRule> &
TaggerData::getForbidRules()
{
  return forbid_rules;
}

void
TaggerData::setForbidRules(vector<TForbidRule> &fr)
{
  forbid_rules = fr;
}  

map<wstring, TTag, Ltstr> &
TaggerData::getTagIndex()
{
  return tag_index;
}

void
TaggerData::setTagIndex(map<wstring, TTag, Ltstr> const &ti)
{
  tag_index = ti;
}
  
vector<wstring> &
TaggerData::getArrayTags()
{
  return array_tags;
}

void
TaggerData::setArrayTags(vector<wstring> const &at)
{
  array_tags = at;
}

vector<TEnforceAfterRule> &
TaggerData::getEnforceRules()
{
  return enforce_rules;
}

void
TaggerData::setEnforceRules(vector<TEnforceAfterRule> const &tear)
{
  enforce_rules = tear;
}

vector<wstring> &
TaggerData::getPreferRules()
{
  return prefer_rules;
}

void
TaggerData::setPreferRules(vector<wstring> const &pr)
{
  prefer_rules = pr;
}

ConstantManager &
TaggerData::getConstants()
{
  return constants;
}

void
TaggerData::setConstants(ConstantManager const &c)
{  
  constants = c;
}


Collection &
TaggerData::getOutput()
{
  return output;
}

void
TaggerData::setOutput(Collection const &c)
{
  output = c;
}
  
void
TaggerData::setProbabilities(int const myN, int const myM, 
                             double **myA, double **myB)
{
  this->destroy();
  N = myN;
  M = myM;
  
  if(N != 0 && M != 0)
  {
    // NxN matrix
    a = new double * [N];
    for(int i = 0; i != N; i++)
    {
      a[i] = new double[N];
      if(myA != NULL)
      {
        for(int j = 0; j != N; j++)
        { 
          a[i][j] = myA[i][j];
        }
      }
    }
  
    // NxM matrix
    b = new double * [N];
    for(int i = 0; i != N; i++)
    {
      b[i] = new double[M];
      if(myB != NULL)
      {
        for(int j = 0; j != M; j++)
        {
          b[i][j] = myB[i][j];
        }
      }
    }
  }
  else
  {
    a = NULL;
    b = NULL;
  }  
}

double ** 
TaggerData::getA()
{
  return a;
}

double ** 
TaggerData::getB()
{
  return b;
}

int 
TaggerData::getN()
{  
  return N;
}

int
TaggerData::getM()
{
  return M;
}

PatternList &
TaggerData::getPatternList()
{
  return plist;
}

void
TaggerData::setPatternList(PatternList const &pl)
{
  plist = pl;
}

void
TaggerData::read(FILE *in)
{
  destroy();

  // open_class
  int val = 0;
  for(int i = Compression::multibyte_read(in); i != 0; i--)
  {
    val += Compression::multibyte_read(in);
    open_class.insert(val);
  }
  
  // forbid_rules
  for(int i = Compression::multibyte_read(in); i != 0; i--)
  {
    TForbidRule aux;
    aux.tagi = Compression::multibyte_read(in);
    aux.tagj = Compression::multibyte_read(in);
    forbid_rules.push_back(aux);
  }

  
  // array_tags
  for(int i = Compression::multibyte_read(in); i != 0; i--)
  {
    array_tags.push_back(Compression::wstring_read(in));
  }
  
  // tag_index
  for(int i = Compression::multibyte_read(in); i != 0; i--)
  {
    wstring tmp = Compression::wstring_read(in);    
    tag_index[tmp] = Compression::multibyte_read(in);
  }

  // enforce_rules  
  for(int i = Compression::multibyte_read(in); i != 0; i--)
  {
    TEnforceAfterRule aux;
    aux.tagi = Compression::multibyte_read(in);
    for(int j = Compression::multibyte_read(in); j != 0; j--)
    {
      aux.tagsj.push_back(Compression::multibyte_read(in));
    }
    enforce_rules.push_back(aux);
  }

  // prefer_rules
  for(int i = Compression::multibyte_read(in); i != 0; i--)
  {
    prefer_rules.push_back(Compression::wstring_read(in));
  }

  // constants
  constants.read(in);

  // output
  output.read(in); 

  // dimensions
  N = Compression::multibyte_read(in);
  M = Compression::multibyte_read(in);

  
  a = new double * [N];
  b = new double * [N];
  for(int i = 0; i != N; i++)
  {
    a[i] = new double[N];
    b[i] = new double[M];
  }
   
  // read a
  for(int i = 0; i != N; i++)
  {
    for(int j = 0; j != N; j++)
    {
      a[i][j] = EndianUtil<double>::read(in);
    }
  }

  // initializing b matix
  for(int i = 0 ; i != N; i++)
  {
    for(int j = 0; j != M; j++)
    {
      b[i][j] = ZERO;
    }
  }

  // read nonZERO values of b
  int nval = Compression::multibyte_read(in);

  for(; nval != 0; nval--)
  {
    int i = Compression::multibyte_read(in);
    int j = Compression::multibyte_read(in);
    b[i][j] = EndianUtil<double>::read(in);
  }

  // read pattern list
  plist.read(in); 
}

void
TaggerData::write(FILE *out)
{
  
  // open_class
  Compression::multibyte_write(open_class.size(), out);  
  int val = 0;
  for(set<TTag>::const_iterator it = open_class.begin(), limit = open_class.end();
      it != limit; it++)
  {
    Compression::multibyte_write(*it-val, out);    
    val = *it;
  }
  
  // forbid_rules
  Compression::multibyte_write(forbid_rules.size(), out);
  for(unsigned int i = 0, limit = forbid_rules.size(); i != limit; i++)
  {
    Compression::multibyte_write(forbid_rules[i].tagi, out);
    Compression::multibyte_write(forbid_rules[i].tagj, out);
  }
  
  // array_tags
  Compression::multibyte_write(array_tags.size(), out);
  for(unsigned int i = 0, limit = array_tags.size(); i != limit; i++)
  {
    Compression::wstring_write(array_tags[i], out);
  }

  // tag_index
  Compression::multibyte_write(tag_index.size(), out);
  for(map<wstring, int, Ltstr>::iterator it = tag_index.begin(), limit = tag_index.end();
      it != limit; it++)
  {
    Compression::wstring_write(it->first, out);
    Compression::multibyte_write(it->second, out);
  }
  
  // enforce_rules
  Compression::multibyte_write(enforce_rules.size(), out);
  for(unsigned int i = 0, limit = enforce_rules.size(); i != limit; i++)
  {
    Compression::multibyte_write(enforce_rules[i].tagi, out);
    Compression::multibyte_write(enforce_rules[i].tagsj.size(), out);
    for(unsigned int j = 0, limit2 = enforce_rules[i].tagsj.size(); j != limit2; j++)
    {
      Compression::multibyte_write(enforce_rules[i].tagsj[j], out);
    }
  }

  // prefer_rules
  Compression::multibyte_write(prefer_rules.size(), out);
  for(unsigned int i = 0, limit = prefer_rules.size(); i != limit; i++)
  {
    Compression::wstring_write(prefer_rules[i], out);
  }
  
  // constants
  constants.write(out);  

  // output
  output.write(out);

  // a matrix
  Compression::multibyte_write(N, out);
  Compression::multibyte_write(M, out);
  for(int i = 0; i != N; i++)
  {
    for(int j = 0; j != N; j++)
    {
      EndianUtil<double>::write(out, a[i][j]);
    }
  }

  // b matrix, writing only useful values
  
  int nval = 0;
  for(int i = 0; i != N; i++)
  {
    for(int j = 0; j != M; j++)
    {
      if(output[j].find(i) != output[j].end())
      {
	nval++;
      }
    }
  }

  Compression::multibyte_write(nval, out);
  for(int i = 0; i != N; i++)
  {
    for(int j = 0; j != M; j++)
    {
      if(output[j].find(i) != output[j].end())
      {
	Compression::multibyte_write(i, out);
	Compression::multibyte_write(j, out);
	EndianUtil<double>::write(out, b[i][j]);
      }
    }
  }  
  
  // write pattern list
  plist.write(out);
}
