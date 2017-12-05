#include <string>
#include <iostream>
#include <vector>

std::vector<std::string> string_parser(std::string in, std::string delimiter);



std::vector<std::string> string_parser(std::string s, std::string delimiter)
{
  std::vector<std::string> res;
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    res.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  res.push_back(s);

  return(res);
}
