CXX = g++
INCLUDE = -I ../include 
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH=$(APPHOME)/lib/
TARGET=libdbsopr.so
all:	$(TARGET)

libdbsopr.so:	dbsopr.o
	$(CXX) dbsopr.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET) 

