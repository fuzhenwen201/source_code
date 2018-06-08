CXX = g++
INCLUDE = -I ../include -I ../include/event2
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH=$(APPHOME)/lib/
TARGET=libredisopr.so
all:	$(TARGET)

libredisopr.so:	redisopr.o
	$(CXX) redisopr.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET) 

