/* CALCLabel.cpp
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

#include "fox-1.6/fx.h"
#include "CALCLabel.h"

enum
{
  MOUSE_NONE=0,
  MOUSE_TRYDRAG=1,
  MOUSE_DRAG=2
};

FXDEFMAP(CALCLabel) CALCLabelMap[]={
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,CALCLabel::onLeftBtnPress),
  FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,0,CALCLabel::onMiddleBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,CALCLabel::onLeftBtnRelease),
  FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE,0,CALCLabel::onMiddleBtnRelease),
  FXMAPFUNC(SEL_MOTION,0,CALCLabel::onMotion),
  FXMAPFUNC(SEL_BEGINDRAG,0,CALCLabel::onBeginDrag),
  FXMAPFUNC(SEL_ENDDRAG,0,CALCLabel::onEndDrag),
  FXMAPFUNC(SEL_DRAGGED,0,CALCLabel::onDragged),
  FXMAPFUNC(SEL_DND_MOTION,0,CALCLabel::onDNDMotion),
  FXMAPFUNC(SEL_DND_DROP,0,CALCLabel::onDNDDrop),
  FXMAPFUNC(SEL_DND_REQUEST,0,CALCLabel::onDNDRequest)
};

FXIMPLEMENT(CALCLabel,FXLabel,CALCLabelMap,ARRAYNUMBER(CALCLabelMap))

CALCLabel::CALCLabel(FXComposite* p,const FXString& text,FXIcon* ic,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb)
: FXLabel(p,text,ic,opts,x,y,w,h,pl,pr,pt,pb),
  dragmode(MOUSE_NONE)
{
}

void CALCLabel::create()
{
  FXLabel::create();
  if(!textType){textType=getApp()->registerDragType(textTypeName);}
  dropEnable();
}

long CALCLabel::onLeftBtnPress(FXObject* sender,FXSelector sel,void* ptr)
{
  FXLabel::onLeftBtnPress(sender,sel,ptr);
  dragmode=MOUSE_TRYDRAG;
  grab();
  return 1;
}

long CALCLabel::onMiddleBtnPress(FXObject* sender,FXSelector sel,void* ptr)
{
  FXLabel::onMiddleBtnPress(sender,sel,ptr);
  dragmode=MOUSE_TRYDRAG;
  grab();
  return 1;
}

long CALCLabel::onLeftBtnRelease(FXObject* sender,FXSelector sel,void* ptr)
{
  FXLabel::onLeftBtnRelease(sender,sel,ptr);
  if(dragmode==MOUSE_DRAG)
  {
    handle(this,MKUINT(0,SEL_ENDDRAG),ptr);
  }
  dragmode=MOUSE_NONE;
  ungrab();
  return 0;
}

long CALCLabel::onMiddleBtnRelease(FXObject* sender,FXSelector sel,void* ptr)
{
  FXLabel::onLeftBtnRelease(sender,sel,ptr);
  if(dragmode==MOUSE_DRAG)
  {
    handle(this,MKUINT(0,SEL_ENDDRAG),ptr);
  }
  dragmode=MOUSE_NONE;
  ungrab();
  return 1;
}

long CALCLabel::onMotion(FXObject* sender,FXSelector sel,void* ptr)
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
    FXLabel::onMotion(sender,sel,ptr);

  return 1;
}

long CALCLabel::onBeginDrag(FXObject*,FXSelector,void*)
{
  beginDrag(&textType,1);
  setDragCursor(getApp()->getDefaultCursor(DEF_DNDSTOP_CURSOR));
  return 1;
}

long CALCLabel::onEndDrag(FXObject*,FXSelector,void*)
{
  endDrag((didAccept()!=DRAG_REJECT));
  setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
  return 1;
}

long CALCLabel::onDragged(FXObject*,FXSelector,void* ptr)
{
  FXEvent* event=(FXEvent*)ptr;
  FXDragAction action=DRAG_COPY;

  if(underCursor())  //Reject
  {
    setDragCursor(getApp()->getDefaultCursor(DEF_DNDSTOP_CURSOR));
    return 1;
  }

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

long CALCLabel::onDNDMotion(FXObject*,FXSelector,void*)
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

long CALCLabel::onDNDDrop(FXObject*,FXSelector,void* ptr)
{
  if(target && target->handle(this,MKUINT(message,SEL_DND_DROP),ptr)) return 1;
  return 0;
}

long CALCLabel::onDNDRequest(FXObject*,FXSelector,void* ptr)
{
  if(target && target->handle(this,MKUINT(message,SEL_DND_REQUEST),ptr)) return 1;
  return 0;
}
