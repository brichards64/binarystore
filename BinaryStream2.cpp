#include <BinaryStream2.h>

BinaryStream2::BinaryStream2(std::string filename, enum_mode method, enum_endpoint endpoint){


  m_mode=NEW;
  m_write=true;
  if(method==1){
  
    fwrapper=new FWrapper();    
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    Bopen= std::bind(&FWrapper::FOpen, fwrapper, _1, _2, _3);
    Bclose= std::bind(&FWrapper::FClose, fwrapper);
    Bread= std::bind(&FWrapper::FRead, fwrapper,_1,_2);
    Bwrite= std::bind(&FWrapper::FWrite, fwrapper,_1,_2);

  }

}


BinaryStream2::~BinaryStream2(){




}

/*
bool BinaryStream2::fwriteWapper(const void* in, unsigned int size){

  FILE* pfile;

  return fwrite(in , size, 1, pfile);

}
*/
/*
bool BinaryStream2::gzwriteWapper(const void* in, unsigned int size){

  int pfile;

return size==gzwrite(*gzfile, in, size);

}
*/

int main(){

  BinaryStream2 bs("hello", NEW, UNCOMPRESSED);
  bs.Bopen("hello", NEW, UNCOMPRESSED);

  std::vector<int> v;

  for(int i=0; i<100000000; i++){
    bs<<i;
    //    v.push_back(i);

  }

  //  bs<<v;

  bs.Bclose();

  return 0;
}

bool SerialisableObject::SerialiseWrapper(BinaryStream2 &bs){
  //  if(!m_serialise) return false; //not sure i should ahve a serialise flag, causes major issues with empty mapps and vector elements!!!! so remove. People wouldnt be calling serialise method if they didnt want to serialse                                                        //std::cout<<"in serialise wrapper"<<std::endl;                                                                                                             
  if(bs.m_write){
  if(!(bs << GetVersion())) return false;
}
 else{
   std::string version="";
   if(!(bs >> version)) return false;
   if(version!=GetVersion()) return false;
 }
return Serialise(bs);
}
