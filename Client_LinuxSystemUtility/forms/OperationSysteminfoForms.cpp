﻿#include "OperationSysteminfoForms.h"
 #include "ui_OperationSysteminfoForms.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

OperationSysteminfoForms::OperationSysteminfoForms(QWidget *parent) :
    QDialog(parent),
    real_time_monitor_table_rows_(0),
    horse_scan_table_rows_(0),
    net_behavior_table_rows_(0),
   OperationSysteminfo_table_rows_(0),
    ui(new Ui::OperationSysteminfoForms)
{
    ui->setupUi(this);
    initForms();
}

OperationSysteminfoForms::~OperationSysteminfoForms()
{
    delete ui;
}

void OperationSysteminfoForms::btnClick()
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

void OperationSysteminfoForms::btnToggle()
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

void OperationSysteminfoForms::HandleServerMessage(const Json::Value &value,int len)
{

            ui->tableWidgetOperationSysteminfoForms->setRowCount(len);



            insertOperationSysteminfoScan(value);


}





void OperationSysteminfoForms::initForms()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableWidgetOperationSysteminfoForms->setColumnWidth(0, 180);
    ui->tableWidgetOperationSysteminfoForms->setColumnWidth(2, 180);
    ui->tableWidgetOperationSysteminfoForms->setColumnWidth(5, 180);

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

    ui->tableWidgetOperationSysteminfoForms->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetOperationSysteminfoForms, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    installEventFilter(this);

//    connect(WebServiceClientWorker::Instance(), SIGNAL(OperationSysteminfoFormsMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void OperationSysteminfoForms::insertOperationSysteminfoScan(const Json::Value &data)
{
    if(data.empty())
        return;



ui->tableWidgetOperationSysteminfoForms->setItem(OperationSysteminfo_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["OS"].asString())));
ui->tableWidgetOperationSysteminfoForms->setItem(OperationSysteminfo_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["Architecture"].asString())));
ui->tableWidgetOperationSysteminfoForms->setItem(OperationSysteminfo_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["CPU"].asString())));
ui->tableWidgetOperationSysteminfoForms->setItem(OperationSysteminfo_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["RAM"].asString())));


        ui->tableWidgetOperationSysteminfoForms->setItem(OperationSysteminfo_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["Hostname"].asString())));
        ui->tableWidgetOperationSysteminfoForms->setItem(OperationSysteminfo_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["SystemPath"].asString())));


        OperationSysteminfo_table_rows_++;


}



void OperationSysteminfoForms::on_actOperationSysteminfoPushButton_clicked()
{
    Json::Value req;

        req["msg"] = "OperationSysteminfoScan";


    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
}
