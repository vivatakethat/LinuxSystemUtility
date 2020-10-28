#include "BluetoothForms.h"
 #include "ui_BluetoothForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

BluetoothForms::BluetoothForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    horse_scan_table_rows_(0),
    net_behavior_table_rows_(0),
   BluetoothForms_table_rows_(0),
    ui(new Ui::BluetoothForms)
{
    ui->setupUi(this);
    initForms();
}

BluetoothForms::~BluetoothForms()
{
    delete ui;
}

void BluetoothForms::btnClick()
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

void BluetoothForms::btnToggle()
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

void BluetoothForms::HandleServerMessage(const Json::Value &value,int len)
{

    if(len!=0)
ui->tableWidgetBluetoothForms->setRowCount(len);
    else {
        ui->tableWidgetBluetoothForms->setRowCount(1);
    }


            insertBluetoothFormsScan(value);


}





void BluetoothForms::initForms()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableWidgetBluetoothForms->setColumnWidth(0, 180);
    ui->tableWidgetBluetoothForms->setColumnWidth(2, 180);
    ui->tableWidgetBluetoothForms->setColumnWidth(5, 180);

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

    ui->tableWidgetBluetoothForms->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetBluetoothForms, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    installEventFilter(this);

//    connect(WebServiceClientWorker::Instance(), SIGNAL(BluetoothFormsMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void BluetoothForms::insertBluetoothFormsScan(const Json::Value &data)
{
    if(data.empty())
    {
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString("No Bluetooth device")));
        return;
    }
    else if(data["msg"]["size"].asInt()==0)
    {
 ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString("No Bluetooth device")));
    }
    else{

ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["id"].asString())));
ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["description"].asString())));
ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["product"].asString())));
ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["vendor"].asString())));


        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["physical_id"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["bus_info"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,6, new QTableWidgetItem(QString::fromStdString(data["msg"]["logical_name"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,7, new QTableWidgetItem(QString::fromStdString(data["msg"]["version"].asString())));

        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,8, new QTableWidgetItem(QString::fromStdString(data["msg"]["serial"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,9, new QTableWidgetItem(QString::fromStdString(data["msg"]["width"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,10, new QTableWidgetItem(QString::fromStdString(data["msg"]["clock"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,11, new QTableWidgetItem(QString::fromStdString(data["msg"]["capabilities"].asString())));

        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,12, new QTableWidgetItem(QString::fromStdString(data["msg"]["configuration"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,13, new QTableWidgetItem(QString::fromStdString(data["msg"]["resources"].asString())));
        ui->tableWidgetBluetoothForms->setItem(BluetoothForms_table_rows_,14, new QTableWidgetItem(QString::fromStdString(data["msg"]["capacity"].asString())));

}
        BluetoothForms_table_rows_++;


}









void BluetoothForms::on_actBluetoothPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "BluetoothScan";


    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
    std::cout<<"line=175 "<< "blutoothScan"<<std::endl;
}
