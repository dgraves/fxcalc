/* CALCStatBox.cpp
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

#include "CALCStatBox.h"
#include "CALCList.h"
#include "CALCWindow.h"
#include "CALCHelp.h"

FXDEFMAP(CALCStatBox) CALCStatBoxMap[]={
  FXMAPFUNC(SEL_DND_DROP,CALCStatBox::ID_VALUES,CALCStatBox::onDNDDrop),
  FXMAPFUNC(SEL_DND_REQUEST,CALCStatBox::ID_VALUES,CALCStatBox::onDNDRequest),
  FXMAPFUNC(SEL_COMMAND,CALCStatBox::ID_FORWARD,CALCStatBox::onCmdForward),
  FXMAPFUNC(SEL_COMMAND,CALCStatBox::ID_LOAD,CALCStatBox::onCmdLoad),
  FXMAPFUNC(SEL_COMMAND,CALCStatBox::ID_DELETE,CALCStatBox::onCmdDelete),
  FXMAPFUNC(SEL_COMMAND,CALCStatBox::ID_CLEAR,CALCStatBox::onCmdClear),
  FXMAPFUNC(SEL_UPDATE,CALCStatBox::ID_POPULATION,CALCStatBox::onUpdPopulation),
  FXMAPFUNCS(SEL_RIGHTBUTTONPRESS,CALCStatBox::ID_VALUES,CALCStatBox::ID_POPULATION,CALCStatBox::onPopupMenu),
  FXMAPFUNC(SEL_COMMAND,CALCStatBox::ID_CLARIFY,CALCStatBox::onCmdClarify)
};

FXIMPLEMENT(CALCStatBox,FXDialogBox,CALCStatBoxMap,ARRAYNUMBER(CALCStatBoxMap))

CALCStatBox::CALCStatBox(FXApp* app)
: FXDialogBox(app,"Statistics Box",DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE|DECOR_MENU|DECOR_RESIZE,0,0,0,0, 0,0,0,0),
  clarify(FALSE)
{
  FXVerticalFrame* contents=new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXVerticalFrame* listFrame=new FXVerticalFrame(contents,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  values=new CALCList(listFrame,0,this,ID_VALUES,LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  FXHorizontalFrame* buttons=new FXHorizontalFrame(contents,PACK_UNIFORM_WIDTH|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  new FXButton(buttons,"Forw\t"STATFORWARD_HELP,NULL,this,ID_FORWARD,FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXButton(buttons,"Load\t"STATLOAD_HELP,NULL,this,ID_LOAD,FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXButton(buttons,"Del\t"STATDELETE_HELP,NULL,this,ID_DELETE,FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXButton(buttons,"Clr\t"STATCLEAR_HELP,NULL,this,ID_CLEAR,FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  FXLabel*label=new FXLabel(contents,"n=0",NULL,LAYOUT_FILL_X);
  label->setTarget(this);
  label->setSelector(ID_POPULATION);
  label->setTipText(STATPOPULATION_HELP);
  label->enable();
}

CALCStatBox::~CALCStatBox()
{
  handle(this,MKUINT(ID_CLEAR,SEL_COMMAND),NULL);
}

void CALCStatBox::create()
{
  FXDialogBox::create();
  resize(200,150);  //Clears the garbage that shows up in the border with default size
}

FXint CALCStatBox::getSelectedItem() const
{
  FXint n,total=values->getNumItems();
  for(n=0;n<total;n++)
  {
    if(values->isItemSelected(n))
      return n;
  }
  return -1;  //List is empty or for some reason nothing is selected
}

void CALCStatBox::addData(const FXString& label,CALCdouble value,FXint pos)
{
  CALCdouble* data=new CALCdouble;
  *data=value;
  if(pos>=0)
    values->insertItem(pos,label,NULL,(void*)data);
  else
    values->appendItem(label,NULL,(void*)data);
}

//Fix Me - divide by zero
CALCdouble CALCStatBox::getMean() const
{
  FXint n=values->getNumItems();
  if(n==0)
    throw FXErrorException("Divide by zero error.");
  return getSum()/(CALCdouble)n;
}

//Fix Me - divide by zero
CALCdouble CALCStatBox::getSquaresMean() const
{
  FXint n=values->getNumItems();
  if(n==0)
    throw FXErrorException("Divide by zero error.");
  return getSquaresSum()/(CALCdouble)n;
}

CALCdouble CALCStatBox::getSum() const
{
  CALCdouble sum=0.0;
  FXint n,total=values->getNumItems();
  for(n=0;n<total;n++)
    sum+=*((CALCdouble*)values->getItemData(n));
  return sum;
}

CALCdouble CALCStatBox::getSquaresSum() const
{
  CALCdouble sum=0.0;
  FXint n,total=values->getNumItems();
  for(n=0;n<total;n++)
    sum+=pow(*((CALCdouble*)values->getItemData(n)),(CALCdouble)2.0);
  return sum;
}

//Calculate standard deviation with population n-1
//Sum square of difference of each value from mean, divides by population-1, and takes square root
//Because the values tend to be closer to their sample mean (computed here)
//than the actual population mean, we compensate by dividing by n-1 rather than n
CALCdouble CALCStatBox::getStandardDevN1() const
{
  CALCdouble dev=0.0;
  CALCdouble mean=0.0;
  FXint n,total=values->getNumItems();

  try
  {
    mean=getMean();
  }
  catch(FXErrorException)
  {
    throw;
  }

  if(total>1)
  {
    for(n=0;n<total;n++)
      dev+=pow((*((CALCdouble*)values->getItemData(n)))-mean,(CALCdouble)2.0);  //Tyoecast to CALCdouble is for doubledouble's pow
    dev/=(CALCdouble)(total-1);
    dev=sqrt(dev);
  }
  return dev;
}

//Calculate standard deviation with population n
//Sum square of difference of each value from mean, divides by population, and takes square root
CALCdouble CALCStatBox::getStandardDevN() const
{
  CALCdouble dev=0.0;
  CALCdouble mean=0.0;
  FXint n,total=values->getNumItems();

  try
  {
    mean=getMean();
  }
  catch(FXErrorException)
  {
    throw;
  }

  if(total>0)
  {
    for(n=0;n<total;n++)
      dev+=pow((*((CALCdouble*)values->getItemData(n)))-mean,(CALCdouble)2.0);
    dev/=(CALCdouble)total;
    dev=sqrt(dev);
  }
  return dev;
}

long CALCStatBox::onDNDDrop(FXObject*,FXSelector,void* ptr)
{
  FXEvent *event=(FXEvent*)ptr;
  FXuchar *data,*junk; FXuint len,dum; FXint pos=-1;
  FXDragAction action=inquireDNDAction();

  if(getDNDData(FROM_DRAGNDROP,textType,data,len))
  {
    // Need to ask the source to delete his copy
    if(action==DRAG_MOVE)
    {
      getDNDData(FROM_DRAGNDROP,deleteType,junk,dum);
      FXASSERT(!junk);
    }

    //We need to determine the value of the dropped data
    FXString current;
    FXString label((FXchar*)data,len);
    CALCdouble value=0.0;
    getApp()->getMainWindow()->handle(this,MKUINT(CALCWindow::ID_GETLCDTEXT,SEL_COMMAND),(void*)&current);
    getApp()->getMainWindow()->handle(this,MKUINT(CALCWindow::ID_PASTELCDTEXT,SEL_COMMAND),(void*)&label);
    getApp()->getMainWindow()->handle(this,MKUINT(CALCWindow::ID_GETLCDTEXT,SEL_COMMAND),(void*)&label);
    getApp()->getMainWindow()->handle(this,MKUINT(CALCWindow::ID_GETLCDVALUE,SEL_COMMAND),(void*)&value);
    getApp()->getMainWindow()->handle(this,MKUINT(CALCWindow::ID_SETLCDTEXT,SEL_COMMAND),(void*)&current);

    pos=values->getInsertPosition();
    addData(label,value,pos);
 
    if(action==DRAG_MOVE&&pos>=0)
      values->selectItem(pos);

    FXFREE(&data);
  }
  return 1;
}

long CALCStatBox::onDNDRequest(FXObject* sender,FXSelector sel,void* ptr)
{
  FXEvent *event=(FXEvent*)ptr;
  FXuchar *data; FXuint len;

  if(event->target==textType)
  {
    FXint target=getSelectedItem();
    if(target>=0)
    {
      FXString str=values->getItemText(target);
      len=str.length();
      FXMALLOC(&data,FXuchar,len);
      memcpy((FXchar*)data,str.text(),len);
      setDNDData(FROM_DRAGNDROP,textType,data,len);
      return 1;
    }
  }

  // Delete dragged text
  if(event->target==deleteType)
  {
    handle(this,MKUINT(ID_DELETE,SEL_COMMAND),NULL);
    return 1;
  }

  return 0;
 }

long CALCStatBox::onCmdForward(FXObject*,FXSelector,void*)
{
  getApp()->getMainWindow()->raise();
  return 1;
}

long CALCStatBox::onCmdLoad(FXObject*,FXSelector,void*)
{
  FXint target=getSelectedItem();
  if(target>=0)
    getApp()->getMainWindow()->handle(this,MKUINT(CALCWindow::ID_SETLCDVALUE,SEL_COMMAND),values->getItemData(target));
  else
    getApp()->beep();
  return 1;
}

long CALCStatBox::onCmdDelete(FXObject*,FXSelector,void*)
{
  FXint target=getSelectedItem();
  if(target>=0)
  {
    CALCdouble* value=(CALCdouble*)values->getItemData(target);
    delete value;
    values->removeItem(target);
  }
  else
    getApp()->beep();
  return 1;
}

long CALCStatBox::onCmdClear(FXObject*,FXSelector,void*)
{
//  values->clearItems();
  while(values->getNumItems()>0)
  {
    CALCdouble* value=(CALCdouble*)values->getItemData(0);
    delete value;
    values->removeItem(0);
  }
  return 1;
}

long CALCStatBox::onUpdPopulation(FXObject* sender,FXSelector,void*)
{
  FXString str;
  str.format("n=%d",values->getNumItems());
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&str);
  return 1;
}

long CALCStatBox::onPopupMenu(FXObject* sender,FXSelector sel,void* ptr)
{
#ifdef HAVE_RTTI
  FXString str="";

  //Attemp to get tip text
  if(SELID(sel)==ID_VALUES)
    str=STATVALUES_HELP;
  else
  {
    try
    {
      FXLabel* helpsource=dynamic_cast<FXLabel*>(sender);
      if(helpsource)
      {
        //Check for tip text
        str=helpsource->getTipText();
      }
    }
    catch(...)
    {
      //Probably an access violation - definately no tip text for you
      ;
    }
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

long CALCStatBox::onCmdClarify(FXObject*,FXSelector,void*)
{
  clarify=TRUE;
  return 1;
}
