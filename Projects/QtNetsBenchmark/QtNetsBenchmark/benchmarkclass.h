/**
 ** @file benchmarkclass.h
 ** @author Angelo Prudentino
 ** @date 2017-02-18
 **
 ** Copyright 2017 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
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

#ifndef BENCHMARKCLASS_H
#define BENCHMARKCLASS_H

#include <QTime>
#include "qntypes.h"
#include "qnpersistencetypes.h"
#include "qnstyletypes.h"

class BenchmarkClass : public QObject{
    Q_OBJECT
private:
    int NUM_CPT;
    int NUM_NOISYMAX;
    int NUM_NOISYADDER;
    int NUM_DECISION;
    int NUM_DETER;
    int NUM_UTIL;
    int NUM_MAU;
    int NUM_MODELS;
    int NUM_TEXTBOXES;
    int NUM_NODES;
    int NUM_EDGES;
    int NUM_STYLABLES;
    int NUM_CLASSES;

    QTime timer;
    ScopedQNNetwork net;
    QNNetworkModelPtr netModel;
    QNPersistenceManagerPtr persistMgr;
    QNStyleManagerPtr styleMgr;

    QStringList nodeIds;
    QStringList edgeIds;
    QStringList modelIds;

    void init();    
    void initClasses();

    void logErr(const QString& aMsg);
    void printResults(const QString& aMsg);
    QString formatTime(const int aMs);

public:
    BenchmarkClass();
    ~BenchmarkClass();

    void benchmark();
};

#endif // BENCHMARKCLASS_H
