#include "stdafx.h"
#include "SpecialTrojan.h"
//#include "InfoStruct.h"

#include "SpecialTrojanServer.h"
#include "jsoncpp/json/json.h"


extern char gszProjectPath[MAX_PATH];

using namespace std;

bool bFirstRun = true;
bool bWriteLog = false;
bool bOnXinXi = false;
bool bFirstSendMessage = true;
extern bool gbCanTrojanScan;
extern HANDLE ghListStringEvent;
extern HANDLE ghTreeUpdataEvent;

//extern vector<ProcessInfo*> vecProInfo;
//extern vector<DriverInfo*> vecDriInfo;
//extern vector<AntorunInfo*> vecAtrInfo;
//extern vector<MessageInfo*> vecMsgInfo;
//extern vector<PortInfo*> vecPortInfo;

//extern vector<SsdtInfo*> vecSsdtInfo;
//extern vector<ShadowSsdtInfo*> vecShadowSsdtInfo;
//extern vector<IdtInfo*> vecIdtInfo;

extern char gszHostName[MAX_PATH];
extern char gszLocalIP[32];
extern DWORD gdwVersion;
//extern vector<ScanFinishInfo*> vecSfInfo;
//extern vector<ScanFinishInfo*> vecScanFinishInfo;
extern char gszExplorer[MAX_PATH];
extern char gszServices[MAX_PATH];
//extern char gszWindowsPath[MAX_PATH];
extern int gnDangerYellowScore;
extern int gnDangerRedScore;
//extern vector<DangerType*> vecDangerType;
extern bool gbHaveOnceScan;

//extern vector<TQListString*> vecListString;
//extern vector<CString*> vecRTNetString;
//extern vector<CString*> vecMoitorString;

extern HANDLE ghRTNetStringEvent;
extern HANDLE ghMoitorEventForTree;

extern ULONG gdwParentPID;
extern ULONG gdwCurPID;


void ViewProcessInfo(void* pViewInfo)
{

	Json::Value info;
	info["msg"] = "ClearProcessInfo";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "ProcessInfo";
	info["ProcessName"] = "sfsf.exe";
	info["PID"] = "343243";
	info["PPID"] = "634534";
	info["IsAuth"] = "待认证";
	info["Path"] = "c:\\gsdfe.exe";
	info["Color"] = 1;
	SendMessageToClient(info.toStyledString());


	info.clear();
	info["msg"] = "ProcessInfo";
	info["ProcessName"] = "ggggg.exe";
	info["PID"] = "3433243";
	info["PPID"] = "6344534";
	info["IsAuth"] = "待认证";
	info["Path"] = "c:\\windows\\g33sdfe.exe";
	info["Color"] = 1;
	SendMessageToClient(info.toStyledString());

}

void ViewHistoryProcessInfo(void* pViewInfo)
{
    //vector<ProcessInfo*>* pVecInfo = (vector<ProcessInfo*>*)pViewInfo;

	Json::Value info;
	info["msg"] = "ClearHistoryProcessInfo";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "HistoryProcessInfo";
	info["ProcessName"] = "gsdfe.exe";
	info["PID"] = "34234";
	info["PPID"] = "7777";
	info["IsAuth"] = "通过";
	info["Path"] = "c:\\gggg.exe";
	info["IsHide"] = "否";
	info["Color"] = 0;
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "HistoryProcessInfo";
	info["ProcessName"] = "gsdrt5fe.exe";
	info["PID"] = "3456234";
	info["PPID"] = "77577";
	info["IsAuth"] = "通过";
	info["Path"] = "c:\\gggfrrrg.exe";
	info["IsHide"] = "否";
	info["Color"] = 0;
	SendMessageToClient(info.toStyledString());
}

void ViewDriverInfo(void* pViewInfo)
{
	Json::Value info;
	info["msg"] = "ClearDriverInfo";
	SendMessageToClient(info.toStyledString());

	info["msg"] = "DriverInfo";
	info["DriverPath"] = "C:\\windows\\system32\\Drivers\\gg.sys";
	info["BaseAddress"] = "0x744535";
	info["Size"] = "5435345";
	info["IsHide"] = "否";
	info["IsAuth"] = "通过";
	info["Color"] = 0;
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "DriverInfo";
	info["DriverPath"] = "C:\\windows\\\\Drivers\\gxxx.sys";
	info["BaseAddress"] = "0x7445535";
	info["Size"] = "5438845";
	info["IsHide"] = "否";
	info["IsAuth"] = "通过";
	info["Color"] = 0;
	SendMessageToClient(info.toStyledString());
}

void ViewAntorunInfo(void* pViewInfo)
{

	Json::Value info;
	info["msg"] = "ClearAutorunInfo";
	SendMessageToClient(info.toStyledString());

	info["msg"] = "AutoRunInfo";
	info["RunItem"] = "gggsdfe";
	info["RegPath"] = "c:\\ggg.exe";
	info["FilePath"] = "c:\\ggg.exe";
	info["IsAuth"] = "通过";
	info["Describe"] = "配置开机级别";
	info["Company"] = "microsoft";
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "AutoRunInfo";
	info["RunItem"] = "g111e";
	info["RegPath"] = "c:\\ggg\\.exe";
	info["FilePath"] = "c:\\gxxxxgg.exe";
	info["IsAuth"] = "通过";
	info["Describe"] = "配置电源管理";
	info["Company"] = "microsoft";
	SendMessageToClient(info.toStyledString());
}

void ViewMessageInfo(void* pViewInfo)
{
	Json::Value info;
	info["msg"] = "ClearMessageHookInfo";
	SendMessageToClient(info.toStyledString());

	info["msg"] = "MessageHookInfo";
	info["Handle"] = "34343";
	info["HookType"] = "inline";
	info["HookFuncAddr"] = "0x4534534543";
	info["PID"] = "34234";
	info["TID"] = "234234";
	info["HookModule"] = "C:\\windows\\ddd.exe";
	info["ProcessPath"] = "C:\\windows\\ggg.dll";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "MessageHookInfo";
	info["Handle"] = "34366643";
	info["HookType"] = "iat";
	info["HookFuncAddr"] = "0x4534543";
	info["PID"] = "34234";
	info["TID"] = "234234";
	info["HookModule"] = "C:\\windows\\google.exe";
	info["ProcessPath"] = "C:\\windows\\ms.dll";
	SendMessageToClient(info.toStyledString());
}

void ViewPortInfo(void* pViewInfo)
{
	Json::Value info;
	info["msg"] = "ClearPortInfo";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "PortInfo";
	info["Protocol"] = "TCP";
	info["LocalAddr"] = "192.168.6.110";
	info["RemoteAddr"] = "192.168.6.220";
	info["Status"] = "listion";
	info["PID"] = "222";
	info["ProcessPath"] = "C:\\gsfsfsf.exe";
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "PortInfo";
	info["Protocol"] = "TCP";
	info["LocalAddr"] = "192.168.6.100";
	info["RemoteAddr"] = "192.168.6.200";
	info["Status"] = "established";
	info["PID"] = "222";
	info["ProcessPath"] = "C:\\gsfsfsf.exe";
	SendMessageToClient(info.toStyledString());

}

void ViewSsdtInfo(void* pViewInfo)
{

	int nColumn = 0;
    //SsdtInfo* pInfo = NULL;

	Json::Value info;
	info["msg"] = "ClearSSDTInfo";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "SSDTInfo";
	info["Num"] = "22";
	info["FuncName"] = "ZwCreateFile";
	info["OriginalAddr"] = "0x7ff36345";
	info["NowAddr"] = "0x7ff36345";
	info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";
	info["InlineHookModule"] = "";
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "SSDTInfo";
	info["Num"] = "232";
	info["FuncName"] = "ZwWriteFIle";
	info["OriginalAddr"] = "0x7ff36345";
	info["NowAddr"] = "0x7ff36345";
	info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";
	info["InlineHookModule"] = "";
	SendMessageToClient(info.toStyledString());
}

void ViewShadowSsdtInfo(void* pViewInfo)
{

	Json::Value info;
	info["msg"] = "ClearShadowSSDTInfo";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "ShadowSSDTInfo";
	info["Num"] = "44";
	info["NowAddr"] = "0x7ff36345";
	info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";

	info.clear();
	info["msg"] = "ShadowSSDTInfo";
	info["Num"] = "77";
	info["NowAddr"] = "0x7ff77345";
	info["Module"] = "C:\\windows\\system32\\ntostrnl.exe";

	SendMessageToClient(info.toStyledString());
}

void ViewIdtInfo(void* pViewInfo)
{

	Json::Value info;
	info["msg"] = "ClearIDTInfo";
	SendMessageToClient(info.toStyledString());

	info["msg"] = "IDTInfo";
	info["Num"] = "46454";
	info["NowAddr"] ="56756756";
	info["Module"] = "C:\\Windows\\system32\\ggg.exe";
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "IDTInfo";
	info["Num"] = "4655454";
	info["NowAddr"] = "5675000";
	info["Module"] = "C:\\Windows\\system32\\telnet.exe";
	SendMessageToClient(info.toStyledString());
}

void ViewIatInfo(void* pViewInfo)
{

	Json::Value info;
	info["msg"] = "ClearIATInfo";
	SendMessageToClient(info.toStyledString());


	info["msg"] = "IATInfo";
	info["ProcessPath"] = "C:\\windows\sdf.exe";
	info["PID"] = "3434";
	info["OriginalAddr"] = "0x45755354";
	info["JumpAddr"] = "0x543454264";
	info["IatHookFunc"] = "CreateFile";
	info["IatHookModule"] = "sfsdfsdfs.exe";
	SendMessageToClient(info.toStyledString());

	info.clear();
	info["msg"] = "IATInfo";
	info["ProcessPath"] = "C:\\windows\tim.exe";
	info["PID"] = "377434";
	info["OriginalAddr"] = "0x457555654";
	info["JumpAddr"] = "0x543884264";
	info["IatHookFunc"] = "ReadFile";
	info["IatHookModule"] = "hide.dll";
	SendMessageToClient(info.toStyledString());

}

bool ViewScanFinishInfo(void* pViewInfo)
{
	Json::Value info;
	info["msg"] = "ClearScanFinishInfo";
	SendMessageToClient(info.toStyledString());

	info["msg"] = "ScanTrojanResult";
	info["Num"] ="33";
	info["TrojanInfo"] = "所在进程PId:22[explorer.exe], 模块 : c : \\windows\\gg.dll - 危险程度:250\n";
	SendMessageToClient(info.toStyledString());

    return true;
}

DWORD WINAPI TQListStringThread(PVOID pParam)
{
	while (1)
	{
        if (gbCanTrojanScan && !bOnXinXi)//###信息处理完就通知到这里gbCanTrojanScan=true开始执行
		{
			if (bFirstSendMessage)
			{
//				ViewProcessInfo(&vecProInfo);
//				ViewHistoryProcessInfo(&vecProInfo);
//				ViewIatInfo(&vecProInfo);
//				ViewPortInfo(&vecPortInfo);

//				ViewDriverInfo(&vecDriInfo);
//				ViewAntorunInfo(&vecAtrInfo);
//				ViewMessageInfo(&vecMsgInfo);
//				ViewSsdtInfo(&vecSsdtInfo);
//				ViewShadowSsdtInfo(&vecShadowSsdtInfo);
//				ViewIdtInfo(&vecIdtInfo);
                bOnXinXi = true;
                bFirstSendMessage = false;

				Json::Value info;//###通知前台首次发送全部信息完成，接下来只发送实时进程和端口信息
				info["msg"] = "FirstSendMessage";
				SendMessageToClient(info.toStyledString());

				info.clear();
				info["msg"] = "NetInfo";
				info["RealTimeNetInfo"] = "PID为553的进程域名解析地址:www.baidu.com, 时间 : 2019.4.22 08:33:20";
				SendMessageToClient(info.toStyledString());


				info.clear();
				info["msg"] = "MonitorInfo";
				info["RealTimeMonitorInfo"] = "PId:343[iexplorer.exe], 建立PE格式文件, 文件路径为c:\\windows\temp.exe, 时间为2019.9.24 09:34:20";
				SendMessageToClient(info.toStyledString());
				ViewScanFinishInfo(NULL);



			}
			else
			{
                //ViewProcessInfo(&vecProInfo);
                //ViewPortInfo(&vecPortInfo);
                bOnXinXi = true;
			}
		}

		//###界面解析vecListString服务器传过来的信息
		Json::Value info;
        //WaitForSingleObject(ghListStringEvent, INFINITE);
		int iColor = 0;
//		for (int i = 0; i<vecListString.size(); i++)
//		{
//			TQListString* pPush = vecListString[i];
//			if (pPush->iColor == 1)
//			{
//				iColor = 1;
//			}
//			if (pPush->iColor == 2)
//			{
//				iColor = 3;
//			}
//			info["msg"] = "TrojanScanInfo";
//			info["ScanInfoDescribe"] = pPush->strInfo.GetBuffer();

//			char sColor[10] = { 0 };
//			//_itoa(iColor, sColor, 10);
//			info["Color"] = sColor;

//			SendMessageToClient(info.toStyledString());
//			delete pPush;
//		}
        //vecListString.clear();
        //SetEvent(ghListStringEvent);



	}
	return 0;
}

void StartScan()
{

	if (bFirstRun)
	{
		Json::Value info;
		info["msg"] = "TrojanScanInfo";
		info["ScanInfoDescribe"] = "正在初始化数据...";
		SendMessageToClient(info.toStyledString());

        std::thread thrd = std::thread(TQListStringThread);
        //CreateThread(0, 0, TQListStringThread, NULL, 0, 0);
        //CreateThread(0, 0, TQViewThread, NULL, 0, 0);

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

