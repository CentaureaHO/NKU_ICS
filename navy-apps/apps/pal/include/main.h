/* -*- mode: c; tab-width: 4; c-basic-offset: 3; c-file-style: "linux" -*- */
//
// Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>.
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef MAIN_H
#define MAIN_H

#include "_common.h"
#include "battle.h"
#include "ending.h"
#include "fight.h"
#include "font.h"
#include "game.h"
#include "global.h"
#include "input.h"
#include "itemmenu.h"
#include "magicmenu.h"
#include "map.h"
#include "palcommon.h"
#include "palette.h"
#include "play.h"
#include "res.h"
#include "rixplay.h"
#include "rngplay.h"
#include "scene.h"
#include "script.h"
#include "sound.h"
#include "text.h"
#include "ui.h"
#include "uibattle.h"
#include "uigame.h"
#include "util.h"
#include "video.h"

#ifdef PAL_HAS_NATIVEMIDI
#include "midi.h"
#endif

VOID PAL_Shutdown(VOID);

#endif
