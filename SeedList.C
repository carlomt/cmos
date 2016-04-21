#include "SeedList.h"

ClassImp(SeedList);

SeedList::SeedList(const int idFrame)
  :TObject(),
   fIdFrame(idFrame)
{
}

SeedList::~SeedList(){}

void SeedList::Add(const Seed &s)
{
#ifdef DEBUG
 std::cout<<"SeedList::Add"<<std::endl;
#endif
 for(std::vector<Seed>::iterator it=fData.begin(); it<fData.end(); it++)
   {
     if(s> *it)
       {
#ifdef DEBUG
	 std::cout<<"Inserting a seed"<<std::endl<<std::endl;
#endif
	 fData.insert(it,s);
	 return;
       }
   }
#ifdef DEBUG
 std::cout<<"Inserting the first seed"<<std::endl<<std::endl;
#endif
 fData.push_back(s);
}
