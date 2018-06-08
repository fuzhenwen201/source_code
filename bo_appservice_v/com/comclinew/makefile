CXX = g++
INCLUDE = -I ../include -I ../../include
CFLAGS = -W -g
#LIBS = -llog -ltcp -lmsgopr -levent_extra -levent_pthreads -levent -levent_core -lcurl  -lcommon -lmemopr
LIBS = -llog  -ltcp -lmsgopr -levent -levent_core -levent_extra -levent_pthreads -lcurl -lredisopr -ltyinhttp -lhiredis -ljson_linux-gcc-4.8.2_libmt -lcommon -lmemopr  
LIBPATH = -L$(APPHOME)/lib
BINPATH = $(APPHOME)/bin/
RPATH= -Wl,-rpath-link
TARGET = test
TARGET_1 = httpclient
all:	$(TARGET_1)
 
$(TARGET):	main.o HttpClient.o HttpCliBusiness.o
	$(CXX) HttpClient.cpp HttpCliBusiness.cpp main.cpp $(CFLAGS) $(INCLUDE) -l log -o $(BINPATH)$(TARGET) $(LIBPATH) $(LIBS) $(RPATH) 
main.o: main.cpp
	$(CXX) -c main.cpp $(INCLUDE) $(LIBPATH)
HttpClient.o: HttpClient.cpp
	$(CXX) -c HttpClient.cpp $(INCLUDE) $(LIBPATH)
HttpCliBusiness.o: HttpCliBusiness.cpp
	$(CXX) -c HttpCliBusiness.cpp $(INCLUDE) $(LIBPATH)

$(TARGET_1): asncmain.o multcurl.o HttpCliMultiBusiness.o AsncHttpClient.o
	$(CXX) asncmain.cpp multcurl.cpp HttpCliMultiBusiness.cpp AsncHttpClient.cpp $(CFLAGS) $(INCLUDE) -l log -o $(BINPATH)$(TARGET_1) $(LIBPATH) $(LIBS) $(RPATH)
asncmain.o: asncmain.cpp  
	$(CXX) -c asncmain.cpp $(INCLUDE) $(LIBPATH)
multcurl.o: multcurl.cpp
	$(CXX) -c multcurl.cpp $(INCLUDE) $(LIBPATH)
HttpCliMultiBusiness.o: HttpCliMultiBusiness.cpp
	$(CXX) -c HttpCliMultiBusiness.cpp $(INCLUDE) $(LIBPATH)
AsncHttpClient.o: AsncHttpClient.cpp
	$(CXX) -c AsncHttpClient.cpp $(INCLUDE) $(LIBPATH)

clean:
	rm -f $(BINPATH)$(TARGET_1)
	rm -f *.o



