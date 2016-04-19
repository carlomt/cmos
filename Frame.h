#ifndef Frame_HH
#define Frame_HH

#include <vector>
#include <iostream>

#include "Rtypes.h"
#include "TObject.h"
#include "TH2F.h"

//#define DIM 307200 //640*480


class Frame : public TObject
{
 public:
  Frame();
  Frame(const size_t nRow, const size_t nCol);
  Frame(const Frame &lval);
  virtual ~Frame();
  
  inline  size_t GetNRow() const {return fNRow;};
  inline  size_t GetNCol() const {return fNCol;};
  
  double operator()(const size_t i,const size_t j);
   double operator()(const size_t i,const size_t j) const;
  inline double At(const size_t i,const size_t j){return operator()(i,j);};

  void Multiply(const Frame &LFrame);
  void Multiply(const double val);
  void Add(const Frame &LFrame);
  void Add(const double val);

  Frame operator+(const Frame &LFrame) const;
  Frame operator+(const double val) const;
  /* Frame operator-(const Frame &LFrame) const; */
  /* Frame operator-(const double val) const; */
  /* Frame operator*(const Frame &LFrame) const; */
  /* Frame operator*(const double val) const; */
  /* Frame operator/(const Frame &LFrame) const; */
  /* Frame operator/(const double val) const; */

  virtual void Clear(Option_t *option="");

  void Draw(Option_t *option="");
  TH2F* GetTH2F(const char *name, const char *title);
  TH2F* GetTH2F(std::string name, std::string title);
  
  void Set(const size_t i, const size_t j, const double val=0);

 private:
  size_t fNRow, fNCol;
  //  int fData[DIM];
  std::vector<double> fData;

 public:

  friend std::ostream& operator<< (std::ostream &out, Frame &CFrame);  

  ClassDef(Frame,1);
};

#endif /* Frame_HH */
