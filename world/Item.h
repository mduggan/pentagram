/*
Copyright (C) 2003-2007 The Pentagram team

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

#ifndef ITEM_H
#define ITEM_H

#include "Object.h"
#include "ShapeInfo.h"

#include "intrinsics.h"
#include "Box.h"
class Container;
class ShapeInfo;
class Shape;
class Gump;
class ODataSource;
class GravityProcess;

class Item : public Object
{
	friend class ItemFactory;

public:
	Item();
	virtual ~Item();

	// p_dynamic_cast stuff
	ENABLE_RUNTIME_CLASSTYPE();

	//! Get the Container this Item is in, if any. (0 if not in a Container)
	ObjId getParent() const { return parent; }

	//! Set the parent container of this item.
	void setParent(ObjId p) { parent = p; }

	//! Get the Container this Item is in, if any. (NULL if not in a Container)
	Container *getParentAsContainer() const;

	//! Get the top-most Container this Item is in, or the Item itself if not
	//! in a container
	Item* getTopItem();

	//! Set item location. This strictly sets the location, and does not
	//! even update CurrentMap
	void setLocation(sint32 x, sint32 y, sint32 z); // this only sets the loc.

	//! Move an item. This moves an item to the new location, and updates
	//! CurrentMap and fastArea if necessary.
	void move(sint32 x, sint32 y, sint32 z);

	//! Move an item. This moves an item to a container and  updates
	//! CurrentMap and fastArea if necessary.
	//! \param container The container this item should be placed in
	//! \return true if item was moved, false if failed
	bool moveToContainer(Container *container, bool checkwghtvol=false);

	//! Move an item to the Ethereal Void
	void moveToEtherealVoid();

	//! Move an item out of the Ethereal Void to where it originally was
	void returnFromEtherealVoid();

	//! Check if moving this item is stealing; call AvatarStoleSomething if so
	void movedByPlayer();

	//! Get the location of the top-most container this Item is in, or
	//! this Item's location if not in a container.
	void getLocationAbsolute(sint32& x, sint32& y, sint32& z) const;

	//! Get this Item's location. Note that this does not return
	//! 'usable' coordinates if the Item is contained or equipped.
	inline void getLocation(sint32& x, sint32& y, sint32& z) const;

	//! Get this Item's Z coordinate.
	sint32 getZ() const;

	//! Set this Item's Z coordinate
	void setZ(sint32 z_) { z = z_; }

	//! Get this Item's location in a ContainerGump. Undefined if the Item 
	//! is not in a Container.
	void getGumpLocation(sint32& x, sint32& y) const;

	//! Set the Item's location in a ContainerGump. NOP if the Item
	//! is not in a Container.
	void setGumpLocation(sint32 x, sint32 y);

	//! Randomize the Item's location in a ContainerGump. Effectively
	//! this sets the coordinates to (255,255) and lets the ContainerGump
	//! randomize the position when it is next opened.
	void randomGumpLocation();

	//! Get the world coordinates of the Item's centre. Undefined if the Item
	//! is contained or equipped.
	void getCentre(sint32& x, sint32& y, sint32& z) const;

	//! Get the size of this item's 3D bounding box, in world coordinates.
	inline void getFootpadWorld(sint32& x, sint32& y, sint32& z) const;

	//! Get the size of this item's 3D bounding box, scaled as in the datafiles
	//! (i.e., the dimensions are not in the same unit as world coordinates!)
	inline void getFootpadData(sint32& x, sint32& y, sint32& z) const;

	//! Get the Box this item occupies in the world. Undef if item is contained
	Pentagram::Box getWorldBox() const;

	//! Get flags
	inline uint16 getFlags() const { return flags; }

	//! Set the flags set in the given mask.
	void setFlag(uint32 mask) { flags |= mask; }

	virtual void setFlagRecursively(uint32 mask) { setFlag(mask); }

	//! Clear the flags set in the given mask.
	void clearFlag(uint32 mask) { flags &= ~mask; }

	//! Set extendedflags
	void setExtFlags(uint32 f) { extendedflags = f; }

	//! Get extendedflags
	inline uint32 getExtFlags() const { return extendedflags; }

	//! Set the extendedflags set in the given mask.
	void setExtFlag(uint32 mask) { extendedflags |= mask; }

	//! Clear the extendedflags set in the given mask.
	void clearExtFlag(uint32 mask) { extendedflags &= ~mask; }

	//! Get this Item's shape number
	uint32 getShape() const { return shape; }

	//! Set this Item's shape number
	void setShape(uint32 shape_)
		{ shape = shape_; cachedShapeInfo = 0; cachedShape = 0; }

	//! Get this Item's frame number
	uint32 getFrame() const { return frame; }

	//! Set this Item's frame number
	void setFrame(uint32 frame_) { frame = frame_; }

	//! Get this Item's quality (a.k.a. 'Q')
	uint16 getQuality() const { return quality; }

	//! Set this Item's quality (a.k.a 'Q');
	void setQuality(uint16 quality_) { quality = quality_; }

	//! Get the 'NpcNum' of this Item. Note that this can represent various
	//! things depending on the family of this Item.
	uint16 getNpcNum() const { return npcnum; }

	//! Set the 'NpcNum' of this Item. Note that this can represent various
	//! things depending on the family of this Item.
	void setNpcNum(uint16 npcnum_) { npcnum = npcnum_; }

	//! Get the 'MapNum' of this Item. Note that this can represent various
	//! things depending on the family of this Item.
	uint16 getMapNum() const { return mapnum; }

	//! Set the 'NpcNum' of this Item. Note that this can represent various
	//! things depending on the family of this Item.
	void setMapNum(uint16 mapnum_) { mapnum = mapnum_; }

	//! Get the ShapeInfo object for this Item. (The pointer will be cached.)
	inline ShapeInfo* getShapeInfo() const;

	//! Get the ShapeInfo object for this Item from the game instance.
	ShapeInfo* getShapeInfoFromGameInstance() const;

	//! Get the Shape object for this Item. (The pointer will be cached.)
	Shape* getShapeObject() const;

	//! Get the family of the shape number of this Item. (This is a
	//! member of the ShapeInfo object.)
	uint16 getFamily();

	//! Check if we can merge with another item.
	bool canMergeWith(Item* other);

	//! Get the open ContainerGump for this Item, if any. (NULL if not open.)
	ObjId getGump() { return gump; }
	//! Call this to notify the Item's open Gump has closed.
	void clearGump(); // set gump to 0 and clear the GUMP_OPEN flag
	//! Open a gump with the given shape for this Item
	ObjId openGump(uint32 gumpshape);
	//! Close this Item's gump, if any
	void closeGump();

	//! Destroy self.
	virtual void destroy(bool delnow = false);

	//! Check if this item overlaps another item in 3D world-space
	bool overlaps(Item& item2) const;

	//! Check if this item overlaps another item in the xy dims in 3D space
	bool overlapsxy(Item& item2) const;

	//! Check if this item is on top another item
	bool isOn(Item& item2) const;

	//! Check if this item can exist at the given coordinates
	bool canExistAt(sint32 x,sint32 y,sint32 z, bool needsupport=false) const;

	//! Get direction from centre to another item's centre.
	//! Undefined if either item is contained or equipped.
	int getDirToItemCentre(Item& item2) const;

	//! get 'distance' to other item. This is the maximum of the differences
	//! between the x, y (and possibly z) coordinates of the items.
	int getRange(Item& item2, bool checkz=false) const;

	//! Check if this item can reach another item. (This includes LoS.)
	//! \param other item to be reached
	//! \param range range
	//! \param x x coordinate of other to use, If zero, use real coords.
	//! \param y y coordinate of other to use
	//! \param z z coordinate of other to use.
	bool canReach(Item* other, int range, sint32 x=0, sint32 y=0, sint32 z=0);

	//! Move the object to (x,y,z) colliding with objects in the way.
	//! \param teleport move without colliding with objects between source and
	//!        destination
	//! \param force force the object to get to the destination without being
	//!        blocked by solid objects
	//! \param hititem if non-NULL, this is set to (one of) the item(s)
	//!        blocking the movement, or to zero if nothing blocked it
	//! \param dirs if non-NULL, this is set to a bitmask of the x/y/z
	//         directions in which movement was blocked (bit 0=x,1=y,2=z)
	//! \returns 0-0x4000 representing how far it got.
	//!          0 = didn't move
	//!          0x4000 = reached destination
	//! \note This can destroy the object
	sint32 collideMove(sint32 x,sint32 y,sint32 z, bool teleport, bool force,
					   ObjId* hititem=0, uint8* dirs=0);

	//! Make the item move up (delta>0) or down (delta<0),
	//! including any items on top of it
	//! \param delta distance in Z-direction to move
	//! \returns 0-0x4000 representing how far it got.
	//!          0 = didn't move
	//!          0x4000 = reached destination
	sint32 ascend(int delta);

	//! Make the item fall down.
	//! This creates a GravityProcess to do the actual work if the Item
	//! doesn't already have one.
	void fall();

	//! Make any items on top of this Item fall down and notify any supporting
	//! items that we're gone by calling the 'release' event.
	//! Note that this Item has to be moved away right after calling grab(),
	//! since otherwise the items will immediately hit this Item again.
	void grab();

	//! Hurl the item in the given direction
	void hurl(int xs, int ys, int zs, int grav);

	//! Set the PID of the GravityProcess for this Item
	void setGravityPID(ProcId pid) { gravitypid = pid; }

	//! Get the PID of the GravityProcess for this Item (or 0)
	ProcId getGravityPID() const { return gravitypid; }

	//! Get the GravityProcess of this Item, creating it if necessary
	virtual GravityProcess* ensureGravityProcess();

	//! Get the weight of this Item
	virtual uint32 getWeight();

	//! Get the weight of this Item and its contents, if any
	virtual uint32 getTotalWeight();

	//! Get the volume this item takes up in a container
	virtual uint32 getVolume();

	//! explode
	void explode();

	//! get the damage type this object does when hitting something
	virtual uint16 getDamageType();

	//! receive a hit
	//! \param other The item delivering the hit
	//! \param dir The direction the hit is coming from (or inverse? CHECKME!)
	//! \param damage The force of the hit. Zero for default
	//! \param type The type of damage done. Zero for default
	virtual void receiveHit(ObjId other, int dir, int damage, uint16 type);

	//! count nearby objects of a given shape
	unsigned int countNearby(uint32 shape, uint16 range);

	//! can this item be dragged?
	bool canDrag();

	//! how far can this item be thrown?
	//! \return range, or 0 if item can't be thrown
	int getThrowRange();

	//! Check this Item against the given loopscript
	//! \param script The loopscript to run
	//! \param scriptsize The size (in bytes) of the loopscript
	//! \return true if the item matches, false otherwise
	bool checkLoopScript(const uint8* script, uint32 scriptsize);
	
	uint32 callUsecodeEvent_look();								// event 0
	uint32 callUsecodeEvent_use();								// event 1
	uint32 callUsecodeEvent_anim();								// event 2
	uint32 callUsecodeEvent_cachein();							// event 4
	uint32 callUsecodeEvent_hit(ObjId hitted, sint16 hitforce);	// event 5
	uint32 callUsecodeEvent_gotHit(ObjId hitter, sint16 hitforce);// event 6
	uint32 callUsecodeEvent_hatch();							// event 7
	uint32 callUsecodeEvent_schedule(uint32 time);				// event 8
	uint32 callUsecodeEvent_release();							// event 9
	uint32 callUsecodeEvent_combine();							// event C
	uint32 callUsecodeEvent_enterFastArea();					// event F
	uint32 callUsecodeEvent_leaveFastArea();					// event 10
	uint32 callUsecodeEvent_cast(uint16 unk);					// event 11
	uint32 callUsecodeEvent_justMoved();						// event 12
	uint32 callUsecodeEvent_AvatarStoleSomething(uint16 unk);	// event 14
	uint32 callUsecodeEvent_guardianBark(sint16 unk);			// event 15

	uint32 use();

	//! Get lerped location.
	inline void getLerped(sint32& x, sint32& y, sint32& z) const
		{ x = ix; y = iy; z = iz; }

	//! Do lerping for an in between frame (0-256)
	//! The result can be retrieved with getLerped(x,y,z)
	//! \param factor The lerp factor: 0 is start of move, 256 is end of move
	inline void doLerp(sint32 factor)
	{
	// Should be noted that this does indeed limit us to 'only' 24bit coords
	// not that it matters because on disk they are unsigned 16 bit

		if (factor == 256)
		{
			ix = l_next.x;
			iy = l_next.y;
			iz = l_next.z;
		}
		else if (factor == 0)
		{
			ix = l_prev.x;
			iy = l_prev.y;
			iz = l_prev.z;
		}
		else
		{
#if 1
			// This way while possibly slower is more accurate
			ix = ((l_prev.x*(256-factor) + l_next.x*factor)>>8);
			iy = ((l_prev.y*(256-factor) + l_next.y*factor)>>8);
			iz = ((l_prev.z*(256-factor) + l_next.z*factor)>>8);
#else
			ix = l_prev.x + (((l_next.x-l_prev.x)*factor)>>8);
			iy = l_prev.y + (((l_next.y-l_prev.y)*factor)>>8);
			iz = l_prev.z + (((l_next.z-l_prev.z)*factor)>>8);
#endif
		}
	}

	//! Setup the lerped info for this gametick and animate the item
	void setupLerp(sint32 gametick);		

	//! The item has entered the fast area
	virtual void enterFastArea(); 

	//! The item has left the fast area
	//! \note This can destroy the object
	virtual void leaveFastArea();

	//! dump some info about this item to pout
	virtual void dumpInfo();

	bool loadData(IDataSource* ids, uint32 version);

	// Intrinsics
	INTRINSIC(I_touch);
	INTRINSIC(I_getX);
	INTRINSIC(I_getY);
	INTRINSIC(I_getZ);
	INTRINSIC(I_getCX);
	INTRINSIC(I_getCY);
	INTRINSIC(I_getCZ);
	INTRINSIC(I_getPoint);
	INTRINSIC(I_getShape);
	INTRINSIC(I_setShape);
	INTRINSIC(I_getFrame);
	INTRINSIC(I_setFrame);
	INTRINSIC(I_getQuality);
	INTRINSIC(I_getUnkEggType);
	INTRINSIC(I_getQuantity);
	INTRINSIC(I_getContainer);
	INTRINSIC(I_getRootContainer);
	INTRINSIC(I_getQ);
	INTRINSIC(I_getQHi);
	INTRINSIC(I_getQLo);
	INTRINSIC(I_setQ);
	INTRINSIC(I_setQHi);
	INTRINSIC(I_setQLo);
	INTRINSIC(I_setQuality);
	INTRINSIC(I_setQuantity);
	INTRINSIC(I_getFamily);
	INTRINSIC(I_getTypeFlag);
	INTRINSIC(I_getStatus);
	INTRINSIC(I_orStatus);
	INTRINSIC(I_andStatus);
	INTRINSIC(I_getFootpadData);
	INTRINSIC(I_overlaps);
	INTRINSIC(I_overlapsXY);
	INTRINSIC(I_isOn);
	INTRINSIC(I_ascend);
	INTRINSIC(I_getWeight);
	INTRINSIC(I_getWeightIncludingContents);
	INTRINSIC(I_getVolume);
	INTRINSIC(I_bark);
	INTRINSIC(I_getMapArray);
	INTRINSIC(I_setMapArray);
	INTRINSIC(I_getNpcNum);
	INTRINSIC(I_getDirToCoords);
	INTRINSIC(I_getDirFromCoords);
	INTRINSIC(I_getDirToItem);
	INTRINSIC(I_getDirFromItem);
	INTRINSIC(I_look);
	INTRINSIC(I_use);
	INTRINSIC(I_gotHit);
	INTRINSIC(I_enterFastArea);
	INTRINSIC(I_ask);
	INTRINSIC(I_getSliderInput);
	INTRINSIC(I_openGump);
	INTRINSIC(I_closeGump);
	INTRINSIC(I_create);
	INTRINSIC(I_legalCreateAtPoint);
	INTRINSIC(I_legalCreateAtCoords);
	INTRINSIC(I_legalCreateInCont);
	INTRINSIC(I_push);
	INTRINSIC(I_pop);
	INTRINSIC(I_popToCoords);
	INTRINSIC(I_popToContainer);
	INTRINSIC(I_popToEnd);
	INTRINSIC(I_destroy);
	INTRINSIC(I_move);
	INTRINSIC(I_legalMoveToPoint);
	INTRINSIC(I_legalMoveToContainer);
	INTRINSIC(I_hurl);
	INTRINSIC(I_shoot);
	INTRINSIC(I_fall);
	INTRINSIC(I_grab);
	INTRINSIC(I_igniteChaos);
	INTRINSIC(I_getFamilyOfType);
	INTRINSIC(I_getEtherealTop);
	INTRINSIC(I_guardianBark);
	INTRINSIC(I_getSurfaceWeight);
	INTRINSIC(I_isExplosive);
	INTRINSIC(I_receiveHit);
	INTRINSIC(I_explode);
	INTRINSIC(I_canReach);
	INTRINSIC(I_getRange);
	INTRINSIC(I_isCrusTypeNPC);

private:
	uint32 shape;	// DO NOT modify this directly! Always use setShape()!

protected:
	uint32 frame;

	sint32 x,y,z; // world coordinates
	uint16 flags;
	uint16 quality;
	uint16 npcnum;
	uint16 mapnum;

	uint32 extendedflags; // pentagram's own flags

	ObjId parent; // objid container this item is in (or 0 for top-level items)

	mutable Shape *cachedShape;
	mutable ShapeInfo *cachedShapeInfo;

	// This is stuff that is used for displaying and interpolation
	struct Lerped
	{
		sint32 x,y,z;
		uint32 shape,frame;
	};
	
	Lerped	l_prev;			// Previous state (relative to camera)
	Lerped	l_next;			// Next (current) state (relative to camera)
	sint32	ix,iy,iz;		// Interpolated position in camera space

	ObjId gump;			// Item's gump
	ProcId gravitypid;		// Item's GravityTracker (or 0)

	//! save the actual Item data 
	virtual void saveData(ODataSource* ods);

private:

	//! Call a Usecode Event. Use the separate functions instead!
	uint32 callUsecodeEvent(uint32 event, const uint8* args=0, int argsize=0);

	//! The gametick setupLerp was last called on
	sint32	last_setup;	

	//! Animate the item (called by setupLerp)
	void animateItem();

public:
	enum statusflags {
		FLG_DISPOSABLE	 = 0x0002,	//!< Item is discarded on map change
		FLG_OWNED		 = 0x0004,	//!< Item is owned by avatar
		FLG_CONTAINED	 = 0x0008,	//!< Item is in a container
		FLG_INVISIBLE	 = 0x0010,	//!< Item is invisible
		FLG_FLIPPED		 = 0x0020,	//!< Item is flipped horizontally
		FLG_IN_NPC_LIST	 = 0x0040,	//!< Item is a NPC
		FLG_FAST_ONLY	 = 0x0080,	//!< Item is discarded when leaving fast area
		FLG_GUMP_OPEN	 = 0x0100,	//!< Item has a gump open
		FLG_EQUIPPED	 = 0x0200,	//!< Item is equipped
		FLG_BOUNCING	 = 0x0400,	//!< Item has bounced
		FLG_ETHEREAL	 = 0x0800,	//!< Item is in the ethereal list
		FLG_HANGING		 = 0x1000,	//!< Item is suspended in the air
		FLG_FASTAREA     = 0x2000,	//!< Item is in the fast area
		FLG_LOW_FRICTION = 0x4000	//!< Item has low friction
	};

	enum extflags {
		EXT_FIXED		 = 0x0001,	//!< Item came from FIXED
		EXT_INCURMAP	 = 0x0002,	//!< Item is in a CurrentMap display list
		EXT_LERP_NOPREV	 = 0x0008,	//!< Item can't be lerped this frame
		EXT_HIGHLIGHT	 = 0x0010,	//!< Item should be Painted highlighted
		EXT_CAMERA		 = 0x0020,	//!< Item is being followed by the camera
		EXT_SPRITE		 = 0x0040,	//!< Item is a sprite
		EXT_TRANSPARENT  = 0x0080,  //!< Item should be painted transparent
		EXT_PERMANENT_NPC= 0x0100	//!< Item is a permanent NPC
	};
};

inline ShapeInfo* Item::getShapeInfo() const
{
	if (!cachedShapeInfo)
		cachedShapeInfo = getShapeInfoFromGameInstance();
	return cachedShapeInfo;
}

inline void Item::getFootpadData(sint32& X, sint32& Y, sint32& Z) const
{
	ShapeInfo* si = getShapeInfo();
	Z = si->z;

	if (flags & Item::FLG_FLIPPED) {
		X = si->y;
		Y = si->x;
	} else {
		X = si->x;
		Y = si->y;
	}
}

// like getFootpadData, but scaled to world coordinates
inline void Item::getFootpadWorld(sint32& X, sint32& Y, sint32& Z) const
{
	ShapeInfo* si = getShapeInfo();
	Z = si->z * 8;

	if (flags & Item::FLG_FLIPPED) {
		X = si->y * 32;
		Y = si->x * 32;
	} else {
		X = si->x * 32;
		Y = si->y * 32;
	}
}

inline void Item::getLocation(sint32& X, sint32& Y, sint32& Z) const
{
	X = x;
	Y = y;
	Z = z;
}

#endif
