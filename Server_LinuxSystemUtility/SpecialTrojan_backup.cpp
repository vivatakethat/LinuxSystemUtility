//#include "stdafx.h"
//#pragma execution_character_set("utf-8")
#include "SpecialTrojan.h"

#include "SpecialTrojanServer.h"
#include "jsoncpp/json/json.h"
#include <semaphore.h>
bool  recyRemveDelimitStr(string &str);
using namespace std;

bool bFirstRun = true;
bool bWriteLog = false;
bool bOnXinXi = false;
bool bFirstSendMessage = true;


bool gbHaveOnceScan = false;
bool gbCanTrojanScan = false;

//HANDLE ghListStringEvent;
sem_t sem;
void* TQViewThread(void *pParam);
void SetListString(string strInfo, int iColor = 0);


 static std::shared_ptr< GetSysInfoImpl>ptr_SysInfo=toGetDynamicPtr("./libGetsystemInfo.so");


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
           // info["msg"] = "TrojanScanInfo";
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
        pthread_create(&thread,NULL,TQListStringThread,NULL);//take:send
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
         //       info["msg"] = "Networkhardware";
         //          info["msg"] = "TrojanScanInfo";
               //     info["ScanInfoDescribe"] = "toGetNetworkHarware";
for(int i=0;i<get_USBEvents.size();i++)
{

                        info["msg"]="USBHistoryEvents";
                            info["msg_subtype"]="conn";
                    info["conn"]=get_USBEvents[i].conn;
     SendMessageToClient(info.toStyledString());
info.clear();
         info["msg"]="USBHistoryEvents";
         info["msg_subtype"]="disconn"
;     info["disconn"]=get_USBEvents[i].disconn;
SendMessageToClient(info.toStyledString());
info.clear();
         info["msg"]="USBHistoryEvents";
         info["pid"]=get_USBEvents[i].pid;
SendMessageToClient(info.toStyledString());
info.clear();
         info["msg"]="USBHistoryEvents";
                                info["prod"]=get_USBEvents[i].prod;
                                SendMessageToClient(info.toStyledString());
                                info.clear();
         info["msg"]="USBHistoryEvents";
                     info["manufact"]=get_USBEvents[i].manufact;
                     SendMessageToClient(info.toStyledString());
                     info.clear();
                              info["msg"]="USBHistoryEvents";
              info["serial"]=get_USBEvents[i].serial;
              SendMessageToClient(info.toStyledString());
              info.clear();

             info["ScanInfoDescribe"] = info["manufact"];
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
