//
// Created by derek on 12/22/16.
//

#ifndef NEW_UNITTEST_H
#define NEW_UNITTEST_H

#include <atomic>
#include "../include/tnode.h"
#include "../include/tnodeconfig.h"
#include "../include/tnodeconfig_v0.h"

namespace snetwork_xservice_tnode_unittest {

    class UnitTest {
    public:
        UnitTest();
        ~UnitTest();

        void Run();

        void Event001(const snetwork_xservice_tnode::TNodeConfigV0 &cfg);
        void Event002(const snetwork_xservice_tnode::TNodeConfigV0 &cfg);
        void Event003(const snetwork_xservice_tnode::TNodeConfigV0 &cfg);
        void Event004();
        void Event005();
        void Event006();

        void TestCase1();
        void TestCase2();
        void TestCase3();
        void TestCase4();
        void TestCase5();
        void TestCase6();
        void TestCase7();
        void TestCase8();
        void TestCase9();
        void TestCase10();
        void TestCase11();
        void TestCase12();
        void TestCase13();
        void TestCase14();
        void TestCase15();
        void TestCase16();
        void TestCase17();
        void TestCase18();

    private:
        snetwork_xservice_tnode::TNode tnode_;
        std::atomic_int test_count_;
        int  test_A;
        int  test_B;
    };
}


#endif //NEW_UNITTEST_H
