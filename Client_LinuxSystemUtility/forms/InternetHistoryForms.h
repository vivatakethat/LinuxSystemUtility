
#ifndef InternetHistoryForms_H
#define InternetHistoryForms_H


#include <QDialog>

#include "json/json.h"
#include"../bussines/webserviceclientworker.h"
namespace Ui {
class InternetHistoryForms;
}

class InternetHistoryForms: public QDialog
{
    Q_OBJECT
public:
    explicit InternetHistoryForms(QWidget *parent = nullptr);
   void  HandleServerMessage(const Json::Value &value,int len);
    ~InternetHistoryForms();
        void test();


private  slots:
            void btnClick();

void btnSysInfo();
void initForms();

void on_fireFoxToolButton_clicked();

void on_actInternetHistoryPushButton_clicked();

void on_chromeToolButton_clicked();

private:
int browerType;
 Ui::InternetHistoryForms * take_Ui;
 void insertDataToBrowserScan(const Json::Value &data,int len);
int firefox_scan_table_rows_;
 int chrome_table_rows_;

};

#endif //
