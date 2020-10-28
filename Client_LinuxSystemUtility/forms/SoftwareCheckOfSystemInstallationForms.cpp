#include "SoftwareCheckOfSystemInstallationForms.h"
#include "ui_SoftwareCheckOfSystemInstallationForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"
#include<iostream>
#include "client_ws.hpp"

using namespace std;
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;
 std::mutex g_SoftwareCheckOfSystemInstallation;
SoftwareCheckOfSystemInstallationForms::SoftwareCheckOfSystemInstallationForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    package_scan_table_rows_(0),
    MountPoint_table_rows_(0),
    ui(new Ui::SoftwareCheckOfSystemInstallationForms)
{

    ui->setupUi(this);
       ui->stackedWidget->setCurrentIndex(0);
    //ui->btnAPT_Package->hide();//take:
    initForms();
}

SoftwareCheckOfSystemInstallationForms::~SoftwareCheckOfSystemInstallationForms()
{
    delete ui;
}

void SoftwareCheckOfSystemInstallationForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    if (name == "PackageInfopushButton") {
        icon.addFile(":/image/horse_check.png", QSize(), QIcon::Normal, QIcon::Off);
     //  ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnPackageInfo") {//rpm
        std::cout<<__LINE__<<std::endl;
       actionType=0;
        icon.addFile(":/image/monitor_check_orange.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnAPT_Package") {
        actionType=1;
        icon.addFile(":/image/netinfo_check.png", QSize(), QIcon::Normal, QIcon::Off);
      ui->stackedWidget->setCurrentIndex(1);
    }

    btn->setIcon(icon);
}

void SoftwareCheckOfSystemInstallationForms::btnToggle()
{
    QToolButton *btn = (QToolButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
       std::cout<<name.toStdString().data()<<std::endl;
        QIcon icon;
        if (name == "btnHorseScan") {
            icon.addFile(":/image/horse_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnPackageInfo") {//RPM
                         ui->stackedWidget->setCurrentIndex(0);
            icon.addFile(":/image/monitor_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnAPT_Package") {
            icon.addFile(":/image/netinfo_normal.png", QSize(), QIcon::Normal, QIcon::Off);
                         ui->stackedWidget->setCurrentIndex(1);
        }
        else if (name == "PackageInfopushButton") {
                    icon.addFile(":/image/netinfo_normal.png", QSize(), QIcon::Normal, QIcon::Off);
//                     //  ui->stackedWidget->setCurrentIndex(0);
                }

        else {
            return;
        }

        btn->setIcon(icon);
    }
}

void SoftwareCheckOfSystemInstallationForms::HandleServerMessage(const Json::Value &value,int len)
{
    string PackageToolType=value["msg"]["PackageToolType"].asString();

    if(PackageToolType=="rpm")
{
        ui->btnAPT_Package->hide();
       //  ui->tableAPTPackageInfo->hide();
          ui->tableWidgetRPMPackagesInfo->setRowCount(len);
          ui->stackedWidget->setCurrentIndex(0);
            insertDataToPackageScan(value);


}
else if(PackageToolType=="apt"){
ui->btnPackageInfo->hide();
   // ui->tableWidgetRPMPackagesInfo->hide();
    ui->tableAPTPackageInfo->setRowCount(len);
    ui->stackedWidget->setCurrentIndex(1);
                  insertDataToPackageScan(value);
   }

}

void SoftwareCheckOfSystemInstallationForms::createMenu(const QPoint &pos)
{
    ui->tableWidgetRPMPackagesInfo->show();//take
    QTableWidgetItem *current_item = ui->tableWidgetRPMPackagesInfo->itemAt(pos);
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



void SoftwareCheckOfSystemInstallationForms::initForms()
{

    ui->tableWidgetRPMPackagesInfo->setRowCount(30);
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

    ui->tableWidgetRPMPackagesInfo->setEnabled(true);

    ui->tableWidgetRPMPackagesInfo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetRPMPackagesInfo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    connect(ui->btnHorseScan, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnPackageInfo, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnAPT_Package, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnHorseScan, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnPackageInfo, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnAPT_Package, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));

 connect(WebServiceClientWorker::Instance(), SIGNAL(horseScanMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void SoftwareCheckOfSystemInstallationForms::insertDataToRealTimeMonitor(const QString &data, const QString content_color)
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

void SoftwareCheckOfSystemInstallationForms::insertDataToPackageScan(const Json::Value &data)
{
    if(data.empty())
        return;


    std::string subMsgType = data["msg"]["PackageToolType"].asString();
    if(subMsgType=="rpm")
    {



ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["Name"].asString())));

ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["Version"].asString())));
ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["Architecture"].asString())));

ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["Install_Date"].asString())));
 ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["msg"]["Group"].asString())));


     ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["msg"]["Size"].asString())));
     ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["License"].asString())));
     //"


     ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 7, new QTableWidgetItem(QString::fromStdString(data["msg"]["Signature"].asString())));
      ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 8, new QTableWidgetItem(QString::fromStdString(data["msg"]["Source_RPM"].asString())));


          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 9, new QTableWidgetItem(QString::fromStdString(data["msg"]["Build_Date"].asString())));
          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 10, new QTableWidgetItem(QString::fromStdString(data["msg"]["Build_Host"].asString())));




          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 11, new QTableWidgetItem(QString::fromStdString(data["msg"]["Relocations"].asString())));
          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 12, new QTableWidgetItem(QString::fromStdString(data["msg"]["Packager"].asString())));


          //
          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 13, new QTableWidgetItem(QString::fromStdString(data["msg"]["Vendor"].asString())));
          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 14, new QTableWidgetItem(QString::fromStdString(data["msg"]["URL"].asString())));




          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 15, new QTableWidgetItem(QString::fromStdString(data["msg"]["Summary"].asString())));
          ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_,16, new QTableWidgetItem(QString::fromStdString(data["msg"]["Description"].asString())));
    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 17, new QTableWidgetItem(QString::fromStdString(data["msg"]["Release"].asString())));


package_scan_table_rows_++;
    }
    else   {//apt
        ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["PackageName"].asString())));

         ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["msg"]["Architecture"].asString())));
         ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["msg"]["Replaces"].asString())));

         ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["msg"]["Breaks"].asString())));
          ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["msg"]["Version"].asString())));


              ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["msg"]["Priority"].asString())));
              ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["Essential"].asString())));
              //"


              ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 7, new QTableWidgetItem(QString::fromStdString(data["msg"]["Section"].asString())));
               ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 8, new QTableWidgetItem(QString::fromStdString(data["msg"]["Origin"].asString())));


                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 9, new QTableWidgetItem(QString::fromStdString(data["msg"]["Maintainer"].asString())));
                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 10, new QTableWidgetItem(QString::fromStdString(data["msg"]["Original_Maintaine"].asString())));




                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 11, new QTableWidgetItem(QString::fromStdString(data["msg"]["Bugs"].asString())));
                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 12, new QTableWidgetItem(QString::fromStdString(data["msg"]["Installed_Size"].asString())));


                   //
                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 13, new QTableWidgetItem(QString::fromStdString(data["msg"]["Task"].asString())));
                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 14, new QTableWidgetItem(QString::fromStdString(data["msg"]["Depends"].asString())));




                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 15, new QTableWidgetItem(QString::fromStdString(data["msg"]["Original_Maintainer"].asString())));
                   ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 16, new QTableWidgetItem(QString::fromStdString(data["msg"]["Supported"].asString())));
             ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 17, new QTableWidgetItem(QString::fromStdString(data["msg"]["APT_Manual_Installed"].asString())));



             ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 18, new QTableWidgetItem(QString::fromStdString(data["msg"]["MD5sum"].asString())));
             ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 19, new QTableWidgetItem(QString::fromStdString(data["msg"]["SHA1"].asString())));
       ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 20, new QTableWidgetItem(QString::fromStdString(data["msg"]["SHA256"].asString())));

             ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 21, new QTableWidgetItem(QString::fromStdString(data["msg"]["Description"].asString())));
             ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 22, new QTableWidgetItem(QString::fromStdString(data["msg"]["APT_Sources"].asString())));
       ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 23, new QTableWidgetItem(QString::fromStdString(data["msg"]["Source"].asString())));

       ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 24, new QTableWidgetItem(QString::fromStdString(data["msg"]["Multi_Arch"].asString())));
       ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 25, new QTableWidgetItem(QString::fromStdString(data["msg"]["Pre_Depends"].asString())));
 ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 26, new QTableWidgetItem(QString::fromStdString(data["msg"]["Provides"].asString())));



 ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 27, new QTableWidgetItem(QString::fromStdString(data["msg"]["APT_Sources"].asString())));
 ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 28, new QTableWidgetItem(QString::fromStdString(data["msg"]["Download_Size"].asString())));
 ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 29, new QTableWidgetItem(QString::fromStdString(data["msg"]["Homepage"].asString())));


 ui->tableAPTPackageInfo->setItem(package_scan_table_rows_, 30, new QTableWidgetItem(QString::fromStdString(data["msg"]["Suggests"].asString())));

package_scan_table_rows_++;


    }
//     ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["prod"].asString())));











//    ui->tableWidgetRPMPackagesInfo->setSortingEnabled(false);

//    if (ui->tableWidgetRPMPackagesInfo->rowCount() == package_scan_table_rows_) {
//        ui->tableWidgetRPMPackagesInfo->setRowCount(package_scan_table_rows_ + 2);
//    }

//    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
//    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["ProcessPath"].asString())));
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 1)->setToolTip(QString::fromStdString(data["ProcessPath"].asString()));
//    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["DoubtableLevel"].asString())));
//    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["FilePath"].asString())));
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 3)->setToolTip(QString::fromStdString(data["FilePath"].asString()));
//    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["OP"].asString())));
//    ui->tableWidgetRPMPackagesInfo->setItem(package_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["Time"].asString())));


//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetRPMPackagesInfo->item(package_scan_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    if (auto_roll)
//        ui->tableWidgetRPMPackagesInfo->scrollToBottom();

//    if (data["Color"].isString()) {
//        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetRPMPackagesInfo, package_scan_table_rows_);
//    }

//    package_scan_table_rows_++;
//    ui->tableWidgetRPMPackagesInfo->setSortingEnabled(true);
}

void SoftwareCheckOfSystemInstallationForms::insertDataToNetBehavior(const QString &data)
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

void SoftwareCheckOfSystemInstallationForms::on_actionPushButton_clicked(bool checked)
{
  //  std::lock_guard<std::mutex> lock(g_mutex);


    Json::Value req;

        req["msg"] = "FileSystemScan";




    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));

//std::cout<<"actionType="<<actionType<<std::endl;

}

void SoftwareCheckOfSystemInstallationForms::on_actionPushButton_clicked()
{

}

void SoftwareCheckOfSystemInstallationForms::on_actionPackageInfoPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "PackageScan";
    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));


}
