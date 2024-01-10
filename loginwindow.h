#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "defines.h"
namespace Ui {
class loginwindow;
}

class loginwindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);
    ~loginwindow();
    QString base64_encode(QString str);
    QString base64_decode(QString str);

private:
    Ui::loginwindow *ui;
    QPoint position;
    bool pressed;

signals:
    void loginStatus(userInfo userData);
private slots:
    void on_pushButtonlogin_clicked();
    void on_lineEdit_username_textEdited(const QString &arg1);
    void on_lineEdit_password_textEdited(const QString &arg1);
    void on_close_clicked();
    void on_minimize_clicked();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // LOGINWINDOW_H
