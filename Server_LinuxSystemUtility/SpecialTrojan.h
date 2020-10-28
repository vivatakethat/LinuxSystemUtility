
#pragma once
#include "stdafx.h"

#include"PublicData.h"


void StartScan();
void USBScan();
 void FileSystemScan();
  void FileSystemLshwScan();
 void MountPointScan();
 void SystemModuleScan();
 void ChromeInternetHistory();
 void FirefoxInternetHistoryScan();
 void AccountSecurityScan();         //12:（国产机设备）操作系统硬件设备
 void CheckInternetConnectScan();
 void  BluetoothScanScan();
 void NetworkAdapterFormsScan();
 void OperationSysteminfoScan();
 void  SystemAccountScan();
 void PackageScan();
 void USBHardWareScan();
 std::string control_inlineimage_post(const std::string & set_value);
 std::string updata_keyword_post(const std::string & keyword);
 std::string flt_file_by_get(const std::string& filename); //second file?
 void SendstrOCR(const std::string strOCR);
void OCR_updatekeyword();
void OCR_KeyWordPost();

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
