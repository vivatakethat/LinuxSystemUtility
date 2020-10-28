#ifndef DYNAMICFORMS_H
#define DYNAMICFORMS_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>

#include "json/json.h"

namespace Ui {
class SystemModuleSecurityCheck;
}

class SystemModuleSecurityCheck : public QDialog
{
    Q_OBJECT

public:
    explicit SystemModuleSecurityCheck(QWidget *parent = 0);
    ~SystemModuleSecurityCheck();

    void HandleServerMessage(const Json::Value &value,int len);

private slots:
    void btnClick();
    void btnToggle();
    void createMenu(const QPoint &pos);
    void openFileDir();

    //###新增加
    void KernelAnalysisHandle(const Json::Value &value);


    void on_actSystemModuleScan_pushButton_clicked();

private:
    void initForms();
    void insertDataToDriverInfo(const Json::Value &data);
    void insertDataToShadowSsdt(const Json::Value &data);
    void insertDataToSsdtInfo(const Json::Value &data);
    void insertDataToIdt(const Json::Value &data);
    void insertDataToSystemModuleScan(const Json::Value &data);

private:
    Ui::SystemModuleSecurityCheck *ui;
    int driver_info_table_rows_;
    int ssdt_info_table_rows_;
    int shadow_ssdt_table_rows_;
    int idt_table_rows;
    int system_module_check_rows_;

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

#endif // DYNAMICFORMS_H
