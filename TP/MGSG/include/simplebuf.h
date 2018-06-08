//
// Created by leaker on 7/28/17.
//

#ifndef MGSG_SIMPLEBUF_H
#define MGSG_SIMPLEBUF_H

#include <stdlib.h>
#include <memory.h>
class SimpleBuf {
public:
    SimpleBuf(): m_buf((char*)malloc(1)),
                 m_size(1),
                 m_write_off(0){

    }
    ~SimpleBuf(){
        if(m_buf != nullptr)
        {
            free(m_buf);
        }
    }

public:
    void WriteBuf(const char* buf, int len);
    int ReadBuf(char* buf, int len);
    int GetBufLenght();
    char* GetBuf();
private:
    char* m_buf;
    int m_write_off;
    int m_size;
};
#endif //MGSG_SIMPLEBUF_H
