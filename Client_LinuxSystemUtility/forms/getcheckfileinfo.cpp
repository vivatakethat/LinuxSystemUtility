#include "getcheckfileinfo.h"
#include "ui_getcheckfileinfo.h"

GetCheckFileInfo::GetCheckFileInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetCheckFileInfo)
{
    ui->setupUi(this);
    initStyle();
}

GetCheckFileInfo::~GetCheckFileInfo()
{
    delete ui;
}
void GetCheckFileInfo::initStyle()
{
    ui->lineEditCheckMan->setPlaceholderText("请输入检查员信息");
    ui->lineEditCompany->setPlaceholderText("请输入检查单位信息");
    ui->pushButton_2->hide();
}

void GetCheckFileInfo::on_pushButton_clicked()
{
    Checkman_name= ui->lineEditCheckMan->text();
    company_name = ui->lineEditCompany->text();
    this->close();
}

void GetCheckFileInfo::on_pushButton_2_clicked()
{

}
QString GetCheckFileInfo::getCheckManName()
{
    return Checkman_name;
}

QString GetCheckFileInfo::getCompanyName()
{
    return company_name;
}
