#ifndef _TRANSFERINSTR_
#define _TRANSFERINSTR_

#include<string>

using namespace std;

enum TransferInstrType
{
  ti_clip_sl,
  ti_clip_tl,
  ti_var,
  ti_lit_tag,
  ti_lit,
  ti_b,
  ti_get_case_from,
  ti_case_of_sl,
  ti_case_of_tl,
  ti_linkto_sl,
  ti_linkto_tl
};

class TransferInstr
{
private:
  TransferInstrType type;
  string content;
  int pos;
  void *pointer;
  bool condition;
  
  void copy(TransferInstr const &o);
  void destroy();
public:
  TransferInstr(){};
  TransferInstr(TransferInstrType t, string const &c, int const p, 
                void *ptr=NULL, bool cond = true);
  ~TransferInstr();
  TransferInstr(TransferInstr const &o);
  TransferInstr & operator =(TransferInstr const &o);
  
  
  TransferInstrType getType();
  string const & getContent();
  int getPos();
  void * getPointer();
  bool getCondition();
};

#endif
