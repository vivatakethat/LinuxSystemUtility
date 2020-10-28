#include "filelogforms.h"
#include "ui_filelogforms.h"

#include "filelogformssetting.h"
#include <sys/socket.h>
#include <unistd.h>

#include <QTableWidgetItem>
#include <QTextCodec>
#include <QLabel>
#include <QKeyEvent>

#include "json/json.h"

#include "webserviceclientworker.h"

#include <QUrl>
#include <QDesktopServices>
#include <QToolButton>
#include "utils.h"
#include <QDebug>
#include <QDebug>
#include <QDateTime>
using namespace std;
FileLogForms::FileLogForms(QWidget *parent) :
    QDialog(parent),
    timer_(nullptr),
    current_row_(0),
    horse_scan_table_rows_(0),
    file_num(0),
    start_scan_(false),
    ui(new Ui::FileLogForms)
{
    ui->setupUi(this);
    initForms();
    setPolicy();
}

void FileLogForms::insertDataToHorseScan(const QString &data, const QString &content_color ,int ops)
{
    if (data.isEmpty())
        return;//take_cancel;

    if (ui->tableWidget->rowCount() == horse_scan_table_rows_) {
        ui->tableWidget->setRowCount(horse_scan_table_rows_ + 2);
    }

 //ui->tableWidgetHorseScan->setItem(2, 2, new QTableWidgetItem(data));//take:debug
//  QString t_Data="take";

     switch(ops)
    {
     case 0:
   ui->tableWidget->setItem(horse_scan_table_rows_, ops, new QTableWidgetItem(data));
break;
     case 1:
         if(data.isEmpty()){}
            //ui->tableWidget->setItem(horse_scan_table_rows_, ops, new QTableWidgetItem("NULL"));
            //ui->tableWidget->setItem(horse_scan_table_rows_, ops, new QTableWidgetItem(data));
         break;
     default:
         break;

}

    if (content_color == "1") {
        //ui->tableWidget->item(horse_scan_table_rows_, 0)->setForeground(Qt::blue);
    } else if (content_color == "2") {
        //QBrush brush;
        //brush.setColor(QColor(238, 118, 0, 255));
        //ui->tableWidget->item(horse_scan_table_rows_, 0)->setForeground(brush);
    } else if (content_color == "3") {
        //ui->tableWidget->item(horse_scan_table_rows_, 0)->setForeground(Qt::red);
    }

    if (auto_roll)
        ui->tableWidget->scrollToBottom();






}

void FileLogForms::getMessageString(std::string message)
{
    ui->tableWidget->setItem(horse_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(message)));
}

void FileLogForms::setFile_numResult(int num)
{
    file_num=num;
}
void FileLogForms::HandLeServerMessage(const Json::Value &value){
    std::cout<<value.size()<<endl;
    for(int i=0 ;i< value.size();i++){
        file_num++;
        ui->tableWidget->setItem(horse_scan_table_rows_, 0, new QTableWidgetItem(QString::fromStdString(value[i]["file"].asString())));
        if(value[i]["ftype"].asInt() == 0){//dan
            filePath.push_back(QString::fromStdString(value[i]["file"].asString()));
            //insertDataToHorseScan(QString::fromStdString(value[i]["res"]["found"].asString()),QString::fromStdString("0"),1);
            ui->tableWidget->setItem(horse_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(value[i]["res"]["found"].asString())));
            secretNum.push_back(QString::fromStdString(value[i]["res"]["found"].asString()));
            std::string keyWordBuffer=value[i]["res"]["info"][0]["keyword"].asString();
            for(int j = 1;j<value[i]["res"]["info"].size();j++){
                keyWordBuffer+=" | ";
                keyWordBuffer+=value[i]["res"]["info"][j]["keyword"].asString();
            }
            //insertDataToHorseScan(QString::fromStdString(keyWordBuffer),QString::fromStdString("0"),2);
            ui->tableWidget->setItem(horse_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(keyWordBuffer)));
            std::string detailsBuffer=value[i]["res"]["info"][0]["details"].asString();
            for(int j = 1;j<value[i]["res"]["info"].size();j++){
                detailsBuffer+=" | ";
                detailsBuffer+=value[i]["res"]["info"][j]["details"].asString();
            }
            //insertDataToHorseScan(QString::fromStdString(detailsBuffer),QString::fromStdString("0"),3);
            ui->tableWidget->setItem(horse_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(detailsBuffer)));
            classifiedContent.push_back(QString::fromStdString(detailsBuffer));
        }else {
            filePath.push_back(QString::fromStdString(value[i]["file"].asString()));
            int num=0;
            for(int j=0;j<value[i]["res"].size();j++)
            {
                num+=value[i]["res"][j]["found"].asInt();
            }
            std::string numBuffer=to_string(num);
            //insertDataToHorseScan(QString::fromStdString(numBuffer),QString::fromStdString("0"),1);
            ui->tableWidget->setItem(horse_scan_table_rows_, 1, new QTableWidgetItem(QString::fromStdString(numBuffer)));
            secretNum.push_back(QString::fromStdString(numBuffer));

            std::string keyWordBuffer=value[i]["res"][0]["res"]["info"][0]["keyword"].asString();

            for(int j=0;j<value[i]["res"].size();j++)
            {
                for(int k=0;k<value[i]["res"][j]["res"]["info"].size();k++){
                    keyWordBuffer+=" | ";
                    keyWordBuffer+=value[i]["res"][j]["res"]["info"][k]["keyword"].asString();
                }
            }
            //insertDataToHorseScan(QString::fromStdString(keyWordBuffer),QString::fromStdString("1"),2);
            ui->tableWidget->setItem(horse_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(keyWordBuffer)));
            std::string detailsBuffer=value[i]["res"][0]["res"]["info"][0]["details"].asString();
            for(int j=0;j<value[i]["res"].size();j++)
            {
                for(int k=0;k<value[i]["res"][j]["res"]["info"].size();k++){
                    detailsBuffer+="|";
                    detailsBuffer+=value[i]["res"][j]["res"]["info"][k]["details"].asString();
                }
            }
            //insertDataToHorseScan(QString::fromStdString(detailsBuffer),QString::fromStdString("0"),3);
            ui->tableWidget->setItem(horse_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(detailsBuffer)));
            classifiedContent.push_back(QString::fromStdString(detailsBuffer));
        }

        horse_scan_table_rows_++;


    }
}

FileLogForms::~FileLogForms()
{
    delete ui;
}


void FileLogForms::tableCellDoubleClicked(int row, int column)
{
    if (row < 0 || row >= current_row_ || row >= ui->tableWidget->rowCount())
        return;

    QString url = ui->tableWidget->item(row, 0)->text();
    QString file_path = QString(" /select,%1").arg(url);
    //ShellExecuteA(0, "open", "explorer.exe", utf8_to_gbk(file_path.toStdString()).c_str(), NULL, true);
}

void FileLogForms::createTableMenu(const QPoint &pos)
{
    QTableWidgetItem *curItem = ui->tableWidget->itemAt(pos);
    if (curItem == NULL)
        return;

    QMenu *popMenu = new QMenu(this);
    popMenu->setObjectName("table_menu");
    QAction *openPathSeed = new QAction("打开文件路径", this);
    openPathSeed->setIcon(QIcon(":/image/file_dir.png"));
    popMenu->addAction(openPathSeed);
    connect(openPathSeed, SIGNAL(triggered(bool)), this, SLOT(actionOpenDirTrigger(bool)));
    popMenu->exec(QCursor::pos());
}

void FileLogForms::actionOpenDirTrigger(bool)
{
    int row = ui->tableWidget->currentRow();

    tableCellDoubleClicked(row, 0);
}

void FileLogForms::insertData()
{
    ui->tableWidget->setRowCount(20);

    for (int i = 0; i < 5; i++) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem("G:\\git.repo\\code\\vm3in1\\patch\\0001-patched.patch"));
        ui->tableWidget->item(i, 0)->setToolTip("c:\\qt5-x86-static-release\\lib\\fonts");
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem("80"));
        ui->tableWidget->item(i, 1)->setForeground(Qt::red);
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem("秘密 | 机密"));
        ui->tableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidget->item(current_row_, 1)->setForeground(Qt::red);
        ui->tableWidget->item(current_row_, 2)->setForeground(Qt::red);

        QLabel *label = new QLabel;
        label->setText("因压缩包文件内容太多，因返<font color=\"red\">机密</font>回包含关键词<font color=\"red\">秘密</font>内查不到关键词,则返回一个空的json数组, 返回内容形如");
        label->setToolTip("因压缩包文件内容太多，因返<font color=\"red\">机密</font>回包含关键词<font color=\"red\">秘密</font>内查不到关键词,则返回一个空的json数组, 返回内容形如");
        label->setFont(QFont("Microsoft Yahei", 11));
        QWidget *widget = new QWidget();
        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->addWidget(label);
        hlayout->setAlignment(widget, Qt::AlignCenter);
        hlayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(hlayout);
        ui->tableWidget->setCellWidget(i, 3, widget);
    }
}


void FileLogForms::initForms()
{

    default_regular_ = ".doc|.docx|.xls|.xlsx|.ppt|.pps|.wps|.et|.dps|.pdf|.rtf|.txt|.htm";
    default_keyword_ = "秘密|机密|绝密";
    default_scan_type_ = 4;
    custom_path_list_.push_back("Desktop");


    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableCellDoubleClicked(int,int)), Qt::DirectConnection);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createTableMenu(QPoint)), Qt::DirectConnection);



    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(displayUseTime()));
    //ui->lcdNumber->display("00:00");
    //ui->progressBar->setValue(0);

    ui->tableWidget->setRowCount(1000);

    //message_box_ = new TipMessageBox(this);
    connect(ui->btnStart, SIGNAL(fileScanRes(QVariant)), this, SLOT(displayAlarmFileInfo(QVariant)));

    connect(ui->btnStart, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnSetting, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnReportForm, SIGNAL(clicked(bool)), this, SLOT(btnClick()));
    connect(ui->btnReportForm, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnStart, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    connect(ui->btnSetting, SIGNAL(toggled(bool)), this, SLOT(btnToggle()));
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width()-this->width())/2, (desktop->height()-this->height())/2);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QColor(255,255,255)));
    ui->widgetShadow->setPalette(palette);
    ui->widgetShadow->setAutoFillBackground(true);
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect();
    shadow_effect->setBlurRadius(10.0);
    shadow_effect->setColor(QColor(0, 0, 0, 150));
    shadow_effect->setOffset(0.5);
    ui->widgetShadow->setGraphicsEffect(shadow_effect);
    //ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setAlternatingRowColors(false);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);
    ui->tableWidget->horizontalHeader()->setFixedHeight(36);

    ui->tableWidget->setColumnWidth(0, 280);
    ui->tableWidget->setColumnWidth(1, 80);
    ui->tableWidget->setColumnWidth(2, 140);

    showDialogOnTopLevel(0);


    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setStyleSheet(
    "QTableWidget::Item{border:0px solid rgb(0,0,0);"
    "border-bottom:1px solid rgb(0,0,0);}"
    );
    //ui->tableWidget->
    //ui->widgetShadow->setFixedSize(831,571);
    //ui->tableWidget->setFixedSize(821,441);



}

void FileLogForms::btnToggle()
{
    QToolButton* btn=(QToolButton*)sender();
    QString name = btn->objectName();
    QIcon icon;
    if(!btn->isChecked()){
        if(name == "btnSetting"){
            icon.addFile(":/image/shadowsst_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnReportForm"){
            icon.addFile(":/image/reportform_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        } else if (name == "btnStart"){
            icon.addFile(":/image/result_detect_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        }
    }
    btn->setIcon(icon);
    //btn->setIconSize(QSize(70,70));

}

void FileLogForms::btnClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString name = btn->objectName();

    QIcon icon;
    if (name == "btnSetting") {
        icon.addFile(":/image/dirver_check.png", QSize(), QIcon::Normal, QIcon::Off);
        if (!start_scan_) {
            FileLogFormsSetting setting_windows;
            setting_windows.exec();
            keyword_list_ = setting_windows.keyword_list();
            regular_list_ = setting_windows.regular_list();
            custom_path_list_ = setting_windows.custom_path_list();
            scan_type_ = setting_windows.scan_type();
        } else {
            FileLogFormsSetting *setting_windows = new FileLogFormsSetting;
            connect(setting_windows, SIGNAL(policyConfig(QVariant)), this, SLOT(getPolicyConfig(QVariant)));
            setting_windows->show();
        }
    } else if (name == "btnReportForm") {
        GetCheckFileInfo checkfileinfotmp ;
        checkfileinfotmp.exec();
        //QDesktopServices :: openUrl(QUrl(QLatin1String("www.baidu.com")));
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zz ddd");

        QString localHostName = QHostInfo::localHostName();
        QHostInfo Info = QHostInfo::fromName(localHostName);
        std::string address="";
        for(int i=0;i<Info.addresses().size();i++)
        {
            address+=(Info.addresses().at(i).toString().toStdString());
        }
        address+="(";
        address+=localHostName.toStdString()+=")";
        std::string applicationPath =QApplication::applicationDirPath().toStdString();
        applicationPath+="/result.html";
        //std::cout<<applicationPath<<"!!!!!!!!!!!!"<<endl;
        GetCheckFileInfo checkFileInfoTmp;
        setHtmlHeadString(applicationPath,checkFileInfoTmp.getCompanyName().toStdString(),
                          current_date.toStdString(),checkFileInfoTmp.getCheckManName().toStdString(),address);
        qDebug()<<applicationPath.c_str();
        for(int i =0;i<keyword_list_.size();i++){
            setSearchKeywordString(applicationPath,keyword_list_.at(i).toStdString());
        }

        for(int i=0;i<custom_path_list_.size();i++){
            setSearchDirString(applicationPath,custom_path_list_.at(i).toStdString()+=" ");
        }
        for(int i=0;i<file_num;i++){
            QFileInfo tmp(filePath.at(i));
            QDateTime lm=tmp.lastModified();
            QString modifie_time = tr("1%").arg(lm.toString("yyyy-MM-dd hh:mm:ss"));
            setSecretFileContent(i,applicationPath.c_str(),
                                 to_string((int)tmp.size()),modifie_time.toStdString(),
                                 secretNum.at(i).toInt(),classifiedContent.at(i).toStdString(),filePath.at(i).toStdString());
        }
        setSecretFileDetails(applicationPath.c_str(),to_string(file_num),"0","0",to_string(file_num));

        icon.addFile(":/image/reportform_check.png", QSize(), QIcon::Normal, QIcon::Off);
    } else if (name == "btnStart") {

        icon.addFile(":/image/result_detecte_check.png", QSize(), QIcon::Normal, QIcon::Off);
        if (!start_scan_) {
            POLICYCONFIG policy_config;
            policy_config.type = scan_type_;
            policy_config.keyword_list = keyword_list_;
            policy_config.regular_list = regular_list_;
            policy_config.custom_path_list = custom_path_list_;

            QVariant config;
            config.setValue(policy_config);

            emit policyConfig(config);

            //emit startScanWork();
            //ui->btnCancel->show();
            start_scan_ = !start_scan_;
            //ui->tableWidget->clearContents();
            switchTaskStatus(false);
            //ui->btnStart->hide();
        } else {
            //pause_ = !pause_;
            //switchTaskStatus(pause_);
        }
        QLabel labelTmp ;
        //labelTmp->

        QString buff="";
        if(regular_list_.size() > 0){
            buff+=regular_list_[0];
        }
        for(int i=1;i<regular_list_.size();i++){
            //qDebug()<<regular_list_[i];
            if(regular_list_[i] == "全选"){
                continue;
            }else {
                buff+="\n";
                buff+=regular_list_[i];
            }
        }
        QString keyWordMessage = SetKeyWordHttpMessage(keyword_list_);


        HttpMessageClient::Instance()->SendMsgToServer(keyWordMessage);
        HttpMessageClient::Instance()->SendMsgToServer(SetRegularHttpMessage(buff));
        HttpMessageClient::Instance()->SendMsgToServer(SetCustomPathHttpMessage(custom_path_list_));
        //ui->tableWidget->setRowCount(file_num);
        if(g_fileconnection != nullptr){
            ui->btnStart->setEnabled(false);
        }else{
            //QMessageBox::information(NULL,"警告","检查后台运行是否正常");
            //btn->setIcon(icon);
            //return;
        }


    }
    btn->setIcon(icon);
}
void FileLogForms::btnRenew(){
    ui->btnStart->setEnabled(true);
}
QString FileLogForms::SetRegularHttpMessage(QString RegularList)
{
    QString RegularPost = "POST ";
    QString RegularExtension = "/pwd/set/ft ";
    QString versionId = "HTTP/1.1\r\nReferer:global\r\n\r\n";
    RegularPost+=RegularExtension+=versionId+=RegularList;
    return RegularPost;
}

QString FileLogForms::SetRegularHttpMessageArchive(QString RegularList)
{
    QString RegularPost = "POST ";
    QString RegularExtension = "/pwd/set/aft ";
    QString versionId = "HTTP/1.1\r\nReferer:global\r\n\r\n";
    RegularPost+=RegularExtension+=versionId+=RegularList;
    return RegularPost;
}

QString FileLogForms::SetCustomPathHttpMessage(QStringList CustomPathList)
{
    QString CustomPathPost="POST ";
    QString CustomPath = "/pwd/set/scanfp ";
    QString versionId = "HTTP/1.1\r\nReferer:global\r\n\r\n";
    CustomPathPost+=CustomPath+=versionId;
    for(int i=0;i<CustomPathList.size();i++){
        CustomPathPost+=CustomPathList.at(i);
        //qDebug()<<RequestPost;
        if(i!=CustomPathList.size()-1){
            CustomPathPost+="\n";
        }
    }
    return CustomPathPost;
}

QString FileLogForms::SetKeyWordHttpMessage(QStringList KeyWordList)
{

    QString keyWordMessage="";
    QString RequestPost="POST ";
    QString sensitiveWord="/pwd/set/sen ";
    QString secretWord="/pwd/set/sec ";
    QString versionId="HTTP/1.1\r\n\r\n";
    QString RequestGet="GET";
    keyWordMessage+=RequestPost+=sensitiveWord+=versionId;


    Json::Value root;

    std::string keyword;
    root["count"] = KeyWordList.size();
    Json::Value jsonkey(Json::arrayValue);
        for (int i = 0 ;i < KeyWordList.size();i++)
        {
            std::string tmpkeyword = KeyWordList[i].toStdString();
            Json::Value jsonKeyword;
            jsonKeyword["keyword"] = tmpkeyword;
            jsonkey[i] = jsonKeyword;
            jsonKeyword["excludes"] = Json::Value(Json::arrayValue);
            //for (int j = 0 ;j <g_Userinfo.keywrod.key[i].filtercount;j++)
            //{
                //std::string tmpfilterkeyword = g_Userinfo.keywrod.key[i].filter[j].szfilterKeywrod;
                //jsonKeyword["excludes"].append(tmpfilterkeyword);
            //}
            //if (g_Userinfo.keywrod.key[i].filtercount == 0 )
            //{
                //jsonKeyword.removeMember("excludes");
           // }
            //else
            //{
             //   jsonkey[i] = jsonKeyword;
           // }

        }
        root["keywords"] = jsonkey;
        keyword = root.toStyledString();
        keyWordMessage+=QString::fromStdString(keyword);
        return keyWordMessage;

}
void FileLogForms::resizeEvent(QResizeEvent *event)
{
    //QSize qsize = this->size();
    //ui->tableWidget->resize(qsize.width(),qsize.height());
    //ui->widget_3->resize(qsize.width()-ui->widget_2->width(),ui->widget_1->height());
    //ui->widget_4->resize(qsize.width(),qsize.height()-ui->widget_2->height()-ui->widget_2->height());
    //ui->tableWidget->resize(qsize.width(),qsize.width());
}

void FileLogForms::insertDataToHorseScan(const Json::Value &data,int num)  //take_add:将传入的json再次解析
{
    cout<<endl<<endl<<endl;
    if(data.empty())
        return;

    ui->tableWidget->setItem(horse_scan_table_rows_, 2, new QTableWidgetItem(QString::fromStdString(data["res"]["info"][num]["keyword"].asString())));
    ui->tableWidget->setItem(horse_scan_table_rows_, 3, new QTableWidgetItem(QString::fromStdString(data["res"]["info"][num]["details"].asString())));


    // ui->tableWidget->setItem(horse_scan_table_rows_, 4, new QTableWidgetItem(QString::fromStdString(data["msg"]["serial"].asString())));
    // ui->tableWidget->setItem(horse_scan_table_rows_, 5, new QTableWidgetItem(QString::fromStdString(data["msg"]["idVendor"].asString())));

    // ui->tableWidget->setItem(horse_scan_table_rows_, 6, new QTableWidgetItem(QString::fromStdString(data["msg"]["prod"].asString())));




horse_scan_table_rows_++;
}



void FileLogForms::insertDataToResDetector(const Json::Value &data)
{


}



void FileLogForms::DisplayProgress(int progress)
{
/*
    ui->progressBar->setValue(progress);
    progress_value_ = progress;

    if (progress >= 100) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->labelConclution->setText(QString("供扫描 %1 项，用时 %2").arg(scan_total_file_number_).arg(formatTime(use_second_, true)));
        if (current_row_ != 0) {
            ui->labelResult->setText(QString("扫描完成，供发现 %1 个疑似敏感文件").arg(current_row_));
        }
        timer_->stop();
        ui->tableWidget->setSortingEnabled(true);
    }
*/
}

void FileLogForms::switchTaskStatus(bool pause)
{

}

void FileLogForms::insertAlarmInfoToTable(const FILESCANRES &file_scan_res)
{
    ui->tableWidget->setItem(current_row_, 0, new QTableWidgetItem(QString::fromStdString(file_scan_res.full_path)));
    ui->tableWidget->setItem(current_row_, 1, new QTableWidgetItem(QString::number(file_scan_res.score)));
    ui->tableWidget->item(current_row_, 1)->setForeground(Qt::red);
    ui->tableWidget->setItem(current_row_, 2, new QTableWidgetItem(QString::fromStdString(file_scan_res.key_word)));
    ui->tableWidget->item(current_row_, 2)->setForeground(Qt::red);
    ui->tableWidget->item(current_row_, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(current_row_, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    QString content_summary = QString::fromStdString(file_scan_res.content_summary);
    QStringList keyword_list = QString::fromStdString(file_scan_res.key_word).split("|");
    for (int i = 0; i < keyword_list.size(); i++) {
        QString keyword = keyword_list.at(i);
        QString replace_str = QString("<font color=\"red\">%1</font>").arg(keyword);
        content_summary.replace(keyword, replace_str);
    }

    QLabel *label = new QLabel;
    label->setText(content_summary);
    label->setFont(QFont("Microsoft Yahei", 11));
    label->setToolTip(content_summary);
    QWidget *widget = new QWidget();
    //QHBoxLayout *hlayout = new QHBoxLayout();
    //hlayout->addWidget(label);
   // hlayout->setAlignment(widget, Qt::AlignCenter);
   // hlayout->setContentsMargins(0, 0, 0, 0);
   // widget->setLayout(hlayout);
    ui->tableWidget->setCellWidget(current_row_, 3, widget);

    ui->tableWidget->item(current_row_, 0)->setToolTip(QString::fromStdString(file_scan_res.full_path));

    current_row_++;
}



void FileLogForms::displayAlarmFileInfo(QVariant alarm_info)
{
    FILESCANRES file_scan_res;
    file_scan_res = alarm_info.value<FILESCANRES>();

    if (ui->tableWidget->rowCount() == current_row_) {
        ui->tableWidget->setRowCount(current_row_ + 5);
    }

//    if (scan_file_list_.contains(file_scan_res.full_path.c_str()))
//        return;

    insertAlarmInfoToTable(file_scan_res);
}

void FileLogForms::setPolicy()
{
    QString config_path = QString("%1%2").arg(QApplication::applicationDirPath()).arg("/policyconfig.ini");
    QFileInfo file(config_path);
    if (file.exists()) {
        QSettings *policy_ini = new QSettings(config_path, QSettings::IniFormat);
        keyword_list_ = policy_ini->value("/policy/keyword").toString().split('|');
        regular_list_ = policy_ini->value("/policy/regular").toString().split('|');
        scan_type_ = policy_ini->value("/policy/type").toInt();
        custom_path_list_ = policy_ini->value("/policy/path").toString().split('|');

        delete policy_ini;
    }

    if (keyword_list_.size() == 0 || regular_list_.size() == 0 ||
            scan_type_ < 1 || scan_type_ > 4 ||
            (scan_type_ == 4 && custom_path_list_.size() == 0)) {
        keyword_list_ = default_keyword_.split('|');
        regular_list_ = default_regular_.split('|');
        scan_type_ = 4;
        custom_path_list_.clear();
        custom_path_list_.push_back("Desktop");
    }
}

void FileLogForms::showDialogOnTopLevel(int ret)
{
    QRect curGemRect = geometry();
    if (ret == 0) {
        Qt::WindowStates winStatus = Qt::WindowNoState;
        if (windowState() & Qt::WindowMaximized) {
            winStatus = Qt::WindowMaximized;
        }
        setWindowState(Qt::WindowMinimized);
        setWindowState(Qt::WindowActive | winStatus);
        setGeometry(curGemRect);
        activateWindow();
        raise();
    }
}

void FileLogForms::getPolicyConfig(QVariant config)
{
    POLICYCONFIG policy_config;
    policy_config = config.value<POLICYCONFIG>();

    keyword_list_ = policy_config.keyword_list;
    regular_list_ = policy_config.regular_list;
    scan_type_ = policy_config.type;
    custom_path_list_ = policy_config.custom_path_list;
}

