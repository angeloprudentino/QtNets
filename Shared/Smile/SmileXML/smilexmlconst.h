/**
 ** @file smilexmlconst.h
 ** @author Angelo Prudentino
 ** @date 2016-10-30
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

#ifndef SMILEXMLCONST_H
#define SMILEXMLCONST_H

#define COLOR_REGEXP "[0-9|a-f|A-F]{6}"
#define CLASSES_TAG "classes"
#define LABEL_TAG "label"
#define LABEL_LEN_TAG "length"
#define LABEL_PROP_TAG "prop"
#define SHOW_TAG "show"

#define SMILE_TAG "smile"
#define SMILE_ID_TAG "id"
#define SMILE_ID_VERSION "version"
#define SMILE_ID_NUMSAMPLES "numsamples"
#define SMILE_PROPERTIES "properties"
#define SMILE_ALGPARAMS "algparams"
#define SMILE_OBSCOST "observationcost"
#define SMILE_EXTENSIONS "extensions"
#define SMILE_PROPERTIES_CHILD "property"
#define SMILE_EPIS "epis"
#define SMILE_EPIS_PROPLEN "proplen"
#define SMILE_EPIS_NUMSTATES1 "numstates1"
#define SMILE_EPIS_NUMSTATES2 "numstates2"
#define SMILE_EPIS_NUMSTATES3 "numstates3"
#define SMILE_EPIS_EPS1 "eps1"
#define SMILE_EPIS_EPS2 "eps2"
#define SMILE_EPIS_EPS3 "eps3"
#define SMILE_EPIS_EPS4 "eps4"
#define SMILE_OBSCOST_NODE "node"
#define SMILE_OBSCOST_NODE_PARENTS "parents"
#define SMILE_OBSCOST_NODE_COST "cost"

#define SMILE_NODES "nodes"
#define SMILE_NODE_TYPE "node_type"
#define SMILE_NODES_TARGET "target"
#define SMILE_NODES_RANKED "ranked"
#define SMILE_NODES_MANDATORY "mandatory"
#define SMILE_NODES_DIAGTYPE "diagtype"
#define SMILE_NODES_CPT "cpt"
#define SMILE_NODES_NOISYMAX "noisymax"
#define SMILE_NODES_NOISYADDER "noisyadder"
#define SMILE_NODES_DETERMINISTIC "deterministic"
#define SMILE_NODES_DECISION "decision"
#define SMILE_NODES_UTILITY "utility"
#define SMILE_NODES_MAU "mau"
#define SMILE_NODE_STATE "state"
#define SMILE_NODE_STATE_ID "id"
#define SMILE_NODE_STATE_LABEL "label"
#define SMILE_NODE_STATE_TARGET "target"
#define SMILE_NODE_STATE_DEFAULT "default"
#define SMILE_NODE_STATE_FAULT "fault"
#define SMILE_NODE_STATE_LIST "state_list"
#define SMILE_NODE_PARENTS "parents"
#define SMILE_NODE_PROB "probabilities"
#define SMILE_NODE_PROP "property"
#define SMILE_NODE_STRENGHTS "strengths"
#define SMILE_NODE_PARAMETERS "parameters"
#define SMILE_NODE_DSTATES "dstates"
#define SMILE_NODE_WEIGHTS "weights"
#define SMILE_NODE_RESULTINGSTATES "resultingstates"
#define SMILE_NODE_UTILITIES "utilities"

#define SMILE_GENIE "genie"
#define SMILE_GENIE_NAME "name"
#define SMILE_GENIE_VERSION "version"
#define SMILE_GENIE_FAULTNAMEFORMAT "faultnameformat"
#define SMILE_GENIE_COMMENT "comment"
#define SMILE_GENIE_DIAGAUTOFORMAT "diagautoformat"
#define SMILE_GENIE_NODE "node"
#define SMILE_GENIE_NODE_STATE "state"
#define SMILE_GENIE_NODE_FAULTNAME "faultname"
#define SMILE_GENIE_SUBMODEL "submodel"
#define SMILE_GENIE_TEXTBOX "textbox"
#define SMILE_GENIE_ARCCOMMENT "arccomment"
#define SMILE_GENIE_DAF_TARGET "target"
#define SMILE_GENIE_DAF_TARGETNR "targetnr"
#define SMILE_GENIE_DAF_OBSERVATION "observation"
#define SMILE_GENIE_DAF_OBSERVATIONNR "observationnr"
#define SMILE_GENIE_DAF_AUXILIARY "auxiliary"
#define SMILE_GENIE_PARENT "parent"
#define SMILE_GENIE_CHILD "child"
#define SMILE_GENIE_COST "cost"
#define SMILE_GENIE_SUBMODEL_WIN "window"
#define SMILE_GENIE_INTERIOR "interior"
#define SMILE_GENIE_COLOR "color"
#define SMILE_GENIE_OUTLINE "outline"
#define SMILE_GENIE_WIDTH "width"
#define SMILE_GENIE_POS "position"
#define SMILE_GENIE_FONT "font"
#define SMILE_GENIE_SIZE "size"
#define SMILE_GENIE_BOLD "bold"
#define SMILE_GENIE_ITALIC "italic"
#define SMILE_GENIE_ALIGN "align"
#define SMILE_GENIE_CAPTION "caption"
#define SMILE_GENIE_FIX "fix"
#define SMILE_GENIE_LINK "link"
#define SMILE_GENIE_LINK_TITLE "title"
#define SMILE_GENIE_LINK_PATH "path"
#define SMILE_GENIE_BARCHART "barchart"
#define SMILE_GENIE_BARCHART_ACTIVE "active"
#define SMILE_GENIE_BARCHART_W "width"
#define SMILE_GENIE_BARCHART_H "height"
#define SMILE_GENIE_QUESTION "question"
#define SMILE_GENIE_COL "col"
#define SMILE_GENIE_ROW "row"
#define SMILE_GENIE_DEFCOMMENT "defcomment"
#define SMILE_GENIE_COSTCOMMENT "costcomment"

#endif // SMILEXMLCONST_H
