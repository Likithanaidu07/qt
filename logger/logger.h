#pragma once

#define LOGSIZE 1024 * 1024//log size in bytes
#include <QObject>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QTime>

namespace logger
{

/**
 * @brief initLogging initializing the logging for logging application.
 * @param loggingApplication application which request for logging.
 * @return true, if logging init success.
 * @return false, if logging init failed.
 */
bool initLogging(const QString& loggingApplication);

/**
 * @brief myMessageHandler custom message handler for Qt logging
 * @param type Message Type
 * @param context Message context contain line number, function and file info.
 * @param msg Message to be logged.
 */
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg);

};

