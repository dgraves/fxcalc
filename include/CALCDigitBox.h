/* CALCDigitBox.h
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

#ifndef _CALCDIGITBOX_H_
#define _CALCDIGITBOX_H_

class CALCDigitBox : public FXDialogBox
{
  FXDECLARE(CALCDigitBox)
protected:
  FXbool clarify;
  FXSpinner* spinner;

  CALCDigitBox(){}

public:
  CALCDigitBox(FXWindow* owner,const FXString& name);
  virtual void create();

  FXuint getNumDigits() const;
  void setNumDigits(FXuint n);

  long onCmdDefaultDigits(FXObject*,FXSelector,void*);
  long onPopupMenu(FXObject*,FXSelector,void*);
  long onCmdClarify(FXObject*,FXSelector,void*);

  enum
  {
    ID_NUMDIGITS=FXDialogBox::ID_LAST,
    ID_DEFAULTDIGITS,
    ID_CLARIFY,
    ID_LAST
  };
};

#endif
