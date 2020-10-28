
#ifndef SPECIALTROJANSERVER
#define SPECIALTROJANSERVER
#include "stdafx.h"
#include "server_ws.hpp"
#include"GetSysInfoData.h"
#include"../thirdpart/Simple-WebSocket-Server/client_http.hpp"
#include"base64.hpp"
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;
using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

void onError(std::shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec);

void onOpen(std::shared_ptr<WsServer::Connection> connection);
void onClose(std::shared_ptr<WsServer::Connection> connection, int status, const std::string & reason);
void onMessage(std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::Message> msg);

void SendMessageToClient(const std::string& msg);

void Init(WsServer::Endpoint& ep);
std::shared_ptr< GetSysInfoImpl> toGetDynamicPtr(const char *pathDynamic);

#endif
