#ifndef PUBLICDATA_H_
#define PUBLICDATA_H_
#define COMPUTER_SYSTEM_INFOMATION 12
#define FILESYSTEM_CHECK 3
#define SYSTEMACCOUNTSECURITYCHECK  1   //SystemAccountSecurityCheck
#define  SYSTEMMODULESECURITYCHECK 2  //系统设备驱动安全检查
#define  INTERNETACCESSRECORD    6  //网络上网记录 Internet access record
#define  INSTALEEDPACKAGE  10   //10.  系统的安装的软件检查
#define  MOUNTPOINT 11   //挂载点
#define  NETWORKHARDWARE 13   //网卡信息
#define USBMASSSTORAGEDEVICE 14   //USB存储设备
#define BLUETOOTH 15   //蓝牙储设备
#define USBEVENTHISTORY 16  // usb插拔记录
#define CONNECTIONHOSTNAME 17//是否能联网
#include <iostream>
#include<vector>
#include<string>

using namespace std;
// struct hwNode_i
// {
   
//   string id, vendor, product, version, date, serial, slot, handle, description,
//     businfo, physid, dev, modalias, subvendor, subproduct;
//   bool enabled;
//   bool claimed;
//   unsigned long long start;
//   unsigned long long size;
//   unsigned long long capacity;
//   unsigned long long clock;
//   unsigned int width;
//   vector < hwNode > children;
//   vector < string > attracted;
//   vector < string > features;
//   vector < string > logicalnames;
//   map < string, string > features_descriptions;
//   vector < resource > resources;
//   map < string, string > config;
//   map < string, value > hints;
// };
 
enum type { MassStorage = 0, Phone,Other };




struct st_checkConnection
{
std::string destination;
int port;
std::string result;
};

struct st_filtered
  {
    int usbProductType;
  char type;
  std::string conn;
  std::string pid;
  std::string prod;
  std::string manufact;
  std::string serial;
  std::string port;
 std::string disconn;
std::string idVendor;
string idProduct;
int iOrder;
bool isCheck=false;
void clear()
{
  type=0;
conn="";
pid="";
prod="";
manufact="";
serial="";
port="";
disconn="";
port="";
disconn="";
idVendor="";
idProduct="";
isCheck=false;

}
  };

// struct st_USB_EventHIstory
// {

//  std::string Connected;
//  std::string User;
//  std::string VID;
//  std::string PID;
//  std::string Product;
//   std::string SerialNumbe; 
//   std::string BusPort;
//   std::string Disconnected;

// };

struct st_Hardware_lshw
{
std::string id;
std::string description;
std::string       product;
 std::string      vendor;
  std::string     physical_id;
   std::string    bus_info;
 std::string      logical_name;
   std::string    version;
  std::string     serial;
     std::string  width;
 std::string      clock;
     std::string  capabilities;
  std::string     configuration;
   std::string    resources;
std::string capacity;
std::string size;//硬盘大小

std::string  IPV4;
std::string MaskIP;
std::string  IPV6;
std::string dns;
};


/*
    description: ATA Disk
       product: VMware Virtual S
       physical id: 0.0.0
       bus info: scsi@0:0.0.0
       logical name: /dev/sda
       version: 0001
       serial: 00000000000000000001
       size: 20GiB (21GB)
       configuration: ansiversion=5 logicalsectorsize=512 sectorsize=512



*/
// struct st_lshwFileSystem
// {
// std::string id;
// std::string description;
// std::string       product;
//  std::string      vendor;
//   std::string     physical_id;
//    std::string    bus_info;
//  std::string      logical_name;
//    std::string    version;
//   std::string     serial;
//      std::string  width;
//  std::string      clock;
//      std::string  capabilities;
//   std::string     configuration;
//    std::string    resources;
// std::string capacity;
// };




struct st_HardWare
{
std::string capacity;//e.g:network
  std::string network_decimalkilos;
 std::string kilobytes;
  std::string Clock;
 unsigned int bit,with;
  std::string size;
  long long capicty;
   std::vector<std::string> logicalnames;
  //std::string logicalnames;
   std::vector < std::string > capabilities;
  //   std::vector <std::string > config;//takethat?
   std::string config;
  std::vector < std::string > resources;

  std::string id, vendor, product, version, date, serial, slot, handle, description,
    businfo, physid, dev, modalias, subvendor, subproduct;

std::string  IPV4;
std::string MaskIP;
std::string  IPV6;


};


struct st_sysinfo
{
  std::string OS;
  std::string Architecture;
  std::string CPU;
  std::string RAM;
  std::string LinuxkernelVersion;
  std::string Hostname;
  std::string SystemPath = "/sys";
};


 

struct st_fileSystem
{
 // std::string DriverName; // 卷标名
	std::string DevPath; // 卷标名
	std::string DriverInterFaceType;
  char SerialNumber[1000];
  char ManufacturerModel[100];
  char FirmwareRevision[100];
  char TransportType[1000];
  char MaximumRPM[1000];
  char Cylinders[1000];
  char Capacity[1000];
  std::string Capability;
 char SCSI_Vendor[1000];
char Product_id[1000];
char Product_ver[1000];
};



struct st_sysAccount
{
  std::string UserName;
  std::string LastPasswordChange;//最近一次修改时间
  std::string PasswdExpires;//密码过期时间
  std::string PasswordInactive;//密码失效时间
  std::string AccountExpires;//账户过期时间
  std::string Minimum_number_of_days_between_password_change;//两次改变密码之间相距的最小天数 
  std::string Maximum_number_of_days_between_password_change;//两次改变密码之间相距的最大天数
  std::string Number_of_days_of_warning_before_password_expires;//在密码过期之前警告的天数 
};
//数据结构： 2系统设备驱动安全检查 

struct st_SystemModuleSecurityCheck
{

std::string ModuleName;

std::string MemorySizeModule;//refers to the memory size of the module, in bytes.

int LoadedTimes;   // lists how many instances of the module are currently loaded. A value of zero represents an unloaded module.

std::string  DependentModule;// the module depends upon another module to be present in order to function, and lists those other modules.

std::string MoslueState;  //   Live, Loading, or Unloading are the only possible values.
std::string  OffsetModule;   // the current kernel memory offset for the loaded module. 

std::string FileName;//  frome /sbin/modinfo 
std::string License;  //  frome /sbin/modinfo 
std::string Description;//  frome /sbin/modinfo 
std::string Author;//  frome /sbin/modinfo 
std::string srcVersion; //  frome /sbin/modinfo
std::string Depends;//  frome /sbin/modinfo 
std::string Retpoline;//  frome /sbin/modinfo 
std::string Intree; //frome /sbin/modinfo;
std::string Vermagic;
std::string Sig_id;
std::string Signer;
std::string Sig_Key;
std::string Sig_HashAlgo;
std::string Signature;
};




enum BrowerType
{
  BT_Chrome,
  BT_Firefox,
  BT_360
};

// 6.网络上网记录
 struct st_BrowserHistory
 {
  st_BrowserHistory()
  {

  }
  int Type;
  std::string user_Name;
   std::vector<std::string> vec_Url;
   std::vector<std::string> vec_Title;
   std::vector<std::string> vec_DateTime;
   int countData;
 };


// 10. Software check of system installation
// struct st_SoftwareCheckOfSystemInstallation
// {
//   std::string Name;  //Package anme
//   std::string Version;
//   std::string Size;
//   std::string HomePage;
//   std::string Vendor;
//   std::string Description;

// };
 
/*
+ rpm -qi info <package>
+ yum list installed

 */


/*
+  Debian /Ubuntu via APT:
get all packages : apt list --installed
get packages detail:    apt show packageName  ||  apt-cache show 

 */
 struct st_aptPackgesInfo  
 {
	 int Type;
   std::string Replaces;
   std::string Breaks;
   std::string PackageName;
   std::string Architecture;
    std::string Version;
    std::string Priority;
    std::string Essential;
  std::string Section;    
  std::string Origin;
  std::string Maintainer;
  std::string Original_Maintaine;
  std::string Bugs;
  std::string Installed_Size;
  std::string Task;
  std::string Depends;
  std::string Filename;
  std::string Original_Maintainer;
  std::string Supported;
   std::string APT_Manual_Installed;
  std::string MD5sum;
  std::string SHA1;
  std::string SHA256;
  std::string Description;
  std::string APT_Sources;
  std::string Source;
  std::string Multi_Arch;
std::string Pre_Depends;
std::string Provides;
std::string Download_Size;
std::string Homepage;
std::string Suggests;
 
 };

 struct st_rpmPackagesInfo
{
	 std::string Name;
	std::string Version;
	std::string Architecture;
	std::string Install_Date;
	std::string Group;
	std::string Size;
	std::string License;
	std::string Signature;
	std::string Source_RPM;
	std::string Build_Date;
	std::string Build_Host;
	std::string Relocations;
	std::string Packager;
	std::string Vendor;
	std::string URL;
	std::string Summary;
	std::string Description;	
	std::string Release;
};

 
 struct st_MountPoint
 {
	 std::string FileSystem;
	 std::string Size;
	 std::string Used;
	 std::string Available;
	 std::string UseagePercentage;
	 std::string MountOn;

 };



#endif




