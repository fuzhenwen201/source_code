#include "threadpool.h"
#include <iostream>

void fun1(int slp)
{
    printf("hello fun1! %d  %ld\n" ,slp,std::this_thread::get_id());
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

struct gfun {
    int operator()(int n) {
        printf("hello gfun! %d %ld\n" ,n, std::this_thread::get_id() );
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return 42;
    }
};

class A { 
public:
    static int Afun(int n = 0) {   //函数必须是 static 的才能直接使用线程池
        std::cout << "hello, Afun! " << n << " " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return n;
    }

    static std::string Bfun(int n, std::string str, char c) {
        std::cout << "hello Bfun! "<< str.c_str() <<"  " << (int)c <<"  " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return str;
    }
};

int main(){
    try {
        stds::threadpool executor(5);
        A a;
        std::future<void> ff = executor.commit(fun1,0);
        std::future<void> ff1 = executor.commit(fun1,1);
        std::future<void> ff2 = executor.commit(fun1,2);

        std::future<int> fg = executor.commit(gfun{},0);
        std::future<int> fg1 = executor.commit(gfun{},1);
        std::future<int> fg2 = executor.commit(gfun{},2);

        std::future<int> af = executor.commit(A::Afun, 1);
        std::future<int> af1 = executor.commit(A::Afun, 2);
        std::future<int> af2 = executor.commit(A::Afun, 3);

        std::future<std::string> gh = executor.commit(A::Bfun, 99,"mult args", 12);
        std::future<std::string> gh1 = executor.commit(A::Bfun, 98,"mult args", 13);
        std::future<std::string> gh2 = executor.commit(A::Bfun, 998,"mult args", 23);

        std::cout << " =======  fun1,55 ========= " << std::this_thread::get_id() << std::endl;
        executor.commit(fun1,55).get();    //调用.get()获取返回值会等待线程执行完

        std::cout << "end... " << std::this_thread::get_id() << std::endl;

        return 0;
    }catch (std::exception& e) {
        std::cout << "some unhappy happened...  " << std::this_thread::get_id() << e.what() << std::endl;
    }
}
