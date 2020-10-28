#include "FileSystemeScanForms.h"
#include "ui_FileSystemeScanForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"
#include<iostream>
#include "client_ws.hpp"

using namespace std;
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;
 std::mutex g_FileSystemMutex;
FileSystemeScanForms::FileSystemeScanForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    filsystem_scan_table_rows_(0),
    MountPoint_table_rows_(0),
    ui(new Ui::FileSystemeScanForms)
{

    ui->setupUi(this);
    ui->btnMountPoint->hide();//take:
    initForms();
}

FileSystemeScanForms::~FileSystemeScanForms()
{
    delete ui;
}

void FileSystemeScanForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    if (name == "btnHorseScan") {
        icon.addFile(":/image/horse_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnFileSystem") {
       actionType=0;
        icon.addFile(":/image/monitor_check_orange.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnMountPoint") {
        actionType=1;
        icon.addFile(":/image/netinfo_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(2);
    }

    btn->setIcon(icon);
}

void FileSystemeScanForms::btnToggle()
{
    QToolButton *btn = (QToolButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
        QIcon icon;
        if (name == "btnHorseScan") {
            icon.addFile(":/image/horse_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnFileSystem") {
            icon.addFile(":/image/monitor_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnMountPoint") {
            icon.addFile(":/image/netinfo_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else {
            return;
        }

        btn->setIcon(icon);
    }
}

void FileSystemeScanForms::HandleServerMessage(const Json::Value &value)
{
    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "MonitorInfo" && value["RealTimeMonitorInfo"].isString()) {
            if (value.isMember("Color") && value["Color"].isString())
                insertDataToRealTimeMonitor(QString::fromStdString(value["RealTimeMonitorInfo"].asString()),
                        QString::fromStdString(value["Color"].asString()));
            else
                insertDataToRealTimeMonitor(QString::fromStdString(value["RealTimeMonitorInfo"].asString()));
        } else if (msg_type == "UPanTrojanInfo") {
            insertDataToFileSystemScan(value);
        } else if (msg_type == "NetInfo" && value["RealTimeNetInfo"].isString()) {
            insertDataToNetBehavior(QString::fromStdString(value["RealTimeNetInfo"].asString()));
        }
    }
    else
    {
        std::string subMsgType = value["msg"]["type"].asString();
        cout<<"sbuMsgType="<<subMsgType<<endl;
        if((subMsgType=="FileSystemEvents")||(subMsgType=="MountPointEvents")||(subMsgType=="FileSystemLshwEvents"))//take_add:
        {
            insertDataToFileSystemScan(value);
         }

    }
}

void FileSystemeScanForms::createMenu(const QPoint &pos)
{
    ui->tableWidgetHorseScan->show();//take
    QTableWidgetItem *current_item = ui->tableWidgetHorseScan->itemAt(pos);
    if (current_item == NULL)
        return;

    QMenu *pop_menu = new QMenu(this);
    QAction *process_path = new QAction("打开进程文件所在位置", this);
    process_path->setObjectName("process");
    process_path->setIcon(QIcon(":/image/dir_path.png"));

    QAction *being_opera_file_path = new QAction("打开被操作文件所在位置", this);
    being_opera_file_path->setObjectName("file");
    being_opera_file_path->setIcon(QIcon(":/image/dir_path.png"));

    pop_menu->addAction(process_path);
    pop_menu->addAction(being_opera_file_path);
    connect(process_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
    connect(being_opera_file_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
    pop_menu->exec(QCursor::pos());
}

void FileSystemeScanForms::openFileDir()
{
    int current_row = ui->tableWidgetHorseScan->currentRow();

    if (current_row < 0 || current_row >= filsystem_scan_table_rows_ || current_row >= ui->tableWidgetHorseScan->rowCount())
        return;

    QString file_path;
    QAction *action = (QAction *)sender();
    QString name = action->objectName();
    if (name == "process") {
        file_path = ui->tableWidgetHorseScan->item(current_row, 1)->text();
    } else if (name == "file") {
        file_path = ui->tableWidgetHorseScan->item(current_row, 3)->text();
    } else {
        return;
    }

    openFileDirFromFullPath(file_path);
}

void FileSystemeScanForms::initForms()
{

    ui->tableWidgetFileSystemInfo->setRowCount(30);
    ui->tableWidgetMountPoint->setRowCount(30);
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_2->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_3->setFrameShape(QFrame::NoFrame);

    ui->btnHorseScan->hide();

    installEventFilter(this);

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

    ui->tableWidgetHorseScan->setEnabled(true);

    ui->tableWidgetHorseScan->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetHorseScan, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    connect(ui->btnHorseScan, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnFileSystem, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnMountPoint, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnHorseScan, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnFileSystem, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnMountPoint, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));

 connect(WebServiceClientWorker::Instance(), SIGNAL(horseScanMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void FileSystemeScanForms::insertDataToRealTimeMonitor(const QString &data, const QString content_color)
{
//    if (data.isEmpty())
//        return;

//    if (ui->tableWidgetRealTimeMonitor->rowCount() == real_time_monitor_table_rows_) {
//        ui->tableWidgetRealTimeMonitor->setRowCount(real_time_monitor_table_rows_ + 2);
//    }

//    ui->tableWidgetRealTimeMonitor->setItem(real_time_monitor_table_rows_, 0, new QTableWidgetItem(data));

//    if (auto_roll)
//        ui->tableWidgetRealTimeMonitor->scrollToBottom();

//    if (content_color == "1") {
//        ui->tableWidgetRealTimeMonitor->item(real_time_monitor_table_rows_, 0)->setForeground(Qt::blue);
//    } else if (content_color == "2") {
//        QBrush brush;
//        brush.setColor(QColor(238, 118, 0, 255));
//        ui->tableWidgetRealTimeMonitor->item(real_time_monitor_table_rows_, 0)->setForeground(brush);
//    } else if (content_color == "3") {
//        ui->tableWidgetRealTimeMonitor->item(real_time_monitor_table_rows_, 0)->setForeground(Qt::red);
//    }

//    real_time_monitor_table_rows_++;
}

void FileSystemeScanForms::insertDataToFileSystemScan(const Json::Value &data)
{
    if(data.empty())
        return;


    std::string subMsgType = data["msg"]["type"].asString();
    if(subMsgType=="FileSystemEvents")
    {



ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["DevPath"].asString())));

ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["SerialNumber"].asString())));
ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["DriverInterFaceType"].asString())));

ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["Capacity"].asString())));
 ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["msg"]["ManufacturerModel"].asString())));


     ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["msg"]["Product_id"].asString())));
     ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["FirmwareRevision"].asString())));
filsystem_scan_table_rows_++;
    }
    else if(subMsgType=="FileSystemLshwEvents")
    {

        ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["id"].asString())));

        ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["description"].asString())));
        ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["product"].asString())));

        ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["logicalname"].asString())));
         ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["msg"]["serial"].asString())));


             ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["msg"]["size"].asString())));
             ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["configuration"].asString())));
        filsystem_scan_table_rows_++;


    }
    else if(subMsgType=="MountPointEvents")  {
cout<<"MountPoint FUnction"<<endl;

ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["FileSystem"].asString())));
ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["Size"].asString())));
ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["Used"].asString())));
ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["Available"].asString())));


        ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["UseagePercentage"].asString())));
        ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["MountOn"].asString())));
MountPoint_table_rows_++;

    }
//     ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["prod"].asString())));











//    ui->tableWidgetHorseScan->setSortingEnabled(false);

//    if (ui->tableWidgetHorseScan->rowCount() == filsystem_scan_table_rows_) {
//        ui->tableWidgetHorseScan->setRowCount(filsystem_scan_table_rows_ + 2);
//    }

//    ui->tableWidgetHorseScan->setItem(filsystem_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
//    ui->tableWidgetHorseScan->setItem(filsystem_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["ProcessPath"].asString())));
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 1)->setToolTip(QString::fromStdString(data["ProcessPath"].asString()));
//    ui->tableWidgetHorseScan->setItem(filsystem_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["DoubtableLevel"].asString())));
//    ui->tableWidgetHorseScan->setItem(filsystem_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["FilePath"].asString())));
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 3)->setToolTip(QString::fromStdString(data["FilePath"].asString()));
//    ui->tableWidgetHorseScan->setItem(filsystem_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["OP"].asString())));
//    ui->tableWidgetHorseScan->setItem(filsystem_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["Time"].asString())));


//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetHorseScan->item(filsystem_scan_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    if (auto_roll)
//        ui->tableWidgetHorseScan->scrollToBottom();

//    if (data["Color"].isString()) {
//        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetHorseScan, filsystem_scan_table_rows_);
//    }

//    filsystem_scan_table_rows_++;
//    ui->tableWidgetHorseScan->setSortingEnabled(true);
}

void FileSystemeScanForms::insertDataToNetBehavior(const QString &data)
{
//    if (data.isEmpty())
//        return;

//    if (ui->tableWidgetNetBehavior->rowCount() == net_behavior_table_rows_) {
//        ui->tableWidgetNetBehavior->setRowCount(net_behavior_table_rows_ + 2);
//    }

//    ui->tableWidgetNetBehavior->setItem(net_behavior_table_rows_, 0, new QTableWidgetItem(data));

//    if (auto_roll)
//        ui->tableWidgetNetBehavior->scrollToBottom();

//    net_behavior_table_rows_++;
}

void FileSystemeScanForms::on_actionPushButton_clicked(bool checked)
{
  //  std::lock_guard<std::mutex> lock(g_mutex);


    Json::Value req;

    //    req["msg"] = "FileSystemScan";

//

       req["msg"] = "FileSystemLshwEvents";
    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));

//std::cout<<"actionType="<<actionType<<std::endl;

}


