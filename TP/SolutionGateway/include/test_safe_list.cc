/*************************************************************************
	> File Name: test_safe_list.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 15 Feb 2017 03:05:18 PM CST
 ************************************************************************/

#include"safe_list.h"
#include<iostream>
using namespace std;
//using namespace snetwork_xservice_solutiongateway;
//
struct A{
	A(int i=0) : data(i) {}
	int data;

	friend bool operator==(const A& l, const A& r);
	friend bool operator==(A& l, A& r);
};

bool operator==(const A& l, const A& r) {
	cout<<"------l.data:"<<l.data<<", r.data:"<<r.data<<endl;
	if (l.data==r.data) {
		return true;
	}

	return false;
}

bool operator==(A& l, A& r) {
	cout<<"======l.data:"<<l.data<<", r.data:"<<r.data<<endl;
	if (l.data==r.data) {
		return true;
	}

	return false;
}

int main(int argc, char* argv[]) {
#if 0
	SafeList<int> l;
	for (size_t i=0; i<10; ++i) {
		l.PushFront(i);
	}

	l.PushFront(7);
	l.PushFront(9);
	l.PushFront(7);
	cout<<"Length="<<l.Length()<<endl;
	l.ForEach([](int a){cout<<a<<"	";});
	auto data = l.Find(8);
	cout<<endl<<endl;
	l.Remove([](int a){return a==6;});
	l.ForEach([](int a){cout<<a<<"	";});
	cout<<endl<<endl;
	int da = 7;
	int flag = l.Update(7, true, [&](int& a){ 
			a*=10;
			});
	l.ForEach([]jjkkkkkkkkkkkkkkk(int a){cout<<a<<"	";});
	cout<<endl<<flag<<endl;
	flag = l.Update(9, false, [&](int& a){
		a *=10;
			});
	l.ForEach([](int a){cout<<a<<"	";});
	cout<<endl<<flag<<endl;
#endif

	SafeList<A> ll;
	for (size_t i=0; i<10; ++i) {
		ll.PushFront(A(i));
	}
	
	A data(7);
	int flag = ll.Update(data, false, [&](A& a){ 
			a.data*=10;
			});

	return 0;
}
