#ifndef FileSystemeScanForms_H_
#define FileSystemeScanForms_H_

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
class FileSystemeScanForms;
}

class FileSystemeScanForms : public QDialog
{
    Q_OBJECT

public:
    explicit FileSystemeScanForms(QWidget *parent = 0);
    ~FileSystemeScanForms();

    void HandleServerMessage(const Json::Value &value);

private slots:
    void btnClick();
    void btnToggle();
    void createMenu(const QPoint &pos);
    void openFileDir();

    void on_actionPushButton_clicked(bool checked);






private:
    void initForms();
    void insertDataToRealTimeMonitor(const QString &data, const QString content_color = "0");
    void insertDataToFileSystemScan(const Json::Value &data);
    void insertDataToNetBehavior(const QString &data);


private:
    Ui::FileSystemeScanForms *ui;
    int real_time_monitor_table_rows_;
    int filsystem_scan_table_rows_;
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

#endif // FileSystemeScanForms_H
