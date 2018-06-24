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
  \author  Joydeep Biswas, (C) 2013
*/
//========================================================================

#include "field_default_constants.h"

#include <math.h>

namespace FieldConstants {

const std::size_t kNumFieldLines = 23;
const FieldLine kFieldLines[kNumFieldLines] = {
    FieldLine("TopTouchLine", -110, 90, 110, 90, 1),
    FieldLine("BottomTouchLine", -110, -90, 110, -90, 1),
    FieldLine("LeftGoalLine", -110, -90, -110, 90, 1),
    FieldLine("RightGoalLine", 110, -90, 110, 90, 1),
    FieldLine("HalfwayLine", 0, -90, 0, 90, 1),
    FieldLine("CenterLine", -110, 0, 110, 0, 1),
    FieldLine("LeftPenaltyStretchL", -110, 25, -95, 25, 1),
    FieldLine("LeftPenaltyStretchM", -95, -25, -95, 25, 1),
    FieldLine("LeftPenaltyStretchR", -110, -25, -95, -25, 1),
    FieldLine("LeftOPenaltyStretchL", -110, 40, -75, 40, 1),
    FieldLine("LeftOPenaltyStretchM", -75, -40, -75, 40, 1),
    FieldLine("LeftOPenaltyStretchR", -110, -40, -75, -40, 1),
    FieldLine("RightPenaltyStretchL", 95, 25, 110, 25, 1),
    FieldLine("RightPenaltyStretchM", 95, -25, 95, 25, 1),
    FieldLine("RightPenaltyStretchR", 95, -25, 110, -25, 1),
    FieldLine("RightOPenaltyStretchL", 75, 40, 110, 40, 1),
    FieldLine("RightOPenaltyStretchM", 75, -40, 75, 40, 1),
    FieldLine("RightOPenaltyStretchR", 75, -40, 110, -40, 1),
    FieldLine("RightPenaltyStretch", 3500, -250, 3500, 250, 1),
    FieldLine("RightPenaltyStretch", 3500, -250, 3500, 250, 1),
    FieldLine("RightPenaltyStretch", 3500, -250, 3500, 250, 1),
    FieldLine("RightPenaltyStretch", 3500, -250, 3500, 250, 1),
    FieldLine("RightPenaltyStretch", 3500, -250, 3500, 250, 1),
};

const std::size_t kNumFieldArcs = 3;
const FieldCircularArc kFieldArcs[kNumFieldArcs] = {
    FieldCircularArc("LeftFieldPenaltyArc",
    -4495, 250, 995, 0, 0.5 * M_PI, 1),
    FieldCircularArc("RightFieldPenaltyArc",
    4495, -250, 995, M_PI, 1.5 * M_PI, 1),
    FieldCircularArc("CenterCircle",
    0, 0, 25, 0, 2.0 * M_PI, 1),
};


}  // namespace FieldConstantsRoboCup2012
