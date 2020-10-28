#include "FileLogCheckHttp.h"
#include "filelogforms.h"
HttpMessageClient::HttpMessageClient(){

}
void HttpMessageClient::Init()
{
    workThread = new std::thread([&]() {
        while (true) {
            WsClient client("127.0.0.1:8301/recognition");
            client.on_message = onMessage;
            client.on_open = onOpen;
            client.on_close = onClose;
            client.on_error = onError;
            client.start();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
   });
}
HttpMessageClient::~HttpMessageClient()
{
    g_fileconnection->send_close(0);
}
void HttpMessageClient::onClose(shared_ptr<WsClient::Connection> connection, int status, const string &reason) {
    if (connection == g_fileconnection) {
        std::lock_guard<std::mutex> lk(g_filemutex);
        g_fileconnection = nullptr;
    }
}

void HttpMessageClient::SendMsgToServer(const QString &message_str)
{
    if (!message_str.isEmpty())
        SendMessage(message_str.toStdString());
}

void HttpMessageClient::SendMessage(const std::string &message_str) {

    std::lock_guard<std::mutex> lock(g_filemutex);
    if (g_fileconnection) {
        //cout<<"11111111"<<endl;
        auto send_stream = make_shared<WsClient::SendStream>();
        *send_stream << message_str;

        g_fileconnection->send(send_stream, [](const SimpleWeb::error_code &ec) {
            if(ec) {
                onError(g_fileconnection, ec);
            }
        });
    } else {
    }
}

void HttpMessageClient::onError(shared_ptr<WsClient::Connection> connection, const SimpleWeb::error_code &ec) {
    if (connection == g_fileconnection) {
        std::lock_guard<std::mutex> lk(g_filemutex);
        g_fileconnection = nullptr;
    }
}

void HttpMessageClient::onMessage(shared_ptr<WsClient::Connection> connection, shared_ptr<WsClient::Message> message) {
    auto msg_str = message->string();
    //msg_str
    //std::string tmp_str = gbk_to_utf8(msg_str);
    Json::Reader Reader;
    Json::Value DevJson;
    Reader.parse(msg_str,DevJson);

    pushData(QString::fromStdString(msg_str));



}

void HttpMessageClient::onOpen(shared_ptr<WsClient::Connection> connection) {
    cout << "Client: Opened connection" << connection.get();
    std::lock_guard<std::mutex> lock(g_filemutex);
    g_fileconnection = connection;
    //isUSBScan=true;//take_add:置false防止线程重复发消息

    //Json::Value req;
    //req["msg"] = "UIConnect";

    //auto send_stream = make_shared<WsClient::SendStream>();
    //*send_stream << req.toStyledString();
    //connection->send(send_stream);
}


