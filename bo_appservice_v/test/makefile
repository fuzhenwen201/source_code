CXX = g++
INCLUDE = -I ../include -I$(BOOST_INCLUDE) -I ../include/event2 
CFLAGS = -W -g
LIBS = -llog -ltcp -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -lredisopr -ltyinhttp -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon -lmemopr  -lboost_system -lstdc++
LIBPATH = -L$(APPHOME)/lib/ -L$(BOOST_LIB)
BINPATH = $(APPHOME)/bin/
TARGET = num
all:	$(TARGET)

num:	
	$(CXX) getonlinenum.cpp $(CFLAGS) $(INCLUDE) -l log  -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS)
clean:	
	rm -f $(BINPATH)$(TARGET) 
