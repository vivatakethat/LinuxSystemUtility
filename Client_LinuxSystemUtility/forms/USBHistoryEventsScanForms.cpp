#include "USBHistoryEventsScanForms.h"
#include "ui_USBHistoryEventsScanForms.h"

//#include "windows.h"
#include <sys/socket.h>
#include <unistd.h>
#include"public.h"

#include <QTableWidgetItem>
#include <QTextCodec>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>

#include "json/json.h"

#include "webserviceclientworker.h"
#include "utils.h"

using namespace std;


USBHistoryEventsScanForms::USBHistoryEventsScanForms(QWidget *parent) :
    QDialog(parent),
    res_detector_rows(0),
    horse_scan_table_rows_(0),
    ui(new Ui::USBHistoryEventsScanForms)
{
    ui->setupUi(this);
    initForms();
}

USBHistoryEventsScanForms::~USBHistoryEventsScanForms()
{
    delete ui;
}

void USBHistoryEventsScanForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();
  cout<<__LINE__<<__FUNCTION__<<endl;
    QIcon icon;
    if (name == "btnUSBScan") {

        icon.addFile(":/image/scan_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnOtherUSB") {
        icon.addFile(":/image/result_detecte_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        return;
    }

    btn->setIcon(icon);
}

void USBHistoryEventsScanForms::btnToggle()
{
    QToolButton *btn = (QToolButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
        QIcon icon;
        if (name == "btnUSBScan") {
            icon.addFile(":/image/scan_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnOtherUSB") {
            icon.addFile(":/image/result_detect_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else {
            return;
        }

        btn->setIcon(icon);
    }
}

void USBHistoryEventsScanForms::HandleServerMessage(const Json::Value &value,int len)
{
cout<<"type="<<value["msg"]["type"]<<endl;

    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "TrojanScanInfo" && value["ScanInfoDescribe"].isString()) {
            if (value.isMember("Color") && value["Color"].isString()) {
                insertDataToHorseScan(QString::fromStdString(value["ScanInfoDescribe"].asString()),
                        QString::fromStdString(value["Color"].asString()));
            } else {
                insertDataToHorseScan(QString::fromStdString(value["ScanInfoDescribe"].asString()));
            }
        }else if(msg_type=="USBHistoryEvents")//take_add:添加USB
        {

cout<<"msg_type"<<msg_type<<endl;


     //   std::string subConnMsgType = value["msg_subtype"].asString();
            insertDataToHorseScan(value);



////take_mark:插入时间
//if(subConnMsgType=="conn")
//{
//if (value.isMember("Color") && value["Color"].isString()) {
//    insertDataToHorseScan(QString::fromStdString(value["conn"].asString()),
//            QString::fromStdString(value["Color"].asString()));
//} else {
//    insertDataToHorseScan(QString::fromStdString(value["conn"].asString()));
//    }
//}

////take_mark:拔出时间
//else if(subConnMsgType=="disconn")
//{
//if (value.isMember("Color") && value["Color"].isString()) {
//    insertDataToHorseScan(QString::fromStdString(value["disconn"].asString()),
//            QString::fromStdString(value["Color"].asString()),1);
//} else {
//    insertDataToHorseScan(QString::fromStdString(value["disconn"].asString()),"0",1);
//    }
//}


//if (value.isMember("Color") && value["Color"].isString()) {
//    insertDataToHorseScan(QString::fromStdString(value["type"].asString()),
//            QString::fromStdString(value["Color"].asString()));
//} else {
//    insertDataToHorseScan(QString::fromStdString(value["type"].asString()));
//}


//            if (value.isMember("Color") && value["Color"].isString()) {
//                insertDataToHorseScan(QString::fromStdString(value["ScanInfoDescribe"].asString()),
//                        QString::fromStdString(value["Color"].asString()));
//            } else {
//                insertDataToHorseScan(QString::fromStdString(value["ScanInfoDescribe"].asString()));
//            }
        }//------<take_mark: eof `添加USB`

        else if (msg_type == "ScanTrojanResult") {
            insertDataToResDetector(value);
        } else if (msg_type == "ClearScanFinishInfo") {
            ui->tableWidgetResDetector->clearContents();
            res_detector_rows = 0;
        }
    }
    else {//take_add:


            std::string subMsgType = value["msg"]["type"].asString();




            if(subMsgType=="USBHistoryEvents")//take_add:添加USB
            {

                    ui->tableWidgetUSBEventsScan->setRowCount(len);

         //   std::string subConnMsgType = value["msg_subtype"].asString();
                insertDataToHorseScan(value);
}
    }
}

void USBHistoryEventsScanForms::btnReportFormClick()
{
    Json::Value req;
    req["msg"] = "outputReportForm";

    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));

}

void USBHistoryEventsScanForms::initForms()
{
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_2->setFrameShape(QFrame::NoFrame);

    installEventFilter(this);
    ui->btnReportForm->installEventFilter(this);

    QList<QTableWidget *> views = ui->widgetBottomDynamic->findChildren<QTableWidget *>();

    foreach (QTableWidget *view, views) {
        view->verticalHeader()->setHidden(true);
        view->horizontalHeader()->setHighlightSections(false);
        view->horizontalHeader()->setStretchLastSection(true);
        view->setAlternatingRowColors(false);
        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->setSelectionMode(QAbstractItemView::SingleSelection);
        view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        view->verticalHeader()->setDefaultSectionSize(38);
//        view->setSortingEnabled(true);
    }

    ui->tableWidgetResDetector->setSortingEnabled(true);
  //  ui->btnOtherUSB->hide();
    connect(ui->btnUSBScan, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnOtherUSB, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnUSBScan, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnOtherUSB, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));

       // connect(ui->regetdata, SIGNAL(clicked(bool)), this, SLOT(on_regetdata_clicked()));


    connect(ui->btnReportForm, SIGNAL(clicked(bool)), this, SLOT(btnReportFormClick()));

//    connect(WebServiceClientWorker::Instance(), SIGNAL(fullScanMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);

   ui->tableWidgetUSBEventsScan->setRowCount(20);//take_mark:控制行数
   // QTableWidgetItem*buff = new QTableWidgetItem("程序正在初始化...");
  //  buff->setFont(QFont(QString::fromLocal8Bit("文泉驿微米黑")));
    // ui->tableWidgetUSBEventsScan->setItem(0, 0, new QTableWidgetItem(QString::fromStdString("程序正在初始化...").toUtf8()));
 //   ui->tableWidgetUSBEventsScan->setItem(0, 0,buff);
    ui->stackedWidget->setCurrentIndex(0);
}

void USBHistoryEventsScanForms::insertDataToHorseScan(const Json::Value &data)
{
        if(data.empty())
            return;
cout<<"data[\"msg\"][\"conn\"].asString())="<<data["msg"]["conn"].asString()<<endl;
if(data["msg"]["conn"].asString()=="")
 ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString("NULL")));

 ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["conn"].asString())));
 ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["disconn"].asString())));

 ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["port"].asString())));
 ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["idProduct"].asString())));


         ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["msg"]["serial"].asString())));
                ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["msg"]["manufact"].asString())));
         ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["idVendor"].asString())));

         ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 7, new QTableWidgetItem(QString::fromStdString(data["msg"]["prod"].asString())));
 horse_scan_table_rows_++;
}

void USBHistoryEventsScanForms::insertDataToHorseScan(const QString &data, const QString &content_color,int ops)
{


    if (data.isEmpty())
        return;//take_cancel;

    if (ui->tableWidgetUSBEventsScan->rowCount() == horse_scan_table_rows_) {
        ui->tableWidgetUSBEventsScan->setRowCount(horse_scan_table_rows_ + 2);
    }

 //ui->tableWidgetUSBEventsScan->setItem(2, 2, new QTableWidgetItem(data));//take:debug
//  QString t_Data="take";

     switch(ops)
    {
     case 0:
   ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, 0, new QTableWidgetItem(data));
break;
     case 1:
         if(data.isEmpty())
           ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, ops, new QTableWidgetItem("NULL"));
 ui->tableWidgetUSBEventsScan->setItem(horse_scan_table_rows_, ops, new QTableWidgetItem(data));
         break;
     default:
         break;

}

    if (content_color == "1") {
        ui->tableWidgetUSBEventsScan->item(horse_scan_table_rows_, 0)->setForeground(Qt::blue);
    } else if (content_color == "2") {
        QBrush brush;
        brush.setColor(QColor(238, 118, 0, 255));
        ui->tableWidgetUSBEventsScan->item(horse_scan_table_rows_, 0)->setForeground(brush);
    } else if (content_color == "3") {
        ui->tableWidgetUSBEventsScan->item(horse_scan_table_rows_, 0)->setForeground(Qt::red);
    }

    if (auto_roll)
        ui->tableWidgetUSBEventsScan->scrollToBottom();

    horse_scan_table_rows_++;
}

void USBHistoryEventsScanForms::insertDataToResDetector(const Json::Value &data)
{
    if (!data["Num"].isString() || !data["TrojanInfo"].isString())
        return;

    if (ui->tableWidgetResDetector->rowCount() == res_detector_rows) {
        ui->tableWidgetResDetector->setRowCount(res_detector_rows + 2);
    }

    ui->tableWidgetResDetector->setItem(res_detector_rows, 0, new QTableWidgetItem(QString::fromStdString(data["Num"].asString())));
    ui->tableWidgetResDetector->setItem(res_detector_rows, 1, new QTableWidgetItem(QString::fromStdString(data["TrojanInfo"].asString())));

    if (auto_roll)
        ui->tableWidgetResDetector->scrollToBottom();

    if (data["Color"].isString()) {
         setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetResDetector, res_detector_rows);
     }

    res_detector_rows++;
}

bool USBHistoryEventsScanForms::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->btnReportForm) {
        if (event->type() == QEvent::Enter) {
            ui->btnReportForm->setIcon(QIcon(":/image/reportform_check.png"));
        } else if (event->type() == QEvent::Leave) {
            ui->btnReportForm->setIcon(QIcon(":/image/reportform_normal.png"));
        }
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        int key = ke->key();
        if (key == Qt::Key_Escape)
            return true;
    }

    return QObject::eventFilter(obj, event);
}


void USBHistoryEventsScanForms::on_regetdata_clicked(bool checked)
{


  //  m_ModuleStatus->isUSBScaner=1;
//isSUBScan=true;

//cout<<"isUSB="<<isSUBScan<<endl;
}

void USBHistoryEventsScanForms::on_regetdata_clicked()
{


    Json::Value req;
        req["msg"] ="USBStartScan";
    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
cout<<__LINE__<<__FUNCTION__<<endl;
}
