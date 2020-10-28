#ifndef USBHistoryEventsScanForms_H
#define USBHistoryEventsScanForms_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>

#include "json/json.h"

namespace Ui {
class USBHistoryEventsScanForms;
}


class USBHistoryEventsScanForms : public QDialog
{
    Q_OBJECT

public:
    explicit USBHistoryEventsScanForms(QWidget *parent = 0);
    ~USBHistoryEventsScanForms();

    void HandleServerMessage(const Json::Value &value,int len);


private slots:
    void btnClick();
    void btnToggle();
    void btnReportFormClick();



    void on_regetdata_clicked(bool checked);

    void on_regetdata_clicked();

private:
    void initForms();
    void insertDataToHorseScan(const Json::Value &data);  //take_add:将传入的json再次解析
    void insertDataToHorseScan(const QString &data, const QString &content_color = "0",int ops=0);
    void insertDataToResDetector(const Json::Value &data);

private:
    Ui::USBHistoryEventsScanForms *ui;
    int horse_scan_table_rows_;
    int res_detector_rows;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // USBHistoryEventsScanForms_H
