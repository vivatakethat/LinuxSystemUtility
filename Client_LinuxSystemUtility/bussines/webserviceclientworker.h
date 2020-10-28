#ifndef WEBSERVICECLIENTWORKER_H
#define WEBSERVICECLIENTWORKER_H

#include <iostream>
#include <QObject>

#include "json/json.h"//###

class WebServiceClientWorker : public QObject
{
    Q_OBJECT

public:
    static WebServiceClientWorker* Instance();

    void DealServerMessageWithFullScan(const std::string &message_str);
    void DealServerMessageWithHorseScan(const std::string &message_str);
    void DealServerMessageWithKernelAnalysis(const std::string &message_str);
    void DealServerMessageWithProcessAnaysis(const std::string &message_str);
    void DealServerOtherMessage(const std::string &message_str);
    void DealServerMessageWithTrojan(const std::string &message_str);

    void SendMsgToServer(const QString &message_str);

    //###新增加
    void ToKernelAnalysisHandle(const Json::Value &data);
    void ToProcessAnalysisHandle(const Json::Value &data);

signals:
    void fullScanMessage(const QString &message_str);
    void horseScanMessage(const QString &message_str);
    void kernelAnalysisMessage(const QString &message_str);
    void processAnalysisMessage(const QString &message_str);
    void trojanInfoMessage(const QString &message_str);
    void rebootMachine();

    //###新增加
    void KernelAnalysisHandle(const Json::Value &data);
    void ProcessAnalysisHandle(const Json::Value &data);



private:
    WebServiceClientWorker();
    ~WebServiceClientWorker();

private:
    static WebServiceClientWorker *this_;
};


#endif // WEBSERVICECLIENTWORKER_H
