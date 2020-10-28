#include "filelogformssetting.h"
#include "ui_filelogformssetting.h"
#include <QHeaderView>
#include <QGraphicsEffect>
#include <QMessageBox>
#include <QGroupBox>
#include <QFile>
#include <QSettings>
#include <QDebug>



#define KEYWORDTABLEROWHEIGHT 40

FileLogFormsSetting::FileLogFormsSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileLogFormsSetting)
{
    ui->setupUi(this);
    initForms();
    initStyle();
    applyLocalPolicy();
}

int FileLogFormsSetting::savePolicy()
{
    regular_list_.clear();

    if (ui->radioButtonAllParition->isChecked()) {
        scan_type_ = 1;
        custom_path_list_.clear();
        custom_path_list_.push_back("/home");

    } else if (ui->radioButtonOther->isChecked()) {
        scan_type_ = 4;
        custom_path_list_.push_back("/");
    }

    QList<QCheckBox *> btns = ui->groupBoxOffice->findChildren<QCheckBox *>();
    foreach (auto btn, btns) {
        if (btn->isChecked()) {
            regular_list_.append(btn->text());
        }
    }

    btns = ui->groupBoxWeb->findChildren<QCheckBox *>();
    foreach (auto btn, btns) {
        if (btn->isChecked()) {
            regular_list_.append(btn->text());
        }
    }

    btns = ui->groupBoxCompress->findChildren<QCheckBox *>();
    foreach (auto btn, btns) {
        if (btn->isChecked()) {
            regular_list_.append(btn->text());
        }
    }

    btns = ui->groupBoxphoto->findChildren<QCheckBox *>();
    foreach (auto btn, btns) {
        if (btn->isChecked()) {
            regular_list_.append(btn->text());
        }
    }

    if (keyword_list_.isEmpty()) {
        //TipMessageBox box(this);
        //box.SetTipInfo("请添加至少一个关键词！");
        //box.exec();
        QMessageBox::information(NULL, "警告", "请添加至少一个关键词！");
        return -1;
    }

    if (regular_list_.isEmpty()) {
        //TipMessageBox box(this);
        //box.SetTipInfo("请添加至少一个扫描文件类型！");
        QMessageBox::information(NULL, "警告", "请添加至少一个扫描文件类型！");
       // box.exec();
        return -1;
    }

    if (scan_type_ == 4 && !(ui->checkBoxDesktop->isChecked() ||
        ui->checkBoxMyDocument->isChecked() || !ui->lineEditCheckPath->text().isEmpty())) {
        QMessageBox::information(NULL, "警告", "请添加至少一个扫描的路径！");
        //box.exec();
        return -1;
    }


    if (scan_type_ == 4) {
        QStringList tmp_list = ui->lineEditCheckPath->text().split(';');
        //qDebug()<<ui->lineEditCheckPath->text();
        custom_path_list_.clear();
        //qDebug()<<tmp_list.size();
        for (int i = 0; i < tmp_list.size(); i++) {
            QString tmp_str = tmp_list.at(i);
            if (tmp_str.isEmpty()){
                continue;
            }

            custom_path_list_.push_back(tmp_str);
            //if(i != tmp_list.size()-1){
                //custom_path_list_.push_back(",");
            //}

/*
            if (tmp_str.contains(':')) {
                if (!custom_path_list_.contains(tmp_str))
                    custom_path_list_.push_back(tmp_str);
            } else {
                int size = custom_path_list_.size();
                //qDebug()<<size;
                if (size > 0) {
                    tmp_str = QString("%1;%2").arg(custom_path_list_.at(size - 1)).arg(tmp_str);
                    custom_path_list_.pop_back();
                    custom_path_list_.append(tmp_str);
                }
            }
            */
        }
    }

    if (ui->checkBoxMyDocument->isChecked())
        custom_path_list_.append("/home");

    if (ui->checkBoxDesktop->isChecked() )
        custom_path_list_.append("Desktop");
//qDebug()<<custom_path_list_;
    QString config_path = QString("%1%2").arg(QApplication::applicationDirPath()).arg("/policyconfig.ini");
    QFile file(config_path);
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.close();
    }

    QSettings *policy_ini = new QSettings(config_path, QSettings::IniFormat);
    policy_ini->setValue("/policy/keyword", keyword_list_.join("|"));
    policy_ini->setValue("/policy/regular", regular_list_.join("|"));
    policy_ini->setValue("/policy/type", scan_type_);
    policy_ini->setValue("/policy/path", custom_path_list_.join("|"));
    //qDebug()<<custom_path_list_;
    delete policy_ini;

    return 0;

}

void FileLogFormsSetting::initStyle()
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    setWindowFlags(Qt::FramelessWindowHint);
    QPalette pal(ui->widget->palette());

    //设置背景
    pal.setColor(QPalette::Background,QColor(192,192,192) );
    ui->widget_2->setAutoFillBackground(true);
    ui->widget_2->setPalette(pal);

    pal.setColor(QPalette::Background,QColor(185,211,238) );
    ui->widget_3->setAutoFillBackground(true);
    ui->widget_3->setPalette(pal);


    ui->widget_4->setAutoFillBackground(true);
    ui->widget_4->setPalette(pal);
    ui->widget_5->setAutoFillBackground(true);
    ui->widget_5->setPalette(pal);
    ui->widget_6->setAutoFillBackground(true);
    ui->widget_6->setPalette(pal);
    ui->widget_7->setAutoFillBackground(true);
    ui->widget_7->setPalette(pal);


    ui->stackedWidget->setFrameShape(QFrame::NoFrame);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影颜色
    shadow->setColor(QColor("#444444"));
    //设置阴影圆角
    shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    ui->widget_3->setGraphicsEffect(shadow);
    ui->widget_2->setGraphicsEffect(shadow);
    ui->tableWidgetKeyWord->setRowCount(500);

    ui->btnSettingClose->setIconSize(QSize(32,32));
    ui->btnSettingClose_2->setIconSize(QSize(32,32));
    //ui->btnSettingClose->setIcon(QIcon(":/images/Setting.png"));
    //ui->btnSettingClose->setText(QString::fromLocal8Bit("close"));
/*
                 ("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2    px groove gray;border-style: outset;}"

                  "QPushButton:hover{background-color:white; color: black;}"

                  "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
*/
    ui->btnAddPath->setStyleSheet
            ("QPushButton{color:#000000;background-color:#F0F0F0;   border-radius: 10px;  border: 2    px groove gray;border-style: outset;}"
             "QPushButton:hover{color:#ffffff;background-color:#FFAF60;}"
             "QPushButton:pressed{color:#ffffff;background-color:#FF8F59;border-style: inset; }");
    ui->btnSettingClose->setStyleSheet
            ("QPushButton{color:#000000;background-color:#F0F0F0;   border-radius: 10px;  border: 2    px groove gray;border-style: outset;}"
             "QPushButton:hover{color:#ffffff;background-color:#FFAF60;}"
             "QPushButton:pressed{color:#ffffff;background-color:#FF8F59;border-style: inset; }");
    ui->btnSettingClose_2->setStyleSheet
            ("QPushButton{color:#000000;background-color:#F0F0F0;   border-radius: 10px;  border: 2    px groove gray;border-style: outset;}"
             "QPushButton:hover{color:#ffffff;background-color:#FFAF60;}"
             "QPushButton:pressed{color:#ffffff;background-color:#FF8F59;border-style: inset; }");
    ui->btnAddkeyWord->setStyleSheet
            ("QPushButton{color:#000000;background-color:#F0F0F0;   border-radius: 10px;  border: 2    px groove gray;border-style: outset;}"
             "QPushButton:hover{color:#ffffff;background-color:#FFAF60;}"
             "QPushButton:pressed{color:#ffffff;background-color:#FF8F59;border-style: inset; }");



    ui->btnBasic->setStyleSheet
            ("QToolButton{color:#000000;background-color:#F0F0F0;   border-radius: 10px;  }"
             "QToolButton:hover{color:#ffffff;background-color:#FFAF60;}"
             "QToolButton:pressed{color:#ffffff;background-color:#FF8F59;border-style: inset; }");
    ui->btnSenior->setStyleSheet
            ("QToolButton{color:#000000;background-color:#F0F0F0;   border-radius: 10px;  }"
             "QToolButton:hover{color:#ffffff;background-color:#FFAF60;}"
             "QToolButton:pressed{color:#ffffff;background-color:#FF8F59;border-style: inset; }");


    //ui->scrollArea_2->setFrameShape(QFrame::NoFrame);
    ui->lineEditKeyWord->setPlaceholderText("请输入关键词");
    ui->lineEditTaskName->setPlaceholderText("测试检查任务");
    //ui->btnSettingClose->hide();

    QHeaderView *verticalHeader = ui->tableWidgetKeyWord->verticalHeader();
    verticalHeader->setHidden(true);    //隐藏序列号统计号
    QHeaderView *horizontalheadr = ui->tableWidgetKeyWord->horizontalHeader();
    horizontalheadr->setHidden(true);        //隐藏表头
    ui->tableWidgetKeyWord->setAlternatingRowColors(false);
    ui->tableWidgetKeyWord->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetKeyWord->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidgetKeyWord->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetKeyWord->setFocusPolicy(Qt::NoFocus);
    ui->tableWidgetKeyWord->setShowGrid(false);
    ui->tableWidgetKeyWord->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetKeyWord->verticalHeader()->setDefaultSectionSize(40);
    ui->tableWidgetKeyWord->setColumnCount(2);
    ui->tableWidgetKeyWord->setColumnWidth(0, 380);
    ui->tableWidgetKeyWord->setFixedHeight(KEYWORDTABLEROWHEIGHT * 6);
    ui->checkBoxDesktop->hide();


    connect(ui->btnAddkeyWord, SIGNAL(clicked(bool)), this, SLOT(btnAddKeyWordClick()));
    connect(ui->btnSettingClose, SIGNAL(clicked(bool)), this, SLOT(btnCloseClick()));

    connect(ui->radioButtonAllParition, SIGNAL(clicked(bool)), this, SLOT(radioButtonScanTypeClick()));
    connect(ui->radioButtonOther, SIGNAL(clicked(bool)), this, SLOT(radioButtonScanTypeClick()));

    connect(ui->checkBoxAllCompress, SIGNAL(clicked(bool)), this, SLOT(checkAllBtnClick()));
    connect(ui->checkBoxAllWeb, SIGNAL(clicked(bool)), this, SLOT(checkAllBtnClick()));
    connect(ui->checkBoxAllOffice, SIGNAL(clicked(bool)), this, SLOT(checkAllBtnClick()));
    connect(ui->checkBoxAllPhoto, SIGNAL(clicked(bool)), this, SLOT(checkAllBtnClick()));

    ui->checkBoxMyDocument->hide();
    //ui->radioButtonOther->setChecked(true);
    //ui->radioButtonOther->hide();
    if(keyword_list_.size()==0)
    {
        keyword_list_.append("秘密");
        keyword_list_.append("机密");
        keyword_list_.append("绝密");
    }

    refreshKeyword();
    connect(ui->btnAddPath, SIGNAL(clicked(bool)), this, SLOT(btnAddPathClick()));

    //ui->widgetCostomCheckPath->show();
    ui->widgetCostomCheckPath->hide();

}

void FileLogFormsSetting::btnAddPathClick()
{

    customCheckPath();
}

void FileLogFormsSetting::btnCloseClick()
{

    if (savePolicy() == 0) {
        POLICYCONFIG policy_config;
        policy_config.type = scan_type_;
        policy_config.keyword_list = keyword_list_;
        policy_config.regular_list = regular_list_;
        policy_config.custom_path_list = custom_path_list_;


        QVariant config;
        config.setValue(policy_config);
        emit policyConfig(config);
        //break;
        close();
    }

}
void FileLogFormsSetting::navigatorBtnClick()
{
    QToolButton *btn = (QToolButton*)sender();
    QString name = btn->objectName();
    if (name == "btnSenior") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "btnBasic") {
        ui->stackedWidget->setCurrentIndex(0);
    }
}
void FileLogFormsSetting::checkAllBtnClick()
{
    QCheckBox *btn = (QCheckBox*)sender();
    QString name = btn->objectName();
    bool bnt_pressed = btn->isChecked();

    QGroupBox *box = nullptr;

    if (name == "checkBoxAllOffice") {
        box = ui->groupBoxOffice;
    } else if (name == "checkBoxAllWeb") {
        box = ui->groupBoxWeb;
    } else if (name == "checkBoxAllCompress") {
        box = ui->groupBoxCompress;
    } else if (name == "checkBoxAllPhoto") {
        box = ui->groupBoxphoto;
    }

    if (box) {
        QList<QCheckBox *> btns = box->findChildren<QCheckBox *>();
        foreach (btn, btns) {
            btn->setChecked(bnt_pressed);
        }
    }
}
void FileLogFormsSetting::radioButtonScanTypeClick()
{
    QRadioButton *btn = (QRadioButton *)sender();
    QString name = btn->objectName();

    if (name == "radioButtonOther") {
        ui->widgetCostomCheckPath->show();

    } else {
        ui->widgetCostomCheckPath->hide();

    }

}
int FileLogFormsSetting::customCheckPath(QString path)
{
    int ret = 0;

    do {
        if (path.isEmpty()) {
            QFileDialog dialog(this);
            dialog.setWindowTitle("选择自查目录");
            dialog.setFileMode(QFileDialog::DirectoryOnly);
            QStringList fileName;
            if(!dialog.exec()) {
                ret = -1;
                break;
            }

            QStringList file_name_list;
            file_name_list = dialog.selectedFiles();
            QString dir_path = file_name_list.at(0).toLocal8Bit().constData();
            path = dir_path;
        } else {
            QDir dir(path);
            if (!dir.exists()) {
                ret = -1;
                break;
            }
        }

        path.replace(QString("/"), QString("\/"));

        if (custom_path_list_.contains(path)) {
            //qDebug() << "have same path--->" << path;
        } else {
            custom_path_list_.push_back(path);
            QString tmp_str = ui->lineEditCheckPath->text();
            if (!tmp_str.isEmpty())
                tmp_str = QString("%1%2;").arg(tmp_str).arg(path);
            else
                tmp_str = QString("%1;").arg(path);
            ui->lineEditCheckPath->setText(tmp_str);
            ui->lineEditCheckPath->setToolTip(tmp_str);
        }
    } while (0);

    return ret;
}

void FileLogFormsSetting::refreshRegular()
{
    if (regular_list_.size() > 0) {
        QList<QCheckBox *> btns = ui->groupBoxOffice->findChildren<QCheckBox *>();
        foreach (auto btn, btns) {
            btn->setChecked(false);
        }

        btns = ui->groupBoxWeb->findChildren<QCheckBox *>();
        foreach (auto btn, btns) {
            btn->setChecked(false);
        }

        btns = ui->groupBoxCompress->findChildren<QCheckBox *>();
        foreach (auto btn, btns) {
            btn->setChecked(false);
        }

        btns = ui->groupBoxphoto->findChildren<QCheckBox *>();
        foreach (auto btn, btns) {
            btn->setChecked(false);
        }

        for (int i = 0; i < regular_list_.size(); i++) {
            QString regular = regular_list_.at(i);
            if (regular == ".doc") {
                ui->checkBoxDoc->setChecked(true);
            } else if (regular.compare(".docx", Qt::CaseInsensitive) == 0) {
                ui->checkBoxDocx->setChecked(true);
            } else if (regular.compare(".xls", Qt::CaseInsensitive) == 0) {
                ui->checkBoxXls->setChecked(true);
            } else if (regular.compare(".xlsx", Qt::CaseInsensitive) == 0) {
                ui->checkBoxXlsx->setChecked(true);
            } else if (regular.compare(".ppt", Qt::CaseInsensitive) == 0) {
                ui->checkBoxPpt->setChecked(true);
            } else if (regular.compare(".pptx", Qt::CaseInsensitive) == 0) {
                ui->checkBoxPptx->setChecked(true);
            } else if (regular.compare(".pps", Qt::CaseInsensitive) == 0) {
                ui->checkBoxPps->setChecked(true);
            } else if (regular.compare(".wps", Qt::CaseInsensitive) == 0) {
                ui->checkBoxWps->setChecked(true);
            } else if (regular.compare(".et", Qt::CaseInsensitive) == 0) {
                ui->checkBoxEt->setChecked(true);
            } else if (regular.compare(".dps", Qt::CaseInsensitive) == 0) {
                ui->checkBoxDps->setChecked(true);
            } else if (regular.compare(".pdf", Qt::CaseInsensitive) == 0) {
                ui->checkBoxPdf->setChecked(true);
            } else if (regular.compare(".rtf", Qt::CaseInsensitive) == 0) {
                ui->checkBoxRtf->setChecked(true);
            } else if (regular.compare(".txt", Qt::CaseInsensitive) == 0) {
                ui->checkBoxTxt->setChecked(true);
            } else if (regular.compare(".htm", Qt::CaseInsensitive) == 0) {
                ui->checkBoxHtm->setChecked(true);
            } else if (regular.compare(".html", Qt::CaseInsensitive) == 0) {
                ui->checkBoxHtml->setChecked(true);
            } else if (regular.compare(".mht", Qt::CaseInsensitive) == 0) {
                ui->checkBoxMht->setChecked(true);
            } else if (regular.compare(".xml", Qt::CaseInsensitive) == 0) {
                ui->checkBoxXml->setChecked(true);
            }  else if (regular.compare(".rar", Qt::CaseInsensitive) == 0) {
                ui->checkBoxRar->setChecked(true);
            } else if (regular.compare(".zip", Qt::CaseInsensitive) == 0) {
                ui->checkBoxZip->setChecked(true);
            } else if (regular.compare(".7z", Qt::CaseInsensitive) == 0) {
                ui->checkBox7z->setChecked(true);
            } else if (regular.compare(".iso", Qt::CaseInsensitive) == 0) {
                ui->checkBoxIso->setChecked(true);
            } else if (regular.compare(".bz2", Qt::CaseInsensitive) == 0) {
                ui->checkBoxBz2->setChecked(true);
            } else if (regular.compare(".rar5", Qt::CaseInsensitive) == 0) {
                ui->checkBoxRar5->setChecked(true);
            } else if (regular.compare(".tar", Qt::CaseInsensitive) == 0) {
                ui->checkBoxTar->setChecked(true);
            } else if (regular.compare(".bmp", Qt::CaseInsensitive) == 0) {
                ui->checkBoxBmp->setChecked(true);
            } else if (regular.compare(".jpg", Qt::CaseInsensitive) == 0) {
                ui->checkBoxJpg->setChecked(true);
            } else if (regular.compare(".jpeg", Qt::CaseInsensitive) == 0) {
                ui->checkBoxJpeg->setChecked(true);
            } else if (regular.compare(".png", Qt::CaseInsensitive) == 0) {
                ui->checkBoxPng->setChecked(true);
            } else if (regular.compare(".dib", Qt::CaseInsensitive) == 0) {
                ui->checkBoxDib->setChecked(true);
            } else if (regular.compare(".gif", Qt::CaseInsensitive) == 0) {
                ui->checkBoxGif->setChecked(true);
            } else if (regular.compare(".tif", Qt::CaseInsensitive) == 0) {
                ui->checkBoxTif->setChecked(true);
            } else if (regular.compare(".tiff", Qt::CaseInsensitive) == 0) {
                ui->checkBoxTiff->setChecked(true);
            }
        }
    }
}


void FileLogFormsSetting::applyLocalPolicy()
{
    QString config_path = QString("%1%2").arg(QApplication::applicationDirPath()).arg("/policyconfig.ini");
    QFileInfo file(config_path);
    if (file.exists()) {
        QSettings *policy_ini = new QSettings(config_path, QSettings::IniFormat);
        QString keyword = policy_ini->value("/policy/keyword").toString();
        QString regular = policy_ini->value("/policy/regular").toString();
        int type = policy_ini->value("/policy/type").toInt();
        QString path = policy_ini->value("/policy/path").toString();

        if (type == 1) {
            ui->radioButtonAllParition->setChecked(true);
            ui->widgetCostomCheckPath->hide();
            ui->checkBoxDesktop->setChecked(false);
        } else if (type == 4 && !path.isEmpty()) {
            QStringList path_list = path.split('|');
            for (int i = 0; i < path_list.size(); i++) {
                customCheckPath(path_list.at(i));
            }

            ui->checkBoxDesktop->setChecked(false);

            if (path_list.contains("Desktop"))
                ui->checkBoxDesktop->setChecked(true);

            if (path_list.contains("MyDocument"))
                ui->checkBoxMyDocument->setChecked(true);

            ui->widgetCostomCheckPath->show();
            ui->radioButtonOther->setChecked(true);
        }

        if (!ui->checkBoxMyDocument->isChecked() && !ui->checkBoxDesktop->isChecked() &&
                custom_path_list_.size() == 0)
            ui->radioButtonAllParition->setChecked(true);

        if (!keyword.isEmpty() && !regular.isEmpty()) {
            keyword_list_ = keyword.split('|');
            regular_list_ = regular.split('|');

            refreshRegular();
            refreshKeyword();
        }
        delete policy_ini;
    }
}

void FileLogFormsSetting::btnAddKeyWordClick()
{
    QString keyword = ui->lineEditKeyWord->text();
    QString tip_info;

    do {
        if (keyword.isEmpty()) {
            QMessageBox::information(NULL, "警告", "关键词为空!");
            break;
        }

        if (keyword_list_.contains(keyword)) {
            QMessageBox::information(NULL, "警告", "该关键词已经存在!");
            break;
        }

        keyword_list_.append(keyword);
        refreshKeyword();
    } while (0);

    ui->lineEditKeyWord->clear();
}
void FileLogFormsSetting::initForms()
{
    connect(ui->btnSenior, SIGNAL(clicked(bool)), this, SLOT(navigatorBtnClick()));
    connect(ui->btnBasic, SIGNAL(clicked(bool)), this, SLOT(navigatorBtnClick()));





}
void FileLogFormsSetting::refreshKeyword()
{
    ui->tableWidgetKeyWord->clearContents();
    int row_size = keyword_list_.size();
    ui->tableWidgetKeyWord->setRowCount(row_size);

    if (row_size > 5) {
        ui->tableWidgetKeyWord->setFixedHeight(KEYWORDTABLEROWHEIGHT * row_size);
    }

    for (int i = 0; i < row_size; i++) {
        ui->tableWidgetKeyWord->setItem(i, 0, new QTableWidgetItem(
        keyword_list_.at(i)));
        QPushButton *btn = new QPushButton;
        btn->setToolTip(QString::number(i));
        btn->setText(" 删除");
        btn->setFixedSize(70, 35);
        btn->setObjectName("btnSubkeyWord");
        btn->setIcon(QIcon(":/image/trash.png"));
        btn->setIconSize(QSize(20, 20));
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(btnSubKeyWordClick()));
        QWidget *widget = new QWidget();
        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->addWidget(btn);
        hlayout->setAlignment(widget, Qt::AlignCenter);
        hlayout->setContentsMargins(0, 0, 20, 0);
        widget->setLayout(hlayout);
        ui->tableWidgetKeyWord->setCellWidget(i, 1, widget);
    }
}
void FileLogFormsSetting::btnSubKeyWordClick()
{
    QPushButton *btn = (QPushButton *)sender();
    int row = btn->toolTip().toInt();

    if (row >= 0 || row < keyword_list_.size()) {
        keyword_list_.removeAt(row);
        refreshKeyword();
    }
}
FileLogFormsSetting::~FileLogFormsSetting()
{
    delete ui;
}

void FileLogFormsSetting::on_btnSettingClose_2_clicked()
{
    this->close();
}
