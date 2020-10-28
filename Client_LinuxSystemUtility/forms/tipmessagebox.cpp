#include "tipmessagebox.h"
#include "ui_tipmessagebox.h"

#include <QGraphicsDropShadowEffect>

#include "iconfont.h"

TipMessageBox::TipMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipMessageBox)
{
    ui->setupUi(this);
    initStyle();
    initForm();
}

TipMessageBox::~TipMessageBox()
{
    delete ui;
}

void TipMessageBox::SetTipInfo(const QString &content)
{
    ui->labelBoxContent->setText(content);
    ui->btnBoxContinue->setText("确定");
    ui->btnBoxQuit->setText("取消");
}

void TipMessageBox::initStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowModality(Qt::ApplicationModal);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QColor(255,255,255)));
    ui->widgetBoxShadow->setPalette(palette);
    ui->widgetBoxShadow->setAutoFillBackground(true);
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect();
    shadow_effect->setBlurRadius(10.0);
    shadow_effect->setColor(QColor(0, 0, 0, 150));
    shadow_effect->setOffset(0.5);
    ui->widgetBoxShadow->setGraphicsEffect(shadow_effect);
}

void TipMessageBox::initForm()
{
    IconFont::Instance()->SetIcon(ui->btnBoxClose, QChar(0xf00d), 20);
    connect(ui->btnBoxClose, SIGNAL(clicked(bool)), this, SLOT(closeBtnClick()));
    connect(ui->btnBoxQuit, SIGNAL(clicked(bool)), this, SLOT(quitBtnClick()));
    connect(ui->btnBoxContinue, SIGNAL(clicked(bool)), this, SLOT(sureBtnClick()));
}

void TipMessageBox::sureBtnClick()
{
    done(0);
}

void TipMessageBox::quitBtnClick()
{
    done(1);
}

void TipMessageBox::closeBtnClick()
{
    done(2);
}
