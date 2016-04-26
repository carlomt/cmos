#ifndef myStoi_hpp
#define myStoi_hpp
#include <sstream>      // std::istringstream
#include <string>

int myStoi(const std::string s)
{
  int res;
  std::istringstream(s) >> res;
  return res;
};


#endif
