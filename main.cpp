#include "mainwindow.h"
#include "loginwindow.h"
#include <QFontDatabase>
#include <QApplication>
#include <QMessageBox>
//#include "UiUtils.h"
QString userNameLogged;
QAtomicInt data_exchangeTimestamp(0);
QAtomicInt portfolio_table_updating_db(0);
std::atomic<int> portfolio_table_slected_idx_for_editing;

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }
    txt = QDateTime::currentDateTime().toString()+": "+txt;

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QString directoryPath = appDataPath+"/logs";
    QDir directory(directoryPath);
    if (!directory.exists()){
        directory.mkpath(".");
    }
    QString fileName = "SpeedTradeLog_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";
    if (directory.exists())
        fileName = directoryPath+"/"+fileName;

    QFile outFile(fileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << "\n";
    outFile.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("New_SpeedTrade");
    QCoreApplication::setOrganizationName("New_AlgoMethods");
    //#ifndef QT_DEBUG
    qInstallMessageHandler(myMessageHandler);
    //#endif
    QFontDatabase::addApplicationFont(":/RacingSansOne-Regular.ttf");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SpeedTrade_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow mainWindow;
   // UiUtils::SetMainWindow(&mainWindow);
    loginwindow loginWindow;
    QObject::connect(&loginWindow, &loginwindow::loginStatus, [&](const userInfo& userInfo) {
        if (userInfo.loggedIn) {
            mainWindow.userData = userInfo;
            QMetaObject::invokeMethod(&mainWindow, "showMaximized", Qt::QueuedConnection);
            QMetaObject::invokeMethod(&loginWindow, "close", Qt::QueuedConnection);
            QMetaObject::invokeMethod(&mainWindow, "loggedIn", Qt::QueuedConnection);
        }
        else if (userInfo.dbError){
            QMetaObject::invokeMethod(qApp, [userInfo]() {
                QMessageBox msgBox;
                msgBox.setText(userInfo.loginResponse);
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            });
        }
        else {
            // Do the warning here
        }
    });

    loginWindow.show();
    return a.exec();
}
