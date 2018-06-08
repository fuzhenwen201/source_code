/*************************************************************************
	> File Name: test.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 07 Mar 2017 06:32:15 PM CST
 ************************************************************************/

#include<iostream>
#include"object_pool.h"

using namespace std;

struct A {
	A(int i=10) {
		std::cout<<i<<std::endl;
	}
};

int main(int argc, char* argv[]) {
	using ObjectPoolInt = ObjectPool<int, 100>;
	//ObjectPool<int, 100>* ob = ObjectPool<int, 100>::GetInstance();
	ObjectPoolInt* ob = ObjectPool<int, 100>::GetInstance();
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

	return 0;
}
