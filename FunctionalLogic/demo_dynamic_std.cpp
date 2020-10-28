//#g++ -ldl  -Wl,-rpath,./ -std=c++11 -o demo_std demo_dynamic_std.cpp


//#include"CAGMDLL.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include<iostream>
#include<memory>
#include<string>
#include"GetSysInfoImpl.h"
//#include <boost/shared_ptr.hpp>
//#include <boost/make_shared.hpp>


typedef std::shared_ptr<void>(*LPDLLCREATEFUNC)(void);
int main(int argc,char**argv)
{
	LPDLLCREATEFUNC lpFunc;
	int iRet = 0;
	std::shared_ptr<void> lpObject;


	//void* hLib = dlopen("./libGate_Gunnebo_QCD_NEW.so", RTLD_LAZY);
	//void* hLib = dlopen("./libGate_Gunnebo.so", RTLD_LAZY);
	if (argc < 2)
	{
		printf("The input Command is invalid!\n");
		printf("Usage: ./a.out [libGate_Gunnebo_QCD_NEW.so]\n");
			return -1;
	}


	void* hLib = dlopen(argv[1], RTLD_LAZY);
	if (NULL == hLib) {
		printf("dlopen error is[%s]\n", dlerror());
	//	iRet = UNKNOW_ERROR;
	}
	else {
		int iRet = 0;
		char * perror;
		lpFunc = (LPDLLCREATEFUNC)dlsym(hLib, "CreateInstance");  //take:����ʵ��
		perror = dlerror();
		if (NULL == perror) {
			iRet = 0;
			lpObject = lpFunc();
		}
		else {
		//	iRet = UNKNOW_ERROR;
			printf("dlsym error :%s\n", perror);
			return  iRet;
		}
			std::shared_ptr<void> take = lpObject;

			

			std::shared_ptr< GetSysInfoImpl> m_OGate = std::static_pointer_cast< GetSysInfoImpl>(lpObject);

			//		m_OGate->test();


//m_OGate->getSysAccountSecurityCheck();

 
vector<st_Hardware_lshw> get_lshwFilesystem;
std::vector<st_Hardware_lshw> get_Network;
    m_OGate->getNetWorkHardWare(get_Network);

// std::vector<st_filtered> GetUSB_EventHIstory;
// m_OGate->getUSB_EventHistory(GetUSB_EventHIstory);

// cout<<"GetUSB_EventHIstory.size="<<GetUSB_EventHIstory.size()<<endl;

// for(int i=0;i<GetUSB_EventHIstory.size();i++)
// {
// 	cout<<"["<<i<<"]"<<GetUSB_EventHIstory[i].conn<<endl;
// }
		// 	if(m_OGate!=NULL)
		// 	{
		// 			cout<<__LINE__<<endl;
		// vector<st_sysAccount> result_Account=m_OGate->getSysAccountSecurityCheck();
		// 				cout<<__LINE__<<endl;
		// for(int i=0;i< result_Account.size();i++)
		// {
		// 	cout<<"Username:"<< result_Account[i].UserName<<endl;
		// }
		// cout<<__LINE__<<endl;
		// 	}
		// 	else
		// 	{
		// 		cout<<"m_OGate is NULL!!!!!"<<endl;
		// 	}
			
	
	}
	return iRet;
}

 



 /*
g++ -ggdb -std=c++11 demo_dynamic_std.cpp -ldl



# so:
g++ -ggdb -Wall -std=c++11 -D __LINUX__ -std=c++11 -lmcheck -shared -fPIC -pthread   -I./single_include -I./parted/include/  -Xlinker --unresolved-symbols=ignore-in-shared-libs  -o libGetsystemInfo.so  GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o libparted.so.2  lshw.o -L. -llshw -lresolv  -ldl 

g++ -ggdb -Wall -std=c++11 -D __LINUX__ -std=c++11 -lmcheck -shared -fPIC -pthread   -I./single_include -I./parted/include/  -Xlinker --unresolved-symbols=ignore-in-shared-libs  -o libGetsystemInfo.so  GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o libparted.so.2  lshw.o -L. -llshw -lresolv  -ldl 



# manjaro:

g++ -ggdb -Wall -std=c++11 -D __LINUX__ -std=c++11 -lmcheck -shared -fPIC -pthread   -I./single_include -I./parted/include/  -Xlinker --unresolved-symbols=ignore-in-shared-libs  -o libGetsystemInfo.so  GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o -L bparted  lshw.o -L. -llshw -lresolv  -ldl 


 */





//   g++ -ggdb --use-ld=gold -O2  -Wall -std=c++11 -D __LINUX__ -std=c++11 -lmcheck -shared -fPIC -pthread   -I./single_include -I./parted/include/  -Xlinker --unresolved-symbols=ignore-in-shared-libs  -o libGetsystemInfo.so  GetSysInfo.cpp  ConvertJSON.cpp  sqlite3.o -L bparted  lshw.o -L. -llshw -lresolv  -ldl 


 
 