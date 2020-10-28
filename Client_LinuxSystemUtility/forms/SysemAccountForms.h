#ifndef SysemAccountForms_H
#define SysemAccountForms_H

#include <QDialog>
#include <QMouseEvent>

#include "json/json.h"

namespace Ui {
class SysemAccountForms;
}

class SysemAccountForms : public QDialog
{
    Q_OBJECT

public:
    explicit SysemAccountForms(QWidget *parent = 0);
    ~SysemAccountForms();

    void SetData(const QString &title, const Json::Value &data);
    void HandleServerMessage(const Json::Value &value,int len);
        void insertSystemAccountScan(const Json::Value &data);
private slots:
    void createMenu(const QPoint &pos);
    void openFileDir();

    void on_actSystemAccount_pushButton_clicked();

private:
    void initStyle();
    void initForms();
    void insertData();
    int    SystemAccount_table_rows_;

private:
    Ui::SysemAccountForms *ui;
    bool mouse_pressed_;
    QPoint mouse_point_;

protected:
    void mouseMoveEvent(QMouseEvent *e) {
        if (mouse_pressed_ && (e->buttons() && Qt::LeftButton)) {
            this->move(e->globalPos() - mouse_point_);
            e->accept();
        }
    }

    void mousePressEvent(QMouseEvent *e) {
        if (e->button() == Qt::LeftButton) {
            mouse_pressed_ = true;
            mouse_point_ = e->globalPos() - this->pos();
            e->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent *) {
        mouse_pressed_ = false;
    }

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

#endif // SysemAccountForms_H
