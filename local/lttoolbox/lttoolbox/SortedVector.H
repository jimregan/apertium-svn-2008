#ifndef _SORTEDVECTOR_
#define _SORTEDVECTOR_

class MatchNode;

class SortedVector
{
private:
  struct SVNode
  {
    int tag;
    MatchNode *dest;
  };
  SVNode *sv;
  int size;
  
  void copy(SortedVector const &o);
  void destroy();
public:
  SortedVector(int const fixed_size);
  ~SortedVector();
  SortedVector(SortedVector const &o);
  SortedVector & operator =(SortedVector const &o);
  
  void add(int tag, MatchNode *dest, int pos);
  MatchNode * search(int tag);
};

#endif
