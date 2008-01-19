#include <lttoolbox/XMLParseUtil.H>

string
XMLParseUtil::attrib(xmlTextReaderPtr reader, string const &name)
{
  xmlChar *attrname = xmlCharStrdup(name.c_str());
  xmlChar *myattr = xmlTextReaderGetAttribute(reader, attrname);
  string result = latin1(myattr);
  xmlFree(myattr);
  xmlFree(attrname);
  return result;
}

string
XMLParseUtil::latin1(xmlChar const *input)
{
 if(input == NULL)
  {
    return "";
  }

  int outputlen = xmlStrlen(input) + 1;
  int inputlen = xmlStrlen(input);

  unsigned char output[outputlen];
  
  if(UTF8Toisolat1(output, &outputlen, input, &inputlen) != 0)
  {
  }

  output[outputlen] = 0;
  string result = reinterpret_cast<char *>(output);
  return result;  
}
