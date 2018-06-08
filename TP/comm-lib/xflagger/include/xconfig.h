/*************************************************************************
	> File Name: xconfig.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 09:43:46 AM CST
 ************************************************************************/
#ifndef XFLAGGER_XCONFIG_H_
#define XFLAGGER_XCONFIG_H_
#include <gflags/gflags.h>

namespace snetwork_xservice_xflagger {
class XConfig {
	public:
		XConfig();
		virtual ~XConfig();

		XConfig(const XConfig& r) = delete;
		XConfig& operator=(const XConfig& r) = delete;
		XConfig(XConfig& r) = delete;
		XConfig& operator=(XConfig& r) = delete;

		virtual void Init() = 0 ;
};

class XFlagger {
	public:
		XFlagger();
		virtual ~XFlagger();

		XFlagger(const XFlagger& r) = delete;
		XFlagger& operator=(const XFlagger& r) = delete;
		XFlagger(XFlagger&& r) = delete;
		XFlagger& operator=(XFlagger&& r) = delete;

		void ParseCommandLine(int* argc, char*** argv, bool cv);
		//virtual void FlagXConfig(XConfig* cfg) = 0 ;
		virtual void FlagXConfig(XConfig* cfg) = 0 ;
		virtual XConfig* GetConfig(void) const = 0;
		
};
}

#endif
