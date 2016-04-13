#include "Frame.h"

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
  fData.resize(fNRow*fNCol);
}

Frame::~Frame(){};

int Frame::operator()(const size_t i,const size_t j)
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

TH2F* Frame::GetTH2F(const char *name, const char *title)
{
  TH2F* res=new TH2F(name,title,fNCol,0,fNCol, fNRow,0,fNRow);
  for(size_t i=0; i<fNCol; i++)
    {
      for(size_t j=0;j<fNRow; j++)
	{
	  res->SetBinContent(i,j, operator()(i,j));
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
