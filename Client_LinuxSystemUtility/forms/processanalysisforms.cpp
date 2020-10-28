#include "processanalysisforms.h"
#include "ui_processanalysisforms.h"

#include <QTableWidgetItem>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "webserviceclientworker.h"
#include "utils.h"

ProcessAnalysisForms::ProcessAnalysisForms(QWidget *parent) :
    QDialog(parent),
    process_info_table_rows_(0),
    self_start_table_rows_(0),
    message_hook_table_rows_(0),
    port_info_table_rows_(0),
    iat_info_table_rows_(0),
    history_process_table_rows_(0),
    ui(new Ui::ProcessAnalysisForms)
{
    ui->setupUi(this);
    initForms();
//    insertTestData();
}

ProcessAnalysisForms::~ProcessAnalysisForms()
{
    delete ui;
}

void ProcessAnalysisForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    Json::Value req;
    if (name == "btnProcessInfo") {

        icon.addFile(":/image/process_check_orange.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "btnSelfStart") {
        req["msg"] = "autorun";
        icon.addFile(":/image/auto_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnMessageHook") {
        req["msg"] = "messagehook";
        icon.addFile(":/image/hook_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "btnPortInfo") {
        icon.addFile(":/image/port_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(3);
    } else if (name == "btnIATInfo") {
        req["msg"] = "iat";
        icon.addFile(":/image/iat_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(4);
    } else if (name == "btnHistoryPorcess") {
        req["msg"] = "historyprocess";
        icon.addFile(":/image/history_check.png", QSize(), QIcon::Normal, QIcon::Off);
        ui->stackedWidget->setCurrentIndex(5);
    } else {
        return;
    }
    if(req["msg"].isString())
    {
        WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
    }
    btn->setIcon(icon);
}




void ProcessAnalysisForms::btnToggle()
{
    QToolButton *btn = (QToolButton *)sender();

    if (!btn->isChecked()) {
        QString name = btn->objectName();
        QIcon icon;
        if (name == "btnProcessInfo") {
            icon.addFile(":/image/process_normal.png", QSize(), QIcon::Normal, QIcon::Off);
            ui->stackedWidget->setCurrentIndex(0);
        } else if (name == "btnSelfStart") {
            icon.addFile(":/image/auto_normal.png", QSize(), QIcon::Normal, QIcon::Off);
            ui->stackedWidget->setCurrentIndex(1);
        } else if (name == "btnMessageHook") {
            icon.addFile(":/image/hook_normal.png", QSize(), QIcon::Normal, QIcon::Off);
            ui->stackedWidget->setCurrentIndex(2);
        } else if (name == "btnPortInfo") {
            icon.addFile(":/image/port_normal.png", QSize(), QIcon::Normal, QIcon::Off);
            ui->stackedWidget->setCurrentIndex(3);
        } else if (name == "btnIATInfo") {
            icon.addFile(":/image/iat_normal.png", QSize(), QIcon::Normal, QIcon::Off);
            ui->stackedWidget->setCurrentIndex(4);
        } else if (name == "btnHistoryPorcess") {
            icon.addFile(":/image/history_normal.png", QSize(), QIcon::Normal, QIcon::Off);
            ui->stackedWidget->setCurrentIndex(5);
        } else {
            return;
        }

        btn->setIcon(icon);
    }
}

void ProcessAnalysisForms::HandleServerMessage(const Json::Value &value)
{
    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "ClearProcessInfo") {
            ui->tableWidgetProcessInfo->clearContents();
            process_info_table_rows_ = 0;
            ui->tableWidgetProcessInfo->setRowCount(2);
        } else if (msg_type == "ClearHistoryProcessInfo") {
            ui->tableWidgetProcessHistory->clearContents();
            history_process_table_rows_ = 0;
            ui->tableWidgetProcessHistory->setRowCount(2);
        } else if (msg_type == "ProcessInfo") {
           insertDataToProcessInfo(value);
        } else if (msg_type == "HistoryProcessInfo") {
            insertDataToHistoryProcess(value);
        } else if (msg_type == "ClearAutorunInfo") {
            ui->tableWidgetSelfStart->clearContents();
            self_start_table_rows_ = 0;
            ui->tableWidgetSelfStart->setRowCount(2);
        } else if (msg_type == "AutoRunInfo") {
            insertDataToSelfStart(value);
        } else if (msg_type == "ClearMessageHookInfo") {
            ui->tableWidgetMessageHook->clearContents();
            message_hook_table_rows_ = 0;
            ui->tableWidgetMessageHook->setRowCount(2);
        } else if (msg_type == "MessageHookInfo") {
            insertDataToMessageHook(value);
        } else if (msg_type == "ClearPortInfo") {
            ui->tableWidgetPortInfo->clearContents();
            port_info_table_rows_ = 0;
            ui->tableWidgetPortInfo->setRowCount(2);
        } else if (msg_type == "PortInfo") {
            insertDataToPortInfo(value);
        } else if (msg_type == "ClearIATInfo") {
            ui->tableWidgetIATInfo->clearContents();
            iat_info_table_rows_ = 0;
            ui->tableWidgetIATInfo->setRowCount(2);
        } else if (msg_type == "IATInfo") {
            insertDataToIatInfo(value);
        } else if (msg_type == "ShowDllRespond") {
            showProcessDllDetail(value);
        }
    }
}

void ProcessAnalysisForms::createMenu(const QPoint &pos)
{
    QTableWidget *table_widget = (QTableWidget *)sender();

    QTableWidgetItem *current_item = table_widget->itemAt(pos);
    if (current_item == NULL)
        return;

    QString name = table_widget->objectName();
    if (name == "tableWidgetProcessInfo") {
        QMenu *pop_menu = new QMenu(this);
        QAction *process_dll_detail = new QAction("查看进程dll详情", this);
        process_dll_detail->setIcon(QIcon(":/image/dll_detail_info.png"));
        QAction *open_file_path = new QAction("打开文件所在位置", this);
        open_file_path->setObjectName("process_path");
        open_file_path->setIcon(QIcon(":/image/dir_path.png"));
        pop_menu->addAction(process_dll_detail);
        pop_menu->addAction(open_file_path);
        connect(process_dll_detail, SIGNAL(triggered(bool)), this, SLOT(showProcessDllDetailAction()));
        connect(open_file_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
        pop_menu->exec(QCursor::pos());
    } else if (name == "tableWidgetProcessHistory") {
        QMenu *pop_menu = new QMenu(this);
        QAction *history_process_file_path = new QAction("打开文件所在位置", this);
        history_process_file_path->setObjectName("history_process_path");
        history_process_file_path->setIcon(QIcon(":/image/dir_path.png"));
        pop_menu->addAction(history_process_file_path);
        connect(history_process_file_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
        pop_menu->exec(QCursor::pos());
    } else if (name == "tableWidgetSelfStart") {
        QMenu *pop_menu = new QMenu(this);
        QAction *module_file_path = new QAction("打开文件所在位置", this);
        module_file_path->setObjectName("self_module_path");
        module_file_path->setIcon(QIcon(":/image/dir_path.png"));
        pop_menu->addAction(module_file_path);
        connect(module_file_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
        pop_menu->exec(QCursor::pos());
    } else if (name == "tableWidgetMessageHook") {
        QMenu *pop_menu = new QMenu(this);
        QAction *message_hook_file_path = new QAction("打开文件所在位置", this);
        message_hook_file_path->setObjectName("hook_process_path");
        message_hook_file_path->setIcon(QIcon(":/image/dir_path.png"));
        pop_menu->addAction(message_hook_file_path);
        connect(message_hook_file_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
        pop_menu->exec(QCursor::pos());
    } else if (name == "tableWidgetPortInfo") {
        QMenu *pop_menu = new QMenu(this);
        QAction *port_process_path = new QAction("打开文件所在位置", this);
        port_process_path->setObjectName("port_process_path");
        port_process_path->setIcon(QIcon(":/image/dir_path.png"));
        pop_menu->addAction(port_process_path);
        connect(port_process_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
        pop_menu->exec(QCursor::pos());
    } else if (name == "tableWidgetIATInfo") {
        QMenu *pop_menu = new QMenu(this);
        QAction *iat_module_path = new QAction("打开文件所在位置", this);
        iat_module_path->setObjectName("iat_module_path");
        iat_module_path->setIcon(QIcon(":/image/dir_path.png"));
        pop_menu->addAction(iat_module_path);
        connect(iat_module_path, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
        pop_menu->exec(QCursor::pos());
    }
}

void ProcessAnalysisForms::showProcessDllDetailAction()
{
    int row = ui->tableWidgetProcessInfo->currentRow();
    if (row < 0 || row > process_info_table_rows_)
        return;

    QString process_id = ui->tableWidgetProcessInfo->item(row, 1)->text();
    show_destail_dll_name_ = ui->tableWidgetProcessInfo->item(row, 0)->text();
    if (!process_id.isEmpty()) {
        Json::Value req;
        req["msg"] = "ShowDll";
        req["pid"] = process_id.toStdString();

        WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));
    }
}

void ProcessAnalysisForms::openFileDir()
{
    QAction *action = (QAction *)sender();
    QString name = action->objectName();

    do {
        QString file_path;
        if (name == "process_path") {
            int current_row = ui->tableWidgetProcessInfo->currentRow();
            if (current_row < 0 || current_row >= process_info_table_rows_ || current_row >= ui->tableWidgetProcessInfo->rowCount())
                break;
            file_path = ui->tableWidgetProcessInfo->item(current_row, 5)->text();
        }  else if (name == "history_process_path") {
            int current_row = ui->tableWidgetProcessHistory->currentRow();
            if (current_row < 0 || current_row >= history_process_table_rows_ || current_row >= ui->tableWidgetProcessHistory->rowCount())
                break;
            file_path = ui->tableWidgetProcessHistory->item(current_row, 5)->text();
        }else if (name == "self_module_path") {
            int current_row = ui->tableWidgetSelfStart->currentRow();
            if (current_row < 0 || current_row >= self_start_table_rows_ || current_row >= ui->tableWidgetSelfStart->rowCount())
                break;
            file_path = ui->tableWidgetSelfStart->item(current_row, 2)->text();
        } else if (name == "hook_process_path") {
            int current_row = ui->tableWidgetMessageHook->currentRow();
            if (current_row < 0 || current_row >= message_hook_table_rows_ || current_row >= ui->tableWidgetMessageHook->rowCount())
                break;
            file_path = ui->tableWidgetMessageHook->item(current_row, 6)->text();
        } else if (name == "port_process_path") {
            int current_row = ui->tableWidgetPortInfo->currentRow();
            if (current_row < 0 || current_row >= port_info_table_rows_ || current_row >= ui->tableWidgetPortInfo->rowCount())
                break;
            file_path = ui->tableWidgetPortInfo->item(current_row, 5)->text();
        } else if (name == "iat_module_path") {
            int current_row = ui->tableWidgetIATInfo->currentRow();
            if (current_row < 0 || current_row >= iat_info_table_rows_ || current_row >= ui->tableWidgetIATInfo->rowCount())
                break;
            file_path = ui->tableWidgetIATInfo->item(current_row, 5)->text();
        } else {
            break;
        }

        openFileDirFromFullPath(file_path);
    } while (0);
}

void ProcessAnalysisForms::initForms()
{
    ui->stackedWidget->setCurrentIndex(0);

    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_2->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_3->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_4->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_5->setFrameShape(QFrame::NoFrame);
    ui->scrollArea_6->setFrameShape(QFrame::NoFrame);

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

    ui->tableWidgetProcessHistory->setColumnWidth(3, 60);
    ui->tableWidgetProcessHistory->setColumnWidth(4, 80);
    ui->tableWidgetProcessInfo->setColumnWidth(3, 60);
    ui->tableWidgetProcessInfo->setColumnWidth(4, 80);
//    ui->tableWidgetSelfStart->setColumnWidth(3, 80);
//    ui->tableWidgetSelfStart->setColumnWidth(4, 215);
    ui->tableWidgetSelfStart->setColumnWidth(1, 200);
    ui->tableWidgetSelfStart->setColumnWidth(2, 215);
    ui->tableWidgetSelfStart->setColumnWidth(3, 80);
    ui->tableWidgetMessageHook->setColumnWidth(6, 200);

    ui->tableWidgetProcessHistory->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetProcessInfo->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetSelfStart->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetMessageHook->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetPortInfo->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetIATInfo->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(ui->btnIATInfo, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnMessageHook, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnProcessInfo, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnPortInfo, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnSelfStart, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnHistoryPorcess, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnIATInfo, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnMessageHook, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnProcessInfo, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnPortInfo, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnSelfStart, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnHistoryPorcess, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->tableWidgetProcessInfo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetSelfStart, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetMessageHook, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetPortInfo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetIATInfo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(ui->tableWidgetProcessHistory, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));


//    connect(WebServiceClientWorker::Instance(), SIGNAL(processAnalysisMessage(QString)), this, SLOT(handleServerMessage(QString)), Qt::QueuedConnection);

   // show_dll_details_windows = new OtherTableForms();
}

void ProcessAnalysisForms::insertDataToProcessInfo(const Json::Value &data)
{
    if (!data["ProcessName"].isString() && !data["PID"].isString() && !data["PPID"].isString() &&
            !data["IsHide"].isString() && !data["IsAuth"].isString() && !data["Path"].isString())
        return;

    ui->tableWidgetProcessInfo->setSortingEnabled(false);

    if (ui->tableWidgetProcessInfo->rowCount() == process_info_table_rows_) {
        ui->tableWidgetProcessInfo->setRowCount(process_info_table_rows_ + 2);
    }

    ui->tableWidgetProcessInfo->setItem(process_info_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["ProcessName"].asString())));
    ui->tableWidgetProcessInfo->setItem(process_info_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
    ui->tableWidgetProcessInfo->setItem(process_info_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["PPID"].asString())));
    ui->tableWidgetProcessInfo->setItem(process_info_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["IsHide"].asString())));
    ui->tableWidgetProcessInfo->setItem(process_info_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["IsAuth"].asString())));
    ui->tableWidgetProcessInfo->setItem(process_info_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["Path"].asString())));

    ui->tableWidgetProcessInfo->item(process_info_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessInfo->item(process_info_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessInfo->item(process_info_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessInfo->item(process_info_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessInfo->item(process_info_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessInfo->item(process_info_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetProcessInfo->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetProcessInfo, process_info_table_rows_);
    }

    process_info_table_rows_++;

    ui->tableWidgetProcessInfo->setSortingEnabled(true);
}

void ProcessAnalysisForms::insertDataToSelfStart(const Json::Value &data)
{
    if (!data["RunItem"].isString() && !data["RegPath"].isString() && !data["FilePath"].isString() &&
            !data["IsAuth"].isString() && !data["Describe"].isString() && !data["Company"].isString())
        return;

    ui->tableWidgetSelfStart->setSortingEnabled(false);

    if (ui->tableWidgetSelfStart->rowCount() == self_start_table_rows_) {
        ui->tableWidgetSelfStart->setRowCount(self_start_table_rows_ + 2);
    }

    ui->tableWidgetSelfStart->setItem(self_start_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["RunItem"].asString())));
    ui->tableWidgetSelfStart->setItem(self_start_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["RegPath"].asString())));
    ui->tableWidgetSelfStart->setItem(self_start_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["FilePath"].asString())));
    ui->tableWidgetSelfStart->setItem(self_start_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["IsAuth"].asString())));
    ui->tableWidgetSelfStart->setItem(self_start_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["Company"].asString())));
    ui->tableWidgetSelfStart->setItem(self_start_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["Describe"].asString())));

    ui->tableWidgetSelfStart->item(self_start_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetSelfStart->item(self_start_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetSelfStart->item(self_start_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetSelfStart->item(self_start_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetSelfStart->item(self_start_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetSelfStart->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetSelfStart, self_start_table_rows_);
    }

    self_start_table_rows_++;

    ui->tableWidgetSelfStart->setSortingEnabled(true);
}

void ProcessAnalysisForms::insertDataToMessageHook(const Json::Value &data)
{
    if (!data["Handle"].isString() && !data["HookType"].isString() && !data["HookFuncAddr"].isString() &&
            !data["PID"].isString() && !data["TID"].isString() && !data["HookModule"].isString() &&
            !data["ProcessPath"].isString())
        return;

    ui->tableWidgetMessageHook->setSortingEnabled(false);

    if (ui->tableWidgetMessageHook->rowCount() == message_hook_table_rows_) {
        ui->tableWidgetMessageHook->setRowCount(message_hook_table_rows_ + 2);
    }

    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["Handle"].asString())));
    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["HookType"].asString())));
    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["HookFuncAddr"].asString())));
    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["TID"].asString())));
    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["HookModule"].asString())));
    ui->tableWidgetMessageHook->setItem(message_hook_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["ProcessPath"].asString())));

    ui->tableWidgetMessageHook->item(message_hook_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetMessageHook->item(message_hook_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetMessageHook->item(message_hook_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetMessageHook->item(message_hook_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetMessageHook->item(message_hook_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetMessageHook->item(message_hook_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetMessageHook->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetMessageHook, message_hook_table_rows_);
    }

    message_hook_table_rows_++;

    ui->tableWidgetMessageHook->setSortingEnabled(true);
}

void ProcessAnalysisForms::insertDataToPortInfo(const Json::Value &data)
{
    if (!data["Protocol"].isString() && !data["LocalAddr"].isString() && !data["RemoteAddr"].isString() &&
            !data["Status"].isString() && !data["PID"].isString() && !data["ProcessPath"].isString())
        return;

    ui->tableWidgetPortInfo->setSortingEnabled(false);

    if (ui->tableWidgetPortInfo->rowCount() == port_info_table_rows_) {
        ui->tableWidgetPortInfo->setRowCount(port_info_table_rows_ + 2);
    }

    ui->tableWidgetPortInfo->setItem(port_info_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["Protocol"].asString())));
    ui->tableWidgetPortInfo->setItem(port_info_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["LocalAddr"].asString())));
    ui->tableWidgetPortInfo->setItem(port_info_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["RemoteAddr"].asString())));
    ui->tableWidgetPortInfo->setItem(port_info_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["Status"].asString())));
    ui->tableWidgetPortInfo->setItem(port_info_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
    ui->tableWidgetPortInfo->setItem(port_info_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["ProcessPath"].asString())));

    ui->tableWidgetPortInfo->item(port_info_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetPortInfo->item(port_info_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetPortInfo->item(port_info_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetPortInfo->item(port_info_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetPortInfo->item(port_info_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetPortInfo->scrollToBottom();

    port_info_table_rows_++;

    ui->tableWidgetPortInfo->setSortingEnabled(true);
}

void ProcessAnalysisForms::insertDataToIatInfo(const Json::Value &data)
{
    if (!data["ProcessPath"].isString() && !data["PID"].isString() && !data["OriginalAddr"].isString() &&
            !data["JumpAddr"].isString() && !data["IatHookFunc"].isString() && !data["IatHookModule"].isString())
        return;

    ui->tableWidgetIATInfo->setSortingEnabled(false);

    if (ui->tableWidgetIATInfo->rowCount() == iat_info_table_rows_) {
        ui->tableWidgetIATInfo->setRowCount(iat_info_table_rows_ + 2);
    }

    ui->tableWidgetIATInfo->setItem(iat_info_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["ProcessPath"].asString())));
    ui->tableWidgetIATInfo->setItem(iat_info_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
    ui->tableWidgetIATInfo->setItem(iat_info_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["OriginalAddr"].asString())));
    ui->tableWidgetIATInfo->setItem(iat_info_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["JumpAddr"].asString())));
    ui->tableWidgetIATInfo->setItem(iat_info_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["IatHookFunc"].asString())));
    ui->tableWidgetIATInfo->setItem(iat_info_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["IatHookModule"].asString())));

    ui->tableWidgetIATInfo->item(iat_info_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetIATInfo->item(iat_info_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetIATInfo->item(iat_info_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetIATInfo->item(iat_info_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidgetIATInfo->item(iat_info_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetIATInfo->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetIATInfo, iat_info_table_rows_);
    }

    iat_info_table_rows_++;

    ui->tableWidgetIATInfo->setSortingEnabled(true);
}

void ProcessAnalysisForms::insertDataToHistoryProcess(const Json::Value &data)
{
    if (!data["ProcessName"].isString() && !data["PID"].isString() && !data["PPID"].isString() &&
            !data["IsHide"].isString() && !data["IsAuth"].isString() && !data["Path"].isString())
        return;

    ui->tableWidgetProcessHistory->setSortingEnabled(false);

    if (ui->tableWidgetProcessHistory->rowCount() == history_process_table_rows_) {
        ui->tableWidgetProcessHistory->setRowCount(history_process_table_rows_ + 2);
    }

    ui->tableWidgetProcessHistory->setItem(history_process_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(data["ProcessName"].asString())));
    ui->tableWidgetProcessHistory->setItem(history_process_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(data["PID"].asString())));
    ui->tableWidgetProcessHistory->setItem(history_process_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["PPID"].asString())));
    ui->tableWidgetProcessHistory->setItem(history_process_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["IsHide"].asString())));
    ui->tableWidgetProcessHistory->setItem(history_process_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["IsAuth"].asString())));
    ui->tableWidgetProcessHistory->setItem(history_process_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["Path"].asString())));

    ui->tableWidgetProcessHistory->item(history_process_table_rows_, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessHistory->item(history_process_table_rows_, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessHistory->item(history_process_table_rows_, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessHistory->item(history_process_table_rows_, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessHistory->item(history_process_table_rows_, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidgetProcessHistory->item(history_process_table_rows_, 5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    if (auto_roll)
        ui->tableWidgetProcessHistory->scrollToBottom();

    if (data["Color"].isString()) {
        setRowContentColor(QString::fromStdString(data["Color"].asString()), ui->tableWidgetProcessHistory, history_process_table_rows_);
    }

    history_process_table_rows_++;

    ui->tableWidgetProcessHistory->setSortingEnabled(true);
}

void ProcessAnalysisForms::showProcessDllDetail(const Json::Value &data)
{
    QString title = QString("进程%1的DLL信息").arg(show_destail_dll_name_);
   // show_dll_details_windows->SetData(title, data);
}

void ProcessAnalysisForms::insertTestData()
{
    ui->tableWidgetProcessInfo->setRowCount(10);
    for (int i = 0; i < 10; i++) {
        ui->tableWidgetProcessInfo->setItem(i, 0, new QTableWidgetItem("苟富贵"));
        ui->tableWidgetProcessInfo->setItem(i, 1, new QTableWidgetItem("苟富贵"));
        ui->tableWidgetProcessInfo->setItem(i, 2, new QTableWidgetItem("苟富贵"));
        ui->tableWidgetProcessInfo->setItem(i, 3, new QTableWidgetItem("苟富贵"));
        ui->tableWidgetProcessInfo->setItem(i, 4, new QTableWidgetItem("苟富贵"));
        ui->tableWidgetProcessInfo->setItem(i, 5, new QTableWidgetItem("苟富贵"));
    }

    Json::Value dll_data;
    Json::Value data;

    for (int i = 0; i < 10; i++) {
        data[i]["ProcessPath"] = "发分";
        data[i]["PID"] = "发分";
        data[i]["OriginalAddr"] = "发分";
        data[i]["JumpAddr"] = "发分";
        data[i]["IatHookFunc"] = "发分";
        data[i]["IatHookModule"] = "发分";
    }
    dll_data["content"] = data;


    showProcessDllDetail(dll_data);
}


void ProcessAnalysisForms::ProcessAnalysisHandle(const Json::Value &value)
{
    if (value["msg"].isString()) {
        std::string msg_type = value["msg"].asString();
        if (msg_type == "ClearHistoryProcessInfo") {
            ui->tableWidgetProcessHistory->clearContents();
            history_process_table_rows_ = 0;
            ui->tableWidgetProcessHistory->setRowCount(2);
        } else if (msg_type == "HistoryProcessInfo") {
            insertDataToHistoryProcess(value);
        } else if (msg_type == "ClearAutorunInfo") {
            ui->tableWidgetSelfStart->clearContents();
            self_start_table_rows_ = 0;
            ui->tableWidgetSelfStart->setRowCount(2);
        } else if (msg_type == "AutoRunInfo") {
            insertDataToSelfStart(value);
        } else if (msg_type == "ClearMessageHookInfo") {
            ui->tableWidgetMessageHook->clearContents();
            message_hook_table_rows_ = 0;
            ui->tableWidgetMessageHook->setRowCount(2);
        } else if (msg_type == "MessageHookInfo") {
            insertDataToMessageHook(value);
        } else if (msg_type == "ClearIATInfo") {
            ui->tableWidgetIATInfo->clearContents();
            iat_info_table_rows_ = 0;
            ui->tableWidgetIATInfo->setRowCount(2);
        } else if (msg_type == "IATInfo") {
            insertDataToIatInfo(value);
        } else if (msg_type == "ShowDllRespond") {
            showProcessDllDetail(value);
        }
    }
}
