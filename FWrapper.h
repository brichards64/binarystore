#ifndef FWRAPPER_H
#define FWRAPPER_H

#include <string.h>
#include <iostream>
#include <unistd.h> //for lseek
#include <vector>
#include <map>
#include <deque>
//#include <SerialisableObject.h>
#include <stdio.h>
#include <sys/stat.h>

enum enum_endpoint { RAM , UNCOMPRESSED , POST_PRE_COMPRESS, COMPRESSED };
enum enum_mode { READ , NEW , APPEND, UPDATE, READ_APPEND, NEW_READ };

class FWrapper{

 public:

  FWrapper();
  ~FWrapper();
  bool FOpen(std::string filename, enum_mode method=UPDATE, enum_endpoint endpoint=POST_PRE_COMPRESS);
  bool FClose();
  bool FRead(void* out, unsigned int size);
  bool FWrite(const void* in, unsigned int size);
  // bool FSeek(unsigned int pos, int whence);
  unsigned int FTell();

  FILE* pfile;
  
 private:
  
  enum_endpoint m_endpoint;
  std::string m_file_name;
  std::string buffer;
  unsigned long m_pos;
  
};

#endif
