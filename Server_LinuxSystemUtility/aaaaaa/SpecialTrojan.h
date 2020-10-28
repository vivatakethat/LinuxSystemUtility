
#pragma once
#include "stdafx.h"

void StartScan();

void Reboot();

void StopScan();

void ShowDll(DWORD dwPid);

void Exit();

void OutputReportForm();


void ViewIatInfo(void* pViewInfo);
void ViewDriverInfo(void* pViewInfo);
void ViewAntorunInfo(void* pViewInfo);
void ViewMessageInfo(void* pViewInfo);
void ViewSsdtInfo(void* pViewInfo);
void ViewShadowSsdtInfo(void* pViewInfo);
void ViewIdtInfo(void* pViewInfo);
void ViewHistoryProcessInfo(void* pViewInfo);
