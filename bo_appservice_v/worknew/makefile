CXX = g++
INCLUDE = -I ../include -I$(BOOST_INCLUDE)
CFLAGS = -W -g
LIBS = -llog -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -lredisopr -ltyinhttp -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon -lmemopr  -lboost_system -lpthread -lrt
LIBPATH = -L$(APPHOME)/lib -L$(BOOST_LIB)
BINPATH = $(APPHOME)/bin/
TARGET = worknew

all: $(TARGET)
worknew: worknew.o cbusiness.o cmsghandle.o workinit.o
	$(CXX) worknew.cpp cbusiness.cpp cmsghandle.cpp workinit.cpp $(CFLAGS) $(INCLUDE) -l log  -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS)
worknew.o : worknew.cpp
	$(CXX) -c worknew.cpp $(INCLUDE) $(LIBPATH)
cbusiness.o : cbusiness.cpp
	$(CXX) -c cbusiness.cpp $(INCLUDE) $(LIBPATH)
cmsghandle.o : cmsghandle.cpp
	$(CXX) -c cmsghandle.cpp $(INCLUDE) $(LIBPATH)
workinit.o : workinit.cpp
	$(CXX) -c workinit.cpp $(INCLUDE) $(LIBPATH)

clean:
	rm -f $(BINPATH)$(TARGET)
	rm -f *.o 
