#include "NetworkAdapterForms.h"
 #include "ui_NetworkAdapterForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

NetworkAdapterForms::NetworkAdapterForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    horse_scan_table_rows_(0),
    net_behavior_table_rows_(0),
   NetworkAdapterForms_table_rows_(0),
    ui(new Ui::NetworkAdapterForms)
{
    ui->setupUi(this);
    initForms();
}

NetworkAdapterForms::~NetworkAdapterForms()
{
    delete ui;
}

void NetworkAdapterForms::btnClick()
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

void NetworkAdapterForms::btnToggle()
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

void NetworkAdapterForms::HandleServerMessage(const Json::Value &value,int len)
{

ui->tableWidgetNetworkAdapterForms->setRowCount(len);



            insertNetworkAdapterFormsScan(value);


}





void NetworkAdapterForms::initForms()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableWidgetNetworkAdapterForms->setColumnWidth(0, 180);
    ui->tableWidgetNetworkAdapterForms->setColumnWidth(2, 180);
    ui->tableWidgetNetworkAdapterForms->setColumnWidth(5, 180);

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

    ui->tableWidgetNetworkAdapterForms->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetNetworkAdapterForms, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    installEventFilter(this);

//    connect(WebServiceClientWorker::Instance(), SIGNAL(NetworkAdapterFormsMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void NetworkAdapterForms::insertNetworkAdapterFormsScan(const Json::Value &data)
{
    if(data.empty())
        return;
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["logical_name"].asString())));
ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["ipv4"].asString())));
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["serial"].asString())));
ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["description"].asString())));
ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["product"].asString())));


ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["maskip"].asString())));
ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,6, new QTableWidgetItem(QString::fromStdString(data["msg"]["dns"].asString())));
ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,7, new QTableWidgetItem(QString::fromStdString(data["msg"]["vendor"].asString())));
ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,8, new QTableWidgetItem(QString::fromStdString(data["msg"]["ipv6"].asString())));


ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,9, new QTableWidgetItem(QString::fromStdString(data["msg"]["id"].asString())));





        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,10, new QTableWidgetItem(QString::fromStdString(data["msg"]["physical_id"].asString())));
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,11, new QTableWidgetItem(QString::fromStdString(data["msg"]["bus_info"].asString())));

        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,12, new QTableWidgetItem(QString::fromStdString(data["msg"]["version"].asString())));


        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,13, new QTableWidgetItem(QString::fromStdString(data["msg"]["width"].asString())));
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,14, new QTableWidgetItem(QString::fromStdString(data["msg"]["clock"].asString())));
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,15, new QTableWidgetItem(QString::fromStdString(data["msg"]["capabilities"].asString())));

        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,16, new QTableWidgetItem(QString::fromStdString(data["msg"]["configuration"].asString())));
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,17, new QTableWidgetItem(QString::fromStdString(data["msg"]["resources"].asString())));
        ui->tableWidgetNetworkAdapterForms->setItem(NetworkAdapterForms_table_rows_,18, new QTableWidgetItem(QString::fromStdString(data["msg"]["capacity"].asString())));


        NetworkAdapterForms_table_rows_++;


}





void NetworkAdapterForms::on_actNetworkAdapterPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "NetworkAdapterFormsScan";


    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
}
