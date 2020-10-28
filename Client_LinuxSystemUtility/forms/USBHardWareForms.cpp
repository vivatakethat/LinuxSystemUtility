#include "USBHardWareForms.h"
 #include "ui_USBHardWareForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

USBHardWareForms::USBHardWareForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    horse_scan_table_rows_(0),
    net_behavior_table_rows_(0),
   USBHardWareForms_table_rows_(0),
    ui(new Ui::USBHardWareForms)
{
    ui->setupUi(this);
    initForms();
}

USBHardWareForms::~USBHardWareForms()
{
    delete ui;
}

void USBHardWareForms::btnClick()
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

void USBHardWareForms::btnToggle()
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

void USBHardWareForms::HandleServerMessage(const Json::Value &value,int len)
{

    if(len!=0)
ui->tableWidgetUSBHardWareForms->setRowCount(len);
    else {
        ui->tableWidgetUSBHardWareForms->setRowCount(1);
    }


            insertUSBHardWareFormsScan(value);


}





void USBHardWareForms::initForms()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableWidgetUSBHardWareForms->setColumnWidth(0, 180);
    ui->tableWidgetUSBHardWareForms->setColumnWidth(2, 180);
    ui->tableWidgetUSBHardWareForms->setColumnWidth(5, 180);

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

    ui->tableWidgetUSBHardWareForms->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetUSBHardWareForms, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    installEventFilter(this);

//    connect(WebServiceClientWorker::Instance(), SIGNAL(USBHardWareFormsMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void USBHardWareForms::insertUSBHardWareFormsScan(const Json::Value &data)
{
    if(data.empty())
    {
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString("No USB device")));
        return;
    }
    else if(data["msg"]["size"].asInt()==0)
    {
 ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString("No USB device")));
    }
    else{

ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["id"].asString())));
ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["description"].asString())));
ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["product"].asString())));
ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["vendor"].asString())));


        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["physical_id"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["bus_info"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,6, new QTableWidgetItem(QString::fromStdString(data["msg"]["logical_name"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,7, new QTableWidgetItem(QString::fromStdString(data["msg"]["version"].asString())));

        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,8, new QTableWidgetItem(QString::fromStdString(data["msg"]["serial"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,9, new QTableWidgetItem(QString::fromStdString(data["msg"]["width"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,10, new QTableWidgetItem(QString::fromStdString(data["msg"]["clock"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,11, new QTableWidgetItem(QString::fromStdString(data["msg"]["capabilities"].asString())));

        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,12, new QTableWidgetItem(QString::fromStdString(data["msg"]["configuration"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,13, new QTableWidgetItem(QString::fromStdString(data["msg"]["resources"].asString())));
        ui->tableWidgetUSBHardWareForms->setItem(USBHardWareForms_table_rows_,14, new QTableWidgetItem(QString::fromStdString(data["msg"]["capacity"].asString())));

}
        USBHardWareForms_table_rows_++;


}







void USBHardWareForms::on_actUSBHardWarPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "USBHardWareScan";


    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
}
