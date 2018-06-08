CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
TARGET = libthreadpool.so
all:	$(TARGET)

libthreadpool.so:	thread.o
	$(CXX) thread.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET) 

