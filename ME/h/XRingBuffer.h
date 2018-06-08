#pragma once  
#include "StockExcDef.h"
#include <muduo/base/Logging.h>
  
typedef unsigned int DWORD;
const unsigned char XRING_BUFFER_READ_POS_AND_WRITE_POS_SIZE = 2;  
  
class XRingBuffer  
{  
public:  
    XRingBuffer(const DWORD size);  
    ~XRingBuffer();  
  
    bool pushData(const void* data, const DWORD size);  
  
    bool copyData(void* dest, const DWORD destSize, const DWORD copySize);  
    bool popData(void* dest, const DWORD destSize, const DWORD popSize);  
    bool popData(const DWORD popSize);  
    const DWORD getUsedSize() const;  
    const DWORD getFreeSize() const;  
private:  
    bool copyDataWithAddReadPosOption(void* dest, const DWORD destSize, const DWORD popSize, bool addReadPos);  
private:  
    char* _buffer;  
    const DWORD _size;  
    volatile DWORD _write_pos;  
    volatile DWORD _read_pos;  
}; 