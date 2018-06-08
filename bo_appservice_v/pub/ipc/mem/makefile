CXX = g++
INCLUDE = -I ../include -I$(BOOST_INCLUDE)
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
#LIBS = -lcommon  -lboost_system -lboost_thread -lrt -lpthread -lboost_atomic -lboost_container -lboost_chrono -lboost_context -lboost_regex
#LIBPATH = -L$(APPHOME)/lib/ -L$(BOOST_LIB)
LIBPATH = $(APPHOME)/lib/
TARGET = libmemopr.so
all:    $(TARGET)

libmemopr.so:   memopr.o memopr_boost.o
	$(CXX) memopr.cpp memopr_boost.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)
memopr.o : memopr.cpp
	$(CXX) -c memopr.cpp $(INCLUDE) $(LIBPATH)
memopr_boost.o : memopr_boost.cpp
	$(CXX) -c memopr_boost.cpp $(INCLUDE) $(LIBPATH)

clean:
	rm -f $(LIBPATH)$(TARGET)
	rm -f *.o
