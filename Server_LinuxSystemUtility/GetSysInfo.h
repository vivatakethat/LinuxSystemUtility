
 

#include"PublicData.h"
#include"GetSysInfoImpl.h"
#include<regex>
#include <cstdlib>

#include <cstdio>

#include<stdio.h>

#include<string.h>

#include<iostream>
#include <fstream>

#include<vector>
      #include <unistd.h>
#include"sqlite3.h"
#include<stdio.h>
// printf
#include<string.h>
// memset
#include<stdlib.h>
// for exit(0);
#include<sys/socket.h>
#include<errno.h>
// For errno - the error number
#include<netdb.h>
// hostent
#include<arpa/inet.h>


// lshw
#include"./lshw/src/core/mounts.h"
#include "./lshw/src/core/options.h"
 
#include "./lshw/src/core/network.h"
#include "./lshw/src/core/hw.h"
#include"./lshw/src/core/disk.h"
#include"./lshw/src/core/main.h"
#include"./lshw/src/core/partitions.h"
#include "./lshw/src/core/print.h"
#include "./lshw/src/core/config.h"
 #ifndef NONLS
#include <locale.h>
#endif


#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)



#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)

#include <windows.h>

#include <intrin.h>

#endif



// struct sysinfo {

//   std::string os;

//   std::string Architecture;

//   std::string cpu;

//   std::string ram;

//   std::string LinuxkernelVersion;

// };
using std::string;
using std::vector;

  
class GetSysInfo:public GetSysInfoImpl
{

public: 

GetSysInfo();
std::vector<st_sysAccount> getSysAccountSecurityCheck();//1.系统账号安全检查

  st_sysinfo getOperationSystemInfo();  //12:（国产机设备）硬件设备

 void getFileSystemInfo(vector<st_Hardware_lshw> &get_disk);//    lshw -c disk

std::vector<st_fileSystem>getFileSystemInfo_InC();//3.文件系统和文件挂载检查

 std::vector<st_SystemModuleSecurityCheck> getSystemModuleSecurityCheck();// [2]系统设备驱动安全检查

    std::vector<st_BrowserHistory> getChromeInternetAccessRecord();  //6.0 Chrome 网络记录
   std::vector<st_BrowserHistory> getFirefoxInternetAccessRecord();  //6.1 Firefox网络记录
 std::vector<st_aptPackgesInfo> getSoftwareCheckOfSystemInstallation_APT(bool &isAPT);//10.系统的安装的软件检查
  std::vector<st_rpmPackagesInfo>  getSoftwareCheckOfSystemInstalltaion_RPM(bool &isRPM); //10.1 系统的安装的软件检查

  std::vector<st_MountPoint>  getSystemMountPoint(); //10.2 当前系统挂载情况
void getNetWorkHardWare(std::vector<st_Hardware_lshw>& get_Network);// 获得硬件信息: 网卡、
void getUSB_MassStorageDevice(std::vector<st_Hardware_lshw>& get_USB);// 获得硬件信息:USB 存储设备 
void getBluetooth(std::vector<st_Hardware_lshw>& vecGetBluetooth);//获取蓝牙设备   IR、

int  getUSB_EventHistory(std::vector<st_filtered>& vecGetUSB_EventHIstory);//系统USB插入拔出时事件记录 :journalctl

void checkInternetConnect(std::vector<st_checkConnection> & vec_st_checkConnection);//检查操作系统是否能上网
void test();

private:

   const char* io_StrDevices(const char * ptr_DeviceName);//HardDriver Function

  sqlite3 *db;

    char *zErrMsg = 0;

    int rc;

    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	int IsFolderExist(const char* path);
	int IsFileExist(const char* path);
 string splite_Item(const string src,const char * key ,const char *eof,char** result);
string GetStdoutFromCommand(string cmd,int&);  //C++封装后的popen
void GetPackateName(const string st_InstalledPackage);
 vector<string> splitGetName(string str,string pattern);
 vector<string> getAllUserName();

 vector<string> m_UserName;
  
};







 