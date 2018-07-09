//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    field_default_constants.h
  \brief   Definition of field dimensions
  \author  Stefan Zickler / Tim Laue, (C) 2009
*/
//========================================================================

#include <vector>
#include "geometry.h"
#include "field.h"

#ifndef FIELD_DEFAULT_CONSTANTS_H
#define FIELD_DEFAULT_CONSTANTS_H

namespace FieldConstants {

const double kFieldLength = 220.0;
const double kFieldWidth = 200.0;
const double kGoalWidth = 40.0;
const double kGoalDepth = 15.0;
const double kBoundaryWidth = 50.0;
const double thinkness = 0.3;
extern const std::size_t kNumFieldLines;
extern const FieldLine kFieldLines[];

extern const std::size_t kNumFieldArcs;
extern const FieldCircularArc kFieldArcs[];

extern const std::size_t kNumFieldTriangles;
extern const FieldTriangle kFieldTris[];

}
#endif // FIELD_H
