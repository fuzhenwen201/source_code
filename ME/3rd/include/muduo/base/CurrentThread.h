// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include <stdint.h>

namespace muduo
{
namespace CurrentThread
{
  // internal
  extern __thread int t_cachedTid;
  extern __thread char t_tidString[32];
  extern __thread int t_tidStringLength;
  extern __thread const char* t_threadName;
  void cacheTid();
  //获得该线程的真实的线程id唯一标识，是因为线程ID在不同的进程中是可以重复的，但是
  //通过调用syscall(SYS_gettid)获得线程ID唯一标识是不会重复的
  inline int tid()
  {
	//在gcc里，该宏用来告诉编译器__builtin_expect条件表达式t_cachedTid == 0在大多数情况下为假，也就是大多数执行else中的内容
	//该宏只是用来优化编译器，节省事件，并不改变if中条件表达式的值
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();

  void sleepUsec(int64_t usec);
}
}

#endif
