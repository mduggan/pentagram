/*
 *  Copyright (C) 2003  The Pentagram Team
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
#include "SliderGump.h"

#include "GameData.h"
#include "ShapeFrame.h"
#include "GumpShapeFlex.h"
#include "Shape.h"
#include "SlidingWidget.h"
#include "Font.h"
#include "RenderSurface.h"
#include "FontShapeFlex.h"
#include "ButtonWidget.h"
#include "UCProcess.h"
#include "Kernel.h"

#include "IDataSource.h"
#include "ODataSource.h"

DEFINE_RUNTIME_CLASSTYPE_CODE(SliderGump,ModalGump);

SliderGump::SliderGump() : ModalGump()
{

}


SliderGump::SliderGump(int x, int y, sint16 min_, sint16 max_,
					   sint16 value_, sint16 delta_)
	: ModalGump(x, y, 5, 5), min(min_), max(max_), delta(delta_), value(value_)
{
	slidergid = okbuttongid = usecodeNotifyPID = 0;
}

SliderGump::~SliderGump()
{

}

/*
  41:0 = slider gump
  42:0,1 = ok button
  43:0,1 = left button
  44:0,1 = right button
  45:0 = slider
*/

static const int gumpshape = 41;
static const int gumpframe = 0;
static const int slidershape = 45;
static const int sliderframe = 0;
static const int slidery = 17;
static const int sliderminx = 55;
static const int slidermaxx = 130;
static const int labelx = 161;
static const int labely = 26;
static const int labelfont = 0;

int SliderGump::getSliderPos()
{
	return sliderminx + (value-min)*(slidermaxx-sliderminx)/(max-min);
}

void SliderGump::setValue(int sliderx)
{
	int val = (sliderx-sliderminx)*(max-min)/(slidermaxx-sliderminx) + min;
	if (val < min) val = min;
	if (val > max) val = max;
	value = min + delta*((sint16)(val/delta));
}

void SliderGump::drawText(RenderSurface* surf)
{
	Pentagram::Font *font = GameData::get_instance()->
		getFonts()->getFont(labelfont);
	char buf[10]; // more than enough for a sint16
	sprintf(buf, "%d", value);
	surf->PrintText(font, buf, labelx, labely);
}

void SliderGump::PaintThis(RenderSurface* surf, sint32 lerp_factor)
{
	Gump::PaintThis(surf, lerp_factor);

	drawText(surf);
}


void SliderGump::InitGump()
{
	ModalGump::InitGump();

	shape = GameData::get_instance()->getGumps()->getShape(gumpshape);
	ShapeFrame* sf = shape->getFrame(0);
	assert(sf);

	dims.w = sf->width;
	dims.h = sf->height;

	Shape* childshape = GameData::get_instance()->
		getGumps()->getShape(slidershape);

	// Create the SlidingWidget
	Gump *widget = new SlidingWidget(getSliderPos(),slidery,
									 childshape,sliderframe);
	slidergid = widget->getObjId();
	widget->InitGump();
	AddChild(widget);

	Shape* okshape = GameData::get_instance()->getGumps()->getShape(42);

//	widget = new ButtonWidget(20, 17, "OK", labelfont);
	widget = new ButtonWidget(14, 17, okshape, 0, okshape, 1);
	widget->InitGump();
	okbuttongid = widget->getObjId();
	AddChild(widget);
}

void SliderGump::ChildNotify(Gump *child, uint32 message)
{
	if (child->getObjId() == okbuttongid &&
		message == ButtonWidget::BUTTON_CLICK)
	{
		Close();
	}
}


void SliderGump::Close(bool no_del)
{
	process_result = value;

	if (usecodeNotifyPID) {
		UCProcess* ucp = p_dynamic_cast<UCProcess*>(Kernel::get_instance()->getProcess(usecodeNotifyPID));
		assert(ucp);
		ucp->setReturnValue(value);
		ucp->wakeUp(value);
	}

	ModalGump::Close(no_del);
}

bool SliderGump::StartDraggingChild(Gump* gump, int mx, int my)
{
	if (gump->getObjId() == slidergid) {
		gump->ParentToGump(mx, my);
		sliderMouseOffset = mx;
		return true;
	}

	return false;
}

void SliderGump::DraggingChild(Gump* gump, int mx, int my)
{
	if (gump->getObjId() == slidergid) {
		setValue(mx-sliderMouseOffset);
		gump->Move(getSliderPos(), slidery);
	}
}

void SliderGump::StopDraggingChild(Gump* gump)
{
}

void SliderGump::setUsecodeNotify(UCProcess* ucp)
{
	assert(ucp);
	usecodeNotifyPID = ucp->getPid();
}

void SliderGump::saveData(ODataSource* ods)
{
	ods->write2(1); //version
	ModalGump::saveData(ods);

	ods->write2(min);
	ods->write2(max);
	ods->write2(value);
	ods->write2(delta);
	ods->write2(slidergid);
	ods->write2(okbuttongid);
	ods->write2(usecodeNotifyPID);
}

bool SliderGump::loadData(IDataSource* ids)
{
	uint16 version = ids->read2();
	if (version != 1) return false;
	if (!ModalGump::loadData(ids)) return false;

	min = ids->read2();
	max = ids->read2();
	value = ids->read2();
	delta = ids->read2();
	slidergid = ids->read2();
	okbuttongid = ids->read2();
	usecodeNotifyPID = ids->read2();

	return true;
}
