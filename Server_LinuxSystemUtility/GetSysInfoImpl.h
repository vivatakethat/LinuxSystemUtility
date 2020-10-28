 
#include"PublicData.h"

class GetSysInfoImpl
{
    public:
virtual std::vector<st_sysAccount> getSysAccountSecurityCheck()=0;//1.系统账号安全检查

  virtual st_sysinfo getOperationSystemInfo()=0;  //12:（国产机设备）操作系统硬件设备

virtual  void  getFileSystemInfo(vector<st_Hardware_lshw> &get_disk)=0;// with"   lshw -c disk'

  virtual std::vector<st_fileSystem>getFileSystemInfo_InC()=0;//3.文件系统和文件挂载检查

  virtual  std::vector<st_SystemModuleSecurityCheck> getSystemModuleSecurityCheck()=0;// [2]系统设备驱动安全检查

   virtual   std::vector<st_BrowserHistory> getChromeInternetAccessRecord()=0;  //6.0 Chrome 网络记录
   virtual  std::vector<st_BrowserHistory> getFirefoxInternetAccessRecord()=0;  //6.1 Firefox网络记录
  virtual  std::vector<st_aptPackgesInfo> getSoftwareCheckOfSystemInstallation_APT(bool & isExistAPT )=0;//10.系统的安装的软件检查
  virtual  std::vector<st_rpmPackagesInfo>  getSoftwareCheckOfSystemInstalltaion_RPM(bool & isExistRPM)=0; //10.1 系统的安装的软件检查

  virtual  std::vector<st_MountPoint>  getSystemMountPoint()=0; //10.2 当前系统挂载情况
 virtual  void getNetWorkHardWare(std::vector<st_Hardware_lshw>& get_Network)=0;// 获得硬件信息: 网卡、
 virtual  void getUSB_MassStorageDevice(std::vector<st_Hardware_lshw>& get_USB)=0;// 获得硬件信息:USB 存储设备 
 virtual  void getBluetooth(std::vector<st_Hardware_lshw>& vecGetBluetooth)=0;//获取蓝牙设备   IR、

 virtual  int  getUSB_EventHistory(std::vector<st_filtered>& vecGetUSB_EventHIstory)=0;//系统USB插入拔出时事件记录 :journalctl

 virtual  void checkInternetConnect(std::vector<st_checkConnection> & vec_st_checkConnection)=0;//检查操作系统是否能上网
virtual void test()=0;
};

typedef   GetSysInfoImpl*  create_t();
typedef void destory_t( GetSysInfoImpl* p);
