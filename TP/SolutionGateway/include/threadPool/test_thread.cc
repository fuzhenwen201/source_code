/*************************************************************************
	> File Name: test_thread.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 21 Feb 2017 05:43:35 PM CST
 ************************************************************************/
#include"ThreadPool.h"
#include <iostream>
#include <vector>
#include <chrono>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

using namespace std;

ThreadPool* pool= nullptr;
struct B {
	void print(const char* msg, size_t len) {
		std::cout<<"msg:"<<msg<<", len="<<len<<"\n";
	}
};

void prin(const char* msg, size_t len) {
		std::cout<<"msg:"<<msg<<", len="<<len<<"\n";
}
class C {
	public:
		C(int data=0) : m_data(data), b(nullptr) {
			b = new B;
		}

		void Do(const char *tmp, size_t len) {
			char* msg = (char*)malloc(len);
			strcpy(msg, tmp);
			msg[strlen(tmp)] = 0;

			pool->Enqueue([this, msg, len]() {
					b->print(msg, len);
					std::cout<<"+++\n";
					free(msg);
					});
		}

		~C() {
			delete b;
		}
	private:
		int m_data;
		B* b;
		
};

void t(int data) {
	thread_local int tl = 0;
	for (int i = 0; i < data; ++i) {
		cout<<"tid:"<<std::this_thread::get_id()<<", addr:"<<&tl<<", value:"<<++tl<<endl;
		sleep(1);
	}
}

struct uv_buf {
	char* buf;
	int len;
};

uv_buf uv_buf_init(char* buf, int len) {
	uv_buf data;
	data.buf=buf;
	data.len = len;
}


int main(int argc, char* argv[]) {
	pool= new ThreadPool(3);
#if 0
	C c(100);
	c.Do("hello world", 12);

#if 1
	unsigned int n = std::thread::hardware_concurrency();
	    std::cout << n << " concurrent threads are supported.\n";

    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool->Enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

	for (int i=0; i< 10; ++i) {
		pool->Enqueue(prin, "hell", 23);
	}
#endif
	sleep(1);
#endif
	pool->Enqueue(t, 3);
	pool->Enqueue(t, 3);
	pool->Enqueue(t, 5);
	pool->Enqueue(t, 3);
	pool->Enqueue(t, 3);
	pool->Enqueue(t, 3);

	char* str = (char*)malloc(100);
	sprintf(str, "%s", "hello wrold");
	uv_buf buf = uv_buf_init(str, strlen(str)+1);

	pause();

	delete pool;

    return 0;
}
