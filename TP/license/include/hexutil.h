#ifndef HEXUTIL_H
#define HEXUTIL_H

#include <string>
#include <vector>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

using namespace std;
typedef unsigned char byte;
string bytesToHexString(const byte *in, size_t size);
vector<byte> hexStringToBytes(const string &str);

class CPUID {
    uint32_t regs[4];

public:
    void load(unsigned i)
    {
        asm volatile
        (
        "cpuid" : "=a" (regs[0]),
        "=b" (regs[1]),
        "=c" (regs[2]),
        "=d" (regs[3]) : "a" (i), "c" (0)
        );
    }

    const uint32_t &EAX() const {return regs[0];}
    const uint32_t &EBX() const {return regs[1];}
    const uint32_t &ECX() const {return regs[2];}
    const uint32_t &EDX() const {return regs[3];}
};

struct HardwareUID {
    long long hostid;
    unsigned int cpu_signature;
    unsigned int cpu_featurebits[3];

    void collect()
    {
        CPUID cpuID;

        // hostid (returns a value based on the IP address)
        hostid = gethostid();

        cpuID.load(1); // Processor Info and Feature Bits
        cpu_signature = cpuID.EAX();
        cpu_featurebits[0] = cpuID.EDX();
        cpu_featurebits[1] = cpuID.ECX();
        cpu_featurebits[2] = cpuID.EBX() & 0x00FFFFFF; // ignore first 2 bytes

    }

    void print()
    {
        cout << "Host id            = " << hostid             << endl;
        cout << "CPU signature      = " << cpu_signature      << endl;
        cout << "CPU featurebits[0] = " << cpu_featurebits[0] << endl;
        cout << "CPU featurebits[1] = " << cpu_featurebits[1] << endl;
        cout << "CPU featurebits[2] = " << cpu_featurebits[2] << endl;
    }
};

#endif // HEXUTIL_H
