#ifndef ABOUTFORMS_H
#define ABOUTFORMS_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class AboutForms;
}

class AboutForms : public QDialog
{
    Q_OBJECT

public:
    explicit AboutForms(QWidget *parent = 0);
    ~AboutForms();

private:
    void initStyle();

private:
    Ui::AboutForms *ui;
    bool mouse_press_;
    QPoint mouse_point_;

protected:
    void mouseMoveEvent(QMouseEvent *e)
    {
        if (mouse_press_ && (e->buttons() && Qt::LeftButton)) {
            this->move(e->globalPos() - mouse_point_);
            e->accept();
        }
    }

    void mousePressEvent(QMouseEvent *e)
    {
        if (e->button() == Qt::LeftButton) {
            mouse_press_ = true;
            mouse_point_ = e->globalPos() - this->pos();
            e->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent *)
    {
        mouse_press_ = false;
    }
};

#endif // ABOUTFORMS_H
