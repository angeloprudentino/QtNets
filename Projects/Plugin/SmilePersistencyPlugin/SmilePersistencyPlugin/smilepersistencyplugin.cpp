/**
 ** @file smilepersistencyplugin.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-09
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

#include <QMetaType>
#include "smilepersistencyplugin.h"
#include "smilexmlparser.h"
#include "smilexmlwriter.h"
#include "smilealgparams.h"
#include "smilegenieext.h"
#include "smilenodegenieext.h"
#include "smileobservationcost.h"
#include "smilenode.h"


SmilePersistencyPlugin::SmilePersistencyPlugin(){
    this->supportedPattern.setPatternSyntax(QRegExp::Wildcard);
    this->supportedPattern.setPattern("*.xdsl");

    //register cmparators
    QMetaType::registerComparators<SmileObservationcost>();
    QMetaType::registerComparators<SmileObscostNode>();
    QMetaType::registerComparators<GenieBarchart>();
    QMetaType::registerComparators<GenieNodeState>();
    QMetaType::registerComparators<GenieNodeStateList>();
    QMetaType::registerComparators<GenieTableCommentList>();
    QMetaType::registerComparators<GenieLink>();
    QMetaType::registerComparators<GenieLinkList>();
    QMetaType::registerComparators<GenieTableComment>();
    QMetaType::registerComparators<SmileNodeStateList>();
    QMetaType::registerComparators<SmileNodeState>();
    QMetaType::registerComparators<SmileGenieExt>();
    QMetaType::registerComparators<GenieDAF>();
    QMetaType::registerComparators<SmileEpis>();
    QMetaType::registerComparators<SmileAlgparams>();
}

QRegExpList SmilePersistencyPlugin::supportedFormats(){
    QRegExpList result;
    result.push_back(this->supportedPattern);
    return result;
}

bool SmilePersistencyPlugin::isSupported(const QString& aFile){
    return this->supportedPattern.exactMatch(aFile);
}

QNError SmilePersistencyPlugin::load(const QString& aFile, ScopedQNNetwork& aNet){
    SmileXmlParser parser(aFile);
    return parser.parse(aNet);
}

QNError SmilePersistencyPlugin::store(const QString& aFile, const ScopedQNNetwork& aNet){
    SmileXmlWriter writer(aFile);
    return writer.write(aNet);
}
