/* CALCPrefsBox.h
 * Copyright (C) 2002 Dustin Graves <dgraves@computer.org>
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

#ifndef _CALCPREFSBOX_H_
#define _CALCPREFSBOX_H_

enum
{
  CALCPREFS_SETTINGS=0,
  CALCPREFS_COLORS=1,
  CALCPREFS_ABOUT=2
};

class CALCWindow;

class CALCPrefsBox : public FXDialogBox
{
  FXDECLARE(CALCPrefsBox)
protected:
  FXTabBook* tabbook;

  CALCPrefsBox() {}

public:
  CALCPrefsBox(CALCWindow* owner);
  ~CALCPrefsBox();

  void setPrefTab(FXuint tab);
  long CALCPrefsBox::onCmdDefaults(FXObject*,FXSelector,void*);

  enum
  {
    ID_DEFAULTS=FXDialogBox::ID_LAST,
    ID_LAST
  };
};

#endif
