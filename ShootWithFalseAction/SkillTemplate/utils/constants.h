/*
 * TITLE:        constants.h
 *
 * PURPOSE:      This is file contains the major system constants
 *               
 * WRITTEN BY:   Michael Bowling, James R Bruce, Brett Browning
 */
/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include "vector.h"

const int MAX_ROBOTS = 12;
const int MAX_TEAM_ROBOTS = 6;
const int MAX_ROBOT_SIZE = 9;
const double BALL_SIZE = 5;
const int SEGMENT_NUM = 4;
//==== Field Dimensions (cm) =========================================//

// diagonal is of 2800 x 2300 is 3623.53
const double FIELD_LENGTH = 605;
const double FIELD_WIDTH = 405;
const double FIELD_LENGTH_H = (FIELD_LENGTH/2);
const double FIELD_WIDTH_H = (FIELD_WIDTH/2);

const double GOAL_WIDTH = 70;
const double GOAL_DEPTH = 18;
const double DEFENSE_WIDTH = 205;
const double DEFENSE_DEPTH = 85;
const double WALL_WIDTH = 1;

const double PENALTY_BUFF = 4;
const double PENALTY_AREA_R = 80;
const double PENALTY_AREA_L = 35;
const double PENALTY_KICKER_L = 75;

const double GOAL_WIDTH_H = (GOAL_WIDTH   /2);
const double GOAL_DEPTH_H = (GOAL_DEPTH   /2);
const double DEFENSE_WIDTH_H = (DEFENSE_WIDTH/2);
const double DEFENSE_DEPTH_H = (DEFENSE_DEPTH/2);
const double CENTER_CIRCLE_RADIUS = 50;
const double PENALTY_BISECTOR = (GOAL_WIDTH/SEGMENT_NUM);
const double ROBOT_HEAD = 7.5;


namespace RuleParam{
	const double Stop_Dist = 50;
	const double Free_Kick_Away_Dist = 50;
	const double Goalie_Away_Goal_Dist = 15;
}


const double OUTER_PENALTY_AREA_R = PENALTY_AREA_R + MAX_ROBOT_SIZE * 4 + RuleParam::Free_Kick_Away_Dist + PENALTY_BUFF;
#endif
