/* CALCDigitBox.cpp
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
#include "CALCdefs.h"
#include "CALCHelp.h"
#include "CALCDigitBox.h"

FXDEFMAP(CALCDigitBox) CALCDigitBoxMap[]={
  FXMAPFUNC(SEL_COMMAND,CALCDigitBox::ID_DEFAULTDIGITS,CALCDigitBox::onCmdDefaultDigits),
  FXMAPFUNCS(SEL_RIGHTBUTTONPRESS,FXSpinner::ID_INCREMENT,FXSpinner::ID_ENTRY,CALCDigitBox::onPopupMenu),
  FXMAPFUNCS(SEL_RIGHTBUTTONPRESS,CALCDigitBox::ID_NUMDIGITS,CALCDigitBox::ID_DEFAULTDIGITS,CALCDigitBox::onPopupMenu),
  FXMAPFUNC(SEL_COMMAND,CALCDigitBox::ID_CLARIFY,CALCDigitBox::onCmdClarify)
};

FXIMPLEMENT(CALCDigitBox,FXDialogBox,CALCDigitBoxMap,ARRAYNUMBER(CALCDigitBoxMap))

CALCDigitBox::CALCDigitBox(FXWindow* owner,const FXString& name)
: FXDialogBox(owner,name,DECOR_TITLE|DECOR_BORDER,0,0,0,0, 0,0,0,0),
  clarify(FALSE)
{
  FXVerticalFrame* contents=new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(contents,"Max number of decimal digits to dislpay:");
  FXHorizontalFrame* digits=new FXHorizontalFrame(contents,LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  new FXButton(digits,"&Default\t"DIGITDEFAULTDIGITS_HELP,NULL,this,ID_DEFAULTDIGITS,FRAME_THICK|FRAME_RAISED|LAYOUT_RIGHT);
  spinner=new FXSpinner(digits,5,this,ID_NUMDIGITS,FRAME_THICK|FRAME_SUNKEN|LAYOUT_RIGHT);
  spinner->setTipText(DIGITNUMDIGITS_HELP);
  spinner->setRange(0,CALCDBL_DIG+2);  //Give it a little extra

  new FXHorizontalSeparator(contents,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  FXHorizontalFrame* buttons=new FXHorizontalFrame(contents,PACK_UNIFORM_WIDTH|LAYOUT_RIGHT|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  new FXButton(buttons,"&Accept",NULL,this,FXDialogBox::ID_ACCEPT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXButton(buttons,"&Cancel",NULL,this,FXDialogBox::ID_CANCEL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y);
}

void CALCDigitBox::create()
{
  FXDialogBox::create();
}

FXuint CALCDigitBox::getNumDigits() const
{
  return spinner->getValue();
}

void CALCDigitBox::setNumDigits(FXuint n)
{
  spinner->setValue(n);
}

long CALCDigitBox::onCmdDefaultDigits(FXObject*,FXSelector,void*)
{
  setNumDigits(CALCDBL_DIG);
  return 1;
}

long CALCDigitBox::onPopupMenu(FXObject*,FXSelector sel,void* ptr)
{
#ifdef HAVE_RTTI
  FXString str="";

  //Attemp to get tip text
  switch(FXSELID(sel))
  {
  case FXSpinner::ID_INCREMENT:
  case FXSpinner::ID_DECREMENT:
  case FXSpinner::ID_ENTRY:
  case ID_NUMDIGITS:
    str=DIGITNUMDIGITS_HELP;
    break;
  case ID_DEFAULTDIGITS:
    str=DIGITDEFAULTDIGITS_HELP;
    break;
  }

  if(!str.empty())
  {
    FXEvent* event=(FXEvent*)ptr;
    FXMenuPane pane(this);
    new FXMenuCommand(&pane,"&Huh?",NULL,this,ID_CLARIFY);

    clarify=FALSE;

    pane.create();
    pane.popup(NULL,event->root_x,event->root_y);
    getApp()->runModalWhileShown(&pane);

    if(clarify)
    {
      FXMenuPane clarification(this,FRAME_LINE);

      FXButton* button=new FXButton(&clarification,str,NULL,&clarification,FXWindow::ID_HIDE,JUSTIFY_LEFT);
      button->setFont(getApp()->getNormalFont());
      button->setTextColor(getApp()->getForeColor());
      button->setBackColor(FXRGB(255,255,192));

      clarification.create();
      clarification.popup(NULL,event->root_x,event->root_y);
      getApp()->runModalWhileShown(&clarification);
    }
  }
#endif
  return 1;
}

long CALCDigitBox::onCmdClarify(FXObject*,FXSelector,void*)
{
  clarify=TRUE;
  return 1;
}
