CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
LIBHOME = -L$(APPHOME)/lib/
LIBS = -ljson_linux-gcc-4.8.2_libmt
TARGET = libtyinhttp.so
all:	$(TARGET)

libtyinhttp.so:	tyinhttp.o
	$(CXX) tyinhttp.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET) $(LIBS) $(LIBHOME)

clean:
	rm -f $(LIBPATH)$(TARGET) 
	rm -f *.o

