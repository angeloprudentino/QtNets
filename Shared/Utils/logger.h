/**
 ** @file logger.h
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

#ifndef LOG_H
#define LOG_H

#include <QMutex>

/**
 * @brief The Logger class manages all the logging operations inside the library
 * @note this class is not copyable
 */
class Logger {
private:
    Q_DISABLE_COPY(Logger)

    /**
     * @brief mutex used to avoid log messages interlacing
     */
    QMutex mutex;

    /**
     * @brief the name of the log file
     */
    QString fileName;

    /**
     * @brief the name of the class that owns the logger object
     */
    QString className;

    /**
     * @brief logToFile is responsible to actually write logs permanently on disk
     * @param aFuncName the name of the caller function
     * @param aMsg the message to log
     */
    void logToFile(const QString& aFuncName, const QString& aMsg);

public:
    /**
     * @brief Logger constructor
     * @param aFileName the name of the log file
     * @param aClassName the name of the class that owns the logger object
     */
    Logger(const QString& aFileName, const QString& aClassName);

    /**
     * @brief debug level logging function
     * @param aFuncName the name of the caller function
     * @param aMsg the message to log
     *
     * @see logToFile
     */
    void d(const QString& aFuncName, const QString& aMsg);

    /**
     * @brief warning level logging function
     * @param aFuncName the name of the caller function
     * @param aMsg the message to log
     *
     * @see logToFile
     */
    void w(const QString& aFuncName, const QString& aMsg);

    /**
     * @brief error level logging function
     * @param aFuncName the name of the caller function
     * @param aMsg the message to log
     *
     * @see logToFile
     */
    void e(const QString& aFuncName, const QString& aMsg);
};

/**
 * @brief LoggerPtr represents a pointer to a logger instance
 * @see Logger
 */
typedef Logger* LoggerPtr;

#endif // LOG_H
