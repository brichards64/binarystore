#include <FWrapper.h>


FWrapper::FWrapper(){

  buffer="";
  m_pos=0;

}

FWrapper::~FWrapper(){

  FClose();

}

bool FWrapper::FOpen(std::string filename, enum_mode method, enum_endpoint endpoint){

  enum_endpoint m_endpoint=endpoint;
  std::string m_file_name=filename;

  if(m_endpoint==UNCOMPRESSED || m_endpoint==POST_PRE_COMPRESS){
    if(method==READ) pfile = fopen(m_file_name.c_str(), "rb");
    else if(method==NEW)  pfile = fopen(m_file_name.c_str(), "wb");
    else if(method==APPEND)  pfile = fopen(m_file_name.c_str(), "ab");
    else if(method==UPDATE){
      struct stat buffer;
      if(stat (filename.c_str(), &buffer) == 0) pfile = fopen(m_file_name.c_str(), "rb+"); //if file exists
      else pfile = fopen(m_file_name.c_str(), "wb+"); //if it doesnt
    }
      else if(method==READ_APPEND) pfile = fopen(m_file_name.c_str(), "ab+");
      else if(method==NEW_READ) pfile = fopen(m_file_name.c_str(), "wb+");
      else return false;
      if(pfile==NULL) return false;
      else return true;
  }
  return false;

}

bool FWrapper::FClose(){

  std::cout<<buffer.length()<<std::endl;
  fwrite(&buffer[0] , 1, buffer.length(), pfile);

  if(pfile!=0 && !fclose(pfile)){
    pfile=0;
    return true;
  }

  return false;

}


bool FWrapper::FWrite(const void* in, unsigned int size){
  buffer.resize(buffer.length()+size);
  memcpy(&(buffer[m_pos]), in, size);
  m_pos+=size;
  //  std::cout<<m_pos<<":"<<buffer.length()<<std::endl;
  return true;
  //  return fwrite(in , size, 1, pfile);

}

bool FWrapper::FRead(void* out, unsigned int size){

  return fread(out , size, 1, pfile);

}

/*
bool FWrapper::FSeek(unsigned int pos, int whence){
  
  if(whence==SEEK_END) return (-1!=lseek(fileno(pfile), pos, whence));
  else return !fseek(pfile, pos, whence);
}
*/

unsigned int FWrapper::FTell(){
  
 return ftell(pfile);
  
}
