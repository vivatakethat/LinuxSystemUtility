#include "webserviceclientworker.h"

#include <json/json.h>
//#include<public.h>
#include "FileLogCheckHttp.h"

bool isUSBScan=false;

#define USE_STANDALONE_ASIO
#define ASIO_STANDALONE
#include "client_ws.hpp"
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;
#include <fstream>
using namespace std;

#include "sconv/codec.hpp"

#include "utils.h"


std::shared_ptr<WsClient::Connection> uiConnection = nullptr;
std::mutex g_mutex;

#define TRUE 1
#define FALSE 0

bool bFirstReceiveMessage=TRUE;

class WebSeriviceClient {
public:
    static WebSeriviceClient *Instance() {
        static WebSeriviceClient *this_ = nullptr;

        if (this_ == nullptr) {
            this_ = new WebSeriviceClient;
        }

        return this_;
    }

    void SendMessage(const std::string &message_str) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (uiConnection) {
            auto send_stream = make_shared<WsClient::SendStream>();
            *send_stream << message_str;

            uiConnection->send(send_stream, [](const SimpleWeb::error_code &ec) {
                if(ec) {
                    onError(uiConnection, ec);
                }
            });
        } else {
        }
    }

    void Init() {
        workThread = new std::thread([&]() {
            while (true) {
     // WsClient client("192.168.6.125:8889/SpecialTrojan");  //
             WsClient client("127.0.0.1:8889/SpecialTrojan");  //
                     // WsClient client("192.168.2.129:33446/SpecialTrojan");  //
           //  WsClient client("192.168.2.129:33446/SpecialTrojan");  //
           //    WsClient client("192.168.2.128:33446/SpecialTrojan");
             //    WsClient client("192.168.2.133:33446/SpecialTrojan");
               //    WsClient client("192.168.2.129:33446/SpecialTrojan");

                client.on_message = onMessage;

                client.on_open = onOpen;
                client.on_close = onClose;
                client.on_error = onError;
                client.start();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

    }

    static void HandleUiConnectOk(shared_ptr<WsClient::Connection> connection) {
        std::lock_guard<std::mutex> lock(g_mutex);
        uiConnection = connection;

        Json::Value req;
        req["msg"] = "USBStartScan";

        auto send_stream = make_shared<WsClient::SendStream>();
        *send_stream << req.toStyledString();
     //   connection->send(send_stream);    // 这里线程默认总是发送
        isUSBScan=true;//take_add:置false防止线程重复发消息
    }

private:
    WebSeriviceClient() : workThread(nullptr) {
    }

    ~WebSeriviceClient() {}

    static void onMessage(shared_ptr<WsClient::Connection> connection, shared_ptr<WsClient::Message> message) {
        auto msg_str = message->string();
            std::cout<<" msg_str:"<< msg_str<<std::endl;
             std::string tmp_str=msg_str;
      //  std::string tmp_str = gbk_to_utf8(msg_str);
     // std::cout<<"tmp_str :"<<tmp_str <<std::endl;
                //cout<<"tmp_str="<<tmp_str<<endl;//take_debug:

        try {
            Json::Value req;
            Json::Reader reader;
            if (reader.parse(tmp_str, req)) {
             //   cout<<"req[\"msg\"].type()="<<req["msg"].type()<<endl;//take_debug:

                if (req["msg"].isString()) {
             std::string msg_type = req["msg"].asString().c_str();
          cout<<"Frome Server:msg_type="<<msg_type<<endl;

                    if (msg_type == "UIConnectOK")
                    {
                       HandleUiConnectOk(connection);
                    }
                    else if(msg_type=="FirstSendMessage")
                    {
                        bFirstReceiveMessage=FALSE;
                    }

                    else if (msg_type == "TrojanScanInfo" || msg_type == "ScanTrojanResult" || msg_type == "ClearScanFinishInfo")
                    {
                        pushData(QString::fromStdString(tmp_str));
                    } else if (msg_type == "MonitorInfo" || msg_type == "NetInfo")
                    {
                        pushData(QString::fromStdString(tmp_str));                      
                    }
                    //###新增加
                    else if (msg_type == "ClearProcessInfo" || msg_type == "ProcessInfo"  ||
                             msg_type == "ClearPortInfo" || msg_type == "PortInfo")
                    {
                        pushData(QString::fromStdString(tmp_str));
                    }
                    else if ( msg_type == "ClearMessageHookInfo" ||msg_type == "MessageHookInfo" || msg_type == "ShowDllRespond" ||
                              msg_type == "AutoRunInfo" || msg_type == "ClearAutorunInfo" || msg_type == "ClearIATInfo" ||
                              msg_type == "IATInfo" || msg_type == "HistoryProcessInfo" || msg_type == "ClearHistoryProcessInfo")
                    {
                        if(bFirstReceiveMessage)
                        {
                           pushData(QString::fromStdString(tmp_str));
                        }
                        else
                        {
                           WebServiceClientWorker::Instance()->ToProcessAnalysisHandle(req);
                        }

                    }
                    else if (msg_type == "ClearDriverInfo" || msg_type == "DriverInfo" || msg_type == "ClearSSDTInfo" ||
                               msg_type == "SSDTInfo" || msg_type == "ClearShadowSSDTInfo" || msg_type == "ShadowSSDTInfo" ||
                               msg_type == "ClearIDTInfo" || msg_type == "IDTInfo")
                    {
                        if(bFirstReceiveMessage)
                        {
                           pushData(QString::fromStdString(tmp_str));
                        }
                        else
                        {
                            WebServiceClientWorker::Instance()->ToKernelAnalysisHandle(req);
                        }

                    }
                    else if (msg_type == "UPanTrojanInfo")
                    {
                        pushData(QString::fromStdString(tmp_str));
                    }
                    else if (msg_type == "Reboot")
                    {
                        WebServiceClientWorker::Instance()->DealServerOtherMessage("Reboot");
                    }

              }


                else
                { //take_mark:


                    std::string sub_Msg_type="";

                             sub_Msg_type = req["msg"]["type"].asString();

                 if(sub_Msg_type == "USBHistoryEvents")//take_add:USB插拔事件
                    {
                     pushData(QString::fromStdString(tmp_str));
                    }
					else if(sub_Msg_type == "KeyWorldFindEvents")//take_add:关键字查找事件
                    {
                     pushData(QString::fromStdString(tmp_str));
                    }
					else if(sub_Msg_type == "ExtensionFindEvents")//take_add:扩展名查找事件
                    {
                     pushData(QString::fromStdString(tmp_str));
                    }
                 else if(sub_Msg_type == "PathEvents")//take_add:路径事件
                    {
                     pushData(QString::fromStdString(tmp_str));
                    }
                 else if((sub_Msg_type=="FileSystemEvents")||(sub_Msg_type=="MountPointEvents" )||(sub_Msg_type=="FileSystemLshwEvents"))
                 {
                        pushData(QString::fromStdString(tmp_str));
                 }
                 else if(sub_Msg_type=="FireFoxScan"||(sub_Msg_type=="ChromeScan"))
                 {
                          pushData(QString::fromStdString(tmp_str));
                 }
                 else if( sub_Msg_type=="SystemModuleReadHatEvents" )
                 {
                        pushData(QString::fromStdString(tmp_str));
                 }
                 else if(sub_Msg_type=="SystemAccountScan" )
                 {
                     pushData(QString::fromStdString(tmp_str));

                 }
                 else if(sub_Msg_type=="CheckInternetConnectScan" )
                 {
                     pushData(QString::fromStdString(tmp_str));

                 }
                 else if(sub_Msg_type=="OperationSysteminfoScan")
                 {
                         pushData(QString::fromStdString(tmp_str));
                 }
                 else if(sub_Msg_type=="NetworkAdapterScan")
                 {
                     pushData(QString::fromStdString(tmp_str));

                 }
                 //USBHardWareScan
                 else if(sub_Msg_type=="USBHardWareScan")
                 {
                     pushData(QString::fromStdString(tmp_str));

                 }
                 // BluetoothScan
                 else if(sub_Msg_type=="BluetoothScan")
                 {
                     pushData(QString::fromStdString(tmp_str));

                 }
                 //PackageScan

                 else if(sub_Msg_type=="PackageScan")
                 {
                     pushData(QString::fromStdString(tmp_str));

                 }
                    else{

                }

            }
            }
        } catch(...) {

        }
    }

    static void onOpen(shared_ptr<WsClient::Connection> connection) {
        cout << "Client: Opened connection" << endl;

        Json::Value req;
        req["msg"] = "UIConnect";

        auto send_stream = make_shared<WsClient::SendStream>();
        *send_stream << req.toStyledString();
        connection->send(send_stream);
    }

    static void onClose(shared_ptr<WsClient::Connection> connection, int status, const string &reason) {
        if (connection == uiConnection) {
            std::lock_guard<std::mutex> lk(g_mutex);
            uiConnection = nullptr;
        }
    }

    static void onError(shared_ptr<WsClient::Connection> connection, const SimpleWeb::error_code &ec) {
        if (connection == uiConnection) {
            std::lock_guard<std::mutex> lk(g_mutex);
            uiConnection = nullptr;
        }
    }

private:
    static WebSeriviceClient *this_;
    std::thread *workThread;

};

WebServiceClientWorker *WebServiceClientWorker::this_ = nullptr;

WebServiceClientWorker *WebServiceClientWorker::Instance()
{
    if (this_ == nullptr) {
        this_ = new WebServiceClientWorker;
    }

    return this_;
}

void WebServiceClientWorker::DealServerMessageWithFullScan(const std::string &message_str)
{
    emit fullScanMessage(QString::fromStdString(message_str));
}

void WebServiceClientWorker::DealServerMessageWithHorseScan(const std::string &message_str)
{
    emit horseScanMessage(QString::fromStdString(message_str));
}

void WebServiceClientWorker::DealServerMessageWithKernelAnalysis(const std::string &message_str)
{
    emit kernelAnalysisMessage(QString::fromStdString(message_str));
}

void WebServiceClientWorker::DealServerMessageWithProcessAnaysis(const std::string &message_str)
{
    emit processAnalysisMessage(QString::fromStdString(message_str));
}

void WebServiceClientWorker::DealServerOtherMessage(const string &message_str)
{
    if (message_str == "Reboot")
        emit rebootMachine();
}

void WebServiceClientWorker::DealServerMessageWithTrojan(const string &message_str)
{
    emit trojanInfoMessage(QString::fromStdString(message_str));
}

void WebServiceClientWorker::SendMsgToServer(const QString &message_str)
{
    if (!message_str.isEmpty())
        WebSeriviceClient::Instance()->SendMessage(message_str.toStdString());
}


WebServiceClientWorker::WebServiceClientWorker()
{
   WebSeriviceClient::Instance()->Init();
    HttpMessageClient::Instance()->Init();
}

WebServiceClientWorker::~WebServiceClientWorker()
{

}

//###新增加
void WebServiceClientWorker::ToKernelAnalysisHandle(const Json::Value &data)
{
    emit KernelAnalysisHandle(data);
}
void WebServiceClientWorker::ToProcessAnalysisHandle(const Json::Value &data)
{
    emit ProcessAnalysisHandle(data);
}
