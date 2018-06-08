CXX = g++
INCLUDE = -I ../include
#CFLAGS = -W -g
CFLAGS = -fPIC -shared -g
LIBPATH = $(APPHOME)/lib/
TARGET = libtinyxml.so
all:	$(TARGET)

libtinyxml.so:	tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o
	$(CXX) tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp $(CFLAGS) $(INCLUDE) -o $(LIBPATH)$(TARGET)

clean:
	rm -f $(LIBPATH)$(TARGET) 

