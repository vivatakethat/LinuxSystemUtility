#ifndef TIPMESSAGEBOX_H
#define TIPMESSAGEBOX_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class TipMessageBox;
}

class TipMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit TipMessageBox(QWidget *parent = 0);
    ~TipMessageBox();

public:
    void SetTipInfo(const QString &content);

private:
    void initStyle();
    void initForm();

private:
    Ui::TipMessageBox *ui;
    bool mouse_press_;
    QPoint mouse_point_;

private slots:
    void sureBtnClick();
    void quitBtnClick();
    void closeBtnClick();


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

#endif // TIPMESSAGEBOX_H
