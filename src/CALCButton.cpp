/* CALCButton.cpp
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

#include "fox/fx.h"
#include "CALCButton.h"

FXIMPLEMENT(CALCButton,FXButton,NULL,0)

CALCButton::CALCButton(FXComposite* p,const FXString& text,FXIcon* ic=NULL,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=BUTTON_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD)
: FXButton(p,text,ic,tgt,sel,opts,x,y,w,h,pl,pr,pt,pb),
  dw(w),
  dh(h)
{
}

FXint CALCButton::getDefaultWidth()
{
  return(dw>0)?dw:FXButton::getDefaultWidth();
}

FXint CALCButton::getDefaultHeight()
{
  return(dh>0)?dh:FXButton::getDefaultHeight();
}
