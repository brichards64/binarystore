#ifndef BINARY_STREAM_2
#define BINARY_STREAM_2

#include <iostream>
#include <functional>
#include <FWrapper.h>
//#include <zlib.h>
#include <SerialisableObject.h>
#include <type_traits>


class BinaryStream2 : public SerialisableObject{


 public:

  BinaryStream2(std::string filename, enum_mode method=UPDATE, enum_endpoint endpoint=POST_PRE_COMPRESS);
  ~BinaryStream2();


  std::function<bool(std::string, enum_mode, enum_endpoint)> Bopen;
  std::function<bool()> Bclose;
  std::function<bool(const void*, unsigned int)> Bwrite;
  std::function<bool(void*, unsigned int)> Bread;
  std::function<bool(unsigned int, int)> Bseek;
  std::function<unsigned int()> Btell;

  bool Print(){};
  bool Serialise(BinaryStream2 &bs){};
  std::string GetVersion(){};


  bool operator<<(std::string& rhs){
    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.length();
      ret*=(*this) << tmp;
      if(tmp) ret*=Bwrite(&(rhs[0]), tmp);
      return ret;
    }
    else return false;
  }

  bool operator>>(std::string& rhs){
    if(m_mode!=NEW && m_mode!=APPEND){
      bool ret=true;
      unsigned int tmp=0;
      ret*=(*this) >> tmp;
      rhs.resize(tmp);
      if(tmp) ret*=Bread(&(rhs[0]), tmp);
      return ret;
    }
    else return false;
  }

  bool operator&(std::string& rhs){
    if(m_write) return (*this) << rhs;
    else return (*this) >> rhs;
  }

  bool operator<<(const std::string& rhs){
    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.length();
      ret*=(*this) << tmp;
      if(tmp) ret*=Bwrite(&(rhs[0]), tmp);
      return ret;
    }
    else return false;
  }

  bool operator&(const std::string& rhs){
    if(m_write) return (*this) << rhs;
    return false;
  }


  template<typename T> bool operator<<(T& rhs){
    if(m_mode!=READ){
      if(std::is_base_of<SerialisableObject,T>::value){
        SerialisableObject* tmp=reinterpret_cast<SerialisableObject*>(&rhs);
        m_write=true;
        return tmp->SerialiseWrapper(*this);
      }
      else return Bwrite(&rhs, sizeof(T));
    }
    else return false;
  }


  template<typename T> bool operator>>(T& rhs){
    if(m_mode!=NEW && m_mode!=APPEND){
      if(std::is_base_of<SerialisableObject,T>::value){
        SerialisableObject* tmp=reinterpret_cast<SerialisableObject*>(&rhs);
        m_write=false;
        return tmp->SerialiseWrapper(*this);
      }
      else return Bread(&rhs, sizeof(T));
    }
    return false;
  }


  template<typename T> bool operator&(T& rhs){
    if(m_write)  return (*this) << rhs;
    else return (*this) >> rhs;
  }

  template<typename T> bool operator<<(const T& rhs){
    if(m_mode!=READ){
      if(std::is_base_of<SerialisableObject,T>::value){
        SerialisableObject* tmp=reinterpret_cast<SerialisableObject*>(&rhs);
        m_write=true;
        return tmp->SerialiseWrapper(*this);
      }
      return Bwrite(&rhs, sizeof(T));
    }
    else return false;
  }

  template<typename T> bool operator&(const T& rhs){
    if(m_write) return (*this) << rhs;
    return false;
  }


  template<typename T> bool operator<<(std::vector<T>& rhs){

    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.size();
      ret*=(*this) << tmp;
      if(tmp){
        if(std::is_base_of<SerialisableObject,T>::value){
          for(typename std::vector<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) ret*=(*this) << (*it);
        }
        else ret*=Bwrite(&(rhs[0]), tmp*sizeof(T));
      }
      return ret;
    }
    else return false;
  }

  template<typename T> bool operator>>(std::vector<T>& rhs){

    if(m_mode!=NEW && m_mode!=APPEND){
      bool ret=true;
      unsigned int tmp=0;
      ret*=(*this) >> tmp;
      rhs.resize(tmp);
      if(tmp){
        if(std::is_base_of<SerialisableObject,T>::value){
          for(typename std::vector<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) ret*=(*this) >> (*it);
        }
        else ret*=Bread(&(rhs[0]), tmp*sizeof(T));
      }
      return ret;
    }
    else return false;
  }

  template<typename T> bool operator&(std::vector<T>& rhs){

    if(m_write) return (*this) << rhs;
    else return (*this) >> rhs;
  }

  bool operator<<(std::vector<std::string>& rhs){
    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.size();
      ret*=(*this) << tmp;
      for(unsigned int i=0; i<tmp; i++){
        ret*=(*this) << rhs.at(i);
      }
      return ret;
    }
    else return false;
  }

  bool operator>>(std::vector<std::string>& rhs){
    if(m_mode!=NEW && m_mode!=APPEND){
      bool ret=true;
      unsigned int tmp=0;
      ret*=(*this) >> tmp;
      rhs.resize(tmp);
      for(unsigned int i=0; i<tmp; i++){
        ret*=(*this) >> rhs.at(i);
      }
      return ret;
    }
    else return false;
  }

  bool operator&(std::vector<std::string>& rhs){
    if(m_write) return (*this) << rhs;
    else return (*this) >> rhs;
  }

  template<typename T, typename U> bool operator<<(std::map<T,U>& rhs){
    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.size();
      ret*=(*this) << tmp;
      for (typename std::map<T,U>::iterator it=rhs.begin(); it!=rhs.end(); ++it){
        T key=it->first;
        U value=it->second;
        ret*=(*this) << key;
        ret*=(*this) << value;
      }
      return ret;
    }
    else return false;
  }

  template<typename T, typename U> bool operator>>(std::map<T,U>& rhs){
    if(m_mode!=NEW && m_mode!=APPEND){
      bool ret=true;
      unsigned int tmp=0;
      ret*=(*this) >> tmp;
      for (unsigned int i=0; i<tmp; i++){
        T key;
        U value;
        ret*=(*this) >> key;
        ret*=(*this) >> value;
        rhs[key]=value;
      }
      return ret;
    }
    else return false;
  }

  template<typename T, typename U> bool operator&(std::map<T,U>& rhs){
    if(m_write) return (*this) << rhs;
    else return (*this) >> rhs;
  }

  template<typename T> bool operator<<(std::deque<T>& rhs){
    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.size();
      ret*=(*this) << tmp;
      if(tmp){
	if(std::is_base_of<SerialisableObject,T>::value){
	  for(typename std::deque<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) ret*=(*this) << (*it);
	}
	else ret*=Bwrite(&(rhs[0]), tmp*sizeof(T));
      }
      return ret;
    }
    else return false;
  }
  
  template<typename T> bool operator>>(std::deque<T>& rhs){
    if(m_mode!=NEW && m_mode!=APPEND){
      bool ret=true;
      unsigned int tmp=0;
      ret*=(*this) >> tmp;
      rhs.resize(tmp);
      if(tmp){
	if(std::is_base_of<SerialisableObject,T>::value){
	  for(typename std::deque<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) ret*=(*this) >> (*it);
	}
	else ret*=Bread(&(rhs[0]), tmp*sizeof(T));
      }
      return ret;
    }
    else return false;
  }
  
  template<typename T> bool operator&(std::deque<T>& rhs){
    if(m_write) return(*this) << rhs;
    else return(*this) >> rhs;
  }
  
  bool operator<<(std::deque<std::string>& rhs){
    if(m_mode!=READ){
      bool ret=true;
      unsigned int tmp=rhs.size();
      ret*=(*this) << tmp;
      for(unsigned int i=0; i<tmp; i++){
	ret*=(*this) << rhs.at(i);
      }
      return ret;
    }
    else return false;
  }
  
  bool operator>>(std::deque<std::string>& rhs){
    if(m_mode!=NEW && m_mode!=APPEND){
      bool ret=true;
      unsigned int tmp=0;
      ret*=(*this) >> tmp;
      rhs.resize(tmp);
      for(unsigned int i=0; i<tmp; i++){
	ret*=(*this) >> rhs.at(i);
      }
      return ret;
    }
    else return false;
  }
  
  bool operator&(std::deque<std::string>& rhs){
    if(m_write) return (*this) << rhs;
    else return (*this) >> rhs;
  }
  
  bool m_write;
  enum_mode m_mode;

 private:



  FWrapper* fwrapper;
  
  //  int def(FILE *source, FILE *dest, int level);
  //int inf(FILE *source, FILE *dest);
  //void zerr(int ret);

  template <typename B, typename D> struct Host{
    
    operator B*() const;
    operator D*();
    
  };
  
  
  template <typename B, typename D> struct check_base {
    template <typename T>
    static short check(D*, T);
    static char check(B*, int);
    
    static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(short);
  };
  


};

#endif
