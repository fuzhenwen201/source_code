//
// Created by alex on 12/28/16.
//

#ifndef TNODE_EXCEPTION_H
#define TNODE_EXCEPTION_H

#include <exception>
#include <string>
#include "snetwork_typedef.h"

namespace snetwork_xservice_tnode
{
#ifndef UNIVERSAL_TNODE_EXCEPTION

#define UNIVERSAL_TNODE_EXCEPTION(errcode,message) (TNodeException((errcode), (message), __FILE__, __LINE__, __DATE__, __TIME__))

#endif


    class TNodeException : public std::exception{
        public:
            TNodeException(uint32 errcode, const char* err_msg,
                       const char* file,
                       uint32 line,
                       const char* date,
                       const char* time);
            ~TNodeException();

            uint32 GetErrorCode();
            const char* what() const throw();
        private:
            uint32 errcode_;
            std::string err_msg_;
    };

}

#endif //TNODE_EXCEPTION_H
