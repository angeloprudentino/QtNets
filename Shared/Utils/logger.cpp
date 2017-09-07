/**
 ** @file logger.cpp
 ** @author Angelo Prudentino
 ** @date 2016-10-26
 **
 ** Copyright 2016 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include "logger.h"

#define LOG_PATH "Log"

const QString formatDateTime(){
    QDateTime now = QDateTime::currentDateTime();
    return now.toString("yyyy-MM-dd hh:mm:ss:zzz");
}

void Logger::logToFile(const QString &aFuncName, const QString &aMsg){
    QDir logDir(LOG_PATH);
    if(!logDir.exists())
        logDir.mkpath(".");

    QString logFile = QString("%1/%2-%3_log.txt").arg(LOG_PATH).arg(this->fileName).arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
    QString toLog = QString("%1 $ %2::%3: %4").arg(formatDateTime()).arg(this->className).arg(aFuncName).arg(aMsg);

    QFile outFile(logFile);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << toLog << endl;
    outFile.close();
}

Logger::Logger(const QString& aFileName, const QString& aClassName){
    this->fileName = aFileName;
    this->className = aClassName;
}

void Logger::d(const QString &aFuncName, const QString &aMsg){
#ifdef QT_DEBUG
    this->logToFile(aFuncName, aMsg);
#endif
}

void Logger::w(const QString &aFuncName, const QString &aMsg){
    QMutexLocker(&this->mutex);

    this->logToFile(aFuncName, "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
    this->logToFile(aFuncName, "w  " + aMsg);
    this->logToFile(aFuncName, "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
}

void Logger::e(const QString &aFuncName, const QString &aMsg){
    QMutexLocker(&this->mutex);

    this->logToFile(aFuncName, "************************************************");
    this->logToFile(aFuncName, "************************************************");
    this->logToFile(aFuncName, "** ");
    this->logToFile(aFuncName, "**  " + aMsg);
    this->logToFile(aFuncName, "** ");
    this->logToFile(aFuncName, "************************************************");
    this->logToFile(aFuncName, "************************************************");
}
