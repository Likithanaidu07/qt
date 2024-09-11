
#include "logger.h"
#include <QCoreApplication>
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include <QDir>
#include <QFileInfoList>
#include <QMutex>
#include <QThread>
#include<QStandardPaths>
#include <qt_windows.h>
#include <windows.h>
#include <io.h>
using namespace logger;

namespace logger
{
QString application_;
QString logFolderName;
QString logFileName;
static QFile* outFile;

static QFile::Permissions m_umask;

void initializeUmask()
{
    m_umask = QFile::WriteGroup | QFile::WriteOther;
}

void initLogFileName(QString app)
{
    outFile->close();

    QFile::Permissions permAll = QFile::ReadOwner
                                 | QFile::ReadGroup
                                 | QFile::ReadOther
                                 | QFile::WriteOwner
                                 | QFile::WriteGroup
                                 | QFile::WriteOther;

    delete outFile;
    outFile = nullptr;    
    logFileName = QString(logFolderName + "/%1_%2__%3.txt").arg(app,QDate::currentDate().toString("dd_MM_yyyy"),QTime::currentTime().toString("hh_mm_ss"));
    outFile = new QFile(logFileName);

    if(!outFile->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug("File not open");
    }
    outFile->close();
    if(!outFile->setPermissions(permAll))
    {
        qWarning()<<"Unable to set Prmission to log file "<<__LINE__ <<" "<<logFileName;
    }

    if(!outFile->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug("File not open");
    }
}

bool  initLogging(const QString &loggingApplication)
{
    initializeUmask();
    qDebug("applicationDirPath [%s]", QCoreApplication::applicationDirPath().toStdString().c_str());
    application_ = loggingApplication;
    logFolderName = QString(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs");
    outFile = new QFile();
    qDebug("logFolderName [%s]", logFolderName.toStdString().c_str());

    // Create folder for logfiles if not exists
    if(!QDir(logFolderName).exists())
    {
        if(!QDir().mkpath(logFolderName))
        {
            return false;
        }
    }
    initLogFileName(application_); //create the logfile name

    qInstallMessageHandler(logger::myMessageHandler);
    return true;
}

void myMessageHandler(QtMsgType type, const QMessageLogContext &context,
                      const QString& msg)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QString txt = "";

    QString dateTimeFormat = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss");

    txt = QString::number(reinterpret_cast<int64_t>(QThread::currentThreadId()));
    switch (type) {
    case QtInfoMsg:
        txt += QString("[%1][Info] %2 :: %3 :: %4 :: %5\n").arg(dateTimeFormat, context.file, context.function, QString::number(context.line),  msg);
        break;
    case QtDebugMsg:
        txt += QString("[%1][Debug] %2 :: %3 :: %4 :: %5\n").arg(dateTimeFormat, context.file, context.function, QString::number(context.line),  msg);
        break;
    case QtWarningMsg:
        txt += QString("[%1][Warning] %2 :: %3 :: %4 :: %5\n").arg(dateTimeFormat, context.file, context.function, QString::number(context.line),  msg);
        break;
    case QtCriticalMsg:
        txt += QString("[%1][Critical] %2 :: %3 :: %4 :: %5\n").arg(dateTimeFormat, context.file, context.function, QString::number(context.line),  msg);
        break;
    case QtFatalMsg:
        txt += QString("[%1][Fatal] %2 :: %3 :: %4 :: %5\n").arg(dateTimeFormat, context.file, context.function, QString::number(context.line),  msg);
        break;
    }

    //To print at console and also disable macro from .pro file for debug, info and warnings
    //fprintf(stderr, "%s\n", txt.toStdString().c_str());
    QByteArray s(txt.toStdString().c_str());
    outFile->write(s);
    outFile->flush();
    s.clear();

    int size = outFile->size();
    if (size > LOGSIZE) //check current log size
    {
        initLogFileName(application_);
    }

}
}
