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

Seed operator()(size_t i){return fData[i];};
Seed operator()(size_t i) const {return fData[i];};

void Add(const Seed &s);

inline size_t GetSize() const {return fData.size();};
inline size_t Size() const {return fData.size();};

private:
int fIdFrame;
std::vector<Seed> fData;

ClassDef(SeedList,1);
};

#endif /* SeedList_HH */
