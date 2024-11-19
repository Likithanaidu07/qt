#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "defines.h"

#define RELEASE_BUILD // comment this to enable logs on console


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
    void on_close_clicked();
    void on_minimize_clicked();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void on_lineEditUsername_textEdited(const QString &arg1);
    void on_lineEdit_Password_textEdited(const QString &arg1);
};

#endif // LOGINWINDOW_H
