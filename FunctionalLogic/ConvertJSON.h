#ifndef __CONVERTJSON_H__
#define __CONVERTJSON_H__



const int g_iOpenFileFailed = -1;
#include <json.hpp>
#include <stdio.h>
#include <sys/stat.h>
#include "GetSysInfo.h"
//#include"PublicData.h"
#define FILEJSONDATA_NAME_MAX 256



using json = nlohmann::json;

  

class GetJSON
{

public:
  GetJSON()
  {
    m_pFile = NULL;
    memset(m_SystemInfoJSONdata_Name, 0x00, FILEJSONDATA_NAME_MAX);
    memset(m_FileSystemInfoJSON_Name, 0x00, FILEJSONDATA_NAME_MAX);
    memset(m_FileSystemInfoJSON_Name,0x00,FILEJSONDATA_NAME_MAX);
   memset(m_SystemAccountSecurityCheck,0x00,FILEJSONDATA_NAME_MAX);
   memset(m_SystemModuleSecurityCheck,0x00,FILEJSONDATA_NAME_MAX);

    strcpy(m_SystemInfoJSONdata_Name, "SysInfo.json");
    strcpy(m_FileSystemInfoJSON_Name, "FileSystemInfo.json");
    strcpy(m_SystemAccountSecurityCheck,"SystemAccountSecurityCheck.json");
strcpy(m_SystemModuleSecurityCheck,"SystemModuleSecurityCheck.json");
strcpy(m_InternetAccessRecord,"Chrome_BrowserHIstory.json");
strcpy(m_FireFoxInternetAccessRecord,"FireFox_BrowserHIstory.json");
strcpy(m_InstalledPackages,"InstalledPakages.json");
strcpy(m_MountPointInfo,"MountPointInfo.json");
strcpy(m_NetWorkHardWareInfo,"NetWorkHardWare.json");
strcpy(m_USB_MassStorageDevice,"USB_MassStorageDevice.json");
 strcpy(m_BlueTooth,"BlueTooth.json");
 strcpy(m_USBEventHistory,"USBEventHistory.json");
 strcpy(m_CheckInternet,"ConnectionHosname.json");
 
  };

  //function: create JSON Data File
int GetSysInfo_Json(); 
int GetFilesystem_Json();
int GetSystemAccountSecurityCheck_Json();
int GetSystemModuleSecurityCheck_Json();
int GetChromeInternetAccessRecord_Json();
int GetFireFoxInternetAccessRecord_Json();
int GetSoftwareCheckOfSystemInstallationAPT_Json();
int GetSoftwareCheckOfSystemInstallationRPM_Json();
int GetMountPoint_Json();
int GetNetWorkHardWare_JSON();//网卡信息
int GetgetUSB_MassStorageDevice_JSON();//USB存储设备硬件信息
int GetgetBlueTooth_JSON();//蓝牙设备信息
int GetUSB_EventsHistory_JSON();  //usb插拔记录
int isConnectInternet_JSON();//检查操作系统是否能上网

  bool IsExist(const char *kpcFile);



private:
std::vector<st_checkConnection>vecConnectionInternet;
std::vector<st_filtered>vecUSB_EventsHistory;
std::vector<st_Hardware_lshw> vecBlueTooth_HW;
std::vector<st_Hardware_lshw> vecUSB_MassStorageDevice_HW;
std::vector<st_Hardware_lshw> vecNetwork_HW;
  st_sysinfo st_Sysinfo;
  std::vector<st_fileSystem> vecFileSystem;
  std::vector< st_sysAccount> vecSysAccountSecurity;
  std::vector<st_SystemModuleSecurityCheck>vecSystemModuleSecurityCheck;
  std::vector<st_BrowserHistory>  vec_CHromBrowserHistory;
  std::vector<st_BrowserHistory>  vec_FireFoxBrowserHistory;
  std::vector<st_aptPackgesInfo> vec_aptPackageInfo;
  std::vector<st_rpmPackagesInfo> vec_rpmPackageInfo;
  std::vector< st_MountPoint> vec_MountPoint;


  json SysInfo_JSON_Array;
  //std::vector<json> vec_FileSystemInfo_Array;
  char m_SystemInfoJSONdata_Name[FILEJSONDATA_NAME_MAX];
  char m_FileSystemInfoJSON_Name[FILEJSONDATA_NAME_MAX];
  char m_SystemAccountSecurityCheck[FILEJSONDATA_NAME_MAX];

char m_SystemModuleSecurityCheck[FILEJSONDATA_NAME_MAX];
char m_InternetAccessRecord[FILEJSONDATA_NAME_MAX];
char m_FireFoxInternetAccessRecord[FILEJSONDATA_NAME_MAX];
char m_InstalledPackages[FILEJSONDATA_NAME_MAX];
char m_MountPointInfo[FILEJSONDATA_NAME_MAX];
char m_NetWorkHardWareInfo[FILEJSONDATA_NAME_MAX];
char m_USB_MassStorageDevice[FILEJSONDATA_NAME_MAX];  
char m_BlueTooth[FILEJSONDATA_NAME_MAX];  
char m_USBEventHistory[FILEJSONDATA_NAME_MAX];  
char m_CheckInternet[FILEJSONDATA_NAME_MAX];  
  GetSysInfo m_cGetSysinfo;
  FILE *m_pFile; //JSON_Data文件句柄

public:
  int m_PreJSON_File(const char *fJSON_Name, void *stData = NULL, int nMark = 12);
  int m_WriteJSON_File(const char *, const json *ptr_JSONdata);
  int test(void);

};
#endif
