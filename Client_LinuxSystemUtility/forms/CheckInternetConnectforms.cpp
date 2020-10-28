

#include "CheckInternetConnectforms.h"
 #include "ui_CheckInternetConnectforms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

CheckInternetConnectforms::CheckInternetConnectforms(QWidget *parent) :
    QDialog(parent),
CheckInternetConnect_table_rows_(0),
    ui(new Ui::CheckInternetConnectforms)
{
    ui->setupUi(this);
    initForms();
}

CheckInternetConnectforms::~CheckInternetConnectforms()
{
    delete ui;
}

void CheckInternetConnectforms::btnClick()
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

void CheckInternetConnectforms::btnToggle()
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

void CheckInternetConnectforms::HandleServerMessage(const Json::Value &value,int len)
{

ui->tableWidgetCheckInternetConnect->setRowCount(len);



            insertMountFileSystemScan(value);


}

void CheckInternetConnectforms::createMenu(const QPoint &pos)
{
//    QTableWidgetItem *current_item = ui->tableWidgetCheckInternetConnect->itemAt(pos);
//    if (current_item == NULL)
//        return;

//    QMenu *pop_menu = new QMenu(this);
//    QAction *process_path = new QAction("打开进程文件所在位置", this);
//    process_path->setObjectName("process");
//    process_path->setIcon(QIcon(":/image/dir_path.png"));

//    QAction *being_opera_file_path = new QAction("打开被操作文件所在位置", this);
//    being_opera_file_path->setObjectName("file");
//    being_opera_file_path->setIcon(QIcon(":/image/dir_path.png"));

//    pop_menu->addAction(process_path);
//    pop_menu->addAction(being_opera_file_path);
//    connect(process_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
//    connect(being_opera_file_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
//    pop_menu->exec(QCursor::pos());
}

void CheckInternetConnectforms::openFileDir()
{

}

void CheckInternetConnectforms::initForms()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableWidgetCheckInternetConnect->setColumnWidth(0, 180);
    ui->tableWidgetCheckInternetConnect->setColumnWidth(2, 180);
    ui->tableWidgetCheckInternetConnect->setColumnWidth(5, 180);

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

    ui->tableWidgetCheckInternetConnect->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetCheckInternetConnect, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    installEventFilter(this);

//    connect(WebServiceClientWorker::Instance(), SIGNAL(CheckInternetConnectformsMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void CheckInternetConnectforms::insertMountFileSystemScan(const Json::Value &data)
{
    if(data.empty())
        return;


ui->tableWidgetCheckInternetConnect->setItem(CheckInternetConnect_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["destination"].asString())));
ui->tableWidgetCheckInternetConnect->setItem(CheckInternetConnect_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["result"].asString())));

CheckInternetConnect_table_rows_++;




}



void CheckInternetConnectforms::on_actCheckInternetConnectPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "CheckInternetConnectScan";


    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
}
