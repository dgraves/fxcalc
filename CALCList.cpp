/* CALCList.cpp
 * Copyright (C) 2001 Dustin Graves <dgraves@computer.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "CALCList.h"

enum
{
  MOUSE_NONE=0,
  MOUSE_TRYDRAG=1,
  MOUSE_DRAG=2
};

FXDEFMAP(CALCList) CALCListMap[]={
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,CALCList::onLeftBtnPress),
  FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,0,CALCList::onMiddleBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,CALCList::onLeftBtnRelease),
  FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE,0,CALCList::onMiddleBtnRelease),
  FXMAPFUNC(SEL_MOTION,0,CALCList::onMotion),
  FXMAPFUNC(SEL_BEGINDRAG,0,CALCList::onBeginDrag),
  FXMAPFUNC(SEL_ENDDRAG,0,CALCList::onEndDrag),
  FXMAPFUNC(SEL_DRAGGED,0,CALCList::onDragged),
  FXMAPFUNC(SEL_DND_MOTION,0,CALCList::onDNDMotion),
  FXMAPFUNC(SEL_DND_DROP,0,CALCList::onDNDDrop),
  FXMAPFUNC(SEL_DND_REQUEST,0,CALCList::onDNDRequest)
};

FXIMPLEMENT(CALCList,FXList,CALCListMap,ARRAYNUMBER(CALCListMap))

CALCList::CALCList(FXComposite *p,FXint nvis,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h)
: FXList(p,nvis,tgt,sel,opts,x,y,w,h),
  dragmode(MOUSE_NONE)
{
}

void CALCList::create()
{
  FXList::create();
  if(!textType){textType=getApp()->registerDragType(textTypeName);}
  dropEnable();
}

FXint CALCList::getInsertPosition() const
{
  FXint item,h,i,p=0;
  FXint x,y; FXuint buttons;

  getCursorPosition(x,y,buttons);
  item=getItemAt(x,y);

  //Is there an item?
  if(item<0)
    return nitems;

  //Does it go before or after the item
  for(i=0;i<item;i++)
    p+=getItemHeight(i);
  p+=getItemHeight(item)/2;

  if(y>p) item++;

  return item;
}

long CALCList::onLeftBtnPress(FXObject* sender,FXSelector sel,void* ptr)
{
  FXList::onLeftBtnPress(sender,sel,ptr);
  dragmode=MOUSE_TRYDRAG;
  grab();
  return 1;
}

long CALCList::onMiddleBtnPress(FXObject* sender,FXSelector sel,void* ptr)
{
  FXList::onMiddleBtnPress(sender,sel,ptr);
  dragmode=MOUSE_TRYDRAG;
  grab();
  return 1;
}

long CALCList::onLeftBtnRelease(FXObject* sender,FXSelector sel,void* ptr)
{
  FXList::onLeftBtnRelease(sender,sel,ptr);
  if(dragmode==MOUSE_DRAG)
  {
    handle(this,MKUINT(0,SEL_ENDDRAG),ptr);
  }
  dragmode=MOUSE_NONE;
  ungrab();
  return 0;
}

long CALCList::onMiddleBtnRelease(FXObject* sender,FXSelector sel,void* ptr)
{
  FXList::onLeftBtnRelease(sender,sel,ptr);
  if(dragmode==MOUSE_DRAG)
  {
    handle(this,MKUINT(0,SEL_ENDDRAG),ptr);
  }
  dragmode=MOUSE_NONE;
  ungrab();
  return 1;
}

long CALCList::onMotion(FXObject* sender,FXSelector sel,void* ptr)
{
  FXEvent* event=(FXEvent*)ptr;

  switch(dragmode)
  {
    case MOUSE_DRAG:
      handle(this,MKUINT(0,SEL_DRAGGED),ptr);
      break;
    case MOUSE_TRYDRAG:
      if(event->moved)
      {
        dragmode=MOUSE_NONE;
        if(handle(this,MKUINT(0,SEL_BEGINDRAG),ptr))
        {
          dragmode=MOUSE_DRAG;
        }
      }
      break;
  }

  if(dragmode==MOUSE_NONE)
    FXList::onMotion(sender,sel,ptr);

  return 1;
}

long CALCList::onBeginDrag(FXObject* sender,FXSelector sel,void* ptr)
{
  beginDrag(&textType,1);
  setDragCursor(getApp()->getDefaultCursor(DEF_DNDMOVE_CURSOR));
  return 1;
}

long CALCList::onEndDrag(FXObject* sender,FXSelector sel,void* ptr)
{
  endDrag((didAccept()!=DRAG_REJECT));
  setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
  return 1;
}

long CALCList::onDragged(FXObject* sender,FXSelector sel,void* ptr)
{
  FXEvent* event=(FXEvent*)ptr;
  FXDragAction action=DRAG_COPY;

  if(underCursor()) action=DRAG_MOVE;

  handleDrag(event->root_x,event->root_y,action);
  if(didAccept()!=DRAG_REJECT)
  {
    if(action==DRAG_MOVE)
      setDragCursor(getApp()->getDefaultCursor(DEF_DNDMOVE_CURSOR));
    else
      setDragCursor(getApp()->getDefaultCursor(DEF_DNDCOPY_CURSOR));
  }
  else
    setDragCursor(getApp()->getDefaultCursor(DEF_DNDSTOP_CURSOR));

  return 1;
}

long CALCList::onDNDMotion(FXObject* sender,FXSelector sel,void* ptr)
{
  FXDragAction action;

  // Correct drop type
  if(offeredDNDType(FROM_DRAGNDROP,textType))
  {
    action=inquireDNDAction();

    // Check for legal DND action
    if(action==DRAG_COPY || action==DRAG_MOVE)
      acceptDrop(DRAG_ACCEPT);

    return 1;
  }

  // Didn't handle it here
  return 0;
}

long CALCList::onDNDDrop(FXObject* sender,FXSelector sel,void* ptr)
{
  if(target && target->handle(this,MKUINT(message,SEL_DND_DROP),ptr)) return 1;
  return 0;
}

long CALCList::onDNDRequest(FXObject* sender,FXSelector sel,void* ptr)
{
  if(target && target->handle(this,MKUINT(message,SEL_DND_REQUEST),ptr)) return 1;
  return 0;
}
