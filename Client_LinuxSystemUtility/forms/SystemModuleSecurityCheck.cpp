#include "SystemModuleSecurityCheck.h"
#include "ui_SystemModuleSecurityCheck.h"

#include <QTableWidgetItem>
#include <QMenu>

#include "webserviceclientworker.h"
#include "utils.h"

SystemModuleSecurityCheck::SystemModuleSecurityCheck(QWidget *parent) :
    QDialog(parent),
    driver_info_table_rows_(0),
    ssdt_info_table_rows_(0),
    shadow_ssdt_table_rows_(0),
    idt_table_rows(0),
    system_module_check_rows_(0),
    ui(new Ui::SystemModuleSecurityCheck)
{
    ui->setupUi(this);
    ui->btnSSDT->hide();
    ui->btnShadowSSDT->hide();
    ui->btnIDT->hide();

    initForms();
}

SystemModuleSecurityCheck::~SystemModuleSecurityCheck()
{
    delete ui;
}

void SystemModuleSecurityCheck::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    Json::Value req;
    if (name == "btnDriver") {
        req["msg"] = "driver";
        icon.addFile(":/image/dirver_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnSSDT") {
        req["msg"] = "ssdt";
        icon.addFile(":/image/ssdt_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnShadowSSDT") {
        req["msg"] = "shadowssdt";
        icon.addFile(":/image/shadowsst_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnIDT") {
        req["msg"] = "idt";
        icon.addFile(":/image/idt_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(3);
    } else {
        return;
    }
    if(req["msg"].isString())
    {
        WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
    }
    btn->setIcon(icon);
}

void SystemModuleSecurityCheck::btnToggle()
{
    QToolButton *btn = (QToolButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
        QIcon icon;
        if (name == "btnDriver") {
            icon.addFile(":/image/dirver_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnSSDT") {
            icon.addFile(":/image/ssdt_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnShadowSSDT") {
            icon.addFile(":/image/shadowsst_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnIDT") {
            icon.addFile(":/image/idt_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else {
            return;
        }

        btn->setIcon(icon);
    }
}

void SystemModuleSecurityCheck::HandleServerMessage(const Json::Value &value,int len)
{
    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "ClearDriverInfo") {
            ui->table_SystemModule_WidgetDriver->clearContents();
            driver_info_table_rows_ = 0;

        } else if (msg_type == "DriverInfo") {
           insertDataToDriverInfo(value);
        } else if (msg_type == "ClearSSDTInfo") {
            ui->tableWidgetSSDT->clearContents();
            ssdt_info_table_rows_ = 0;
            ui->tableWidgetSSDT->setRowCount(2);
        } else if (msg_type == "SSDTInfo") {
            insertDataToSsdtInfo(value);
        } else if (msg_type == "ClearShadowSSDTInfo") {
            ui->tableWidgetShadowSSDT->clearContents();
            shadow_ssdt_table_rows_ = 0;
             ui->tableWidgetShadowSSDT->setRowCount(2);
        } else if (msg_type == "ShadowSSDTInfo") {
            insertDataToShadowSsdt(value);
        }  else if (msg_type == "ClearIDTInfo") {
            ui->tableWidgetIDT->clearContents();
            idt_table_rows = 0;
            ui->tableWidgetIDT->setRowCount(2);
        } else if (msg_type == "IDTInfo") {
            insertDataToIdt(value);
        }
    }
    else {

        std::string subMsgType = value["msg"]["type"].asString();

        ui->table_SystemModule_WidgetDriver->setRowCount(len);

        if((subMsgType=="SystemModuleReadHatEvents")||(subMsgType=="???"))//take_add:
        {
            insertDataToSystemModuleScan(value);
         }



    }
}

void SystemModuleSecurityCheck::createMenu(const QPoint &pos)
{
    QTableWidget *table_widget = (QTableWidget *)sender();

    QTableWidgetItem *current_item = table_widget->itemAt(pos);
    if (current_item == NULL)
        return;

    QMenu *pop_menu = new QMenu(this);
    QAction *dirver_file = new QAction("打开文件所在位置", this);

    QString name = table_widget->objectName();
    if (name == "table_SystemModule_WidgetDriver") {
        dirver_file->setObjectName("driver");
    } else if (name == "tableWidgetSSDT") {
        dirver_file->setObjectName("ssdt");
    } else if (name == "tableWidgetShadowSSDT") {
        dirver_file->setObjectName("shadowssdt");
    } else if (name == "tableWidgetIDT") {
        dirver_file->setObjectName("idt");
    }

    dirver_file->setIcon(QIcon(":/image/dir_path.png"));
    pop_menu->addAction(dirver_file);
    connect(dirver_file, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
    pop_menu->exec(QCursor::pos());
}

void SystemModuleSecurityCheck::openFileDir()
{
    QAction *action = (QAction *)sender();
    QString name = action->objectName();

    do {
        QString file_path;
        if (name == "driver") {
            int current_row = ui->table_SystemModule_WidgetDriver->currentRow();
            if (current_row < 0 || current_row >= driver_info_table_rows_ || current_row >= ui->table_SystemModule_WidgetDriver->rowCount())
                break;
            file_path = ui->table_SystemModule_WidgetDriver->item(current_row, 4)->text();
        } else if (name == "ssdt") {
            int current_row = ui->tableWidgetSSDT->currentRow();
            if (current_row < 0 || current_row >= ssdt_info_table_rows_ || current_row >= ui->tableWidgetSSDT->rowCount())
                break;
            file_path = ui->tableWidgetSSDT->item(current_row, 4)->text();
        } else if (name == "shadowssdt") {
            int current_row = ui->tableWidgetShadowSSDT->currentRow();
            if (current_row < 0 || current_row >= shadow_ssdt_table_rows_ || current_row >= ui->tableWidgetShadowSSDT->rowCount())
                break;
            file_path = ui->tableWidgetShadowSSDT->item(current_row, 2)->text();
        } else if (name == "idt") {
            int current_row = ui->tableWidgetIDT->currentRow();
            if (current_row < 0 || current_row >= idt_table_rows || current_row >= ui->tableWidgetIDT->rowCount())
                break;
            file_path = ui->tableWidgetIDT->item(current_row, 2)->text();
        } else{
            break;
        }

        if (file_path.isEmpty())
            break;

        openFileDirFromFullPath(file_path);
    } while (0);
}

void SystemModuleSecurityCheck::initForms()
{    ui->table_SystemModule_WidgetDriver->setRowCount(20);
    ui->stackedWidget->setCurrentIndex(0);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_2->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_3->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_4->setFrameShape(QFrame::NoFrame);

    QList<QTableWidget *> views = ui->widgetBottomDynamic->findChildren<QTableWidget *>();

    installEventFilter(this);

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


    ui->table_SystemModule_WidgetDriver->setColumnWidth(0, 180);
    ui->table_SystemModule_WidgetDriver->setColumnWidth(1, 180);
    ui->table_SystemModule_WidgetDriver->setColumnWidth(2, 61);
    ui->table_SystemModule_WidgetDriver->setColumnWidth(3, 80);
    ui->table_SystemModule_WidgetDriver->setColumnWidth(4, 280);

    ui->tableWidgetSSDT->setColumnWidth(4, 200);
    ui->tableWidgetSSDT->setColumnWidth(5, 200);
    ui->tableWidgetSSDT->setColumnWidth(3, 180);

    ui->tableWidgetShadowSSDT->setColumnWidth(1, 180);

    ui->tableWidgetIDT->setColumnWidth(1, 180);

    ui->table_SystemModule_WidgetDriver->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetSSDT->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetShadowSSDT->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetIDT->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetSSDT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->table_SystemModule_WidgetDriver, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetShadowSSDT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetIDT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

    connect(ui->btnDriver, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnIDT, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnShadowSSDT, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnSSDT, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnDriver, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnIDT, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnShadowSSDT, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnSSDT, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));

//    connect(WebServiceClientWorker::Instance(), SIGNAL(kernelAnalysisMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);
}

void SystemModuleSecurityCheck::insertDataToDriverInfo(const Json::Value &data)
{
    if (!data["DriverPath"].isString() && !data["BaseAddress"].isString() && !data["Size"].isString() &&
            !data["IsHide"].isString() && !data["IsAuth"].isString())
        return;

    ui->table_SystemModule_WidgetDriver->setSortingEnabled(false);

    if (ui->table_SystemModule_WidgetDriver->rowCount() == driver_info_table_rows_) {
        ui->table_SystemModule_WidgetDriver->setRowCount(driver_info_table_rows_ + 2);
    }

    ui->table_SystemModule_WidgetDriver->setItem(driver_info_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["BaseAddress"].asString())));
    ui->table_SystemModule_WidgetDriver->setItem(driver_info_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["Size"].asString())));
    ui->table_SystemModule_WidgetDriver->setItem(driver_info_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["IsHide"].asString())));
    ui->table_SystemModule_WidgetDriver->setItem(driver_info_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["IsAuth"].asString())));
    ui->table_SystemModule_WidgetDriver->setItem(driver_info_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["DriverPath"].asString())));


    ui->table_SystemModule_WidgetDriver->item(driver_info_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->table_SystemModule_WidgetDriver->item(driver_info_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->table_SystemModule_WidgetDriver->item(driver_info_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->table_SystemModule_WidgetDriver->item(driver_info_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->table_SystemModule_WidgetDriver->item(driver_info_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->table_SystemModule_WidgetDriver->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->table_SystemModule_WidgetDriver, driver_info_table_rows_);
    }

    driver_info_table_rows_++;

    ui->table_SystemModule_WidgetDriver->setSortingEnabled(true);
}

void SystemModuleSecurityCheck::insertDataToShadowSsdt(const Json::Value &data)
{
    if (!data["Num"].isString() && !data["NowAddr"].isString() && !data["Module"].isString())
        return;

    ui->tableWidgetShadowSSDT->setSortingEnabled(false);

    if (ui->tableWidgetShadowSSDT->rowCount() == shadow_ssdt_table_rows_) {
        ui->tableWidgetShadowSSDT->setRowCount(shadow_ssdt_table_rows_ + 2);
    }

    ui->tableWidgetShadowSSDT->setItem(shadow_ssdt_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["Num"].asString())));
    ui->tableWidgetShadowSSDT->setItem(shadow_ssdt_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["NowAddr"].asString())));
    ui->tableWidgetShadowSSDT->setItem(shadow_ssdt_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["Module"].asString())));

    ui->tableWidgetShadowSSDT->item(shadow_ssdt_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetShadowSSDT->item(shadow_ssdt_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetShadowSSDT->item(shadow_ssdt_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetShadowSSDT->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetShadowSSDT, shadow_ssdt_table_rows_);
    }

    shadow_ssdt_table_rows_++;

    ui->tableWidgetShadowSSDT->setSortingEnabled(true);
}

void SystemModuleSecurityCheck::insertDataToSsdtInfo(const Json::Value &data)
{
    if (!data["Num"].isString() && !data["FuncName"].isString() && !data["OriginalAddr"].isString() &&
            !data["NowAddr"].isString() && !data["Module"].isString() && !data["Inline Hook"].isString() &&
            !data["InlineHookModule"].isString())
        return;

    ui->tableWidgetSSDT->setSortingEnabled(false);

    if (ui->tableWidgetSSDT->rowCount() == ssdt_info_table_rows_) {
        ui->tableWidgetSSDT->setRowCount(ssdt_info_table_rows_ + 2);
    }

    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["Num"].asString()).simplified()));
    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["FuncName"].asString())));
    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["OriginalAddr"].asString())));
    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["NowAddr"].asString())));
    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["Module"].asString())));
    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["Inline Hook"].asString())));
    ui->tableWidgetSSDT->setItem(ssdt_info_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["InlineHookModule"].asString())));

    ui->tableWidgetSSDT->item(ssdt_info_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetSSDT->item(ssdt_info_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetSSDT->item(ssdt_info_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetSSDT->item(ssdt_info_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetSSDT->item(ssdt_info_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetSSDT->item(ssdt_info_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetSSDT->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetSSDT, ssdt_info_table_rows_);
    }

    ssdt_info_table_rows_++;

    ui->tableWidgetSSDT->setSortingEnabled(true);
}

void SystemModuleSecurityCheck::insertDataToIdt(const Json::Value &data)
{
    if (!data["Num"].isString() && !data["NowAddr"].isString() && !data["Module"].isString())
        return;

    ui->tableWidgetIDT->setSortingEnabled(false);

    if (ui->tableWidgetIDT->rowCount() == idt_table_rows) {
        ui->tableWidgetIDT->setRowCount(idt_table_rows + 2);
    }

    ui->tableWidgetIDT->setItem(idt_table_rows, 0, new QTableWidgetItem(QString::fromStdString(data["Num"].asString())));
    ui->tableWidgetIDT->setItem(idt_table_rows, 1, new QTableWidgetItem(QString::fromStdString(data["NowAddr"].asString())));
    ui->tableWidgetIDT->setItem(idt_table_rows, 2, new QTableWidgetItem(QString::fromStdString(data["Module"].asString())));

    ui->tableWidgetIDT->item(idt_table_rows, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetIDT->item(idt_table_rows, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetIDT->item(idt_table_rows, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetIDT->scrollToBottom();

    idt_table_rows++;

    ui->tableWidgetIDT->setSortingEnabled(true);
}


//###新增加
void SystemModuleSecurityCheck::KernelAnalysisHandle(const Json::Value &value)
{

    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "ClearDriverInfo") {
            ui->table_SystemModule_WidgetDriver->clearContents();
            driver_info_table_rows_ = 0;
             ui->table_SystemModule_WidgetDriver->setRowCount(2);
        } else if (msg_type == "DriverInfo") {
            insertDataToDriverInfo(value);
        } else if (msg_type == "ClearSSDTInfo") {
            ui->tableWidgetSSDT->clearContents();
            ssdt_info_table_rows_ = 0;
            ui->tableWidgetSSDT->setRowCount(2);
        } else if (msg_type == "SSDTInfo") {
            insertDataToSsdtInfo(value);
        } else if (msg_type == "ClearShadowSSDTInfo") {
            ui->tableWidgetShadowSSDT->clearContents();
            shadow_ssdt_table_rows_ = 0;
              ui->tableWidgetShadowSSDT->setRowCount(2);
        } else if (msg_type == "ShadowSSDTInfo") {
            insertDataToShadowSsdt(value);
        }  else if (msg_type == "ClearIDTInfo") {
            ui->tableWidgetIDT->clearContents();
            idt_table_rows = 0;
             ui->tableWidgetIDT->setRowCount(2);
        } else if (msg_type == "IDTInfo") {
            insertDataToIdt(value);
        }
    }
}

void SystemModuleSecurityCheck::on_actSystemModuleScan_pushButton_clicked()
{


    Json::Value req;

        req["msg"] = "SystemModuletEvents";
    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
   std::cout<<__LINE__<<__FUNCTION__<<std::endl;

}
 void SystemModuleSecurityCheck::insertDataToSystemModuleScan(const Json::Value &data)
{
            if(data.empty())
                    return;
            std::string subMsgType = data["msg"]["type"].asString();
            if(subMsgType=="SystemModuleReadHatEvents")
            {

//ui->tableWidgetFileSystemInfo->setItem(filsystem_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["msg"]["DevPath"].asString())));
//  ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,0,new QTableWidgetItem(QString::fromStdString(data["msg"]["SystemModuleReadHatEvents"].asString())));
  ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,0,new QTableWidgetItem(QString::fromStdString(data["msg"]["ModuleName"].asString())));
  ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,1,new QTableWidgetItem(QString::fromStdString(data["msg"]["MemorySizeModulee"].asString())));

  ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,2,new QTableWidgetItem(QString::fromStdString(data["msg"]["LoadedTimes"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,3,new QTableWidgetItem(QString::fromStdString(data["msg"]["DependentModule"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,4,new QTableWidgetItem(QString::fromStdString(data["msg"]["MoslueState"].asString())));

            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,5,new QTableWidgetItem(QString::fromStdString(data["msg"]["OffsetModule"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,6,new QTableWidgetItem(QString::fromStdString(data["msg"]["FileName"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,7,new QTableWidgetItem(QString::fromStdString(data["msg"]["License"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,8,new QTableWidgetItem(QString::fromStdString(data["msg"]["Description"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,9,new QTableWidgetItem(QString::fromStdString(data["msg"]["Author"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,10,new QTableWidgetItem(QString::fromStdString(data["msg"]["srcVersion"].asString())));
            ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,11,new QTableWidgetItem(QString::fromStdString(data["msg"]["Depends"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,12,new QTableWidgetItem(QString::fromStdString(data["msg"]["Retpoline"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,13,new QTableWidgetItem(QString::fromStdString(data["msg"]["Intree"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,14,new QTableWidgetItem(QString::fromStdString(data["msg"]["Vermagic"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,15,new QTableWidgetItem(QString::fromStdString(data["msg"]["Sig_id"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,16,new QTableWidgetItem(QString::fromStdString(data["msg"]["Signer"].asString())));

                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,17,new QTableWidgetItem(QString::fromStdString(data["msg"]["Sig_Key"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,18,new QTableWidgetItem(QString::fromStdString(data["msg"]["Sig_HashAlg"].asString())));
                   ui->table_SystemModule_WidgetDriver->setItem(system_module_check_rows_,19,new QTableWidgetItem(QString::fromStdString(data["msg"]["Signature"].asString())));
system_module_check_rows_++;
            }

 }
