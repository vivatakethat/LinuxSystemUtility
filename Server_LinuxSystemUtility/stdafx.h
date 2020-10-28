// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

//#include "targetver.h"

#include <stdio.h>
//#include <tchar.h>

//#include <WinSock2.h>
//#include <windows.h>
//#include <atlstr.h>
// TODO:  在此处引用程序需要的其他头文件
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <limits.h>
using namespace std;

#define USE_STANDALONE_ASIO
#define ASIO_STANDALONE

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef ANY_SIZE
#define ANY_SIZE 1
#endif

#define MAX_PATH          260
#define WINAPI  __stdcall

typedef unsigned char UCHAR;
typedef unsigned int BOOL;
typedef BYTE *PBYTE;
typedef unsigned long DWORD;

typedef unsigned long long ULONG_PTR, *PULONG_PTR;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef void *PVOID,*HANDLE;
typedef unsigned short USHORT;
typedef char CHAR;









