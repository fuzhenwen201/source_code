/*************************************************************************
	> File Name: test.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 07 Mar 2017 06:32:15 PM CST
 ************************************************************************/

#include<iostream>
#include"object_pool.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<thread>
#include"xdatetime.h"

#define MAX_SIZE 10000
#define CNT_SIZE 1000

using namespace std;

struct A {
	A(int i=10) {
		std::cout<<i<<std::endl;
	}
};

template<typename T, size_t size>
struct MemorySize {
	T data[size];
};


const char*  str = "{\"reqid\":\"7be70b5c51aa11e7bf8e741bc2859a36\",\"sessionid\":\"7be70b5c51aa11e7bf8e741bc2859a36\",\"groupid\":0,\"clientid\":72,\"accountid\":0,\"boaccountid\":0,\"assetid\":0,\"direction\":0,\"expiryperiod\":5,\"amount\":100,\"sgid\":10,\"orderstatus\":136,\"margin\":0,\"freemargin\":918900,\"profit\":-81100,\"status\":1,\"prevbalance\":0,\"changed\":1000000,\"closebalance\":0}";

void asthread(void) {
	CCalcTime calc;
	calc.Begin();
	for (size_t i=0; i<CNT_SIZE; ++i) {
		for (size_t j=0; j < MAX_SIZE; ++j) {
			char* astr = nullptr;
			asprintf(&astr, "%s", str);
			i+j;
			i-j;
			free(astr);
			astr = nullptr;
		}
	}
	long ret = calc.End();
	printf("asprintf: %ld\n",ret);
}

void mathread(void) {
	CCalcTime calc;
	calc.Begin();

	for (size_t i=0; i<CNT_SIZE; ++i) {
		for (size_t j=0; j < MAX_SIZE; ++j) {
			char* mstr = (char*)malloc(500);
			memcpy(mstr, str, 340);
			mstr[340]=0;
			i+j;
			i-j;
			free(mstr);
			mstr = nullptr;
		}
	}
	long ret = calc.End();
	printf("malloc: %ld\n",ret);
}

void obthread(void) {
	CCalcTime calc;
	calc.Begin();

	for (size_t i=0; i<CNT_SIZE; ++i) {
		for (size_t j=0; j < MAX_SIZE; ++j) {
			MemorySize<char, 500>* mob1 = ObjectPool<MemorySize<char, 500>, 100>::GetInstance()->GetObject();
			memcpy(mob1->data, str, 340);
			i+j;
			i-j;
			ObjectPool<MemorySize<char, 500>, 100>::GetInstance()->ReleaseObject(mob1);
		}
	}
	long ret = calc.End();
	printf("object: %ld\n",ret);
}

int main(int argc, char* argv[]) {
	ObjectPool<int, 100>* ob = ObjectPool<int, 100>::GetInstance();
	int* ob1 = ObjectPool<int, 100>::GetInstance()->GetObject();
	int* ob2 = ObjectPool<int, 100>::GetInstance()->GetObject();
	int* ob3 = ObjectPool<int, 100>::GetInstance()->GetObject();
	int* ob4 = ObjectPool<int, 100>::GetInstance()->GetObject();
	
	*ob1 = 10;
	*ob2 = 20;
	*ob3 = 30;
	*ob4 = 40;
	ObjectPool<int, 100>::GetInstance()->ReleaseObject(ob1);
	ObjectPool<int, 100>::GetInstance()->ReleaseObject(ob2);
	ObjectPool<int, 100>::GetInstance()->ReleaseObject(ob3);
	ObjectPool<int, 100>::GetInstance()->ReleaseObject(ob4);

	ObjectPool<MemorySize<char, 500>, 100>* mob = ObjectPool<MemorySize<char, 500>, 100>::GetInstance(); 
	MemorySize<char, 500>* mob2 = ObjectPool<MemorySize<char, 500>, 100>::GetInstance()->GetObject();
	MemorySize<char, 500>* mob3 = ObjectPool<MemorySize<char, 500>, 100>::GetInstance()->GetObject();

	std::thread t1(asthread);
	std::thread t2(mathread);
	std::thread t3(obthread);
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}

