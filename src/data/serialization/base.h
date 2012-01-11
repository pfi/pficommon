// Copyright (c)2008-2011, Preferred Infrastructure Inc.
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
// 
//     * Neither the name of Preferred Infrastructure nor the names of other
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef INCLUDE_GUARD_PFI_DATA_SERIALIZATION_BASE_H_
#define INCLUDE_GUARD_PFI_DATA_SERIALIZATION_BASE_H_

#include <cstdio>
#include <iostream>

#include "../../lang/safe_bool.h"
#include "../../system/endian_util.h"

namespace pfi{
namespace data{
namespace serialization{

class access{
public:
  template<class Archive, class T>
  static void serialize(Archive &ar, T &v){
    v.serialize(ar);
  }
};

template <class Archive, class T>
void serialize(Archive &ar, T &v)
{
  access::serialize(ar, v);
}

template <class Archive, class T>
Archive &operator&(Archive &ar, T &v)
{
  serialize(ar, v);
  return ar;
}

template <class Archive, class T>
Archive &operator&(Archive &ar, const T &v)
{
  serialize(ar, v);
  return ar;
}

class binary_iarchive : public pfi::lang::safe_bool<binary_iarchive>{
public:
  binary_iarchive(std::istream &is)
    : is(is)
    , buf(is.rdbuf())
    , bad(false){
  }

  static const bool is_read = true;

  template <int N>
  void read(char *p){
    read(p, N);
  }

  void read(char *p, int size){
    for (int i=0;i<size-1;i++)
      *p++=buf->sbumpc();
    int t=buf->sbumpc();
    if (t==EOF) bad=true;
    *p++=t;
  }

  bool bool_test() const{
    return !bad /*&& !!is*/;
  }

private:
  std::istream &is;
  std::streambuf *buf;
  bool bad;
};

template <>
inline void binary_iarchive::read<1>(char *p)
{
  *p++=buf->sgetc();
  if (buf->sbumpc()==EOF) bad=true;
}

template <>
inline void binary_iarchive::read<2>(char *p)
{
  *p++=buf->sbumpc();
  *p++=buf->sgetc();
  if (buf->sbumpc()==EOF) bad=true;
}

template <>
inline void binary_iarchive::read<4>(char *p)
{
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sgetc();
  if (buf->sbumpc()==EOF) bad=true;
}

template <>
inline void binary_iarchive::read<8>(char *p)
{
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sbumpc();
  *p++=buf->sgetc();
  if (buf->sbumpc()==EOF) bad=true;
}

template <class T>
binary_iarchive &operator>>(binary_iarchive &ar, T &v)
{
  ar & v;
  return ar;
}

template <class T>
binary_iarchive &operator>>(binary_iarchive &ar, const T &v)
{
  ar & v;
  return ar;
}

#define gen_serial_binary_iarchive(tt)					\
  template <>								\
  inline void serialize(binary_iarchive &ar, tt &n)			\
  {									\
    tt tmp;								\
    ar.read<sizeof(tmp)>(reinterpret_cast<char*>(&tmp));		\
    if (ar) n=pfi::system::endian::from_little(tmp);			\
  }									\

gen_serial_binary_iarchive(bool);
gen_serial_binary_iarchive(char);
gen_serial_binary_iarchive(signed char);
gen_serial_binary_iarchive(unsigned char);
gen_serial_binary_iarchive(short);
gen_serial_binary_iarchive(unsigned short);
gen_serial_binary_iarchive(int);
gen_serial_binary_iarchive(unsigned int);
gen_serial_binary_iarchive(long);
gen_serial_binary_iarchive(unsigned long);
gen_serial_binary_iarchive(long long);
gen_serial_binary_iarchive(unsigned long long);
gen_serial_binary_iarchive(float);
gen_serial_binary_iarchive(double);
gen_serial_binary_iarchive(long double);

#undef gen_serial_binary_iarchive

class binary_oarchive : public pfi::lang::safe_bool<binary_oarchive>{
public:
  binary_oarchive(std::ostream &os)
    : os(os)
    , it(os){
  }
  virtual ~binary_oarchive() {

  }

  static const bool is_read = false;

  template<int N>
  void write(const char *p){
    write(p, N);
  }

  void write(const char *p, int size){
    for (int i=0;i<size;i++)
      *it=*p++;
  }

  void flush(){
    os.flush();
  }

  bool bool_test() const{
    return !!os;
  }

private:
  std::ostream &os;
  std::ostreambuf_iterator<char> it;
};

template <>
inline void binary_oarchive::write<1>(const char *p)
{
  *it=*p++;
}

template <>
inline void binary_oarchive::write<2>(const char *p)
{
  *it=*p++;
  *it=*p++;
}

template <>
inline void binary_oarchive::write<4>(const char *p)
{
  *it=*p++;
  *it=*p++;
  *it=*p++;
  *it=*p++;
}

template <>
inline void binary_oarchive::write<8>(const char *p)
{
  *it=*p++;
  *it=*p++;
  *it=*p++;
  *it=*p++;
  *it=*p++;
  *it=*p++;
  *it=*p++;
  *it=*p++;
}


template <class T>
binary_oarchive &operator<<(binary_oarchive &ar, T &v)
{
  ar & v;
  return ar;
}

template <class T>
binary_oarchive &operator<<(binary_oarchive &ar, const T &v)
{
  ar & v;
  return ar;
}

#define gen_serial_binary_oarchive(tt)				\
  template <>							\
    inline void serialize(binary_oarchive &ar, tt &n)		\
  {								\
    tt tmp=pfi::system::endian::to_little(n);		\
    ar.write<sizeof(tmp)>(reinterpret_cast<const char*>(&tmp)); \
  }								\

gen_serial_binary_oarchive(bool);
gen_serial_binary_oarchive(const bool);
gen_serial_binary_oarchive(char);
gen_serial_binary_oarchive(const char);
gen_serial_binary_oarchive(signed char);
gen_serial_binary_oarchive(const signed char);
gen_serial_binary_oarchive(unsigned char);
gen_serial_binary_oarchive(const unsigned char);
gen_serial_binary_oarchive(short);
gen_serial_binary_oarchive(const short);
gen_serial_binary_oarchive(unsigned short);
gen_serial_binary_oarchive(const unsigned short);
gen_serial_binary_oarchive(int);
gen_serial_binary_oarchive(const int);
gen_serial_binary_oarchive(unsigned int);
gen_serial_binary_oarchive(const unsigned int);
gen_serial_binary_oarchive(long);
gen_serial_binary_oarchive(const long);
gen_serial_binary_oarchive(unsigned long);
gen_serial_binary_oarchive(const unsigned long);
gen_serial_binary_oarchive(long long);
gen_serial_binary_oarchive(const long long);
gen_serial_binary_oarchive(unsigned long long);
gen_serial_binary_oarchive(const unsigned long long);
gen_serial_binary_oarchive(float);
gen_serial_binary_oarchive(const float);
gen_serial_binary_oarchive(double);
gen_serial_binary_oarchive(const double);
gen_serial_binary_oarchive(long double);
gen_serial_binary_oarchive(const long double);

#undef gen_serial_binary_oarchive

} // serialization
} // data
} // pfi
#endif // #ifndef INCLUDE_GUARD_PFI_DATA_SERIALIZATION_BASE_H_
