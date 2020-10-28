
#ifndef FILELOGCHECKHTTP_H
#define FILELOGCHECKHTTP_H
#include <iostream>
#include <QObject>

#include "sconv/codec.hpp"

#include "json/json.h"//###

#include "utils.h"
#include <future>

#include <json/json.h>
#include <fstream>

#define USE_STANDALONE_ASIO
#define ASIO_STANDALONE
#include "client_ws.hpp"

using namespace std;
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

//std::shared_ptr<WsClient::Connection> uiConnection = nullptr;
//std::mutex g_mutex;

static std::shared_ptr<WsClient::Connection> g_fileconnection = nullptr;
static std::mutex g_filemutex;

class HttpMessageClient{
public:
    //static bool isConnet;
    static HttpMessageClient *Instance() {
        //isConnet=false;
        //Init();
        static HttpMessageClient *this_ = nullptr;

        if (this_ == nullptr) {
            this_ = new HttpMessageClient;
        }

        return this_;
    }
    void Init();
    static void onClose(shared_ptr<WsClient::Connection> connection, int status, const string &reason);
    static void SendMsgToServer(const QString &message_str);
    static void SendMessage(const std::string &message_str);
    static void onError(shared_ptr<WsClient::Connection> connection, const SimpleWeb::error_code &ec);
    static void onMessage(shared_ptr<WsClient::Connection> connection, shared_ptr<WsClient::Message> message);
    static void onOpen(shared_ptr<WsClient::Connection> connection);
    ~HttpMessageClient();
private:

    HttpMessageClient();
    std::thread *workThread;
};


#endif // FILELOGCHECKHTTP_H

