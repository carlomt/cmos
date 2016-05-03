#include "Seed.h"
#include "MyException.h"

#include <cmath>
#include <sstream>

ClassImp(Seed);

Seed::Seed()
  :TObject(),
   fRow(0),
   fCol(0),
   fSideSize(0),
   fIdFrame(-99),
   fLastAddedPixel(0)
{
}

Seed::~Seed(){}

Seed::Seed(const size_t Col, const size_t Row, const std::vector<double> &val, const int idFrame)
  :TObject(),
   fRow(Row),
   fCol(Col),
   fSideSize(val.size()),
   fIdFrame(idFrame),
   fLastAddedPixel(0)
{
  fData=val;
}



Seed::Seed(const size_t Col, const size_t Row, const int idFrame, const int size)
  :TObject(),
   fRow(Row),
   fCol(Col),
   fSideSize(size),
   fIdFrame(idFrame),
   fLastAddedPixel(0)
{
  fData.resize(size*size);
}

void Seed::Clear(Option_t *option)
{
  fData.clear();
  fData.resize(fSideSize*fSideSize);
}

double Seed::operator()(const int i, const int j)
{
  //  size_t m=sqrt(fData.size()-1)/2;
  int m=(fSideSize-1)/2;
  int x=i+m;
  int y=j+m;
  int k=x + y*fSideSize;
#ifdef DEBUG
  std::cout<<" Seed::operator() "<<i<<" "<<j<<" "<<k<<" "<<fData[k]<<std::endl;
  std::cout<<" x "<<x<<" y "<<y<<std::endl;
  std::cout<<" fSideSize: "<<fSideSize<<" m "<<m<<std::endl;
#endif
  if(i<GetPixelMin() || i>GetPixelMax())
    {
      std::ostringstream msg;
      msg<<"Seed::operator() Min: "<<GetPixelMin()<<" Max: "<<GetPixelMax()<<" i: "<<i;
      throwException(msg.str().c_str());
    }
  if(j<GetPixelMin() || j>GetPixelMax())
    {
      std::ostringstream msg;
      msg<<"Seed::operator() Min: "<<GetPixelMin()<<" Max: "<<GetPixelMax()<<" j: "<<j;
      throwException(msg.str().c_str());
    }
  return fData[k];
}

double Seed::operator()(const int i, const int j) const
{
  //  size_t m=sqrt(fData.size()-1)/2;
  int m=(fSideSize-1)/2;
  int x=i+m;
  int y=j+m;
  int k=x + y*fSideSize;
#ifdef DEBUG
  std::cout<<" Seed::operator() "<<i<<" "<<j<<" "<<k<<" "<<fData[k]<<std::endl;
  std::cout<<" x "<<x<<" y "<<y<<std::endl;
  std::cout<<" fSideSize: "<<fSideSize<<" m "<<m<<std::endl;
#endif
  if(i<GetPixelMin() || i>GetPixelMax())
    {
  std::ostringstream msg;
  msg<<"Seed::operator() Min: "<<GetPixelMin()<<" Max: "<<GetPixelMax()<<" i: "<<i;
  throwException(msg.str().c_str());
}
  if(j<GetPixelMin() || j>GetPixelMax())
    {
      std::ostringstream msg;
      msg<<"Seed::operator() Min: "<<GetPixelMin()<<" Max: "<<GetPixelMax()<<" j: "<<j;
      throwException(msg.str().c_str());
    }
  return fData[k];
}

bool Seed::operator>(const Seed &l) const
{
  return (this->At(0,0)>l(0,0));
}

bool Seed::operator<(const Seed &l) const
{
  return (this->At(0,0)<l(0,0));
}

bool Seed::operator==(const Seed &l) const
{
  if(fIdFrame==l.GetIdFrame() && fRow==l.GetRow() && fCol==l.GetCol())
    {
      return true;
    }
  return false;
}


void Seed::Set(const size_t Col, const size_t Row, const std::vector<double> &val, const int idFrame)
{
  fCol=Col;
  fRow=Row;
  fData=val;
  fIdFrame=idFrame;
}

void Seed::Set(const size_t Col, const size_t Row, const int idFrame)
{
  fCol=Col;
  fRow=Row;
  fIdFrame=idFrame;
}


void Seed::AddPixel(const double val)
{
#ifdef DEBUG
  std::cout<<" Seed::AddPixel "<<fLastAddedPixel<<" "<<val<<std::endl;
#endif
  fData[fLastAddedPixel]=val;
  fLastAddedPixel++;
}

std::ostream& operator<< (std::ostream &out, Seed &CSeed)
{
  for(int j=CSeed.GetPixelMin(); j<CSeed.GetPixelMax(); j++)
    {
  for(int i=CSeed.GetPixelMin(); i<CSeed.GetPixelMax(); i++)
    {
	  out << CSeed(i,j) << " ";
	}
      out << std::endl;
    }
  return out;
}
