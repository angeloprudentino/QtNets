/**
 ** @file basestyletypes.h
 ** @author Angelo Prudentino
 ** @date 2016-12-29
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

#ifndef BASESTYLETYPES_H
#define BASESTYLETYPES_H

#include <QObject>
#include <QMetaType>

/**
 * @brief Enumerates all possible shapes of a node item
 */
enum BSNodeShape{
    square /// square
    ,roundedSquare /// rounded corners square
    ,rectagle /// rectangle
    ,roundedRect /// rounded corners rectangle
    ,rhombus /// rhombhus
    ,triangle /// triangle
    ,reverseTriangle /// reverse triangle
    ,ellipse /// ellipse
    ,circle /// circle
    ,hexagon /// hexagon
    ,image /// custom image
};
Q_DECLARE_METATYPE(BSNodeShape)

/**
 * @brief Enumerates all possible shapes of an arrow
 */
enum BSArrowStyle{
    none /// no arrow
    ,empty /// empty arrow
    ,full /// standard arrow
    ,circular /// circular arrow
    ,diamond /// diamond arrow
};
Q_DECLARE_METATYPE(BSArrowStyle)

/**
 * @brief Enumerates all possible text's horizontal positions
 */
enum BSTextAlign{
    textLeft /// left aligned
    ,textCenter /// center aligned
    ,textRight /// right aligned
};
Q_DECLARE_METATYPE(BSTextAlign)

/**
 * @brief Enumerates all possible coloring styles
 * @see Qt::BrushStyle
 */
enum BSBrushStyle{
    transparent /** no color */ = Qt::NoBrush
    ,solid /** solid */ = Qt::SolidPattern
    ,dense1 /** dense maximum level */ = Qt::Dense1Pattern
    ,dense2 /** dense */ = Qt::Dense2Pattern
    ,dense3 /** dense */ = Qt::Dense3Pattern
    ,dense4 /** dense */ = Qt::Dense4Pattern
    ,dense5 /** dense */ = Qt::Dense5Pattern
    ,dense6 /** dense */ = Qt::Dense6Pattern
    ,dense7 /** dense minimum level */ = Qt::Dense7Pattern
    ,horizontal /** horizontal lines */ = Qt::HorPattern
    ,vertiacal /** vertical lines */ = Qt::VerPattern
    ,cross /** crossing lines */ = Qt::CrossPattern
    ,backDiagonal /** backword diagonal lines */ = Qt::BDiagPattern
    ,forwardDiagonal /** forward diagonale lines */ = Qt::FDiagPattern
    ,crossDiagonal /** diagonal crossing lines */ = Qt::DiagCrossPattern
    ,linearGradient /** linear gradient */ = Qt::LinearGradientPattern
    ,radialGradient /** radial gradient */ = Qt::RadialGradientPattern
};
Q_DECLARE_METATYPE(BSBrushStyle)

/**
 * @brief Enumerates all possible line styles
 * @see Qt::PenStyle
 */
enum BSLineStyle{
    noLine /** no line */ = Qt::NoPen
    ,solidLine /** solid line */ = Qt::SolidLine
    ,dashLine /** dashed line */ = Qt::DashLine
    ,dotLine /** dotted line */ = Qt::DotLine
    ,dashDotLine /** dash - dot line */ = Qt::DashDotLine
    ,dashDotDotLine /** dash - dot - dot line */ = Qt::DashDotDotLine
};
Q_DECLARE_METATYPE(BSLineStyle)

/**
 * @brief Enumerates all possible pen cap styles
 * @see Qt::PenCapStyle
 */
enum BSCapStyle{
    flatCap /** flat cap */ = Qt::FlatCap
    ,squareCap /** square cap */ = Qt::SquareCap
    ,roundCap /** rounded cap */ = Qt::RoundCap
};
Q_DECLARE_METATYPE(BSCapStyle)

/**
 * @brief Enumerates all possible join styles
 * @see Qt::PenJoinStyle
 */
enum BSJoinStyle{
    miterJoin /** miter join */ = Qt::MiterJoin
    ,bevelJoin /** bevel join */ = Qt::BevelJoin
    ,roundJoin /** round join */ = Qt::RoundJoin
};
Q_DECLARE_METATYPE(BSJoinStyle)

#endif // BASESTYLETYPES_H
