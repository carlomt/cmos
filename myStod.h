#ifndef myStod_hpp
#define myStod_hpp
#include <sstream>      // std::istringstream
#include <string>

double myStod(const std::string s)
{
  double res;
  std::istringstream(s) >> res;
  return res;
};

#endif
