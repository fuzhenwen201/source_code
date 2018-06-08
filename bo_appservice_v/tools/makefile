CXX = g++
INCLUDE = -I ../include 
CFLAGS = -W -g
LIBS = -llog -ltcp -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -ltyinhttp -lredisopr -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon
LIBPATH = -L$(APPHOME)/lib/
BINPATH = $(APPHOME)/bin/
TARGET = redis_tool
$(TARGET):redis_tool.o
	$(CXX) redis_tool.cpp $(CFLAGS) $(INCLUDE) -l log  -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS)

clean:
	rm -f $(BINPATH)$(TARGET) *.o
