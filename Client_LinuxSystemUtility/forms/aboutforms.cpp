#include "aboutforms.h"
#include "ui_aboutforms.h"

#include <QGraphicsDropShadowEffect>

#include "iconfont.h"

AboutForms::AboutForms(QWidget *parent) :
    QDialog(parent),
    mouse_press_(false),
    ui(new Ui::AboutForms)
{
    ui->setupUi(this);
    initStyle();
}

AboutForms::~AboutForms()
{
    delete ui;
}

void AboutForms::initStyle()
{
    this->setWindowIcon(QIcon(":/image/blue.svg"));
    this->setWindowTitle("关于我们");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QColor(255,255,255)));
    ui->widgetAboutShadow->setPalette(palette);
    ui->widgetAboutShadow->setAutoFillBackground(true);
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect();
    shadow_effect->setBlurRadius(10.0);
    shadow_effect->setColor(QColor(0, 0, 0, 150));
    shadow_effect->setOffset(0.5);
    ui->widgetAboutShadow->setGraphicsEffect(shadow_effect);

    IconFont::Instance()->SetIcon(ui->btnAboutClose, QChar(0xf00d), 12);
    connect(ui->btnAboutClose, SIGNAL(clicked(bool)), this, SLOT(hide()));
}
