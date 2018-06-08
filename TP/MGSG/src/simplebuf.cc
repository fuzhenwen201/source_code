//
// Created by sam on 7/28/17.
//

#include "simplebuf.h"
void SimpleBuf::WriteBuf(const char *buf, int len) {
    if(buf == nullptr || len == 0){
        return;
    }

    if((m_size - m_write_off) < len){
        realloc(m_buf, m_write_off + len);
        m_size = m_write_off + len;
    }

//    if((m_size - m_write_off) < len){
//        //realloc(m_buf, (len - (m_size - m_write_off)) + m_write_off);
//        realloc(m_buf, len + 2*m_write_off - m_size);
//        m_size = m_write_off + len;
//    }

    memcpy((char*)m_buf + m_write_off, buf, len);
    m_write_off += len;
}

int SimpleBuf::ReadBuf(char *buf, int len) {
    if(buf == nullptr || len == 0) {
        return -1;
    }

    if(len > m_write_off) {
        len = m_write_off;
    }

    memcpy(buf, m_buf, len);
    memmove(m_buf, (const char*)m_buf + len, m_write_off - len);
    m_write_off -= len;

    return len;
}

int SimpleBuf::GetBufLenght() {
    return  m_write_off;
}

char* SimpleBuf::GetBuf() {
    return m_buf;
}