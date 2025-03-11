#include "loginwindow.h"
#include "mysql_conn.h"
#include "ui_loginwindow.h"
#include <QtConcurrent/QtConcurrent>
#include <QFontDatabase>
#include <qpainter.h>
#include <QMouseEvent>
#include "sql_conn_utils.h"
#include "mainwindow.h"

//extern MainWindow *MainWindowObj;


loginwindow::loginwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);

//#ifdef QT_DEBUG
#ifndef RELEASE_BUILD
    ui->lineEditUsername->setText("JNK82");
    ui->lineEdit_Password->setText("123");
    ui->pushButtonlogin->click();
#endif

 //   MainWindowObj = (MainWindow*) parent;
    QFontDatabase::addApplicationFont(":/RacingSansOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/WorkSans-Bold.ttf");

    ui->progressBar->setStyleSheet("border-radius: 5px;"
                                    "background: #D9D9D9;");

    ui->progressBar->setVisible(false);

    ui->lineEditUsername->setFocus();
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QPixmap pixmap(":/ALGO METHODS.png");


    QPixmap pixmaplogo(":/Vector.png");
    pixmap = pixmap.scaled(ui->name->width(),ui->name->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmaplogo = pixmaplogo.scaled(ui->logo->width(),ui->logo->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap pixmapclose(":/close_window_icon.png");
    QPixmap pixmapminimize(":/minimize_window_icon.png");
    ui->close->setIcon(pixmapclose);
    ui->minimize->setIcon(pixmapminimize);
    ui->name->setPixmap(pixmap);
    ui->logo->setPixmap(pixmaplogo);
    ui->label_wron_input->setVisible(false);
    ui->footer->setFont(QFont("Racing Sans One"));
    QFont font=ui->pushButtonlogin->font();
    font.setFamily("Work Sans");
    ui->pushButtonlogin->setFont(font);
    connect(ui->lineEditUsername, &QLineEdit::returnPressed, this, [=]() {
        ui->lineEdit_Password->setFocus();
    });
    connect(ui->lineEdit_Password, &QLineEdit::returnPressed, this, [=]() {
        ui->pushButtonlogin->setFocus();
        on_pushButtonlogin_clicked();
    });
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    if(groups.contains("AutoLoginSett")){
        settings.beginGroup("AutoLoginSett");
        if (settings.contains("remember_me")){
            QString remember_me = settings.value("remember_me").toString();
            if(remember_me=="1"){
                QString login_username = "";
                QString login_password = "";

                if (settings.contains("remember_me")){
                    login_username = base64_decode(settings.value("login_username").toString());
                    login_password = base64_decode(settings.value("login_password").toString());
                    ui->lineEditUsername->setText(login_username);
                    ui->lineEdit_Password->setText(login_password);
                }
            }
        }
        settings.endGroup();
    }

}

QString loginwindow::base64_encode(QString str){
    QByteArray ba;
    ba.append(str.toStdString().c_str());
    return ba.toBase64();
}

QString loginwindow::base64_decode(QString str){
    QByteArray ba;
    ba.append(str.toStdString().c_str());
    return QByteArray::fromBase64(ba);
}


loginwindow::~loginwindow()
{
    delete ui;
    QApplication::quit();
}

void loginwindow::on_pushButtonlogin_clicked()
{
    QString login_id = ui->lineEditUsername->text();
    QString passWord = ui->lineEdit_Password->text();
    if(login_id==""||passWord==""){
        userInfo userData;
        userData.loggedIn=false;
        userData.errorCode=T_LoginErroCode::EMPTY_CREDENTIALS;
        //emit loginStatus(userData);
        ui->label_wron_input->setVisible(true);
        ui->label_wron_input->setText("Username or Password cannot be empty !");
    }
    else{
        ui->pushButtonlogin->setEnabled(false);
        // mysql_conn *db_conn = new mysql_conn(MainWindowObj,"login_conn");
        mysql_conn *db_conn = new mysql_conn(0,"login_conn");

//        SqlConnUtils::SetConnection(db_conn);
        auto login_BackgroundTask = [this, login_id, passWord,db_conn]() {

            userInfo userData = db_conn->login(login_id, passWord);
//            if(userData.loggedIn){
//                //save user id and password
//                QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//                QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
////                QStringList groups = settings.childGroups();
////                if(!groups.contains("AutoLoginSett")){
////                    settings.beginGroup("AutoLoginSett");
////                    settings.setValue("remember_me",remeber_me?"1":"0");
//////                    settings.setValue("login_username",remeber_me?base64_encode(login_id):"");
//////                    settings.setValue("login_password",remeber_me?base64_encode(passWord):"");
////                    settings.endGroup();
////                }
////                else{
////                    settings.beginGroup("AutoLoginSett");
//////                    settings.setValue("remember_me",remeber_me?"1":"0");
//////                    settings.setValue("login_username",remeber_me?base64_encode(login_id):"");
//////                    settings.setValue("login_password",remeber_me?base64_encode(passWord):"");
////                    settings.endGroup();
////                }
//            }
            QMetaObject::invokeMethod(this, [this,userData]() {
                ui->pushButtonlogin->setEnabled(true);
                if(userData.loggedIn==false&&userData.dbError==false){
                    if (userData.errorCode == T_LoginErroCode::USERNAME_WRONG ){
                        ui->label_wron_input->setVisible(true);
                        ui->label_wron_input->setText("Wrong Username !");
                    }
                    else if(userData.errorCode == T_LoginErroCode::PASSWORD_WRONG){
                        ui->label_wron_input->setVisible(true);
                        ui->label_wron_input->setText("Wrong Password !");
                    }
                    else if(userData.errorCode == T_LoginErroCode::INACTIVE){
                        QMessageBox msgBox(this);
                        msgBox.setText(userData.loginResponse);
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.exec();
                    }
                }
                else
                {
                    emit loginStatus(userData);
                }
            });
        };
        QFuture<void> future = QtConcurrent::run(login_BackgroundTask);
    }

}


void loginwindow::on_lineEditUsername_textEdited(const QString &arg1)
{
    if(ui->label_wron_input->isVisible())
        ui->label_wron_input->setVisible(false);
}



void loginwindow::on_lineEdit_Password_textEdited(const QString &arg1)
{
    if(ui->label_wron_input->isVisible())
        ui->label_wron_input->setVisible(false);
}

void loginwindow::on_close_clicked()
{
    close();
}

void loginwindow::on_minimize_clicked()
{
    showMinimized();
}

void loginwindow::mousePressEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child!=ui->title_bar)        //mTitlebar is the QLabel on which we want to implement window drag
    {
        return;
    }
    pressed = true;
    position = event->pos();
}

void loginwindow::mouseMoveEvent(QMouseEvent *event) {
    if(pressed){
    if (event->buttons() == Qt::LeftButton) {
        if (position.x() != 0 || position.y() != 0) {
            move(event->globalPosition().x() - position.x(), event->globalPosition().y() - position.y());
        }
    }
    }
}
void loginwindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
    ui->lineEditUsername->clear();
    ui->lineEdit_Password->clear();
    }
}

void loginwindow::mouseReleaseEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child!=ui->title_bar)
    {
    return;
    }
    pressed = false;
}

//void loginwindow::mouseDoubleClickEvent(QMouseEvent *event) {
//    if (event->buttons() == Qt::LeftButton) {
//        QWidget* widget = childAt(event->x(), event->y());
//        if(widget == ui->buttons) {
//            isMaximized() ? showNormal() : showMaximized();
//        }
//    }
//}

//bool loginwindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
//    Q_UNUSED(eventType)
//    MSG *param = static_cast<MSG *>(message);
//    switch (param->message){
//    case WM_NCHITTEST:{
//        int nX = GET_X_LPARAM(param->lParam) - geometry().x();
//        int nY = GET_Y_LPARAM(param->lParam) - geometry().y();

//        if (childAt(nX, nY) != ui->header && childAt(nX, nY) != ui->body){
//            return QWidget::nativeEvent(eventType, message, result);
//        }

//        *result = HTCAPTION;

//        if ((nX > 0) && (nX < borderSize)){
//            *result = HTLEFT;
//        }

//        if ((nX > width() - borderSize) && (nX < width())){
//            *result = HTRIGHT;
//        }

//        if ((nY > 0) && (nY < borderSize)){
//            *result = HTTOP;
//        }

//        if ((nY > height() - borderSize) && (nY < height())){
//            *result = HTBOTTOM;
//        }

//        if ((nX > 0) && (nX < borderSize) && (nY > 0) && (nY < borderSize)){
//            *result = HTTOPLEFT;
//        }

//        if ((nX > width() - borderSize) && (nX < width()) && (nY > 0) && (nY < borderSize)){
//            *result = HTTOPRIGHT;
//        }

//        if ((nX > 0) && (nX < borderSize) && (nY > height() - borderSize) && (nY < height())){
//            *result = HTBOTTOMLEFT;
//        }

//        if ((nX > width() - borderSize) && (nX < width()) && (nY > height() - borderSize) && (nY < height())){
//            *result = HTBOTTOMRIGHT;
//        }

//        return true;
//    }
//    }

//    return QWidget::nativeEvent(eventType, message, result);
//}




