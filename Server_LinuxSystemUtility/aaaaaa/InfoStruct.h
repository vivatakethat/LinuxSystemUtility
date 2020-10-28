
//#include "iphlpapi.h"
//#include "Winsock2.h"
//#include <tlhelp32.h>
//#include <Psapi.h>
#include "stdafx.h"


#define giDangerNumInfo 28  	//
class AInfoFromDriver
{
public:
	UCHAR iDanger;
	UCHAR iRegType;
    string strRegPath;
    string strItem;
    string strPath;

	AInfoFromDriver();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class MCTInfoFromDriver
{
public:
	DWORD dwPId;
	ULONG_PTR dwBaseAddr;
	DWORD dwSize;
	LONGLONG llTime;
    string strPath;

	MCTInfoFromDriver();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class PCTInfoFromDriver
{
public:
	DWORD dwPId;
	DWORD dwPPId;
	UCHAR iCreate;
	LONGLONG llTime;

	PCTInfoFromDriver();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class TCTInfoFromDriver
{
public:
	DWORD dwPId;
	DWORD dwPPId;
	DWORD dwTId;
	LONGLONG llTime;

	TCTInfoFromDriver();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class NCTInfoFromDriver
{
public:
	DWORD dwPId;
	DWORD dwIP;
	DWORD dwPort;
	DWORD uSourcePort;
	LONGLONG llTime;
	UCHAR iType;//TCP 0,UDP 1

	NCTInfoFromDriver();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class IatNode
{
public:
	ULONG_PTR dwNowRav;    
	ULONG_PTR dwRealRav;
	BOOL  bHook;
    string strFuncName;
    string strIatModule;
	IatNode();
};

class IatInfo
{
public:
	ULONG_PTR dwNtdll;
	ULONG_PTR dwUser32;
    string  strNtdll;
    string  strUser32;
	vector<IatNode*> vecNtdll;
	vector<IatNode*> vecUser32;
	IatInfo();
};

class AntorunInfo
{
public:
	UCHAR   iDanger;		  //Σ�ճ̶�, 0Ϊ��ȫ,1Ϊ��ͨ��2Ϊ���� ,3����
	int		iSafe;
    string strItem;   //������
    string strPath;	//·��
    string strRegPath;
    string strTime;
    string strCompany;
    string strDescription;
	AntorunInfo();
};

//iTypeָ�����ͣ�pActionָ����Ӧ�ṹ
//1������
//2Զ�߳�
//3����
//4ssdt
//5shadowssdt
//6idt
//7��Ϣ����
//8port
//9Iat
//10 InjectInfo
class ActionType
{
public:
	UCHAR iType;//0�����̡�1Զ�̣߳�2��Ϣ���ӣ�3apc��
	PVOID pAction;
	ActionType();
};

class FuncAction
{
public:
	int nType;
	DWORD dwSrcPId;
	ULONGLONG dwRetAddr;
	DWORD dwDestPId;
	ULONGLONG dwDestAddr;
	ULONGLONG dwSrcAddr;
	DWORD dwSize;
	DWORD dwPort;
	int nShow;
	UCHAR iTcp;

    string strDll;
    string strHost;
    string strCmdLine;
    string strExePath;
    string strTime;
	FuncAction();
	BOOL CopyData(FuncAction* pAction);
    BOOL GetString(string& strAction);
	BOOL InitInfo(PBYTE pData,int nSize);
	BOOL IsSameData(FuncAction* pAction);
};

class DllInfo
{
public:
	BOOL	bHide;//�Ƿ�����
	BOOL    bLive;//�Ƿ��ڼ���
	int		iSafe;//�Ƿ�ͨ����֤,0δ��֤,1ͨ����֤��2δͨ����֤,3δͨ����֤������
	ULONG_PTR	dwBaseAddr;   // ģʽ��ʼ��ַ
	DWORD	dwSize; //ģ���С
	DWORD   dwPId;
	LONGLONG llTime;
	vector<ActionType*> vecActionType;
	vector<FuncAction*> vecFuncAction;
    vector<string*> vecFuncName;
	
	int aDangerInfo[giDangerNumInfo];//Σ����Ϣ��
    string	strFullPath; //ģ������·��

	DllInfo();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class ProcessInfo
{
public:
	DWORD	dwPId;//����ID
	DWORD	dwPPId;//������ID
	LONGLONG llTime;
	BOOL	bLive;//�Ƿ����
	BOOL	bHide;//�Ƿ�����
	BOOL  bHaveInit;//�Ƿ��Ѿ���ʼ��
	vector<DllInfo*> vecDllInfo;//�����������ع���DLL��Ϣ
//	vector<ActionType*> vecActionType;
	vector<DWORD*> vecDangerPId;
	IatInfo Iat;//IAT��Ϣ

	DllInfo* pExeInfo;
	
    string	strFullPath;//��������·��
    string strFullPathAndParameters;//###��������·���Ͳ���
    string	strExeName;//������

	ProcessInfo();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
	BOOL SetProcessInfo();
	BOOL CheckDllInfo();
	BOOL CheckDllInfoEx();
    DllInfo* GetDllInfoFromPath(string& strPath);
	BOOL GeneralInitDllInfo();
};


class DriverInfo
{
public:
	ULONG_PTR	dwBaseAddr;    // ģ����ʼ��ַ
	ULONG	dwSize; // ģ���С
	BOOL	bHide;//�ж��Ƿ�����
	int		iSafe;
	int aDangerInfo[giDangerNumInfo];//Σ����Ϣ��
    string	strFullPath; //����·��
	vector<ActionType*> vecActionType;
	DriverInfo();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);

};





/*class TrojanAction
{
    string strTime;//��Ϊʱ��
    string strAction;//��Ϊ����
};

class TrojanInfo
{
public:
	DWORD dwPId;//ģ����������
    string strFullPath; //ģ������·��
	int  iSafe; //�Ƿ�ͨ����֤
	BOOL bLive;//�Ƿ�
	int aDangerInfo[giDangerNumInfo];//Σ����Ϣ��
	vector<TrojanAction*> VecNetAction;//������Ϊ�б�
	vector<TrojanAction*> VecMainAction;//����Ϊ�б�
};*/

class DnsInfo
{
public:
	DWORD dwPId;//DNS�������ڽ���
	LONGLONG llTime;
    string strDomain;//DNS��������
	DnsInfo();
};





class MessageInfo
{
public:
	DWORD dwPId;
	DWORD dwTId;
	HANDLE hHandle;
	ULONG_PTR dwHookFuncAddr;
    string strType;
    string strHookDll;
    string strHookProcess;
	MessageInfo();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class PortInfo
{
public:
	DWORD dwPId;
	DWORD dwLocalPort;
	DWORD dwRemotePort;

    string strType;
    string strHost;
    string strLocalHost;
    string strRemoteHost;
    string strState;
    string strFullPath;
	PortInfo();
	BOOL InitInfo(PBYTE pData,DWORD dwLen);
};

class SsdtInfo
{
public:
	DWORD dwIndex;    //����ID
	ULONG_PTR dwRealAddr;    // ��ĵ�ַ
	ULONG_PTR dwNowAddr; // ���ڵ�ַ
	BOOL  bInLineHOOK;
    string strModulePath;
    string strServiceName; //��������
    string strInLineModule;
	
	SsdtInfo();
	BOOL InitInfo(PBYTE pData,int nIndex);
};

class ShadowSsdtInfo
{
public:
	DWORD dwIndex;    //����ID
	ULONG_PTR dwNowAddr; // ���ڵ�ַ
    string strModulePath;

	ShadowSsdtInfo();
	BOOL InitInfo(PBYTE pData,int nIndex);
};

class IdtInfo
{
public:
	DWORD dwIndex;
	ULONG_PTR dwNowAddr;
    string strModulePath;
	IdtInfo();
	BOOL InitInfo(PBYTE pData,int nIndex);
};


class InstallInfo
{
public:
    string strPath;
    string strDir;
	BOOL bDePan;
	BOOL bSafe;
    vector<string*> vecHost;
	InstallInfo();
};

class CheckFileInfo
{
public:
    string strPath;
	BOOL bSafe;
	CheckFileInfo();
};

class DangerType
{
public:
	int nDanger;
	int nScore;
    string strDanger;
	DangerType();
};
class DangerSmart
{
public:
	int nNum;
	int nScore;
	int nType1;
	int nType2;
	DangerSmart();
};

class ScanFinishInfo
{
public:
	void* pInfo;
	BOOL bDll;
	ScanFinishInfo();
};

class TQWINPEINFO
{
public:
    string strPath;
    string strScore;
	TQWINPEINFO();
};

struct TQListString
{
    string strInfo;
	int iColor;
};

struct TQSafeNet
{
    string strExeName;
    string strHost;
};

//�ṹ���� 
typedef struct tagMIB_TCPEXROW
{ 
	DWORD dwState; //tcp����״̬ 
	DWORD dwLocalAddr; //����ip��ַ  
	DWORD dwLocalPort; //�򿪱��ض˿� 
	DWORD dwRemoteAddr; //Զ��ip��ַ 
	DWORD dwRemotePort; //Զ�̶˿ں� 
	DWORD dwProcessId; //���̱�ʶ 
} MIB_TCPEXROW,*PMIB_TCPEXROW; 
 
typedef struct tagMIB_TCPEXTABLE
{ 
	DWORD dwNumEntries; 
	MIB_TCPEXROW table[ANY_SIZE]; 
} MIB_TCPEXTABLE,*PMIB_TCPEXTABLE; 
 
typedef struct tagMIB_UDPEXROW
{  
	DWORD dwLocalAddr;//����ip��ַ 
	DWORD dwLocalPort;//�򿪱��ض˿� 
	DWORD dwProcessId;//���̱�ʶ 
}MIB_UDPEXROW,*PMIB_UDPEXROW; 
 
typedef struct tagMIB_UDPEXTABLE
{ 
	DWORD dwNumEntries; 
	MIB_UDPEXROW table[ANY_SIZE]; 
}MIB_UDPEXTABLE,*PMIB_UDPEXTABLE; 
 

typedef struct HANDLEENTRY{
	PVOID  phead;          //pointer to the real object
	ULONG_PTR  pOwner;          //pointer to owning entity (pti or ppi)
	BYTE  bType;           //type of object
	BYTE  bFlags;       //flags - like destroy flag
	short  wUniq;       //uniqueness count
}HANDLEENTRY,*PHANDLEENTRY;


typedef struct SERVERINFO{ 	//si
	short wRIPFlags ;               //RIPF_ flags
	short wSRVIFlags ;              //SRVIF_ flags
	short wRIPPID ;              //PID of process to apply RIP flags to (zero means all)
	short wRIPError ;              //Error to break on (zero means all errors are treated equal)
	ULONG_PTR cHandleEntries;          //count of handle entries in array
}SERVERINFO,*PSERVERINFO;



//###win7 32λ
//win32k!tagSERVERINFO
//+ 0x000 dwSRVIFlags      : Uint4B
//+ 0x004 cHandleEntries : Uint4B

//###�޸�����
typedef struct SERVERINFO32 { 	//si
	DWORD dwSRVIFlags;               //RIPF_ flags
	ULONG_PTR cHandleEntries;          //count of handle entries in array
}SERVERINFO32, *PSERVERINFO32;


typedef struct SHAREDINFO{
	PSERVERINFO psi;                   //tagSERVERINFO
	PHANDLEENTRY aheList;                //_HANDLEENTRY - handle table pointer
	ULONG pDispInfo;              //global displayinfo
	ULONG_PTR ulSharedDelta;           //delta between client and kernel mapping of ...
}SHAREDINFO,*PSHAREDINFO; 


typedef struct _SYSTEM_MODULE_INFORMATION { 
    ULONG Reserved[2]; 
    ULONG_PTR Base; 
    ULONG Size; 
    ULONG Flags; 
    USHORT Index; 
    USHORT Unknown; 
    USHORT LoadCount; 
    USHORT ModuleNameOffset; 
    CHAR ImageName[256]; 
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION; 

typedef struct _SYSTEM_MODULE_INFORMATIONEX { 
	ULONG Reserved[2]; 
	ULONG_PTR MappedBase; 
	ULONG_PTR Base; 
	ULONG Size; 
	ULONG Flags; 
	USHORT Index; 
	USHORT Unknown; 
	USHORT LoadCount; 
	USHORT ModuleNameOffset; 
	CHAR ImageName[256]; 
} SYSTEM_MODULE_INFORMATIONEX, *PSYSTEM_MODULE_INFORMATIONEx; 

typedef long NTSTATUS;
//typedef __success(return >= 0) LONG NTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemModuleInformation = 11
} SYSTEM_INFORMATION_CLASS;

typedef struct _tagSysModuleList {
    ULONG ulCount;
    SYSTEM_MODULE_INFORMATION smi[1];
} SYSMODULELIST, *PSYSMODULELIST;

typedef struct _tagSysModuleListEx {
	ULONG ulCount;
	SYSTEM_MODULE_INFORMATIONEX smi[1];
} SYSMODULELISTEX, *PSYSMODULELISTEX;

typedef DWORD ( WINAPI *QSIPTR )( SYSTEM_INFORMATION_CLASS, PVOID, DWORD, PULONG );



BOOL TQAnalysisFromFileData();
BOOL TQMoitorFromFileData();

BOOL TQGetInfoFromDriver();
BOOL TQGetStandardPath(string& strStdPath,string& strPath,UCHAR iPath = 0);
DllInfo* GetDllInfoFromPIDName(ULONG dwPID,string& strName);
DllInfo* GetDllInfoFromPIDAddr(ULONG dwPID,ULONG_PTR dwAddr);
DllInfo* GetDllInfoFromPID(ULONG dwPID,string& strPath);
DriverInfo* GetDriverInfoFromPath(string& strPath);

ProcessInfo* GetProcessInfoFromPId(DWORD dwPId);
DllInfo* GetDllInfoFromPID(ULONG dwPID);
BOOL TQGetDllPathByAddr(DWORD dwPId,ULONG_PTR dwAddr, string& strPath);
BOOL TQGetProcessPathByPID(ULONG dwPID,string& strPath);
BOOL TQGetDriverPathByAddr(ULONG_PTR dwAddr, string& strPath);
BOOL TQGetIATInfo(HANDLE hProcess,HMODULE hModule,ProcessInfo* pProcessInfo);
PSYSMODULELIST GetGeneralDriverList();
PSYSMODULELISTEX GetGeneralDriverListEx();
ULONG_PTR LocateSharedInfo();
BOOL TQDeviceIoControl(DWORD dwCode,LPVOID pInBuffer,DWORD dwInSize,
					   LPBYTE* pOutBuffer,LPDWORD pdwReturn);
BOOL GetPortFrom2K() ;
BOOL GetPortFromNo2K();
BOOL GetPortFromWin7AndLater();
BOOL InitSsdtInfo();
BOOL ConformAInfo();
BOOL ConformMCTInfo();
BOOL ConformPCTInfo();
BOOL ConformTCTInfo();
BOOL ConformNCTInfo();
BOOL ConformMsgInfo();
BOOL ConformPortInfo();
BOOL GetAntorunFromReg();
DWORD WINAPI TQRtThread(PVOID pParam);
DWORD WINAPI TQMoitorThread(PVOID pParam);

DWORD WINAPI TQCheckThread(PVOID pParam);
DWORD WINAPI TQTrojanThread(PVOID pParam);
DWORD WINAPI TQViewThread(PVOID pParam);

BOOL GetRtInfo();
BOOL GetMoitorInfo();
ULONG Rav2Raw(PVOID pFileData, ULONG rav);


BOOL TQUninstallProject();
BOOL TQInstallProject();
BOOL TQInitProjectInfo();
BOOL TQGetCmpAndDptInfo(string& strFullPath,
                          string& strCompany,string& strDescription);
BOOL TQGetInstallInfo();
BOOL GetInjectInfo();
BOOL ConformInjectInfo();
BOOL ConformActionType();
int GetTrojanScore(int* pDangerInfo);
int GetCommonDangerString(int* pDangerInfo,string& strDanger);
BOOL GetPortInfo();
void SetListString(string& strInfo, int iColor = 0);
void SetRTNetString(string& strInfo);
void SetMoitorForTree(string& strInfo);

BOOL SetInjectPIdToDri(PVOID pBuffer, ULONG uSize);
BOOL TQInitDirInfo();
BOOL TQGetMenuViewInfo();
BOOL TQGetQuickLaunchInfo();
void AnalyseShortCut(char* szlnkFilePath,char* szExePath);

void TQCloseService(char* szName, BOOL bUnInstall);
BOOL TQCreateAndStartService(char* szName, char* szPath, int nStartType);
BOOL TQStopService(char* szName);

DWORD TQGetOsVersion();

