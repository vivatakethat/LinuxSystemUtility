#ifndef CheckInternetConnectforms_H
#define CheckInternetConnectforms_H



#include <QDialog>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QTableWidget>

#include "json/json.h"


namespace Ui {
class CheckInternetConnectforms;
}

class CheckInternetConnectforms : public QDialog
{
    Q_OBJECT

public:
    explicit CheckInternetConnectforms(QWidget *parent = 0);
    ~CheckInternetConnectforms();

    void HandleServerMessage(const Json::Value &value,int len);

private slots:
    void btnClick();
    void btnToggle();
    void createMenu(const QPoint &pos);
    void openFileDir();



    void on_actCheckInternetConnectPushButton_clicked();

private:
    void initForms();
    void insertMountFileSystemScan(const Json::Value &data);
    int CheckInternetConnect_table_rows_;

private:
    Ui::CheckInternetConnectforms *ui;

    int Checkinternetconnec_table_rows_;

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

#endif // CheckInternetConnectforms_H
