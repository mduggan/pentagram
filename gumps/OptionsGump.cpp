/*
 *  Copyright (C) 2004-2005  The Pentagram Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "pent_include.h"
#include "OptionsGump.h"

#include "RenderSurface.h"
#include "DesktopGump.h"
#include "ButtonWidget.h"
#include "TextWidget.h"
#include "ControlsGump.h"
#include "PagedGump.h"

#include "IDataSource.h"
#include "ODataSource.h"

#include <SDL.h>

static const int font = 0;

DEFINE_RUNTIME_CLASSTYPE_CODE(OptionsGump,Gump);

OptionsGump::OptionsGump(): Gump(0, 0, 5, 5)
{
}

OptionsGump::~OptionsGump()
{
}

void OptionsGump::InitGump(Gump* newparent, bool take_focus)
{
	Gump::InitGump(newparent, take_focus);

	dims.w = 220;
	dims.h = 120;

	int x = dims.w / 2 + 14;
	int y = 4;
	Gump * widget;

#if 0
	widget = new ButtonWidget(x, y, "1. Video", true, font, 0x804000B0);
	widget->InitGump(this);
	widget->SetIndex(1);
#endif
	y+= 14;

#if 0
	widget = new ButtonWidget(x, y, "2. Audio", true, font, 0x804000B0);
	widget->InitGump(this);
	widget->SetIndex(2);
#endif
	y+= 14;

	widget = new ButtonWidget(x, y, "3. Controls", true, font, 0x804000B0);
	widget->InitGump(this);
	widget->SetIndex(3);
	y+= 14;

#if 0
	widget = new ButtonWidget(x, y, "4. Gameplay", true, font, 0x804000B0);
	widget->InitGump(this);
	widget->SetIndex(4);
#endif
	y+= 14;
}

void OptionsGump::ChildNotify(Gump *child, uint32 message)
{
	if (message == ButtonWidget::BUTTON_CLICK)
	{
		selectEntry(child->GetIndex());
	}
}

void OptionsGump::PaintThis(RenderSurface* surf, sint32 lerp_factor, bool scaled)
{
	Gump::PaintThis(surf, lerp_factor, scaled);
}

static const int gumpShape = 35;

bool OptionsGump::OnKeyDown(int key, int mod)
{
	if (key == SDLK_ESCAPE)
	{
		parent->Close();
	}
	else if (key >= SDLK_1 && key <=SDLK_9)
	{
		// Minor hack.
		selectEntry(key - SDLK_1 + 1);
		return true;
	}
	return true;
}

void OptionsGump::selectEntry(int entry)
{
	switch (entry)
	{
	case 1:
	{	// Video
	} break;
	case 2:
	{	// Audio
	} break;
	case 3:
	{	// Controls
		PagedGump * gump = new PagedGump(34, -38, 3, gumpShape);
		gump->InitGump(this);

		ControlsGump::showEngineMenu(gump);
		ControlsGump::showU8Menu(gump);

		// The parent to this gump is actually  a PagedGump,
		// and this gump does not cover the entire parent.
		gump->setRelativePosition(CENTER);
	} break;	
	case 4:
	{	// Gameplay
	} break;
	case 5:
	{
	} break;
	case 6:
	{
	} break;
	default:
		break;
	}
}

bool OptionsGump::loadData(IDataSource* ids)
{
	return true;
}

void OptionsGump::saveData(ODataSource* ods)
{
}

