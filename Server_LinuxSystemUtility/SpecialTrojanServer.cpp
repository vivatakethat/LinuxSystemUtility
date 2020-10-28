//#pragma execution_character_set("utf-8")
#include <iostream>
#include <vector>
#include "SpecialTrojanServer.h"

#include "jsoncpp/json/json.h"

#include "SpecialTrojan.h"
std::shared_ptr<WsServer::Connection> uiConnection;
std::mutex uiMutex;


using namespace std;


extern BOOL gbCanTrojanScan;



void onError(std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
	std::cout << "Server: Error in connection " << connection.get() << ". "
		<< "Error: " << ec << ", error message: " << ec.message() << std::endl;

	if (connection == uiConnection) {
		std::lock_guard<std::mutex> lk(uiMutex);
		uiConnection = nullptr;
	}
}

void onOpen(std::shared_ptr<WsServer::Connection> connection) {
	std::cout << "Server: Opened connection " << connection.get() << std::endl;
}

void onClose(std::shared_ptr<WsServer::Connection> connection, int status, const std::string & reason) {
	std::cout << "Server: Closed connection " << connection.get() << " with status code " << status << std::endl;
	if (connection == uiConnection) {
		std::lock_guard<std::mutex> lk(uiMutex);
		uiConnection = nullptr;
	}
}

void onMessage(std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::Message> msg) {

	auto msgStr = msg->string();
    cout<<"[server],msgStr="<<msgStr<<"line:45 function:onMessag "<<endl;

	try {
		do {




			Json::Reader reader;
			Json::Value value;
			reader.parse(msgStr, value);
            std::cout<<"server,debug :"<<__LINE__<<__FUNCTION__<<"value[\"msg\"]="<<value["msg"]<<std::endl;
			if (!value.isObject() || !value["msg"].isString())
			{
				break;
			}
			if (value["msg"] == "UIConnect")
			{
				std::lock_guard<std::mutex> lk(uiMutex);
				uiConnection = connection;
				value["msg"] = "UIConnectOK";

				auto send_stream = make_shared<WsServer::SendStream>();
				*send_stream << value.toStyledString();
				connection->send(send_stream);
				break;
			}
            if (value["msg"] == "USBStartScan")
			{
                USBScan();
            //	StartScan();
				break;
			}
            if (value["msg"] == "FileSystemScan")
            {

  std::cout<<__LINE__<<__FUNCTION__<<std::endl;
                FileSystemScan();
                break;
            }

//FileSystemLshwEvents
            if (value["msg"] == "FileSystemLshwEvents")
            {


                FileSystemLshwScan();
                break;
            }


            if (value["msg"] == "CheckInternetConnectScan")
            {


                CheckInternetConnectScan();
                break;
            }
            if (value["msg"] == "SystemAccountScan")
            {

  std::cout<<__LINE__<<__FUNCTION__<<std::endl;
                SystemAccountScan();
                break;
            }
            if(value["msg"]=="MountPointScan")
            {

                        MountPointScan();
              break;
            }
            //
            if(value["msg"]=="OperationSysteminfoScan")
            {

                       OperationSysteminfoScan();
              break;
            }
            if(value["msg"]=="SystemModuletEvents")
            {

                        SystemModuleScan();
              break;
            }
            if(value["msg"]=="FireFoxScan")
            {

                        FirefoxInternetHistoryScan();

              break;
            }
            // ChromeInternetHistory();
            if(value["msg"]=="ChromeScan")
            {

                      ChromeInternetHistory();

              break;
            }

            //AccountSecurityCheck
            if(value["msg"]=="AccountSecurityCheckScan")
            {

                     AccountSecurityScan();
              break;
            }
        //  NetworkAdapterFormsScan
            if(value["msg"]=="NetworkAdapterFormsScan")
            {

                     NetworkAdapterFormsScan();
              break;
            }
            //USBHardWareScan
            if(value["msg"]=="USBHardWareScan")
            {

                     USBHardWareScan();
              break;
            }
            //BluetoothScan
            if(value["msg"]=="BluetoothScan")
            {

                     BluetoothScanScan();
              break;
            }
            if(value["msg"]=="ChromeScan")
            {


              break;
            }
                // PackageScan()
            if(value["msg"]=="PackageScan")
            {

            PackageScan();
              break;
            }
            if(value["msg"]=="UpdataKeyword")
            {
                std::string result=value["keyword"].toStyledString();
                 updata_keyword_post(result);
              break;
            }
            if(value["msg"]=="Scanfile")
            {
                     std::string result=value["fullpath"].asString();
                     //std::cout<<__LINE__<<__FUNCTION__<<"result="<<result<<endl;

              std::string get_str=  flt_file_by_get(result);
              SendstrOCR(get_str);
              break;
            }





			if (value["msg"] == "ShowDll")
			{
				break;
			}	
			if (value["msg"] == "outputReportForm")
			{
				OutputReportForm();
				break;
			}
			if (value["msg"] == "exit")
			{
				Exit();
				break;
			}
			if (value["msg"] == "reboot")
			{
				break;
			}

			//###前台传消息过来，后台才发数据到前台去
			if (gbCanTrojanScan)//###等待信息处理完成
			{
				if (value["msg"] == "iat")
				{

                    ViewIatInfo(NULL);
					break;
				}
				if (value["msg"] == "driver")
				{

                    ViewDriverInfo(NULL);
					break;
				}
				if (value["msg"] == "autorun")
				{

                    ViewAntorunInfo(NULL);
					break;
				}
				if (value["msg"] == "messagehook")
				{
	
                    ViewMessageInfo(NULL);
					break;
				}
				if (value["msg"] == "ssdt")
				{
				
                    ViewSsdtInfo(NULL);
					break;
				}
				if (value["msg"] == "shadowssdt")
				{
				
                    ViewShadowSsdtInfo(NULL);
					break;
				}
				if (value["msg"] == "idt")
				{
	
                    ViewIdtInfo(NULL);
					break;
				}
				if (value["msg"] == "historyprocess")
				{
			
                    ViewHistoryProcessInfo(NULL);//###增加历史进程
					break;
				}
				
			}


			
		} while (0);

	}
	catch (...) {

	}
}

void SendMessageToClient(const std::string& msg) {
    cout<<msg<<endl;//take:debug
	std::lock_guard<std::mutex> lk(uiMutex);
	if (uiConnection)
	{
		auto send_stream = make_shared<WsServer::SendStream>();
		*send_stream << msg;

		uiConnection->send(send_stream, [](const SimpleWeb::error_code &ec)
		{
			if (ec)
			{
				onError(uiConnection, ec);
			}
		});
	}
}


void Init(WsServer::Endpoint& ep) {
	ep.on_message = onMessage;
	ep.on_error = onError;
	ep.on_close = onClose;
	ep.on_open = onOpen;
}


std::shared_ptr< GetSysInfoImpl> toGetDynamicPtr(const char *pathDynamic)
{
    typedef std::shared_ptr<void>(*LPDLLCREATEFUNC)(void);
    LPDLLCREATEFUNC lpFunc;
    int iRet = 0;
    std::shared_ptr<void> lpObject;


    //void* hLib = dlopen("./libGate_Gunnebo_QCD_NEW.so", RTLD_LAZY);
    //void* hLib = dlopen("./libGate_Gunnebo.so", RTLD_LAZY);



    void* hLib = dlopen(pathDynamic, RTLD_LAZY);
    if (NULL == hLib) {
        printf("dlopen error is[%s]\n", dlerror());
    //	iRet = UNKNOW_ERROR;
    }
    else {
        int iRet = 0;
        char * perror;
        lpFunc = (LPDLLCREATEFUNC)dlsym(hLib, "CreateInstance");  //take:����ʵ��
        perror = dlerror();
        if (NULL == perror) {
            iRet = 0;
            lpObject = lpFunc();
        }
        else {
        //	iRet = UNKNOW_ERROR;
            printf("dlsym error :%s\n", perror);
            return  NULL;
        }
            std::shared_ptr<void> take = lpObject;



            std::shared_ptr< GetSysInfoImpl> m_OGate = std::static_pointer_cast< GetSysInfoImpl>(lpObject);

m_OGate->test();



            if(m_OGate!=NULL)
            {
                cout<<"load dynamic is Sussecc!!"<<endl;
                return m_OGate;
                 //   cout<<__LINE__<<endl;
       // vector<st_sysAccount> result_Account=m_OGate->getSysAccountSecurityCheck();
                    //    cout<<__LINE__<<endl;
//		for(int i=0;i< result_Account.size();i++)
//		{
//			cout<<"Username:"<< result_Account[i].UserName<<endl;
//		}
        cout<<__LINE__<<endl;
            }
            else
            {
                cout<<"m_OGate is NULL!!!!!"<<endl;
            }
            //  free(m_OGate);
            m_OGate=NULL;


    }
  //  return iRet;




}
