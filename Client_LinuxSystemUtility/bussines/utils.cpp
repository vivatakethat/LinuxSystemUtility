#include "utils.h"

#include <QApplication>
#include <QDebug>
#include <QQueue>
#include <QReadWriteLock>

//#include <windows.h>
#include <sys/socket.h>
#include <unistd.h>
#include<iostream>

//#include <tchar.h>

#include "sconv/codec.hpp"

bool auto_roll = true;
QQueue<QString> g_data_queue;
QReadWriteLock g_rw_lock;

//typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
//LPFN_ISWOW64PROCESS fnIsWow64Process;
//BOOL IsWow64()
//{
//  BOOL bIsWow64 = FALSE;
//  fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
//  GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

//  if(NULL != fnIsWow64Process)
//  {
//    if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) {
//    }
//  }
//  return bIsWow64;
//}

//bool is64BitOs()
//{
//    bool ret = false;
//    SYSTEM_INFO si;
//    GetNativeSystemInfo(&si);

//    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
//            || si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
//        ret = true;

//    return ret;
//}


//BOOL enableShutdownPrivilege()
//{
//    HANDLE hProcess = NULL;
//    HANDLE hToken = NULL;
//    LUID uID = {0};
//    TOKEN_PRIVILEGES stToken_Privileges = {0};

//    hProcess = ::GetCurrentProcess();  //获取当前应用程序进程句柄

//    if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken))  //打开当前进程的访问令牌句柄(OpenProcessToken函数调用失败返回值为零)
//        return FALSE;

//    if(!::LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&uID))  //获取权限名称为"SeShutdownPrivilege"的LUID(LookupPrivilegeValue函数调用失败返回值为零)
//        return FALSE;

//    stToken_Privileges.PrivilegeCount = 1;  //欲调整的权限个数
//    stToken_Privileges.Privileges[0].Luid = uID;  //权限的LUID
//    stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  //权限的属性,SE_PRIVILEGE_ENABLED为使能该权限

//    if(!::AdjustTokenPrivileges(hToken,FALSE,&stToken_Privileges,sizeof stToken_Privileges,NULL,NULL))  //调整访问令牌里的指定权限(AdjustTokenPrivileges函数调用失败返回值为零)
//        return FALSE;

//    if(::GetLastError() != ERROR_SUCCESS)  //查看权限是否调整成功
//        return FALSE;

//    ::CloseHandle(hToken);
//    return TRUE;
//}

//void killProcess(const QString &proccess_name)
//{
//    QString strProcess;
//    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//    PROCESSENTRY32 pInfo;
//    pInfo.dwSize = sizeof(pInfo);
//    Process32First(hSnapShot, &pInfo);
//    do {
//        strProcess = QString::fromWCharArray(pInfo.szExeFile);

//        if (strProcess == proccess_name) {
//            QString cmd;
//            cmd = QString("taskkill /F /PID %1 /T").arg(pInfo.th32ProcessID);
//            system(cmd.toStdString().c_str());
//        }
//    } while (Process32Next(hSnapShot, &pInfo));
//}

//void reboot()
//{
//    BOOL bForce = true;
//    enableShutdownPrivilege();  //使能关机特权函数
//    if(bForce)
//       ::ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);  //强制重启
//}

//void setAutoStart()
//{
//    qDebug() << "start write auto start!";
//    HKEY hKey;
//    //找到系统的启动项
////    LPCTSTR lpRun = (wchar_t*)QString("Software\\Microsoft\\Windows\\CurrentVersion\\Run").utf16();
//    //打开启动项Key
//    long lRet;
//    if(IsWow64())
//    {
//        lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE|KEY_WOW64_64KEY, &hKey);
//    }
//    else
//    {
//        lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);
//    }
//    QString appName = QApplication::applicationName();
//    QString appPath = QApplication::applicationFilePath().replace("/", "\\");
//    if (lRet == ERROR_SUCCESS) {

//        DWORD dwRet = appPath.length() * 2;
//        //添加一个子Key,并设置值
//        lRet = RegSetValueExA(hKey, utf8_to_gbk(appName.toStdString()).c_str(), 0, REG_SZ,
//                (BYTE*)utf8_to_gbk(appPath.toStdString()).c_str(), dwRet);
//        //关闭注册表
//        RegCloseKey(hKey);
//        if (lRet != ERROR_SUCCESS) {
//            qDebug() << "write auto start failed!-->" << appName;
//        }
//    } else {
//        qDebug() << "open regedit failed!-->Software\\Microsoft\\Windows\\CurrentVersion\\Run";
//    }
//}


//void canCelAutoStart()
//{
//    HKEY hKey;
////    LPCTSTR lpRun = (wchar_t*)QString("Software\\Microsoft\\Windows\\CurrentVersion\\Run").utf16();
////    //打开启动项Key
////    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
//    long lRet;
//    if(IsWow64())
//    {
//        lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE|KEY_WOW64_64KEY, &hKey);
//    }
//    else
//    {
//        lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);
//    }
//    QString appName = QApplication::applicationName();
//    if (lRet == ERROR_SUCCESS) {
//        lRet = RegDeleteValueA(hKey, appName.toStdString().c_str());
//        //关闭注册表
//        RegCloseKey(hKey);
//        if (lRet != ERROR_SUCCESS) {
//            qDebug() << "cancel auto start failed!" << appName;
//        }
//    } else {
//        qDebug() << "open regedit failed!-->Software\\Microsoft\\Windows\\CurrentVersion\\Run";
//    }
//}

void setRowContentColor(const QString content_color, QTableWidget *table, int set_row)
{
    if (table == nullptr || set_row >= table->rowCount() || set_row < 0)
        return;

    QBrush brush;
    if (content_color == "1") {
        brush = QBrush(Qt::blue);
    } else if (content_color == "2") {
        brush.setColor(QColor(238, 118, 0, 255));
    } else if (content_color == "3") {
        brush = QBrush(Qt::red);
    }

    int column_count = table->columnCount();
    for (int i = 0; i < column_count; i++) {
        table->item(set_row, i)->setForeground(brush);
    }
}

void openFileDirFromFullPath(const QString &full_path)
{
    if (full_path.isEmpty())
        return;
    qDebug() << "file_path:" << full_path;

    QString file_path = QString(" /select,%1").arg(full_path);

    //###进程分析里去掉进程路径后面的参数
    file_path=file_path.toLower();
    int exeaddr=0;
    QString exe=".exe";
    if((exeaddr=file_path.indexOf(exe))>0)
    {
        file_path=file_path.left(exeaddr+4);
    }


//    //###防止文件重定向的干扰
//    DisableWow64FsRedirection(TRUE);
//    ShellExecuteA(0, "open", "explorer.exe", utf8_to_gbk(file_path.toStdString()).c_str(), NULL, SW_SHOWDEFAULT);
//    DisableWow64FsRedirection(FALSE);
}

void pushData(const QString &data)
{

    g_rw_lock.lockForWrite();
    g_data_queue.enqueue(data);
    g_rw_lock.unlock();
}

QString popData()
{
    QString data;
    g_rw_lock.lockForRead();
    if (!g_data_queue.isEmpty())
        data = g_data_queue.dequeue();
    g_rw_lock.unlock();

    return data;
}

//BOOL DisableWow64FsRedirection(BOOL disable){
//    typedef BOOL (__stdcall *FuncIsWow64Process)(HANDLE, PBOOL);
//    typedef BOOL (__stdcall *FuncWow64DisableWow64FsRedirection)(PVOID*);
//    typedef BOOL (__stdcall *FuncWow64RevertWow64FsRedirection)(PVOID);
//    static FuncIsWow64Process xIsWow64Process = NULL;
//    static FuncWow64DisableWow64FsRedirection xDisableFsRedirection = NULL;
//    static FuncWow64RevertWow64FsRedirection xRevertFsRedirection = NULL;
//    static BOOL iswow64 = 0;
//    if(!xIsWow64Process){
//        xIsWow64Process = (FuncIsWow64Process)::GetProcAddress(::GetModuleHandleA("kernel32.dll"), "IsWow64Process");
//        if(!xIsWow64Process)
//            return TRUE;
//        if(!xIsWow64Process(GetCurrentProcess(), &iswow64))
//            return FALSE;
//    }
//    if(!iswow64)
//        return TRUE;
//    if(!xDisableFsRedirection){
//        xDisableFsRedirection = (FuncWow64DisableWow64FsRedirection)::GetProcAddress(::GetModuleHandleA("kernel32.dll"), "Wow64DisableWow64FsRedirection");
//        if(!xDisableFsRedirection)
//            return FALSE;
//    }
//    if(!xRevertFsRedirection){
//        xRevertFsRedirection = (FuncWow64RevertWow64FsRedirection)::GetProcAddress(::GetModuleHandleA("kernel32.dll"), "Wow64RevertWow64FsRedirection");
//        if(!xRevertFsRedirection)
//            return FALSE;
//    }
//    static __declspec(thread) void* old_value = NULL;
//    static __declspec(thread) BOOL disabled = FALSE;
//    if(disable){
//        if(disabled)
//            return TRUE;
//        if(xDisableFsRedirection(&old_value)){
//            disabled = TRUE;
//            return TRUE;
//        }else
//            return FALSE;
//    }else{
//        if(!disabled)
//            return TRUE;
//        if(xRevertFsRedirection(old_value)){
//            disabled = FALSE;
//            return TRUE;
//        }else
//            return FALSE;
//    }
//}

