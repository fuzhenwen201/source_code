CXX = g++
INCLUDE = -I ../include
CFLAGS = -W -g
LIBS = -llog -ltcp -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -lredisopr -ltyinhttp -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon -lmemopr -lstdc++
#LIBS = -llog -lcommon   -lredisopr -lhiredis -ljson_linux-gcc-4.8.2_libmt
LIBPATH = -L$(APPHOME)/lib
BINPATH = $(APPHOME)/bin/
RPATH= -Wl,-rpath-link
TARGET = delhismarket
all:	$(TARGET)

delhismarket:	delhismarket.o
	$(CXX) delhismarket.cpp $(CFLAGS) $(INCLUDE) -l log -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS) $(RPATH)

clean:
	rm -f $(BINPATH)$(TARGET)
	rm -f *.o 
