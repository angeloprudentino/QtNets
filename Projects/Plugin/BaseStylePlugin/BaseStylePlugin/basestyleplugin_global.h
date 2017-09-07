/**
 ** @file basestyleplugin_global.h
 ** @author Angelo Prudentino
 ** @date 2017-01-11
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

#ifndef BASESTYLEPLUGIN_GLOBAL_H
#define BASESTYLEPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASESTYLEPLUGIN_LIBRARY)
#  define BASESTYLEPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASESTYLEPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BASESTYLEPLUGIN_GLOBAL_H
