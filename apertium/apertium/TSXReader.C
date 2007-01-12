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
#include <apertium/TSXReader.H>
#include <lttoolbox/XMLParseUtil.H>
#include <lttoolbox/Compression.H>
#include <apertium/StringUtils.H>

#include <cstdlib>
#include <iostream>

void
TSXReader::copy(TSXReader const &o)
{
}

void
TSXReader::destroy()
{
}

TSXReader::TSXReader()
{
  open_class = &(tdata.getOpenClass());
  forbid_rules = &(tdata.getForbidRules());
  tag_index = &(tdata.getTagIndex());
  array_tags = &(tdata.getArrayTags());
  enforce_rules = &(tdata.getEnforceRules());
  prefer_rules = &(tdata.getPreferRules());
  plist = &(tdata.getPatternList());
  constants = &(tdata.getConstants());
}

TSXReader::~TSXReader()
{
  destroy();
}

TSXReader::TSXReader(TSXReader const &o)
{
  copy(o);
}


void
TSXReader::clearTagIndex()
{
  tag_index->clear();
  array_tags->clear();
  newTagIndex("LPAR");
  newTagIndex("RPAR");
  newTagIndex("LQUEST");
  newTagIndex("CM");
  newTagIndex("SENT");
  newTagIndex("kEOF");
  newTagIndex("kUNDEF");
}

void
TSXReader::step()
{
  int retval = xmlTextReaderRead(reader);
  if(retval != 1)
  {
    parseError("unexpected EOF");
  }
  name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
  type = xmlTextReaderNodeType(reader);
}

TSXReader &
TSXReader::operator =(TSXReader const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

string
TSXReader::attrib(string const &name)
{
  return XMLParseUtil::attrib(reader, name);
} 

void
TSXReader::parseError(string const &message)
{
  cerr << "Error: (" << xmlTextReaderGetParserLineNumber(reader);
  cerr << "): " << message << "." << endl;
  exit(EXIT_FAILURE);
}

void
TSXReader::newTagIndex(string const &tag)
{
  if(tag_index->find("TAG_" + tag) != tag_index->end())
  {
    parseError("'" + tag + "' already defined");
  }

  array_tags->push_back("TAG_" + tag);
  (*tag_index)["TAG_" + tag] = array_tags->size() - 1;
}

void
TSXReader::newDefTag(string const &tag)
{
  if(tag_index->find("TAG_" + tag) != tag_index->end())
  {
    parseError("'" + tag + "' already defined");
  }

  array_tags->push_back(tag);
  (*tag_index)["TAG_" + tag] = array_tags->size() - 1;
}

void
TSXReader::newConstant(string const &constant)
{
  constants->setConstant(constant, array_tags->size());
  array_tags->push_back(constant);
}

void
TSXReader::procDefLabel()
{
  string name_attr = attrib("name");
  string closed_attr = attrib("closed");
  newDefTag(name_attr);

  if(closed_attr != "true")
  {
    open_class->insert((*tag_index)["TAG_"+name_attr]);
  }

  while(type != XML_READER_TYPE_END_ELEMENT || name != "def-label")
  {
    step();

    if(name == "tags-item")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	plist->insert((*tag_index)["TAG_"+name_attr], attrib("lemma"),
		     attrib("tags"));
      }
    }
    else if(name == "def-label")
    {
      return;
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else
    {
      parseError("unexpected '<" + name + ">' tag");
    }
  }
}

void
TSXReader::procDefMult()
{
  string name_attr = attrib("name");
  string closed_attr = attrib("closed");
  newDefTag(name_attr);
  if(closed_attr != "true")
  {
    open_class->insert((*tag_index)["TAG_"+name_attr]);
  }

  while(type != XML_READER_TYPE_END_ELEMENT || name != "def-mult")
  {
    step();
    if(name == "sequence")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	plist->beginSequence();
	while(type != XML_READER_TYPE_END_ELEMENT || name != "sequence")
	{
	  step();
	  if(name == "label-item")
	  {
	    if(type != XML_READER_TYPE_END_ELEMENT)
	    {
	      plist->insert((*tag_index)["TAG_"+name_attr],
                            (*tag_index)["TAG_"+attrib("label")]);
	    }
	  }
	  else if(name == "tags-item")
	  {
	    if(type != XML_READER_TYPE_END_ELEMENT)
	    {
	      plist->insert((*tag_index)["TAG_"+name_attr],
			    attrib("lemma"), attrib("tags"));
	    }
	  }
	  else if(name == "sequence")
	  {
	    break;
	  }
	  else if(name == "#text")
	  {
	    // do nothing
	  }
	  else if(name == "#comment")
	  {
	    // do nothing
          }
	}
	plist->endSequence();
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "def-mult")
    { 
      // do nothing
    }
    else
    {
      parseError("unexpected '<" + name + ">' tag");
    }
  }
}

void
TSXReader::procTagset()
{ 
  while(type == XML_READER_TYPE_END_ELEMENT || name != "tagset")
  {
    step();
    if(name != "#text" && name != "tagger" && name != "tagset")
    {
      parseError("'<" + name + ">' tag unexpected");
    }
  }
  
  while(type != XML_READER_TYPE_END_ELEMENT || name != "tagset")
  {
    step();
    if(name == "def-label")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	procDefLabel();
      }
    }
    else if(name == "def-mult")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        procDefMult();
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "tagset")
    {
      // do nothing
    }
    else
    {
      parseError("Unexpected '<" + name + ">' tag");
    }
  }
}


void
TSXReader::procLabelSequence()
{
  TForbidRule forbid_rule;

  step();
  while(name == "#text" || name == "#comment")
  {
    step();
  }
  if(name != "label-item")
  {
    parseError("<label-item> tag expected");
  }
  
  forbid_rule.tagi = (*tag_index)["TAG_" + attrib("label")];

  step();
  while(name == "#text" || name == "#comment")
  {
    step();
  }
  if(name != "label-item")
  {
    parseError("<label-item> tag expected");
  }
  forbid_rule.tagj = (*tag_index)["TAG_" + attrib("label")];
  
  forbid_rules->push_back(forbid_rule);
}

void
TSXReader::procForbid()
{
  while(type != XML_READER_TYPE_END_ELEMENT || name != "forbid")
  {
    step();
    if(name == "label-sequence")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	procLabelSequence();
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "forbid")
    {
      if(type == XML_READER_TYPE_END_ELEMENT)
      {
	break;
      }
      else
      {
	parseError("Unexpected '" + name + "' open tag");
      }
    }
    else
    {
      parseError("Unexpected '" + name + "' tag");
    }
  }  
}

void
TSXReader::procEnforce()
{
  TEnforceAfterRule aux;
  while(type != XML_READER_TYPE_END_ELEMENT || name != "enforce-rules")
  {
    step();
    if(name == "enforce-after")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	aux.tagi = (*tag_index)["TAG_" + attrib("label")];
      }
      else
      {
	enforce_rules->push_back(aux);
	aux.tagsj.clear();
      }
    }
    else if(name == "label-set")
    {
      // do nothing
    }
    else if(name == "label-item")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	aux.tagsj.push_back((*tag_index)["TAG_" + attrib("label")]);
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "enforce-rules")
    {
      if(type == XML_READER_TYPE_END_ELEMENT)
      {
	break;
      }
      else
      {
	parseError("Unexpected 'enforce-rules' open tag");
      }
    }
    else
    {
      parseError("Unexpected '" + name + "' tag");
    }
  }
}

void
TSXReader::procPreferences()
{
  while(type != XML_READER_TYPE_END_ELEMENT || name != "preferences")
  {
    step();
    if(name == "prefer")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        string const tags = "<" + StringUtils::substitute(attrib("tags"), ".", "><") + ">";
	prefer_rules->push_back(tags);
      }
    }
    else if(name == "#text")
    {
      //do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "preferences")
    {
      if(type == XML_READER_TYPE_END_ELEMENT)
      {
	break;
      }
      else
      {
	parseError("Unexpected 'preferences' open tag");
      }
    }
    else
    {
      parseError("Unexpected '" + name + "' tag");
    }
  }
}

void
TSXReader::read(string const &filename)
{
  reader = xmlReaderForFile(filename.c_str(), NULL, 0);
  if(reader == NULL)
  {
    cerr << "Error: Cannot open '" << filename << "'." << endl;
    exit(EXIT_FAILURE);
  }

  open_class->clear();
  forbid_rules->clear();
  clearTagIndex();
  enforce_rules->clear();

  procTagset();

  step();
  while(name == "#text" || name == "#comment")
  {
    step();
  }
  if(name == "forbid")
  {
    procForbid();
    step();
    while(name == "#text" || name == "#comment")
    {
      step();
    }
  }
  if(name == "enforce-rules")
  {
    procEnforce();
    step();
    while(name == "#text" || name == "#comment")
    {
      step();
    }
  }
  if(name == "preferences")
  {
    procPreferences();
  }

  xmlFreeTextReader(reader);
  xmlCleanupParser();

  newConstant("kMOT");
  newConstant("kDOLLAR");
  newConstant("kBARRA");
  newConstant("kMAS");
  newConstant("kIGNORAR");
  newConstant("kBEGIN");
  newConstant("kUNKNOWN");
  
  plist->insert((*tag_index)["TAG_LPAR"], "", "lpar");
  plist->insert((*tag_index)["TAG_RPAR"], "", "rpar");
  plist->insert((*tag_index)["TAG_LQUEST"], "", "lquest");
  plist->insert((*tag_index)["TAG_CM"], "", "cm");
  plist->insert((*tag_index)["TAG_SENT"], "", "sent");
//  plist->insert((*tag_index)["TAG_kMAS"], "+", "");
  plist->buildTransducer();
}

void
TSXReader::write(string const &filename)
{
  FILE *out = fopen(filename.c_str(), "w");
  if(!out)
  {
    cerr << "Error: cannot open '" << filename;
    cerr << "' for writing" << endl;
    exit(EXIT_FAILURE);
  }

  tdata.write(out);

  fclose(out);
}

TaggerData &
TSXReader::getTaggerData()
{
  return tdata;
}
