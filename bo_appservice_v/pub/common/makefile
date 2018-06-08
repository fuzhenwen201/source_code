CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
TARGET = libcommon.so
all:	$(TARGET)

libcommon.so:	common.o
	$(CXX) common.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET)
	rm -f $(APPHOME)/pub/commom/*.o 

