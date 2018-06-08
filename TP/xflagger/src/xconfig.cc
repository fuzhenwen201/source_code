/*************************************************************************
	> File Name: xconfig.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 10:05:59 AM CST
 ************************************************************************/

#include"xconfig.h"

namespace snetwork_xservice_xflagger {
/* XConfig begin ***/
XConfig::XConfig() {
}

XConfig::~XConfig() {
}
/* XConfig end ***/

/* XFlagger begin ***/
XFlagger::XFlagger() {

}

XFlagger::~XFlagger() {
}

void XFlagger::ParseCommandLine(int* argc, char*** argv, bool cv) {
    google::ParseCommandLineFlags(argc, argv, cv);
}
/* XFlagger end ***/
}



