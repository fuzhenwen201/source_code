CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
TARGET = libtcp.so
all:	$(TARGET)

libtcp.so:	tcp.o
	$(CXX) tcp.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET) 
	rm -f $(APPHOME)/pub/com/tcp/*.o

