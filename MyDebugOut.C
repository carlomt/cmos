#include "MyDebugOut.h"

MyDebugOut::MyDebugOut(const std::ostringstream &msg)
{
#ifdef DEBUG
  std::cout<<msg.str()<<std::endl;
#endif
}

MyDebugOut::MyDebugOut(const std::string &msg)
{
#ifdef DEBUG
  std::cout<<msg<<std::endl;
#endif
}
