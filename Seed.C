#include "Seed.h"
#include <cmath>

ClassImp(Seed);

Seed::Seed()
  :TObject(),
   fRow(0),
   fCol(0),
   fIdFrame(-99),
   fLastAddedPixel(0)
{
}

Seed::~Seed(){}

Seed::Seed(const size_t Col, const size_t Row, const std::vector<double> &val, const int idFrame)
  :TObject(),
   fRow(Row),
   fCol(Col),
   fIdFrame(idFrame),
   fLastAddedPixel(0)
{
  fData=val;
}



Seed::Seed(const size_t Col, const size_t Row, const int idFrame, const size_t size)
  :TObject(),
   fRow(Row),
   fCol(Col),
   fIdFrame(idFrame),
   fLastAddedPixel(0)
{
  fData.resize(size);
}

double Seed::operator()(const int posX, const int posY)
{
  size_t m=sqrt(fData.size()-1)/2;
  size_t k=posX+m + (posY+m)*m;
  return fData[k];
}

double Seed::operator()(const int posX, const int posY) const
{
  size_t m=sqrt(fData.size()-1)/2;
  size_t k=posX+m + (posY+m)*m;
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
  fData[fLastAddedPixel]=val;
  fLastAddedPixel++;
}

std::ostream& operator<< (std::ostream &out, Seed &CSeed)
{
  size_t m=sqrt(CSeed.fData.size());
  for(size_t j=0; j<m; j++)
    {
      for(size_t i=0; i<m; i++)
	{
	  out << CSeed(i,j) << " ";
	}
      out << std::endl;
    }
  return out;
}
