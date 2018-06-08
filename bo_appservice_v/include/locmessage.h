#ifndef _LOCMESSAGE_H_
#define _LOCMESSAGE_H_

#include "log.h"
#include "const.h"
#include "redis.h"
#include "kerner.h"
#include "msgopr.h"
#include "memopr.h"
#include "common.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./json/json.h"
#include "./curl/curl.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <sys/time.h>
#include <sstream>
#include <map>
#include <list>

#include <hash_map>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>


using boost::asio::ip::tcp;

class locmsg
{
	public:
		locmsg();

		const char* data() const ;

		char* data();


		size_t length() const;

		const char* body() const;

		char* body();

		size_t body_length() const ;

		void body_length(size_t new_length) ;

		bool decode_header();

		void encode_header();

	private:
		char data_[LOC_HEADER_LEN + LOC_BODY_LEN];
		size_t body_length_;
};

#endif
