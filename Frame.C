#include "Frame.h"
#include "MyException.h"

ClassImp(Frame);

Frame::Frame()
:TObject(),
  fNRow(0),   
  fNCol(0)
{
}

Frame::Frame(const size_t nRow, const size_t nCol)
  :TObject(),
   fNRow(nRow),   
   fNCol(nCol)
{
  //  fData = new int[fNRow*fNCol];
  //  fData = new int[DIM];
#ifdef DEBUG
  std::cout<<"constr "<<nRow<<" "<<nCol<<std::endl;
  std::cout<<"constr "<<fNRow<<" "<<fNCol<<std::endl;
#endif

  fData.resize(fNRow*fNCol);
}

Frame::Frame(const Frame &lval)
  :TObject(),
   fNRow(lval.GetNRow()),   
   fNCol(lval.GetNCol())
{
  fData.resize(fNRow*fNCol);
  memcpy(&fData.at(0), &lval.fData.at(0), lval.fData.size());
}

Frame::~Frame(){};

double Frame::operator()(const size_t i,const size_t j)
{
  if(i<fNRow && j<fNCol)
    {
      size_t k=i+j*fNRow;
      return fData[k];
    }
  else
    {
      //      std::cerr<<"ERROR Frame::operator()"<<std::endl;
      throwException("Frame::operator()");
      return -99;
    }
}

//const
 double Frame::operator()(const size_t i,const size_t j) const
{
  if(i<fNRow && j<fNCol)
    {
      size_t k=i+j*fNRow;
      return fData[k];
    }
  else
    {
      std::cerr<<"ERROR Frame::operator()"<<std::endl;
      return -99;
    }
}

void Frame::Set(const size_t i, const size_t j, const double val)
{  
  if(i<fNRow && j<fNCol)
    {
      size_t k=i+j*fNRow;
      fData[k]=val;
    }
  else
    {
      std::cerr<<"ERROR Frame::Set()"<<std::endl;
      // std::cout<<"sizes "<<fNRow<<" "<<fNCol<<std::endl;
      // std::cout<<"pos   "<<i<<" "<<j<<std::endl;
    }
}

void Frame::Clear(Option_t *option)
{
  fData.clear();
}

TH2F* Frame::GetTH2F(const char *name, const char *title)
{
  TH2F* res=new TH2F(name,title,fNCol,0,fNCol, fNRow,0,fNRow);
  for(size_t i=0; i<fNCol; i++)
    {
      for(size_t j=0;j<fNRow; j++)
	{
	  res->SetBinContent(i+1,j+1, operator()(i,j));
	}
    }
  return res;
}

TH2F* Frame::GetTH2F(std::string name, std::string title)
{
  return GetTH2F(name.c_str(), title.c_str());
}

void Frame::Draw(Option_t *option)
{
  TH2F *tmp =GetTH2F("Frame_tmp","Frame");
  tmp->Draw(option);
}




void Frame::Multiply(const Frame &lval)
{
  if(fNRow!=lval.GetNRow() || fNCol!=lval.GetNCol())
    {
      std::cerr<<"ERROR Frame::Multiply"<<std::endl;
      std::cout<<"this sizes "<<fNRow<<" "<<fNCol<<std::endl;
      std::cout<<"L sizes "<<lval.GetNRow()<<" "<<lval.GetNCol()<<std::endl;
    }
  for(size_t i=0;i<fNRow;i++)
    {
      for(size_t j=0;j<fNCol;j++)
	{
	  //	  this->At(i,j)*lval(i,j);
	  Set(i,j, operator()(i,j)*lval(i,j));
	}
    }
}

void Frame::Multiply(const double val)
{
  for(size_t i=0;i<fNRow;i++)
    {
      for(size_t j=0;j<fNCol;j++)
	{
	  //	  this->At(i,j)*val;
	  Set(i,j, operator()(i,j)*val);
	}
    }
}

void Frame::Add(const Frame &lval)
{
  if(fNRow!=lval.GetNRow() || fNCol!=lval.GetNCol())
    {
      std::cerr<<"ERROR Frame::Add"<<std::endl;
      std::cout<<"this sizes "<<fNRow<<" "<<fNCol<<std::endl;
      std::cout<<"L sizes "<<lval.GetNRow()<<" "<<lval.GetNCol()<<std::endl;
    }
  for(size_t i=0;i<fNRow;i++)
    {
      for(size_t j=0;j<fNCol;j++)
	{
	  //	  At(i,j)+lval(i,j);
	  Set(i,j, operator()(i,j)+lval(i,j));
	}
    }
}

void Frame::Add(const double val)
{
 for(size_t i=0;i<fNRow;i++)
    {
      for(size_t j=0;j<fNCol;j++)
	{
	  //	  this->At(i,j)+val;
	  Set(i,j, operator()(i,j)+val);
	}
    }
}

Frame Frame::operator+(const Frame &LFrame) const
{
  Frame res(*this);
  res.Add(LFrame);
  return res;
}

Frame  Frame::operator+(const double val) const
{
  Frame res(*this);
  res.Add(val);
  return res;
}
  
// Frame  Frame::operator-(const Frame &LFrame)
// {
//   Frame res(*this);
//   res.Add(LFrame);
//   return res;
// }

//   Frame  Frame::operator-(const double val);
//   Frame  Frame::operator*(const Frame &LFrame);
//   Frame  Frame::operator*(const double val);
//   Frame  Frame::operator/(const Frame &LFrame);
//   Frame  Frame::operator/(const double val);



std::ostream& operator<< (std::ostream &out, Frame &CFrame)
{
  for(size_t i=0; i<CFrame.GetNRow(); i++)
    {
      for(size_t j=0;j<CFrame.GetNCol(); j++)
	{
	  out << CFrame(i,j) << " ";
	}
      out << std::endl;
    }
  return out;
}
