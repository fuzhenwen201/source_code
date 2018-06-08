#ifndef _UNPACK_H_
#define _UNPACK_H_

#include "log.h"
#include "tcp.h"
#include "const.h"
#include "kerner.h"
#include "msgopr.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./curl/curl.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

HttpMap unPacHttp;

void CovHttpMap2Str(pHttpMap pMap, char* buf);



#endif
