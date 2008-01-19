#include <apertium/TransferInstr.H>

void
TransferInstr::copy(TransferInstr const &o)
{
  type = o.type;
  content = o.content;
  pos = o.pos;
  pointer = o.pointer;
  condition = o.condition;
}

void
TransferInstr::destroy()
{
}

TransferInstr::TransferInstr(TransferInstrType t, string const &c, 
                             int const p, void *ptr, bool cond)
{
  type = t;
  content = c;
  pos = p;
  pointer = ptr;
  condition = cond;
}

TransferInstr::~TransferInstr()
{
  destroy();
}

TransferInstr::TransferInstr(TransferInstr const &o)
{
  copy(o);
}

TransferInstr &
TransferInstr::operator =(TransferInstr const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

TransferInstrType
TransferInstr::getType()
{
  return type;
}

string const &
TransferInstr::getContent()
{
  return content;
}

int
TransferInstr::getPos()
{
  return pos;
}

void *
TransferInstr::getPointer()
{
  return pointer;
}

bool
TransferInstr::getCondition()
{
  return condition;
}
