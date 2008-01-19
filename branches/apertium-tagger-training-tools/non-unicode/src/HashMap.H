#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <iostream>

#include <string>
#include <sstream>
#include <fstream>

#ifdef __WIN32
  #include "stl_hash.h"
  #include "stl_set.h"
#else
  #if defined (__GNUC__) && ( __GNUC__ <= 2 )
    #include <hash_set>
    #include <hash_map>
  #else
    #include <ext/hash_map>
    #include <ext/hash_set>
    using namespace __gnu_cxx;
  #endif
#endif

using namespace std;

// Hash for strings (type string)
struct string_eqstr {
  bool operator()(const string &s1, const string &s2) const  {
    return s1 == s2;
  }
};

struct string_fnchash {
  hash<const char*> H;
  unsigned operator()(const string &s) const {
    return H(s.c_str());
  }
};
                
template <typename data> class HashStringMap : public hash_map<string, data, string_fnchash, string_eqstr> {
public:
  // Constructors
  HashStringMap() : hash_map<string, data, string_fnchash, string_eqstr>() {};
  HashStringMap(unsigned int n) : hash_map<string, data, string_fnchash, string_eqstr>(n) {};
};

template <typename data> class HashUnsignedMap : public hash_map<unsigned, data> {
public:
  // Constructors
  HashUnsignedMap() : hash_map<unsigned, data>() {};
  HashUnsignedMap(unsigned n) : hash_map<unsigned, data>(n) {};
};

template <typename data> class HashIntMap : public hash_map<int, data> {
public:
  // Constructors
  HashIntMap() : hash_map<int, data>() {};
  HashIntMap(int n) : hash_map<int, data>(n) {};
};

#endif

