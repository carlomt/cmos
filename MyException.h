#ifndef MyException_HH
#define MyException_HH

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <execinfo.h>
#include <stdlib.h>

class MyException : public std::runtime_error 
{
  std::string msg;

 public:
 MyException(const std::string &arg, const char *file, int line) :
  std::runtime_error(arg) 
    {
      //      handler();
      std::ostringstream o;
      o << file << ":" << line << ": " << arg <<std::endl;
      o << "Backtrace: " << std::endl;
      msg = o.str()+handler();
    };

  ~MyException() throw() {};
  const char *what() const throw() 
  {
    return msg.c_str();
  };

  std::string handler()
    {
      void *trace_elems[20];
      int trace_elem_count(backtrace( trace_elems, 20 ));
      char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
      std::ostringstream o;
      for ( int i = 0 ; i < trace_elem_count ; ++i )
	{
	  o << stack_syms[i] << std::endl;
	}
      free( stack_syms );

      //    exit(1);
      return o.str();
    };   
  
};

#define throwException(arg) throw MyException(arg, __FILE__, __LINE__);


#endif
