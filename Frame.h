#ifndef Frame_HH
#define Frame_HH

#include <vector>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include "Rtypes.h"
#include "TObject.h"
#include "TH2F.h"

#include "Seed.h"
#include "SeedList.h"

//#define DIM 307200 //640*480


class Frame : public TObject
{
 public:
  Frame();
  Frame(const size_t nRow, const size_t nCol, const int Id=-99);
  Frame(const Frame &lval);
  virtual ~Frame();
  
  inline  size_t GetNRow() const {return fNRow;};
  inline  size_t GetNCol() const {return fNCol;};
  
  inline void SetId(const int Id){fId=Id;};
  inline  int GetId() const {return fId;};
  
  double operator()(const size_t i,const size_t j);
  double operator()(const size_t i,const size_t j) const;
  inline double At(const size_t i,const size_t j){return operator()(i,j);};
  inline double At(const size_t i,const size_t j) const {return operator()(i,j);};

  int ReadFile(const std::string filename, const bool isBinary=false);

  void Multiply(const Frame &LFrame);
  inline void Multiply(const Frame *LFrame){return Multiply(*LFrame);};
  void Multiply(const double val);
  void Add(const Frame &LFrame);
  inline void Add(const Frame *LFrame){return Add(*LFrame);};
  void Add(const double val);

  void Subtract(const Frame &LFrame);

  void Divide(const Frame &LFrame);

  void Resize(const size_t nRow, const size_t nCol);

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
  
  /* TH1F* GetTH1F(const char *name, const char *title); */
  /* TH1F* GetTH1F(std::string name, std::string title); */

  void Set(const size_t i, const size_t j, const double val=0);
  void SetAdd(const size_t i, const size_t j, const double val=0); //Added by collamaf

  SeedList FindSeeds(const double thres, const size_t fiducialSideDim=3,  const size_t seedSide=7, const size_t localMaximumCheckSide=3) const;

  size_t RemovePixelBelowThres(const int thres=0, const double val=0);

 private:
  size_t fNRow, fNCol;
  int fId;
  //  int fData[DIM];
	std::vector<double> fData;

 public:

  friend std::ostream& operator<< (std::ostream &out, Frame &CFrame);  

  ClassDef(Frame,1);
};

#endif /* Frame_HH */
