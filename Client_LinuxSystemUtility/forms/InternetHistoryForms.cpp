
#include "utils.h"

// Ui::InternetHistoryForms take;
#include"InternetHistoryForms.h"
#include"ui_InternetHistoryForms.h"
#include <QTableWidgetItem>
#include <QMenu>
#include<iostream>
InternetHistoryForms::InternetHistoryForms(QWidget *parent):
        QDialog(parent),firefox_scan_table_rows_(0),browerType(0),chrome_table_rows_(0),
         take_Ui(new Ui::InternetHistoryForms)
{
        take_Ui->setupUi(this);
  take_Ui->stackedWidget->setCurrentIndex(0);

            initForms();

            Ui::InternetHistoryForms take;
}

void InternetHistoryForms::initForms()
{

  // take_Ui->tableWidgetFirefox->setRowCount(20);//take_cancel:
  // take_Ui->tableWidgetChrome->setRowCount(20);
   connect(take_Ui->fireFoxToolButton, SIGNAL(clicked(bool)), this, SLOT(on_fireFoxToolButton_clicked()));
     connect(take_Ui->chromeToolButton, SIGNAL(clicked(bool)), this, SLOT(on_chromeToolButton_clicked()));
  // std::cout<<__LINE__<<__FUNCTION__<< std::endl;
   // take_Ui->setupUi()
    connect(take_Ui->fireFoxToolButton, SIGNAL(clicked(bool)), this, SLOT(on_fireFoxToolButton_clicked()));

         connect(take_Ui->chromeToolButton, SIGNAL(clicked(bool)), this, SLOT(on_chromeToolButton_clicked()));




         connect(take_Ui->fireFoxToolButton, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
                  connect(take_Ui->chromeToolButton, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
//         connect(ui->btnMessageHook, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
//         connect(ui->btnProcessInfo, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
//         connect(ui->btnPortInfo, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
//         connect(ui->btnSelfStart, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
//         connect(ui->btnHistoryPorcess, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
//         connect(ui->btnIATInfo, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
//         connect(ui->btnMessageHook, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));

}

    InternetHistoryForms::~InternetHistoryForms()
{
  delete take_Ui;

}

  void  InternetHistoryForms::btnSysInfo()
  {

//cout<<__LINE__<<endl;
  }

  void InternetHistoryForms::test()
  {
        std::cout<<"InternetHistoryForms::test"<<std::endl;
  }

void InternetHistoryForms::on_fireFoxToolButton_clicked()
{


  browerType=0;
}

void InternetHistoryForms::on_actInternetHistoryPushButton_clicked()
{

    Json::Value req;

    if(browerType==0)
 {
        req["msg"] = "FireFoxScan";

}else if(browerType==1)
    {
        req["msg"] = "ChromeScan";

    }
    else {
        {
                    req["msg"] = "FireFoxScan";//defuault;
        }
    }
    firefox_scan_table_rows_=0;
    chrome_table_rows_=0;
    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));

std::cout<<"browerType="<<browerType<<std::endl;




}

void InternetHistoryForms::on_chromeToolButton_clicked()
{
  browerType=1;


}

void InternetHistoryForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();
int   browerType;
    QIcon icon;
    Json::Value req;
    if (name == "fireFoxToolButton") {
        icon.addFile(":/image/process_check_orange.png", QSize(), QIcon::Normal, QIcon::Off);
       browerType=0;
       take_Ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "chromeToolButton") {
        req["msg"] = "autorun";
          browerType=1;
        icon.addFile(":/image/auto_check.png", QSize(), QIcon::Normal, QIcon::Off);
      take_Ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnMessageHook") {
        req["msg"] = "messagehook";
        icon.addFile(":/image/hook_check.png", QSize(), QIcon::Normal, QIcon::Off);
     //   ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnPortInfo") {
        icon.addFile(":/image/port_check.png", QSize(), QIcon::Normal, QIcon::Off);
      //  ui->stackedWidget->setCurrentIndex(3);
    } else if (name == "btnIATInfo") {
        req["msg"] = "iat";
        icon.addFile(":/image/iat_check.png", QSize(), QIcon::Normal, QIcon::Off);
   //     ui->stackedWidget->setCurrentIndex(4);
    } else if (name == "btnHistoryPorcess") {
        req["msg"] = "historyprocess";
        icon.addFile(":/image/history_check.png", QSize(), QIcon::Normal, QIcon::Off);
      //  ui->stackedWidget->setCurrentIndex(5);
    } else {
        return;
    }



   // Json::Value req;

//    if(browerType==0)
// {
//        req["msg"] = "FireFoxScan";

//}else if(browerType==1)
//    {
//        req["msg"] = "ChromeScan";

//    }
//    else {
//        {
//                    req["msg"] = "FireFoxScan";//defuault;
//        }
//    }
//    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));



//    if(req["msg"].isString())
//    {
//        WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
//    }
    btn->setIcon(icon);
}
void InternetHistoryForms::HandleServerMessage(const Json::Value &value,int len)
{
    std::string subMsgType = value["msg"]["type"].asString();
    if(len==0)
    {
        if(subMsgType=="FireFoxScan")
        {
          take_Ui->tableWidgetFirefox->setRowCount(5);
           insertDataToBrowserScan(value,len);

            }
        else if(subMsgType=="ChromeScan"){
  take_Ui->tableWidgetChrome->setRowCount(5);
          insertDataToBrowserScan(value,len);
        }
       }
else{
       take_Ui->tableWidgetFirefox->setRowCount(len);
                take_Ui->tableWidgetChrome->setRowCount(len);
                  insertDataToBrowserScan(value,len);
    }


}

void InternetHistoryForms::insertDataToBrowserScan(const Json::Value &data,int len)
{
    if(data.empty())
        return;




    std::string subMsgType = data["msg"]["type"].asString();
    if(subMsgType=="FireFoxScan")
    {

        if(len==0)
        {
            take_Ui->tableWidgetFirefox->setItem(firefox_scan_table_rows_, 0,  new QTableWidgetItem(QString::fromStdString("No FireFox Internet History find")));
                        firefox_scan_table_rows_++;
        return;
        }

take_Ui->tableWidgetFirefox->setItem(firefox_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["user_Name"].asString())));

take_Ui->tableWidgetFirefox->setItem(firefox_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["url"].asString())));
take_Ui->tableWidgetFirefox->setItem(firefox_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["title"].asString())));
//std::cout<<"data[\"msg\"][\"title\"].asString()="<<data["msg"]["title"].asString()<<std::endl;
take_Ui->tableWidgetFirefox->setItem(firefox_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["date"].asString())));

firefox_scan_table_rows_++;

    }
    else if (subMsgType=="ChromeScan")
    {
        if(len==0)
        {
            take_Ui->tableWidgetChrome->setItem(chrome_table_rows_, 0,  new QTableWidgetItem(QString::fromStdString("No Chrome Internet History find")));
                        chrome_table_rows_++;
        return;
        }

        take_Ui->tableWidgetChrome->setItem(chrome_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["user_Name"].asString())));

        take_Ui->tableWidgetChrome->setItem(chrome_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["url"].asString())));
        take_Ui->tableWidgetChrome->setItem(chrome_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["title"].asString())));
      //  std::cout<<"data[\"msg\"][\"title\"].asString()="<<data["msg"]["title"].asString()<<std::endl;
        take_Ui->tableWidgetChrome->setItem(chrome_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["date"].asString())));

        chrome_table_rows_++;

    }


}


