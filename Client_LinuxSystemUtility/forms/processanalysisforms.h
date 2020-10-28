#ifndef ProcessAnalysisForms_H
#define ProcessAnalysisForms_H

#include <QDialog>

#include "json/json.h"

#include "SysemAccountForms.h"

namespace Ui {
class ProcessAnalysisForms;
}

class ProcessAnalysisForms : public QDialog
{
    Q_OBJECT


public:
    explicit ProcessAnalysisForms(QWidget *parent = 0);
    ~ProcessAnalysisForms();

    void HandleServerMessage(const Json::Value &value);

private slots:
    void btnClick();
    void btnToggle();
    void createMenu(const QPoint &pos);
    void showProcessDllDetailAction();
    void openFileDir();

    void ProcessAnalysisHandle(const Json::Value &value);

private:
    void initForms();
    void insertDataToProcessInfo(const Json::Value &data);
    void insertDataToSelfStart(const Json::Value &data);
    void insertDataToMessageHook(const Json::Value &data);
    void insertDataToPortInfo(const Json::Value &data);
    void insertDataToIatInfo(const Json::Value &data);
    void insertDataToHistoryProcess(const Json::Value &data);
    void showProcessDllDetail(const Json::Value &data);
    void insertTestData();

private:
    Ui::ProcessAnalysisForms *ui;
    int history_process_table_rows_;
    int process_info_table_rows_;
    int self_start_table_rows_;
    int message_hook_table_rows_;
    int port_info_table_rows_;
    int iat_info_table_rows_;
  //  OtherTableForms *show_dll_details_windows;
    QString show_destail_dll_name_;

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

#endif // ProcessAnalysisForms_H
