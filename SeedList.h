#ifndef SeedList_HH
#define SeedList_HH

#include <vector>
#include <iostream>

#include "Rtypes.h"
#include "TObject.h"

#include "Seed.h"

class SeedList : public TObject
{
 public:
  SeedList(const int idFrame=-99);
  virtual ~SeedList();
  
  Seed operator[](size_t i){return fData[i];};
  Seed operator[](size_t i) const {return fData[i];};
  
  inline Seed At(size_t i){return fData[i];};
  inline Seed At(size_t i) const {return fData[i];};
  
  Seed operator()(size_t i){return fData[i];};
  Seed operator()(size_t i) const {return fData[i];};
  
  void Add(const Seed &s);
    
  inline void SetIdFrame(const int idFrame){fIdFrame=idFrame;};
  inline int GetIdFrame(){return fIdFrame;};
  inline size_t GetSize() const {return fData.size();};
  inline size_t Size() const {return fData.size();};
  
  virtual void Clear(Option_t *option="") {fData.clear(); fIdFrame=-99;};
  
 private:
  int fIdFrame;
  std::vector<Seed> fData;
  
  ClassDef(SeedList,1);
};

#endif /* SeedList_HH */
