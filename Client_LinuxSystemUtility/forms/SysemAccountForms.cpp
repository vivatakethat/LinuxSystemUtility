#include "SysemAccountForms.h"
#include "ui_SysemAccountForms.h"

#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QMenu>
#include <QSizeGrip>
#include "webserviceclientworker.h"
#include "iconfont.h"
#include "utils.h"

SysemAccountForms::SysemAccountForms(QWidget *parent) :
    QDialog(parent),
    mouse_pressed_(false),
       SystemAccount_table_rows_(0),
    ui(new Ui::SysemAccountForms)
{
    ui->setupUi(this);
    initStyle();
    initForms();
//    insertData();
}

SysemAccountForms::~SysemAccountForms()
{
    delete ui;
}

void SysemAccountForms::initStyle()
{
  //  this->setWindowTitle("金密恶意程序辅助检测工具");
      this->setWindowTitle("Linux终端设备检测工具");
    this->setWindowIcon(QIcon(":/image/blue.svg"));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
   // this->setWindowTitle("金密恶意程序辅助检测工具");
this->setWindowTitle("Linux终端设备检测工具");
    this->move((QApplication::desktop()->width() - this->width()) / 2,  (QApplication::desktop()->height() - this->height()) / 2);
}

void SysemAccountForms::initForms()
{
    ui->scrollArea->setFrameShape(QFrame::NoFrame);

    ui->tableSystemAccountWidget->verticalHeader()->setHidden(true);
    ui->tableSystemAccountWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableSystemAccountWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableSystemAccountWidget->setAlternatingRowColors(false);
    ui->tableSystemAccountWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableSystemAccountWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableSystemAccountWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableSystemAccountWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableSystemAccountWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableSystemAccountWidget->verticalHeader()->setDefaultSectionSize(40);
    ui->tableSystemAccountWidget->setSortingEnabled(true);

    ui->tableSystemAccountWidget->setFixedHeight(493);

    ui->tableSystemAccountWidget->setColumnWidth(3, 61);
    ui->tableSystemAccountWidget->setColumnWidth(4, 80);

    installEventFilter(this);

    ui->tableSystemAccountWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableSystemAccountWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

   // connect(ui->btnOtherTableClose, SIGNAL(clicked(bool)), this, SLOT(hide()));

 //   IconFont::Instance()->SetIcon(ui->btnOtherTableClose, QChar(0xf00d), 12);

    setWindowModality(Qt::ApplicationModal);

    ui->verticalLayout->addWidget(new QSizeGrip(this), 0, Qt::AlignBottom | Qt::AlignRight);
}

void SysemAccountForms::insertData()
{
    ui->tableSystemAccountWidget->clearContents();
    ui->tableSystemAccountWidget->setFixedHeight(20 * 41);
    ui->tableSystemAccountWidget->setRowCount(20);

    for (int i = 0; i < 10; i++) {
        ui->tableSystemAccountWidget->setItem(i, 0, new QTableWidgetItem("0x00003213"));
        ui->tableSystemAccountWidget->setItem(i, 1, new QTableWidgetItem("0x0003123"));
        ui->tableSystemAccountWidget->setItem(i, 2, new QTableWidgetItem("c/fsdaf/erwr"));
        ui->tableSystemAccountWidget->setItem(i, 3, new QTableWidgetItem("是"));
        ui->tableSystemAccountWidget->setItem(i, 4, new QTableWidgetItem("未认证"));

        ui->tableSystemAccountWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableSystemAccountWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableSystemAccountWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableSystemAccountWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

        void  SysemAccountForms::insertSystemAccountScan(const Json::Value &data)
        {
                                                                        ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,0, new QTableWidgetItem(QString::fromStdString(data["msg"]["UserName"].asString())));

            ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,1, new QTableWidgetItem(QString::fromStdString(data["msg"]["LastPasswordChange"].asString())));
                              ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,2, new QTableWidgetItem(QString::fromStdString(data["msg"]["PasswdExpires"].asString())));
                                        ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,3, new QTableWidgetItem(QString::fromStdString(data["msg"]["PasswordInactive"].asString())));
            ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,4, new QTableWidgetItem(QString::fromStdString(data["msg"]["AccountExpires"].asString())));
            ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,5, new QTableWidgetItem(QString::fromStdString(data["msg"]["Minimum_number_of_days_between_password_change"].asString())));


            ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,6, new QTableWidgetItem(QString::fromStdString(data["msg"]["Maximum_number_of_days_between_password_change"].asString())));


                    ui->tableSystemAccountWidget->setItem(SystemAccount_table_rows_,7, new QTableWidgetItem(QString::fromStdString(data["msg"]["Number_of_days_of_warning_before_password_expires"].asString())));





                    SystemAccount_table_rows_++;


        }

    void SysemAccountForms::HandleServerMessage(const Json::Value &value,int len)
    {
        ui->tableSystemAccountWidget->setRowCount(len);
                  insertSystemAccountScan(value);


    }

void SysemAccountForms::SetData(const QString &title, const Json::Value &data)
{
    ui->labelOtherTableTitle->setText(title);

    ui->tableSystemAccountWidget->clearContents();

    if (data["DllList"].isArray()) {
        ui->tableSystemAccountWidget->setRowCount(data["DllList"].size());

        for (int i = 0; i < data["DllList"].size(); i++) {
            ui->tableSystemAccountWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data["DllList"][i]["BaseAddress"].asString())));
            ui->tableSystemAccountWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data["pid"].asString())));
            ui->tableSystemAccountWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data["DllList"][i]["Size"].asString())));
            ui->tableSystemAccountWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(data["DllList"][i]["IsHide"].asString())));
            ui->tableSystemAccountWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(data["DllList"][i]["IsAuth"].asString())));
            ui->tableSystemAccountWidget->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(data["DllList"][i]["DllPath"].asString())));

            ui->tableSystemAccountWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableSystemAccountWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableSystemAccountWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableSystemAccountWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableSystemAccountWidget->item(i, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableSystemAccountWidget->item(i, 5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            if (data["DllList"][i]["Color"].isString()) {
                setRowContentColor(QString::fromStdString(data["DllList"][i]["Color"].asString()), ui->tableSystemAccountWidget, i);
            }
        }

        if (auto_roll)
            ui->tableSystemAccountWidget->scrollToBottom();

        show();
    }
}

void SysemAccountForms::createMenu(const QPoint &pos)
{
    QTableWidget *table_widget = (QTableWidget *)sender();

    QTableWidgetItem *current_item = table_widget->itemAt(pos);
    if (current_item == NULL)
        return;

    QMenu *pop_menu = new QMenu(this);
    QAction *dll_file = new QAction("打开文件所在位置", this);
    dll_file->setObjectName("dll_file");
    dll_file->setIcon(QIcon(":/image/dir_path.png"));
    pop_menu->addAction(dll_file);
    connect(dll_file, SIGNAL(triggered(bool)), this, SLOT(openFileDir()));
    pop_menu->exec(QCursor::pos());
}

void SysemAccountForms::openFileDir()
{
    QAction *action = (QAction *)sender();
    QString name = action->objectName();

    do {
        QString file_path;
        if (name == "dll_file") {
            int current_row = ui->tableSystemAccountWidget->currentRow();
            if (current_row < 0 || current_row >= ui->tableSystemAccountWidget->rowCount())
                break;

            file_path = ui->tableSystemAccountWidget->item(current_row, 3)->text();
        } else {
            break;
        }

        if (file_path.isEmpty())
            break;

        openFileDirFromFullPath(file_path);
    } while (0);
}

void SysemAccountForms::on_actSystemAccount_pushButton_clicked()
{

    Json::Value req;

        req["msg"] = "SystemAccountScan";
    WebServiceClientWorker::Instance()->SendMsgToServer(QString::fromStdString(req.toStyledString()));


}
