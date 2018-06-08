CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
TARGET = libmempool.so
all:	$(TARGET)

libmempool.so:	mempool.o
	$(CXX) mempool.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET)
	rm -f *.o
