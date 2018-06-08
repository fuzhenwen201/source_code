// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

namespace muduo
{
//该类主要用于做互斥和条件等待的计数器功能，一等进入条件等待之后，将需要等待N次count_donw
//当计数器变为0的时候，才会发出信号通知条件进行激活状态
class CountDownLatch : noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};

}
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
