#include "iconfont.h"

IconFont* IconFont::_instance = 0;

IconFont::IconFont(QObject*):
    QObject(qApp)
{
    int fontId = QFontDatabase::addApplicationFont(":/image/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName);
}

void IconFont::SetIcon(QLabel *lab, QChar c, int size)
{
    iconFont.setPointSize(size);
    lab->setFont(iconFont);
    lab->setText(c);
}

void IconFont::SetIcon(QPushButton *btn, QChar c, int size, QString str)
{
    iconFont.setPointSize(size);
    btn->setFont(iconFont);
    if(str.isEmpty())
        btn->setText(c);
    else {
        btn->setText(QString("%1\r\n%2").arg(c).arg(str));
    }
}
