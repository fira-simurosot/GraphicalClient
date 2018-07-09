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


const std::size_t kNumFieldLines = 24 + 16 - 1;
const FieldLine kFieldLines[kNumFieldLines] = {
    FieldLine("TopTouchLine", -110, 90, 110, 90, thinkness),
    FieldLine("BottomTouchLine", -110, -90, 110, -90, thinkness),
    FieldLine("LeftGoalLine", -110, -90, -110, 90, thinkness),
    FieldLine("RightGoalLine", 110, -90, 110, 90, thinkness),
    FieldLine("HalfwayLine", 0, -90, 0, 90, thinkness),
//    FieldLine("CenterLine", -110, 0, 110, 0, 1),

    FieldLine("LeftPenaltyStretchU", -110, 25, -95, 25, thinkness),
    FieldLine("LeftPenaltyStretchM", -95, -25, -95, 25, thinkness),
    FieldLine("LeftPenaltyStretchB", -110, -25, -95, -25, thinkness),
    FieldLine("LeftOPenaltyStretchU", -110, 40, -75, 40, thinkness),
    FieldLine("LeftOPenaltyStretchM", -75, -40, -75, 40, thinkness),
    FieldLine("LeftOPenaltyStretchB", -110, -40, -75, -40, thinkness),

    FieldLine("RightPenaltyStretchU", 95, 25, 110, 25, thinkness),
    FieldLine("RightPenaltyStretchM", 95, -25, 95, 25, thinkness),
    FieldLine("RightPenaltyStretchB", 95, -25, 110, -25, thinkness),
    FieldLine("RightOPenaltyStretchU", 75, 40, 110, 40, thinkness),
    FieldLine("RightOPenaltyStretchM", 75, -40, 75, 40, thinkness),
    FieldLine("RightOPenaltyStretchB", 75, -40, 110, -40, thinkness),

    FieldLine("LeftGoalU", -125, 20, -110, 20, thinkness),
    FieldLine("LeftGoalM", -125, -20, -125, 20, thinkness),
    FieldLine("LeftGoalB", -125, -20, -110, -20, thinkness),
    FieldLine("RightGoalU", 110, 20, 125, 20, thinkness),
    FieldLine("RightGoalM", 125, -20, 125, 20, thinkness),
    FieldLine("RightGoalB", 110, -20, 125, -20, thinkness),

    FieldLine("TopLeftLeftH", -82, 60, -80, 60, 0.5),
    FieldLine("TopLeftLeftV", -80, 61, -80, 59, 0.5),
    FieldLine("TopLeftRightH", -30, 60, -28, 60, 0.5),
    FieldLine("TopLeftRightV", -30, 61, -30, 59, 0.5),
    FieldLine("TopRightLeftH", 28, 60, 30, 60, 0.5),
    FieldLine("TopRightLeftV", 30, 61, 30, 59, 0.5),
    FieldLine("TopRightRightH", 80, 60, 82, 60, 0.5),
    FieldLine("TopRightRightV", 80, 61, 80, 59, 0.5),
    FieldLine("BotLeftLeftH", -82, -60, -80, -60, 0.5),
    FieldLine("BotLeftLeftV", -80, -61, -80, -59, 0.5),
    FieldLine("BotLeftRightH", -30, -60, -28, -60, 0.5),
    FieldLine("BotLeftRightV", -30, -61, -30, -59, 0.5),
    FieldLine("BotRightLeftH", 28, -60, 30, -60, 0.5),
    FieldLine("BotRightLeftV", 30, -61, 30, -59, 0.5),
    FieldLine("BotRightRightH", 80, -60, 82, -60, 0.5),
    FieldLine("BotRightRightV", 80, -61, 80, -59, 0.5)
};

const std::size_t kNumFieldArcs = 3 + 6;
const FieldCircularArc kFieldArcs[kNumFieldArcs] = {
    FieldCircularArc("Left2FieldPenaltyArc", -87, 0, 17, -0.8, 0.8, thinkness),
    FieldCircularArc("RightFieldPenaltyArc", 87, 0, 17, 2.35, 3.95, thinkness),
    FieldCircularArc("CenterCircle", 0, 0, 25, 0, 2.0 * M_PI, thinkness),
    FieldCircularArc("LeftPenaltyPoint", -72.5, 0, 0.5, 0, 2.0 * M_PI, thinkness),
    FieldCircularArc("RightPenaltyPoint", 72.5, 0, 0.5, 0, 2.0 * M_PI, thinkness),
    FieldCircularArc("LeftTopFB", -55, 60, 0.5, 0, 2.0 * M_PI, thinkness),
    FieldCircularArc("LeftBotFB", -55, -60, 0.5, 0, 2.0 * M_PI, thinkness),
    FieldCircularArc("RightTopFB", 55, 60, 0.5, 0, 2.0 * M_PI, thinkness),
    FieldCircularArc("RightBotFB", 55, -60, 0.5, 0, 2.0 * M_PI, thinkness)
};

const std::size_t kNumFieldTriangles = 4;
const FieldTriangle kFieldTris[kNumFieldTriangles] = {
    FieldTriangle("LeftTopTri", -110, 80, -110, 90, -100, 90),
    FieldTriangle("RightTopTri", 100, 90, 110, 90, 110, 80),
    FieldTriangle("LeftBotTri", -110, -80, -110, -90, -100, -90),
    FieldTriangle("RightBotTri", 100, -90, 110, -90, 110, -80)

};


}  // namespace FieldConstantsRoboCup2012
