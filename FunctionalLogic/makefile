CC= g++  
 
  
CFLAGS= -ggdb -Wall -D __linux__  -std=c++11   -fPIC
INCLUDE= -I./single_include  
  


libGetsystemInfo.so: convertjson.o sqlite3.o GetSysInfo.o
	g++ -ggdb -Wall -std=c++11 -D __LINUX__ -std=c++11  -shared -g  -fPIC -pthread    -I./single_include     GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o lshw.o    -o libGetsystemInfo.so
	cp libGetsystemInfo.so /home/takethat/github_repository/LinuxSystemUtility/build-Server_LinuxSystemUtilityu2044-Desktop_Qt_5_13_0_GCC_64bit-Debug



#libGetsystemInfo.so: convertjson.o sqlite3.o GetSysInfo.o
#	g++ -ggdb -Wall -std=c++11 -D __LINUX__ -std=c++11 -lmcheck -shared -fPIC -pthread   -I./single_include    -Xlinker --unresolved-symbols=ignore-in-shared-libs  -o libGetsystemInfo.so  GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o -Wl,-rpath,. -L. lshw.o -llshw -lresolv  -ldl 
 
	 
GetSysInfo.o: GetSysInfo.cpp 
	$(CC)  -c $(CFLAGS) $(INCLUDE) -o GetSysInfo.o GetSysInfo.cpp

convertjson.o: ConvertJSON.cpp  
	rm  -f getsystem *.o *.h.gch *hpp.gch
	$(CC)  -c $(CFLAGS) $(INCLUDE) -o convertjson.o ConvertJSON.cpp  

sqlite3.o: sqlite3.c 
	gcc -c -fPIC sqlite3.c -o sqlite3.o -lpthread -ldl


.PHONY:	clean
clean:
	rm  -f getsystem sqlite3.o  convertjson.o  GetSysInfo.o *.h.gch *hpp.gch libGetsystemInfo.so





# g++ -ggdb -Wall -std=c++11 -D __LINUX__ -std=c++11 -lmcheck -shared -fPIC -pthread   -I./single_include -I./parted/include/  -Xlinker --unresolved-symbols=ignore-in-shared-libs  -o libGetsystemInfo.so  GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o libparted.so.2  lshw.o -L. -llshw -lresolv  -ldl
 
