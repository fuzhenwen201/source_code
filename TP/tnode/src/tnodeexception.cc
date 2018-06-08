//
// Created by alex on 12/28/16.
//
#include <sstream>
#include "tnodeexception.h"

using namespace snetwork_xservice_tnode;


TNodeException::TNodeException(uint32 errcode, const char* err_msg,
                                const char* file,
                                uint32 line,
                                const char* date,
                                const char* time){
    std::ostringstream s1;

    exception();
    errcode_ = errcode;

    s1 << "\nVersion Date : " << date << " " << time << "\nFile :" << file << "\nLine : " << line << "\nError : " << errcode_ << "\nError Message : " << err_msg << std::endl;
    err_msg_ = s1.str();
}

TNodeException::~TNodeException(){

}

uint32 TNodeException::GetErrorCode(){
    return errcode_;
}

const char* TNodeException::what() const throw(){
    return err_msg_.c_str();
}