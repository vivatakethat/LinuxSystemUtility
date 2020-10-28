#include "GetSysInfo.h"
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
//#include <parted/parted.h>
#include <asm/byteorder.h>
#include <linux/fs.h>
#include <iostream>
#include <vector>
#include <string>

#include <memory>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <stddef.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>
#include <scsi/sg.h>

#include <scsi/scsi_ioctl.h>
#include <stdio.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <mntent.h>
#include <sys/vfs.h>

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#include <unistd.h>

 
 
#include <arpa/nameser.h>
#include <resolv.h>

#ifndef LINKDLL
#ifdef __cplusplus
#define LINKDLL extern "C"
#else
#define LINKDLL
#endif
#endif

//#include<sleep.h>
/*  Hard Disk Metadata    */
#define DUMPXML 1
#define DUMPTXT 2
#define DUMPCSV 3
#define NONEWLINE 0
#define DI_VERSION "1.0"
#define TRANSPORT_MAJOR 0xDE
#define TRANSPORT_MINOR 0xDF
#define ATA_PIDENTIFY 0xA1
#define ATA_IDENTIFY 0xEC
#define NMRR 0xD9
#define CAPAB 0x31
#define CMDS_SUPP_1 0x53
#define VALID 0xC000
#define VALID_VAL 0x4000
#define SUPPORT_48_BIT 0x0400
#define LBA_SUP 0x0200
#define LBA_LSB 0x64
#define LBA_MID 0x65
#define LBA_48_MSB 0x66
#define LBA_64_MSB 0x67

/*SCSI  */
#define RCAP_COMMAND 0x25
#define RCAP_COMMAND_LEN 10
#define RCAP_REPLY_LEN 8
//scsi  GET serices
#define INQ_REPLY_LEN 96
#define INQ_CMD_CODE 0x12
#define INQ_CMD_LEN 6
//scsi
#define SENSE_LEN 255 //SCSI
#define BLOCK_LEN 32
#define SCSI_TIMEOUT 20000

//Mount Point
static const unsigned long long G = 1024 * 1024 * 1024ull;
static const unsigned long long M = 1024 * 1024;
static const unsigned long long K = 1024;
static char str[20];
char *kscale(unsigned long b, unsigned long bs);

using namespace std;

void set_xfer_data(struct sg_io_hdr *p_hdr, void *data, unsigned int length);           //SCSI
void set_sense_data(struct sg_io_hdr *p_hdr, unsigned char *data, unsigned int length); //scsi
unsigned char sense_buffer[SENSE_LEN];
unsigned char data_buffer[BLOCK_LEN * 256]; //FOR SCSI
unsigned char *show_vendor(struct sg_io_hdr *hdr);
unsigned char *show_product(struct sg_io_hdr *hdr);
unsigned char *show_product_rev(struct sg_io_hdr *hdr);
int execute_Inquiry(int fd, int page_code, int evpd, struct sg_io_hdr *p_hdr);
struct sg_io_hdr *init_io_hdr();
void show_sense_buffer(struct sg_io_hdr *hdr);
void destroy_io_hdr(struct sg_io_hdr *p_hdr);

int hostname_to_ip(const char *hostname, char *ip); //解析hostname ip
string addStringToHead(const string str_source, const char *add);
unsigned long long getSCSI_DriverCapacity(string devicePath);
pid_t getProcessPidByName(const char *proc_name);
__u16 *get_diskinfo(int fd);

//void dump_partitions(char *device, int dumpmode, int nlmode);
/*
description: check wheather exist this file
return ; 1(exist) 0 (non )
 */
int exist(const char *fname);
void dump(const char *device);

char *ascii_string(__u16 *p, unsigned int len);

struct hd_geometry *get_geometry(int fd);
#define USE_CAPAB
//sqlite3 callback fundtion
int GetSysInfo::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for (i = 0; i < argc; i++)
  {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

char *get_capacity(int fd, __u16 id[])
{
  unsigned int sector_bytes = 512;
  static char str[20];
  __u64 sectors = 0;
#ifdef USE_CAPAB
  memset(&str, 0, sizeof(str));
  if (id[CAPAB] & LBA_SUP)
  {
    if (((id[CMDS_SUPP_1] & VALID) == VALID_VAL) && (id[CMDS_SUPP_1] & SUPPORT_48_BIT))
    {
      sectors = (__u64)id[LBA_64_MSB] << 48 | (__u64)id[LBA_48_MSB] << 32 |
                (__u64)id[LBA_MID] << 16 | id[LBA_LSB];
    }
  }
#else
  unsigned int sector32 = 0;
  if (!(ioctl(fd, BLKGETSIZE64, &sectors)))
  { // bytes
    sectors /= sector_bytes;
  }
  else if (!(ioctl(fd, BLKGETSIZE, &sector32)))
  { // sectors
    sectors = sector32;
  }
  else
    return "";
#endif
  sectors *= (sector_bytes / 512);
  sectors = (sectors << 9) / 1000000;
  if (sectors > 1000)
    sprintf(str, "%lluGB", (unsigned long long)sectors / 1000);
  else
    sprintf(str, "%lluMB", (unsigned long long)sectors);
  return str;
}

__u16 *get_diskinfo(int fd)
{
  static __u8 args[4 + 512];
  // __u16 *id = (void *)(args + 4);
  __u16 *id = reinterpret_cast<__u16 *>(args + 4);
  // __u16 *id = (args + 4);
  //  __u16 *id = (__u16 *)(args + 4);
  int i;
  memset(args, 0, sizeof(args));
  args[0] = ATA_IDENTIFY;
  args[3] = 1;
  if (ioctl(fd, HDIO_DRIVE_CMD, args))
  {
    args[0] = ATA_PIDENTIFY;
    args[1] = 0;
    args[2] = 0;
    args[3] = 1;
    if (ioctl(fd, HDIO_DRIVE_CMD, args))
    {
      perror("ERROR: HDIO_DRIVE_CMD failed ");
      return 0;
    }
  }
  /* byte-swap data to match host endianness */
  for (i = 0; i < 0x100; ++i)
    __le16_to_cpus(&id[i]);
  return id;
}

char *get_rpm(__u16 id[])
{
  static char str[6];
  __u16 i = id[NMRR];
  sprintf(str, "%u", i);
  return str;
}
/* yes - these are shortcuts! */
//static __u16 *id = (void *)NULL;

//static __u16 *id = 0;
__u16 *id = 0;

//static struct hd_geometry *g;
static struct hd_geometry *g;

//static int fd = 0;
int fd = 0;

//
// Routine currently only handles SATA drives.  Extra code needs to be added to support PATA, SCSI, USB, etc.
//

char *get_transport(__u16 id[])
{
  __u16 major, minor;
  unsigned int ttype, stype;
  major = id[TRANSPORT_MAJOR];
  minor = id[TRANSPORT_MINOR];
  if (major == 0x0000 || major == 0xffff)
    return "";
  ttype = major >> 12;   /* transport type */
  stype = major & 0xfff; /* subtype */
  if (ttype == 1)
  {
    if (stype & 0x2f)
    {
      if (stype & (1 << 5))
        return "SATA Rev 3.0";
      else if (stype & (1 << 4))
        return "SATA Rev 2.6";
      else if (stype & (1 << 3))
        return "SATA Rev 2.5";
      else if (stype & (1 << 2))
        return "SATA II Extensions";
      else if (stype & (1 << 1))
        return "SATA 1.0a";
    }
  }
}

char *ascii_string(__u16 *p,
                   unsigned int len)
{
  if (p == NULL)
  {
    fprintf(stderr, "ERROR: ascii_string ptr is null\n");
    return NULL;
  }
  __u8 i, c;
  char cl;
  static char str[60];
  char *s = str;
  memset(&str, 0, sizeof(str));
  /* find first character */
  for (i = 0; i < len; i++)
  {
    if (((char)0x00ff & ((*p) >> 8)) != ' ')
      break;
    if ((cl = (char)0x00ff & (*p)) != ' ')
    {
      if (cl != '\0')
        *s++ = cl;
      p++;
      i++;
      break;
    }
    p++;
  }
  /* copy from here to end */
  for (; i < len; i++)
  {
    c = (*p) >> 8;
    if (c)
      *s++ = c;
    c = (*p);
    if (c)
      *s++ = c;
    p++;
  }
  /* remove trailing blanks */
  s = str;
  while (*s)
    s++;
  while (*--s == ' ')
    *s = 0;
  return str;
}

st_sysinfo GetSysInfo::getOperationSystemInfo()
{
  st_sysinfo sys;
  FILE *out;
  char buffer[1024];
  memset(&buffer, 0x00, sizeof(buffer));
#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)

#elif defined(__linux__)
  //  sys.os = "{\Linux,\";
  out = popen("hostnamectl | grep 'Operating System:' | sed 's/[[:blank:]]*Operating System:[[:blank:]]*//' | awk 1 ORS=' ' ", "r");
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    sys.OS = buffer;
    //   sys.os.append("}");
    //  	printf("[debug]%d  [sys.os]: %s\n",__LINE__,buffer);
    memset(&buffer, 0x00, sizeof(buffer));
  }
  out = popen("lscpu | grep Architecture |  sed 's/Architecture:[[:blank:]]*//' | awk 1 ORS=' '", "r");
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    sys.Architecture = buffer;
    memset(&buffer, 0x00, sizeof(buffer));
  }
  out = popen("cat /proc/cpuinfo | grep 'model name' | uniq |"
              " sed 's/model name[[:blank:]]*:[[:blank:]]*//' | awk 1 ORS=' '",
              "r");
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    sys.CPU = buffer;
    memset(&buffer, 0x00, sizeof(buffer));
  }
  out = popen("cat /proc/meminfo | grep 'MemTotal' | sed 's/MemTotal:[[:blank:]]*//'"
              " | awk '{$1=$1/1024^2; print $1,\"GB\";}' | awk 1 ORS=' '",
              "r");
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    sys.RAM = buffer;
    memset(&buffer, 0x00, sizeof(buffer));
  }
  out = popen("uname -r | awk 1 ORS=' '", "r");
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    sys.LinuxkernelVersion = buffer;
  }
  memset(&buffer, 0x00, sizeof(buffer));
  //hostname
  out = popen("hostname |awk 1 ORS=' '", "r");
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    sys.Hostname = buffer;
  }
  pclose(out);

#endif
  return sys;
}

void GetSysInfo::test() //debug
{

  cout << "debug; void  GetSysInfo::test()" << endl;
}

void GetSysInfo::getFileSystemInfo(vector<st_Hardware_lshw> &get_disk)
{

  char buff_ProcessPath[1024];
  getcwd(buff_ProcessPath, 1024);

#ifndef NONLS
  printf("%d\n", __LINE__);
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8");
  textdomain(PACKAGE);
#endif

  disable("isapnp");

  disable("output:json");
  disable("output:db");
  disable("output:xml");
  disable("output:html");
  disable("output:hwpath");
  disable("output:businfo");
  disable("output:X");
  disable("output:quiet");
  disable("output:sanitize");
  disable("output:numeric");
  enable("output:time");

  // define some aliases for nodes classes
  alias("disc", "disk");
  alias("cpu", "processor");
  alias("lan", "network");
  alias("net", "network");
  alias("video", "display");
  alias("sound", "multimedia");
  alias("modem", "communication");

  printf("proccer_path:");
  printf(buff_ProcessPath);
  std::vector<st_HardWare> rt_VecBluetoothardWare;

  cout << "[line]=" << __LINE__ << "[fn]="
       << "getFileSystemInfo" << endl;
  hwNode computer("computer",
                  hw::generic);

  scan_system(computer);

  int argc = 3;
  char *argv_all[] = {"process", "-c", "disk"};

  if (!parse_options(argc, argv_all))
  {
    printf("error\n");
    exit(1);
  }
  rt_VecBluetoothardWare = print(computer, enabled("output:html"));
  cout << "[size]=" << rt_VecBluetoothardWare.size();

  for (std::vector<st_HardWare>::iterator it = rt_VecBluetoothardWare.begin(); it != rt_VecBluetoothardWare.end(); ++it)
  {

    //   cout<<__LINE__<<"it->id="<<it->id<<endl;

    if ((it->id.find("disk") != string::npos) || (it->id.find("cdrom") != string::npos))
    {

      st_Hardware_lshw t_lshwFilesystem;
      // t_lshwFilesystem.capacity=it->capacity;
      t_lshwFilesystem.description = it->description;
      t_lshwFilesystem.product = it->product;
      t_lshwFilesystem.vendor = it->vendor;
      t_lshwFilesystem.physical_id = it->physid;
      t_lshwFilesystem.bus_info = it->businfo;
      t_lshwFilesystem.size = it->size;
      t_lshwFilesystem.serial = it->serial;
      //   cout<<"description:"<<it->description<<endl;
      //   cout<<"product:"<<it->product<<endl;
      //  cout<<"vendor:"<<it->vendor<<endl;
      //  cout<<"physid:"<<it->physid<<endl;
      //  cout<<"businfo:"<<it->businfo<<endl;

      string t_logicalname;
      //       // cout<<"logicalnames:";
      for (int i = 0; i < it->logicalnames.size(); i++)
      {
        // cout<<it->logicalnames[i]<<" ";
        t_logicalname.append(it->logicalnames[i]);
      }
      t_lshwFilesystem.logical_name = t_logicalname;
      t_lshwFilesystem.version = it->version;
      t_lshwFilesystem.serial = it->serial;

      //  cout<<"\n"<<"version:";
      //    cout<<it->version<<endl;
      //    cout<<"serial:";
      //    cout<<it->serial<<endl;

      //t_lshwFilesystem.width=std::to_string( it->with);
      t_lshwFilesystem.width.append(" bits");
      //cout<<"with:"<<it->with<<endl;

      // t_lshwFilesystem.clock=it->Clock;
      //  cout<<"clock:"<<it->Clock <<endl;

      string t_capabilities;
      //cout<<"capabilities:";
      for (int i = 0; i < it->capabilities.size(); i++)
      {
        //   cout<<it->capabilities[i]<<" ";
        t_capabilities.append(it->capabilities[i]);
      }
      //  cout<<endl;
      t_lshwFilesystem.capabilities = t_capabilities;

      string t_config;
      //  cout<<"configuration:";
      //  for(int i=0;i<it->config.size();i++)
      //  {
      //cout<<it->config[i]<<" ";
      // t_config.append(it->config[i]);
      t_config = it->config;
      //   }
      //  cout<<endl;
      t_lshwFilesystem.configuration = t_config;

      // string t_resources;
      //   //   cout<<"resources:";
      //      for(int i=0;i<it->resources.size();i++)
      //      {
      //              //  cout<< it->resources[i]<<" ";
      //              t_resources.append(it->resources[i]);
      //      }
      // //  cout<<endl;
      //               t_lshwFilesystem.resources=t_resources;
      t_lshwFilesystem.id = it->id;

      get_disk.push_back(t_lshwFilesystem);
    }
    else
      continue;
  }
     remove_options("disk");
  desotry(); //清楚静态对象

  char buff_ProcessPathCheck[1024];
  getcwd(buff_ProcessPathCheck, 1024);
  int ret_Chdir;
  if (strcmp(buff_ProcessPath, buff_ProcessPathCheck) != 0)
    ret_Chdir = chdir(buff_ProcessPath);

  if (ret_Chdir == -1)
  {
    fprintf(stderr, "Failed to open %s\n"
                    "The reason *may* have been %s\n",
            buff_ProcessPath, strerror(errno));
  }
}

const char *GetSysInfo::io_StrDevices(const char *ptr_DeviceName)
{
  return NULL;
}

vector<st_fileSystem> GetSysInfo::getFileSystemInfo_InC()
{

  int IsExistudevadm = exist("/usr/bin/udevadm");
  if (!IsExistudevadm)
  {
    IsExistudevadm = exist("/sbin/udevadm");
  }

  //  /bin/ls /sys/block/* | grep block | grep sd | wc -l | awk 1 ORS=''

  if (geteuid() != 0)
  {
    fprintf(stderr, "To Get FileSystem Infomation,  must run as root!\n");
    exit(1);
  }
  int n_Count = 0;
  // st_fileSystem t_filesystem = {
  //     " ",
  //     0 ,
  //     " ",
  //     " ",
  // };
  vector<st_fileSystem> rt_Vector;
  FILE *out = NULL;
  char buffer[1024];
  memset(&buffer, 0x00, sizeof(buffer));
  //  out=popen("fdisk -l | grep 'Disk model' | wc -l | sed 's/[[:blank:]]*//' | awk 1 ORS=' ' ","r");
  //	out = popen("exec bash -c \"/usr/bin/ls /sys/block/sd* | grep block | grep sd | wc -l | awk 1 ORS=''\"", "r");
  //out = popen("exec bash -c \"ls /sys/block/sd* | grep block | grep sd | wc -l | awk 1 ORS=''\"", "r");

  out = popen("exec bash -c \"ls /sys/block/ | grep sd.* | wc -l | awk 1 ORS=''\"", "r");

  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    n_Count = atoi(buffer); //Count of HardDriver Disk

    if (n_Count == 0)
    {
      pclose(out);
      out = NULL;
      fprintf(stderr, "Cannot any HardDisk Driver(eg. GPT, MBR, Sun, SGI and BSD partition tables)!\n");
      return rt_Vector;
    }
    pclose(out);
    out = NULL;
    memset(&buffer, 0x00, sizeof(buffer));
  }
  // vector<st_fileSystem> tVec_filesystem(n_Count, t_filesystem);
  vector<st_fileSystem> tVec_filesystem(n_Count);

  vector<string> tVec_BlockPath(n_Count);

  char t_Command[1000] = {0};
  // printf("debug! [n_Count]%d ,[tVec_filesystem.size]%d\n ",n_Count,tVec_filesystem.size());
  for (int i_Line = 1, i = 0; i_Line < n_Count + 1; i_Line++, i++)
  {
    // "fdisk -l | grep 'Disk /' | sed 's/Disk[[:blank:]]*//' | sed 's/\b:.*//' | sed -n '1p' | awk 1 ORS=''
    // sprintf(t_Command,"exec bash -c \"/usr/bin/ls /sys/block/* | grep sd | sed -n %dp  | awk '{sub(/.$/,\"\")}1' | awk 1 ORS=''\"",i_Line);
    //sprintf(t_Command, "exec bash -c \"/usr/bin/ls /sys/block/ | grep sd | sed -n %dp | awk 1 ORS=''\"", i_Line);  //takethat:这个在ubuntu14路径不对
    //cout << "t_Command=" << t_Command << endl;
    sprintf(t_Command, "exec bash -c \"ls /sys/block/ | grep sd | sed -n %dp | awk 1 ORS=''\"", i_Line);
    int a;
    string sdName = GetStdoutFromCommand(t_Command, a);
    if (sdName.empty())
    {
      printf("%d  peopen call fail, error\n", __LINE__);
      continue;
    }
    else
    {
      //  tVec_BlockPath[i] = sdName;
      string take_BlockDevicePath = addStringToHead(sdName, "/sys/block/");
      // tVec_BlockPath.push_back(take_BlockDevicePath);
      tVec_BlockPath[i] = take_BlockDevicePath;

      char *t_Rsutl;
      //	string sdName = splite_Item(blockName, "/sys/block/", "end", &t_Rsutl);

      string take_DevicePath = addStringToHead(sdName, "/dev/");
      tVec_filesystem[i].DevPath = take_DevicePath; //获取设备名称  /dev/sd*

      if (IsExistudevadm) /*   /usr/bin/udevadm info --path=/sys/block/sda  */
      {
        int b;
        char c_getInterfaceType[100] = {0};

        /*ex:  /usr/bin/udevadm info --path=/sys/block/sda | grep "E: ID_BUS=" | sed 's/\b.*=//'    */
        sprintf(c_getInterfaceType, "exec bash -c \"udevadm info --path=%s | grep \\\"E: ID_BUS=\\\" | sed 's/\\b.*=//'| awk 1 ORS=''\"", take_BlockDevicePath.c_str()); //vivatakethat:想想为何要这样  '\\\而不是\'

        cout << "c_getInterfaceType =" << c_getInterfaceType << endl;
        string st_InterFaceType = GetStdoutFromCommand(c_getInterfaceType, b);

        tVec_filesystem[i].DriverInterFaceType = st_InterFaceType;
      }

    } //-< else
  }

  //- <-----获取设备名以及 udevadm info scsi/ata类型完毕

  for (int i = 0; i < tVec_filesystem.size(); i++)
  {

    if (tVec_filesystem[i].DriverInterFaceType.compare(0, tVec_filesystem[i].DriverInterFaceType.size(), "ata") == 0)
    {
      printf("--->ata--->\n");
      int n_Count = 0;

      if ((fd = open(tVec_filesystem[i].DevPath.c_str(), O_RDONLY | O_NONBLOCK)) < 0)
      {
        // if ((fd = open("/dev/sda", O_RDONLY|O_NONBLOCK)) < 0) {
        //  fprintf(stderr, "ERROR: Cannot open device %s\n", argv[1]);
        printf("ERROR: Cannot open device %s\n", tVec_filesystem[i].DevPath.c_str());
        continue; //vivatakethat:open函数大概移动硬盘会失败，那应该怎么做?
      }
      //   printf("debug:line:[%d]",__LINE__);
      id = get_diskinfo(fd);
      if (id == 0)
      {
        cout << tVec_filesystem[i].DevPath.c_str() << " get_diskinfo wrong,continue this driver..." << endl;
        continue;
      }
      g = get_geometry(fd);
      if (g == NULL)
      {
        fprintf(stderr, "g is NULL!\n");
        continue;
      }
      sprintf(tVec_filesystem[i].ManufacturerModel, "%s", ascii_string(&id[27], 20));
      sprintf(tVec_filesystem[i].SerialNumber, "%s", ascii_string(&id[10], 10));
      sprintf(tVec_filesystem[i].FirmwareRevision, "%s", ascii_string(&id[23], 4));
      sprintf(tVec_filesystem[i].TransportType, "%s", get_rpm(id));
      sprintf(tVec_filesystem[i].MaximumRPM, "%s", get_transport(id));
      sprintf(tVec_filesystem[i].Capacity, "%s", get_capacity(fd, id));
      sprintf(tVec_filesystem[i].Cylinders, "%u", g->cylinders);

      fd = 0;
    }    // type: ata  -<
    else //-> scsi
    {
      printf("--->scsi--->\n");
      /* 获得SCSI硬盘容量*/
      if (tVec_filesystem[i].DevPath.empty())
      {
        continue;
      }
      unsigned long long n_Capacity = getSCSI_DriverCapacity(tVec_filesystem[i].DevPath);
      sprintf(tVec_filesystem[i].Capacity, "%ldGB", n_Capacity / 1024 / 1024 / 1024);

      /* 获得SCSI硬盘序列号  */

      // ex.     udevadm info --path=/sys/block/sdc | grep "E: ID_SERIAL_SHORT" | sed 's/\b.*=//'
      // demo:  sprintf(c_getInterfaceType, "exec bash -c \"/usr/bin/udevadm info --path=%s | grep \\\"E: ID_BUS=\\\" | sed 's/\\b.*=//'| awk 1 ORS=''\"", blockName.c_str());
      sprintf(t_Command, "exec bash -c \"/usr/bin/udevadm info --path=%s | grep \\\"E: ID_SERIAL_SHORT\\\" | sed 's/\\b.*=//'| awk 1 ORS=''\"", tVec_BlockPath[i].c_str());
      int c;
      string st_SerialNumber = GetStdoutFromCommand(t_Command, c);

      sprintf(tVec_filesystem[i].SerialNumber, "%s", st_SerialNumber.c_str());

      /*SCSI 发指令查询  --> */
      struct sg_io_hdr *p_hdr = init_io_hdr();

      set_xfer_data(p_hdr, data_buffer, BLOCK_LEN * 256);
      set_sense_data(p_hdr, sense_buffer, SENSE_LEN);

      int status = 0;

      int fd = open(tVec_filesystem[i].DevPath.c_str(), O_RDWR);
      if (fd > 0)
      {
        status = execute_Inquiry(fd, 0, 0, p_hdr);
        printf("the return status is %d\n", status);
        if (status != 0)
        {
          show_sense_buffer(p_hdr);
        }
        else
        {
          //	sprintf(tVec_filesystem[i].SCSI_Vendor,"%s",(const char *)show_vendor(p_hdr));

          char *us_vendor = reinterpret_cast<char *>(show_vendor(p_hdr)); //takethat:这个有时候获取的是0x000;
                                                                          // strcpy(tVec_filesystem[i].SCSI_Vendor,us_vendor);

          char *us_product = reinterpret_cast<char *>(show_product(p_hdr));
          //	sprintf(tVec_filesystem[i].Product_id,"%s",	(const char *)show_product(p_hdr));
          // strcpy(tVec_filesystem[i].Product_id,us_product);//takethat:这个有时候获取的是0x000;

          //			sprintf(tVec_filesystem[i].Product_ver,"%s",(const char *)show_product_rev(p_hdr));
          char *us_product_rev = reinterpret_cast<char *>(show_product_rev(p_hdr));
          //strcpy(tVec_filesystem[i].Product_ver,us_product_rev);//takethat:这个有时候获取的是0x000;
        }
      }
      else
      {
        printf("failed to open sg file %s\n", tVec_filesystem[i].DevPath.c_str());
      }
      close(fd);
      destroy_io_hdr(p_hdr);

      /*SCSI 发指令查询-< */
    }

  } //			-<  to `for`

  return tVec_filesystem;
}

/*void dump_partitions(const char *device, int dumpmode, int nlmode)
{
  PedDevice *dev = (PedDevice *)NULL;
  PedDiskType *type;
  PedDisk *disk = (PedDisk *)NULL;
  PedPartition *part;
  PedPartitionFlag flag;
  PedUnit default_unit;
  int has_free_arg = 0;
  char *start;
  char *end;
  char *size;
  char flags[100];
  const char *partname;
  const char *parttype;
  const char *partlabel;
  const char *partflags;
  int first_flag;
  dev = ped_device_get(device);
  if (!ped_device_open(dev))
  {
    fprintf(stderr, "ERROR: ped-device-opem\n");
    exit(1);
  }
  disk = ped_disk_new(dev);
  if (!disk)
  {
    fprintf(stderr, "ERROR: ped-disk-new\n");
    exit(1);
  }
  start = ped_unit_format(dev, 0);
  default_unit = ped_unit_get_default();
  end = ped_unit_format_byte(dev, dev->length * dev->sector_size - (default_unit == PED_UNIT_CHS || default_unit == PED_UNIT_CYLINDER));
  switch (dumpmode)
  {
  case DUMPXML:
    if (nlmode)
      printf("\n    ");
    printf("<partitiontype>%s<paritiontype>", disk->type->name);
    if (nlmode)
      printf("\n    ");
    printf("<partitions>");
    break;
  case DUMPTXT:
    printf("    Partition Type: %s\n", disk->type->name);
    printf("    No.  Start   End     Size      Type      Filesystem   Name  Flags\n");
    break;
  case DUMPCSV:
    putchar('"');
    putchar(',');
    putchar('"');
    printf("%s", disk->type->name);
    break;
  }
  free(start);
  free(end);
  for (part = ped_disk_next_partition(disk, NULL); part;
       part = ped_disk_next_partition(disk, part))
  {
    if ((!has_free_arg && !ped_partition_is_active(part)) ||
        part->type & PED_PARTITION_METADATA)
      continue;
    start = ped_unit_format(dev, part->geom.start);
    end = ped_unit_format_byte(dev, (part->geom.end + 1) * (dev)->sector_size - 1);
    size = ped_unit_format(dev, part->geom.length);
    if (!(part->type & PED_PARTITION_FREESPACE))
    {
      parttype = ped_partition_type_get_name(part->type);
      partlabel = ped_partition_get_name(part);
    }
    else
    {
      parttype = "";
      partlabel = "";
    }
    // flags
    memset(&flags, 0, sizeof(flags));
    first_flag = 1;
    //    for (flag = ped_partition_flag_next(0); flag;
    for (flag = ped_partition_flag_next(static_cast<PedPartitionFlag>(0)); flag;
         flag = ped_partition_flag_next(flag))
    {
      if (ped_partition_get_flag(part, flag))
      {
        if (first_flag)
        {
          first_flag = 0;
        }
        else
        {
          strcat(flags, ", ");
        }
        partflags = ped_partition_flag_get_name(flag);
        strcat(flags, partflags);
      }
    }
    switch (dumpmode)
    {
    case DUMPXML:
      if (nlmode)
        printf("\n        ");
      if (part->num >= 0)
        printf("<partition number=\"%d\">", part->num);
      else
        printf("<partition number=\"0\">");
      if (nlmode)
        printf("\n            ");
      printf("<start>%s</start>", start);
      if (nlmode)
        printf("\n            ");
      printf("<end>%s</end>", end);
      if (nlmode)
        printf("\n            ");
      printf("<size>%s</size>", size);
      if (nlmode)
        printf("\n            ");
      printf("<type>%s</type>", parttype);
      if (nlmode)
        printf("\n            ");
      printf("<filesystem>%s</filesystem>", part->fs_type ? part->fs_type->name : "");
      if (nlmode)
        printf("\n            ");
      printf("<label>%s</label>", partlabel);
      if (nlmode)
        printf("\n            ");
      printf("<flags>%s</flags>", flags);
      if (nlmode)
        printf("\n        ");
      printf("</partition>");
      break;
    case DUMPTXT:
      if (part->num >= 0)
        printf("    %02d", part->num);
      else
        printf("        ");
      printf("  %6s  %6s  %6s  %10s", start, end, size, parttype);
      printf("  %6s", part->fs_type ? part->fs_type->name : "");
      printf("  %10s  %s\n", partlabel, flags);
      break;
    case DUMPCSV:
      putchar('"');
      putchar(',');
      putchar('"');
      if (part->num >= 0)
        printf("%02d", part->num);
      putchar('"');
      putchar(',');
      putchar('"');
      printf("%s", start);
      putchar('"');
      putchar(',');
      putchar('"');
      printf("%s", end);
      putchar('"');
      putchar(',');
      putchar('"');
      printf("%s", size);
      putchar('"');
      putchar(',');
      putchar('"');
      printf("%s", parttype);
      putchar('"');
      putchar(',');
      putchar('"');
      if (part->fs_type)
        printf("%s", part->fs_type->name);
      putchar('"');
      putchar(',');
      putchar('"');
      printf("%s", partlabel);
      putchar('"');
      putchar(',');
      putchar('"');
      printf("%s", flags);
      break;
    }
    free(start);
    free(end);
    free(size);
  }
  switch (dumpmode)
  {
  case DUMPXML:
    if (nlmode)
      printf("\n    ");
    printf("</partitions>");
    break;
  case DUMPTXT:
    break;
  case DUMPCSV:
    putchar('"');
    break;
  }
}
*/
void dump(const char *device)
{
  int len = strlen(device) + 8;
  int i = 0;
  printf("\nDEVICE: %s\n", device);
  while (i++ < len)
    putchar('-');
  putchar('\n');
  printf("Manufacturer Model: %s\n", ascii_string(&id[27], 20));
  printf("     Serial Number: %s\n", ascii_string(&id[10], 10));
  printf(" Firmware Revision: %s\n", ascii_string(&id[23], 4));
  printf("    Transport Type: %s\n", get_transport(id));
  printf("       Maximum RPM: %s\n", get_rpm(id));
  printf("          Capacity: %s\n", get_capacity(fd, id));
  printf("  Number Cylinders: %u\n", g->cylinders);
}

struct hd_geometry *get_geometry(int fd)
{
  static struct hd_geometry geometry;
  if (ioctl(fd, HDIO_GETGEO, &geometry))
  {
    perror("ERROR: HDIO_GETGEO failed");
  }
  return &geometry;
}

vector<st_sysAccount> GetSysInfo::getSysAccountSecurityCheck()
{
 // cout << __LINE__ << endl;                                                         //debug
  //cout << "vector<st_sysAccount> GetSysInfo::getSysAccountSecurityCheck()" << endl; //debug
  FILE *out = NULL;
  char buffer[1024];
  memset(&buffer, 0x00, sizeof(buffer));
  int n_Size = 0;
  out = popen("exec bash -c \"eval getent passwd {$(awk '/^UID_MIN/ {print $2}' /etc/login.defs)..$(awk '/^UID_MAX/ {print $2}' /etc/login.defs)} | wc -l | awk 1 ORS=''\"", "r"); //get count of normal user  //need to test
  fgets(buffer, sizeof(buffer), out);

  string rt_str;

  string t_str;
  char *rt_char = NULL;

  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    //cout<<"buffer:"<<buffer<<endl;
    n_Size = atoi(buffer);
    //printf("debug, line=%d  , n_Size=%d\n",__LINE__,n_Size);
    memset(&buffer, 0x00, sizeof(buffer));
    pclose(out);
    out = NULL;
  }
  out = popen("getent passwd 0 | wc -l | awk 1 ORS=' '", "r"); //get count of normal user
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    //cout<<"buffer:"<<buffer<<endl;
    n_Size += atoi(buffer);
    //printf("debug, line=%d  , n_Size=%d\n",__LINE__,n_Size);
    memset(&buffer, 0x00, sizeof(buffer));
    pclose(out);
    out = NULL;
  }
  //printf("debug: n_Size=%d\n",n_Size);
  vector<st_sysAccount> rt_Vec(n_Size); //Consructor n_Size Object;
  //  out=popen("fdisk -l | grep 'Disk model' | wc -l | sed 's/[[:blank:]]*//' | awk 1 ORS=' ' ","r");
  // out = popen("chage -l $USER   | awk 1 ORS=' '", "r");
  char t_Command[1000];
  for (int i = 1; i < n_Size; i++)
  {
    //  printf("i=%d, n_Size + 1=%d\n",i,n_Size);
    sprintf(t_Command, "exec bash -c  \"eval getent passwd {$(awk '/^UID_MIN/ {print $2}' /etc/login.defs)..$(awk '/^UID_MAX/ {print $2}' /etc/login.defs)} | cut -d: -f1 | sed -n %dp | awk 1 ORS=''\"", i); //need to test
    //cout<<"debug: t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r"); //Get User Name with pipe
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
      continue;
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      //   cout<<"debug:buffer="<<buffer<<endl;
      // usleep(100);
      rt_Vec[i].UserName = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      memset(&t_Command, 0x00, sizeof(t_Command));
      pclose(out);
      out = NULL;
    }

    // Get: Last password change
    //sprintf(t_Command, "chage -l %s | sed -n 1p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str());
    sprintf(t_Command, "chage -l %s | sed -n 1p | awk 1 ORS=' '", rt_Vec[i].UserName.c_str());
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);

      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);//中文处理
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }

      rt_Vec[i].LastPasswordChange = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }
    //Get Password expires
    sprintf(t_Command, "chage -l %s | sed -n 2p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str());
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);

      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }
      // cout<<"buffer:"<<buffer<<endl;
      rt_Vec[i].PasswdExpires = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }

    // Get Password inactive
    sprintf(t_Command, "chage -l %s | sed -n 3p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str(), i);
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);

      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }
      // cout<<"buffer:"<<buffer<<endl;
      rt_Vec[i].PasswordInactive = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }
    // Get Account expires
    sprintf(t_Command, "chage -l %s | sed -n 4p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str());
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }

      // cout<<"buffer:"<<buffer<<endl;
      rt_Vec[i].AccountExpires = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }
    //Get: Minimum number of days between password change
    sprintf(t_Command, "chage -l %s | sed -n 5p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str(), i);
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);

      // cout<<"buffer:"<<buffer<<endl;
      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }

      rt_Vec[i].Minimum_number_of_days_between_password_change = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }
    //Get: Maximum number of days between password change
    sprintf(t_Command, "chage -l %s | sed -n 6p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str(), i);
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);

      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }

      // cout<<"buffer:"<<buffer<<endl;
      rt_Vec[i].Maximum_number_of_days_between_password_change = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }
    //Get : Number of days of warning before password expires
    sprintf(t_Command, "chage -l %s | sed -n 7p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[i].UserName.c_str(), i);
    //   cout<<"debug: t_Command="<<t_Command<<endl;
    //   printf("debug: line=%d\n",__LINE__);
    out = popen(t_Command, "r"); //Get Last password change
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      t_str = buffer;
      rt_char = NULL;
      rt_str = splite_Item(t_str, ":", "end", &rt_char);
      if (rt_str.empty())
      {
        rt_str = splite_Item(t_str, "：", "end", &rt_char);
        //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
      }

      // cout<<"buffer:"<<buffer<<endl;
      rt_Vec[i].Number_of_days_of_warning_before_password_expires = rt_str;
      //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[i].LastPasswordChange<<endl;
      //    getchar();
      memset(&buffer, 0x00, sizeof(buffer));
      // pclose(out);
      pclose(out);
      out = NULL;
    }
  } //--<  end of for

  //root专门处理
  //Get whether has root account
  out = popen("grep -i root /etc/passwd | awk 1 ORS=' '", "r");
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    if (buffer == "\n")
    {
      printf("Current OS has no 'root' account\n");
    }
    else
    {
      rt_Vec[0].UserName = "root";
    }
  }
  // Get: Last password change
  sprintf(t_Command, "chage -l %s | sed -n 1p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  //   cout<<"debug: t_Command="<<t_Command<<endl;
  //   printf("debug: line=%d\n",__LINE__);
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }

    // cout<<"buffer:"<<buffer<<endl;
    // rt_Vec[i].Number_of_days_of_warning_before_password_expires = rt_str;

    // cout<<"buffer:"<<buffer<<endl;
    rt_Vec[0].LastPasswordChange = rt_str;
    //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[0].LastPasswordChange<<endl;
    //    getchar();
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }
  //Get root Password expires
  sprintf(t_Command, "chage -l %s | sed -n 2p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  //   cout<<"debug: t_Command="<<t_Command<<endl;
  //   printf("debug: line=%d\n",__LINE__);
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }

    // cout<<"buffer:"<<buffer<<endl;
    rt_Vec[0].PasswdExpires = rt_str;
    //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[0].LastPasswordChange<<endl;
    //    getchar();
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }
  // Get Password inactive
  sprintf(t_Command, "chage -l %s | sed -n 3p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  //   cout<<"debug: t_Command="<<t_Command<<endl;
  //   printf("debug: line=%d\n",__LINE__);
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }
    // cout<<"buffer:"<<buffer<<endl;
    rt_Vec[0].PasswordInactive = rt_str;
    //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[0].LastPasswordChange<<endl;
    //    getchar();
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }
  // Get Account expires
  sprintf(t_Command, "chage -l %s | sed -n 4p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  //   cout<<"debug: t_Command="<<t_Command<<endl;
  //   printf("debug: line=%d\n",__LINE__);
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    // cout<<"buffer:"<<buffer<<endl;

    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }

    rt_Vec[0].AccountExpires = rt_str;
    //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[0].LastPasswordChange<<endl;
    //    getchar();
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }
  //Get root: Minimum number of days between password change
  sprintf(t_Command, "chage -l %s | sed -n 5p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  //   cout<<"debug: t_Command="<<t_Command<<endl;
  //   printf("debug: line=%d\n",__LINE__);
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }
    // cout<<"buffer:"<<buffer<<endl;
    rt_Vec[0].Minimum_number_of_days_between_password_change = rt_str;
    //   cout<<"debug: "<<" rt_Vec["<<i<<"].LastPasswordChange ="<< rt_Vec[0].LastPasswordChange<<endl;
    //    getchar();
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }
  sprintf(t_Command, "chage -l %s | sed -n 6p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    // cout<<"buffer:"<<buffer<<endl;
    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }

    rt_Vec[0].Maximum_number_of_days_between_password_change = rt_str;
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }
  //Get root : Number of days of warning before password expires
  sprintf(t_Command, "chage -l %s | sed -n 7p | sed 's/.*://' | awk 1 ORS=' '", rt_Vec[0].UserName.c_str());
  out = popen(t_Command, "r"); //Get root Last password change
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);

    t_str = buffer;
    rt_char = NULL;
    rt_str = splite_Item(t_str, ":", "end", &rt_char);
    if (rt_str.empty())
    {
      rt_str = splite_Item(t_str, "：", "end", &rt_char);
      //cout<<__LINE__<<" rt_char="<<rt_str<<endl;
    }
    // cout<<"buffer:"<<buffer<<endl;
    rt_Vec[0].Number_of_days_of_warning_before_password_expires = rt_str;
    memset(&buffer, 0x00, sizeof(buffer));
    // pclose(out);
    pclose(out);
    out = NULL;
  }

  //debug:
  // for (int i = 0; i < n_Size; i++)
  // {
  //   cout << rt_Vec[i].UserName << endl;
  //   cout << rt_Vec[i].PasswordInactive << endl;
  //   cout << rt_Vec[i].AccountExpires << endl;
  //   cout << rt_Vec[i].LastPasswordChange << endl;
  //   cout << rt_Vec[i].Maximum_number_of_days_between_password_change << endl;
  //   cout << rt_Vec[i].Minimum_number_of_days_between_password_change << endl;
  //   cout << rt_Vec[i].Number_of_days_of_warning_before_password_expires << endl;
  //   cout << "\n";
  // }
  return rt_Vec;
}

std::vector<st_BrowserHistory> GetSysInfo::getFirefoxInternetAccessRecord() //6.1火狐浏览器
{
  int nUser = m_UserName.size();

  int t_CountData = 0;
  if (nUser < 0)
  {
    vector<st_BrowserHistory> empty_vector;
    return empty_vector;
  }
  vector<st_BrowserHistory> get_FirefoxBrowserHistory(nUser); //0. Constructor with how many users

  for (int i = 0; i < m_UserName.size(); i++)
  {
    get_FirefoxBrowserHistory[i].user_Name = m_UserName[i]; // UserName;

    pid_t rt_Pid = getProcessPidByName("firefox"); //takethat:这个应该放在外面?
    if (rt_Pid != -1)
    {
      char kill_Command[100] = "killall firefox";
      printf("Do: %s\n", kill_Command);
      int rt_CmdSys = system(kill_Command);
      // printf("rt_CmdSys=%d\n",rt_CmdSys);
      sleep(2); // sec;
    }
    else
    {
      pid_t rt_Pid = getProcessPidByName("firefox-esr");
      char kill_Command[100] = "killall firefox-esr";
      printf("Do: %s\n", kill_Command);
      int rt_CmdSys = system(kill_Command);
      // printf("rt_CmdSys=%d\n",rt_CmdSys);
      sleep(2); // sec;
    }

    char sql_Command[100] = {0};

    ///home/takethat/.mozilla/firefox/
    //sprintf(sql_Command, "/home/%s/.config/google-chrome/Default/History", m_UserName[i].c_str());
    //---->  撰文针对root  用户加了判断

    string rt_Command;

    //if ("root"==m_UserName[i].c_str() ) //vivatakethat:这个判断条件有误
    if (m_UserName[i] == "root")
    {

      int a = 0;
      sprintf(sql_Command, "ls -d  /root/.mozilla/firefox/*.default"); //take:这里这能判断普用户的friefox记录
                                                                       //	sprintf(sql_Command, "/usr/bin/ls -d  /root/.mozilla/firefox/*.default");//take:这里这能判断普用户的friefox记录
      rt_Command = GetStdoutFromCommand(sql_Command, a);
      if (rt_Command.empty())
      {

        cout << "User:" << m_UserName[i].c_str() << " without Firefox Browser Hisotry Folder" << endl; //说明没这个文件
        continue;
      }

      rt_Command.erase(rt_Command.size() - 1); //take:去除空格
    }
    else
    {
      int a = 0;
      sprintf(sql_Command, "ls -d  /home/%s/.mozilla/firefox/*.default", m_UserName[i].c_str()); //take:这里这能判断普用户的friefox记录
      rt_Command = GetStdoutFromCommand(sql_Command, a);
      if (rt_Command.empty())
      {

        cout << "User:" << m_UserName[i].c_str() << " without Firefox Browser Hisotry Folder" << endl; //说明没这个文件
        continue;
      }

      rt_Command.erase(rt_Command.find_last_of('\n'), string::npos); //去除回车
    }

    if (IsFolderExist(rt_Command.c_str()) == 0)
    {
      fprintf(stderr, "ERROR: %s  ---< user:%s  cannot find FireFox Browser History Database Folder\n", strerror(errno), m_UserName[i].c_str());
      continue;
    }
    else
    {
      //printf("File [%s] Exist!\n", rt_Command.c_str());
      rt_Command.append("\/places.sqlite");

      //if (IsFileExist(rt_Command.c_str()) == -1)   //
      if (IsFileExist(rt_Command.c_str()) == 1)
      {
        //	cout << rt_Command.c_str() << "exist!" << endl;
        // get FireFoxhistory

        //cout << "sql_Command=" << rt_Command.c_str() << endl;  //debug
        //cout<<"username="<<rt_User[i].c_str()<<"|"<<endl;
        rc = sqlite3_open(rt_Command.c_str(), &db);
        if (rc)
        {
          fprintf(stderr, "Can't open database:%s; errmsg:%s\n", sql_Command, sqlite3_errmsg(db));
          sqlite3_close(db);
          continue;
          //  return(1);
        }
        // else
        // {
        //   printf("debug: sqlite3_opoen sussess\n);
        // }

        // rc = sqlite3_exec(db, "SELECT url,title,datetime(last_visit_time/1000000-11644473600,'unixepoch','localtime') FROM urls where url not like 'fi%' and url not like 'c%'", callback, 0, &zErrMsg);

        sqlite3_stmt *pStmt;
        vector<string> v_str_url;
        vector<string> v_str_visittime;
        vector<string> v_str_title;
        v_str_url.clear();
        v_str_title.clear();
        v_str_visittime.clear();

        char *lpText;
        //	wstring str_sqlstr="SELECT url,title,datetime(last_visit_time/1000000-11644473600,'unixepoch','localtime') FROM urls where url not like 'fi%' and url not like 'c%'";

        /* 刘老师的代码*/
        const char *cmd_SQL = "SELECT url, title,datetime(last_visit_date/1000000,'unixepoch','localtime') FROM moz_places where url not like 'fi%' and url not like 'p%'";

        //const char * cmd_SQL = "SELECT datetime(a.visit_date / 1000000, 'unixepoch') AS visit_date, b.url FROM moz_historyvisits AS a JOIN moz_places AS b ON a.place_id = b.id WHERE 1 ORDER BY a.visit_date ASC;";

        /*	int sqlresult = sqlite3_exec(pDB,"PRAGMA auto_vacuum = 1",NULL,NULL, NULL);*/
        int sqlresult = sqlite3_prepare(db, cmd_SQL, -1, &pStmt, NULL);

        //printf("debug,line=%d\n",__LINE__);
        if (sqlresult != SQLITE_OK)
        {
          if (db)
            sqlite3_close(db);

          printf("debug,line=%d sqlresult=%d\n ", __LINE__, sqlresult);
          continue;
          //	return -1;
        }

        get_FirefoxBrowserHistory[i].Type = BrowerType::BT_Firefox;
        while (sqlite3_step(pStmt) == SQLITE_ROW)
        {
          //    printf("debug,line=%d\n",__LINE__);
          //  getchar();
          v_str_url.push_back((char *)sqlite3_column_text(pStmt, 0));
          t_CountData++;

          lpText = (char *)sqlite3_column_text(pStmt, 1);
          if (lpText)
          {
            v_str_title.push_back(lpText);
            t_CountData++;
            //	get_FirefoxBrowserHistory[i].vec_Title = v_str_title;
          }
          else
          {
            v_str_title.push_back("---");
            t_CountData++;
            //		get_FirefoxBrowserHistory[i].vec_Title = v_str_title;
          }
          lpText = (char *)sqlite3_column_text(pStmt, 2);
          if (lpText)
          {
            v_str_visittime.push_back(lpText);
            t_CountData++;
            //		get_FirefoxBrowserHistory[i].vec_DateTime = v_str_visittime;
          }
          else
          {
            v_str_visittime.push_back("---");
            t_CountData++;
            //		get_FirefoxBrowserHistory[i].vec_DateTime = v_str_visittime;
          }
        }

        get_FirefoxBrowserHistory[i].vec_Url = v_str_url;
        get_FirefoxBrowserHistory[i].vec_Title = v_str_title;
        get_FirefoxBrowserHistory[i].vec_DateTime = v_str_visittime;
        get_FirefoxBrowserHistory[i].countData = t_CountData;
        sqlite3_finalize(pStmt);
        sqlite3_close(db);
      }
    }
  } //-< end of for loop

  //debug-Firefox->
  /*for (int i = 0; i < get_FirefoxBrowserHistory.size(); i++)
	{

		if (get_FirefoxBrowserHistory[i].vec_Url.empty())
		{
			continue;
		}


		cout << m_UserName[i] << ":" << endl;
		printf("get_FireFox BrowserHistory[%d].vec_DateTime.size()=%d press ENTERNET to continue\n", i, get_FirefoxBrowserHistory[i].vec_DateTime.size()); getchar();

		for (int j = 0; j < get_FirefoxBrowserHistory[i].vec_DateTime.size(); j++)
		{
			cout << "DateTime:" << get_FirefoxBrowserHistory[i].vec_DateTime[j] << endl;
			cout << "Title:" << get_FirefoxBrowserHistory[i].vec_Title[j] << endl;
			cout << "Url:" << get_FirefoxBrowserHistory[i].vec_Url[j] << endl;
		}
		cout << '\n';
	}*/

  //	debug--<

  return get_FirefoxBrowserHistory;
}

//
vector<st_BrowserHistory> GetSysInfo::getChromeInternetAccessRecord() ////6.0网络记录
{

  int nUser = m_UserName.size();

  if (nUser < 0)
  {
    vector<st_BrowserHistory> empty_vector;
    return empty_vector;
  }
  vector<st_BrowserHistory> get_ChromeBrowserHistory(nUser);

  for (int i = 0; i < m_UserName.size(); i++)
  {
    get_ChromeBrowserHistory[i].user_Name = m_UserName[i]; // UserName;
    pid_t rt_Pid = getProcessPidByName("chrome");
    if (rt_Pid != -1)
    {
      char kill_Command[100] = "killall chrome";

      printf("Do: %s\n", kill_Command);
      int rt_CmdSys = system(kill_Command);
      // printf("rt_CmdSys=%d\n",rt_CmdSys);

      sleep(2); // sec;
    }

    char sql_Command[100] = {0};
    if (m_UserName[i] == "root") //---->  撰文针对root  用户加了判断
    {
      sprintf(sql_Command, "/root/.config/google-chrome/Default/History");
    }
    else
    {
      // get chrome history

      //sprintf(sql_Command, "/home/%s/.mozilla/firefox/bq6moamg.default/places.sqlite",rt_User[i].c_str());
      sprintf(sql_Command, "/home/%s/.config/google-chrome/Default/History", m_UserName[i].c_str());
      //cout<<"sql_Command="<<sql_Command<<endl;  //debug
      //cout<<"username="<<rt_User[i].c_str()<<"|"<<endl;
    }

    rc = sqlite3_open(sql_Command, &db);
    if (rc)
    {
      fprintf(stderr, "Can't open database:%s; errmsg:%s\n", sql_Command, sqlite3_errmsg(db));
      sqlite3_close(db);
      continue;
      //  return(1);
    }
    // else
    // {
    //   printf("debug: sqlite3_opoen sussess\n);
    // }

    // rc = sqlite3_exec(db, "SELECT url,title,datetime(last_visit_time/1000000-11644473600,'unixepoch','localtime') FROM urls where url not like 'fi%' and url not like 'c%'", callback, 0, &zErrMsg);

    sqlite3_stmt *pStmt;
    vector<string> v_str_url;
    vector<string> v_str_visittime;
    vector<string> v_str_title;
    v_str_url.clear();
    v_str_title.clear();
    v_str_visittime.clear();

    char *lpText;
    //	wstring str_sqlstr="SELECT url,title,datetime(last_visit_time/1000000-11644473600,'unixepoch','localtime') FROM urls where url not like 'fi%' and url not like 'c%'";

    const char *cmd_SQL = "SELECT url,title,datetime(last_visit_time/1000000-11644473600,'unixepoch','localtime') FROM urls where url not like 'fi%' and url not like 'c%'";

    /*	int sqlresult = sqlite3_exec(pDB,"PRAGMA auto_vacuum = 1",NULL,NULL, NULL);*/
    int sqlresult = sqlite3_prepare(db, cmd_SQL, -1, &pStmt, NULL);

    //printf("debug,line=%d\n",__LINE__);
    if (sqlresult != SQLITE_OK)
    {
      if (db)
        sqlite3_close(db);

      printf("debug,line=%d sqlresult=%d\n ", __LINE__, sqlresult);
      continue;
      //	return -1;
    }

    //cout << "current user=" << get_ChromeBrowserHistory[i].user_Name << endl; getchar();//debug

    get_ChromeBrowserHistory[i].Type = BrowerType::BT_Chrome;
    while (sqlite3_step(pStmt) == SQLITE_ROW)
    {
      //    printf("debug,line=%d\n",__LINE__);
      //  getchar();
      v_str_url.push_back((char *)sqlite3_column_text(pStmt, 0));
      get_ChromeBrowserHistory[i].vec_Url = v_str_url;

      lpText = (char *)sqlite3_column_text(pStmt, 1);
      if (lpText)
      {
        v_str_title.push_back(lpText);
        get_ChromeBrowserHistory[i].vec_Title = v_str_title;
      }
      else
      {
        v_str_title.push_back("---");
        get_ChromeBrowserHistory[i].vec_Title = v_str_title;
      }
      lpText = (char *)sqlite3_column_text(pStmt, 2);
      if (lpText)
      {
        v_str_visittime.push_back(lpText);
        get_ChromeBrowserHistory[i].vec_DateTime = v_str_visittime;
      }
      else
      {
        v_str_visittime.push_back("---");
        get_ChromeBrowserHistory[i].vec_DateTime = v_str_visittime;
      }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
  }

  // //debug-->
  // for(int i=0;i<rt_User.size();i++)
  // {

  //  if(rt_User[i].empty())
  //  {
  //  continue;
  //  }

  // cout<<rt_User[i]<<":"<<endl;
  // printf("get_ChromeBrowserHistory[%d].vec_DateTime.size()=%d press ENTERNET to continue\n",i,get_ChromeBrowserHistory[i].vec_DateTime.size());getchar());

  //   for(int j=0;j<get_ChromeBrowserHistory[i].vec_DateTime.size();j++)
  //   {
  //       cout<<"DateTime:"<<get_ChromeBrowserHistory[i].vec_DateTime[j]<<endl;
  //    cout<<"Title:"<<get_ChromeBrowserHistory[i].vec_Title[j]<<endl;
  //       cout<<"Url:"<<get_ChromeBrowserHistory[i].vec_Url[j]<<endl;
  //   }
  // cout<<'\n';
  // }
  //debug--<

  // for(int i=0;i<rt_User.size();i++)
  // {

  //  if(get_ChromeBrowserHistory[i].vec_DateTime.empty())
  //  {
  //  continue;
  //  }

  //   for(int j=0;j<get_ChromeBrowserHistory[i].vec_DateTime.size();j++)
  //   {
  //    if( !rt_User[i].empty())
  //    {
  //     rt_t_BrowserHistory[i].vec_Name.assign(rt_User.begin(),rt_User.end());

  //        cout<<"debug,after operator= ,  user="<<rt_t_BrowserHistory[i].vec_Name[j]<<endl;
  //    }

  //     rt_t_BrowserHistory[i].vec_DateTime.assign(get_ChromeBrowserHistory[i].vec_DateTime.begin(),get_ChromeBrowserHistory[i].vec_DateTime.end());

  //      rt_t_BrowserHistory[i].vec_Title.assign(get_ChromeBrowserHistory[i].vec_Title.begin(),get_ChromeBrowserHistory[i].vec_Title.end());

  //           rt_t_BrowserHistory[i].vec_Url.assign(get_ChromeBrowserHistory[i].vec_Url.begin(),get_ChromeBrowserHistory[i].vec_Url.end());

  //   //     cout<<"DateTime:"<<<<endl;
  //   //  cout<<"Title:"<<get_ChromeBrowserHistory[i].vec_Title[j]<<endl;
  //   //     cout<<"Url:"<<get_ChromeBrowserHistory[i].vec_Url[j]<<endl;
  //   }

  // }

  //rt_t_BrowserHistory=get_ChromeBrowserHistory;

  // //debug-->
  // cout<<"start debug...->:"<<endl;
  // for(int i=0;i<rt_User.size();i++)
  // {

  // if(rt_t_BrowserHistory[i].vec_Title.empty())
  //  {
  //  continue;
  //  }

  //cout<<rt_User[i]<<":"<<endl;

  //printf("rt_t_BrowserHistory[%d].vec_DateTime.size()=%d press ENTERNET to continue\n",i,rt_t_BrowserHistory[i].vec_DateTime.size());  getchar();
  //   for(int j=0;j<rt_t_BrowserHistory[i].vec_DateTime.size();j++)
  //   {
  //       cout<<"DateTime:"<<rt_t_BrowserHistory[i].vec_DateTime[j]<<endl;
  //    cout<<"Title:"<<rt_t_BrowserHistory[i].vec_Title[j]<<endl;
  //       cout<<"Url:"<<rt_t_BrowserHistory[i].vec_Url[j]<<endl;
  //   }
  // cout<<'\n';
  //}
  //debug--<
  return get_ChromeBrowserHistory;
}

vector<st_SystemModuleSecurityCheck> GetSysInfo::getSystemModuleSecurityCheck() // 2.系统设备驱动安全检查
{
  // get how many rows:   awk '{printf $1 "\n"}' /proc/modules | wc -l
  FILE *out = NULL;
  char buffer[1024];
  memset(&buffer, 0x00, sizeof(buffer));
  int n_Size = 0;
  out = popen("cat /proc/modules | wc -l | awk 1 ORS=''", "r"); //get how many rows
  fgets(buffer, sizeof(buffer), out);
  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    //cout<<"buffer:"<<buffer<<endl;
    n_Size = atoi(buffer);
    //printf("debug, line=%d  , n_Size=%d\n",__LINE__,n_Size);
    memset(&buffer, 0x00, sizeof(buffer));
    pclose(out);
    out = NULL;
  }
  //n_Size;
  vector<st_SystemModuleSecurityCheck> rt_SystemModuleSecurityCheck(n_Size); //constructor
  //  cout << "debug;proc/modules rows= " << n_Size << endl;
  //cout << "debug: size=" << rt_SystemModuleSecurityCheck.size() << endl;
  for (int i = 0, j = 1; j < n_Size + 1; i++, j++)
  {
    // /*takethat:debug*/
    // if (n_Size > 500)
    // {
    //   i = n_Size - 100;
    //   j = i + 1;
    // }

    char t_Command[1000] = {0};
    //sprintf(t_Command,"/sbin/lsmod | awk '{printf $1 \"\\n\"}' | sed -n %dp | awk 1 ORS=''",j);  // Command base on /sbin/lsmod
    sprintf(t_Command, "cat /proc/modules | awk '{printf $1 \"\\n\"}' | sed -n %dp | awk 1 ORS=''", j); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r"); //get how many rows
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].ModuleName = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command: to get how many instances of the module are currently loaded.
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "cat /proc/modules | awk '{printf $2 \"\\n\"}' | sed -n %dp | awk 1 ORS=''", j); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].MemorySizeModule = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command: to get  how many instances of the module are currently loaded.
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "cat /proc/modules | awk '{printf $3 \"\\n\"}' | sed -n %dp | awk 1 ORS=''", j); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].LoadedTimes = atoi(buffer);
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command: to get the fourth column: module depends upon another module to be present in order to function
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "cat /proc/modules | awk '{printf $4 \"\\n\"}' | sed -n %dp | awk 1 ORS=''", j); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].DependentModule = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command: to get the fifth column: what load state the module is in :Live, Loading ,or Unloading are the only possible values
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "cat /proc/modules | awk '{printf $5 \"\\n\"}' | sed -n %dp | awk 1 ORS=''", j); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].MoslueState = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command: to get the sixth column: the current kernel memory offset for the loaded module
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "cat /proc/modules | awk '{printf $6 \"\\n\"}' | sed -n %dp | awk 1 ORS=''", j); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].OffsetModule = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command:/sbin/modinfo   FileName
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | sed -n 1p | sed 's/\\bf.*[[:blank:]]//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // Command base on /sbin/lsmod
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].FileName = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command:/sbin/modinfo  :License;
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/license/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    // cout<<"debug, t_Command="<<t_Command<<endl;
    // getchar();
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].License = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command:/sbin/modinfo  :description;
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/description/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Description = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    //  command:/sbin/modinfo  :author
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/author/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Author = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :srcversion
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/srcversion/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].srcVersion = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :depends
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/depends/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Depends = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :Retpoline
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/retpoline/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Retpoline = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :Intree
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/intree/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Intree = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :Vermagic
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/vermagic/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Vermagic = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :Sig_id
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/sig_id/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Sig_id = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :Signer
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/signer/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Signer = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo  :Sig_Key
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/sig_key/{print}' | sed 's/.*[[:blank:]]//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Sig_Key = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo：      sig_hashalgo
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | awk '/sig_hashalgo/{print}' | cut -d \":\" -f 2 | sed 's/\\\\*[[:blank:]]*//' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
    //cout<<"debug, t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug: buffer="<<buffer<<endl;
      rt_SystemModuleSecurityCheck[i].Sig_HashAlgo = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
    // command:/sbin/modinfo：      signature
    memset(t_Command, 0x00, sizeof(t_Command));
    sprintf(t_Command, "/sbin/modinfo %s | sed -n '/signature:/,$p' | sed s'/signature:*[[:blank:]]*//' | sed  '/parm*/,$ d' | sed 's/[[:blank:]]//g' | awk 1 ORS=''", rt_SystemModuleSecurityCheck[i].ModuleName.c_str()); // source Command =/sbin/modinfo %s | sed -n 1p | sed 's/\\bd.*[[:blank:]]//'
                                                                                                                                                                                                                            // cout<<"debug, t_Command="<<t_Command<<endl;   //debug
    //getchar();
    out = popen(t_Command, "r");
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      //debug
      //  cout<<"debug: buffer="<<buffer<<endl;
      // getchar();
      rt_SystemModuleSecurityCheck[i].Signature = buffer;
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(out);
      out = NULL;
    }
  }
  //   //Debug:--->
  //   cout << "debug: for check:vector<st_SystemModuleSecurityCheck> Data" << endl;
  //   for (int i = 0; i < n_Size; i++)
  //   {
  //     cout << "rt_SystemModuleSecurityCheck[" << i << "].ModuleName=" << rt_SystemModuleSecurityCheck[i].ModuleName << endl;
  //      cout << "rt_SystemModuleSecurityCheck[" << i << "].MemorySizeModule=" << rt_SystemModuleSecurityCheck[i].MemorySizeModule << endl;
  //       cout << "rt_SystemModuleSecurityCheck[" << i << "].LoadedTimes =" << rt_SystemModuleSecurityCheck[i].LoadedTimes  << endl;
  //     cout << "rt_SystemModuleSecurityCheck[" << i << "].DependentModule =" << rt_SystemModuleSecurityCheck[i].DependentModule << endl;
  //        cout << "rt_SystemModuleSecurityCheck[" << i << "].MoslueState =" << rt_SystemModuleSecurityCheck[i].MoslueState << endl;
  //       cout << "rt_SystemModuleSecurityCheck[" << i << "].OffsetModule  =" << rt_SystemModuleSecurityCheck[i].OffsetModule  << endl;
  //        cout<<"rt_SystemModuleSecurityCheck["<<i<<"].FileName"<<rt_SystemModuleSecurityCheck[i].FileName<<endl;
  //         cout<<"rt_SystemModuleSecurityCheck["<<i<<"].License="<<rt_SystemModuleSecurityCheck[i].License<<endl;
  //           cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Description="<<rt_SystemModuleSecurityCheck[i].Description<<endl;cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Author="<<rt_SystemModuleSecurityCheck[i].Author<<endl;
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].srcVersion="<<rt_SystemModuleSecurityCheck[i].srcVersion<<endl;
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Depends="<<rt_SystemModuleSecurityCheck[i].Depends<<endl;
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Retpoline="<<rt_SystemModuleSecurityCheck[i].Retpoline<<endl;
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Intree="<<rt_SystemModuleSecurityCheck[i].Intree<<endl;  //
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Vermagic="<<rt_SystemModuleSecurityCheck[i].Vermagic<<endl;//
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Sig_id="<<rt_SystemModuleSecurityCheck[i].Sig_id<<endl;//
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Signer="<<rt_SystemModuleSecurityCheck[i].Signer<<endl;//
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Sig_Key="<<rt_SystemModuleSecurityCheck[i].Sig_Key<<endl;//
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Sig_HashAlgo="<<rt_SystemModuleSecurityCheck[i].Sig_HashAlgo<<endl;
  // cout<<"rt_SystemModuleSecurityCheck["<<i<<"].Signature="<<rt_SystemModuleSecurityCheck[i].Signature<<endl;
  //
  //cout<<'\n';
  //Debug--<
  return rt_SystemModuleSecurityCheck;
}

pid_t getProcessPidByName(const char *proc_name)
{
  FILE *fp;
  char buf[100];
  char cmd[200] = {'\0'};
  pid_t pid = -1;
  sprintf(cmd, "pidof %s", proc_name);

  if ((fp = popen(cmd, "r")) != NULL)
  {
    if (fgets(buf, 255, fp) != NULL)
    {
      pid = atoi(buf);
      printf("The Chrome is exist ,the pid = %d \n", pid);
    }
  }

  pclose(fp);
  return pid;
}

vector<st_rpmPackagesInfo> GetSysInfo::getSoftwareCheckOfSystemInstalltaion_RPM(bool &isRPM)
{
  int isExistRPM = exist("/usr/bin/rpm");
  isRPM = isExistRPM;
  vector<st_rpmPackagesInfo> rt_PackagesInfo;
  if (isExistRPM) // rpm?
  {
    /*

		  rpm -qa | wc -l   (  count of the installed packages )

		  rpm -qi <PackName>

		  yum info <PackIName>


		  */

    char c_Command[100] = {0};

    FILE *pFile = NULL;
    char buffer[1024];
    memset(&buffer, 0x00, sizeof(buffer));
    int n_Size = 0;

    pFile = popen("rpm -qa | wc -l | awk 1 ORS=''", "r"); //get how many installed packages;

    if (pFile == NULL)
    {
      printf("Error command  Failed: %s  line=%d\n", strerror(errno), __LINE__);
    }
    else
    {
      fgets(buffer, sizeof(buffer), pFile);
      //cout<<"buffer:"<<buffer<<endl;
      n_Size = atoi(buffer);
      //printf("debug, line=%d  , n_Size=%d\n",__LINE__,n_Size);
      memset(&buffer, 0x00, sizeof(buffer));
      pclose(pFile);
      pFile = NULL;
    }
    vector<st_rpmPackagesInfo> PackagesInfo(n_Size); //constructor size of vector

    sprintf(c_Command, "exec bash -c \"rpm -qa 2>/dev/null\""); //list all rpm package

    int cout_Packages;
    string get_Installed = GetStdoutFromCommand(c_Command, cout_Packages);

    vector<string> take_PackageName = splitGetName(get_Installed, "\n");

    for (int i = 0; i < take_PackageName.size(); i++) //get package name by travese;
    {
      PackagesInfo[i].Name = take_PackageName[i];
      //cout << PackagesInfo[i].Name << endl;
    }
    int n_Times = 0;
    char *get_Value = NULL;
    for (std::vector<st_rpmPackagesInfo>::size_type i = 0; i < PackagesInfo.size(); i++) //get item  with splite
    {
      sprintf(c_Command, "rpm -qi %s", PackagesInfo[i].Name.c_str());
      int n_Line = 0;

      string rt_Popen = GetStdoutFromCommand(c_Command, n_Line);

      if (rt_Popen.empty())
      {
        continue;
      }
      else
      {
        n_Times++;
        cout << "(doing loop to obtain RPM PackageInstalled Info[ " << n_Times << " ] to " << PackagesInfo.size() << " ...)" << endl;

        //  cout << "rt_Popen=" << rt_Popen << endl; getchar();   //debug
        string get_Str;
        get_Str = splite_Item(rt_Popen, "Name        :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Name = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Version     :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Version = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Release     :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Release = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Architecture:", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Architecture = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Install Date:", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Install_Date = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Group       :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Group = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Size        :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Size = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "License     :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].License = get_Value;
          get_Value = NULL;
        }
        get_Str = splite_Item(rt_Popen, "Signature   :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Signature = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Source RPM :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Source_RPM = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Build Date  :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Build_Date = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Build Host  :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Build_Host = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Relocations :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Relocations = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Packager    :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Packager = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Vendor      :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Vendor = get_Value;
          get_Value = NULL;
        }
        get_Str = splite_Item(rt_Popen, "URL         :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].URL = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Summary     :", "\n", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Summary = get_Value;
          get_Value = NULL;
        }

        get_Str = splite_Item(rt_Popen, "Description :", "end", &get_Value);
        if (!get_Str.empty())
        {
          PackagesInfo[i].Description = get_Value;
          get_Value = NULL;
        }

        //	  cout <<"PackagesInfo["<<i<<"].Name="<< PackagesInfo[i].Name << endl; getchar();//debug
      }
    }
    rt_PackagesInfo = PackagesInfo;
    return rt_PackagesInfo;
  }
  return rt_PackagesInfo; // return empty vector data
}

vector<st_aptPackgesInfo> GetSysInfo::getSoftwareCheckOfSystemInstallation_APT(bool &isAPT)
{

  vector<st_aptPackgesInfo> rt_PackagesInfo;
  int IsExistAPT = exist("/usr/bin/apt");
  int IsExistAPTcache = exist("/usr/bin/apt-cache");

  char c_Command[100] = {0};

  if (IsExistAPT || IsExistAPTcache)
  {

    if (IsExistAPT) //apt:->
    {
      isAPT = true;
      FILE *pFile = NULL;
      char buffer[1024];
      memset(&buffer, 0x00, sizeof(buffer));
      int n_Size = 0;
      pFile = popen("apt list --installed | wc -l | awk 1 ORS=''", "r"); //get how many installed packages;

      if (pFile == NULL)
      {
        printf("Error command  Failed: %s  line=%d\n", strerror(errno), __LINE__);
      }
      else
      {
        fgets(buffer, sizeof(buffer), pFile);
        //cout<<"buffer:"<<buffer<<endl;
        n_Size = atoi(buffer);
        //printf("debug, line=%d  , n_Size=%d\n",__LINE__,n_Size);
        memset(&buffer, 0x00, sizeof(buffer));
        pclose(pFile);
        pFile = NULL;
      }
      vector<st_aptPackgesInfo> PackagesInfo(n_Size); //constructor size of vector

      // for(std::vector<st_aptPackgesInfo>::size_type i=2,j=0;i<PackagesInfo.size();i++,j++)
      // {
      // sprintf(c_Command,"exec bash -c \"apt list --installed 2>/dev/null | sed -n %dp |  cut -d/ -f1  | awk 1 ORS=''\"",i);//get package name by travese;
      sprintf(c_Command, "exec bash -c \"apt list --installed 2>/dev/null\""); //get package name by travese;//后面的作为辅助信息也要

      int cout_Packages;
      string get_Installed = GetStdoutFromCommand(c_Command, cout_Packages);

      vector<string> take_PackageName = splitGetName(get_Installed, "/");//加上字段是属性  已签名

      for (int i = 0; i < take_PackageName.size(); i++)
      {
        PackagesInfo[i].PackageName = take_PackageName[i];
      }

      // for(int i=0;i<take.size();i++)//debug
      // {
      //   cout<<"take["<<i<<"]"<<"="<<take[i]<<endl<<endl;;
      // }

      //     pFile=popen(c_Command, "r");

      // if (pFile == NULL)
      // {
      //  printf("Error command  Failed: %s  line=%d\n",strerror(errno),__LINE__);
      // }
      // else
      // {
      //   fgets(buffer, sizeof(buffer), pFile);
      //   //cout<<"buffer:"<<buffer<<endl;
      //   PackagesInfo[j].PackageName=buffer;
      //  // printf("debug,  PackagesInfo[%d].PackageName=%s   line=%d  , n_Size=%d\n",j, PackagesInfo[j].PackageName.c_str(),__LINE__,n_Size); //debug
      //   memset(&buffer, 0x00, sizeof(buffer));
      //   memset(&c_Command,0x00,sizeof(c_Command));
      //   pclose(pFile);
      //   pFile= NULL;

      // }

      // }
      //get `Essential` item  //  apt show  dash | awk '/Essential/{print}' | sed 's:*[[:blank:]]//' | awk 1 ORS=''
      int n_Times = 0;
      char *get_Value;
      for (std::vector<st_aptPackgesInfo>::size_type i = 0; i < PackagesInfo.size(); i++)
      {

        //->for debug
        // if (i == 100)
        // {
        // 	i = PackagesInfo.size() - 1;
        // }
        //-< debug

        sprintf(c_Command, "apt-cache show %s", PackagesInfo[i].PackageName.c_str());//把这个设置 `详细信息按钮里`

        int n_Line = 0;

        string rt_Popen = GetStdoutFromCommand(c_Command, n_Line);

        if (rt_Popen.empty())
        {
          continue;
        }
        else
        {
          n_Times++;
          cout << "(doing loop to obtain PackageInstalled[ " << n_Times << " ] to " << PackagesInfo.size() << " ... )" << endl;

          string get_Str = splite_Item(rt_Popen, "Package:", "\n", &get_Value);
          //cout<<"Package:"<<get_Str<<endl;  getchar();

          PackagesInfo[i].Type = 0; //APT type

          get_Str = splite_Item(rt_Popen, "Version:", "\n", &get_Value);
          //cout<<"Version:"<<get_Str<<endl;
          if (!get_Str.empty())
          {
            PackagesInfo[i].Version = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Priority:", "\n", &get_Value);
          //cout<<"Priority:"<<get_Str<<endl;
          if (!get_Str.empty())
            PackagesInfo[i].Priority = get_Value;
          memset(&get_Value, 0x00, sizeof(get_Value));

          get_Str = splite_Item(rt_Popen, "Section:", "\n", &get_Value);
          //cout<<"Section:"<<get_Str<<endl;
          if (!get_Str.empty())
          {
            PackagesInfo[i].Section = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Installed-Size:", "\n", &get_Value);
          if (!get_Str.empty())
          {

            PackagesInfo[i].Installed_Size = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Origin:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            PackagesInfo[i].Origin = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Supported:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].Supported = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          //
          get_Str = splite_Item(rt_Popen, "Filename:", "\n", &get_Value);
          if (!get_Str.empty())
          {
          //cout<<"Origin:"<<get_Str<<endl;
          PackagesInfo[i].Filename = get_Value;
          memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Suggests:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].Suggests = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Task:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].Task = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "MD5sum:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].MD5sum = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "SHA1:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].SHA1 = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "APT-Manual-Installed:", "\n", &get_Value);
          //cout<<"Origin:"<<get_Str<<endl;
          if (!get_Str.empty())
          {
            PackagesInfo[i].APT_Manual_Installed = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Original-Maintainer:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].Original_Maintainer = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Provides:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].Provides = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "SHA256:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].SHA256 = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Bugs:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            PackagesInfo[i].Bugs = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Maintainer:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Origin:"<<get_Str<<endl;
            PackagesInfo[i].Maintainer = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Depends:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Depends:"<<get_Str<<endl;
            PackagesInfo[i].Depends = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          //

          get_Str = splite_Item(rt_Popen, "Download-Size:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Download-Size:"<<get_Str<<endl;
            PackagesInfo[i].Download_Size = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Description:", "end", &get_Value);

          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Description = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Source:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Source = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Breaks:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Breaks = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Replaces:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Replaces = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Architecture:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Architecture = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Homepage:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Homepage = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Essential:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Essential = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Section:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Section = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "APT_Sources:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].APT_Sources = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Source:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Source = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Multi_Arch:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Multi_Arch = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          get_Str = splite_Item(rt_Popen, "Pre_Depends:", "\n", &get_Value);
          if (!get_Str.empty())
          {
            //cout<<"Description:"<<get_Str<<endl;
            PackagesInfo[i].Pre_Depends = get_Value;
            memset(&get_Value, 0x00, sizeof(get_Value));
          }

          // printf("debug,  PackagesInfo[%d].PackageName=%s   line=%d  , n_Size=%d\n",j, PackagesInfo[j].PackageName.c_str(),__LINE__,n_Size); //debug
          memset(&buffer, 0x00, sizeof(buffer));
          memset(&c_Command, 0x00, sizeof(c_Command));

        } //-< '}' for else

      } //-<  '}' for 'for loop'

      // // get `Architecture` item ;  //
      // if(IsExistAPTcache)
      // for(int i=0;i<PackagesInfo.size();i++)
      // {
      // sprintf(c_Command,"exec bash -c \"apt show  dash | awk '/Essential/{print}' | sed 's:*[[:blank:]]//' | awk 1 ORS=''\"");
      //       pFile=popen(c_Command, "r");

      //   if (pFile == NULL)
      //   {
      //    printf("Error command  Failed: %s  line=%d\n",strerror(errno),__LINE__);
      //   }
      //   else
      //   {
      //     fgets(buffer, sizeof(buffer), pFile);
      //     //cout<<"buffer:"<<buffer<<endl;
      //     PackagesInfo[i].Essential=buffer;
      //    // printf("debug,  PackagesInfo[%d].PackageName=%s   line=%d  , n_Size=%d\n",j, PackagesInfo[j].PackageName.c_str(),__LINE__,n_Size); //debug
      //     memset(&buffer, 0x00, sizeof(buffer));
      //     memset(&c_Command,0x00,sizeof(c_Command));
      //     pclose(pFile);
      //     pFile= NULL;

      //   }

      // }

      //debug->
      //
      //for( vector<st_aptPackgesInfo>::size_type i=0;i<PackagesInfo.size();i++)
      //{
      //
      //  cout<<"---------["<<i<<"]-----------------"<<endl;
      //  //getchar();  //debug
      //cout<<"package:"<<PackagesInfo[i].PackageName<<endl;
      //
      //
      //if(!PackagesInfo[i].Priority.empty())
      //cout<<"Priority"<<PackagesInfo[i].Priority<<endl;
      //
      //if(!PackagesInfo[i].Essential.empty())
      //cout<<"Essential:"<<PackagesInfo[i].Essential<<endl;
      //
      //
      //if(!PackagesInfo[i].Origin.empty())
      //cout<<"Origin:"<<PackagesInfo[i].Origin<<endl;
      //
      //
      //if(!PackagesInfo[i].Maintainer.empty())
      //cout<<"Maintainer:"<<PackagesInfo[i].Maintainer<<endl;
      //
      //
      //if(!PackagesInfo[i].Bugs.empty())
      //cout<<"Bugs:"<<PackagesInfo[i].Bugs<<endl;
      //
      //if(!PackagesInfo[i].Installed_Size.empty())
      //cout<<"Installed-Size:"<<PackagesInfo[i].Installed_Size<<endl;
      // if(!PackagesInfo[i].Depends.empty())
      //cout<<"Depends:"<<PackagesInfo[i].Depends<<endl;
      //
      // if(!PackagesInfo[i].Breaks.empty())
      //cout<<"Breaks:"<<PackagesInfo[i].Breaks<<endl;
      //
      //
      // if(!PackagesInfo[i].Replaces.empty())
      //cout<<"Replaces:"<<PackagesInfo[i].Replaces<<endl;
      //
      //
      // if(!PackagesInfo[i].Download_Size.empty())
      //cout<<"Download-Size:"<<PackagesInfo[i].Download_Size<<endl;
      //
      // if(!PackagesInfo[i].Homepage.empty())
      //cout<<"Homepage:"<<PackagesInfo[i].Homepage<<endl;
      //
      //if(!PackagesInfo[i].Architecture.empty())
      //cout<<"Architecture:"<<PackagesInfo[i].Architecture<<endl;
      ////
      //if(!PackagesInfo[i].Section.empty())
      //cout<<"Section:"<<PackagesInfo[i].Section<<endl;
      //
      //
      //if(!PackagesInfo[i].Source.empty())
      //cout<<"Source:"<<PackagesInfo[i].Source<<endl;
      //
      //
      //if(!PackagesInfo[i].Task.empty())
      //cout<<"Task:"<<PackagesInfo[i].Task<<endl;
      //
      //if(!PackagesInfo[i].Supported.empty())
      //cout<<"Supported:"<<PackagesInfo[i].Supported<<endl;
      //
      //if(!PackagesInfo[i].Version.empty())
      //cout<<"Version:"<<PackagesInfo[i].Version<<endl;
      //
      //if(!PackagesInfo[i].Provides.empty())
      //cout<<"Provides:"<<PackagesInfo[i].Provides<<endl;
      //
      //
      //if(!PackagesInfo[i].APT_Sources.empty())
      //cout<<"APT_Sources:"<<PackagesInfo[i].APT_Sources<<endl;
      //
      //}

      //debug-<
      rt_PackagesInfo = PackagesInfo;

    } // -<   for 'apt':

    else if (0) // what  ?
    {
      isAPT = false;

      return rt_PackagesInfo;

    } //<-  what

  } //-<  for  `if(IsExistAPT||IsExistAPTcache||IsExitYUM)`
  else
  {
    isAPT = false;
    // no any package manager
    return rt_PackagesInfo;
  }
}

std::vector<st_MountPoint> GetSysInfo::getSystemMountPoint()
{
  std::vector<st_MountPoint> rt_MountPoint;

  FILE *mount_table;
  struct mntent *mount_entry;
  struct statfs s;
  unsigned long blocks_used;
  unsigned blocks_percent_used;
  //	int  blocks_percent_used;
  const char *disp_units_hdr = NULL;
  mount_table = NULL;
  mount_table = setmntent("/etc/mtab", "r");
  if (!mount_table)
  {
    fprintf(stderr, "set mount entry error\n");
    return rt_MountPoint;
  }
  disp_units_hdr = "     Size";
  //printf("Filesystem           %-15sUsed Available %s Mounted on\n",
  // disp_units_hdr, "Use%");
  while (1)
  {
    const char *device;
    const char *mount_point;
    if (mount_table)
    {
      mount_entry = getmntent(mount_table);
      if (!mount_entry)
      { //takethat:读到了结尾
        endmntent(mount_table);
        break;
      }
    }
    else
      continue;

    st_MountPoint t_MountPointData;
    device = mount_entry->mnt_fsname;
    mount_point = mount_entry->mnt_dir;
    //fprintf(stderr, "mount info: device=%s mountpoint=%s\n", device, mount_point);
    if (statfs(mount_point, &s) != 0)
    {
      fprintf(stderr, "statfs failed!\n");
      continue;
    }
    if ((s.f_blocks > 0) || !mount_table)
    {
      //  blocks_used = s.f_blocks - s.f_bfree;
      //  blocks_percent_used = 0;
      //  if (blocks_used + s.f_bavail)
      //  {
      // 	 blocks_percent_used = (blocks_used * 100ULL + (blocks_used + s.f_bavail) / 2) / (blocks_used + s.f_bavail);
      //  }
      blocks_used = s.f_blocks - s.f_bfree;
      blocks_percent_used = 0;
      if (blocks_used + s.f_bavail)
      {
        blocks_percent_used = (blocks_used * 100ULL + (blocks_used + s.f_bavail) / 2) / (blocks_used + s.f_bavail);
      }

      /* GNU coreutils 6.10 skips certain mounts, try to be compatible.  */
      if (strcmp(device, "rootfs") == 0)
        continue;
      /*		 if (printf("\n%-20s" + 1, device) > 20)
				 printf("\n%-20s", "");*/

      char buff_MountPointInfo[100] = {0};

      sprintf(buff_MountPointInfo, "%-20s", device);

      t_MountPointData.FileSystem = buff_MountPointInfo;
      memset(&buff_MountPointInfo, 0x00, sizeof(buff_MountPointInfo));

      char s1[20];
      char s2[20];
      char s3[20];
      strcpy(s1, kscale(s.f_blocks, s.f_bsize));
      strcpy(s2, kscale(s.f_blocks - s.f_bfree, s.f_bsize));
      strcpy(s3, kscale(s.f_bavail, s.f_bsize));
      //  printf(" %9s %9s %9s %3u%% %s\n",
      // 	s1,
      // 	s2,
      // 	s3,
      // 	blocks_percent_used, mount_point);

      sprintf(buff_MountPointInfo, "%9s", s1);
      t_MountPointData.Size = buff_MountPointInfo;
      memset(&buff_MountPointInfo, 0x00, sizeof(buff_MountPointInfo));

      sprintf(buff_MountPointInfo, "%9s", s2);
      t_MountPointData.Used = buff_MountPointInfo;
      memset(&buff_MountPointInfo, 0x00, sizeof(buff_MountPointInfo));

      sprintf(buff_MountPointInfo, "%9s", s3);
      t_MountPointData.Available = buff_MountPointInfo;
      memset(&buff_MountPointInfo, 0x00, 100);

      sprintf(buff_MountPointInfo, "%3u%%", blocks_percent_used);
      //printf("useragepercentage=%s\n",buff_MountPointInfo);
      t_MountPointData.UseagePercentage = buff_MountPointInfo;
      //cout<<"str="<< t_MountPointData.UseagePercentage<<endl;
      memset(&buff_MountPointInfo, 0x00, sizeof(buff_MountPointInfo));

      sprintf(buff_MountPointInfo, "%s", mount_point);
      t_MountPointData.MountOn = buff_MountPointInfo;
      memset(&buff_MountPointInfo, 0x00, sizeof(buff_MountPointInfo));

      rt_MountPoint.push_back(t_MountPointData);
    }
  }

  return rt_MountPoint;
}

void GetSysInfo::checkInternetConnect(std::vector<st_checkConnection> &vec_st_checkConnection)
{
  //检测操作系统是否能连通外部链接

  st_checkConnection array_Hostname[3];
  array_Hostname[0].destination = "www.baidu.com";

  array_Hostname[1].destination = "www.163.com";
  array_Hostname[2].destination = "www.qq.com";

  const char *t_Hostname[3];
  t_Hostname[0] = array_Hostname[0].destination.c_str();
  t_Hostname[1] = array_Hostname[1].destination.c_str();
  t_Hostname[2] = array_Hostname[2].destination.c_str();
  char ip[100];

  for (int i = 0; i < 3; i++)
  {
    int retIsCheckNetwork0 = hostname_to_ip(t_Hostname[i], ip);
    if (retIsCheckNetwork0 == -1)
    {
      array_Hostname[i].result = "No Internet!(Fail to connection)";
      vec_st_checkConnection.push_back(array_Hostname[i]);
    }
    else
    {
      array_Hostname[i].result = "Connection!";
      vec_st_checkConnection.push_back(array_Hostname[i]);
    }

    //cout<<" retIsCheckNetwork0:"<< retIsCheckNetwork0<<endl;
  }
}
int GetSysInfo::getUSB_EventHistory(std::vector<st_filtered> &vecGetUSB_EventHIstory) //系统USB插入拔出时事件记录
{

  /*
  先判断有没有journalctl 或者journalctl在哪里
  ubuntu14就没有Journalctl

中标麒麟/Centos 7有Journalctl但是没有参数  short-iso-precise  只有short-iso

*/

  if (1)
  {
  }
  else
  {
    /* code */
  }

  int coutUSB_EventHistory = 0;

  /*

journalctl |grep -iw 'usb'|grep -iwe 'Product:\|Manufacturer:\|SerialNumber:\|USB disconnect'|awk '{$3=" ";print $0}'|sed s/"   "/" "/

源: journalctl |grep -iw 'usb'|grep -iwe 'Product:\|Manufacturer:\|SerialNumber:\|USB disconnect'|awk '{$3=" ";print $0}'|sed s/"   "/" "/ 

string cmd_Journalctl="journalctl |grep -iw \'usb\'|grep -iwe \'Product:\\|Manufacturer:\\|SerialNumber:\\|USB disconnect'|awk \'{$3=\" \";print $0}\'|sed s/\"   \"/\" \"/";

*/

  /*

拔出U盘:    2019-09-25T15:16:55+0800 t470p kernel: usb 1-2: USB disconnect, device number 14

*/

  //  string cmd_Journalctl="journalctl |grep -iw \'usb\'|grep -iwe \'Product:\\|Manufacturer:\\|SerialNumber:\\|USB disconnect'";

  //  string cmd_Journalctl="journalctl -o short-iso";
  //

  string cmd_Journalctl = "journalctl -o short-iso";

  struct st_baseFiltered
  {
    char type;
    string date;
    string source;
    int order;
    int usbProductType = MassStorage;

    void clear()
    {
      type = 0;
      date = "";
      source = "";
    }
  };

  std::vector<st_baseFiltered> vec_BaseFiltered;

  std::vector<st_filtered> vec_Filtered;
  static int c_disconn = 0;
  static int c_CompareUSB = 0;
  static int c_order = 0;
  string data;
  FILE *stream;
  const int max_buffer = 1024 * 1024;
  char buffer[max_buffer];
  //cmd.append(" 2>dev/null \" ");
  string str_buffer;

  //cout<<"cmd.c_str()="<<cmd.c_str()<<endl; getchar();
  stream = popen(cmd_Journalctl.c_str(), "r");

  string st_takeUSB;
  if (stream == NULL)
  {
    printf("Error command  Failed: %s  line=%d\n", strerror(errno), __LINE__);
    return -1;
  }
  else
  {
    //  if (stream) {
    while (!feof(stream))
    {
      if (fgets(buffer, max_buffer, stream) != NULL)
      {
        st_takeUSB.clear(); //take:这句没有会出错,为何?
                            //data.append(buffer);
                            //  cout<<"buffer:"<<buffer<<endl;
                            //static int c_con=0,d_con=0;
        str_buffer = buffer;
        st_filtered temp_Filtered;
        st_baseFiltered temp_BaseFiltered;
        temp_BaseFiltered.usbProductType = Other; //默认设置其他类型
                                                  //cout<<"str_buffer:"<<str_buffer<<endl;

        // if(str_buffer.find("usb")!=string::npos)
        // {
        //   cout<<__LINE__<<"[debug]"<<str_buffer<<endl;

        // }

        const std::regex txt_regex("(?:]|:) usb (.*?): ");        //take:这个正则表达至少要GCC4.9才有效不然是失效的
        bool isGetUSB = std::regex_search(str_buffer, txt_regex); //take:str_buffer USB记录信息
        if (isGetUSB)
        {
          st_takeUSB = "";
          st_takeUSB.append(str_buffer);

          //   str_buffer+='\n'; //take:加上换行符!
          //cout<<__LINE__<<":"<<str_buffer<<endl; getchar();  //take:debug
          if (string::npos != str_buffer.find("xHCI Host Controller"))
          {
            continue;
          }

          if (string::npos != str_buffer.find("New USB device found") || string::npos != str_buffer.find("Product: ") || string::npos != str_buffer.find("Manufacturer: ") || string::npos != str_buffer.find("SerialNumber: "))
          {
            size_t intercept_TIME = str_buffer.find(' ');

            string str_date = str_buffer.substr(0, intercept_TIME);
            string take_Intercept = str_buffer.substr(intercept_TIME);

            temp_BaseFiltered.type = 'c';
            temp_BaseFiltered.date = str_date;
            temp_BaseFiltered.source = take_Intercept;
            c_order++;
            temp_BaseFiltered.order = c_order;
            vec_BaseFiltered.push_back(temp_BaseFiltered);
            temp_BaseFiltered.clear();
            //c_con++;
          }
          else if (string::npos != str_buffer.find("USB disconnect") || string::npos != str_buffer.find("USB device number")) // ` USB device number `是关机后重启
          {
            c_order++;
            size_t intercept_DisTime = str_buffer.find(' ');

            string str_DisconnDate = str_buffer.substr(0, intercept_DisTime);
            string take_DisIntercept = str_buffer.substr(intercept_DisTime);
            c_disconn++;
            temp_BaseFiltered.type = 'd';
            temp_BaseFiltered.date = str_DisconnDate;
            // cout<<__LINE__<<",  temp_BaseFiltered.date:"<<  temp_BaseFiltered.date<<endl;
            temp_BaseFiltered.source = take_DisIntercept;
            temp_BaseFiltered.order = c_order;

            vec_BaseFiltered.push_back(temp_BaseFiltered);
            temp_BaseFiltered.clear();
          }
          //USB Mass Storage device
          else if (string::npos != str_buffer.find("USB Mass Storage device")) //take_mark:存储设备判断条件
          {

            temp_BaseFiltered.usbProductType = MassStorage;
          }

          else
            continue;
        }
        else
        {
          continue;
        }
      }
      else
        continue; //take: 不包含usb字符,  所以continue下一句
    }             //--<:  eof of while()

  } //eof of stream

  /*debug*/

  if (!vec_BaseFiltered.empty())
  {
    if (vec_BaseFiltered[0].type == 'd')
    {
      vec_BaseFiltered[0].clear();
    }
  }
  else
    return -1;
  // cout<<"---------------------------------------------->"<<endl;
  // for(int k=0;k<vec_BaseFiltered.size();k++)
  // {
  //  cout<<"type:"<<vec_BaseFiltered[k].type<<endl;
  //   cout<<"date:"<<vec_BaseFiltered[k].date<<endl;
  //   cout<<"source:"<<vec_BaseFiltered[k].source<<endl;
  //  }
  //  cout<<"after: c_disconn="<<c_disconn<<endl;
  // cout<<"Press ENnter to continue!"<<endl; // getchar();

  int link;
  bool interrupted = true;
  const std::regex idevendor_regex{"idVendor=(\\w+)"};
  const std::regex idProduct_regex("idProduct=(\\w+)");
  const std::regex usb_regex("usb (.*[0-9]):");
  st_filtered tem_USBData;

  // tem_USBData.usbProductType=;

  for (int i = 0, j = 0; i < vec_BaseFiltered.size(); i++)
  {

    if (vec_BaseFiltered[i].type == 'c')
    {
      if (std::string::npos != vec_BaseFiltered[i].source.find("New USB device found"))
      {
        link = 0;

        std::smatch m;

        std::regex_search(vec_BaseFiltered[i].source, m, usb_regex);
        tem_USBData.port = m[0];                        //Port
        tem_USBData.iOrder = vec_BaseFiltered[i].order; //
        if (std::regex_search(vec_BaseFiltered[i].source, m, idevendor_regex))
        {
          string t_vendor = m[0];
          std::size_t pos_idVendor = t_vendor.find("idVendor=");
          tem_USBData.idVendor = t_vendor.substr(pos_idVendor + strlen("idVendor="));

          tem_USBData.conn = vec_BaseFiltered[i].date; //conn 时间
        }
        if (std::regex_search(vec_BaseFiltered[i].source, m, idProduct_regex))
        {
          string t_idProduct = m[0];
          std::size_t pos_idProduct = t_idProduct.find("idProduct=");
          tem_USBData.idProduct = t_idProduct.substr(pos_idProduct + strlen("idProduct="));
        }
        interrupted = false;
        link++;
        continue;
      }

      if (interrupted == false)
      { //继续读

        const std::regex product_Regex("Product: (.*?$)");
        size_t n_pos = vec_BaseFiltered[i].source.find('\n'); //take:这里去除换符号，不然后面的正则表达式会失效
        if (n_pos != std::string::npos)
        {
          vec_BaseFiltered[i].source = vec_BaseFiltered[i].source.substr(0, n_pos);
        }
        //  cout<< "vec_BaseFiltered["<<i<<"].source" << vec_BaseFiltered[i].source <<endl;getchar();//take:debug
        const std::regex manufacturer_Regex("Manufacturer: (.*?$)");
        const std::regex serialnumber_Regex("SerialNumber: (.*?$)");
        std::smatch m_product;
        std::smatch m_manufacutre;
        std::smatch m_seri;

        if (std::regex_search(vec_BaseFiltered[i].source, m_product, product_Regex)) //Product:
        {
          string t_srProduct = m_product[0];
          std::size_t ops_Product = t_srProduct.find("Product:");
          tem_USBData.prod = t_srProduct.substr(ops_Product + strlen("Product:"));

          continue;
        }
        else if (std::regex_search(vec_BaseFiltered[i].source, m_product, manufacturer_Regex)) //Manufacturer:
        {
          string t_stManufact = m_product[0];
          std::size_t ops_Manufact = t_stManufact.find("Manufacturer:");

          tem_USBData.manufact = t_stManufact.substr(ops_Manufact + strlen("Manufacturer:"));

          continue;
        }
        else if (std::regex_search(vec_BaseFiltered[i].source, m_seri, serialnumber_Regex)) //seri
        {
          string t_stSerialnumber = m_seri[0];
          std::size_t ops_SerialNumber = t_stSerialnumber.find("SerialNumber:");
          tem_USBData.serial = t_stSerialnumber.substr(ops_SerialNumber + strlen("SerialNumber:"));

          vec_Filtered.push_back(tem_USBData); //take: 读完一条
          interrupted = true;
          tem_USBData.clear();
          continue;
        }
        else //没有读完也要插入一条
        {
          interrupted = true;
          tem_USBData.iOrder = vec_BaseFiltered[i].order; //
          vec_Filtered.push_back(tem_USBData);            //take:插入一条数据

          tem_USBData.clear();
          continue;
        }
      }
      else
        continue;

    } //eof  `if type ==c`
    else
      continue;
  } // eof for

  static int j = 0;
  for (int i = 0; i < vec_BaseFiltered.size(); i++)
  {
    if (j < c_disconn)
    {

      //  if(vec_BaseFiltered[i].date=="2019-10-29T11:14:20+0800")
      //  {
      //    cout<<"find out debug"<<endl;
      //    getchar();

      //    cout<<__LINE__<<endl;
      //  }

      //  cout<<"vec_Filtered.size()="<<vec_Filtered.size()<<endl;
      //         cout<<__LINE__<<endl;cout<<"Press ENTER to continue"<<endl; getchar();//debug
      if (vec_BaseFiltered[i].type == 'd') //匹配断开时间
      {
        std::smatch m_discon;
        std::regex_search(vec_BaseFiltered[i].source, m_discon, usb_regex);

        string dis_port = m_discon[0];
        if (dis_port.empty())
        {
          continue;
        }

        if (!vec_Filtered.empty())
        {
          for (int j2 = 0; j2 < vec_Filtered.size(); j2++)
          {
            if (vec_Filtered[j2].isCheck != true)
            {
              //   cout<<__LINE__<<endl;//debug

              if (dis_port == vec_Filtered[j2].port)//这个判断条件是否满足：可能还要加上时间相差最近，2020-03-04
              {
                if (vec_BaseFiltered[i].order > vec_Filtered[j2].iOrder)
                {
                  vec_Filtered[j2].disconn = vec_BaseFiltered[i].date;
                  vec_Filtered[j2].isCheck = true;

                  // cout<<"compare disconn usb_port!!!!"<<endl;
                  // cout<<"vec_Filtered[j].disconn="<< vec_Filtered[j2].disconn<<endl;
                  //  cout<<"Pless ENTER to continue!"<<endl;// getchar();  //take:debug
                  c_CompareUSB++;
                  j++;
                  break;
                }
                else
                {
                  vec_BaseFiltered[i].clear();
                }
              }
            }
          }
          continue;
        }
        else
          break;
      } //  eof  `  if (vec_BaseFiltered[i].type=='d') `
      else
        continue;
    }
    else
      break;
  } //take:  eof  for 'd': `for(int i=0,j=0;i<vec_BaseFiltered.size();i++)`

  //cout<<"c_disconn:"<<c_disconn<<"Enternet to continue\n"<<endl;getchar();

  //debug

  vecGetUSB_EventHIstory = vec_Filtered;

  // cout<<"after  c_disconn:"<< c_disconn<<endl;
  // cout<<"c_CompareUSB:"<<c_CompareUSB<<endl;
  // cout<<"Press Enternet:\n"<<endl;//getchar();
  // for(int i=0;i<vec_Filtered.size();i++)
  // {

  //   cout<<"["<<i<<"]:"<<endl;
  //   cout<<"conn"<<vec_Filtered[i].conn<<endl;

  //  cout<<"disconn:"<< vec_Filtered[i].disconn<<endl;
  //   cout<<"port:"<<vec_Filtered[i].port<<endl;
  //  cout<<"idProduct:"<<vec_Filtered[i].idProduct<<endl;
  //  cout<<"idVendor:"<<vec_Filtered[i].idVendor<<endl;

  //  cout<<"prod:"<<vec_Filtered[i].prod<<endl;
  //  cout<<"serial:"<<vec_Filtered[i].serial<<endl;
  //  cout<<"manufact:"<<vec_Filtered[i].manufact<<endl;
  // cout<<"----------------------------------------------------------------------"<<endl;
  //  }
}

void GetSysInfo::getBluetooth(std::vector<st_Hardware_lshw> &vecGetBluetooth)
{

  char buff_ProcessPath[1024];
  getcwd(buff_ProcessPath, 1024);

#ifndef NONLS
  printf("%d\n", __LINE__);
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8");
  textdomain(PACKAGE);
#endif

  disable("isapnp");

  disable("output:json");
  disable("output:db");
  disable("output:xml");
  disable("output:html");
  disable("output:hwpath");
  disable("output:businfo");
  disable("output:X");
  disable("output:quiet");
  disable("output:sanitize");
  disable("output:numeric");
  enable("output:time");

  // define some aliases for nodes classes
  alias("disc", "disk");
  alias("cpu", "processor");
  alias("lan", "network");
  alias("net", "network");
  alias("video", "display");
  alias("sound", "multimedia");
  alias("modem", "communication");

  printf("proccer_path:");
  printf(buff_ProcessPath);
  std::vector<st_HardWare> rt_VecBluetoothardWare;

  cout << "[line]=" << __LINE__ << "[fn]="
       << "getBluetooth" << endl;
  hwNode computer("computer",
                  hw::generic);

  scan_system(computer);

  int argc = 3;
  char *argv_all[] = {"process", "", ""};

  if (!parse_options(argc, argv_all))
  {
    printf("error\n");
    exit(1);
  }
  rt_VecBluetoothardWare = print(computer, enabled("output:html"));
  cout << "size=" << rt_VecBluetoothardWare.size();

  for (std::vector<st_HardWare>::iterator it = rt_VecBluetoothardWare.begin(); it != rt_VecBluetoothardWare.end(); ++it)
  {
    if (it->description.empty() || (it->description.find("Bluetooth") == string::npos))
    {
      //  cout<<"empty1"<<endl;
      continue;
    }

    st_Hardware_lshw t_Bluetooth;
    // t_Bluetooth.capacity=it->capacity;
    t_Bluetooth.description = it->description;
    t_Bluetooth.product = it->product;
    t_Bluetooth.vendor = it->vendor;
    t_Bluetooth.physical_id = it->physid;
    t_Bluetooth.bus_info = it->businfo;
    //   cout<<"description:"<<it->description<<endl;
    //   cout<<"product:"<<it->product<<endl;
    //  cout<<"vendor:"<<it->vendor<<endl;
    //  cout<<"physid:"<<it->physid<<endl;
    //  cout<<"businfo:"<<it->businfo<<endl;

    //       string t_logicalname;
    //       // cout<<"logicalnames:";
    //        for(int i=0;i<it->logicalnames.size();i++)
    //        {
    //            // cout<<it->logicalnames[i]<<" ";
    //            t_logicalname.append(it->logicalnames[i]);
    //        }
    // t_Bluetooth.logical_name=t_logicalname;
    t_Bluetooth.version = it->version;
    t_Bluetooth.serial = it->serial;

    //  cout<<"\n"<<"version:";
    //    cout<<it->version<<endl;
    //    cout<<"serial:";
    //    cout<<it->serial<<endl;

    //t_Bluetooth.width=std::to_string( it->with);
    t_Bluetooth.width.append(" bits");
    //cout<<"with:"<<it->with<<endl;

    // t_Bluetooth.clock=it->Clock;
    //  cout<<"clock:"<<it->Clock <<endl;

    string t_capabilities;
    //cout<<"capabilities:";
    for (int i = 0; i < it->capabilities.size(); i++)
    {
      //   cout<<it->capabilities[i]<<" ";
      t_capabilities.append(it->capabilities[i]);
    }
    //  cout<<endl;
    t_Bluetooth.capabilities = t_capabilities;

    string t_config;
    //  cout<<"configuration:";
    //  for(int i=0;i<it->config.size();i++)
    //  {
    //cout<<it->config[i]<<" ";
    //     t_config.append(it->config[i]);
    t_config = it->config;
    //  }
    //  cout<<endl;
    t_Bluetooth.configuration = t_config;

    // string t_resources;
    //   //   cout<<"resources:";
    //      for(int i=0;i<it->resources.size();i++)
    //      {
    //              //  cout<< it->resources[i]<<" ";
    //              t_resources.append(it->resources[i]);
    //      }
    // //  cout<<endl;
    //               t_Bluetooth.resources=t_resources;
    t_Bluetooth.id = it->id;

    vecGetBluetooth.push_back(t_Bluetooth);
  }

  desotry(); //清楚静态对象

  char buff_ProcessPathCheck[1024];
  getcwd(buff_ProcessPathCheck, 1024);
  int ret_Chdir;
  if (strcmp(buff_ProcessPath, buff_ProcessPathCheck) != 0)
    ret_Chdir = chdir(buff_ProcessPath);

  if (ret_Chdir == -1)
  {
    fprintf(stderr, "Failed to open %s\n"
                    "The reason *may* have been %s\n",
            buff_ProcessPath, strerror(errno));
  }
}

void GetSysInfo::getUSB_MassStorageDevice(std::vector<st_Hardware_lshw> &vecGetUSB_MassStorageDevice)
{

  char buff_ProcessPath[1024];
  getcwd(buff_ProcessPath, 1024);

#ifndef NONLS
  printf("%d\n", __LINE__);
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8");
  textdomain(PACKAGE);
#endif

  disable("isapnp");

  disable("output:json");
  disable("output:db");
  disable("output:xml");
  disable("output:html");
  disable("output:hwpath");
  disable("output:businfo");
  disable("output:X");
  disable("output:quiet");
  disable("output:sanitize");
  disable("output:numeric");
  enable("output:time");

  // define some aliases for nodes classes
  alias("disc", "disk");
  alias("cpu", "processor");
  alias("lan", "network");
  alias("net", "network");
  alias("video", "display");
  alias("sound", "multimedia");
  alias("modem", "communication");

  printf("proccer_path:");
  printf(buff_ProcessPath);
  std::vector<st_HardWare> rt_VecUSBHardWare;

  hwNode computer("computer",
                  hw::generic);

  scan_system(computer);

  int argc = 3;
  char *argv_all[] = {"process", "", ""};

  if (!parse_options(argc, argv_all))
  {
    printf("error\n");
    exit(1);
  }
  rt_VecUSBHardWare = print(computer, enabled("output:html"));
  cout << "size=" << rt_VecUSBHardWare.size();
  cout << "----network-->" << endl;
  for (std::vector<st_HardWare>::iterator it = rt_VecUSBHardWare.begin(); it != rt_VecUSBHardWare.end(); ++it)
  {
    //其他的打印机或者红外可以在这里加！
    if (it->description.empty() || (it->id.find("usb") == string::npos) || (it->description.find("Mass storage device") == string::npos))
    {
      //  cout<<"empty1"<<endl;
      continue;
    }

    st_Hardware_lshw t_USBH;
    // t_USBH.capacity=it->capacity;
    t_USBH.description = it->description;
    t_USBH.product = it->product;
    t_USBH.vendor = it->vendor;
    t_USBH.physical_id = it->physid;
    t_USBH.bus_info = it->businfo;
    //   cout<<"description:"<<it->description<<endl;
    //   cout<<"product:"<<it->product<<endl;
    //  cout<<"vendor:"<<it->vendor<<endl;
    //  cout<<"physid:"<<it->physid<<endl;
    //  cout<<"businfo:"<<it->businfo<<endl;

    //       string t_logicalname;
    //       // cout<<"logicalnames:";
    //        for(int i=0;i<it->logicalnames.size();i++)
    //        {
    //            // cout<<it->logicalnames[i]<<" ";
    //            t_logicalname.append(it->logicalnames[i]);
    //        }
    // t_USBH.logical_name=t_logicalname;
    t_USBH.version = it->version;
    t_USBH.serial = it->serial;

    //  cout<<"\n"<<"version:";
    //    cout<<it->version<<endl;
    //    cout<<"serial:";
    //    cout<<it->serial<<endl;

    //t_USBH.width=std::to_string( it->with);
    t_USBH.width.append(" bits");
    //cout<<"with:"<<it->with<<endl;

    // t_USBH.clock=it->Clock;
    //  cout<<"clock:"<<it->Clock <<endl;

    string t_capabilities;
    //cout<<"capabilities:";
    for (int i = 0; i < it->capabilities.size(); i++)
    {
      //   cout<<it->capabilities[i]<<" ";
      t_capabilities.append(it->capabilities[i]);
    }
    //  cout<<endl;
    t_USBH.capabilities = t_capabilities;

    string t_config;
    //  cout<<"configuration:";
    //  for(int i=0;i<it->config.size();i++)
    //  {
    //           //cout<<it->config[i]<<" ";
    //   t_config.append(it->config[i]);
    t_config = it->config;
    //  }
    //  cout<<endl;
    t_USBH.configuration = t_config;

    // string t_resources;
    //   //   cout<<"resources:";
    //      for(int i=0;i<it->resources.size();i++)
    //      {
    //              //  cout<< it->resources[i]<<" ";
    //              t_resources.append(it->resources[i]);
    //      }
    // //  cout<<endl;
    //               t_USBH.resources=t_resources;
    t_USBH.id = it->id;

    vecGetUSB_MassStorageDevice.push_back(t_USBH);
  }

  desotry(); //清楚静态对象

  char buff_ProcessPathCheck[1024];
  getcwd(buff_ProcessPathCheck, 1024);
  int ret_Chdir;
  if (strcmp(buff_ProcessPath, buff_ProcessPathCheck) != 0)
    ret_Chdir = chdir(buff_ProcessPath);

  if (ret_Chdir == -1)
  {
    fprintf(stderr, "Failed to open %s\n"
                    "The reason *may* have been %s\n",
            buff_ProcessPath, strerror(errno));
  }
}

void GetSysInfo::getNetWorkHardWare(std::vector<st_Hardware_lshw> &get_Network)
{

  char buff_ProcessPath[1024];
  getcwd(buff_ProcessPath, 1024);

#ifndef NONLS
  printf("%d\n", __LINE__);
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8");
  textdomain(PACKAGE);
#endif

  disable("isapnp");

  disable("output:json");
  disable("output:db");
  disable("output:xml");
  disable("output:html");
  disable("output:hwpath");
  disable("output:businfo");
  disable("output:X");
  disable("output:quiet");
  disable("output:sanitize");
  disable("output:numeric");
  enable("output:time");

  // define some aliases for nodes classes
  alias("disc", "disk");
  alias("cpu", "processor");
  alias("lan", "network");
  alias("net", "network");
  alias("video", "display");
  alias("sound", "multimedia");
  alias("modem", "communication");

  printf("proccer_path:");
  printf(buff_ProcessPath);

  std::vector<st_HardWare> rt_VecHardWare;
  std::vector<st_HardWare> rt_VecInPutHardWare;
  st_HardWare rt_hardWare;
  //   disable("output:sanitize");
  //  disable("output:html");
  //  disable("output:numeric");
  // enable("output:time");

  printf("----\n");
  hwNode computer("computer",
                  hw::generic);

  scan_system(computer);

  int argc = 3;
  // char * argv_print[]={"process","",""};
  char *argv[] = {"process", "-class", "network"};
  if (!parse_options(argc, argv))
  {
    printf("error\n");
    exit(1);
  }
  rt_VecHardWare = print(computer, enabled("output:html"));
  cout << "size=" << rt_VecHardWare.size();
 cout<<"[line]="<<__LINE__<<"[fn]="<<"getNetWorkHardWare"<<endl;//debug

  for (std::vector<st_HardWare>::iterator it = rt_VecHardWare.begin(); it != rt_VecHardWare.end(); ++it)
  {
    if (it->description.empty())
    {
      //  cout<<"empty1"<<endl;
      continue;
    }

    st_Hardware_lshw t_network;
    t_network.capacity = it->capacity;
    t_network.description = it->description;
    t_network.product = it->product;
    t_network.vendor = it->vendor;
    t_network.physical_id = it->physid;
    t_network.bus_info = it->businfo;
    //   cout<<"description:"<<it->description<<endl;
    //   cout<<"product:"<<it->product<<endl;
    //  cout<<"vendor:"<<it->vendor<<endl;
    //  cout<<"physid:"<<it->physid<<endl;
    //  cout<<"businfo:"<<it->businfo<<endl;

    string t_logicalname;
    // cout<<"logicalnames:";
    for (int i = 0; i < it->logicalnames.size(); i++)
    {
      // cout<<it->logicalnames[i]<<" ";
      t_logicalname.append(it->logicalnames[i]);
    }
    t_network.logical_name = t_logicalname;
    t_network.version = it->version;
    t_network.serial = it->serial;

    //  cout<<"\n"<<"version:";
    //    cout<<it->version<<endl;
    //    cout<<"serial:";
    //    cout<<it->serial<<endl;

    t_network.width = std::to_string(it->with);
    t_network.width.append(" bits");
    //cout<<"with:"<<it->with<<endl;

    t_network.clock = it->Clock;
    //  cout<<"clock:"<<it->Clock <<endl;

    string t_capabilities;
    //cout<<"capabilities:";
    for (int i = 0; i < it->capabilities.size(); i++)
    {
      //   cout<<it->capabilities[i]<<" ";
      t_capabilities.append(it->capabilities[i]);
    }
    //  cout<<endl;
    t_network.capabilities = t_capabilities;

    string t_config;
    //  cout<<"configuration:";
    //  for(int i=0;i<it->config.size();i++)
    //  {
    //cout<<it->config[i]<<" ";
    //   t_config.append(it->config[i]);
    t_config = it->config;
    // }
    //  cout<<endl;
    t_network.configuration = t_config;

    string t_resources;
    //   cout<<"resources:";
    for (int i = 0; i < it->resources.size(); i++)
    {
      //  cout<< it->resources[i]<<" ";
      t_resources.append(it->resources[i]);
    }
    //  cout<<endl;
    t_network.resources = t_resources;
    t_network.id = it->id;

    get_Network.push_back(t_network);
  }

  cout<<"[line]="<<__LINE__<<"[fn]="<<"getNetWorkHardWare"<<endl;//debug

  struct ifaddrs *ifAddrStruct = NULL;
  void *tmpAddrPtr = NULL;
  void *tmpNetMaskAddrPtr = NULL;
  getifaddrs(&ifAddrStruct);
  char IPV4addressBuffer[INET_ADDRSTRLEN];
  char maskAddressBuffer[INET_ADDRSTRLEN];
  char IPV6addressBuffer[INET6_ADDRSTRLEN];
   string t_ifname;
  while (ifAddrStruct != NULL)
  {
    if (ifAddrStruct->ifa_addr->sa_family == AF_INET)
    { // check it is IP4
      // is a valid IP4 Address
      tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
      tmpNetMaskAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_netmask)->sin_addr;
      memset(IPV4addressBuffer, INET_ADDRSTRLEN, 0x00);
      memset(IPV4addressBuffer, INET_ADDRSTRLEN, 0x00);
      memset(maskAddressBuffer, INET_ADDRSTRLEN, 0x00);
      inet_ntop(AF_INET, tmpAddrPtr, IPV4addressBuffer, INET_ADDRSTRLEN);
      inet_ntop(AF_INET, tmpNetMaskAddrPtr, maskAddressBuffer, INET_ADDRSTRLEN);
     // printf("%s IP4 Address %s\n", ifAddrStruct->ifa_name, IPV4addressBuffer);//同一个网卡多个ＩＰ可能要处理 要加ＤＮＳ;DHCP
    //  printf("%s IP MaskIP %s\n", ifAddrStruct->ifa_netmask, maskAddressBuffer);
t_ifname=ifAddrStruct->ifa_name;
    for (int i=0;i<get_Network.size();i++)
      {


      string t_netname=get_Network[i].logical_name;
               if((t_netname==t_ifname))
                {
                
                     get_Network[i].IPV4=IPV4addressBuffer;
                   
                           get_Network[i].MaskIP=maskAddressBuffer;
  cout<<__LINE__<<endl;
printf(" get_Network[%d].IPV4=%s\n",i,get_Network[i].IPV4.c_str());
 
printf(" get_Network[%d].MaskIP=%s\n",i,get_Network[i].MaskIP.c_str());
                
      }
    }

    }
    else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6)
    { // check it is IP6
      // is a valid IP6 Address
      tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
      memset(IPV6addressBuffer, INET6_ADDRSTRLEN, 0x00);
      inet_ntop(AF_INET6, tmpAddrPtr, IPV6addressBuffer, INET6_ADDRSTRLEN);
      //printf("%s IP6 Address %s\n", ifAddrStruct->ifa_name, IPV6addressBuffer);
             t_ifname=ifAddrStruct->ifa_name;



                 for (int i=0;i<get_Network.size();i++)
      {


      string t_netname=get_Network[i].logical_name;
               if((t_netname==t_ifname))
                {
                
                      
                          get_Network[i].IPV6=IPV6addressBuffer;
                           get_Network[i].MaskIP=maskAddressBuffer;
  cout<<__LINE__<<endl;
 
printf(" get_Network[%d].IPV6=%s\n",i,get_Network[i].IPV6.c_str());
printf(" get_Network[%d].MaskIP=%s\n",i,get_Network[i].MaskIP.c_str());
                
      }
    }
     
    }

      ifAddrStruct = ifAddrStruct->ifa_next;
  }


//获取操作系统DNS
     struct __res_state res;
    res.options &= ~ RES_INIT;

    int err = res_ninit(&res);
    if (err) {
        fprintf(stderr, "res_init error: %d\n", err);
       // return err;
    }

    char ip[16];
    for(int i = 0 ; i < res.nscount; ++i) {
        ip[0] = '\0';
        if (! inet_ntop(AF_INET, &res.nsaddr_list[i].sin_addr, ip, sizeof(ip))) {
            perror("inet_ntop");
            continue;
        }
        printf("dns: %s\n", ip);
        get_Network[i].dns.append(ip);
   
    }


    res_nclose(&res);

 
    // cout<<"-<<---network---\n\n\n"<<endl;
cout<<"[line]="<<__LINE__<<"[fn]="<<"getNetWorkHardWare"<<endl;//debug
    remove_options("network");
    desotry(); //清楚静态对象

    char buff_ProcessPathCheck[1024];
    getcwd(buff_ProcessPathCheck, 1024);
    int ret_Chdir;
    if (strcmp(buff_ProcessPath, buff_ProcessPathCheck) != 0)
      ret_Chdir = chdir(buff_ProcessPath);

    if (ret_Chdir == -1)
    {
      fprintf(stderr, "Failed to open %s\n"
                      "The reason *may* have been %s\n",
              buff_ProcessPath, strerror(errno));
    }
   
}

string GetSysInfo::splite_Item(const string src, const char *key, const char *eof, char **result)
{

  string rt_Str;
  if (src.empty())
  {
    return rt_Str;
  }

  string::size_type posKey;
  posKey = src.find(key);
  if (posKey == string::npos)
  {
    //cout<<"src="<<src<<"key="<<key<<"sPosKey=string::npos"<<endl;
    *result = NULL;
    return rt_Str;
  }

  int len_Key = strlen(key);
  //cout<<"len_Key="<<len_Key;
  string sub1 = src.substr(posKey + len_Key);
  //cout<<"sub1:"<<sub1<<endl;

  string::size_type postEof = sub1.find(eof);
  //cout<<"poseEof="<<postEof;

  if (eof == "end")
  {
    *result = new char[sub1.length() + 1];
    strcpy(*result, sub1.c_str());

    //cout<<"debug: "<<*result<<endl;

    //sub1.erase(sub1.find_last_of('\n'), string::npos);    //去除回车
    //sub1.erase(sub1.find_last_of('\n'), string::npos);    //去除回车

    return sub1;
  }

  if (postEof == string::npos)
  {

    cout << __LINE__ << "  sub1=" << sub1 << endl;
    //rt_Str=sub1;

    return rt_Str;
  }

  string sub2 = sub1.substr(0, postEof);

  *result = new char[sub2.length() + 1];
  strcpy(*result, sub2.c_str());
  //cout<<"debug: "<<*result<<endl;

  return sub2;
}

string GetSysInfo::GetStdoutFromCommand(string cmd, int &count_Line)
{

  count_Line = 0;
  string data;
  FILE *stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  //cmd.append(" 2>dev/null \" ");

  //cout<<"cmd.c_str()="<<cmd.c_str()<<endl; getchar();
  stream = popen(cmd.c_str(), "r");

  if (stream == NULL)
  {
    printf("Error command  Failed: %s  line=%d\n", strerror(errno), __LINE__);
    return data;
  }
  else
  {

    if (stream)
    {
      while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL)
        {
          data.append(buffer);
          count_Line++;
        }
      pclose(stream);
    }
    return data;
  }
}

void GetSysInfo::GetPackateName(const string st_InstalledPackage)
{

  string t_String;
}

//字符串分割函数
vector<string> GetSysInfo::splitGetName(string str, string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> st_Splite1;
  std::vector<std::string> st_Splite2;
  str += pattern; //扩展字符串以方便操作
                  // cout<<"str="<<str<<"\n press ENTER"<<endl; getchar(); //debug
  int size = str.size();

  for (int i = 0; i < size; i++)
  {
    pos = str.find(pattern, i);
    if (pos < size)
    {
      string s = str.substr(i, pos - i);

      st_Splite1.push_back(s);
      i = pos + pattern.size() - 1;
    }
  }

  for (int i = 0; i < st_Splite1.size(); i++)
  {
    pos = st_Splite1[i].find("\n");
    if (pos == string::npos)
    {

      //cout<<"!!  pos= npos!! "<<endl;
      //cout<<"st_Splite1["<<i<<"]="<<st_Splite1[i]<<endl;
    }
    // cout<<"pos="<<pos<<endl;
    string get_Name = st_Splite1[i].substr(pos + 1);
    // cout<<"get_Name="<<get_Name<<endl;

    if (!get_Name.empty())
    {
      st_Splite2.push_back(get_Name);
    }
  }

  return st_Splite2;
}

/*

作用:  获得操作系统所有用户名称，返回vector容器
 */
vector<string> GetSysInfo::getAllUserName()
{

  //get all user
  FILE *out = NULL;
  char buffer[1024];
  memset(&buffer, 0x00, sizeof(buffer));
  int n_Size = 0;

  const char *c_Command = "exec bash -c \"eval getent passwd {$(awk \'/^UID_MIN/ {print $2}\' /etc/login.defs)..$(awk \'/^UID_MAX/ {print $2}\' /etc/login.defs)} | wc -l | awk 1 ORS=\'\'\" "; //get coutn of list

  // cout<<"c_Command="<<c_Command<<"    press getchar()"<<endl;getchar();  //debug
  out = popen(c_Command, "r"); //get count of normal user
  //这句ubuntu在Ubuntu运行会得到空，不知道为何？
  // out = popen("eval getent passwd {$(awk '/^UID_MIN/ {print $2}' /etc/login.defs)..$(awk '/^UID_MAX/ {print $2}' /etc/login.defs)}  | cut -d: -f1 | wc -l | awk 1 ORS=''", "r");

  if (out == NULL)
  {
    printf("Error opening file unexist.ent: %s ,line=%d\n", strerror(errno), __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    //   cout<<"buffer:"<<buffer<<endl;
    n_Size = atoi(buffer);
    //    printf("debug, line=%d  , n_Size=%d  press Enter\n",__LINE__,n_Size);getchar();//debug
    memset(&buffer, 0x00, sizeof(buffer));
    pclose(out);
    out = NULL;
  }
  out = popen("getent passwd 0 | wc -l | awk 1 ORS=''", "r"); //get count of root user

  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    //cout<<"buffer:"<<buffer<<endl;
    n_Size += atoi(buffer);
    //printf("debug, line=%d  , n_Size=%d\n",__LINE__,n_Size);
    memset(&buffer, 0x00, sizeof(buffer));
    pclose(out);
    out = NULL;
  }

  //  out=popen("fdisk -l | grep 'Disk model' | wc -l | sed 's/[[:blank:]]*//' | awk 1 ORS=' ' ","r");
  // out = popen("chage -l $USER   | awk 1 ORS=' '", "r");
  char t_Command[1000];
  for (int i = 1; i < n_Size; i++)
  {
    //printf("i=%d, n_Size + 1=%d\n",i,n_Size);
    //  sprintf(t_Command, "eval getent passwd {$(awk '/^UID_MIN/ {print $2}' /etc/login.defs)..$(awk '/^UID_MAX/ {print $2}' /etc/login.defs)} | cut -d: -f1 | sed -n %dp | awk 1 ORS=''", i);//this satement don't work in ubuntu /bin/sh

    sprintf(t_Command, " exec bash -c \"eval getent passwd {$(awk '/^UID_MIN/ {print $2}' /etc/login.defs)..$(awk '/^UID_MAX/ {print $2}' /etc/login.defs)} | cut -d: -f1 | sed -n %dp | awk 1 ORS=''\" ", i);

    //cout<<"debug: t_Command="<<t_Command<<endl;
    out = popen(t_Command, "r"); //Get User Name with pipe
    fgets(buffer, sizeof(buffer), out);
    if (out == NULL)
    {
      printf("%d  peopen call fail, error\n", __LINE__);
      continue;
    }
    else
    {
      fgets(buffer, sizeof(buffer), out);
      // cout<<"debug:get name, buffer="<<buffer<< "  press Enter to continue"<<endl;getchar();//debug
      // usleep(100);
      //   m_UserName[i] = buffer;
      m_UserName.push_back(buffer);

      memset(&buffer, 0x00, sizeof(buffer));
      memset(&t_Command, 0x00, sizeof(t_Command));
      pclose(out);
      out = NULL;
    }
  }
  //Get whether has root account
  out = popen("grep -i root /etc/passwd | awk 1 ORS=''", "r");

  if (out == NULL)
  {
    printf("%d  peopen call fail, error\n", __LINE__);
  }
  else
  {
    fgets(buffer, sizeof(buffer), out);
    if (buffer == "\n")
    {
      printf("Current OS has no 'root' account\n");
    }
    else
    {
      m_UserName.push_back("root");
    }
  }
  //---<end: get user list

  return m_UserName;
}

GetSysInfo::GetSysInfo()
{
  cout << __LINE__ << endl; //debug

  m_UserName = getAllUserName();
}

int GetSysInfo::IsFolderExist(const char *path)
{
  DIR *dp;
  if ((dp = opendir(path)) == NULL)
  {
    return 0;
  }

  closedir(dp);
  return -1;
}

//检查文件(所有类型)是否存在
//1:存在 0:不存在
int GetSysInfo::IsFileExist(const char *path)
{
  return !access(path, F_OK);
}

unsigned long long getSCSI_DriverCapacity(string devicePath)
{
  int fd, i;
  /* READ_CAPACITY command block */
  unsigned char RCAP_CmdBlk[RCAP_COMMAND_LEN] =
      {RCAP_COMMAND, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  //	sg_io_hdr_t sg_io;
  struct sg_io_hdr sg_io;
  unsigned char rcap_buff[RCAP_REPLY_LEN];
  unsigned int lastblock, blocksize;
  unsigned long long disk_cap;
  unsigned char sense_buf[32];

  /* Open the sg device */
  if ((fd = open(devicePath.c_str(), O_RDONLY)) < 0)
  {
    printf("Bad Open devicePath.c_str() \n");
    return -1;
    //exit(1);
  }

  /* Initialize */
  memset(&sg_io, 0, sizeof(sg_io_hdr_t));

  /* Command block address and length */
  sg_io.cmdp = RCAP_CmdBlk;
  sg_io.cmd_len = RCAP_COMMAND_LEN;

  /* Response buffer address and length */
  sg_io.dxferp = rcap_buff;
  sg_io.dxfer_len = RCAP_REPLY_LEN;

  /* Sense buffer address and length */
  sg_io.sbp = sense_buf;
  sg_io.mx_sb_len = sizeof(sense_buf);
  /* Control information */
  sg_io.interface_id = 'S';
  sg_io.dxfer_direction = SG_DXFER_FROM_DEV;
  sg_io.timeout = 10000; /* 10 seconds */

  /* Issue the SG_IO ioctl */
  if (ioctl(fd, SG_IO, &sg_io) < 0)
  {
    printf("Bad SG_IO\n");
    return -1;
    //exit(1);
  }

  /* Obtain results */
  if ((sg_io.info & SG_INFO_OK_MASK) == SG_INFO_OK)
  {
    /* Address of last disk block */
    lastblock = ((rcap_buff[0] << 24) | (rcap_buff[1] << 16) |
                 (rcap_buff[2] << 8) | (rcap_buff[3]));

    /* Block size */
    blocksize = ((rcap_buff[4] << 24) | (rcap_buff[5] << 16) |
                 (rcap_buff[6] << 8) | (rcap_buff[7]));

    /* Calculate disk capacity */
    disk_cap = (lastblock + 1);
    disk_cap *= blocksize;
    //printf("Disk Capacity = %llu Bytes\n", disk_cap);
    return disk_cap;
  }
  close(fd);
}

string addStringToHead(const string str_source, const char *add)
{

  if (str_source.empty())
  {
    return str_source;
  }
  string t_Head(add);

  t_Head.append(str_source);
  return t_Head;
}

char *kscale(unsigned long b, unsigned long bs)
{
  unsigned long long size = b * (unsigned long long)bs;
  if (size > G)
  {
    sprintf(str, "%0.2f GB", size / (G * 1.0));
    return str;
  }
  else if (size > M)
  {
    sprintf(str, "%0.2f MB", size / (1.0 * M));
    return str;
  }
  else if (size > K)
  {
    sprintf(str, "%0.2f K", size / (1.0 * K));
    return str;
  }
  else
  {
    sprintf(str, "%0.2f B", size * 1.0);
    return str;
  }
}

int exist(const char *fname)
{
  FILE *file;
  if ((file = fopen(fname, "r")))
  {
    fclose(file);
    return 1;
  }
  return 0;
}

unsigned char *show_vendor(struct sg_io_hdr *hdr)
{
  unsigned char *buffer = (unsigned char *)hdr->dxferp; //takethat:做了强转
  int i;
  // printf("vendor id:");
  // for (i=8; i<16; ++i) {
  // 	putchar(buffer[i]);
  // }
  // putchar('\n');

  unsigned char take_vendor[8] = {0};
  strncpy((char *)take_vendor, (const char *)buffer + 8, 8);
  //printf("[take_id]%s\n",take_vendor);
  return take_vendor;
}

unsigned char *show_product(struct sg_io_hdr *hdr)
{
  unsigned char *buffer = (unsigned char *)hdr->dxferp;
  int i;
  // printf("product id:");
  // for (i=16; i<32; ++i) {
  // 	putchar(buffer[i]);
  // }
  //	putchar('\n');

  unsigned char take_productId[32] = {0};
  strncpy((char *)take_productId, (const char *)buffer + 16, 16);
  //printf("[take_productId]%s\n",take_productId);

  return take_productId;
}

unsigned char *show_product_rev(struct sg_io_hdr *hdr)
{
  unsigned char *buffer = (unsigned char *)hdr->dxferp;
  int i;
  // printf("product ver:");
  // for (i=32; i<36; ++i) {
  // 	putchar(buffer[i]);
  // }
  // putchar('\n');

  unsigned char take_product_rev[4] = {0};
  strncpy((char *)take_product_rev, (const char *)buffer + 32, 4);
  //printf("[take_product_rev]%s\n",take_product_rev);

  return take_product_rev;
}

/***************************************************************************
 * name: execute_Inquiry
 * parameter:
 * 		fd:			file descripter
 * 		page_code:	cdb page code
 * 		evpd:			cdb evpd
 * 		p_hdr:		poiter to sg_io_hdr struct
 * function: make Inquiry cdb and execute it.
 * **************************************************************************/
int execute_Inquiry(int fd, int page_code, int evpd, struct sg_io_hdr *p_hdr)
{
  unsigned char cdb[10];

  /* set the cdb format */
  cdb[0] = 0x12; /*This is for Inquery*/
  cdb[1] = evpd & 1;
  cdb[2] = page_code & 0xff;
  cdb[3] = 0;
  cdb[4] = 0xff;
  cdb[5] = 0; /*For control filed, just use 0*/
  cdb[6] = 0;
  cdb[7] = 0;
  cdb[8] = 0;
  cdb[9] = 0;

  p_hdr->dxfer_direction = SG_DXFER_FROM_DEV;
  p_hdr->cmdp = cdb;
  p_hdr->cmd_len = 6;

  int ret = ioctl(fd, SG_IO, p_hdr);
  if (ret < 0)
  {
    printf("Sending SCSI Command failed.\n");
    close(fd);
    exit(1);
  }

  return p_hdr->status;
}

/***************************************************************************
 * name: init_io_hdr
 * parameter:
 * function: initialize the sg_io_hdr struct fields with the most common
 * 			 value
 * **************************************************************************/
struct sg_io_hdr *init_io_hdr()
{

  struct sg_io_hdr *p_scsi_hdr = (struct sg_io_hdr *)malloc(sizeof(struct sg_io_hdr));
  memset(p_scsi_hdr, 0, sizeof(struct sg_io_hdr));
  if (p_scsi_hdr)
  {
    p_scsi_hdr->interface_id = 'S';          /* this is the only choice we have! */
    p_scsi_hdr->flags = SG_FLAG_LUN_INHIBIT; /* this would put the LUN to 2nd byte of cdb*/
  }

  return p_scsi_hdr;
}

void show_sense_buffer(struct sg_io_hdr *hdr)
{
  unsigned char *buffer = hdr->sbp;
  int i;
  for (i = 0; i < hdr->mx_sb_len; ++i)
  {
    putchar(buffer[i]);
  }
}

void destroy_io_hdr(struct sg_io_hdr *p_hdr)
{
  if (p_hdr)
  {
    free(p_hdr);
  }
}

void set_xfer_data(struct sg_io_hdr *p_hdr, void *data, unsigned int length)
{
  if (p_hdr)
  {
    p_hdr->dxferp = data;
    p_hdr->dxfer_len = length;
  }
}

void set_sense_data(struct sg_io_hdr *p_hdr, unsigned char *data,
                    unsigned int length)
{
  if (p_hdr)
  {
    p_hdr->sbp = data;
    p_hdr->mx_sb_len = length;
  }
}

int hostname_to_ip(const char *hostname, char *ip)
{
  struct hostent *he;
  struct in_addr **addr_list;
  int i;
  if ((he = gethostbyname(hostname)) == NULL)
  {
    // gain the host info
    //herror("gethostbyname");
    cout << "Count not connect " << hostname << endl;
    return -1;
  }
  addr_list = (struct in_addr **)he->h_addr_list;
  for (i = 0; addr_list[i] != NULL; i++)
  {
    // virtue the number one one;
    strcpy(ip, inet_ntoa(*addr_list[i]));
    return 0;
  }
  return 1;
}

LINKDLL
{
  //����ʵ������
  std::shared_ptr<void> CreateInstance(void)
  {
    return std::make_shared<GetSysInfo>();
  }
}
