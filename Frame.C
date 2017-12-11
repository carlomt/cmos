#include "Frame.h"
#include "MyException.h"
#include "MyDebugOut.h"

#include <fstream>


ClassImp(Frame);

Frame::Frame()
:TObject(),
fNRow(0),
fNCol(0),
fId(0)
{
}

Frame::Frame(const size_t nRow, const size_t nCol, const int Id)
:TObject(),
fNRow(nRow),
fNCol(nCol),
fId(Id)
{
    //  fData = new int[fNRow*fNCol];
    //  fData = new int[DIM];
#ifdef DEBUG
    std::cout<<"constr "<<nRow<<" "<<nCol<<std::endl;
#endif
    
    fData.resize(fNRow*fNCol);
#ifdef DEBUG
    cout<<"PRE INIZ "<<endl;
    for (int ss=0; ss<fNRow*fNCol; ss++){
        cout<<fData.at(ss)<<endl;
    }
#endif	
}

Frame::Frame(const Frame &lval)
:TObject(),
fNRow(lval.GetNRow()),
fNCol(lval.GetNCol()),
fId(lval.GetId())
{
    fData.resize(fNRow*fNCol);
    memcpy(&fData.at(0), &lval.fData.at(0), lval.fData.size());
}

Frame::~Frame(){};

double Frame::operator()(const size_t i,const size_t j)
{
    if(i<fNCol && j<fNRow)
    {
        size_t k=i+j*fNCol;
        return fData[k];
    }
    else
    {
        //      std::cerr<<"ERROR Frame::operator()"<<std::endl;
        std::ostringstream msg;
        msg<<"Frame::operator() NRow: "<<fNRow<<" NCol: "<<fNCol<<" requested: "<<i<<" "<<j;
        throwException(msg.str().c_str());
        return -99;
    }
}

int Frame::ReadFile(const std::string filename,  const bool isBinary)
{     
#ifdef DEBUG
    std::cout<<" Frame::ReadFile reading "<<filename<<std::endl;
#endif
    
    if(fNRow==0 || fNCol==0)
    {
        std::ostringstream msg;
        msg<<"Frame::ReadFile the number of row or col is 0, did you initialized them?";
        throwException(msg.str().c_str());
    }
    
    std::ifstream reader;
    
    int filelen;
    FILE * infile;
    //std::cout<<"Is file binary? "<< isBinary <<std::endl;
    
    if(isBinary)
    {
        reader.open(filename.c_str(), std::ios::binary);
        //std::cout<<"Reading binary file"<<std::endl;
        
        const char *cstr = filename.c_str();
        infile = fopen(cstr,"rb");
        fseek(infile, 0, SEEK_END);
        filelen = ftell(infile);
        rewind(infile);
        //std::cout << filelen << " read values (filelen)" <<std::endl;
    }
    else
    {
        //std::cout <<"File is not binary" <<std::endl;
        reader.open(filename.c_str(),std::ios_base::in);
    }
    
    if(!reader.is_open())
    {
        std::ostringstream msg;
        msg<<"Frame::ReadFile input file: "<<filename;
        throwException(msg.str().c_str());
    }
    int counter=0;
    unsigned int tmp_ptr1;
    unsigned int tmp_ptr2;
    unsigned int tmp_value1, tmp_value2;
    //rewind(infile);
    int kk = 0;
    unsigned char *buffer = (unsigned char*)malloc(filelen);
    if(isBinary){
        int value = fread(buffer,filelen,sizeof(unsigned char),infile);
        //std::cout <<"int value: "<< value <<std::endl;
    }
    
    for(size_t j=0; j<fNRow; j++)
    {
        for(size_t i=0; i<fNCol; i++)
        {
            double tmp=-99;
            if(isBinary){
                tmp_ptr1 = (unsigned int)buffer[kk];
                kk = kk+1;
                tmp_ptr2 = (unsigned int)buffer[kk];
                kk = kk+1;
                tmp_value2 = tmp_ptr2;
                tmp_value1 = tmp_ptr1;
                tmp = tmp_value2*256 + tmp_value1;
                //std::cout <<"tmp variable: "<< tmp <<std::endl;
            }
            /*if(reader.eof())
             {
             std::ostringstream msg1;
             msg1<<"Frame::ReadFile input file  "<<filename;
             msg1<<"shorter than expected ("<<fNRow*fNCol<<")";
             msg1<<counter<<" values have been red";
             throwException(msg1.str().c_str());
             }*/
            else{
                //std::cout<<"File is not binary"<<std::endl;
                reader >> tmp;
                //std::cout << "tmp variable: "<<tmp<<std::endl;
            }
            
            Set(i,j,tmp);
            if(tmp>=157)
            {
                std::ostringstream msg2;
                msg2<<"Frame::ReadFile "<<i<<" "<<j<<"  in value: "<<tmp;
                msg2<<"  out value: "<<At(i,j);
                
                
                
                
#ifdef DEBUG
                std::cout<<msg2.str()<<std::endl;
#endif
                
            }
            counter++;
        }
    }
    reader.close();
    return counter;
}

//const
double Frame::operator()(const size_t i,const size_t j) const
{
    if(i<fNCol && j<fNRow)
    {
        size_t k=i+j*fNCol;
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
    if(i<fNCol && j<fNRow)
    {
        size_t k=i+j*fNCol;
        fData[k]=val;
    }
    else
    {
        std::cerr<<"ERROR Frame::Set()"<<std::endl;
        // std::cout<<"sizes "<<fNRow<<" "<<fNCol<<std::endl;
        // std::cout<<"pos   "<<i<<" "<<j<<std::endl;
    }
}

void Frame::SetAdd(const size_t i, const size_t j, const double val)
{
    if(i<fNCol && j<fNRow)
    {
        size_t k=i+j*fNCol;
        fData[k]+=val;
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
    fData.resize(fNRow*fNCol);
    fId=-99;
}

TH2F* Frame::GetTH2F(const char *name, const char *title)
{
    TH2F* res=new TH2F(name,title,fNCol,0,fNCol, fNRow,0,fNRow);
    for(size_t j=0; j<fNRow; j++)
    {
        for(size_t i=0;i<fNCol; i++)
        {
            //	  std::cout<<i<<" "<<j<<std::endl;
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
    for(size_t j=0; j<fNRow; j++)
    {
        for(size_t i=0; i<fNCol; i++)
        {
            //	  this->At(i,j)*lval(i,j);
            Set(i,j, operator()(i,j)*lval(i,j));
        }
    }
}

void Frame::Multiply(const double val)
{
    for(size_t j=0; j<fNRow; j++)
    {
        for(size_t i=0; i<fNCol; i++)
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
    for(size_t i=0; i<fData.size(); i++)
    {
        fData[i]+=lval.fData[i];
    }
}

void Frame::Add(const double val)
{
    for(size_t i=0; i<fData.size(); i++)
    {
        fData[i]+=val;
    }
}

void Frame::Subtract(const Frame &LFrame)
{
    for(size_t i=0; i<fData.size(); i++)
    {
        fData[i]-=LFrame.fData[i];
    }
}

void Frame::Divide(const Frame &LFrame)
{
    for(size_t i=0; i<fData.size(); i++)
    {
        fData[i]/=LFrame.fData[i];
    }
}


void Frame::Resize(const size_t nRow, const size_t nCol)
{
    fNRow=nRow;
    fNCol=nCol;
    fData.resize(fNRow*fNCol);
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

size_t Frame::RemovePixelBelowThres(const int thres, const double val)
{
    size_t counter=0;
    for(size_t k=0; k<fData.size(); k++)
    {
        if(fData[k]<thres)
        {
            counter++;
            fData[k]=val;
        }
    }
    return counter;
}

SeedList Frame::FindSeeds(const double thres, const size_t fiducialSideDim,  const size_t seedSide, const size_t localMaximumCheckSide) const
{
#ifdef DEBUG
    std::cout<<" Frame::FindSeeds "<<std::endl;
    std::cout<<" Frame ID: "<<fId<<std::endl;
    std::cout.flush();
#endif
    //  SeedList *res=new SeedList(fId);
    SeedList res(fId);
#ifdef DEBUG
    std::cout<<" res malloc "<<std::endl;
    std::cout.flush();
#endif
    
    for(size_t j=fiducialSideDim; j<(fNRow-fiducialSideDim); j++)
    {
        for(size_t i=fiducialSideDim; i<(fNCol-fiducialSideDim); i++)
        {
#ifdef DEBUG
            std::cout<<" Frame::FindSeeds i "<<i<<" j "<<j<<std::endl;
            std::cout<<" fNCol "<<fNCol<< " fNRow "<<fNRow<<std::endl;
            std::cout.flush();
#endif
            
            if( At(i,j) > thres )
            {
                bool addThis=true;
                double thisCandidate = this->At(i,j);
#ifdef DEBUG
                std::cout<<" Frame::FindSeeds thisCandidate: "<<thisCandidate<<std::endl;
                std::cout.flush();
#endif
                //check neighbours
                const size_t lim=(localMaximumCheckSide-1)/2;
                for(size_t jj=(j-lim); jj<=(j+lim); jj++)
                {
                    for(size_t ii=(i-lim); ii<=(i+lim); ii++)
                    {
                        if(thisCandidate < this->At(ii,jj))
                        {
                            addThis=false;
                        }
                    }
                }//end check neghbours
                if(addThis)
                {
                    Seed tmp(i,j,fId,seedSide);
                    size_t step=(seedSide-1)/2;
#ifdef DEBUG
                    std::cout<<std::endl<<"Adding a seed "<<thisCandidate <<std::endl;
                    std::cout<<"step: "<<step<<std::endl;
                    std::cout.flush();
#endif
                    
                    for(size_t jj=(j-step); jj<=(j+step); jj++)
                    {
                        for(size_t ii=(i-step); ii<=(i+step); ii++)
                        {
                            double ttt=At(ii,jj);
#ifdef DEBUG
                            std::cout<<" adding the value "<<ttt<<std::endl;
                            std::cout.flush();
#endif
                            tmp.AddPixel(ttt);
                        }
                    }
                    
                    res.Add(tmp);
                }
            }//end if At(i,j) > thres
#ifdef DEBUG
            std::cout<<" Frame::FindSeeds this value: "<<this->At(i,j)<<std::endl;
            std::cout.flush();
#endif
        }//end for i
    }//end for j
#ifdef DEBUG
    //  std::cout<<" Frame::FindSeeds returning: "<<res->Size()<<std::endl;
    std::cout<<" Frame::FindSeeds returning: "<<res.Size()<<std::endl;
    std::cout.flush();
#endif
    return res;
}


std::ostream& operator<< (std::ostream &out, Frame &CFrame)
{
    for(size_t j=0; j<CFrame.GetNRow(); j++)
    {
        for(size_t i=0; i<CFrame.GetNCol(); i++)
        {
            out << CFrame(i,j) << " ";
        }
        out << std::endl;
    }
    return out;
}
