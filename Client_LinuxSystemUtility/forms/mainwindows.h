#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H

#include <QDialog>
#include <QMouseEvent>
#include <QThread>
#include <QTimer>

#include "SystemModuleSecurityCheck.h"
#include "USBHistoryEventsScanForms.h"
#include "processanalysisforms.h"
#include "FileSystemeScanForms.h"
#include "aboutforms.h"
#include "MountFileSystemForms.h"
#include"InternetHistoryForms.h"
#include"CheckInternetConnectforms.h"
#include"OperationSysteminfoForms.h"
#include "filelogforms.h"
#include"NetworkAdapterForms.h"
#include"USBHardWareForms.h"
#include"BluetoothForms.h"
#include"SoftwareCheckOfSystemInstallationForms.h"
namespace Ui {
class MainWindows;
}

class MainWindows : public QDialog
{
    Q_OBJECT


public:
    explicit MainWindows(QWidget *parent = 0);
    ~MainWindows();

private slots:
    void navigationBarBtnClick();
    void navigationBarBtnToggle();
    void btnCloseClick();
    void btnMaxClick();
    void btnMinClick();
    void btnMenuClick();
    void showAboutForms();
    void rebootMachine();
    void autoRollActionTriggle();
    void refreshData();





private:
    void initStyle();
    void initForms();

private:
    SystemModuleSecurityCheck *SystemModule_analysis_windows_;
    USBHistoryEventsScanForms *full_scan_windows_;
    ProcessAnalysisForms *process_analysis_windows_;
    FileSystemeScanForms *filesystem_scan_windows;
    AboutForms *about_windows_;
 MountFileSystemForms *mountfilesystem_windows_;
     InternetHistoryForms * InternetHistory_windows;//take:add;
     SysemAccountForms * SystemAccount_windows;
     CheckInternetConnectforms*  CheckInternetConnect_windows;
    FileLogForms *file_log_forms_windows;
    OperationSysteminfoForms* OperationSysteminfoForms_windows;
    NetworkAdapterForms* NetworkAdapterForms_windows_;
    USBHardWareForms* USBHardWareForms_windows_;
    BluetoothForms* BluetoothForms_windows_;
    SoftwareCheckOfSystemInstallationForms *SoftwareCheckOfSystemInstallationForms_windows_;


private:

    Ui::MainWindows *ui;
    QPoint mouse_point_;
    QRect location_;
    bool mouse_pressed_;
    bool max_;
    QThread net_worker_thread_;
    QStringList btn_check_image_list_;
    QStringList bnt_uncheck_image_list_;
    QTimer *refresh_data_timer_;

    //与缩放相关的变量
    int zoom_;
    QRect zoom_localtion_;
    QPoint zoom_pos_;

protected:
    void mouseMoveEvent(QMouseEvent *e) {
        if (mouse_pressed_ && (e->buttons() && Qt::LeftButton) && !max_) {
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

    bool eventFilter(QObject *obj, QEvent *e);/* {
        if (event->type() == QEvent::MouseButtonDblClick) {
            this->btnMaxClick();
            return true;
        }

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            int key = ke->key();
            if (key == Qt::Key_Escape)
                return true;
        }

        return QObject::eventFilter(obj, event);
    }*/
};

#endif // MAINWINDOWS_H
