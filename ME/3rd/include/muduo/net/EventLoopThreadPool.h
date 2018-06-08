// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include <muduo/base/noncopyable.h>
#include <muduo/base/Types.h>

#include <functional>
#include <memory>
#include <vector>

namespace muduo
{

namespace net
{

class EventLoop; 
class EventLoopThread;

class EventLoopThreadPool : noncopyable
{
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start(const ThreadInitCallback& cb = ThreadInitCallback());

  // valid after calling start()
  /// round-robin
  EventLoop* getNextLoop();

  /// with the same hash code, it will always return the same EventLoop
  EventLoop* getLoopForHash(size_t hashCode);

  std::vector<EventLoop*> getAllLoops();

  bool started() const
  { return started_; }

  const string& name() const
  { return name_; }

 private:

  EventLoop* baseLoop_;
  string name_;
  bool started_;
  int numThreads_;
  int next_;
  //用来存储事件线程的容器
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  //每一个线程所对应的loop都是在线程绑定的自定义线程函数中构造的栈对象
  std::vector<EventLoop*> loops_;
};

}
}

#endif  // MUDUO_NET_EVENTLOOPTHREADPOOL_H
