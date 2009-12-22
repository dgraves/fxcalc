/* CALCStatBox.h
 * Copyright (C) 2001,2002 Dustin Graves <dgraves@computer.org>
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

#ifndef _CALCSTATBOX_H_
#define _CALCSTATBOX_H_

class CALCList;
class CALCWindow;

class CALCStatBox : public FXDialogBox
{
  FXDECLARE(CALCStatBox)
protected:
  FXbool clarify;
  CALCList* values;

  CALCStatBox() {}
  FXint getSelectedItem() const;

public:
  CALCStatBox(CALCWindow* owner);
  ~CALCStatBox();

  virtual void create();

  void addData(const FXString& label,CALCdouble value,FXint pos=-1);
  CALCdouble getMean() const;
  CALCdouble getSquaresMean() const;
  CALCdouble getSum() const;
  CALCdouble getSquaresSum() const;
  CALCdouble getStandardDevN1() const;       //Calculate standard deviation with population n-1
  CALCdouble getStandardDevN() const;        //Calculate standard deviation with population n

  long onDNDDrop(FXObject*,FXSelector,void*);
  long onDNDRequest(FXObject*,FXSelector,void*);
  long onCmdForward(FXObject*,FXSelector,void*);
  long onCmdLoad(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
  long onCmdClear(FXObject*,FXSelector,void*);
  long onUpdPopulation(FXObject*,FXSelector,void*);

  long onPopupMenu(FXObject*,FXSelector,void*);
  long onCmdClarify(FXObject*,FXSelector,void*);

  enum
  {
    ID_VALUES=FXDialogBox::ID_LAST,
    ID_FORWARD,
    ID_LOAD,
    ID_DELETE,
    ID_CLEAR,
    ID_POPULATION,
    ID_CLARIFY,
    ID_LAST
  };
};

#endif
