CXX = g++
INCLUDE = -I ../include -I$(BOOST_INCLUDE)
CFLAGS = -W -g  
CXXFLAGS      += -std=gnu++11
LIBS = -llog -ltcp -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -lredisopr -ltyinhttp -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon -lmemopr -lboost_system -lboost_thread
LIBPATH = -L$(APPHOME)/lib/ -L$(BOOST_LIB)
BINPATH = $(APPHOME)/bin/
TARGET = onlineservice
all:	$(TARGET)

onlineservice: onlineservice.o
	$(CXX) onlineservice.cpp $(CFLAGS) $(INCLUDE) -l log  -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS)
onlineservice.o: onlineservice.cpp
	$(CXX) -c onlineservice.cpp $(INCLUDE) $(LIBPATH)


clean:
	rm -f $(BINPATH)$(TARGET)
	rm -f *.o 
