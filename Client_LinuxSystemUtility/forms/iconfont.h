#ifndef ICONFONT_H
#define ICONFONT_H

#include <QObject>
#include <QFont>
#include <QFontDatabase>
#include <QMutex>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class IconFont : public QObject
{
private:
    explicit IconFont(QObject *parent = 0);
    QFont iconFont;
    static IconFont* _instance;

public:
    static IconFont* Instance()
    {
        if (!_instance) {
            _instance = new IconFont;
        }
        return _instance;
    }

    void SetIcon(QLabel* lab, QChar c, int size = 10);
    void SetIcon(QPushButton* btn, QChar c, int size = 10, QString str="");
};

#endif // ICONFONT_H
