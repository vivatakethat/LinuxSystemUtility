#ifndef GETCHECKFILEINFO_H
#define GETCHECKFILEINFO_H

#include <QDialog>

namespace Ui {
class GetCheckFileInfo;
}

class GetCheckFileInfo : public QDialog
{
    Q_OBJECT

public:
    explicit GetCheckFileInfo(QWidget *parent = 0);
    ~GetCheckFileInfo();
    void initStyle();
    QString getCheckManName();
    QString getCompanyName();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GetCheckFileInfo *ui;
    QString company_name;
    QString Checkman_name;
};

#endif // GETCHECKFILEINFO_H
