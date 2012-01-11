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

#ifndef INCLUDE_GUARD_PFI_CONCURRENT_MUTEX_H_
#define INCLUDE_GUARD_PFI_CONCURRENT_MUTEX_H_

#include "../lang/scoped_ptr.h"

#include "lock.h"

namespace pfi{
namespace concurrent{

class condition_impl;

class mutex_base : public lockable{
  friend class condition_impl;
protected:
  explicit mutex_base(bool recursive);
  ~mutex_base();

public:
  bool lock();
  bool unlock();

private:
  class impl;
  pfi::lang::scoped_ptr<impl> pimpl;
};

// non-recursive mutex

class mutex : public mutex_base{
public:
  mutex(): mutex_base(false) {}
};

// recursive mutex

class r_mutex : public mutex_base{
public:
  r_mutex(): mutex_base(true) {}
};

} // concurrent
} // pfi
#endif // #ifndef INCLUDE_GUARD_PFI_CONCURRENT_MUTEX_H_
