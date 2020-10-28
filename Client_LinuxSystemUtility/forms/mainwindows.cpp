#include "mainwindows.h"
#include "ui_mainwindows.h"

#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QMenu>

#include "iconfont.h"
#include "webserviceclientworker.h"
#include "tipmessagebox.h"
#include "utils.h"
#include<iostream>
using namespace std;
MainWindows::MainWindows(QWidget *parent) :
    QDialog(parent),
    max_(false),
    ui(new Ui::MainWindows)
{
    ui->setupUi(this);
   ui->btnProcessCheck->hide();
    initStyle();
    initForms();
}


MainWindows::~MainWindows()
{
    delete ui;
}

void MainWindows::navigationBarBtnClick()
{
    QPushButton *btn = (QPushButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    if (name == "btnUSBHistoryEventsScan") {
        icon.addFile(":/image/broom_check.svg", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnFileSystem") {
        icon.addFile(":/image/monitor_check.svg", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnInternetHistory") {
        icon.addFile(":/image/process_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnModuleSecurityCheck") {
        icon.addFile(":/image/disk_check.svg", QSize(), QIcon::Normal, QIcon::Off);
      //  ui->stackedWidget->setCurrentIndex(3);
    } else if (name == "btnTrojan") {
        icon.addFile(":/image/trojan_check.svg", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(3);
    }else if(name=="btnMountFileSystem")
    {
         icon.addFile(":/image/trojan_check.svg", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(4);
    }
    else if(name =="AccountSecurity_pushButton_2")
    {

  ui->stackedWidget->setCurrentIndex(5);

    }
    else if(name =="ConnectInternet_pushButton")
    {

  ui->stackedWidget->setCurrentIndex(6);

    }else if (name == "btnFileLog") {
        icon.addFile(":/image/process_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(7);
		}
    else if(name=="btnOperationSysteminfo")
    {
        ui->stackedWidget->setCurrentIndex(8);
    }
    else if(name=="pushButton_NetworkAdapter")
    {
ui->stackedWidget->setCurrentIndex(9);
    }
    else if(name=="USBHardWarepushButton")
    {
            ui->stackedWidget->setCurrentIndex(10);
    }
    else if(name=="BluetoothpushButton")
    {
            ui->stackedWidget->setCurrentIndex(11);
    }
    //PackageInfopushButton
    else if(name=="PackageInfopushButton")
    {
            ui->stackedWidget->setCurrentIndex(12);
    }
    //btnProcessCheck
    else if(name=="btnProcessCheck")
    {
            ui->stackedWidget->setCurrentIndex(13);
    }
    else {
        return;
    }

    btn->setIcon(icon);
}

void MainWindows::navigationBarBtnToggle()
{
    QPushButton *btn = (QPushButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
        QIcon icon;
        if (name == "btnFullScan") {
            icon.addFile(":/image/broom.svg", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnFileSystem") {
            icon.addFile(":/image/monitor.svg", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnInternetHistory") {
            icon.addFile(":/image/process.svg", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnModuleSecurityCheck") {
            icon.addFile(":/image/disk.svg", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnTrojan") {
            icon.addFile(":/image/trojan_normal.svg", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnFileLog") {
            icon.addFile(":/image/process.svg", QSize(), QIcon::Normal, QIcon::Off);
        } else {
            return;
        }
        btn->setIcon(icon);
    }
}

void MainWindows::btnCloseClick()
{
    Json::Value req;
    req["msg"] = "exit";
  //  WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));//take_debug:暂时注释,方便调试


//    canCelAutoStart();
    this->close();
}

void MainWindows::btnMaxClick()
{
    if (max_) {
        this->setGeometry(location_);
        IconFont::Instance()->SetIcon(ui->btnMax, QChar(0xf2d0), 10);
    } else {
        location_ = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconFont::Instance()->SetIcon(ui->btnMax, QChar(0xf2d2), 10);
    }

    max_ = !max_;
}

void MainWindows::btnMinClick()
{
    this->showMinimized();
}

void MainWindows::btnMenuClick()
{
    QMenu *menu = new QMenu(this);
    QAction *about_action = new QAction("关于我们", this);
    about_action->setIcon(QIcon(":/image/about.png"));

    QAction *roll_action = new QAction("自动滚动", this);

    if (auto_roll == true)
        roll_action->setIcon(QIcon(":/image/hook.png"));
    else
        roll_action->setIcon(QIcon(":/image/hook_orange.png"));

    menu->addAction(roll_action);
    menu->addAction(about_action);
    connect(about_action, SIGNAL(triggered(bool)), this, SLOT(showAboutForms()));
    connect(roll_action, SIGNAL(triggered(bool)), this, SLOT(autoRollActionTriggle()));
    menu->exec(QCursor::pos());
}

void MainWindows::showAboutForms()
{
    about_windows_->show();
}

void MainWindows::rebootMachine()
{
    TipMessageBox box;
    if (box.exec() == 0) {        
//        setAutoStart();

        Json::Value req;
        req["msg"] = "reboot";
        WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
    }
}

void MainWindows::autoRollActionTriggle()
{
    auto_roll = !auto_roll;
}

void MainWindows::refreshData()
{
    QString tmp_str = popData();
    if(tmp_str.isEmpty())
        return ;

    Json::Value req;
    Json::Reader reader;

    if (reader.parse(tmp_str.toStdString(), req)) {
        if (req["msg"]["type"].isString()) {
            std::string msg_type = req["msg"]["type"].asString().c_str();
                if(msg_type=="USBHistoryEvents")
                {


       int lenUSBEventScan = req["msg"]["size"].asInt();

  full_scan_windows_->HandleServerMessage(req,lenUSBEventScan);
                }
                else if(msg_type=="FileSystemEvents")
                {
                               filesystem_scan_windows->HandleServerMessage(req);
                 //   cout<<__LINE__<<__FUNCTION__<<endl;
                }
                else if(msg_type=="FileSystemLshwEvents")
                {
                               filesystem_scan_windows->HandleServerMessage(req);

                }

                else if((msg_type=="MountPointEvents"))
                {
                    int lenMountPoint = req["msg"]["size"].asInt();
                              mountfilesystem_windows_->HandleServerMessage(req,lenMountPoint);
                 //   cout<<__LINE__<<__FUNCTION__<<endl;
                }
                else if((msg_type=="FireFoxScan")||(msg_type=="ChromeScan"))
                {
                           int lenBrowerHistory= req["msg"]["size"].asInt();
            InternetHistory_windows->HandleServerMessage(req,lenBrowerHistory);

                }
                else if(msg_type=="SystemModuleReadHatEvents")
                {
                    std::cout<<req["msg"]["size"].asString()<<std::endl;

       int lenSystemModule = req["msg"]["size"].asInt();
                    SystemModule_analysis_windows_->HandleServerMessage(req,lenSystemModule);
                  }
                else if(msg_type=="CheckInternetConnectScan")
                {
                    std::cout<<req["msg"]["size"].asString()<<std::endl;

       int lenSystemModule = req["msg"]["size"].asInt();
                   CheckInternetConnect_windows->HandleServerMessage(req,lenSystemModule);
                  }
                else if(msg_type=="SystemAccountScan")
                {
                    std::cout<<req["msg"]["size"].asString()<<std::endl;

       int lenSystemModule = req["msg"]["size"].asInt();
                      SystemAccount_windows->HandleServerMessage(req,lenSystemModule);
                  }
                else if(msg_type=="OperationSysteminfoScan")
                {


       int lenSystemModule = req["msg"]["size"].asInt();
                   OperationSysteminfoForms_windows->HandleServerMessage(req,lenSystemModule);
                  }
                else if(msg_type=="NetworkAdapterScan")
                {


       int lenSystemModule = req["msg"]["size"].asInt();
                   NetworkAdapterForms_windows_->HandleServerMessage(req,lenSystemModule);
                  }
                else if(msg_type=="PackageScan")
                {


       int lenSystemModule = req["msg"]["SIZE"].asInt();


                  SoftwareCheckOfSystemInstallationForms_windows_->HandleServerMessage(req,lenSystemModule);

                  }

                //USBHardWareScan
                else if(msg_type=="USBHardWareScan")
                {


       int lenSystemModule = req["msg"]["size"].asInt();
                   USBHardWareForms_windows_->HandleServerMessage(req,lenSystemModule);
                  }
                //BluetoothScan
                else if(msg_type=="BluetoothScan")
                {


       int lenSystemModule = req["msg"]["size"].asInt();
                   BluetoothForms_windows_->HandleServerMessage(req,lenSystemModule);
                  }
             else if (msg_type == "TrojanScanInfo" || msg_type == "ScanTrojanResult" || msg_type == "ClearScanFinishInfo") {
                full_scan_windows_->HandleServerMessage(req,0);
            }  else if (msg_type == "MonitorInfo" || msg_type == "NetInfo") {
                filesystem_scan_windows->HandleServerMessage(req);
            } else if (msg_type == "ClearProcessInfo" || msg_type == "ProcessInfo" || msg_type == "ClearMessageHookInfo" ||
                       msg_type == "MessageHookInfo" || msg_type == "ClearPortInfo" || msg_type == "PortInfo" ||
                       msg_type == "ShowDllRespond" || msg_type == "AutoRunInfo" || msg_type == "ClearAutorunInfo" ||
                       msg_type == "ClearIATInfo" || msg_type == "IATInfo" || msg_type == "HistoryProcessInfo" ||
                       msg_type == "ClearHistoryProcessInfo") {
                process_analysis_windows_->HandleServerMessage(req);
            } else if (msg_type == "ClearDriverInfo" || msg_type == "DriverInfo" || msg_type == "ClearSSDTInfo" ||
                       msg_type == "SSDTInfo" || msg_type == "ClearShadowSSDTInfo" || msg_type == "ShadowSSDTInfo" ||
                       msg_type == "ClearIDTInfo" || msg_type == "IDTInfo") {
                SystemModule_analysis_windows_->HandleServerMessage(req,10);
            } else if (msg_type == "UPanTrojanInfo") {
                mountfilesystem_windows_->HandleServerMessage(req,0);
            }

        }
        else if(req["file_num"].asString() !="0")//用来处理文件检查系统的消息接收
        {
            //std::cout<<res["file_num"]<<endl;
            file_log_forms_windows->HandLeServerMessage(req["res"]);
        }
        if(req["status"] == 0){
            FileLogForms buff;
            buff.btnRenew();
        }
    }
}

void MainWindows::initStyle()
{

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->setWindowIcon(QIcon(":/image/blue.svg"));
  //  this->setWindowTitle("金密恶意程序辅助检测工具");  //Linux终端设备检测工具
      this->setWindowTitle("Linux终端设备检测工具");
    this->move((QApplication::desktop()->width() - this->width()) / 2,  (QApplication::desktop()->height() - this->height()) / 2);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QColor(255, 255, 255)));
    ui->widgetShadow->setPalette(palette);
    ui->widgetShadow->setAutoFillBackground(true);
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect();
    shadow_effect->setBlurRadius(10.0);
    shadow_effect->setColor(QColor(0, 0, 0, 150));
    shadow_effect->setOffset(0.5);
    ui->widgetShadow->setGraphicsEffect(shadow_effect);
}

void MainWindows::initForms()
{

    qRegisterMetaType<Json::Value>("Json::Value");
    qRegisterMetaType<Json::Value>("Json::Value&");//###

    WebServiceClientWorker *server_client_worker = WebServiceClientWorker::Instance();
    if (server_client_worker) {
        server_client_worker->moveToThread(&net_worker_thread_);
        connect(server_client_worker, SIGNAL(rebootMachine()), this, SLOT(rebootMachine()));
    }

    QList<QPushButton *>btns = ui->navigationBar->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(navigationBarBtnClick()));//这里绑定了窗体导航栏的机制
        connect(btn, SIGNAL(toggled(bool)), this, SLOT(navigationBarBtnToggle()));
    }

    connect(ui->btnClose, SIGNAL(clicked(bool)), this, SLOT(btnCloseClick()));
    connect(ui->btnMin, SIGNAL(clicked(bool)), this, SLOT(btnMinClick()));
    connect(ui->btnMax, SIGNAL(clicked(bool)), this, SLOT(btnMaxClick()));
    connect(ui->btnMenu, SIGNAL(clicked(bool)), this, SLOT(btnMenuClick()));

    IconFont::Instance()->SetIcon(ui->btnClose, QChar(0xf00d), 12);
    IconFont::Instance()->SetIcon(ui->btnMax, QChar(0xf2d0), 9);
    IconFont::Instance()->SetIcon(ui->btnMin, QChar(0xf2d1), 9);
    IconFont::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 12);

    full_scan_windows_ = new USBHistoryEventsScanForms();
    ui->stackedWidget->addWidget(full_scan_windows_);



    filesystem_scan_windows = new FileSystemeScanForms;
    ui->stackedWidget->addWidget(filesystem_scan_windows);

    InternetHistory_windows=new InternetHistoryForms;//take:add
       ui->stackedWidget->addWidget(InternetHistory_windows);


    SystemModule_analysis_windows_ = new SystemModuleSecurityCheck();
    ui->stackedWidget->addWidget(SystemModule_analysis_windows_);

    mountfilesystem_windows_ = new MountFileSystemForms();
    ui->stackedWidget->addWidget(mountfilesystem_windows_);
    SystemAccount_windows= new SysemAccountForms();
    ui->stackedWidget->addWidget(SystemAccount_windows);

   CheckInternetConnect_windows=new CheckInternetConnectforms();
    ui->stackedWidget->addWidget( CheckInternetConnect_windows);
    file_log_forms_windows=new FileLogForms();
    ui->stackedWidget->addWidget(file_log_forms_windows);

     OperationSysteminfoForms_windows=new  OperationSysteminfoForms();
ui->stackedWidget->addWidget(OperationSysteminfoForms_windows);

NetworkAdapterForms_windows_=new NetworkAdapterForms();
ui->stackedWidget->addWidget(NetworkAdapterForms_windows_);
USBHardWareForms_windows_=new USBHardWareForms();
ui->stackedWidget->addWidget(USBHardWareForms_windows_);



BluetoothForms_windows_=new BluetoothForms();
ui->stackedWidget->addWidget(BluetoothForms_windows_);
SoftwareCheckOfSystemInstallationForms_windows_= new SoftwareCheckOfSystemInstallationForms();
ui->stackedWidget->addWidget(SoftwareCheckOfSystemInstallationForms_windows_);

process_analysis_windows_ = new ProcessAnalysisForms();
ui->stackedWidget->addWidget(process_analysis_windows_);


    about_windows_ = new AboutForms;

    ui->stackedWidget->setCurrentIndex(0);





    bnt_uncheck_image_list_.append(":/image/broom.svg");
    bnt_uncheck_image_list_.append(":/image/horse.svg");
    bnt_uncheck_image_list_.append(":/image/process.svg");
    bnt_uncheck_image_list_.append(":/image/disk.svg");
    bnt_uncheck_image_list_.append(":/image/trojan_check.svg");
	bnt_uncheck_image_list_.append(":/image/process.svg");

    installEventFilter(this);
    ui->titleBar->installEventFilter(this);
    ui->labelZoom->installEventFilter(this);
    ui->labelZoom->setCursor(Qt::SizeFDiagCursor);



    //###新增加
    connect(server_client_worker, SIGNAL(ProcessAnalysisHandle(const Json::Value&)), process_analysis_windows_, SLOT(ProcessAnalysisHandle(const Json::Value&)));
    connect(server_client_worker, SIGNAL(KernelAnalysisHandle(const Json::Value&)), SystemModule_analysis_windows_, SLOT(KernelAnalysisHandle(const Json::Value&)));


    refresh_data_timer_ = new QTimer();
    refresh_data_timer_->setInterval(30);
    connect(refresh_data_timer_, SIGNAL(timeout()), this, SLOT(refreshData()));
    refresh_data_timer_->start();
}

bool MainWindows::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonDblClick) {
        this->btnMaxClick();
        return true;
    } else if (e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        int key = ke->key();
        if (key == Qt::Key_Escape)
            return true;
    }

    if (obj == ui->titleBar) {
        if(e->type() == QEvent::MouseButtonDblClick) {
            btnMaxClick();
            return true;
        }
    } else if (obj == ui->labelZoom) {
        if(e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *event = (QMouseEvent *)e;
            if (event->buttons() & Qt::LeftButton) {
                if(max_) {
                    return true;
                }
                zoom_ = true;
                zoom_localtion_ = geometry();
                zoom_pos_ = event->globalPos();
                return true;
            }
        } else if(e->type() == QEvent::MouseMove) {
            QMouseEvent *event = (QMouseEvent *)e;
            if (zoom_ && (event->buttons() & Qt::LeftButton)) {
                int dx = event->globalPos().x() - zoom_pos_.x();
                int dy = event->globalPos().y() - zoom_pos_.y();
                QRect rc = zoom_localtion_;
                rc.setRight(rc.right() + dx);
                rc.setBottom(rc.bottom() + dy);
                setGeometry(rc);
                update();
                return true;
            }
        } else if(e->type() == QEvent::MouseButtonRelease) {
            zoom_ = false;
            return true;
        }
    }

    return QObject::eventFilter(obj, e);
}


