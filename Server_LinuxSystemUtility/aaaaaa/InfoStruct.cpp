#include "stdafx.h"
#include "InfoStruct.h"

extern HANDLE ghListStringEvent;
extern HANDLE ghTreeUpdataEvent;
extern vector<ProcessInfo*> vecProInfo;
extern vector<DriverInfo*> vecDriInfo;
extern vector<AntorunInfo*> vecAtrInfo;

extern vector<MessageInfo*> vecMsgInfo;
extern vector<PortInfo*> vecPortInfo;

extern vector<SsdtInfo*> vecSsdtInfo;
extern vector<ShadowSsdtInfo*> vecShadowSsdtInfo;
extern vector<IdtInfo*> vecIdtInfo;
extern BOOL gbCanTrojanScan;

extern HANDLE ghRTNetStringEvent;
extern HANDLE ghMoitorEventForTree;

extern ULONG gdwParentPID;
extern ULONG gdwCurPID;

vector<TQListString*> vecListString;
vector<CString*> vecRTNetString;
vector<CString*> vecMoitorString;

#ifdef _DEBUG
#define KdPrint(_x_) DbgPrint _x_
#else
#define KdPrint(_x_)
#endif
extern VOID DbgPrint(const char*format, ...);

typedef BOOL (WINAPI *ENUMPROCESSMODULEEX)(
_In_  HANDLE hProcess,
_Out_writes_bytes_(cb)  HMODULE *lphModule,
_In_  DWORD cb,
_Out_  LPDWORD lpcbNeeded,
_In_  DWORD dwFilterFlag
);

typedef struct tagMODULEENTRY
{
	ULONG_PTR BaseAddr;        // Base addr
	char    szPath[MAX_PATH];
} MODULEENTRY;


vector<AInfoFromDriver*> vecAIFD;
vector<MCTInfoFromDriver*> vecMIFD;
vector<PCTInfoFromDriver*> vecPIFD;
vector<TCTInfoFromDriver*> vecTIFD;
vector<NCTInfoFromDriver*> vecNIFD;

vector<AntorunInfo*> vecAtrInfo;
vector<ProcessInfo*> vecProInfo;

vector<ProcessInfo*> vecHistoryProInfo;

vector<DriverInfo*> vecDriInfo;

vector<MessageInfo*> vecMsgInfo;
vector<PortInfo*> vecPortInfo;

vector<SsdtInfo*> vecSsdtInfo;
vector<ShadowSsdtInfo*> vecShadowSsdtInfo;
vector<IdtInfo*> vecIdtInfo;

vector<InstallInfo*> vecInstallInfo;
vector<FuncAction*> vecActionInfo;

extern char gszNtoskrnlPath[MAX_PATH];
extern DWORD gdwVersion;

extern char gszHostName[MAX_PATH];
extern char gszLocalIP[32];
extern char gszSystemPath[MAX_PATH];


PCTInfoFromDriver::PCTInfoFromDriver()
{
	dwPId = 0;
	dwPPId = 0;
	llTime = 0;
	iCreate = 1;
}

TCTInfoFromDriver::TCTInfoFromDriver()
{
	dwPId = 0;
	dwPPId = 0;
	dwTId = 0;
	llTime = 0;
}


NCTInfoFromDriver::NCTInfoFromDriver()
{
	dwPId = 0;
	dwIP= 0;
	dwPort = 0;
	iType = 0;
	llTime = 0;
}


MCTInfoFromDriver::MCTInfoFromDriver()
{
	dwPId = 0;
	dwBaseAddr = 0;
	dwSize = 0;
	strPath = "";
	llTime = 0;
}

AInfoFromDriver::AInfoFromDriver()
{
	iRegType = 222;
	iDanger = 0;
	strPath = "";
	strItem = "";
	strRegPath = "";
}

IatInfo::IatInfo()
{
	dwNtdll = 0;
	dwUser32 = 0;
	strNtdll = "";
	strUser32 = "";
}

IatNode::IatNode()
{
	dwNowRav = 0;    
	dwRealRav = 0;
	bHook = FALSE;
	strFuncName = "";
	strIatModule = "";
}

ActionType::ActionType()
{
	iType = 0;
	pAction = NULL;
}

ProcessInfo::ProcessInfo()
{
	dwPId = 0;
	dwPPId = 0;
	bLive = FALSE;
	bHide = FALSE;
	llTime = 0;
	bHaveInit = FALSE;
	strFullPath = "";
	strFullPathAndParameters = "";
	strExeName = "";
	pExeInfo = NULL;
}



DllInfo::DllInfo()
{
	bHide = FALSE;
	bLive = FALSE;
	iSafe = 0;
	dwBaseAddr = 0;
	dwSize = 0;
	dwPId = 0;
	llTime = 0;
	strFullPath = "";
	ZeroMemory(aDangerInfo,giDangerNumInfo*sizeof(int));
}



DriverInfo::DriverInfo()
{
	dwBaseAddr = 0;
	dwSize = 0;
	bHide = FALSE;
	strFullPath = "";
	iSafe = 0;
	ZeroMemory(aDangerInfo,giDangerNumInfo*sizeof(int));
}


AntorunInfo::AntorunInfo()
{
	iDanger = 0;
	strItem = "";
	strPath = "";
	strRegPath = "";
	strTime = "";
	strCompany = "";
	strDescription = "";
	iSafe = 0;
}


MessageInfo::MessageInfo()
{
	dwPId = 0;//###进程id
	dwTId = 0;//###线程id
	hHandle = 0;//###句柄
	dwHookFuncAddr = 0;//###HOOK函数地址
	strType = "";//###钩子类型
	strHookDll = "";//###钩子模块
	strHookProcess = "";//###申请钩子的进程
}

PortInfo::PortInfo()
{
	dwPId = 0;
	dwLocalPort = 0;
	strType = "";
	strLocalHost = "";
	strRemoteHost = "";
	strState = "";
	strFullPath = "";
	dwRemotePort = 0;
	strHost = "";
}

SsdtInfo::SsdtInfo()
{
	dwIndex = 0;
	dwRealAddr = 0;
	dwNowAddr = 0;
	bInLineHOOK = FALSE;
	strModulePath = "";
	strServiceName = "";
	strInLineModule = "";
}

ShadowSsdtInfo::ShadowSsdtInfo()
{
	dwIndex = 0;
	dwNowAddr = 0;
	strModulePath = "";
}

IdtInfo::IdtInfo()
{
	dwIndex = 0;
	dwNowAddr = 0;
	strModulePath = "";
}


CheckFileInfo::CheckFileInfo()
{
	bSafe = FALSE;
	strPath = "";
}


InstallInfo::InstallInfo()
{
	strPath = "";
	strDir = "";
	bSafe = TRUE;
	bDePan = TRUE;
}

DangerType::DangerType()
{
	nDanger = 0;
	nScore = 0;
	strDanger = "";
}

DangerSmart::DangerSmart()
{
	nNum = 0;
	nScore = 0;
	nType1 = 0;
	nType2 = 0;
}

ScanFinishInfo::ScanFinishInfo()
{
	pInfo = NULL;
	bDll = TRUE;
};

TQWINPEINFO::TQWINPEINFO()
{	
	strPath = "";
	strScore = "";
};


FuncAction::FuncAction()
{
	nType = 0;
	dwSrcPId = 0;
	dwRetAddr = 0;
	dwDestPId = 0;
	dwDestAddr = 0;
	dwSrcAddr = 0;
	dwSize = 0;
	dwPort = 0;
	nShow = 0;
	iTcp = 0;
	strDll = "";
	strHost = "";
	strCmdLine = "";
	strExePath = "";
	strTime = "";

}


void SetListString(CString& strInfo,int iColor)
{
	TQListString* pPush = new TQListString;
	pPush->strInfo = strInfo;
	pPush->iColor = iColor;
	WaitForSingleObject(ghListStringEvent, INFINITE);//###为了同步一个push_back完成后另一个才能push_back
	vecListString.push_back(pPush);
	SetEvent(ghListStringEvent);//###通知下一个
}






