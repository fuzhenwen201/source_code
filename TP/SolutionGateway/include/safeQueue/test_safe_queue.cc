/*************************************************************************
	> File Name: test_safe_list.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 15 Feb 2017 03:05:18 PM CST
 ************************************************************************/

#include"../safe_queue.h"
#include<iostream>
#include<thread>
//#include <atomic>

using namespace std;
//using namespace snetwork_xservice_solutiongateway;
SafeQueue<int> q;

void Push(int data) {
	for (int i=11; i<21; ++i) {
		q.Push(i);
	}
}

void Pop() {
	int i;
	while (1) {
		q.WaitPop(i);
	}
}

bool Pop1(int data) {
	int i;
	static int cnt = 0;

	while (1) {
	if (q.Pop(i)) {
		std::cout<<i<<"|"<<++cnt<<"+++data"<<endl;

		//return true;
	} else {
		std::cout<<"Empty\n";

		return false;
	}
	}
}

int main(int argc, char* argv[]) {
	for (size_t i=1; i<11; ++i) {
		q.Push(i);
	}

	std::thread t1(Push, 1);
	//std::thread t2(Pop);
	std::thread t3(Pop1, 2);

	t1.join();
	//t2.join();
	t3.join();

	return 0;
}
