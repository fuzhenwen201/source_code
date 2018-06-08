CXX = g++ 
INCLUDE = -I ../include -I$(BOOST_INCLUDE)
CFLAGS = -W -g
LIBS = -llog -ltcp -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -lredisopr -ltyinhttp -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon -lmemopr
LIBPATH = -L$(APPHOME)/lib/ -L$(BOOST_LIB)
BINPATH = $(APPHOME)/bin/
TARGET = daemon
all:	$(TARGET)

daemon:   daemon.o
	$(CXX) daemon.cpp $(CFLAGS) $(INCLUDE) -l log  -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS)

daemon.o : daemon.cpp
	$(CXX) -c daemon.cpp $(INCLUDE) $(LIBPATH)

clean:
	rm -f $(BINPATH)$(TARGET)
	rm -f *.o 
