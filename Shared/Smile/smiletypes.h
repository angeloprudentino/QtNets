/**
 ** @file smiletypes.h
 ** @author Angelo Prudentino
 ** @date 2016-11-09
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

#ifndef SMILETYPES_H
#define SMILETYPES_H

#include <QScopedPointer>
#include <QList>


class SmileEpis;
class SmileObscostNode;
class GenieDAF;
class GenieLink;
class GenieLinkList;


/**
 * @brief Represents a pointer to an epis object
 * @see SmileEpis
 */
typedef SmileEpis* SmileEpisPtr;

/**
 * @brief Represents a unique pointer to an epis object
 * @see SmileEpis
 */
typedef QScopedPointer<SmileEpis> ScopedSmileEpis;

/**
 * @brief Represents a list of observationcost's node instances
 * @see SmileObscostNode
 */
typedef QList<SmileObscostNode> SmileObscostNodeList;

/**
 * @brief Represents a pointer to a list of observationcost's node instances
 * @see SmileObscostNodeList
 */
typedef SmileObscostNodeList* SmileObscostNodeListPtr;

/**
 * @brief Represents a unique pointer to a list of observationcost's node instances
 * @see SmileObscostNodeList
 */
typedef QScopedPointer<SmileObscostNodeList> ScopedSmileObscostNodeList;

/**
 * @brief Represents a pointer to a genie's diagautoformat instance
 * @see GenieDAF
 */
typedef GenieDAF* GenieDAFPtr;

/**
 * @brief Represents a unique pointer to a genie's diagautoformat instance
 * @see GenieDAF
 */
typedef QScopedPointer<GenieDAF> ScopedGenieDAF;

/**
 * @brief Represents a pointer to a genie link object
 * @see GenieLink
 */
typedef GenieLink* GenieLinkPtr;

/**
 * @brief Represents a pointer to a list of genie link pointers
 * @see GenieLinkList
 */
typedef GenieLinkList* GenieLinkListPtr;

/**
 * @brief Represents a unique pointer to a list of genie link pointers
 * @see GenieLinkList
 */
typedef QScopedPointer<GenieLinkList> ScopedGenieLinkList;

#endif // SMILETYPES_H
