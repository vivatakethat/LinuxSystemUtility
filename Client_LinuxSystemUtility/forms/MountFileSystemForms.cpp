#include "MountFileSystemForms.h"
 #include "ui_MountFileSystemForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

MountFileSystemForms::MountFileSystemForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    horse_scan_table_rows_(0),
    net_behavior_table_rows_(0),
    MountPoint_table_rows_(0),
    ui(new Ui::MountFileSystemForms)
{
    ui->setupUi(this);
    initForms();
}

MountFileSystemForms::~MountFileSystemForms()
{
    delete ui;
}

void MountFileSystemForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    if (name == "btnHorseScan") {
        icon.addFile(":/image/horse_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnRealTimeMonitor") {
        icon.addFile(":/image/monitor_check_orange.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnNetBehavior") {
        icon.addFile(":/image/netinfo_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(2);
    }

    btn->setIcon(icon);
}

void MountFileSystemForms::btnToggle()
{
    QToolButton *btn = (QToolButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
        QIcon icon;
        if (name == "btnHorseScan") {
            icon.addFile(":/image/horse_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnRealTimeMonitor") {
            icon.addFile(":/image/monitor_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnNetBehavior") {
            icon.addFile(":/image/netinfo_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else {
            return;
        }

        btn->setIcon(icon);
    }
}

void MountFileSystemForms::HandleServerMessage(const Json::Value &value,int len)
{

ui->tableWidgetMountPoint->setRowCount(len);
    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "UPanMountFileSystemForms") {
int len=value["msg"]["size"].asInt();
            insertMountFileSystemScan(value);
        }
    }
    else
    {
        std::string subMsgType = value["msg"]["type"].asString();


            insertMountFileSystemScan(value);


    }
}


void MountFileSystemForms::initForms()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableWidgetMountPoint->setColumnWidth(0, 180);
    ui->tableWidgetMountPoint->setColumnWidth(2, 180);
    ui->tableWidgetMountPoint->setColumnWidth(5, 180);

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
        view->setSortingEnabled(true);
    }

    ui->tableWidgetMountPoint->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetMountPoint, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    installEventFilter(this);

//    connect(WebServiceClientWorker::Instance(), SIGNAL(MountFileSystemFormsMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void MountFileSystemForms::insertMountFileSystemScan(const Json::Value &data)
{
    if(data.empty())
        return;





ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["FileSystem"].asString())));
ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["Size"].asString())));
ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["Used"].asString())));
ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["Available"].asString())));


        ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["UseagePercentage"].asString())));
        ui->tableWidgetMountPoint->setItem(MountPoint_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["MountOn"].asString())));
MountPoint_table_rows_++;



//    if (!data["PID"].isString() || !data["ProcessPath"].isString() && !data["DoubtableLevel"].isString() &&
//            !data["FilePath"].isString() && !data["OP"].isString() && !data["Time"].isString())
//        return;

//    ui->tableWidgetMountPoint->setSortingEnabled(false);

//    if (ui->tableWidgetMountPoint->rowCount() == horse_scan_table_rows_) {
//        ui->tableWidgetMountPoint->setRowCount(horse_scan_table_rows_ + 2);
//    }

//    ui->tableWidgetMountPoint->setItem(horse_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
//    ui->tableWidgetMountPoint->setItem(horse_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["DoubtableLevel"].asString())));
//    ui->tableWidgetMountPoint->setItem(horse_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["FilePath"].asString())));
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 2)->setToolTip(QString::fromStdString(data["FilePath"].asString()));
//    ui->tableWidgetMountPoint->setItem(horse_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["OP"].asString())));
//    ui->tableWidgetMountPoint->setItem(horse_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["Time"].asString())));
//    ui->tableWidgetMountPoint->setItem(horse_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["ProcessPath"].asString())));
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 5)->setToolTip(QString::fromStdString(data["ProcessPath"].asString()));


//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    ui->tableWidgetMountPoint->item(horse_scan_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    if (auto_roll)
//        ui->tableWidgetMountPoint->scrollToBottom();

//    if (data["Color"].isString()) {
//        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetMountPoint, horse_scan_table_rows_);
//    }

//    horse_scan_table_rows_++;
//    ui->tableWidgetMountPoint->setSortingEnabled(true);
}

void MountFileSystemForms::on_actMountFileSystemPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "MountPointScan";


    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
}
