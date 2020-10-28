//#include"sysinfo.hpp"

#include"ConvertJSON.h"

//#include<json.hpp>

#include<string>



//using json = nlohmann::json;

using namespace std;

int main()

{



  //sysinfo s_i = get_sysinfo();

	// json  get_sysinfo_json=s_i.OS;

//  cout<<"[OS]"<<  s_i.os << endl;

//  cout<<"[CPU]"<< s_i.cpu << endl;

//  cout<<"[RAW]"<< s_i.ram << endl;

//  cout<<"[LinuxkernelVersion]:" << s_i.LinuxkernelVersion << endl;

// json j;

// j["OS"]=s_i.os;

// j["CPU"]=s_i.cpu;

// cout<<j.dump(4)<<endl;

// cout<<"----------------------------------------------------"<<endl;





// Array

// json Arr_json;

// Arr_json["SysInfo"]={

// {"OS",s_i.OS},

// {"CPU",s_i.CPU},

// {"RAW",s_i.RAM},

// {"LinuxkernelVersion",s_i.LinuxkernelVersion},

// {"Architecture",s_i.Architecture},

// {"Hostname",s_i.Hostname},

// };



//  cout<<Arr_json["SysInfo"].dump(4)<<endl;



GetJSON take;
// take.GetSystemAccountSecurityCheck_Json();
//take.GetSysInfo_Json();

// take.GetSystemModuleSecurityCheck_Json();
//  take.GetChromeInternetAccessRecord_Json();
// take.GetFireFoxInternetAccessRecord_Json();
  take.GetSoftwareCheckOfSystemInstallationAPT_Json();
// take.GetSoftwareCheckOfSystemInstallationRPM_Json();
// take.GetMountPoint_Json();
 
//  take.GetFilesystem_Json();
 
//  take.GetNetWorkHardWare_JSON();

 // take.GetgetUSB_MassStorageDevice_JSON();
// take.GetgetBlueTooth_JSON();

 //take.GetUSB_EventsHistory_JSON();

// take.isConnectInternet_JSON();

//-----------------------------------------------------------


// GetSysInfo takethat;
// std::vector<st_checkConnection> vec_Connection;
// takethat.checkInternetConnect(vec_Connection);
// for(int i=0;i<vec_Connection.size();i++)
// {
//   cout<<vec_Connection[i].destination<<endl;
//     cout<<vec_Connection[i].result<<endl;
// }

//  GetSysInfo takethat;
// std::vector<st_filtered> vec_Filtered;
// takethat.getUSB_EventHistory(vec_Filtered);
// cout<<"vec_Filtered.size()="<<vec_Filtered.size()<<endl;
//for(int i=0;i<vec_Filtered.size();i++)



return 0;



}

	 







