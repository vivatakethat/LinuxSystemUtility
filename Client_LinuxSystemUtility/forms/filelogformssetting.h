#ifndef FILELOGFORMSSETTING_H
#define FILELOGFORMSSETTING_H

#include <QDialog>
#include "filelogforms.h"
#include <QFileDialog>

namespace Ui {
class FileLogFormsSetting;
}

class FileLogFormsSetting : public QDialog
{
    Q_OBJECT

public:
    explicit FileLogFormsSetting(QWidget *parent = nullptr);
    ~FileLogFormsSetting();
    void initForms();
    void initStyle();

    QStringList keyword_list() const { return keyword_list_; }
    QStringList regular_list() const { return regular_list_; }
    QStringList custom_path_list() const { return custom_path_list_; }
    int scan_type() const { return scan_type_; }
signals:
    void policyConfig(QVariant policy_config);
private slots:
    int customCheckPath(QString path = "");
    void btnAddKeyWordClick();
    void navigatorBtnClick();
    void checkAllBtnClick();
    void btnSubKeyWordClick();
    void btnCloseClick();
    void radioButtonScanTypeClick();
    void btnAddPathClick();
    void on_btnSettingClose_2_clicked();

private:
    void refreshKeyword();
    int savePolicy();
    void applyLocalPolicy();
    void refreshRegular();
private:
    Ui::FileLogFormsSetting *ui;

    QStringList keyword_list_;
    QStringList regular_list_;
    QStringList custom_path_list_;
    int scan_type_;
};

#endif // FILELOGFORMSSETTING_H
