#ifndef FILELOGFORMS_H
#define FILELOGFORMS_H
#include <QDialog>
#include "html.h"
#include "json/json.h"
#include <QString>
#include <QEvent>
#include <QKeyEvent>
#include <QFileInfo>
#include <QSettings>
#include <QMenu>
#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QTimer>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHBoxLayout>
#include "FileLogCheckHttp.h"
#include <QHostInfo>
#include "getcheckfileinfo.h"
#include <QMessageBox>
typedef struct FileScanRes {
    int score;
    std::string full_path;
    std::string key_word;
    std::string content_summary;
}FILESCANRES;

Q_DECLARE_METATYPE(FILESCANRES);

typedef struct PolicyConfig {
    int type;
    QStringList keyword_list;
    QStringList regular_list;
    QStringList custom_path_list;
}POLICYCONFIG;

Q_DECLARE_METATYPE(POLICYCONFIG);


namespace Ui {
class FileLogForms;
}

class FileLogForms : public QDialog
{
    Q_OBJECT

public:
    explicit FileLogForms(QWidget *parent = nullptr);
    ~FileLogForms();
    void HandLeServerMessage(const Json::Value &value);
    QString SetKeyWordHttpMessage(QStringList KeyWordList);
    QString SetRegularHttpMessage(QString RegularList);
    QString SetCustomPathHttpMessage(QStringList CustomPathList);
    QString SetRegularHttpMessageArchive(QString RegularList);
    void getMessageString(std::string message);
    void setMessageString(std::string message);
    void setFile_numResult(int num);
    void btnRenew();
public slots:
    void DisplayProgress(int progress);

signals:
    void policyConfig(QVariant policy_config);
    //void startScanWork();

private slots:
    void btnToggle();
    void btnClick();
    void getPolicyConfig(QVariant config);


    void showDialogOnTopLevel(int ret);
    void tableCellDoubleClicked(int row, int column);

    void displayAlarmFileInfo(QVariant alarm_info);
    void createTableMenu(const QPoint &pos);
    void actionOpenDirTrigger(bool);

private:
    void initForms();
    void insertDataToHorseScan(const Json::Value &data,int num);  //take_add:将传入的json再次解析
    void insertDataToHorseScan(const QString &data, const QString &content_color = "0",int ops=0);
    void insertDataToResDetector(const Json::Value &data);
    void setPolicy();
    void switchTaskStatus(bool pause);
    void insertAlarmInfoToTable(const FILESCANRES &file_scan_res);
    void insertData();
    //void initWorker();
protected:
        virtual void resizeEvent(QResizeEvent *event) override;

private:
    int file_num;
    QStringList sumFileName;
    QStringList filePath;
    QStringList classifiedContent;
    QStringList secretNum;
    std::string tableWidgetMessage;
    Ui::FileLogForms *ui;
    int real_time_monitor_table_rows_;
    int horse_scan_table_rows_;
    int net_behavior_table_rows_;
    bool start_scan_;
    QTimer *timer_;

    //policy
    QStringList keyword_list_;
    QStringList regular_list_;
    QStringList custom_path_list_;
    int scan_type_;


    QString default_regular_;
    QString default_keyword_;
    int default_scan_type_;

    int current_row_;

    QThread scan_thread_;

    //QWebView *mp_HTML;

};

#endif // FILELOGFORMS_H
