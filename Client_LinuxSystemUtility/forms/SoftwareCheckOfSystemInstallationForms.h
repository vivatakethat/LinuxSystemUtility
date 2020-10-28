#ifndef SoftwareCheckOfSystemInstallationForms_H_
#define SoftwareCheckOfSystemInstallationForms_H_

#include <QDialog>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QTableWidget>

#include "json/json.h"
#include"../bussines/webserviceclientworker.h"
#include <thread>
#include <mutex>
#include <memory>


namespace Ui {
class SoftwareCheckOfSystemInstallationForms;
}

class SoftwareCheckOfSystemInstallationForms : public QDialog
{
    Q_OBJECT

public:
    explicit SoftwareCheckOfSystemInstallationForms(QWidget *parent = 0);
    ~SoftwareCheckOfSystemInstallationForms();

    void HandleServerMessage(const Json::Value &value,int len);

private slots:
    void btnClick();
    void btnToggle();
    void createMenu(const QPoint &pos);


    void on_actionPushButton_clicked(bool checked);


    void on_actionPushButton_clicked();

    void on_actionPackageInfoPushButton_clicked();

private:
    void initForms();
    void insertDataToRealTimeMonitor(const QString &data, const QString content_color = "0");
    void insertDataToPackageScan(const Json::Value &data);
    void insertDataToNetBehavior(const QString &data);


private:
    Ui::SoftwareCheckOfSystemInstallationForms *ui;
    int real_time_monitor_table_rows_;
    int package_scan_table_rows_;
    int MountPoint_table_rows_;
    int actionType;

protected:
    bool eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            int key = ke->key();
            if (key == Qt::Key_Escape)
                return true;
        }

        return QObject::eventFilter(obj, event);
    }
};

#endif // SoftwareCheckOfSystemInstallationForms_H
