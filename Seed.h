#ifndef Seed_HH
#define Seed_HH

#include <vector>
#include <iostream>

#include "Rtypes.h"
#include "TObject.h"

class Seed : public TObject
{
 public:
  Seed();
  Seed(const size_t Col, const size_t Row, const std::vector<double> &val, const int idFrame=-99);
  Seed(const size_t Col, const size_t Row, const int idFrame=-99, const size_t size=49);
  virtual ~Seed();

double operator()(const int posX, const int posY); // position with respect to the relative maximum i.e. 0,0 is the center 
double operator()(const int posX, const int posY) const;

inline double At(const int posX, const int posY){return operator()(posX,posY);};
inline double At(const int posX, const int posY) const {return operator()(posX,posY);};

bool operator>(const Seed &l) const;
bool operator<(const Seed &l) const;
bool operator==(const Seed &l) const;
  
void Set(const size_t Col, const size_t Row, const std::vector<double> &val, const int idFrame=-99);
void Set(const size_t Col, const size_t Row, const int idFrame=-99);

void AddPixel(const double val);//add a pixel to the seed, it starts from upper left

inline int GetIdFrame() const {return fIdFrame;};
inline size_t GetRow() const {return fRow;};
inline size_t GetCol() const {return fCol;};

private:
size_t fRow, fCol;
std::vector<double> fData;
int fIdFrame;
size_t fLastAddedPixel;

 public:
  
  friend std::ostream& operator<< (std::ostream &out, Seed &CSeed);  
  
  ClassDef(Seed,1);
};

#endif /* Seed_HH */