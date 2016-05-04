#ifndef MyDebugOut_HH
#define MyDebugOut_HH

#include <iostream>
#include <sstream>

//#define MyDbgMsg  std::ostringstream
class MyDebugOut
{
 public:
  MyDebugOut(const std::ostringstream &msg);
  MyDebugOut(const std::string &msg);
};

/* class MyDebugOut: public std::ostream */
/* { */
/*   friend std::ostream& operator<<(std::ostream &out, const std::string var); */
/*  private: */
  
/*   class StreamBuffer: public std::stringbuf */
/*    { */
/*    private: */
/*      std::ostream &out; */
/*      std::string prefix; */
     
/*    public: */
/*      StreamBuffer(std::ostream& str, const std::string &p); */
/*      virtual int sync(); */
/*    }; */
 
/*     StreamBuffer buffer; */
    
/*  public: */
/*     Output(const std::string &prefix); */
/* }; */


#endif
