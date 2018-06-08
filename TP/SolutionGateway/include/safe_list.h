/*************************************************************************
	> File Name: safe_list.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 08 Feb 2017 07:14:21 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_SAFT_LIST_H_
#define SOLUTIONGATEWAY_SAFT_LIST_H_
#include<mutex>
#include<chrono>
#include<unistd.h>
#include<thread>
#include<atomic> 

#define SAFE_LIST_ELAPSED_SECONDS	60
//namespace snetwork_xservice_solutiongateway {
template<typename T>
class SafeList {
	public:
		struct node {
			node* next;
			T data;
			std::mutex m_mutex;

			/* constructor */
			node(); /* : data(nullptr), next(nullptr) {} */
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
		SafeList();
		~SafeList();
		SafeList(const SafeList& r) = delete;
		SafeList& operator=(const SafeList&&) = delete;
		SafeList(SafeList&& r) = delete;
		SafeList& operator=(SafeList&& r) = delete;

	public:
		size_t Length(void) const {
			//std::lock_guard<std::mutex> lk(m_head->m_mutex);

			return m_len;
		}

		bool Empty(void) const {
			//std::lock_guard<std::mutex> lk(m_head->m_mutex);

			return m_len==0;
		}

	public:
		void PushFront(const T& data);
		void PushFront(T&& data);
		
		template<typename Function>
		void ForEach(Function f);

		//node* Find(const T& data);
		T* Find(const T& data);
		int Update(const T& data, bool all = false);
		int Update(T&& data, bool all = false);

#if 0
		template<typename Predicate>
		int Update(const T* data, bool all, Predicate p);
#endif
		template<typename Predicate>
		int Update(const T& data, bool all, Predicate p);
		template<typename Predicate>
		int Update(T&& data, bool all, Predicate p);

		template<typename Predicate>
		int Remove(Predicate p);

		template<typename Predicate>
		int RemoveAll(Predicate p);

		template<typename Predicate>
		int UpdateAll(Predicate p);

	public:
		template<typename Predicate>
		void Run(Predicate p);
	private:
		node* m_head;
		//size_t m_len;
		std::atomic<int> m_len;
};

/* node begin ****/
template<typename T>
SafeList<T>::node::node() {
	//this->data = new T(data);
	//this->data = data;
	next = nullptr;
}
template<typename T>
SafeList<T>::node::node(const T& data) {
	//this->data = new T(data);
	this->data = data;

	next = nullptr;
}

template<typename T>
SafeList<T>::node::node(T&& data) {
	//this->data = new T(std::move(data));
	this->data = std::move(data);

	next = nullptr;
}

template<typename T>
SafeList<T>::node::node(const node& r) {
	//this->data = new T(r.data);
	this->data = r.data;

	next = nullptr;
}

template<typename T>
SafeList<T>::node::node(node&& r) {
	//this->data = new T(std::move(r.data));
	this->data = std::move(r.data);

	next = nullptr;
}

template<typename T>
typename SafeList<T>::node& SafeList<T>::node::operator=(const node& r) {
	if (this == &r) {
		return *this;
	}

	//this->data = new T(r.data);
	this->data = r.data;
	next = nullptr;

	return *this;
}

template<typename T>
typename SafeList<T>::node& SafeList<T>::node::operator=(node&& r) {
	if (this == &r) {
		return *this;
	}

	//this->data = new T(std::move(r.data));
	this->data = std::move(r.data);
	next = nullptr;

	return *this;
}

template<typename T>
SafeList<T>::node::~node() {
#if 0
	if (data != nullptr) {
		delete data;
		data = nullptr;
	}
#endif
	next = nullptr;
}
/* node end ****/

/*  list begin *****/
template<typename T>
SafeList<T>::SafeList() {
	m_head = new node;
	m_len = 0;
}

template<typename T>
SafeList<T>::~SafeList() {
	node* curr = m_head;
	node* dst = nullptr;
	node* next = nullptr;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		dst = next;
		curr->next = curr->next->next;
		lk=std::move(next_lk);
		delete dst;
		dst = nullptr;
	}
	m_len = 0;
}

template<typename T>
void SafeList<T>::PushFront(const T& data) {
	node* newNode = new node(data);
	std::lock_guard<std::mutex> lk(m_head->m_mutex);
	newNode->next = m_head->next;
	m_head->next = newNode;
	++m_len;
}

template<typename T>
void SafeList<T>::PushFront(T&& data) {
	node* newNode = new node(std::move(data));
	std::lock_guard<std::mutex> lk(m_head->m_mutex);
	newNode->next = m_head->next;
	m_head->next = newNode;
	++m_len;
}

template<typename T>
template<typename Function>
void SafeList<T>::ForEach(Function f) {
	node* curr = m_head;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		lk.unlock();
		f(next->data);
		curr = next;
		lk=std::move(next_lk);
	}
}

template<typename T>
//typename SafeList<T>::node* SafeList<T>::Find(const T& data) {
T* SafeList<T>::Find(const T& data) {
	node* curr = m_head;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		lk.unlock();
		if(data == next->data) {
			//return next;
			return &next->data;
		} else {
			curr = next;
			lk=std::move(next_lk);
		}
	}

	return nullptr;
}

template<typename T>
int SafeList<T>::Update(const T& data, bool all) {
	node* curr = m_head;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = 0;
	if (all == false) {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				next->data = data;
				++ret;

				return ret;
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	} else {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				next->data = data;
				++ret;
				curr = next;
				lk=std::move(next_lk);
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	}

	return ret;
}

template<typename T>
int SafeList<T>::Update(T&& data, bool all) {
	node* curr = m_head;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = 0;
	if (all == false) {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				next->data = std::move(data);
				++ret;

				return ret;
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	} else {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				next->data = std::move(data);
				++ret;
				curr = next;
				lk=std::move(next_lk);
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	}

	return ret;
}

template<typename T>
template<typename Predicate>
int SafeList<T>::Update(const T& data, bool all, Predicate p) {
	node* curr = m_head;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = 0;
	if (all == false) {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				//next->data = data;
				p(next->data);
				++ret;

				return ret;
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	} else {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				//next->data = data;
				p(next->data);
				++ret;
				curr = next;
				lk=std::move(next_lk);
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	}

	return ret;
}

template<typename T>
template<typename Predicate>
int SafeList<T>::Update(T&& data, bool all, Predicate p) {
	node* curr = m_head;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = 0;
	if (all == false) {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				//next->data = std::move(data);
				p(next->data);
				++ret;

				return ret;
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	} else {
		while (next = curr->next) {
			std::unique_lock<std::mutex> next_lk(next->m_mutex);
			lk.unlock();
			if(data == next->data) {
				//next->data = std::move(data);
				p(next->data);
				++ret;
				curr = next;
				lk=std::move(next_lk);

				//return true;
			} else {
				curr = next;
				lk=std::move(next_lk);
			}
		}
	}

	return ret;
}

#if 0
template<typename T>
template<typename Predicate>
void SafeList<T>::Remove(Predicate p) {
	node* curr = m_head;
	node* dst = nullptr;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		if(p(next->data)) {
			dst = next;
			curr->next = curr->next->next;
			--m_len;

			break;
		} else {
			curr = next;
			lk=std::move(next_lk);
		}
	}

	delete dst;
	dst = nullptr;
}
#endif
template<typename T>
template<typename Predicate>
int SafeList<T>::RemoveAll(Predicate p) {
	node* curr = m_head;
	node* dst = nullptr;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = m_len;
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		if(p(next->data)) {
			dst = next;
			curr->next = curr->next->next;
			--m_len;
			delete dst;
			dst = nullptr;

			//break;
		} else {
			curr = next;
			lk=std::move(next_lk);
		}
	}

	return ret;
}

template<typename T>
template<typename Predicate>
int SafeList<T>::Remove(Predicate p) {
	node* curr = m_head;
	node* dst = nullptr;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = 0;
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		if(p(next->data)) {
			dst = next;
			curr->next = curr->next->next;
			--m_len;
			++ret;
			delete dst;
			dst = nullptr;

			break;
		} else {
			curr = next;
			lk=std::move(next_lk);
		}
	}

	return ret;
}

template<typename T>
template<typename Predicate>
int SafeList<T>::UpdateAll(Predicate p) {
	node* curr = m_head;
	node* dst = nullptr;
	std::unique_lock<std::mutex> lk(m_head->m_mutex);
	node* next = nullptr;
	int ret = 0;
	while (next = curr->next) {
		std::unique_lock<std::mutex> next_lk(next->m_mutex);
		p(next->data);
		++ret;
		curr = next;
		lk=std::move(next_lk);
	}

	return ret;
}

template<typename T>
template<typename Predicate>
void SafeList<T>::Run(Predicate p) {
	std::thread{[this](Predicate p) {
		std::chrono::system_clock::time_point end;
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds;
		int sleepTime = 0;
		while (1) {
			Remove(p);
			end = std::chrono::system_clock::now();
			elapsed_seconds = end - start;
			sleepTime = elapsed_seconds.count() - SAFE_LIST_ELAPSED_SECONDS;
			start = std::move(end);

			if (sleepTime > 0) {
				sleep(elapsed_seconds.count());
			}
		}
	}
	};
}
/*  list end *****/

//}

#endif
