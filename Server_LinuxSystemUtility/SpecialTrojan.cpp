//#include "stdafx.h"
//#pragma execution_character_set("utf-8")
#include "SpecialTrojan.h"

#include "SpecialTrojanServer.h"
#include "jsoncpp/json/json.h"
#include <semaphore.h>
#include"PublicData.h"
#define PATH_MAX 4096
string pwdProcess();
bool  recyRemveDelimitStr(string &str);
using namespace std;

bool bFirstRun = true;
bool bWriteLog = false;
bool bOnXinXi = false;
bool bFirstSendMessage = true;


bool gbHaveOnceScan = false;
bool gbCanTrojanScan = false;
bool gbCanFileSystem=false;

//HANDLE ghListStringEvent;
sem_t sem;
void* TQViewThread(void *pParam);
void SetListString(string strInfo, int iColor = 0);

string libPath= pwdProcess()+"libGetsystemInfo.so";

//libPath+=;
 static std::shared_ptr< GetSysInfoImpl>ptr_SysInfo=toGetDynamicPtr(libPath.c_str());

// static std::shared_ptr< GetSysInfoImpl>ptr_SysInfo=toGetDynamicPtr("./libGetsystemInfo.so");


struct TQListString
{
    string strInfo;
	int iColor;
};

vector<TQListString*> vecListString;

void SetListString(string strInfo, int iColor)
{
  while(!recyRemveDelimitStr(strInfo));//take:删除str里的回车,不然client显示不全

    TQListString* pPush = new TQListString;
    pPush->strInfo = strInfo;
    pPush->iColor = iColor;
  //  WaitForSingleObject(ghListStringEvent, INFINITE);//###为了同步一个push_back完成后另一个才能push_back
    sem_wait(&sem);
    vecListString.push_back(pPush);
    //SetEvent(ghListStringEvent);//###通知下一个
    sem_post(&sem);
}


void ViewProcessInfo(void* pViewInfo)
{



    Json::Value info;
    info["msg"] = "ClearProcessInfo";
  //  SendMessageToClient(info.toStyledString());


    info["msg"] = "ProcessInfo";
    info["ProcessName"] = "sfsf.exe";
    info["PID"] = "343243";
    info["PPID"] = "634534";
    info["IsAuth"] = "待认证";
    info["Path"] = "c:\\gsdfe.exe";
    info["Color"] = 1;
  //  SendMessageToClient(info.toStyledString());


    info.clear();
    info["msg"] = "ProcessInfo";
    info["ProcessName"] = "ggggg.exe";
    info["PID"] = "3433243";
    info["PPID"] = "6344534";
    info["IsAuth"] = "待认证";
    info["Path"] = "c:\\windows\\g33sdfe.exe";
    info["Color"] = 1;
  //  SendMessageToClient(info.toStyledString());

}

void ViewHistoryProcessInfo(void* pViewInfo)
{
    Json::Value info;
    info["msg"] = "ClearHistoryProcessInfo";
    //SendMessageToClient(info.toStyledString());


    info["msg"] = "HistoryProcessInfo";
    info["ProcessName"] = "gsdfe.exe";
    info["PID"] = "34234";
    info["PPID"] = "7777";
    info["IsAuth"] = "通过";
    info["Path"] = "c:\\gggg.exe";
    info["IsHide"] = "否";
    info["Color"] = 0;
    //SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "HistoryProcessInfo";
    info["ProcessName"] = "gsdrt5fe.exe";
    info["PID"] = "3456234";
    info["PPID"] = "77577";
    info["IsAuth"] = "通过";
    info["Path"] = "c:\\gggfrrrg.exe";
    info["IsHide"] = "否";
    info["Color"] = 0;
    //SendMessageToClient(info.toStyledString());
}

void ViewDriverInfo(void* pViewInfo)
{
    Json::Value info;
    info["msg"] = "ClearDriverInfo";
    //SendMessageToClient(info.toStyledString());

    info["msg"] = "DriverInfo";
    info["DriverPath"] = "C:\\windows\\system32\\Drivers\\gg.sys";
    info["BaseAddress"] = "0x744535";
    info["Size"] = "5435345";
    info["IsHide"] = "否";
    info["IsAuth"] = "通过";
    info["Color"] = 0;
  //  SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "DriverInfo";
    info["DriverPath"] = "C:\\windows\\\\Drivers\\gxxx.sys";
    info["BaseAddress"] = "0x7445535";
    info["Size"] = "5438845";
    info["IsHide"] = "否";
    info["IsAuth"] = "通过";
    info["Color"] = 0;
    //SendMessageToClient(info.toStyledString());
}

void ViewAntorunInfo(void* pViewInfo)
{

    Json::Value info;
    info["msg"] = "ClearAutorunInfo";
    //SendMessageToClient(info.toStyledString());

    info["msg"] = "AutoRunInfo";
    info["RunItem"] = "gggsdfe";
    info["RegPath"] = "c:\\ggg.exe";
    info["FilePath"] = "c:\\ggg.exe";
    info["IsAuth"] = "通过";
    info["Describe"] = "配置开机级别";
    info["Company"] = "microsoft";
    //SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "AutoRunInfo";
    info["RunItem"] = "g111e";
    info["RegPath"] = "c:\\ggg\\.exe";
    info["FilePath"] = "c:\\gxxxxgg.exe";
    info["IsAuth"] = "通过";
    info["Describe"] = "配置电源管理";
    info["Company"] = "microsoft";
    //SendMessageToClient(info.toStyledString());
}

void ViewMessageInfo(void* pViewInfo)
{
    Json::Value info;
    info["msg"] = "ClearMessageHookInfo";
    //SendMessageToClient(info.toStyledString());

    info["msg"] = "MessageHookInfo";
    info["Handle"] = "34343";
    info["HookType"] = "inline";
    info["HookFuncAddr"] = "0x4534534543";
    info["PID"] = "34234";
    info["TID"] = "234234";
    info["HookModule"] = "C:\\windows\\ddd.exe";
    info["ProcessPath"] = "C:\\windows\\ggg.dll";
    //SendMessageToClient(info.toStyledString());


    info["msg"] = "MessageHookInfo";
    info["Handle"] = "34366643";
    info["HookType"] = "iat";
    info["HookFuncAddr"] = "0x4534543";
    info["PID"] = "34234";
    info["TID"] = "234234";
    info["HookModule"] = "C:\\windows\\google.exe";
    info["ProcessPath"] = "C:\\windows\\ms.dll";
    //SendMessageToClient(info.toStyledString());
}

void ViewPortInfo(void* pViewInfo)
{
    Json::Value info;
    info["msg"] = "ClearPortInfo";
    //SendMessageToClient(info.toStyledString());


    info["msg"] = "PortInfo";
    info["Protocol"] = "TCP";
    info["LocalAddr"] = "192.168.6.110";
    info["RemoteAddr"] = "192.168.6.220";
    info["Status"] = "listion";
    info["PID"] = "222";
    info["ProcessPath"] = "C:\\gsfsfsf.exe";
    //SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "PortInfo";
    info["Protocol"] = "TCP";
    info["LocalAddr"] = "192.168.6.100";
    info["RemoteAddr"] = "192.168.6.200";
    info["Status"] = "established";
    info["PID"] = "222";
    info["ProcessPath"] = "C:\\gsfsfsf.exe";
    //SendMessageToClient(info.toStyledString());

}

void ViewSsdtInfo(void* pViewInfo)
{
    Json::Value info;
    info["msg"] = "ClearSSDTInfo";
    //SendMessageToClient(info.toStyledString());


    info["msg"] = "SSDTInfo";
    info["Num"] = "22";
    info["FuncName"] = "ZwCreateFile";
    info["OriginalAddr"] = "0x7ff36345";
    info["NowAddr"] = "0x7ff36345";
    info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";
    info["InlineHookModule"] = "";
    //SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "SSDTInfo";
    info["Num"] = "232";
    info["FuncName"] = "ZwWriteFIle";
    info["OriginalAddr"] = "0x7ff36345";
    info["NowAddr"] = "0x7ff36345";
    info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";
    info["InlineHookModule"] = "";
    //SendMessageToClient(info.toStyledString());
}

void ViewShadowSsdtInfo(void* pViewInfo)
{

    Json::Value info;
    info["msg"] = "ClearShadowSSDTInfo";
    //SendMessageToClient(info.toStyledString());


    info["msg"] = "ShadowSSDTInfo";
    info["Num"] = "44";
    info["NowAddr"] = "0x7ff36345";
    info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";

    info.clear();
    info["msg"] = "ShadowSSDTInfo";
    info["Num"] = "77";
    info["NowAddr"] = "0x7ff77345";
    info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";

    //SendMessageToClient(info.toStyledString());
}

void ViewIdtInfo(void* pViewInfo)
{

    Json::Value info;
    info["msg"] = "ClearIDTInfo";
    //SendMessageToClient(info.toStyledString());

    info["msg"] = "IDTInfo";
    info["Num"] = "46454";
    info["NowAddr"] ="56756756";
    info["Module"] = "C:\\Windows\\system32\\ggg.exe";
    //SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "IDTInfo";
    info["Num"] = "4655454";
    info["NowAddr"] = "5675000";
    info["Module"] = "C:\\Windows\\system32\\telnet.exe";
    //SendMessageToClient(info.toStyledString());
}

void ViewIatInfo(void* pViewInfo)
{

    Json::Value info;
    info["msg"] = "ClearIATInfo";
    //SendMessageToClient(info.toStyledString());


    info["msg"] = "IATInfo";
    info["ProcessPath"] = "C:\\windows\sdf.exe";
    info["PID"] = "3434";
    info["OriginalAddr"] = "0x45755354";
    info["JumpAddr"] = "0x543454264";
    info["IatHookFunc"] = "CreateFile";
    info["IatHookModule"] = "sfsdfsdfs.exe";
    //SendMessageToClient(info.toStyledString());

    info.clear();
    info["msg"] = "IATInfo";
    info["ProcessPath"] = "C:\\windows\tim.exe";
    info["PID"] = "377434";
    info["OriginalAddr"] = "0x457555654";
    info["JumpAddr"] = "0x543884264";
    info["IatHookFunc"] = "ReadFile";
    info["IatHookModule"] = "hide.dll";
    //SendMessageToClient(info.toStyledString());

}

bool ViewScanFinishInfo(void* pViewInfo)
{
    Json::Value info;
    info["msg"] = "ClearScanFinishInfo";
    //SendMessageToClient(info.toStyledString());

    info["msg"] = "ScanTrojanResult";
    info["Num"] ="33";
    info["TrojanInfo"] = "所在进程PId:22[explorer.exe], 模块 : c : \\windows\\gg.dll - 危险程度:250\n";
    //SendMessageToClient(info.toStyledString());

    return true;
}

void* TQListStringThread(void* pParam)
{
    while (1)
    {
        if (gbCanTrojanScan && !bOnXinXi)//###信息处理完就通知到这里gbCanTrojanScan=true开始执行
        {
            if (bFirstSendMessage)
            {
                ViewProcessInfo(NULL);
                ViewHistoryProcessInfo(NULL);
                ViewIatInfo(NULL);
                ViewPortInfo(NULL);

                ViewDriverInfo(NULL);
                ViewAntorunInfo(NULL);
                ViewMessageInfo(NULL);
                ViewSsdtInfo(NULL);
                ViewShadowSsdtInfo(NULL);
                ViewIdtInfo(NULL);
                bOnXinXi = true;
                bFirstSendMessage = false;

                Json::Value info;//###通知前台首次发送全部信息完成，接下来只发送实时进程和端口信息
                info["msg"] = "FirstSendMessage";
                //SendMessageToClient(info.toStyledString());

                info.clear();
                info["msg"] = "NetInfo";
                info["RealTimeNetInfo"] = "PID为553的进程域名解析地址:www.baidu.com, 时间 : 2019.4.22 08:33:20";
                //SendMessageToClient(info.toStyledString());


                info.clear();
                info["msg"] = "MonitorInfo";
                info["RealTimeMonitorInfo"] = "PId:343[iexplorer.exe], 建立PE格式文件, 文件路径为c:\\windows\temp.exe, 时间为2019.9.24 09:34:20";
                //SendMessageToClient(info.toStyledString());
                ViewScanFinishInfo(NULL);
            }
            else
            {

                ViewProcessInfo(NULL);
                ViewPortInfo(NULL);
                bOnXinXi = true;
            }
        }

        //###界面解析vecListString服务器传过来的信息
        Json::Value info;
        sem_wait(&sem);
        //WaitForSingleObject(ghListStringEvent, INFINITE);
        int iColor = 0;
        for (int i = 0; i<vecListString.size(); i++)
        {
            TQListString* pPush = vecListString[i];
            if (pPush->iColor == 1)
            {
                iColor = 1;
            }
            if (pPush->iColor == 2)
            {
                iColor = 3;
            }

             info["msg"] = "USBHistoryEvents";
            info["ScanInfoDescribe"] = pPush->strInfo.c_str();

            char sColor[10] = { 0 };
            sprintf(sColor,"%d",iColor);
            //itoa(iColor, sColor, 10);
            info["Color"] = sColor;

            SendMessageToClient(info.toStyledString());
            delete pPush;
        }
        vecListString.clear();
        //SetEvent(ghListStringEvent);
        sem_post(&sem);


    }
    return 0;
}

void OperationSysteminfoScan()
{
    Json::Value info_OperationSysteminfo;
        Json::Value data_OperationSysteminfo;
        st_sysinfo get_OperationSysteminfo;
   get_OperationSysteminfo= ptr_SysInfo->getOperationSystemInfo();

data_OperationSysteminfo["type"]="OperationSysteminfoScan";

data_OperationSysteminfo["size"]=1;

data_OperationSysteminfo["OS"]=get_OperationSysteminfo.OS;

data_OperationSysteminfo["Architecture"]=get_OperationSysteminfo.Architecture;
data_OperationSysteminfo["CPU"]=get_OperationSysteminfo.CPU;

data_OperationSysteminfo["RAM"]=get_OperationSysteminfo.RAM;//
data_OperationSysteminfo["LinuxkernelVersion"]=get_OperationSysteminfo.LinuxkernelVersion;

data_OperationSysteminfo["Hostname"]=get_OperationSysteminfo.Hostname;
data_OperationSysteminfo["SystemPath"]=get_OperationSysteminfo.SystemPath;
info_OperationSysteminfo["msg"]=data_OperationSysteminfo;
SendMessageToClient(info_OperationSysteminfo.toStyledString());
return;
}



void NetworkAdapterFormsScan()
{
    Json::Value info_NetworkAdapter;
         Json::Value data_NetworkAdapter;
                std::vector<st_Hardware_lshw>get_NetworkAdapter;
                   cout<<__LINE__<<__FUNCTION__<<endl;
         ptr_SysInfo->getNetWorkHardWare(get_NetworkAdapter);
         cout<<__LINE__<<__FUNCTION__<<endl;
    std::size_t lenNetworkAdapter=get_NetworkAdapter.size();
           for(int i=0;i<lenNetworkAdapter;i++)
             {


               data_NetworkAdapter["type"]="NetworkAdapterScan";
     data_NetworkAdapter["size"]=lenNetworkAdapter;
               data_NetworkAdapter["id"]=get_NetworkAdapter[i].id;
              data_NetworkAdapter["description"]=get_NetworkAdapter[i].description;
          data_NetworkAdapter["product"]=get_NetworkAdapter[i].product;

          data_NetworkAdapter["vendor"]=get_NetworkAdapter[i].vendor;
                    data_NetworkAdapter["physical_id"]=get_NetworkAdapter[i].physical_id;

                              data_NetworkAdapter["bus_info"]=get_NetworkAdapter[i].bus_info;
                             data_NetworkAdapter["logical_name"]=get_NetworkAdapter[i].logical_name;
           data_NetworkAdapter["version"]=get_NetworkAdapter[i].version;
                      data_NetworkAdapter["serial"]=get_NetworkAdapter[i].serial;
                                 data_NetworkAdapter["width"]=get_NetworkAdapter[i].width;
                                            data_NetworkAdapter["clock"]=get_NetworkAdapter[i].clock;

                                                       data_NetworkAdapter["capabilities"]=get_NetworkAdapter[i].capabilities;
                                                                  data_NetworkAdapter["configuration"]=get_NetworkAdapter[i].configuration;
                                                                             data_NetworkAdapter["resources"]=get_NetworkAdapter[i].resources;
                                                                 data_NetworkAdapter["capacity"]=get_NetworkAdapter[i].capacity;

                                                                 data_NetworkAdapter["ipv4"]=get_NetworkAdapter[i].IPV4;
                                                                  data_NetworkAdapter["maskip"]=get_NetworkAdapter[i].MaskIP;
                                                                    data_NetworkAdapter["ipv6"]=get_NetworkAdapter[i].IPV6;
                                                       data_NetworkAdapter["dns"]=get_NetworkAdapter[i].dns;

     info_NetworkAdapter["msg"]=data_NetworkAdapter;
      SendMessageToClient(info_NetworkAdapter.toStyledString());
                    }
return;
}
void CheckInternetConnectScan()
{

    Json::Value info_Checkinternetconnectforms;
        Json::Value data_Checkinternetconnectforms;
        std::vector<st_checkConnection>get_Checkinternetconnectforms;
  ptr_SysInfo->checkInternetConnect( get_Checkinternetconnectforms);
std::size_t lenCheckinternetconnectforms=get_Checkinternetconnectforms.size();
   for(int i=0;i<lenCheckinternetconnectforms;i++)
     {



       data_Checkinternetconnectforms["type"]="CheckInternetConnectScan";
data_Checkinternetconnectforms["size"]=lenCheckinternetconnectforms;
       data_Checkinternetconnectforms["destination"]=get_Checkinternetconnectforms[i].destination;    //去除回车;
      data_Checkinternetconnectforms["result"]=get_Checkinternetconnectforms[i].result;

info_Checkinternetconnectforms["msg"]=data_Checkinternetconnectforms;
SendMessageToClient(info_Checkinternetconnectforms.toStyledString());
   }

return;
}

void AccountSecurityScan()
{

    Json::Value info_AccountSecurity;
        Json::Value data_AccountSecurity;
        std::vector<st_sysAccount>get_AccountSecurity;
   get_AccountSecurity= ptr_SysInfo->getSysAccountSecurityCheck();
std::size_t lenAccountSecurity=get_AccountSecurity.size();
   for(int i=0;i<lenAccountSecurity;i++)
     {



       data_AccountSecurity["type"]="AccountSecurityCheckScan";
data_AccountSecurity["size"]=lenAccountSecurity;
       data_AccountSecurity["UserName"]=get_AccountSecurity[i].UserName.erase(get_AccountSecurity[i].UserName.find_last_of('\t'), string::npos);    //去除回车;
      data_AccountSecurity["LastPasswordChange"]=get_AccountSecurity[i].LastPasswordChange.erase(get_AccountSecurity[i].LastPasswordChange.find_last_of('\t'), string::npos);
  data_AccountSecurity["PasswdExpires"]=get_AccountSecurity[i].PasswdExpires.erase(get_AccountSecurity[i].PasswdExpires.find_last_of('\t'), string::npos);

  data_AccountSecurity["PasswordInactive"]=get_AccountSecurity[i].PasswordInactive.erase(get_AccountSecurity[i].PasswordInactive.find_last_of('\t'), string::npos);
            data_AccountSecurity["AccountExpires"]=get_AccountSecurity[i].AccountExpires;
    data_AccountSecurity["Minimum_number_of_days_between_password_change"]=get_AccountSecurity[i].Minimum_number_of_days_between_password_change;
   data_AccountSecurity["Maximum_number_of_days_between_password_change"]=get_AccountSecurity[i].Maximum_number_of_days_between_password_change;
data_AccountSecurity["Number_of_days_of_warning_before_password_expires"]=get_AccountSecurity[i].Number_of_days_of_warning_before_password_expires;
info_AccountSecurity["msg"]=data_AccountSecurity;
SendMessageToClient(info_AccountSecurity.toStyledString());

       }

return;
}

void  BluetoothScanScan()
{

    Json::Value info_Bluetooth;
            Json::Value data_Bluetooth;
                   std::vector<st_Hardware_lshw>get_Bluetooth;
                      cout<<__LINE__<<__FUNCTION__<<endl;
          // ptr_SysInfo->getUSB_MassStorageDevice(get_Bluetooth);
                       ptr_SysInfo->getBluetooth(get_Bluetooth);
      //   cout<<__LINE__<<__FUNCTION__<<endl;
       std::size_t lenBluetooth=get_Bluetooth.size();
     if( lenBluetooth==0)
     {
  cout<<"No BluetoothScanSca!"<<endl;
       data_Bluetooth["type"]="BluetoothScan";
  data_Bluetooth["size"]=lenBluetooth;
  info_Bluetooth["msg"]=data_Bluetooth;
   SendMessageToClient(info_Bluetooth.toStyledString());
     cout<<__LINE__<<__FUNCTION__<<endl;
   return;

     }
              for(int i=0;i<lenBluetooth;i++)
                {


                  data_Bluetooth["type"]="BluetoothScan";
        data_Bluetooth["size"]=lenBluetooth;
                  data_Bluetooth["id"]=get_Bluetooth[i].id;
                 data_Bluetooth["description"]=get_Bluetooth[i].description;
             data_Bluetooth["product"]=get_Bluetooth[i].product;

             data_Bluetooth["vendor"]=get_Bluetooth[i].vendor;
                       data_Bluetooth["physical_id"]=get_Bluetooth[i].physical_id;

                                 data_Bluetooth["bus_info"]=get_Bluetooth[i].bus_info;
                                data_Bluetooth["logical_name"]=get_Bluetooth[i].logical_name;
              data_Bluetooth["version"]=get_Bluetooth[i].version;
                         data_Bluetooth["serial"]=get_Bluetooth[i].serial;
                                    data_Bluetooth["width"]=get_Bluetooth[i].width;
                                               data_Bluetooth["clock"]=get_Bluetooth[i].clock;

                                                          data_Bluetooth["capabilities"]=get_Bluetooth[i].capabilities;
                                                                     data_Bluetooth["configuration"]=get_Bluetooth[i].configuration;
                                                                                data_Bluetooth["resources"]=get_Bluetooth[i].resources;
                                                                    data_Bluetooth["capacity"]=get_Bluetooth[i].capacity;
        info_Bluetooth["msg"]=data_Bluetooth;
         SendMessageToClient(info_Bluetooth.toStyledString());
           cout<<__LINE__<<__FUNCTION__<<endl;

                       }

return;

}



 void USBHardWareScan()
 {
     Json::Value info_USBHardWareForms;
          Json::Value data_USBHardWareForms;
                 std::vector<st_Hardware_lshw>get_USBHardWareForms;
                    //cout<<__LINE__<<__FUNCTION__<<endl;
          ptr_SysInfo->getUSB_MassStorageDevice(get_USBHardWareForms);
    //   cout<<__LINE__<<__FUNCTION__<<endl;
     std::size_t lenUSBHardWareForms=get_USBHardWareForms.size();
   if( lenUSBHardWareForms==0)
   {
cout<<"No USBHardWareS!"<<endl;
       data_USBHardWareForms["type"]="USBHardWareScan";
data_USBHardWareForms["size"]=lenUSBHardWareForms;
info_USBHardWareForms["msg"]=data_USBHardWareForms;
 SendMessageToClient(info_USBHardWareForms.toStyledString());
 return;

   }
            for(int i=0;i<lenUSBHardWareForms;i++)
              {


                data_USBHardWareForms["type"]="USBHardWareScan";
      data_USBHardWareForms["size"]=lenUSBHardWareForms;
                data_USBHardWareForms["id"]=get_USBHardWareForms[i].id;
               data_USBHardWareForms["description"]=get_USBHardWareForms[i].description;
           data_USBHardWareForms["product"]=get_USBHardWareForms[i].product;

           data_USBHardWareForms["vendor"]=get_USBHardWareForms[i].vendor;
                     data_USBHardWareForms["physical_id"]=get_USBHardWareForms[i].physical_id;

                               data_USBHardWareForms["bus_info"]=get_USBHardWareForms[i].bus_info;
                              data_USBHardWareForms["logical_name"]=get_USBHardWareForms[i].logical_name;
            data_USBHardWareForms["version"]=get_USBHardWareForms[i].version;
                       data_USBHardWareForms["serial"]=get_USBHardWareForms[i].serial;
                                  data_USBHardWareForms["width"]=get_USBHardWareForms[i].width;
                                             data_USBHardWareForms["clock"]=get_USBHardWareForms[i].clock;

                                                        data_USBHardWareForms["capabilities"]=get_USBHardWareForms[i].capabilities;
                                                                   data_USBHardWareForms["configuration"]=get_USBHardWareForms[i].configuration;
                                                                              data_USBHardWareForms["resources"]=get_USBHardWareForms[i].resources;
                                                                  data_USBHardWareForms["capacity"]=get_USBHardWareForms[i].capacity;
      info_USBHardWareForms["msg"]=data_USBHardWareForms;
       SendMessageToClient(info_USBHardWareForms.toStyledString());
     //  cout<<"info_USBHardWareForms.toStyledString:"<<info_USBHardWareForms.toStyledString()<<endl;
       //       cout<<__LINE__<<__FUNCTION__<<endl;
                     }




 }

 void PackageScan()
 {
     Json::Value info_Package;
          Json::Value data_Package;
                 std::vector<st_aptPackgesInfo >get_Package;
                    bool isExistAPT;
                        bool isExistRPM;

            get_Package= ptr_SysInfo->getSoftwareCheckOfSystemInstallation_APT(isExistAPT);
            if(isExistAPT)
            {
     std::size_t lenPackage=get_Package.size();
            for(int i=0;i<lenPackage;i++)
              {


                data_Package["type"]="PackageScan";
      data_Package["SIZE"]=lenPackage;
   data_Package["PackageToolType"]="apt";
     data_Package["PackageName"]=get_Package[i].PackageName;
        data_Package["Architecture"]=get_Package[i].Architecture;
                data_Package["Replaces"]=get_Package[i].Replaces;
               data_Package["Breaks"]=get_Package[i].Breaks;



                     data_Package["Version"]=get_Package[i].Version;

                               data_Package["Priority"]=get_Package[i].Priority;
                              data_Package["Essential"]=get_Package[i].Essential;
            data_Package["Section"]=get_Package[i].Section;

            data_Package["Origin"]=get_Package[i].Origin;
                         data_Package["Maintainer"]=get_Package[i].Maintainer;;
                        data_Package["Original_Maintaine"]=get_Package[i].Original_Maintaine;
                    data_Package["Bugs"]=get_Package[i].Bugs;

                    data_Package["Installed_Size"]=get_Package[i].Installed_Size;
                              data_Package["Task"]=get_Package[i].Task;

                                        data_Package["Depends"]=get_Package[i].Depends;
                                       data_Package["Filename"]=get_Package[i].Filename;
                     data_Package["Original_Maintainer"]=get_Package[i].Original_Maintainer;

                     data_Package["Supported"]=get_Package[i].Supported;
                                  data_Package["APT_Manual_Installed"]=get_Package[i].APT_Manual_Installed;
                                 data_Package["MD5sum"]=get_Package[i].MD5sum;
                             data_Package["SHA1"]=get_Package[i].SHA1;

                             data_Package["SHA256"]=get_Package[i].SHA256;
                                       data_Package["Description"]=get_Package[i].Description;

                                                 data_Package["APT_Sources"]=get_Package[i].APT_Sources;
                                                data_Package["Source"]=get_Package[i].Source;
                              data_Package["Multi_Arch"]=get_Package[i].Multi_Arch;
//
                              data_Package["Pre_Depends"]=get_Package[i].Pre_Depends;
                                        data_Package["Provides"]=get_Package[i].Provides;

                                                  data_Package["APT_Sources"]=get_Package[i].APT_Sources;
                                                 data_Package["Download_Size"]=get_Package[i].Download_Size;
                               data_Package["Homepage"]=get_Package[i].Homepage;
        data_Package["Suggests"]=get_Package[i].Suggests;


      info_Package["msg"]=data_Package;
       SendMessageToClient(info_Package.toStyledString());
                     }
                return;
                }
            else
            {
                Json::Value info_RPM_Package;
                     Json::Value data_RPM_Package;
                   std::vector<st_rpmPackagesInfo >get_RPM_Package;

            get_RPM_Package=ptr_SysInfo->getSoftwareCheckOfSystemInstalltaion_RPM(isExistRPM);

            std::size_t len_RPM_Package=get_RPM_Package.size();
                    if(isExistRPM)
                    {

                        for(int i=0;i<len_RPM_Package;i++)
                        {
                        data_RPM_Package["type"]="PackageScan";
              data_RPM_Package["SIZE"]=len_RPM_Package;
           data_RPM_Package["PackageToolType"]="rpm";

                        data_RPM_Package["Name"]=get_RPM_Package[i].Name;
                       data_RPM_Package["Version"]=get_RPM_Package[i].Version;
                   data_RPM_Package["Architecture"]=get_RPM_Package[i].Architecture;

                   data_RPM_Package["Install_Date"]=get_RPM_Package[i].Install_Date;

                             data_RPM_Package["Group"]=get_RPM_Package[i].Group;


                                      data_RPM_Package["Size"]=get_RPM_Package[i].Size;
                    data_RPM_Package["License"]=get_RPM_Package[i].License;

                    data_RPM_Package["Signature"]=get_RPM_Package[i].Signature;


                                 data_RPM_Package["Source_RPM"]=get_RPM_Package[i].Source_RPM;
                                data_RPM_Package["Build_Date"]=get_RPM_Package[i].Build_Date;
                            data_RPM_Package["Build_Host"]=get_RPM_Package[i].Build_Host;

                            data_RPM_Package["Relocations"]=get_RPM_Package[i].Relocations;
                                      data_RPM_Package["Packager"]=get_RPM_Package[i].Packager;

                                                data_RPM_Package["Vendor"]=get_RPM_Package[i].Vendor;
                                               data_RPM_Package["URL"]=get_RPM_Package[i].URL;
                             data_RPM_Package["Summary"]=get_RPM_Package[i].Summary;
                             data_RPM_Package["Description"]=get_RPM_Package[i].Description;
           data_RPM_Package["Release"]=get_RPM_Package[i].Release;

                             info_RPM_Package["msg"]=data_RPM_Package;
                                   SendMessageToClient(info_RPM_Package.toStyledString());

                                }
                        return;

                    }
                    else {
                        return;
                    }




            }


 }
void  SystemAccountScan()
{
    Json::Value info_SystemAccount;
         Json::Value data_SystemAccount;
                std::vector<st_sysAccount>get_SystemAccount;
           get_SystemAccount= ptr_SysInfo->getSysAccountSecurityCheck();
    std::size_t lenSystemAccount=get_SystemAccount.size();
           for(int i=0;i<lenSystemAccount;i++)
             {


               data_SystemAccount["type"]="SystemAccountScan";
     data_SystemAccount["size"]=lenSystemAccount;
               data_SystemAccount["UserName"]=get_SystemAccount[i].UserName;
              data_SystemAccount["LastPasswordChange"]=get_SystemAccount[i].LastPasswordChange;
          data_SystemAccount["PasswdExpires"]=get_SystemAccount[i].PasswdExpires;

          data_SystemAccount["PasswordInactive"]=get_SystemAccount[i].PasswordInactive;
                    data_SystemAccount["AccountExpires"]=get_SystemAccount[i].AccountExpires;

                              data_SystemAccount["Minimum_number_of_days_between_password_change"]=get_SystemAccount[i].Minimum_number_of_days_between_password_change;
                             data_SystemAccount["Maximum_number_of_days_between_password_change"]=get_SystemAccount[i].Maximum_number_of_days_between_password_change;
           data_SystemAccount["Number_of_days_of_warning_before_password_expires"]=get_SystemAccount[i].Number_of_days_of_warning_before_password_expires;
     info_SystemAccount["msg"]=data_SystemAccount;
      SendMessageToClient(info_SystemAccount.toStyledString());
                    }

return;
}

 void FirefoxInternetHistoryScan()
 {


//     struct st_BrowserHistory
//     {
//      st_BrowserHistory()
//      {

//      }
//      int Type;
//      std::string user_Name;
//       std::vector<std::string> vec_Url;
//       std::vector<std::string> vec_Title;
//       std::vector<std::string> vec_DateTime;
//     };
 Json::Value info_FireFoxHistory;
     Json::Value data_FirefoxHistory;
            std::vector<st_BrowserHistory>get_FireFoxHistory;
       get_FireFoxHistory= ptr_SysInfo->getFirefoxInternetAccessRecord();
std::size_t lenBrowserHistory=get_FireFoxHistory.size();
       for(int i=0;i<lenBrowserHistory;i++)
         {


                for(int j=0;j<get_FireFoxHistory[i].vec_DateTime.size();j++)
                {
           data_FirefoxHistory["type"]="FireFoxScan";
 data_FirefoxHistory["size"]=get_FireFoxHistory[i].countData;
           data_FirefoxHistory["user_Name"]=get_FireFoxHistory[i].user_Name;
          data_FirefoxHistory["url"]=get_FireFoxHistory[i].vec_Url[j];
      data_FirefoxHistory["title"]=get_FireFoxHistory[i].vec_Title[j];

      data_FirefoxHistory["date"]=get_FireFoxHistory[i].vec_DateTime[j];
                info_FireFoxHistory["msg"]=data_FirefoxHistory;
  SendMessageToClient(info_FireFoxHistory.toStyledString());
                }
                if(get_FireFoxHistory[i].vec_DateTime.size()==0)
                {
                    cout<<__LINE__<<__FUNCTION__<<"lenBrowserHistory="<<lenBrowserHistory<<endl;
                                   data_FirefoxHistory["type"]="FireFoxScan";
                data_FirefoxHistory["size"]=0;
                 info_FireFoxHistory["msg"]=data_FirefoxHistory;
                                SendMessageToClient(info_FireFoxHistory.toStyledString());

                }
}
return;

 }
 void ChromeInternetHistory()
 {
     Json::Value info_ChromeHistory;
         Json::Value data_ChromeHistory;
                std::vector<st_BrowserHistory>get_ChromeHistory;
           get_ChromeHistory= ptr_SysInfo->getChromeInternetAccessRecord();
    std::size_t lenBrowserHistory=get_ChromeHistory.size();
       cout<<__LINE__<<__FUNCTION__<<"lenBrowserHistory=  "<<lenBrowserHistory<<endl;
//    if(lenBrowserHistory==3)
//    {
//        cout<<__LINE__<<__FUNCTION__<<"lenBrowserHistory="<<lenBrowserHistory<<endl;
//                       data_ChromeHistory["type"]="ChromeScan";
//     data_ChromeHistory["size"]=0;
//     info_ChromeHistory["msg"]=data_ChromeHistory;
//                    SendMessageToClient(info_ChromeHistory.toStyledString());

//    }
           for(int i=0;i<lenBrowserHistory;i++)
             {


                    for(int j=0;j<get_ChromeHistory[i].vec_DateTime.size();j++)
                    {
               data_ChromeHistory["type"]="ChromeScan";
     data_ChromeHistory["size"]=get_ChromeHistory[i].countData;
               data_ChromeHistory["user_Name"]=get_ChromeHistory[i].user_Name;
              data_ChromeHistory["url"]=get_ChromeHistory[i].vec_Url[j];
          data_ChromeHistory["title"]=get_ChromeHistory[i].vec_Title[j];

          data_ChromeHistory["date"]=get_ChromeHistory[i].vec_DateTime[j];
                    info_ChromeHistory["msg"]=data_ChromeHistory;
                      cout<<__LINE__<<__FUNCTION__<<"lenBrowserHistory="<<lenBrowserHistory<<endl;
      SendMessageToClient(info_ChromeHistory.toStyledString());
                    }
                        if(get_ChromeHistory[i].vec_DateTime.size()==0)
                        {
                            cout<<__LINE__<<__FUNCTION__<<"lenBrowserHistory="<<lenBrowserHistory<<endl;
                                           data_ChromeHistory["type"]="ChromeScan";
                         data_ChromeHistory["size"]=0;
                         info_ChromeHistory["msg"]=data_ChromeHistory;
                                        SendMessageToClient(info_ChromeHistory.toStyledString());

                        }

             }
return;
 }

void SystemModuleScan()//
{

    Json::Value info;
        std::vector<st_SystemModuleSecurityCheck>get_SystemModuleVec;
  get_SystemModuleVec= ptr_SysInfo->getSystemModuleSecurityCheck();


  Json::Value info_SystemModule,info_len;
     Json::Value data_SystemModule;
   std::cout<<__LINE__<<__FUNCTION__<<std::endl;
  std::size_t lenVec=get_SystemModuleVec.size();

  char c_len[5]={0};
sprintf(c_len,"%d",lenVec);



     for(int i=0;i<lenVec ;i++)
     {
         data_SystemModule["type"]="SystemModuleReadHatEvents";

 data_SystemModule["size"]=lenVec;
                 data_SystemModule["ModuleName"]=get_SystemModuleVec[i].ModuleName;
           data_SystemModule["MemorySizeModulee"]=get_SystemModuleVec[i].MemorySizeModule;
             data_SystemModule["LoadedTimes"]=get_SystemModuleVec[i].LoadedTimes;
             data_SystemModule["DependentModule"]=get_SystemModuleVec[i].DependentModule;
          data_SystemModule["MoslueState"]=get_SystemModuleVec[i].MoslueState;
          data_SystemModule["OffsetModule"]=get_SystemModuleVec[i].OffsetModule;
          data_SystemModule["FileName"]=get_SystemModuleVec[i].FileName;
       data_SystemModule["License"]=get_SystemModuleVec[i].License;
      data_SystemModule["Description"]=get_SystemModuleVec[i].Description;
      data_SystemModule["Author"]=get_SystemModuleVec[i].Author;
          data_SystemModule["srcVersion"]=get_SystemModuleVec[i].srcVersion;
       data_SystemModule["Depends"]=get_SystemModuleVec[i].Depends;
      data_SystemModule["Retpoline"]=get_SystemModuleVec[i].Retpoline;
        data_SystemModule["Intree"]=get_SystemModuleVec[i].Intree;
                data_SystemModule["Vermagic"]=get_SystemModuleVec[i].Vermagic;
                  data_SystemModule["Sig_id"]=get_SystemModuleVec[i].Sig_id;
  data_SystemModule["Signer"]=get_SystemModuleVec[i].Signer;
    data_SystemModule["Sig_Key"]=get_SystemModuleVec[i].Sig_Key;
  data_SystemModule["Sig_HashAlg"]=get_SystemModuleVec[i].Sig_HashAlgo;
  data_SystemModule["Signature"]=get_SystemModuleVec[i].Signature;

info_SystemModule["msg"]=data_SystemModule;
   SendMessageToClient(info_SystemModule.toStyledString());
   std::cout<<__LINE__<<__FUNCTION__<<std::endl;
     }
return;
}
void MountPointScan()//take_add:挂载点
{

    Json::Value info;
        std::vector<st_MountPoint>get_MountPoint;
     get_MountPoint= ptr_SysInfo->getSystemMountPoint();


             Json::Value info_MountPoint;
                Json::Value data_MountPoint;

                std::size_t len=get_MountPoint.size();

                                for(int i=0;i<len;i++)
                                {
                                    data_MountPoint["type"]="MountPointEvents";
data_MountPoint["size"]=len;
                                    data_MountPoint["FileSystem"]=get_MountPoint[i].FileSystem;
                                   data_MountPoint["Size"]=get_MountPoint[i].Size;
                               data_MountPoint["Used"]=get_MountPoint[i].Used;
                               data_MountPoint["Available"]=get_MountPoint[i].Available;
                                    data_MountPoint["UseagePercentage"]=get_MountPoint[i].UseagePercentage;
                                             data_MountPoint["MountOn"]=get_MountPoint[i].MountOn;
//
                           info_MountPoint["msg"]=data_MountPoint;
                           SendMessageToClient(info_MountPoint.toStyledString());
                                }

return ;
}

void OCR_updatekeyword()
{
    Json::Value info_InlineImagePost;
    Json::Value data_InlineImagePost;
    string strInlineImagePost="测试";

string retInlineIMagePost=updata_keyword_post(strInlineImagePost);

cout<<"retInlineIMagePost="<<retInlineIMagePost<<endl;
cout<<"strInlineImagePost="<<strInlineImagePost<<endl;

}


 void FileSystemLshwScan()
{

/*

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
};

*/


     /*

root@hk:~/git_repository/linuxscs/portable-system-info-cpp_ubuntu# lshw -c disk
  *-disk
       description: SCSI Disk
       product: VMware Virtual S
       vendor: VMware,
       physical id: 0.0.0
       bus info: scsi@32:0.0.0
       logical name: /dev/sdb
       version: 1.0
       size: 80GiB (85GB)
       capabilities: 7200rpm partitioned partitioned:dos
       configuration: ansiversion=2 logicalsectorsize=512 sectorsize=512 signature=d29b202b
  *-disk
       description: ATA Disk
       product: VMware Virtual S
       physical id: 0.0.0
       bus info: scsi@0:0.0.0
       logical name: /dev/sda
       version: 0001
       serial: 00000000000000000001
       size: 20GiB (21GB)
       configuration: ansiversion=5 logicalsectorsize=512 sectorsize=512
  *-cdrom
       description: DVD-RAM writer
       product: VMware SATA CD01
       vendor: NECVMWar
       physical id: 0.0.0
       bus info: scsi@1:0.0.0
       logical name: /dev/cdrom
       logical name: /dev/cdrw
       logical name: /dev/dvd
       logical name: /dev/sr0
       version: 1.00
       capabilities: removable audio cd-r cd-rw dvd dvd-r dvd-ram
       configuration: ansiversion=5 status=open

*/

     Json::Value info;
      std::vector<st_Hardware_lshw>get_FilesystemLshw;
     ptr_SysInfo->getFileSystemInfo(get_FilesystemLshw);

       Json::Value info_FileSystemLshw;
       Json::Value data_FileSystemLshw;

       std::cout<<"get_FilesystemLshw.size()="<<get_FilesystemLshw.size()<<std::endl;
                       for(int i=0;i<get_FilesystemLshw.size();i++)
                       {
                           data_FileSystemLshw["type"]="FileSystemLshwEvents";

                           data_FileSystemLshw["id"]=get_FilesystemLshw[i].id;
                          data_FileSystemLshw["description"]=get_FilesystemLshw[i].description;
                            data_FileSystemLshw["product"]=get_FilesystemLshw[i].description;
                      data_FileSystemLshw["product"]=get_FilesystemLshw[i].product;
                          data_FileSystemLshw["serial"]=get_FilesystemLshw[i].serial;
     data_FileSystemLshw["logicalname"]=get_FilesystemLshw[i].logical_name;
     data_FileSystemLshw["size"]=get_FilesystemLshw[i].size;

  data_FileSystemLshw["configuration"]=get_FilesystemLshw[i].configuration;

                  info_FileSystemLshw["msg"]=data_FileSystemLshw;
                  SendMessageToClient(info_FileSystemLshw.toStyledString());
                 // std::cout<<__LINE__<<__FUNCTION__<<std::endl;
                       }



}

void FileSystemScan()//
{

    bFirstRun=true;//take_debug:
    if (bFirstRun)
    {

/*
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
*/  std::cout<<__LINE__<<__FUNCTION__<<std::endl;

        Json::Value info;
        std::vector<st_fileSystem>get_Filesystem;
       get_Filesystem= ptr_SysInfo->getFileSystemInfo_InC();

         Json::Value info_FileSystem;
         Json::Value data_FileSystem;

         std::cout<<"get_Filesystem.size()="<<get_Filesystem.size()<<std::endl;
                         for(int i=0;i<get_Filesystem.size();i++)
                         {
                             data_FileSystem["type"]="FileSystemEvents";

                             data_FileSystem["DriverInterFaceType"]=get_Filesystem[i].DriverInterFaceType;
                            data_FileSystem["SerialNumber"]=get_Filesystem[i].SerialNumber;
                        data_FileSystem["ManufacturerModel"]=get_Filesystem[i].ManufacturerModel;
                        data_FileSystem["FirmwareRevision"]=get_Filesystem[i].FirmwareRevision;
                             data_FileSystem["Capacity"]=get_Filesystem[i].Capacity;
                                      data_FileSystem["SCSI_Vendor"]=get_Filesystem[i].SCSI_Vendor;
                                                        data_FileSystem["Product_id"]=get_Filesystem[i].Product_id;
                                                        data_FileSystem["DevPath"]=get_Filesystem[i].DevPath;
                    info_FileSystem["msg"]=data_FileSystem;
                    SendMessageToClient(info_FileSystem.toStyledString());
  std::cout<<__LINE__<<__FUNCTION__<<std::endl;
                         }



        bFirstRun = false;
    }
    return;
}



void USBScan()
{


    if(ptr_SysInfo==NULL)
    {
  cout<<__LINE__<<"ptr_SysInfo is NULL"<<endl;
    }

std::vector<st_filtered>get_USBEvents;
ptr_SysInfo->getUSB_EventHistory(get_USBEvents);
//cout<<"----------------------------"<<endl;
//for(int i=0;i<get_USBEvents.size();i++)
//{
//cout<<"["<<i<<"]="<<get_USBEvents[i].conn<<endl;
//cout<<get_USBEvents[i].pid<<endl;
//cout<<get_USBEvents[i].port<<endl;
//cout<<get_USBEvents[i].prod<<endl;
//cout<<get_USBEvents[i].serial<<endl;
//cout<<get_USBEvents[i].type<<endl;
//cout<<get_USBEvents[i].disconn<<endl;
//}
//cout<<"----------------------------"<<endl;


Json::Value info;
Json::Value data;

int len=get_USBEvents.size();
for(int i=0;i<len;i++)
{
data["size"]=len;
data["type"]="USBHistoryEvents";
//  info["msg_subtype"]="conn";
data["conn"]=get_USBEvents[i].conn;
        data["port"]=get_USBEvents[i].port;
data["idProduct"]=get_USBEvents[i].idProduct;
data["serial"]=get_USBEvents[i].serial;
data["idVendor"]=get_USBEvents[i].idVendor;
data["prod"]=get_USBEvents[i].prod;
//manufact
data["manufact"]=get_USBEvents[i].manufact;
data["disconn"]=get_USBEvents[i].disconn;
info["msg"]=data;
cout<<"info[\"msg\"][\"type\"]="<<info["msg"]["type"]<<endl;



SendMessageToClient(info.toStyledString());


}


return ;
}




void StartScan()//
{

	if (bFirstRun)
	{
//                  if(ptr_SysInfo==NULL)
//                  {
//                cout<<__LINE__<<"ptr_SysInfo is NULL"<<endl;
//                  }
//         std::vector<st_Hardware_lshw> get_USBEvents;
//        ptr_SysInfo->getNetWorkHardWare(get_USBEvents);

/*
 * std::string id;
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
};


*/
        Json::Value info;
//    info["msg"] = "Networkhardware";
//        info["ScanInfoDescribe"] = "toGetNetworkHarware";

//     info["id"]=get_USBEvents[i].id;
//        info["description"]=get_USBEvents[i].description;
//            info["product"]=get_USBEvents[i].product;
//                    info["vendor"]=get_USBEvents[i].vendor;
//         info["physical_id"]=get_USBEvents[i].physical_id;
//  info["bus_info"]=get_USBEvents[i].bus_info;


        info["1"]="111111111111";
     info["2"]="2222222222222";
   info["3"]="3333333";

        //SendMessageToClient(info.toStyledString());
        sem_init(&sem,0,0);
        sem_post(&sem);
        pthread_t thread;
      //  pthread_create(&thread,NULL,TQListStringThread,NULL);//take:send
            pthread_create(&thread,NULL,TQViewThread,NULL);
        //CreateThread(0, 0, TQListStringThread, NULL, 0, 0);
      //  CreateThread(0, 0, TQViewThread, NULL, 0, 0);

        bFirstRun = false;
	}
}


void Exit()
{
    exit(0);
    //ExitProcess(0);
}

void OutputReportForm()
{
    bWriteLog = true;
}



bool SetModuleDangerInfo()
{
    SetListString("未通过数字认证驱动:tp.sys",0);
//    SetListString("发现隐藏驱动,路径:c:\\windows\\ggg.sys", 2);
//    SetListString("未通过数字认证进程PID为:234,路径为:c:\\windows\\ccc.exe", 1);
//    SetListString("有可疑自启动模块:c:\\ggg.exe", 1);
    return true;
}


bool TrojanOutResult()
{
//    SetListString(string(""));
//    SetListString(string(""));
//    SetListString(string("木马扫描已经完成,一共发现777个木马,查看木马行为详细信息请点击[木马扫描结果]"));
//    SetListString(string("现在开始进行U盘摆渡木马查杀."));
//    SetListString(string(""));
//    SetListString(string(""));

    return true;
}

void* TQTrojanThread(void* pParam)  //对应的USB插拔时间扫描
{
    while (1)
    {
        if (gbCanTrojanScan)
        {
//            char type;
//            std::string conn;
//            std::string pid;
//            std::string prod;
//            std::string manufact;
//            std::string serial;
//            std::string port;
//           std::string disconn;
//          std::string idVendor;
//          string idProduct;

                              if(ptr_SysInfo==NULL)
                              {
                            cout<<__LINE__<<"ptr_SysInfo is NULL"<<endl;
                              }

                     std::vector<st_filtered>get_USBEvents;
                    ptr_SysInfo->getUSB_EventHistory(get_USBEvents);
                    cout<<"----------------------------"<<endl;
for(int i=0;i<get_USBEvents.size();i++)
{
    cout<<"["<<i<<"]="<<get_USBEvents[i].conn<<endl;
cout<<get_USBEvents[i].pid<<endl;
cout<<get_USBEvents[i].port<<endl;
cout<<get_USBEvents[i].prod<<endl;
cout<<get_USBEvents[i].serial<<endl;
cout<<get_USBEvents[i].type<<endl;
cout<<get_USBEvents[i].disconn<<endl;
}
       cout<<"----------------------------"<<endl;


                  Json::Value info;
                  Json::Value data;
         //       info["msg"] = "Networkhardware";
         //          info["msg"] = "TrojanScanInfo";
               //     info["ScanInfoDescribe"] = "toGetNetworkHarware";
                  int len=get_USBEvents.size();
for(int i=0;i<len;i++)
{
      data["size"]=len;
                        data["type"]="USBHistoryEvents";
                          //  info["msg_subtype"]="conn";
                    data["conn"]=get_USBEvents[i].conn;
                                  data["port"]=get_USBEvents[i].port;
     data["idProduct"]=get_USBEvents[i].idProduct;
         data["serial"]=get_USBEvents[i].serial;
 data["idVendor"]=get_USBEvents[i].idVendor;
 data["prod"]=get_USBEvents[i].prod;
   //  SendMessageToClient(info.toStyledString());
//info.clear();
        // info["msg"]="USBHistoryEvents";
        // info["msg_subtype"]="disconn"
   data["disconn"]=get_USBEvents[i].disconn;
 info["msg"]=data;
 cout<<"info[\"msg\"][\"type\"]="<<info["msg"]["type"]<<endl;
    //  info["msg"]=data.toStyledString();


SendMessageToClient(info.toStyledString());

//info.clear();
//         info["msg"]="USBHistoryEvents";
//         info["pid"]=get_USBEvents[i].pid;
//SendMessageToClient(info.toStyledString());
//info.clear();
//         info["msg"]="USBHistoryEvents";
//                                info["prod"]=get_USBEvents[i].prod;
//                                SendMessageToClient(info.toStyledString());
//                                info.clear();
//         info["msg"]="USBHistoryEvents";
//                     info["manufact"]=get_USBEvents[i].manufact;
//                     SendMessageToClient(info.toStyledString());
//                     info.clear();
//                              info["msg"]="USBHistoryEvents";
//              info["serial"]=get_USBEvents[i].serial;
//              SendMessageToClient(info.toStyledString());
//              info.clear();

//             info["ScanInfoDescribe"] = info["manufact"];
      //  SendMessageToClient(info.toStyledString());

   //   SetListString(info.toStyledString());//take_mark:
}
        //    SetModuleDangerInfo();

     //      TrojanOutResult();//###处理木马信息：打分
            gbHaveOnceScan = true;
            bOnXinXi = false;
        }
        sleep(60);//###1分钟
    }
    return 0;
}



void *TQViewThread(void *pParam)
{


//    ghListStringEvent = CreateEvent(NULL, false, true, NULL);
//    if (!ghListStringEvent)
//    {
//        return 1;
//    }

    sleep(1);
   // SetListString("获取内核模块信息...");
  //  SetListString("111_Ttaitl");
//    sleep(1);
//    SetListString(string("获取SSDT信息..."));
//    sleep(1);
//    SetListString(string("获取SHADOW SSDT信息..."));
//    sleep(1);
//    SetListString(string("获取IDT信息..."));
//    sleep(1);
//    SetListString(string("获取AUTORUN信息..."));
//    sleep(1);
//    SetListString(string("获取进程信息..."));
//    sleep(1);
//    SetListString(string("获取消息钩子信息..."));//###测试这里win10 x64 1803 获取钩子信息蓝屏 已解决
//    sleep(1);
//    SetListString(string("获取打开端口信息..."));
//    sleep(1);
//    SetListString(string("获取IAT信息..."));
//    sleep(1);
//    SetListString(string("开始信息处理..."));
//    sleep(1);
//    SetListString(string("  "));
//    sleep(1);
//    SetListString(string("信息处理已经完成,可以进入[进程分析]与[内核分析]查看详细内容"));
//    sleep(1);
//    SetListString(string("  "));
//    sleep(1);
//    SetListString(string("准备木马分析..."));
    gbCanTrojanScan = true;
     gbCanFileSystem=true;


    pthread_t thread;
    pthread_create(&thread,NULL,TQTrojanThread,NULL);

    //HANDLE hTrojanThread = CreateThread(0, 0, TQTrojanThread, NULL, 0, 0);
    //CloseHandle(hTrojanThread);


    return NULL;
}



bool  recyRemveDelimitStr(string &str)
{

size_t n_delimit=str.find('\n');

if(n_delimit!=string::npos)
{
       string temp=str.substr(0,n_delimit);
       size_t n_end=str.size();
       string temp2=str.substr(n_delimit+1,n_end);//
       temp+=temp2;
       str=temp;

       return false;
}
else
{
   return true;
}


}
std::string control_inlineimage_post(const std::string & set_value)
{
    std::string ret;
    do
    {
        std::string tmp = set_value;
        //tmp = gbk_to_utf8(tmp);
        std::string url = std::string("/pwd/set/embed");
        try {
            HttpClient client("127.0.0.1:7604");
            auto r1 = client.request("POST", url, tmp);

            std::stringstream ss;
            ss << r1->content.rdbuf();
            ret = ss.str();
        }
        catch (...)
        {

        }
    } while (0);

    return ret;
}
std::string updata_keyword_post(const std::string & keyword)
{
    std::string ret;
    do
    {
        std::string tmp = keyword;
       // tmp = gbk_to_utf8(tmp);
        //tmp = base64pp::Base64::encode(tmp);
        std::string url = std::string("/pwd/set/sen");
        try {
            HttpClient client("127.0.0.1:7604");
            auto r1 = client.request("POST", url, tmp);

            std::stringstream ss;
            ss << r1->content.rdbuf();
            ret = ss.str();
        }
        catch (...)
        {

        }
    } while (0);

    return ret;
}

std::string flt_file_by_get(const std::string& filename)
{
          std::cout<<__LINE__<<__FUNCTION__<<"filename="<<filename<<std::endl;
  //  LOG_INFO("%s",filename.c_str());
    std::string ret;
    do
    {
        std::string tmp = filename;
        //tmp = gbk_to_utf8(tmp);
                 std::cout<<__LINE__<<__FUNCTION__<<"tmp="<<tmp<<std::endl;
        tmp = base64pp::Base64::encode(tmp);

        std::string url = std::string("/flt/file/") + tmp;

       // LOG_INFO("%s",url.c_str());
        try {
            HttpClient client("127.0.0.1:7604");
            auto r1 = client.request("GET", url);
            std::cout<<__LINE__<<__FUNCTION__<<"url="<<url<<std::endl;
            std::stringstream ss;
            ss << r1->content.rdbuf();
            ret = ss.str();
        }
        catch (...)
        {

        }
    } while (0);

    return ret;
}

 void SendstrOCR(const std::string strOCR)
 {


                SendMessageToClient(strOCR);
                std::cout<<"strOCR="<<strOCR<<std::endl;


 }
string pwdProcess()
 {
     char dir[PATH_MAX] = {0};
         int n = readlink("/proc/self/exe", dir, PATH_MAX);
         //printf("PATH_MAX: %d\n", PATH_MAX);
         //printf("readlink return: %d\n", n);
     //	printf("dir: %s\n", dir);

      for(;n>0;n--)
      {
          if(dir[n]=='/')
          {
              n++;
              break;
          }
      }
         char resultDir[PATH_MAX] = {0};
     strncpy(resultDir, dir, n);

string str_dir(resultDir,resultDir+strlen(resultDir));
//cout<<"str_dir="<<str_dir<<endl;
     return str_dir;

 }



