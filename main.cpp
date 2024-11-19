 #include "mainwindow.h"
#include "loginwindow.h"
#include <QFontDatabase>
#include <QApplication>
#include <QMessageBox>
#include "logger/logger.h"
//#include "UiUtils.h"
QString userNameLogged;
QAtomicInt data_exchangeTimestamp(0);
QAtomicInt portfolio_table_updating_db(0);
std::atomic<int> portfolio_table_slected_idx_for_editing;
MainWindow *MainWindowObj;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("New_SpeedTrade");
    QCoreApplication::setOrganizationName("New_AlgoMethods");

 //#ifndef QT_DEBUG

 #ifdef RELEASE_BUILD
    logger::initLogging("SpeedTradeLog_");
 #endif
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

    if(QFileInfo::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/contracts"+"/contracts.bin"))
    {
        QFile::remove(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/contracts"+"/contracts.bin");
    }

    MainWindow mainWindow;
    MainWindowObj = &mainWindow;


   // UiUtils::SetMainWindow(&mainWindow);
    //loginwindow loginWindow(MainWindowObj);
    loginwindow loginWindow;

    QObject::connect(&loginWindow, &loginwindow::loginStatus,&mainWindow, &MainWindow::loggedInSucessful);
    QObject::connect(&loginWindow, &loginwindow::loginStatus, [&](const userInfo& userInfo) {
        if (userInfo.loggedIn) {
            MainWindowObj->userData = userInfo;
            QMetaObject::invokeMethod(&loginWindow, "close", Qt::QueuedConnection);
            QMetaObject::invokeMethod(MainWindowObj, "loggedIn", Qt::QueuedConnection);
            QMetaObject::invokeMethod(MainWindowObj, "showMaximized", Qt::QueuedConnection);

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

        }
    });

    loginWindow.show();
    return a.exec();
}
