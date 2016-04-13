#ifndef Frame_HH
#define Frame_HH

#include <vector>
#include <iostream>

#include "Rtypes.h"
#include "TObject.h"
#include "TH2F.h"

#define DIM 307200 //640*480


class Frame : public TObject
{
 public:
  Frame();
  Frame(const size_t nRow, const size_t nCol);
  virtual ~Frame();
  
  inline const size_t GetNRow(){return fNRow;};
  inline const size_t GetNCol(){return fNCol;};
  
  int operator()(const size_t i,const size_t j);

  void Draw(Option_t *option="");
  TH2F* GetTH2F(const char *name, const char *title);
  TH2F* GetTH2F(std::string name, std::string title);
  
 private:
  size_t fNRow, fNCol;
  //  int fData[DIM];
  std::vector<int> fData;

 public:

  friend std::ostream& operator<< (std::ostream &out, Frame &CFrame);  

  ClassDef(Frame,1);
};

#endif /* Frame_HH */
