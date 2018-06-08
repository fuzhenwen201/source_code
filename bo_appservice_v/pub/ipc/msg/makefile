CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
TARGET = libmsgopr.so
all:	$(TARGET)

libmsgopr.so:	msgopr.o
	$(CXX) msgopr.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET)
	rm -f *.o
