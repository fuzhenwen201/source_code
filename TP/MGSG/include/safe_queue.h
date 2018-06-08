/*************************************************************************
	> File Name: safe_queue.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Sun 09 Jul 2017 12:43:05 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_SAFT_QUQUE_H_
#define SOLUTIONGATEWAY_SAFT_QUEUE_H_
#include<mutex>
#include<condition_variable>
#include<unistd.h>
#include<thread>

template<typename T>
class SafeQueue {
	public:
		struct node {
			node* next;
			T data;

			node();
			node(const T& data);
			node(T&& data);
			~node();
			/* copy constructor */
			node(const node& r);
			node(node&& r);
			/* assigment */
			node& operator=(const node& r);
			node& operator=(node&& r);
		};

	public:
		SafeQueue();
		~SafeQueue();

		SafeQueue(const SafeQueue& r) = delete;
		SafeQueue& operator=(const SafeQueue& r) = delete;
		SafeQueue(SafeQueue&& r) = delete;
		SafeQueue& operator=(SafeQueue&& r) = delete;

	public:
		bool Empty(void) const;
		void Push(const T& data);
		void Push(T&& data);
		bool Pop(T& data);
		void WaitPop(T& data);

	private:
		node* GetTail() {
			std::lock_guard<std::mutex> tailLock(mxTail);

			return tail;
		}

	private:
		node* head;
		node* tail;
		mutable	std::mutex mxHead;
		std::mutex mxTail;
		std::condition_variable cond;
};

/* node begin ****/
template<typename T>
SafeQueue<T>::node::node() {
	next = nullptr;
}
template<typename T>
SafeQueue<T>::node::node(const T& data) {
	this->data = data;
	next = nullptr;
}

template<typename T>
SafeQueue<T>::node::node(T&& data) {
	this->data = std::move(data);
	next = nullptr;
}

template<typename T>
SafeQueue<T>::node::node(const node& r) {
	this->data = r.data;
	next = nullptr;
}

template<typename T>
SafeQueue<T>::node::node(node&& r) {
	this->data = std::move(r.data);
	next = nullptr;
}

template<typename T>
typename SafeQueue<T>::node& SafeQueue<T>::node::operator=(const node& r) {
	if (this == &r) {
		return *this;
	}

	this->data = r.data;
	next = nullptr;

	return *this;
}

template<typename T>
typename SafeQueue<T>::node& SafeQueue<T>::node::operator=(node&& r) {
	if (this == &r) {
		return *this;
	}

	this->data = std::move(r.data);
	next = nullptr;

	return *this;
}

template<typename T>
SafeQueue<T>::node::~node() {
	next = nullptr;
}
/* node end ****/


/* Queue begin ***/
template<typename T>
SafeQueue<T>::SafeQueue() 
	: head(new node)
	, tail(head) {
	/* empty/default constor: head == node */
}

template<typename T>
SafeQueue<T>::~SafeQueue() {
	node* next = nullptr;
	node* curr = head;
	node* dst = nullptr;
	std::lock_guard<std::mutex> lk(mxHead);
	while (next = curr->next) {
		dst = next;
		curr->next = curr->next->next;
		delete dst;
		dst = nullptr;
	}
	delete head;
}

template<typename T>
void SafeQueue<T>::Push(const T& data) {
	node* newTail = new node;

	{
		std::lock_guard<std::mutex> tailLock(mxTail);
		tail->data = data;
		tail->next = newTail;
		tail = newTail;
	}

	cond.notify_one();
}

template<typename T>
void SafeQueue<T>::Push(T&& data) {
	node* newTail = new node;

	{
		std::lock_guard<std::mutex> tailLock(mxTail);
		tail->data = std::move(data);
		tail->next = newTail;
		tail = newTail;
	}

	cond.notify_one();
}

template<typename T>
bool SafeQueue<T>::Pop(T& data) {
	std::lock_guard<std::mutex> lk(mxHead);
	if (head == GetTail()) {
		return false;
	}

	data = std::move(head->data);
	node* oldHead = head;
	head = head->next;
	delete oldHead;

	return true;
}

template<typename T>
void SafeQueue<T>::WaitPop(T& data) {
	std::unique_lock<std::mutex> headLock(mxHead);

	cond.wait(headLock, [&] {
		return head != GetTail();
	});

	data = std::move(head->data);
	node* oldHead = head;
	head = head->next;
	delete oldHead;
}

template<typename T>
bool SafeQueue<T>::Empty() const {
	std::lock_guard<std::mutex> headLock(mxHead);

	return (head == GetTail());
}
/* Queue end ***/
#endif

