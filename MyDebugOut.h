#ifndef MyDebugOut_HH
#define MyDebugOut_HH

#include <iostream>

#define MyDbgMsg  std::ostringstream

void MyDebugOut(const std::ostringstream &msg) 
{
#ifdef DEBUG  
  std::cout<<msg.str()<<std::endl; 
#endif
  return;
}

void MyDebugOut(const std::string &msg) 
{
#ifdef DEBUG  
  std::cout<<msg<<std::endl; 
#endif
  return;
}


#endif
