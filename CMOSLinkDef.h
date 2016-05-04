#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all extern;
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;

#pragma link C++ class Frame+;
#pragma link C++ function operator<< (std::ostream &out, Frame &Frame);

#pragma link C++ class Seed+;
#pragma link C++ function operator<< (std::ostream &out, Seed &Seed);

#pragma link C++ class SeedList+;

#pragma link C++ class Analisi+;
//#pragma link C++ function operator<< (std::ostream &out, Seed &Seed);

//#pragma link C++ global gROOT;
//#pragma link C++ global gEnv; 

#endif
