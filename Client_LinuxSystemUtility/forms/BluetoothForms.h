



#ifndef BluetoothForms_H

#define BluetoothForms_H

#include <QDialog>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QTableWidget>

#include "json/json.h"

namespace Ui {
class BluetoothForms;
}

class BluetoothForms : public QDialog
{
    Q_OBJECT

public:
    explicit BluetoothForms(QWidget *parent = 0);
    ~BluetoothForms();

    void HandleServerMessage(const Json::Value &value,int len);

private slots:
    void btnClick();
    void btnToggle();




    void on_actBluetoothPushButton_clicked();

private:
    void initForms();
    void insertBluetoothFormsScan(const Json::Value &data);

private:
    Ui::BluetoothForms *ui;
    int real_time_monitor_table_rows_;
    int horse_scan_table_rows_;
    int net_behavior_table_rows_;
    int  BluetoothForms_table_rows_;

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

#endif // BluetoothForms


