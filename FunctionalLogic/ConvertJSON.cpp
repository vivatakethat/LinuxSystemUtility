//ConvertJson.cpp
#include "ConvertJSON.h"
#include "PublicData.h"
#include <iostream>
#include <time.h>
#include <sys/time.h>
 #include <unistd.h>
 #include<stdio.h>
#define LOG_MAX_BUF_SIZE 10240
char m_cBuf1[10 * LOG_MAX_BUF_SIZE]; //JSON记录
using namespace std;

int GetJSON::isConnectInternet_JSON()//检查联网情况
{
 m_cGetSysinfo.checkInternetConnect(vecConnectionInternet);
			if(vecConnectionInternet.empty())
			{
				printf("No HostName Connection Check History!");
				return -1;
			}


			for(int i=0;i<vecConnectionInternet.size();i++)
			{
					char connectHostname[100];
		memset(&connectHostname, 0x00, sizeof(connectHostname));
		sprintf(connectHostname,"HostnameConnection[%d]", i);
	 	json connHostname_json;
	 connHostname_json[connectHostname] =
	 {
			{"destination:",vecConnectionInternet[i].destination},
					{"ConnectionResult:",vecConnectionInternet[i].result},
	 };
	 	m_PreJSON_File(m_CheckInternet, &connHostname_json, CONNECTIONHOSTNAME);
		 connHostname_json.clear();
			}
return 0;
}
int GetJSON::GetFilesystem_Json()
{
	  vecFileSystem = m_cGetSysinfo.getFileSystemInfo_InC();
	int len = vecFileSystem.size();
	if(len==0)
	{
		printf("There is no FileSystem in this System!\n");
		return 0;
	}
	for (int i = 0; i < len; i++)
	{
		char list_hdd[100];
		memset(&list_hdd, 0x00, sizeof(list_hdd));
		sprintf(list_hdd, "hdd%d", i);
	 	json hdd_json;
	 	hdd_json[list_hdd] =
			{
				{"DevPath", vecFileSystem[i].DevPath},
				{"SerialNumber", vecFileSystem[i].SerialNumber},
				{"ManufacturerModel", vecFileSystem[i].ManufacturerModel},
				{"FirmwareRevision", vecFileSystem[i].FirmwareRevision},
				{"TransportType", vecFileSystem[i].TransportType},
				{"MaximumRPM", vecFileSystem[i].MaximumRPM},
				{"Cylinders", vecFileSystem[i].Cylinders},
				{"Capacity", vecFileSystem[i].Capacity},
					{"DriverInterFaCeType", vecFileSystem[i].DriverInterFaceType},
						{"Product_id", vecFileSystem[i].Product_id},
							{"Product_ver", vecFileSystem[i].Product_ver},
									{"SCS_Vendor", vecFileSystem[i].SCSI_Vendor},
			};
		m_PreJSON_File(m_FileSystemInfoJSON_Name, &hdd_json, FILESYSTEM_CHECK);
		hdd_json.clear();
	}
	return 0;
}
int GetJSON::GetSysInfo_Json()

{ st_Sysinfo = m_cGetSysinfo.getOperationSystemInfo();
	SysInfo_JSON_Array["SysInfo"] = {
		{"OS", st_Sysinfo.OS},
		{"CPU", st_Sysinfo.CPU},
		{"RAW", st_Sysinfo.RAM},
		{"LinuxkernelVersion", st_Sysinfo.LinuxkernelVersion},
		{"Architecture", st_Sysinfo.Architecture},
		{"Hostname", st_Sysinfo.Hostname},
		{"SystemPath", st_Sysinfo.SystemPath},
	};
	m_PreJSON_File(m_SystemInfoJSONdata_Name, &SysInfo_JSON_Array, COMPUTER_SYSTEM_INFOMATION);
	
	return 0;
}
int GetJSON::GetSystemAccountSecurityCheck_Json(){
	
	  vecSysAccountSecurity= m_cGetSysinfo.getSysAccountSecurityCheck();
int n_Size = vecSysAccountSecurity.size();
if(n_Size==0)
{
	printf("There is no any AccountInfo !");
	return 0;
}

	for (int i = 0; i <n_Size; i++)
	{
		char buffer_User[100];
		memset(&buffer_User, 0x00, sizeof(buffer_User));
		sprintf(buffer_User, "user%d", i);
	 	json user_json;
	 	user_json[buffer_User] =
			{
				{"UserName", vecSysAccountSecurity[i].UserName},
				{"LastPasswordChange", vecSysAccountSecurity[i].LastPasswordChange},
					{"PasswordExpires ", vecSysAccountSecurity[i].PasswdExpires},
					{"PasswordInactive ", vecSysAccountSecurity[i].PasswordInactive},
				 {"AccountExpires ", vecSysAccountSecurity[i].AccountExpires},
				 {"Minimum_number_of_days_between_password_change", vecSysAccountSecurity[i].Minimum_number_of_days_between_password_change},
				 {" Maximum_number_of_days_between_password_change",vecSysAccountSecurity[i].Maximum_number_of_days_between_password_change},
				 	 {" Number_of_days_of_warning_before_password_expires",vecSysAccountSecurity[i].Number_of_days_of_warning_before_password_expires},
			};
		m_PreJSON_File(m_SystemAccountSecurityCheck, &user_json, SYSTEMACCOUNTSECURITYCHECK);
		user_json.clear();
		
	}
	return 0;
}
int GetJSON::GetSystemModuleSecurityCheck_Json()
{
	vecSystemModuleSecurityCheck=m_cGetSysinfo.getSystemModuleSecurityCheck(); 

int n_Size=vecSystemModuleSecurityCheck.size();

if(n_Size==0)
{
	printf("This is cannout find any Module!");
	return 0;
}
for (int i = 0; i <n_Size; i++)
	{
		char buffer_Modules[100];
		memset(&buffer_Modules, 0x00, sizeof(buffer_Modules));
		sprintf(buffer_Modules, "Module%d", i);
	 	json module_json;
	 	module_json[buffer_Modules] =
			{
						{"ModuleName",vecSystemModuleSecurityCheck[i].ModuleName},
						{"MemorySizeModule",vecSystemModuleSecurityCheck[i].MemorySizeModule},
						{"LoadedTimes", vecSystemModuleSecurityCheck[i].LoadedTimes },
							{"DependentModule",vecSystemModuleSecurityCheck[i].DependentModule},
							{ "MoslueState;",vecSystemModuleSecurityCheck[i].MoslueState  },
							{ "OffsetModule",vecSystemModuleSecurityCheck[i].OffsetModule  },
							{"FileName",vecSystemModuleSecurityCheck[i].FileName},
								{"License",vecSystemModuleSecurityCheck[i].License},
				{"Description",vecSystemModuleSecurityCheck[i].Description},
			{"Author",vecSystemModuleSecurityCheck[i].Author},
								{"srcVersion",vecSystemModuleSecurityCheck[i].srcVersion},
									{"Depends",vecSystemModuleSecurityCheck[i].Depends},
									{"Retpolines",vecSystemModuleSecurityCheck[i].Retpoline},
								{"Intree",vecSystemModuleSecurityCheck[i].Intree},
										{"Vermagic",vecSystemModuleSecurityCheck[i].Vermagic},
											{"Sig_id",vecSystemModuleSecurityCheck[i].Sig_id},
												{"Signer",vecSystemModuleSecurityCheck[i].Signer},
													{"Sig_Key",vecSystemModuleSecurityCheck[i].Sig_Key},
													{"Sig_HashAlgo",vecSystemModuleSecurityCheck[i].Sig_HashAlgo},
														{"Signature",vecSystemModuleSecurityCheck[i].Signature},
	};
		m_PreJSON_File(m_SystemModuleSecurityCheck, &module_json, SYSTEMMODULESECURITYCHECK);
		module_json.clear();
	}
	return 0;
}


int GetJSON::GetChromeInternetAccessRecord_Json()
{
	vec_CHromBrowserHistory=m_cGetSysinfo.getChromeInternetAccessRecord();
json browserHistory_json;
//std::cout<<"debug;  vec_CHromBrowserHistory.size()="<<vec_CHromBrowserHistory.size()<<std::endl;
for(int i=0;i<vec_CHromBrowserHistory.size();i++)
{
//	printf("vec_CHromBrowserHistory.size()=%d\n ",vec_CHromBrowserHistory.size());//debug
	if(vec_CHromBrowserHistory[i].vec_Title.empty())
	{
		printf("There is no User:%s  Chrome'S Browser History!\n", vec_CHromBrowserHistory[i].user_Name.c_str());
		continue;
	}
	else
	{
	 

		//printf("Preredd to write json; line=%d  vec_CHromBrowserHistory[%d].vec_DateTime.size()=%d press ENTERNET\n",__LINE__,i,vec_CHromBrowserHistory[i].vec_DateTime.size()); getchar();//debug
		//	char user_BrowserHistory[100]={0};
					char user_BrowserHistory[1000];
			for(int j=0;j< vec_CHromBrowserHistory[i].vec_DateTime.size();j++)
		{
			if(vec_CHromBrowserHistory[i].Type==0)
			{
					 memset(&user_BrowserHistory,0x00,sizeof(user_BrowserHistory));
				//  sprintf(user_BrowserHistory,"%s'sChrome:BroserHIstory[%d]",vec_CHromBrowserHistory[i].user_Name.c_str(),t_j);
				 snprintf(user_BrowserHistory,sizeof(user_BrowserHistory),"[%s]\'sChrome:BroserHIstory[%d]",vec_CHromBrowserHistory[i].user_Name.c_str(),j);
			}
			else if(vec_CHromBrowserHistory[i].Type==1)
			{
		//  sprintf(user_BrowserHistory,"%s'sFirefox:BroserHIstory[%d]",vec_CHromBrowserHistory[i].user_Name.c_str(),t_j);
		 snprintf(user_BrowserHistory,100,"[%s] \'sFirefox:BroserHIstory[%d]",vec_CHromBrowserHistory[i].user_Name.c_str(),j);
			} 
			else
			{
				//  sprintf(user_BrowserHistory,"%s's360SE:BroserHIstory[%d]",vec_CHromBrowserHistory[i].user_Name.c_str(),t_j);
					 snprintf(user_BrowserHistory,100,"[%s] \'s360SE:BroserHIstory[%d]",vec_CHromBrowserHistory[i].user_Name.c_str(),j);
			}
user_BrowserHistory[99]=0;
		//	std::cout<<"debug; user_BrowserHistory="<<user_BrowserHistory<<std::endl;//debug
		//printf("j=%d  vec_CHromBrowserHistory[%d].vec_DateTime.size()=%d \n",j,i,vec_CHromBrowserHistory[i].vec_DateTime.size());//debug
	//std::cout<<"debug; user_BrowserHistory="<<user_BrowserHistory<<std::endl;//debug
	 				browserHistory_json[user_BrowserHistory] =
					 {
					 	{"Title",vec_CHromBrowserHistory[i].vec_Title[j]},
						{"DateTime",vec_CHromBrowserHistory[i].vec_DateTime[j]},
						{"Url",vec_CHromBrowserHistory[i].vec_Url[j] },
						// {"DateTime",vec_CHromBrowserHistory[i].vec_DateTime[j]},
						// {"Url",vec_CHromBrowserHistory[i].vec_Url[j] },
					 };
// 		      std::cout<<"DateTime:"<<vec_CHromBrowserHistory[i].vec_DateTime[j]<<std::endl;
//    std::cout<<"Title:"<<vec_CHromBrowserHistory[i].vec_Title[j]<<std::endl;
//       std::cout<<"Url:"<<vec_CHromBrowserHistory[i].vec_Url[j]<<std::endl;
					 	m_PreJSON_File(m_InternetAccessRecord, &browserHistory_json, INTERNETACCESSRECORD);
						 browserHistory_json.clear();


					//	 usleep(500);
						 //	printf("debug; line=%d\n",__LINE__);//debug
				}
			}
	}





return 0;
}



int GetJSON::GetFireFoxInternetAccessRecord_Json()   // FireFox BrowserHistory to Json
{
		vec_FireFoxBrowserHistory=m_cGetSysinfo.getFirefoxInternetAccessRecord();

	json browserHistory_FireFox_Json;
	//std::cout<<"debug;  vec_FireFoxBrowserHistory.size()="<<vec_FireFoxBrowserHistory.size()<<std::endl;
	for (int i = 0; i < vec_FireFoxBrowserHistory.size(); i++)
	{
		//	printf("vec_FireFoxBrowserHistory.size()=%d\n ",vec_FireFoxBrowserHistory.size());//debug
		if (vec_FireFoxBrowserHistory[i].vec_Title.empty())
		{

			printf("There is no User:%s  FireFox's Browser History JSON FileData !\n", vec_FireFoxBrowserHistory[i].user_Name.c_str());
			continue;
		}
		else
		{

		 


			//printf("Preredd to write json; line=%d  vec_FireFoxBrowserHistory[%d].vec_DateTime.size()=%d press ENTERNET\n",__LINE__,i,vec_FireFoxBrowserHistory[i].vec_DateTime.size()); getchar();//debug
			//	char user_BrowserHistory[100]={0};
			char user_BrowserHistory[1000] = {0};
			for (int j = 0; j < vec_FireFoxBrowserHistory[i].vec_DateTime.size(); j++)
			{
				if (vec_FireFoxBrowserHistory[i].Type == 0)
				{
					memset(&user_BrowserHistory, 0x00, sizeof(user_BrowserHistory));
				sprintf(user_BrowserHistory,"%s'sChrome:BroserHIstory[%d]",vec_FireFoxBrowserHistory[i].user_Name.c_str(),j);
					//snprintf(user_BrowserHistory, sizeof(user_BrowserHistory), "%s\'sChrome:BroserHIstory", vec_FireFoxBrowserHistory[i].user_Name.c_str());
				}
				else if (vec_FireFoxBrowserHistory[i].Type == 1)
				{
					//  sprintf(user_BrowserHistory,"%s'sFirefox:BroserHIstory[%d]",vec_FireFoxBrowserHistory[i].user_Name.c_str(),t_j);
					snprintf(user_BrowserHistory, 100, "[%s] 'sFirefox:BroserHIstory[%d]", vec_FireFoxBrowserHistory[i].user_Name.c_str(),j);
				}
				else
				{
					//  sprintf(user_BrowserHistory,"%s's360SE:BroserHIstory[%d]",vec_FireFoxBrowserHistory[i].user_Name.c_str(),t_j);
					snprintf(user_BrowserHistory, 100, "%s's360SE:BroserHIstory", vec_FireFoxBrowserHistory[i].user_Name.c_str());
				}
				//user_BrowserHistory[99]= 0;
				//	std::cout<<"debug; user_BrowserHistory="<<user_BrowserHistory<<std::endl;//debug
				//printf("j=%d  vec_FireFoxBrowserHistory[%d].vec_DateTime.size()=%d \n",j,i,vec_FireFoxBrowserHistory[i].vec_DateTime.size());//debug
				//std::cout<<"debug; user_BrowserHistory="<<user_BrowserHistory<<std::endl;//debug
				browserHistory_FireFox_Json[user_BrowserHistory] =
				{
					{ "Title",vec_FireFoxBrowserHistory[i].vec_Title[j] },
				{ "DateTime",vec_FireFoxBrowserHistory[i].vec_DateTime[j] },
				{ "Url",vec_FireFoxBrowserHistory[i].vec_Url[j] },
				// {"DateTime",vec_FireFoxBrowserHistory[i].vec_DateTime[j]},
				// {"Url",vec_FireFoxBrowserHistory[i].vec_Url[j] },
				};
				// 		      std::cout<<"DateTime:"<<vec_FireFoxBrowserHistory[i].vec_DateTime[j]<<std::endl;
				//    std::cout<<"Title:"<<vec_FireFoxBrowserHistory[i].vec_Title[j]<<std::endl;
				//       std::cout<<"Url:"<<vec_FireFoxBrowserHistory[i].vec_Url[j]<<std::endl;
				m_PreJSON_File(m_FireFoxInternetAccessRecord, &browserHistory_FireFox_Json, INTERNETACCESSRECORD);
				browserHistory_FireFox_Json.clear();

				//	 usleep(500);
				//	printf("debug; line=%d\n",__LINE__);//debug
			}
		}
	}
 

	return 0;









}

int GetJSON::GetSoftwareCheckOfSystemInstallationRPM_Json()
{
	bool isExistRPM;
	 	vec_rpmPackageInfo= m_cGetSysinfo.getSoftwareCheckOfSystemInstalltaion_RPM(isExistRPM);//  获取安装软件信息RPM

	if (vec_rpmPackageInfo.empty())
	{
		printf(" Without any RPM Installed Packasges Infomation\n");
		return 0;
	}

	json InstalledPackages_Json;
	char name_Item[100] = { 0 };




	for (int i = 0; i < vec_rpmPackageInfo.size(); i++)
	{
	memset(&name_Item,0x00,100);
			sprintf(name_Item, "InstalledPackage[%d]:", i);
	//	sprintf(name_Item, "InstalledPackage:");

		/*	 Breaks;
			 PackageName;
			 Architecture;
			 Version;
			 Priority;
			 Essential;
			 Section;
			 Origin;
			 Maintainer;
			 Original_Maintaine;
			 Bugs;
			 Installed_Size;
			 Task;
			 Depends;
			 Filename;
			 Original_Maintainer;
			 Supported;
			 APT_Manual_Installed;
			 MD5sum;
			 SHA1;
			 SHA256;
			 Description;
			 APT_Sources;
			 Source;
			 Multi_Arch;
			 Pre_Depends;
			 Provides;
			 Download_Size;
			 Homepage;
	*/


	 	InstalledPackages_Json[name_Item] =     
		{

			{ "Name", vec_rpmPackageInfo[i].Name },
				{"Version", vec_rpmPackageInfo[i].Version},

		{ "Architecture",vec_rpmPackageInfo[i].Architecture },

		{"Install_Date",vec_rpmPackageInfo[i].Install_Date},
		{"Group",vec_rpmPackageInfo[i].Group},

		{"Size",vec_rpmPackageInfo[i].Size},
		{"License",vec_rpmPackageInfo[i].License},
		{"Signature",vec_rpmPackageInfo[i].Signature},
		{"Source_RPM",vec_rpmPackageInfo[i].Source_RPM},
		{"Build_Date",vec_rpmPackageInfo[i].Build_Date },
		{"Build_Host",vec_rpmPackageInfo[i].Build_Host},

		{"Relocations",vec_rpmPackageInfo[i].Relocations},

		{"Packager",vec_rpmPackageInfo[i].Packager },
		{"Vendor",vec_rpmPackageInfo[i].Vendor},
		//	{"Filename",vec_rpmPackageInfo[i].Filename},
			{"URL",vec_rpmPackageInfo[i].URL},

			{"Summary",vec_rpmPackageInfo[i].Summary },
			{"Description",vec_rpmPackageInfo[i].Description  },
 

		};
		m_PreJSON_File(m_InstalledPackages, &InstalledPackages_Json, INSTALEEDPACKAGE);
		InstalledPackages_Json.clear();


	}
	return 0;

}


int GetJSON::GetSoftwareCheckOfSystemInstallationAPT_Json()
{
	bool isExistAPT;
		vec_aptPackageInfo = m_cGetSysinfo.getSoftwareCheckOfSystemInstallation_APT(isExistAPT);//  获取安装软件信息
	if (vec_aptPackageInfo.empty())
	{
		printf(" Without any ATP Installed Packasges Infomation\n");
		return 0;
	}

	json InstalledPackages_Json;
	char name_Item[100] = { 0 };
	

	

	for (int i = 0; i < vec_aptPackageInfo.size(); i++)
	{
	memset(&name_Item,0x00,100);
	 	sprintf(name_Item, "InstalledPackage[%d]:", i);
	 
	
		/*	 Breaks;
			 PackageName;
			 Architecture;
			 Version;
			 Priority;
			 Essential;
			 Section;
			 Origin;
			 Maintainer;
			 Original_Maintaine;
			 Bugs;
			 Installed_Size;
			 Task;
			 Depends;
			 Filename;
			 Original_Maintainer;
			 Supported;
			 APT_Manual_Installed;
			 MD5sum;
			 SHA1;
			 SHA256;
			 Description;
			 APT_Sources;
			 Source;
			 Multi_Arch;
			 Pre_Depends;
			 Provides;
			 Download_Size;
			 Homepage;
	*/


		InstalledPackages_Json[name_Item] =
		{

			{ "PackageName", vec_aptPackageInfo[i].PackageName },
				{"Replaces", vec_aptPackageInfo[i].Replaces},

		{ "Architecture",vec_aptPackageInfo[i].Architecture },

		{"Version",vec_aptPackageInfo[i].Version},
		{"Priority",vec_aptPackageInfo[i].Priority},
		{"Essential",vec_aptPackageInfo[i].Essential},
		{"Section",vec_aptPackageInfo[i].Section},
		{"Origin",vec_aptPackageInfo[i].Origin},
		{"Maintainer",vec_aptPackageInfo[i].Maintainer},
		{"Original_Maintaine",vec_aptPackageInfo[i].Original_Maintaine },
		{"Bugs",vec_aptPackageInfo[i].Bugs},
		{"Installed_Size",vec_aptPackageInfo[i].Installed_Size},
		{"Task",vec_aptPackageInfo[i].Task },
		{"Depends",vec_aptPackageInfo[i].Depends},
	//	{"Filename",vec_aptPackageInfo[i].Filename},
		{"Suggests",vec_aptPackageInfo[i].Suggests},
		{"Original_Maintainer",vec_aptPackageInfo[i].Original_Maintainer },
		{"Supported",vec_aptPackageInfo[i].Supported  },
		{ "APT_Manual_Installed",vec_aptPackageInfo[i].APT_Manual_Installed },
		{"MD5sum",vec_aptPackageInfo[i].MD5sum},
		{"SHA1",vec_aptPackageInfo[i].SHA1},
		{"SHA256",vec_aptPackageInfo[i].SHA256},
		{ "Description",vec_aptPackageInfo[i].Description },
		{"APT_Sources",vec_aptPackageInfo[i].APT_Sources},
		{"Source",vec_aptPackageInfo[i].Source},
		{"Multi_Arch",vec_aptPackageInfo[i].Multi_Arch},
		{"Pre_Depends",vec_aptPackageInfo[i].Pre_Depends},

		{"Provides",vec_aptPackageInfo[i].Provides},
		{"Download_Size",vec_aptPackageInfo[i].Download_Size},

		{"Homepage",vec_aptPackageInfo[i].Homepage},

		 };
		 m_PreJSON_File(m_InstalledPackages, &InstalledPackages_Json, INSTALEEDPACKAGE);
		 InstalledPackages_Json.clear();


	}
	return 0;

}



int GetJSON::GetUSB_EventsHistory_JSON( )
{

		m_cGetSysinfo.getUSB_EventHistory(vecUSB_EventsHistory);

			if(vecUSB_EventsHistory.empty())
			{
					printf("could not find any USB events history in this OS!\n");
					return -1;
			}
 
json USBHistory_Json;
 	char USBHistory_name_Item[100] = { 0 };
for(int i=0;i<vecUSB_EventsHistory.size();i++)
{

 
	USBHistory_Json.clear();
	sprintf(USBHistory_name_Item,"USBEventHistory[%d]",i);
		USBHistory_Json[USBHistory_name_Item] = {
	 {"conn:",vecUSB_EventsHistory[i].conn},
     	 {"disconn:",vecUSB_EventsHistory[i].disconn},
		       	 {"idProduct:",vecUSB_EventsHistory[i].idProduct},
							       	 {"idVendor:",vecUSB_EventsHistory[i].idVendor},
								{"manufac:",vecUSB_EventsHistory[i].manufact},
								 {"pid:",vecUSB_EventsHistory[i].pid},
									{"port:",vecUSB_EventsHistory[i].port},
						 {"production:",vecUSB_EventsHistory[i].prod},
						 	 {"serial:",vecUSB_EventsHistory[i].serial},
		};


		m_PreJSON_File(m_USBEventHistory, &USBHistory_Json,  USBEVENTHISTORY);
		
			memset(&USBHistory_name_Item,0x00,100);

		}
		return 1;

return 0;

}
int GetJSON::GetgetBlueTooth_JSON()
{

m_cGetSysinfo.getBluetooth(vecBlueTooth_HW);
	if(vecBlueTooth_HW.empty())
			{
			printf(" Without any Bluetooth HardWare  Infomation\n");
				return 0;
			}
 
json Bluetooth_Json;
 	char Bluetooth_name_Item[100] = { 0 };
for(int i=0;i<vecBlueTooth_HW.size();i++)
{

 
	Bluetooth_Json.clear();
	sprintf(Bluetooth_name_Item,"Bluetooth[%d]",i);
		Bluetooth_Json[Bluetooth_name_Item] = {
	 {"description",vecBlueTooth_HW[i].description},
      {  "product",vecBlueTooth_HW[i].product},
      {"vendor",vecBlueTooth_HW[i].vendor},
        {"physical_id", vecBlueTooth_HW[i].physical_id},
        {"bus_info",vecBlueTooth_HW[i].bus_info},
      //  {"logical_name",vecBlueTooth_HW[i].logical_name},
        {"version",vecBlueTooth_HW[i].version},
        {"serial",vecBlueTooth_HW[i].serial},
   //  {"width",vecBlueTooth_HW[i].width},
    //   {"clock",vecBlueTooth_HW[i].clock},
       {"capabilities",vecBlueTooth_HW[i].capabilities},
       {"configuration",vecBlueTooth_HW[i].configuration},
     //  {"resources",vecBlueTooth_HW[i].resources},
	 //  {"capacity",vecBlueTooth_HW[i].capacity},
	   	   {"id",vecBlueTooth_HW[i].id},
		};


		m_PreJSON_File(m_BlueTooth, &Bluetooth_Json, BLUETOOTH);
		
			memset(&Bluetooth_name_Item,0x00,100);

		}
		return 1;
}
int GetJSON::GetgetUSB_MassStorageDevice_JSON()
{
    m_cGetSysinfo.getUSB_MassStorageDevice(vecUSB_MassStorageDevice_HW);    
			if(vecUSB_MassStorageDevice_HW.empty())
			{
			printf(" Without any USB HardWare  Infomation\n");
				return 0;
			}
 
json USB_Json;
 	char USB_MassStorageDevice_name_Item[100] = { 0 };
for(int i=0;i<vecUSB_MassStorageDevice_HW.size();i++)
{

 
	USB_Json.clear();
	sprintf(USB_MassStorageDevice_name_Item,"USB[%d]",i);
		USB_Json[USB_MassStorageDevice_name_Item] = {
	 {"",vecUSB_MassStorageDevice_HW[i].description},
      {  "",vecUSB_MassStorageDevice_HW[i].product},
      {"",vecUSB_MassStorageDevice_HW[i].vendor},
        {"physical_id", vecUSB_MassStorageDevice_HW[i].physical_id},
        {"bus_info",vecUSB_MassStorageDevice_HW[i].bus_info},
      //  {"logical_name",vecUSB_MassStorageDevice_HW[i].logical_name},
        {"version",vecUSB_MassStorageDevice_HW[i].version},
        {"serial",vecUSB_MassStorageDevice_HW[i].serial},
   //  {"width",vecUSB_MassStorageDevice_HW[i].width},
    //   {"clock",vecUSB_MassStorageDevice_HW[i].clock},
       {"capabilities",vecUSB_MassStorageDevice_HW[i].capabilities},
       {"configuration",vecUSB_MassStorageDevice_HW[i].configuration},
     //  {"resources",vecUSB_MassStorageDevice_HW[i].resources},
	 //  {"capacity",vecUSB_MassStorageDevice_HW[i].capacity},
	   	   {"id",vecUSB_MassStorageDevice_HW[i].id},
		};


		m_PreJSON_File(m_USB_MassStorageDevice, &USB_Json, USBMASSSTORAGEDEVICE);
		
			memset(&USB_MassStorageDevice_name_Item,0x00,100);

		}
}




int GetJSON::GetNetWorkHardWare_JSON()
{
       m_cGetSysinfo.getNetWorkHardWare(vecNetwork_HW);    
			if(vecNetwork_HW.empty())
			{
			printf(" Without any NetWork HardWare  Infomation\n");
				return 0;
			}

  

json NetWork_Json;
 	char networkHardWare_name_Item[100] = { 0 };
for(int i=0;i<vecNetwork_HW.size();i++)
{

 
	NetWork_Json.clear();
	sprintf(networkHardWare_name_Item,"NetWork[%d]",i);
		NetWork_Json[networkHardWare_name_Item] = {
	 {"description",vecNetwork_HW[i].description},
      {  "product",vecNetwork_HW[i].product},
      {"vendor",vecNetwork_HW[i].vendor},
        {"physical_id", vecNetwork_HW[i].physical_id},
        {"bus_info",vecNetwork_HW[i].bus_info},
        {"logical_name",vecNetwork_HW[i].logical_name},
        {"version",vecNetwork_HW[i].version},
        {"serial",vecNetwork_HW[i].serial},
     {"width",vecNetwork_HW[i].width},
       {"clock",vecNetwork_HW[i].clock},
       {"capabilities",vecNetwork_HW[i].capabilities},
       {"configuration",vecNetwork_HW[i].configuration},
       {"resources",vecNetwork_HW[i].resources},
	   {"capacity",vecNetwork_HW[i].capacity},
	   	   {"id",vecNetwork_HW[i].id},
		};


		m_PreJSON_File(m_NetWorkHardWareInfo, &NetWork_Json, NETWORKHARDWARE);
		
			memset(&networkHardWare_name_Item,0x00,100);
}



}

int GetJSON::GetMountPoint_Json()
{
		vec_MountPoint = m_cGetSysinfo.getSystemMountPoint();//获取系统挂载点信息
	if (vec_MountPoint.empty())
	{
		printf(" Without any  MountPoint  Infomation\n");
		return 0;
	}

	json MountPointInfo_Json;
 	char name_Item[100] = { 0 };
 
	for (int i = 0; i < vec_MountPoint.size(); i++)
	{

		sprintf(name_Item,"Mount%d",i);
	//	cout<<"jsonWrite["<<i<<"]="<<vec_MountPoint[1].UseagePercentage<<endl;//debug

		MountPointInfo_Json[name_Item] = {
			{"FileSystem",vec_MountPoint[i].FileSystem   },
		{"Available",vec_MountPoint[i].Available},
		{"MountOn",vec_MountPoint[i].MountOn },
		{"Size",vec_MountPoint[i].Size},
		{"UseagePercentage",vec_MountPoint[i].UseagePercentage},
		{"Used",vec_MountPoint[i].Used},

		};

		m_PreJSON_File(m_MountPointInfo, &MountPointInfo_Json, MOUNTPOINT);
		MountPointInfo_Json.clear();
			memset(&name_Item,000,100);
	}




}



 



/**************************************************************************************************
*函数名：		IsExist
*参数：			kpcFile
*返回值：		true	成功
*				false	失败
*函数功能说明：	检查文件是否存在
**************************************************************************************************/
bool GetJSON::IsExist(const char *kpcFile)
{
	bool bRet = true;
	struct stat statFileInfo = {0};
	if (0 != stat(kpcFile, &statFileInfo))
	{
		//	printf("[line]:%d\n",__LINE__);
		if (ENOENT == errno)
		{ //文件不存在
			bRet = false;
		}
	}
	//rintf("[line]:%d\n",__LINE__);
	struct timespec ts;
	// clock_gettime(CLOCK_REALTIME, &ts);
	struct tm t;
	char date_time[64];
	strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", localtime_r(&statFileInfo.st_mtim.tv_sec, &t));
	// printf("clock_gettime : date_time=%s, tv_nsec=%ld\n", date_time, statFileInfo.st_mtim.tv_nsec);
	return bRet;
}
int GetJSON::m_PreJSON_File(const char *fJSON_Name, void *stData, int nMark)
{
	//	printf("debug[line]: %d\n",__LINE__);
	switch (nMark)
	{
	case COMPUTER_SYSTEM_INFOMATION: /* constant-expression */
	{
		json *srcData = (json *)stData;
		int ret = m_WriteJSON_File(fJSON_Name, srcData);
	}
	break;
	case FILESYSTEM_CHECK:
		/* code */
		{
			json *srcDataFileSystemInfo = (json *)stData;
			//	printf("[debug] :%d [fn]: GetJSON::m_PreJSON_File \n",__LINE__);
			//	cout<<"deubg:\n"<<srcDataFileSystemInfo->dump(4)<<endl;
			int rt_FileSystemInfo = m_WriteJSON_File(fJSON_Name, srcDataFileSystemInfo);
		}
		break;
		case SYSTEMACCOUNTSECURITYCHECK:
		{
			json * ptr_SysAccountSecurityCheck= (json *)stData;
			//	printf("[debug] :%d [fn]: GetJSON::m_PreJSON_File \n",__LINE__);
			//	cout<<"deubg:\n"<<ptr_SysAccountSecurityCheck->dump(4)<<endl;
			int rt_SysAccountSecurity = m_WriteJSON_File(fJSON_Name, ptr_SysAccountSecurityCheck);
		}
		break;
		case SYSTEMMODULESECURITYCHECK:
		{
	json * ptr_SysteMoudleSecurityCheck= (json *)stData;
		int rt_SysAccountSecurity = m_WriteJSON_File(fJSON_Name, ptr_SysteMoudleSecurityCheck);
		}
		break;
		case INTERNETACCESSRECORD:
		{
	json * ptr_InternetAccessRecord= (json *)stData;
		//std::cout<<ptr_InternetAccessRecord->dump(4);
	//	std::cout<<"over cout ptr_InternetAccessRecord!"<<std::endl;
		int rt_InternetAccessRecord= m_WriteJSON_File(fJSON_Name,ptr_InternetAccessRecord);
		}
		break;
		case INSTALEEDPACKAGE:
		{
			json * ptr_InstalledPackage = (json *)stData;
			//std::cout<<ptr_InternetAccessRecord->dump(4);
			//	std::cout<<"over cout ptr_InternetAccessRecord!"<<std::endl;
			int rt_InternetAccessRecord = m_WriteJSON_File(fJSON_Name, ptr_InstalledPackage);

		}
		case MOUNTPOINT:
		{
			json * ptr_MountPoint = (json *)stData;
		 
			int rt_MountPoint= m_WriteJSON_File(fJSON_Name, ptr_MountPoint);
		}
		break;
		case NETWORKHARDWARE:
		{
	json * ptr_NetWorkHardWare = (json *)stData;
int rt_NetWorkHardWare= m_WriteJSON_File(fJSON_Name, ptr_NetWorkHardWare);

		}
		break;
		case USBMASSSTORAGEDEVICE:
		{
	json * ptr_USBHardWare = (json *)stData;
int rt_USBHardWare= m_WriteJSON_File(fJSON_Name, ptr_USBHardWare);
		}
		break;
		case BLUETOOTH:
		{
				json * ptr_BluetoothHardWare = (json *)stData;
int rt_BluetoothHardWare= m_WriteJSON_File(fJSON_Name, ptr_BluetoothHardWare);
		}
		break;
				case USBEVENTHISTORY:
		{
				json * ptr_USBEventHistory = (json *)stData;
int rt_USBEventHistory= m_WriteJSON_File(fJSON_Name, ptr_USBEventHistory);
		}
		break;
				case CONNECTIONHOSTNAME:
		{
				json * ptr_ConnectionHostname = (json *)stData;
int rt_ConnectionHostname= m_WriteJSON_File(fJSON_Name, ptr_ConnectionHostname);
		}
	default:
		break;
	}
		//printf("debug; line=%d\n",__LINE__);//debug
	return 0;
}
int GetJSON::m_WriteJSON_File(const char *ptr_Name, const json *ptr_JSONdata)
{
	int iRet = 0;
	//文件不存在
	if (!IsExist(ptr_Name))
	{
		if (NULL != m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
		//打开文件
 	m_pFile = fopen(ptr_Name, "a+b");
 
		if (NULL == m_pFile)
		{
			iRet = g_iOpenFileFailed;
		fprintf (stderr, "Failed to open %s\n"
"The reason *may* have been %s\n",
ptr_Name, strerror(errno));
		//	exit(EXIT_FAILURE);
			printf("open %s failed,[line]=%d\n", ptr_Name,__LINE__);
			return iRet;
		}
	}
	else //文件存在就直接打开
	{
		if (NULL == m_pFile)
		{
			//打开文件
			m_pFile = fopen(ptr_Name, "a+b");
			if (NULL == m_pFile)
			{
				iRet = g_iOpenFileFailed;
						fprintf (stderr, "Failed to open %s\n"
"The reason *may* have been %s\n",
ptr_Name, strerror(errno));
				printf("open %s failed\n", ptr_Name);
				return iRet;
			}
		}
	}
	//写文件
//	string	stData=SysInfo_JSON_Array.dump(4);
 
	string stData = ptr_JSONdata->dump(4);
//	printf("debug; line=%d\n",__LINE__);//debug
//	 std::cout<<stData<<std::endl;//debug
	const char *ptr_buffer = new char[stData.length() + 1];
	ptr_buffer = stData.c_str();
	// cout<<"debug; GetJSON::m_WriteJSON_File  ptr_buffer="<<ptr_buffer<<endl;
	size_t c_rt = fwrite(ptr_buffer, 1, strlen(ptr_buffer), m_pFile);
	if (c_rt != strlen(ptr_buffer))
	{
		printf("this number differs from the count parameter, c_rt=%d vs strlen(ptr_buffer)=%d\n", c_rt, strlen(ptr_buffer));
	}
	//	printf("debug; line=%d\n",__LINE__);//debug
	if (ferror(m_pFile))
		printf("Error Writing to %s.json\n", ptr_Name);
	fflush(m_pFile); //注： 在linux中fflush只是将内容写进内核缓冲区，让驱动程序定时调用fsync来同步。
					 //在windows中描述的是flush到 operating-system buffer，貌似也是系统缓冲区。只有fopen中用了c参数才会直接写磁盘。
					 //简单来讲就是，只要系统不崩溃或异常关闭，fflush就是可靠的。直接写磁盘的代价是远比写进内核缓冲区要大的。
	fclose(m_pFile); //
	m_pFile = NULL;
	//	printf("debug; line=%d\n",__LINE__);//debug
}
int GetJSON::test(void)
{
	printf(" etJSON::m_WriteJSON_File \n");
	return 0;
}
 


