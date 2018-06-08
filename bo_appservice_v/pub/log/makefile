CC = gcc
C++ = g++
LINK = g++
CCFLAGS = $(COMPILER_FLAGS) -c -g -fPIC
C++FLAGS = $(COMPILER_FLAGS) -c -g -fPIC
TARGET = liblog.so

INCLUDE = -I ../include
C++FILES = log.cpp
CFILES = slog.c




OBJFILE = $(CFILES:.c=.o) $(C++FILES:.cpp=.o)

all:	$(TARGET)

$(TARGET): $(OBJFILE)
	$(LINK) $^  -Wall -fPIC -shared -o $@


%.o:%.c
	$(CC) -o $@ $(CCFLAGS) $< $(INCLUDE)

%.o:%.cpp
	$(C++) -o $@ $(C++FLAGS) $< $(INCLUDE)
LIBPATH = $(APPHOME)/lib/

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJFILE)
install:
	install -m 644 liblog.so $(LIBPATH)
