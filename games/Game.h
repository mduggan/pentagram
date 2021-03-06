/*
Copyright (C) 2004-2005 The Pentagram team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef GAME_H
#define GAME_H

#include "GameInfo.h"
#include "intrinsics.h"
class ODataSource;

class Game {
public:
	Game();
	virtual ~Game();

	static Game* get_instance() { return game; }

	//! load/init game's data files
	virtual bool loadFiles()=0;

	//! initialize new game
	virtual bool startGame()=0;

	//! start initial usecode
	virtual bool startInitialUsecode(const std::string &savegame)=0;

	//! write game-specific savegame info (avatar stats, equipment, ...)
	virtual void writeSaveInfo(ODataSource* ods)=0;

	virtual ProcId playIntroMovie()=0;
	virtual ProcId playEndgameMovie()=0;
	virtual void playCredits()=0;
	virtual void playQuotes()=0;

	static Game* createGame(GameInfo* info);

	INTRINSIC(I_playEndgame);

protected:
	static Game* game;
};


#endif
