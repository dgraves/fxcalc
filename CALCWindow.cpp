/* CALCWindow.cpp
 * Copyright (C) 2000,2001 Dustin Graves <dgraves@computer.org>
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

#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#include <winuser.h>
#endif
#include "fox/fxkeys.h"

#include "icons.h"
#include "CALCWindow.h"
#include "CALCLabel.h"
#include "CALCStatBox.h"
#include "CALCDigitBox.h"
#include "CALCHelp.h"

#define BUTTONWIDTH 40
#define BUTTONHEIGHT 30
#define FRAMEGAP 12

#define FUNCTION  FXRGB(100,40,10)
#define VALUE     FXRGB(64,0,96)
#define OPERATOR  FXRGB(160,0,0)

FXDEFMAP(CALCWindow) CALCWindowMap[]={
  FXMAPFUNC(SEL_CLIPBOARD_REQUEST,0,CALCWindow::onClipboardRequest),
  FXMAPFUNC(SEL_CLOSE,0,CALCWindow::onCmdQuit),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_QUIT,CALCWindow::onCmdQuit),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_COPY,CALCWindow::onCmdCopy),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_PASTE,CALCWindow::onCmdPaste),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_PASTE,CALCWindow::onUpdPaste),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_NUMDIGITS,CALCWindow::onCmdNumDigits),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DIGITGROUPING,CALCWindow::onCmdDigitGrouping),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_DIGITGROUPING,CALCWindow::onUpdDigitGrouping),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_USETOOLTIPS,CALCWindow::onCmdUseTooltips),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_USETOOLTIPS,CALCWindow::onUpdUseTooltips),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_ABOUT,CALCWindow::onCmdAbout),
  FXMAPFUNC(SEL_DND_DROP,CALCWindow::ID_LCD,CALCWindow::onDNDDrop),
  FXMAPFUNC(SEL_DND_REQUEST,CALCWindow::ID_LCD,CALCWindow::onDNDRequest),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_GETLCDTEXT,CALCWindow::onCmdGetLCDText),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_SETLCDTEXT,CALCWindow::onCmdSetLCDText),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_PASTELCDTEXT,CALCWindow::onCmdPasteLCDText),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_GETLCDVALUE,CALCWindow::onCmdGetLCDValue),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_SETLCDVALUE,CALCWindow::onCmdSetLCDValue),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_STANDARD,CALCWindow::ID_SCIENTIFIC,CALCWindow::onCmdMode),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_STANDARD,CALCWindow::ID_SCIENTIFIC,CALCWindow::onUpdMode),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_BINARY,CALCWindow::ID_HEXADECIMAL,CALCWindow::onCmdBase),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_BINARY,CALCWindow::ID_HEXADECIMAL,CALCWindow::onUpdBase),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_DEGREES,CALCWindow::ID_GRADIENTS,CALCWindow::onCmdRep),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_DEGREES,CALCWindow::ID_GRADIENTS,CALCWindow::onUpdRep),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_BYTE,CALCWindow::ID_DLWORD,CALCWindow::onCmdWordType),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_BYTE,CALCWindow::ID_DLWORD,CALCWindow::onUpdWordType),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_INV,CALCWindow::onCmdInv),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_INV,CALCWindow::onUpdInv),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_HYP,CALCWindow::onCmdHyp),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_HYP,CALCWindow::onUpdHyp),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_PARENLABEL,CALCWindow::onUpdParenLabel),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_MEMORYLABEL,CALCWindow::onUpdMemoryLabel),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_BACKSPACE,CALCWindow::onCmdBackSpace),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_CLEARENTRY,CALCWindow::onCmdClearEntry),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_CLEARALL,CALCWindow::onCmdClearAll),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_SHOWSTATBOX,CALCWindow::onCmdShowStatBox),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_MEAN,CALCWindow::onCmdMean),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_SUM,CALCWindow::onCmdSum),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_STANDARDDEV,CALCWindow::onCmdStandardDev),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_ADDSTAT,CALCWindow::onCmdAddStat),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_MEAN,CALCWindow::ID_ADDSTAT,CALCWindow::onUpdStatButtons),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_0,CALCWindow::ID_9,CALCWindow::onCmdDigit),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_MULTIPLY,CALCWindow::ID_EQUAL,CALCWindow::onCmdOperator),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_SQRT,CALCWindow::onCmdSqrt),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_PERCENT,CALCWindow::onCmdPercent),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_NOT,CALCWindow::onCmdNot),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_TRUNCATE,CALCWindow::onCmdTruncate),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_UMINUS,CALCWindow::onCmdUnaryMinus),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DECPOINT,CALCWindow::onCmdDecPoint),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_LEFTSHIFT,CALCWindow::onUpdLeftShift),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_TRUNCATE,CALCWindow::onUpdTruncate),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_A,CALCWindow::ID_F,CALCWindow::onCmdHexDigit),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_A,CALCWindow::ID_F,CALCWindow::onUpdHexDigit),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_8,CALCWindow::ID_9,CALCWindow::onUpdOctDigit),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_2,CALCWindow::ID_7,CALCWindow::onUpdBinDigit),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_FIXED,CALCWindow::onCmdFixed),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DMS,CALCWindow::onCmdDMS),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_SIN,CALCWindow::onCmdSin),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_COS,CALCWindow::onCmdCos),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_TAN,CALCWindow::onCmdTan),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_FIXED,CALCWindow::onUpdFixed),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_DMS,CALCWindow::onUpdDMS),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_SIN,CALCWindow::onUpdSin),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_COS,CALCWindow::onUpdCos),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_TAN,CALCWindow::onUpdTan),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_LPAREN,CALCWindow::onCmdLParen),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_EXPONENT,CALCWindow::onCmdExponent),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_POWY,CALCWindow::onCmdPowY),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_POW3,CALCWindow::onCmdPow3),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_POW2,CALCWindow::onCmdPow2),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_EXPONENT,CALCWindow::onUpdExponent),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_POWY,CALCWindow::onUpdPowY),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_POW3,CALCWindow::onUpdPow3),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_POW2,CALCWindow::onUpdPow2),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_RPAREN,CALCWindow::onCmdRParen),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_LN,CALCWindow::onCmdLn),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_LOG,CALCWindow::onCmdLog),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_FACTORIAL,CALCWindow::onCmdFactorial),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_INVERT,CALCWindow::onCmdInvert),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_LN,CALCWindow::onUpdLn),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_LOG,CALCWindow::onUpdLog),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_MEMORYCLEAR,CALCWindow::onCmdMemoryClear),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_MEMORYRECALL,CALCWindow::onCmdMemoryRecall),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_MEMORYSTORE,CALCWindow::onCmdMemoryStore),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_MEMORYADD,CALCWindow::onCmdMemoryAdd),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_PI,CALCWindow::onCmdPI),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_PI,CALCWindow::onUpdPI),

  //The popup dialog
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,CALCWindow::ID_LCD,CALCWindow::onPopupMenu),
  FXMAPFUNCS(SEL_RIGHTBUTTONRELEASE,CALCWindow::ID_BINARY,CALCWindow::ID_PI,CALCWindow::onPopupMenu),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_CLARIFY,CALCWindow::onCmdClarify)
};

FXIMPLEMENT(CALCWindow,FXMainWindow,CALCWindowMap,ARRAYNUMBER(CALCWindowMap))

CALCWindow::CALCWindow(FXApp* app)
: FXMainWindow(app,"fxcalc",NULL,NULL,DECOR_TITLE|DECOR_MINIMIZE|DECOR_CLOSE|DECOR_BORDER|DECOR_MENU),
  tooltip(NULL),
  op(0),
  inv(FALSE),
  hyp(FALSE),
  digitgrouping(FALSE),
  started(FALSE),
  mantissa(FALSE),
  exponent(FALSE),
  value(0.0),
  memory(0.0),
  mode(CALC_SCIENTIFIC),
  base(BASE_TEN),
  rep(DEGREES),
  word(NUM_DLWORD),
  notation(NOTATION_NONE),
  ndigits(CALCDBL_DIG),
  clarify(FALSE)
{
  statBox=new CALCStatBox(app);

  bigcalc=new FXBMPIcon(getApp(),big_calc,0,IMAGE_ALPHAGUESS);
  smallcalc=new FXBMPIcon(getApp(),small_calc,0,IMAGE_ALPHAGUESS);
  setIcon(bigcalc);
  setMiniIcon(smallcalc);

  storePair.op=ID_ADD;
  storePair.value=0.0;

  FXMenubar* menu=new FXMenubar(this);
  filemenu=new FXMenuPane(this);
  new FXMenuTitle(menu,"&File",NULL,filemenu);
  new FXMenuCommand(filemenu,"&Quit\tCtrl+Q",NULL,this,ID_QUIT);
  editmenu=new FXMenuPane(this);
  new FXMenuTitle(menu,"&Edit",NULL,editmenu);
  new FXMenuCommand(editmenu,"&Copy\tCtrl+C",NULL,this,ID_COPY);
  new FXMenuCommand(editmenu,"&Paste\tCtrl+V",NULL,this,ID_PASTE);
  new FXMenuSeparator(editmenu);
  new FXMenuCommand(editmenu,"&Display Settings...\tCtrl+E",NULL,this,ID_NUMDIGITS);
  viewmenu=new FXMenuPane(this);
  new FXMenuTitle(menu,"&View",NULL,viewmenu);
  new FXMenuCommand(viewmenu,"&Standard",NULL,this,ID_STANDARD);
  new FXMenuCommand(viewmenu,"S&cientific",NULL,this,ID_SCIENTIFIC);
  extrasSep=new FXMenuSeparator(viewmenu);
  hexCommand=new FXMenuCommand(viewmenu,"&Hex",NULL,this,ID_HEXADECIMAL);
  decCommand=new FXMenuCommand(viewmenu,"&Dec",NULL,this,ID_DECIMAL);
  octCommand=new FXMenuCommand(viewmenu,"&Oct",NULL,this,ID_OCTAL);
  binCommand=new FXMenuCommand(viewmenu,"&Bin",NULL,this,ID_BINARY);
  new FXMenuSeparator(viewmenu);
  new FXMenuCommand(viewmenu,"Digit &Grouping",NULL,this,ID_DIGITGROUPING);

  helpmenu=new FXMenuPane(this);
  new FXMenuTitle(menu,"&Help",NULL,helpmenu);
  new FXMenuCommand(helpmenu,"&Tool tips\tCtrl+B",NULL,this,ID_USETOOLTIPS);
  new FXMenuSeparator(helpmenu);
  new FXMenuCommand(helpmenu,"&About",smallcalc,this,ID_ABOUT);

  FXHorizontalFrame* lcdframe=new FXHorizontalFrame(this,LAYOUT_FILL_X,0,0,0,0, 4,4,2,2);
  lcd=new CALCLabel(lcdframe,"0.",NULL,JUSTIFY_RIGHT|FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X);
  lcd->setTarget(this);
  lcd->setSelector(ID_LCD);
  lcd->setTipText(LCD_HELP);
  lcd->setBackColor(getApp()->getBackColor());
  lcd->enable(); //So it gets the mouse clicks

  modesFrame=new FXHorizontalFrame(this,LAYOUT_FILL_X,0,0,0,0, 4,4,2,2, 2,2);
  FXHorizontalFrame* baseFrame=new FXHorizontalFrame(modesFrame,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXRadioButton* r=new FXRadioButton(baseFrame,"Hex\t"HEXADECIMAL_HELP,this,ID_HEXADECIMAL,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F5);
  r=new FXRadioButton(baseFrame,"Dec\t"DECIMAL_HELP,this,ID_DECIMAL,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F6);
  r=new FXRadioButton(baseFrame,"Oct\t"OCTAL_HELP,this,ID_OCTAL,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F7);
  r=new FXRadioButton(baseFrame,"Bin\t"BINARY_HELP,this,ID_BINARY,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F8);
  modeSwitch=new FXSwitcher(modesFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);
  FXHorizontalFrame* repFrame=new FXHorizontalFrame(modeSwitch,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  r=new FXRadioButton(repFrame,"Degrees\t"DEGREES_HELP,this,ID_DEGREES,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F2);
  r=new FXRadioButton(repFrame,"Radians\t"RADIANS_HELP,this,ID_RADIANS,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F3);
  r=new FXRadioButton(repFrame,"Gradients\t"GRADIENTS_HELP,this,ID_GRADIENTS,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F4);
  FXHorizontalFrame* wordFrame=new FXHorizontalFrame(modeSwitch,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  r=new FXRadioButton(wordFrame,"dlword\t"DLWORD_HELP,this,ID_DLWORD,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F12);
  r=new FXRadioButton(wordFrame,"lword\t"LWORD_HELP,this,ID_LWORD,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F2);
  r=new FXRadioButton(wordFrame,"word\t"WORD_HELP,this,ID_WORD,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F3);
  r=new FXRadioButton(wordFrame,"byte\t"BYTE_HELP,this,ID_BYTE,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  r->addHotKey(KEY_F4);

  FXHorizontalFrame* numpad=new FXHorizontalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,2,2, FRAMEGAP,0);

  scientificFrame=new FXHorizontalFrame(numpad,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXVerticalFrame* leftSci=new FXVerticalFrame(scientificFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXHorizontalFrame* optFrame=new FXHorizontalFrame(leftSci,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXCheckButton* c=new FXCheckButton(optFrame,"Inv\t"INV_HELP,this,ID_INV,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  c->addHotKey(KEY_i);
  c->addHotKey(KEY_I);
  c=new FXCheckButton(optFrame,"Hyp\t"HYP_HELP,this,ID_HYP,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  c->addHotKey(KEY_h);
  c->addHotKey(KEY_H);
  FXHorizontalFrame* sciButtons=new FXHorizontalFrame(leftSci,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, FRAMEGAP,0);
  FXVerticalFrame* statFrame=new FXVerticalFrame(sciButtons,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXButton* b=new FXButton(statFrame,"Sta\t"SHOWSTATBOX_HELP,NULL,this,ID_SHOWSTATBOX,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_s,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_S,CONTROLMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(statFrame,"Ave\t"MEAN_HELP,NULL,this,ID_MEAN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_a,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_A,CONTROLMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(statFrame,"Sum\t"SUM_HELP,NULL,this,ID_SUM,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_t,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_T,CONTROLMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(statFrame,"s\t"STANDARDDEV_HELP,NULL,this,ID_STANDARDDEV,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_d,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_D,CONTROLMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(statFrame,"Dat\t"ADDSTAT_HELP,NULL,this,ID_ADDSTAT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_Insert);
  b->setTextColor(FUNCTION);
  FXMatrix* sciMatrix=new FXMatrix(sciButtons,5,MATRIX_BY_ROWS|PACK_UNIFORM_WIDTH|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(sciMatrix,"F-E\t"FIXED_HELP,NULL,this,ID_FIXED,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_v);
  b->addHotKey(KEY_V);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"dms\t"DMS_HELP,NULL,this,ID_DMS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_m);
  b->addHotKey(KEY_M);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"sin\t"SIN_HELP,NULL,this,ID_SIN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_s);
  b->addHotKey(KEY_S);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"cos\t"COS_HELP,NULL,this,ID_COS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_o);
  b->addHotKey(KEY_O);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"tan\t"TAN_HELP,NULL,this,ID_TAN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_t);
  b->addHotKey(KEY_T);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"(\t"LPAREN_HELP,NULL,this,ID_LPAREN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_parenleft,SHIFTMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"exp\t"EXPONENT_HELP,NULL,this,ID_EXPONENT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_x);
  b->addHotKey(KEY_X);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"x^y\t"POWY_HELP,NULL,this,ID_POWY,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_y);
  b->addHotKey(KEY_Y);
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"x^3\t"POW3_HELP,NULL,this,ID_POW3,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_numbersign,SHIFTMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(sciMatrix,"x^2\t"POW2_HELP,NULL,this,ID_POW2,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_at,SHIFTMASK));
  b->setTextColor(FUNCTION);
  FXVerticalFrame* rightSci=new FXVerticalFrame(scientificFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXVerticalFrame* plFrame=new FXVerticalFrame(rightSci,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXLabel* parenLabel=new FXLabel(plFrame," ",NULL,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  parenLabel->setTarget(this);
  parenLabel->setSelector(ID_PARENLABEL);
  parenLabel->setTipText(PARENLABEL_HELP);
  parenLabel->enable(); //So it gets the mouse clicks
  FXVerticalFrame* pbFrame=new FXVerticalFrame(rightSci,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(pbFrame,")\t"RPAREN_HELP,NULL,this,ID_RPAREN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_parenright,SHIFTMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(pbFrame,"ln\t"LN_HELP,NULL,this,ID_LN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_n);
  b->addHotKey(KEY_N);
  b->setTextColor(FUNCTION);
  b=new FXButton(pbFrame,"log\t"LOG_HELP,NULL,this,ID_LOG,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_l);
  b->addHotKey(KEY_L);
  b->setTextColor(FUNCTION);
  b=new FXButton(pbFrame,"n!\t"FACTORIAL_HELP,NULL,this,ID_FACTORIAL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_exclam,SHIFTMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(pbFrame,"1/x\t"INVERT_HELP,NULL,this,ID_INVERT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_r);
  b->addHotKey(KEY_R);
  b->setTextColor(FUNCTION);

  FXHorizontalFrame* standardFrame=new FXHorizontalFrame(numpad,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, FRAMEGAP,0);
  FXVerticalFrame* leftStan=new FXVerticalFrame(standardFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXVerticalFrame* mlFrame=new FXVerticalFrame(leftStan,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXLabel* memLabel=new FXLabel(mlFrame," ",NULL,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  memLabel->setTarget(this);
  memLabel->setSelector(ID_MEMORYLABEL);
  memLabel->setTipText(MEMORYLABEL_HELP);
  memLabel->enable(); //So it gets the mouse clicks
  FXVerticalFrame* mbFrame=new FXVerticalFrame(leftStan,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(mbFrame,"MC\t"MEMORYCLEAR_HELP,NULL,this,ID_MEMORYCLEAR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_l,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_L,CONTROLMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(mbFrame,"MR\t"MEMORYRECALL_HELP,NULL,this,ID_MEMORYRECALL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_r,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_R,CONTROLMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(mbFrame,"MS\t"MEMORYSTORE_HELP,NULL,this,ID_MEMORYSTORE,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_m,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_M,CONTROLMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(mbFrame,"M+\t"MEMORYADD_HELP,NULL,this,ID_MEMORYADD,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_p,CONTROLMASK));
  b->addHotKey(MKUINT(KEY_P,CONTROLMASK));
  b->setTextColor(OPERATOR);
  pi=new FXButton(mbFrame,"PI\t"PI_HELP,NULL,this,ID_PI,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  pi->addHotKey(KEY_p);
  pi->addHotKey(KEY_P);
  pi->setTextColor(OPERATOR);

  FXVerticalFrame* rightStan=new FXVerticalFrame(standardFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXHorizontalFrame* controlFrame=new FXHorizontalFrame(rightStan,PACK_UNIFORM_WIDTH|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(controlFrame,"Erase\t"BACKSPACE_HELP,NULL,this,ID_BACKSPACE,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  b->addHotKey(KEY_BackSpace);
  b->setTextColor(OPERATOR);
  b=new FXButton(controlFrame,"CE\t"CLEARENTRY_HELP,NULL,this,ID_CLEARENTRY,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  b->addHotKey(KEY_Delete);
  b->addHotKey(KEY_KP_Delete);
  b->setTextColor(OPERATOR);
  b=new FXButton(controlFrame,"C\t"CLEARALL_HELP,NULL,this,ID_CLEARALL,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  b->addHotKey(KEY_Escape);
  b->setTextColor(OPERATOR);
  FXVerticalFrame* stanFrame=new FXVerticalFrame(rightStan,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXHorizontalFrame* stanButtons=new FXHorizontalFrame(stanFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXMatrix* stanMatrix=new FXMatrix(stanButtons,4,MATRIX_BY_COLUMNS|PACK_UNIFORM_WIDTH|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(stanMatrix,"7\t"SEVEN_HELP,NULL,this,ID_7,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_7);
  b->addHotKey(KEY_KP_7);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"8\t"EIGHT_HELP,NULL,this,ID_8,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_8);
  b->addHotKey(KEY_KP_8);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"9\t"NINE_HELP,NULL,this,ID_9,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_9);
  b->addHotKey(KEY_KP_9);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"/\t"DIVIDE_HELP,NULL,this,ID_DIVIDE,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_slash);
  b->addHotKey(KEY_KP_Divide);
  b->setTextColor(OPERATOR);
  b=new FXButton(stanMatrix,"4\t"FOUR_HELP,NULL,this,ID_4,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_4);
  b->addHotKey(KEY_KP_4);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"5\t"FIVE_HELP,NULL,this,ID_5,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_5);
  b->addHotKey(KEY_KP_5);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"6\t"SIX_HELP,NULL,this,ID_6,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_6);
  b->addHotKey(KEY_KP_6);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"*\t"MULTIPLY_HELP,NULL,this,ID_MULTIPLY,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_asterisk,SHIFTMASK));
  b->addHotKey(KEY_KP_Multiply);
  b->setTextColor(OPERATOR);
  b=new FXButton(stanMatrix,"1\t"ONE_HELP,NULL,this,ID_1,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_1);
  b->addHotKey(KEY_KP_1);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"2\t"TWO_HELP,NULL,this,ID_2,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_2);
  b->addHotKey(KEY_KP_2);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"3\t"THREE_HELP,NULL,this,ID_3,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_3);
  b->addHotKey(KEY_KP_3);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"-\t"SUBTRACT_HELP,NULL,this,ID_SUBTRACT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_minus);
  b->addHotKey(KEY_KP_Subtract);
  b->setTextColor(OPERATOR);
  b=new FXButton(stanMatrix,"0\t"ZERO_HELP,NULL,this,ID_0,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_0);
  b->addHotKey(KEY_KP_0);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"+/-\t"UMINUS_HELP,NULL,this,ID_UMINUS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_F9);
  b->setTextColor(OPERATOR);
  b=new FXButton(stanMatrix,".\t"DECPOINT_HELP,NULL,this,ID_DECPOINT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_period);
  b->addHotKey(KEY_KP_Decimal);
  b->addHotKey(KEY_comma);
  b->setTextColor(VALUE);
  b=new FXButton(stanMatrix,"+\t"ADD_HELP,NULL,this,ID_ADD,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_plus,SHIFTMASK));
  b->addHotKey(KEY_KP_Add);
  b->setTextColor(OPERATOR);
  advancedOps=new FXMatrix(stanButtons,4,MATRIX_BY_ROWS|PACK_UNIFORM_WIDTH|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(advancedOps,"mod\t"MODULUS_HELP,NULL,this,ID_MODULUS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_percent,SHIFTMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"or\t"OR_HELP,NULL,this,ID_OR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_bar,SHIFTMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"lsh\t"LEFTSHIFT_HELP,NULL,this,ID_LEFTSHIFT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_less,SHIFTMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"=\t"EQUAL_HELP,NULL,this,ID_EQUAL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_equal);
  b->addHotKey(KEY_Return);
  b->addHotKey(KEY_KP_Enter);
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"and\t"AND_HELP,NULL,this,ID_AND,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_ampersand,SHIFTMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"xor\t"XOR_HELP,NULL,this,ID_XOR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_asciicircum,SHIFTMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"not\t"NOT_HELP,NULL,this,ID_NOT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_asciitilde,SHIFTMASK));
  b->setTextColor(OPERATOR);
  b=new FXButton(advancedOps,"int\t"TRUNCATE_HELP,NULL,this,ID_TRUNCATE,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_semicolon);
  b->setTextColor(OPERATOR);
  simpleOps=new FXVerticalFrame(stanButtons,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  b=new FXButton(simpleOps,"sqrt\t"SQRT_HELP,NULL,this,ID_SQRT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_at,SHIFTMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(simpleOps,"%\t"PERCENT_HELP,NULL,this,ID_PERCENT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(MKUINT(KEY_percent,SHIFTMASK));
  b->setTextColor(FUNCTION);
  b=new FXButton(simpleOps,"1/x\t"INVERT_HELP,NULL,this,ID_INVERT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_r);
  b->addHotKey(KEY_R);
  b->setTextColor(FUNCTION);
  b=new FXButton(simpleOps,"=\t"EQUAL_HELP,NULL,this,ID_EQUAL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_equal);
  b->addHotKey(KEY_Return);
  b->addHotKey(KEY_KP_Enter);
  b->setTextColor(OPERATOR);

  hexFrame=new FXHorizontalFrame(stanFrame,PACK_UNIFORM_WIDTH|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);
  b=new FXButton(hexFrame,"A\t"A_HELP,NULL,this,ID_A,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_a);
  b->addHotKey(KEY_A);
  b->setTextColor(FUNCTION);
  b=new FXButton(hexFrame,"B\t"B_HELP,NULL,this,ID_B,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_b);
  b->addHotKey(KEY_B);
  b->setTextColor(FUNCTION);
  b=new FXButton(hexFrame,"C\t"C_HELP,NULL,this,ID_C,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_c);
  b->addHotKey(KEY_C);
  b->setTextColor(FUNCTION);
  b=new FXButton(hexFrame,"D\t"D_HELP,NULL,this,ID_D,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_d);
  b->addHotKey(KEY_D);
  b->setTextColor(FUNCTION);
  b=new FXButton(hexFrame,"E\t"E_HELP,NULL,this,ID_E,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_e);
  b->addHotKey(KEY_E);
  b->setTextColor(FUNCTION);
  b=new FXButton(hexFrame,"F\t"F_HELP,NULL,this,ID_F,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  b->addHotKey(KEY_f);
  b->addHotKey(KEY_F);
  b->setTextColor(FUNCTION);
}

CALCWindow::~CALCWindow()
{
  delete filemenu;
  delete editmenu;
  delete viewmenu;
  delete helpmenu;
  delete bigcalc;
  delete smallcalc;
  delete statBox;
}

void CALCWindow::create()
{
  FXMainWindow::create();
  FXint x=getApp()->reg().readIntEntry("SETTINGS","x",10);
  FXint y=getApp()->reg().readIntEntry("SETTINGS","y",10);
  FXuint m=getApp()->reg().readUnsignedEntry("SETTINGS","mode",CALC_SCIENTIFIC);
  ndigits=getApp()->reg().readUnsignedEntry("SETTINGS","digits",ndigits);  //Use default initialized value if it's not present
  if(getApp()->reg().readIntEntry("SETTINGS","tooltips",FALSE))
    handle(this,MKUINT(ID_USETOOLTIPS,SEL_COMMAND),NULL);

  //Make sure it's within a valid range
  //Digit box gives a leeway of 2, so this value is considered valid
  //but if it's any larger than that we make it equal to the float.h size
  if(ndigits>CALCDBL_DIG+2)
    ndigits=CALCDBL_DIG;

  move(x,y);
  setMode(m);
}

void CALCWindow::setMode(FXuint m)
{
  if(m==CALC_STANDARD)
  {
    extrasSep->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    hexCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    decCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    octCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    binCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    modesFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    scientificFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    pi->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    advancedOps->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    simpleOps->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    hexFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
  }
  else
  {
    extrasSep->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    hexCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    decCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    octCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    binCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    modesFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    scientificFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    pi->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    advancedOps->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    simpleOps->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    hexFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
  }

  resize(getDefaultWidth(),getDefaultHeight());
  handle(this,MKUINT(ID_CLEARALL,SEL_COMMAND),NULL);
  base=BASE_TEN;
  rep=DEGREES;
  mode=m;
}

FXuint CALCWindow::getMode() const
{
  return mode;
}

void CALCWindow::setBase(FXuint b)
{
  FXint which=(b==BASE_TEN)?0:1;
  CALCdouble val=getLabelText();
  base=b;
  modeSwitch->handle(this,MKUINT(FXSwitcher::ID_OPEN_FIRST+which,SEL_COMMAND),NULL);
  setLabelText(val);
  started=FALSE;
}

FXuint CALCWindow::getBase() const
{
  return base;
}

void CALCWindow::setRepresentation(FXuint r)
{
  rep=r;
}

FXuint CALCWindow::getRepresentation() const
{
  return rep;
}

void CALCWindow::setWordType(FXuint w)
{
  CALCdouble val=getLabelText();
  word=w;
  setLabelText(val);
  started=FALSE;
}

FXuint CALCWindow::getWordType() const
{
  return word;
}

void CALCWindow::setNotation(FXuint n)
{
  CALCdouble val=getLabelText();
  notation=n;
  setLabelText(val);
  started=FALSE;
}

FXuint CALCWindow::getNotation() const
{
  return notation;
}

void CALCWindow::setDigitGrouping(FXbool b)
{
  CALCdouble val=getLabelText();
  digitgrouping=b;
  setLabelText(val);
  started=FALSE;
}

FXbool CALCWindow::getDigitGrouping() const
{
  return digitgrouping;
}

CALCdouble CALCWindow::getLabelText() const
{
  CALCint64 ival=0;
  CALCdouble fval=0.0;
  FXString str=lcd->getText();
  FXint n=0;
  FXint len=str.length();

  //Remove digit grouping stuff
  if(digitgrouping)
  {
    FXuchar dig=(base==BASE_TEN)?',':' ';
    FXint i,j=0;
    for(i=0;i<len;i++)
      if(str[i]!=dig) str[j++]=str[i];
    str[j]='\0';
    len=j;
  }

  if(base==BASE_TWO)
  {
    while(n<len)
      ival=(ival<<1)|(str[n++]-'0');
    fval=calci64todd(ival);
  }
  else if(base==BASE_EIGHT)
  {
    while(n<len)
      ival=(ival<<3)|(str[n++]-'0');
    fval=calci64todd(ival);
  }
  else if(base==BASE_SIXTEEN)
  {
    FXchar c=0;
    while(n<len)
    {
      c=str[n++];
      c-=(isdigit(c))?'0':('A'-10);
      ival=(ival<<4)|c;
    }
    fval=calci64todd(ival);
  }
  else
  {
#ifdef HAVE_DOUBLEDOUBLE
    fval=atodd(str.text());
#else
    sscanf(str.text(),"%lf",&fval);
#endif
  }

  return fval;
}

//FIX ME - notation for e when not in sci mode
void CALCWindow::setLabelText(CALCdouble val)
{
  CALCuint64 ival=calcddtoui64(val);
  FXString str=(ival==0)?"0":"";
  FXint count=0;

  //Make proper word size
  ival&=(0xFFFFFFFF00000000*((word==NUM_DLWORD)?1:0))|(0xFFFF0000*((word>=NUM_LWORD)?1:0))|(0xFF00*((word>=NUM_WORD)?1:0))|(0xFF*((word>=NUM_BYTE)?1:0));

  if((base!=BASE_TEN))
  {
    //No integers larger than 2^64 or our precision
    FXint power=((CALCDBL_MANT_DIG>64)?64:CALCDBL_MANT_DIG);
    if(val<pow((CALCdouble)2.0,(CALCdouble)power))
    {
      if(base==BASE_TWO)
      {
        while(ival!=0)
        {
          if(digitgrouping&&(count++%8==0)&&(count!=1))
            str.prepend(' ');
          str.prepend('0'+((FXchar)ival&0x01));
          ival>>=1;
        }
      }
      else if(base==BASE_EIGHT)
      {
        while(ival!=0)
        {
          if(digitgrouping&&(count++%3==0)&&(count!=1))
            str.prepend(' ');
          str.prepend('0'+((FXchar)ival&0x07));
          ival>>=3;
        }
      }
      else if(base==BASE_SIXTEEN)
      {
        FXchar digits[]="0123456789ABCDEF";
        while(ival!=0)
        {
          if(digitgrouping&&(count++%2==0)&&(count!=1))
            str.prepend(' ');
          str.prepend(digits[((FXchar)ival&0x0F)]);
          ival>>=4;
        }
      }
    }
    else
      str="0";
  }
  else
  {
    if((notation==NOTATION_NONE)/*||(val==0)*/)
    {
#ifdef HAVE_DOUBLEDOUBLE
      str=qtoa(NULL,ndigits,'g',val);
#else
      str.format("%.*lg",ndigits,val);
#endif

      //If there's no '.' add it
      if(str.findb('.')==-1)
      {
        FXint epos=str.findb('e');
        if(epos!=-1)
          str.insert(epos,'.');
        else
          str.append('.');
      }
    }
    else
    {
      FXint len=0,epos=0,pos=0;
#ifdef HAVE_DOUBLEDOUBLE
      str=qtoa(NULL,ndigits,'e',val);
#else
      str.format("%.*le",ndigits,val);
#endif
      
      //remove zeros from sci mode
      len=str.length();
      epos=str.findb('e');

      pos=epos+2;
      while((pos<len-1)&&(str[pos]=='0'))
        str.remove(pos++);

      pos=epos-1;
      while((pos>0)&&(str[pos]=='0'))
        str.remove(pos--);
    }

    if(digitgrouping)
    {
      count=str.findb('.')-3;
      while(count>0)
      {
        str.insert(count,',');
        count-=3;
      }
    }
  }

  lcd->setText(str);
}

long CALCWindow::onCmdQuit(FXObject*,FXSelector,void*)
{
  getApp()->reg().writeIntEntry("SETTINGS","x",getX());
  getApp()->reg().writeIntEntry("SETTINGS","y",getY());
  getApp()->reg().writeUnsignedEntry("SETTINGS","mode",getMode());
  getApp()->reg().writeUnsignedEntry("SETTINGS","digits",ndigits);
  getApp()->reg().writeIntEntry("SETTINGS","tooltips",(tooltip==NULL)?FALSE:TRUE);
  getApp()->reg().write();
  getApp()->stop();
  return 1;
}

long CALCWindow::onClipboardRequest(FXObject* sender,FXSelector sel,void* ptr)
{
  FXEvent *event=(FXEvent*)ptr;

  if(event->target==stringType)
  {
    FXuint len=clipping.length();
    FXuchar* data=NULL;
    FXCALLOC(&data,FXuchar,len+1);
    if(!data)
      fxwarning("%s::onClipboardRequest: out of memory\n",getClassName()); 
    else
    {
      memcpy(data,clipping.text(),len);
      setDNDData(FROM_CLIPBOARD,stringType,data,len+1);
    }
    return 1;
  }
  return 0;
}

long CALCWindow::onCmdCopy(FXObject*,FXSelector,void*)
{
  FXDragType types[]={stringType};
  acquireClipboard(types,1);
  clipping=lcd->getText();
  return 1;
}

void CALCWindow::pasteData(FXuchar* data,FXuint len)
{
  FXuchar c;
  FXuint n;
  FXbool neg=FALSE;

  handle(this,MKUINT(ID_CLEARENTRY,SEL_COMMAND),NULL);
  started=TRUE;
  for(n=0;n<len;n++)
  {
    if(data[n]=='\0') break;

    if(isdigit(data[n]))
    {
      if(((base==BASE_EIGHT)&&('7'<data[n]))||((base==BASE_TWO)&&('1'<data[n])))
        getApp()->beep();
      else
      {
        handle(this,MKUINT(ID_0+(data[n]-'0'),SEL_COMMAND),NULL);
        if(neg)
        {
          handle(this,MKUINT(ID_UMINUS,SEL_COMMAND),NULL);
          neg=FALSE;
        }
      }
    }
    else if(isalpha(data[n]))
    {
      c=toupper(data[n]);
      if(('E'==c)&&(base==BASE_TEN)&&((n+1)<len)&&((data[n+1]=='-')||(data[n+1]=='+')))
      {
        handle(this,MKUINT(ID_EXPONENT,SEL_COMMAND),NULL);
        if(data[++n]=='-')
          handle(this,MKUINT(ID_UMINUS,SEL_COMMAND),NULL);
      }
      else if(('A'<=c)&&('F'>=c)&&(!mantissa)&&(!exponent)&&(base==BASE_SIXTEEN))
        handle(this,MKUINT(ID_A+(c-'A'),SEL_COMMAND),NULL);
      else
        getApp()->beep();
    }
    else if((n==0)&&(data[n]=='-')&&(base==BASE_TEN))
      neg=TRUE;
    else if((data[n]=='.')&&(base==BASE_TEN))
    {
      //We don't enter mantissa mode if there's no mantissa
      if(((n+1)<len)&&(data[n+1]!='\0'))
        handle(this,MKUINT(ID_DECPOINT,SEL_COMMAND),NULL);
    }
    else
      getApp()->beep();
  }
}

//Message is forwarded here from the CALCLabel
long CALCWindow::onDNDDrop(FXObject* sender,FXSelector sel,void* ptr){
  FXuchar *data,*junk; FXuint len,dum;

  if(getDNDData(FROM_DRAGNDROP,textType,data,len))
  {
    // Need to ask the source to delete his copy
    if(inquireDNDAction()==DRAG_MOVE)
    {
      getDNDData(FROM_DRAGNDROP,deleteType,junk,dum);
      FXASSERT(!junk);
    }

    pasteData(data,len);
    FXFREE(&data);
  }
  return 1;
}

//Message is forwarded here from the CALCLabel
long CALCWindow::onDNDRequest(FXObject* sender,FXSelector sel,void* ptr)
{
  FXEvent *event=(FXEvent*)ptr;
  FXuchar *data; FXuint len;

  if(event->target==textType)
  {
    FXString str=lcd->getText();
    len=str.length();
    FXMALLOC(&data,FXuchar,len);
    memcpy((FXchar*)data,str.text(),len);
    setDNDData(FROM_DRAGNDROP,textType,data,len);
    return 1;
  }

  return 0;
 }

//FIX ME - should this also interpret commands.  I don't think so.
long CALCWindow::onCmdPaste(FXObject*,FXSelector,void*)
{
  FXuchar* data=NULL;
  FXuint len=0;
  if(getDNDData(FROM_CLIPBOARD,stringType,data,len))
  {
    pasteData(data,len);
    FXFREE(&data);
  }

  return 1;
}

long CALCWindow::onUpdPaste(FXObject* sender,FXSelector,void*)
{
  FXuchar* data=NULL;
  FXuint size=0;
  FXuint msg=ID_DISABLE;

  if(getDNDData(FROM_CLIPBOARD,stringType,data,size))
  {
    msg=ID_ENABLE;
    FXFREE(&data);
  }
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);

  return 1;
}

long CALCWindow::onCmdNumDigits(FXObject*,FXSelector,void*)
{
  CALCDigitBox dialog(this,"Display Settings");
  dialog.setNumDigits(ndigits);
  if(dialog.execute())
  {
    ndigits=dialog.getNumDigits();
    //Refresh display
    setLabelText(getLabelText());
  }
  return 1;
}

long CALCWindow::onCmdDigitGrouping(FXObject*,FXSelector,void*)
{
  setDigitGrouping(!digitgrouping);
  return 1;
}

long CALCWindow::onUpdDigitGrouping(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(digitgrouping)?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdUseTooltips(FXObject*,FXSelector,void*)
{
  if(tooltip==NULL)
  {
    tooltip=new FXTooltip(getApp(),TOOLTIP_PERMANENT);
    tooltip->create();
  }
  else
  {
    delete tooltip;
    tooltip=NULL;
  }

  return 1;
}

long CALCWindow::onUpdUseTooltips(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(tooltip==NULL)?ID_UNCHECK:ID_CHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdAbout(FXObject*,FXSelector,void*)
{
  FXString msg;
  FXString bits("???");
  if(CALCDBL_MANT_DIG==53)
    bits="64";
  else if(CALCDBL_MANT_DIG==64)
    bits="80";
  else if((CALCDBL_MANT_DIG==113)||(CALCDBL_MANT_DIG==107))
    bits="128";
  else if(CALCDBL_MANT_DIG==24)
    bits="32";

  msg.format("Scientific Calculator (IEEE 754 %s-bit floating point)\nVersion "VERSION"\n\nCopyright (C) 2000,2001 Dustin Graves (dgraves@computer.org)\n\n"\
"This software uses the FOX Platform Independent GUI Toolkit Library.\n"\
"The FOX Library is Copyright (C) 1997,2000,2001 Jeroen van der Zijp and is\n"\
"available freely under the GNU Lesser Public License at the following site:\n"\
"http://www.cfdrc.com/FOX/fox.html",bits.text());

#ifdef HAVE_DOUBLEDOUBLE
  msg.append("\n\n"\
"This software uses version 2.2 of the `doubledouble' software package which\n"\
"implements doubled-double (approximately 30 decimal place) floating point\n"\
"arithmetic on IEEE 754 floating-point hardware. \n"\
"The doubledouble library is Copyright (C) 1997 Keith Martin Briggs and is\n"\
"available freely under the GNU General Public License at the following site:\n"\
"http://www.btexact.com/people/briggsk2/doubledouble.html");
#endif

  FXMessageBox about(this,"About fxcalc",msg,bigcalc,MBOX_OK|DECOR_TITLE|DECOR_BORDER);
  about.execute();  
  return 1;
}

long CALCWindow::onCmdGetLCDText(FXObject* sender,FXSelector sel,void* ptr)
{
  *((FXString*)ptr)=lcd->getText();
  return 1;
}

long CALCWindow::onCmdSetLCDText(FXObject* sender,FXSelector sel,void* ptr)
{
  lcd->setText(*((FXString*)ptr));
  return 1;
}

long CALCWindow::onCmdPasteLCDText(FXObject* sender,FXSelector sel,void* ptr)
{
  FXString* str=(FXString*)ptr;
  FXuint len=str->length();
  FXuchar* data=NULL;
  FXMALLOC(&data,FXuchar,len);
  memcpy((FXchar*)data,str->text(),len);
  pasteData(data,len);
  FXFREE(&data);
  return 1;
}

long CALCWindow::onCmdGetLCDValue(FXObject* sender,FXSelector sel,void* ptr)
{
  *((CALCdouble*)ptr)=getLabelText();
  return 1;
}

long CALCWindow::onCmdSetLCDValue(FXObject* sender,FXSelector sel,void* ptr)
{
  setLabelText(*((CALCdouble*)ptr));
  return 1;
}

long CALCWindow::onCmdMode(FXObject*,FXSelector sel,void*)
{
  setMode(CALC_STANDARD+(SELID(sel)-ID_STANDARD));
  return 1;
}

long CALCWindow::onUpdMode(FXObject* sender,FXSelector sel,void*)
{
  FXuint msg=(mode==((FXuint)(CALC_STANDARD+(SELID(sel)-ID_STANDARD))))?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdBase(FXObject*,FXSelector sel,void*)
{
  setBase(0x01<<(SELID(sel)-ID_BINARY));
  return 1;
}

long CALCWindow::onUpdBase(FXObject* sender,FXSelector sel,void*)
{
  FXuint msg=(base==((FXuint)(0x01<<(SELID(sel)-ID_BINARY))))?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}


long CALCWindow::onCmdRep(FXObject*,FXSelector sel,void*)
{
  setRepresentation(0x01<<(SELID(sel)-ID_DEGREES));
  return 1;
}

long CALCWindow::onUpdRep(FXObject* sender,FXSelector sel,void*)
{
  FXuint msg=(rep==((FXuint)(0x01<<(SELID(sel)-ID_DEGREES))))?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdWordType(FXObject*,FXSelector sel,void*)
{
  setWordType(0x01<<(SELID(sel)-ID_BYTE));
  return 1;
}

long CALCWindow::onUpdWordType(FXObject* sender,FXSelector sel,void*)
{
  FXuint msg=(word==((FXuint)(0x01<<(SELID(sel)-ID_BYTE))))?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdInv(FXObject*,FXSelector,void*)
{
  setLabelText(getLabelText());  //Process the exponent
  inv=!inv;
  return 1;
}

long CALCWindow::onCmdHyp(FXObject*,FXSelector,void*)
{
  setLabelText(getLabelText());  //Process the exponent
  hyp=!hyp;
  return 1;
}

long CALCWindow::onUpdInv(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(inv)?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdHyp(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(hyp)?ID_CHECK:ID_UNCHECK;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdParenLabel(FXObject* sender,FXSelector,void*)
{
  FXString str(" ");
  if(stack.no()>0)
    str.format("(=%d",stack.no());
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&str);
  return 1;
}

long CALCWindow::onUpdMemoryLabel(FXObject* sender,FXSelector,void*)
{
  FXString str(" ");
  if(memory!=((CALCdouble)0.0))
    str="M";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&str);
  return 1;
}

long CALCWindow::onCmdBackSpace(FXObject*,FXSelector,void*)
{
  FXString val=lcd->getText();
  FXint pos=val.length()-1;
  FXchar last=val[pos];

  if(!started)
  {
    getApp()->beep();
  }
  else
  {
    if(exponent)
    {
      val.remove(pos);
      if((val[pos-1]=='+')||(val[pos-1]=='-'))
      {
        //Do we remove the exp altogether or is it zero?
        if(last=='0')
        {
          val.remove(pos-2,2);
          exponent=FALSE;
        }
        else
          val.append('0');
      }
    }
    else if(mantissa)
    {
      //Check for special case
      if(last=='.')
        mantissa=FALSE;
      else
        val.remove(pos);
    }
    else
    {
      if(base==BASE_TEN)
        pos-=1;

      //Remove digit before '.'
      if((pos)==0)
        val[pos]='0';
      else
      {
        val.remove(pos);

        //Remove digit grouping and then re add it
        if(digitgrouping)
        {
          FXuchar grp=(base==BASE_TEN)?',':' ';
          FXint gap;
          FXint i,j=0,len=val.length();
          for(i=0;i<len;i++)
          if(val[i]!=grp) val[j++]=val[i];
            val[j]='\0';
          pos=(base==BASE_TEN)?j-1:j;

          switch(base)
          {
          case BASE_TWO:
            gap=8;
            break;
          case BASE_SIXTEEN:
            gap=2;
            break;
          case BASE_EIGHT:
          case BASE_TEN:
            gap=3;
          }
          pos-=gap;

          while(pos>0)
          {
            val.insert(pos,grp);
            pos-=gap;
          }
        }
      }
    }
  }
  lcd->setText(val);
  return 1;
}

long CALCWindow::onCmdClearEntry(FXObject*,FXSelector,void*)
{
  exponent=mantissa=started=FALSE;
  setLabelText(0);
  return 1;
}

long CALCWindow::onCmdClearAll(FXObject*,FXSelector,void*)
{
  //Clear value, op, and set notation back to normal
  exponent=mantissa=started=FALSE;
  value=0.0;
  op=0;
  storePair.op=0;
  storePair.value=0.0;
  notation=NOTATION_NONE;
  setLabelText(0);
  return 1;
}

long CALCWindow::onCmdShowStatBox(FXObject*,FXSelector,void*)
{
  if(statBox->shown())
    statBox->raise();
  else
    statBox->show();
  return 1;
}

long CALCWindow::onCmdMean(FXObject*,FXSelector,void*)
{
  try
  {
    if(inv)
      setLabelText(statBox->getSquaresMean());
    else
      setLabelText(statBox->getMean());
  }
  catch(FXErrorException err)
  {
    lcd->setText(err.what());
  }

  started=FALSE;
  return 1;
}

long CALCWindow::onCmdSum(FXObject*,FXSelector,void*)
{
  if(inv)
    setLabelText(statBox->getSquaresSum());
  else
    setLabelText(statBox->getSum());
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdStandardDev(FXObject*,FXSelector,void*)
{
  try
  {
    if(inv)
      setLabelText(statBox->getStandardDevN());
    else
      setLabelText(statBox->getStandardDevN1());
  }
  catch(FXErrorException err)
  {
    lcd->setText(err.what());
  }

  started=FALSE;
  return 1;
}

long CALCWindow::onCmdAddStat(FXObject*,FXSelector,void*)
{
  statBox->addData(lcd->getText(),getLabelText());
  started=FALSE;
  return 1;
}

long CALCWindow::onUpdStatButtons(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(statBox->shown())?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

FXbool CALCWindow::octIsValid(FXString& str,FXint max)
{
  FXbool success=TRUE;
  FXint length=str.length()-str.count(' ');
  if(length==max-1)
  {
    switch(word)
    {
    case NUM_BYTE:
    case NUM_LWORD:
      if(str[0]>'3')
        success=FALSE;
      break;
    case NUM_WORD:
    case NUM_DLWORD:
      if(str[0]>'1')
        success=FALSE;
      break;
    }
  }
  return success;
}

//FIX ME - Test exp value against CALCDBL_MAX_EXP and CALCDBL_MIN_EXP
//FIX ME - Calculation of digit limit for BASE_TEN
long CALCWindow::onCmdDigit(FXObject*,FXSelector sel,void*)
{
  FXString val=lcd->getText();
  FXint pos=val.length()-1;
  FXint max=(val[0]=='-')?33:32;
  FXchar dig='0'+(SELID(sel)-ID_0);

  if(!started)
  {
    started=TRUE;
    exponent=mantissa=FALSE;
    val.format("%c",dig);
    if(base==BASE_TEN)
      val.append(".");
    lcd->setText(val);
    return 1;
  }

  //Determine max number of digits
  switch(base)
  {
  case BASE_TWO:
    max=word*8;
    break;
  case BASE_EIGHT:
    max=word*3;
    if(word==NUM_LWORD)
      max-=1;
    else if(word==NUM_DLWORD)
      max-=2;
    break;
  case BASE_SIXTEEN:
    max=word*2;
    break;
  }

  if(exponent)
  {
    //Allow 4 exp digits for now - this is too many for 64-bit floats
    if((pos-(val.findb('e')+1))==4)
      getApp()->beep();
    else
    {
      if((val[pos]=='0')&&((val[pos-1]=='+')||(val[pos-1]=='-')))
        val.replace(pos,dig);
      else
        val.append(dig);
    }
  }
  else if(mantissa)
  {
    if((pos+1)<(max+2))  //Plus two for '.' and one leading digit
      val.append(dig);
    else
      getApp()->beep();
  }
  else
  {
    if(base==BASE_TEN)
      pos-=1;

    FXuchar grp=(base==BASE_TEN)?',':' ';

    //For Octal there is an upper bound on the left most digit
    if((((pos-val.count(grp))+1)<max)&&(octIsValid(val,max)||(base!=BASE_EIGHT)))
    {
      //A label with '0' is a special case
      if(((pos+1)==1)&&(val[pos]=='0'))
      {
        if(dig!='0')
          val[pos]=dig;
      }
      else
      {
        if(digitgrouping)
        {
          FXint i,j=0,len=val.length();
          for(i=0;i<len;i++)
          if(val[i]!=grp) val[j++]=val[i];
            val[j]='\0';
          pos=(base==BASE_TEN)?j-2:j-1;
        }

        val.insert(pos+1,dig);

        if(digitgrouping)
        {
          FXint count;
          FXint gap;

          switch(base)
          {
          case BASE_TWO:
            gap=8;
            count=pos-6;
            break;
          case BASE_SIXTEEN:
            gap=2;
            count=pos;
            break;
          case BASE_EIGHT:
          case BASE_TEN:
            gap=3;
            count=pos-1;
          }

          while(count>0)
          {
            val.insert(count,grp);
            count-=gap;
          }
        }
      }
    }
    else
      getApp()->beep();
  }

  lcd->setText(val);
  return 1;
}

CALCdouble CALCWindow::truncate(const CALCdouble& x)
{
  return (x<((CALCdouble)0.0))?-floor(-x):floor(x);
}

CALCdouble CALCWindow::processOp(FXuint op,CALCdouble val)
{
  CALCdouble result=value;
  CALCint64 bits=calcddtoi64(truncate(result));

  switch(op)
  {
  case ID_MULTIPLY:
    result*=val;
    break;
  case ID_DIVIDE:
    if(val==((CALCdouble)0.0))
      throw FXErrorException("Divide by zero error.");
    result/=val;
    break;
  case ID_ADD:
    result+=val;
    break;
  case ID_SUBTRACT:
    result-=val;
    break;
  case ID_MODULUS:
    if(val==((CALCdouble)0.0))
      throw FXErrorException("Divide by zero error.");
//Doing conversions to i64 and back again may be better than floors, in cases of precision errors with mantissas of -0.3e-16 or the like, etc
//    result-=calcddtoi64(truncate(result)/val)*val;
    result-=truncate(truncate(result)/val)*val;
    break;
  case ID_AND:
    bits&=calcddtoi64(val);
    result=calci64todd(bits);
    break;
  case ID_OR:
    bits|=calcddtoi64(val);
    result=calci64todd(bits);
    break;
  case ID_XOR:
    bits^=calcddtoi64(val);
    result=calci64todd(bits);
    break;
  case ID_LEFTSHIFT:
    if(inv)
      bits>>=calcddtoi64(val);
    else
      bits<<=calcddtoi64(val);
    result=calci64todd(bits);
    break;
  case 0:
  default:
    result=val;
  }
  return result;
}

//doubledouble has some major pow issues regarding negative numbers
//pow(-4,2) raises an error, because the doubledouble pow implemntation is
//simply y*log(x) and x can not be <= 0
CALCdouble CALCWindow::powY(const CALCdouble& x,const CALCdouble& y)
{
  //Corrections for the pow problem
#ifdef HAVE_DOUBLEDOUBLE
  if(x==((CALCdouble)0.0))
    return (CALCdouble)0.0;

  FXbool neg=FALSE;
  FXbool reneg=FALSE;
  if(x<((CALCdouble)0.0))
  {
    if((calcddtoi64(y)%2)==0)
    {
      if(inv)
        throw FXErrorException("Invalid value for operation.");
      reneg=FALSE;  //even exponent always makes even result
    }
    else
      reneg=TRUE;
    neg=TRUE;
  }

  CALCdouble r=pow((neg)?(x*-1.0):x,(inv)?(1.0/y):y);
  return (reneg)?(r*-1.0):r;
#else
  if(inv)
  {
    if((x<((CALCdouble)0.0))&&(calcddtoi64(y)%2==0))
      throw FXErrorException("Invalid value for operation.");
    return pow(x,1.0/y);
  }
  else
    return pow(x,y);
#endif
}

long CALCWindow::onCmdOperator(FXObject*,FXSelector sel,void*)
{
  CALCdouble val=getLabelText();

  try
  {
    if(op==ID_POWY)
    {
      CALCdouble powy=powY(storePair.value,val);
      value=processOp(storePair.op,powy);
    }
    else if(op==ID_EQUAL&&SELID(sel)==ID_EQUAL)  //repeat the powy
    {
      if(storePair.op==ID_POWY)
      {
        value=powY(val,storePair.value);
      }
      else if(storePair.op==0)
        value=val;
      else
        value=processOp(storePair.op,storePair.value);
    }
    else
      value=processOp(op,val);

    started=FALSE;
    //OP to execute when equal is pressed multiple times in a row
    if(op!=ID_EQUAL&&SELID(sel)==ID_EQUAL)
    {
      storePair.op=op;
      storePair.value=val;
    }
    op=SELID(sel);

    setLabelText(value);
  }
  catch(FXErrorException err)
  {
    lcd->setText(err.what());
    started=FALSE;
  }

  return 1;
}

long CALCWindow::onCmdSqrt(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  if(val>=((CALCdouble)0.0))
  {
    val=sqrt(val);
    setLabelText(val);
  }
  else
  {
    lcd->setText("Invalid value for operation.");
  }
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdPercent(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  val=value*(val/100.0);
  setLabelText(val);
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdNot(FXObject*,FXSelector,void*)
{
  CALCint64 val=calcddtoi64(getLabelText());
  val^=(~0);
  setLabelText(calci64todd(val));
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdTruncate(FXObject*,FXSelector,void*)
{
  if(inv)
  {
    CALCdouble val=getLabelText();
    setLabelText(val-truncate(val));
  }
  else
  {
    mantissa=exponent=FALSE;
    setLabelText(truncate(getLabelText()));
  }
  started=FALSE;
  return 1;
}

long CALCWindow::onUpdLeftShift(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"rsh":"lsh";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onUpdTruncate(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"frac":"int";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onCmdUnaryMinus(FXObject*,FXSelector,void*)
{
  if(exponent)
  {
    FXString str=lcd->getText();
    FXint pos=str.findb('e')+1;
    str[pos]=(str[pos]=='+')?'-':'+';
    lcd->setText(str);
  }
  else
  {
    CALCdouble val=getLabelText();
    if(val!=((CALCdouble)0.0))
      val*=-1;
    setLabelText(val);
  }
  return 1;
}

long CALCWindow::onCmdDecPoint(FXObject*,FXSelector,void*)
{
  if(base!=BASE_TEN)
  {
    getApp()->beep();
    return 1;
  }

  //Is it started?
  if(!started)
  {
    lcd->setText("0.");
    started=TRUE;
  }
  mantissa=TRUE;
  return 1;
}

long CALCWindow::onCmdHexDigit(FXObject*,FXSelector sel,void*)
{
  FXString val=lcd->getText();
  FXint pos=val.length();
  FXint max=word*2;
  FXchar dig='A'+(SELID(sel)-ID_A);

  if(!started)
  {
    started=TRUE;
    exponent=mantissa=FALSE;
    val.format("%c",dig);
    lcd->setText(val);
    return 1;
  }

  if(pos<max)
  {
    //A label with '0' is a special case
    if((pos==1)&&(val[pos-1]=='0'))
    {
      if(dig!='0')
        val[pos-1]=dig;
    }
    else
    {
      if(digitgrouping)
      {
        FXint i,j=0,len=val.length();
        for(i=0;i<len;i++)
        if(val[i]!=' ') val[j++]=val[i];
          val[j]='\0';
        pos=j;
      }

      val.insert(pos,dig);

      if(digitgrouping)
      {
        pos-=1;
        while(pos>0)
        {
          val.insert(pos,' ');
          pos-=2;
        }
      }
    }
  }
  else
    getApp()->beep();

  lcd->setText(val);
  return 1;
}

long CALCWindow::onUpdHexDigit(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(base==BASE_SIXTEEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdOctDigit(FXObject* sender,FXSelector,void*)
{
  FXuint msg=((base==BASE_EIGHT)||(base==BASE_TWO))?ID_DISABLE:ID_ENABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdBinDigit(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(base==BASE_TWO)?ID_DISABLE:ID_ENABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdFixed(FXObject*,FXSelector,void*)
{
  setNotation((notation==NOTATION_NONE)?NOTATION_SCI:NOTATION_NONE);
  return 1;
}

long CALCWindow::onCmdDMS(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  CALCdouble deg=0.0;
  CALCdouble min=0.0;
  CALCdouble sec=0.0;
  FXbool neg=FALSE;

  if(val<((CALCdouble)0.0))
  {
    neg=TRUE;
    val*=-1.0;
  }
  deg=truncate(val);

  if(inv)
  {
    //To degrees
    min=(val-truncate(val))*100.0;
    sec=(min-truncate(min))*100.0;
    min=truncate(min)+sec/60.0;
    deg+=min/60.0;
  }
  else
  {
    //To DMS
    min=60.0*(val-truncate(val));
    sec=60.0*(min-truncate(min));
    deg+=(truncate(min)/100.0)+(sec/10000.0);
  }

  if(neg)
    deg*=-1.0;

  setLabelText(deg);
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdSin(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();

  if(inv&&hyp)
    val=asinh(val);
  else if(hyp)
    val=sinh(val);
  else if(inv)
  {
    if((val>((CALCdouble)1.0))||(val<((CALCdouble)-1.0)))
    {
      lcd->setText("Invalid value for operation.");
      started=FALSE;
      return 1;
    }
    else
    {
      val=asin(val);
      if(rep==DEGREES)
        val*=RAD2DEG;
      else if(rep==GRADIENTS)
        val*=RAD2GRAD;
    }
  }
  else
  {
    if(rep==DEGREES)
      val*=DEG2RAD;
    else if(rep==GRADIENTS)
      val*=GRAD2RAD;
    val=sin(val);
  }

  setLabelText(val);
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdCos(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();

  if(inv&&hyp)
  {
    if(val==((CALCdouble)0.0))
    {
      lcd->setText("Invalid value for operation.");
      started=FALSE;
      return 1;
    }
    val=acosh(val);
  }
  else if(hyp)
    val=cosh(val);
  else if(inv)
  {
    if((val>((CALCdouble)1.0))||(val<((CALCdouble)-1.0)))
    {
      lcd->setText("Invalid value for operation.");
      started=FALSE;
      return 1;
    }
    else
    {
      val=acos(val);
      if(rep==DEGREES)
        val*=RAD2DEG;
      else if(rep==GRADIENTS)
        val*=RAD2GRAD;
    }
  }
  else
  {
    if(rep==DEGREES)
      val*=DEG2RAD;
    else if(rep==GRADIENTS)
      val*=GRAD2RAD;
    val=cos(val);
  }

  setLabelText(val);
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdTan(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();

  try
  {
    if(inv&&hyp)
    {
      if((val>=((CALCdouble)1.0))||(val<=((CALCdouble)-1.0)))
      {
        if(val==((CALCdouble)1.0))
          throw FXErrorException("Undefined - result of divide by zero error.");
        else
          throw FXErrorException("Invalid value for operation.");
      }
      val=atanh(val);
    }
    else if(hyp)
      val=tanh(val);
    else if(inv)
    {
      val=atan(val);
      if(rep==DEGREES)
        val*=RAD2DEG;
      else if(rep==GRADIENTS)
        val*=RAD2GRAD;
    }
    else
    {
      //FIX ME - This is too complicated, plus it doesn't handle radians all that well
      //Maybe I need to put a threshold on the results of trig funcitions
      //so that mantissas < 5e-16 are considered zero.  That would fix the cos problems.
      CALCdouble tester=0.0;
      if(rep==DEGREES)
      {
        tester=(val<((CALCdouble)0.0))?360.0+(-fmod(-val,360)):fmod(val,360);
        if(tester==((CALCdouble)90.0)||tester==((CALCdouble)270.0))
          throw FXErrorException("Invalid value for operation.");
        val*=DEG2RAD;
      }
      else if(rep==GRADIENTS)
      {
        tester=(val<((CALCdouble)0.0))?400.0+(-fmod(-val,400)):fmod(val,400);
        if(tester==((CALCdouble)100.0)||tester==((CALCdouble)300.0))
          throw FXErrorException("Invalid value for operation.");
        val*=GRAD2RAD;
      }
      else //RADIANS
      {
        tester=(val<((CALCdouble)0.0))?CALC_2PI+(val-truncate(truncate(val)/CALC_2PI)*CALC_2PI):(val-truncate(truncate(val)/CALC_2PI)*CALC_2PI);
        //Maybe I should drop the last digit from tester and do a strncmp??
        if(tester==(CALC_PI/2.0)||tester==(CALC_PI+(CALC_PI/2.0)))
          throw FXErrorException("Invalid value for operation.");
      }

      //This helps with some of the radian input that sneaks through the above test.
      //Results of this test will depend on mantissa length and cos implementation.
      //Some return very small values for cos(90)
      if(cos(val)==((CALCdouble)0.0))
        throw FXErrorException("Invalid value for operation.");

      val=tan(val);
    }
    setLabelText(val);
  }
  catch(FXErrorException err)
  {
    lcd->setText(err.what());
  }

  started=FALSE;
  return 1;
}

long CALCWindow::onUpdFixed(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdDMS(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdSin(FXObject* sender,FXSelector,void*)
{
  FXString txt="sin";
  if(inv)
    txt.prepend("a");
  if(hyp)
    txt.append("h");
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);

  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdCos(FXObject* sender,FXSelector,void*)
{
  FXString txt="cos";
  if(inv)
    txt.prepend("a");
  if(hyp)
    txt.append("h");
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);

  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdTan(FXObject* sender,FXSelector,void*)
{
  FXString txt="tan";
  if(inv)
    txt.prepend("a");
  if(hyp)
    txt.append("h");
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);

  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onCmdLParen(FXObject*,FXSelector,void*)
{
  if(started)
  {
    getApp()->beep();
    return 1;
  }

  FXString str=lcd->getText();
#ifdef HAVE_DOUBLEDOUBLE
  stackPair sp;
  sp.op=op; sp.value=value;
#else
  stackPair sp={op,value};
#endif
  stack.append(sp);

  op=0;
  value=0.0;

  if(str[0]=='(')
    str.append('(');
  else
    str="(";

  lcd->setText(str);
  started=FALSE;

  return 1;
}

long CALCWindow::onCmdExponent(FXObject*,FXSelector,void*)
{
  if(!started)
    getApp()->beep();
  else
  {
    FXString val=lcd->getText();
    val.append("e+0");
    exponent=TRUE;
    lcd->setText(val);
  }

  return 1;
}

long CALCWindow::onCmdPowY(FXObject*,FXSelector,void*)
{
  storePair.op=op;
  storePair.value=getLabelText();
  op=ID_POWY;
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdPow3(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  if(inv)
  {
    try
    {
      //powY will automatically turn 3 to 1/3 because inv is true
      setLabelText(powY(val,3.0));
    }
    catch(FXErrorException err)
    {
      lcd->setText(err.what());
      started=FALSE;
    }
  }
  else
    setLabelText(val*val*val);
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdPow2(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  if(inv)
    setLabelText(sqrt(val));
  else
    setLabelText(val*val);
  started=FALSE;
  return 1;
}

long CALCWindow::onUpdExponent(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);
  return 1;
}

long CALCWindow::onUpdPowY(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"x^1/y":"x^y";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onUpdPow3(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"x^1/3":"x^3";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onUpdPow2(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"x^1/2":"x^2";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onCmdRParen(FXObject*,FXSelector,void*)
{
  FXString str=lcd->getText();
  FXint pos=stack.no();

  if(pos-->0)
  {
    if(str[0]=='(')
    {
      FXint len=str.length();
      str.remove(0);
      if(len==1)
        str=(base==BASE_TEN)?"0.":"0";
      lcd->setText(str);
    }
    else
    {
      //Process our op
      if(op!=ID_EQUAL)
        handle(this,MKUINT(ID_EQUAL,SEL_COMMAND),NULL);
    }

    //Remove from stack and reset
    stackPair sp=stack[pos];
    stack.remove(pos);

    op=sp.op;
    value=sp.value;
  }
  started=FALSE;

  return 1;
}

long CALCWindow::onCmdLn(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  if(inv)
    setLabelText(exp(val));
  else
  {
    //val must be >= 0
    if(val<=((CALCdouble)0.0))
      lcd->setText("Invalid value for operation.");
    else
      setLabelText(log(val));
  }
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdLog(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  if(inv)
    setLabelText(pow(((CALCdouble)10.0),val));
  else
  {
    //val must be >= 0
    if(val<=((CALCdouble)0.0))
      lcd->setText("Invalid value for operation.");
    else
    setLabelText(log10(val));
  }
  started=FALSE;
  return 1;
}

//FIX ME - error for big numbers w/exponent
long CALCWindow::onCmdFactorial(FXObject*,FXSelector,void*)
{
  CALCdouble val=1.0;
  CALCdouble n=getLabelText();
  FXint i=1;

  if(n>=((CALCdouble)0))
  {
    while(((CALCdouble)i)<n)
      val*=++i;
    setLabelText(val);
  }
  else
  {
    lcd->setText("Invalid value for operation.");
  }

  started=FALSE;
  return 1;
}

long CALCWindow::onCmdInvert(FXObject*,FXSelector,void*)
{
  CALCdouble val=getLabelText();
  setLabelText(1.0/val);
  started=FALSE;
  return 1;
}

long CALCWindow::onUpdLn(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"e^x":"ln";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onUpdLog(FXObject* sender,FXSelector,void*)
{
  FXString txt=(inv)?"10^x":"log";
  sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&txt);
  return 1;
}

long CALCWindow::onCmdMemoryClear(FXObject*,FXSelector,void*)
{
  memory=0.0;
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdMemoryRecall(FXObject*,FXSelector,void*)
{
  setLabelText(memory);
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdMemoryStore(FXObject*,FXSelector,void*)
{
  memory=getLabelText();
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdMemoryAdd(FXObject*,FXSelector,void*)
{
  memory+=getLabelText();
  started=FALSE;
  return 1;
}

long CALCWindow::onCmdPI(FXObject*,FXSelector,void*)
{
  if(inv)
    setLabelText(CALC_2PI);
  else
    setLabelText(CALC_PI);
  started=FALSE;
  return 1;
}

long CALCWindow::onUpdPI(FXObject* sender,FXSelector,void*)
{
  FXuint msg=(base==BASE_TEN)?ID_ENABLE:ID_DISABLE;
  sender->handle(this,MKUINT(msg,SEL_COMMAND),NULL);

  return 1;
}

long CALCWindow::onPopupMenu(FXObject* sender,FXSelector sel,void* ptr)
{
#ifdef HAVE_RTTI
  //Attemp to get tip text
  try
  {
    FXLabel* helpsource=dynamic_cast<FXLabel*>(sender);
    if(helpsource)
    {
      //Check for tip text
      FXString str=helpsource->getTipText();
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
    }
  }
  catch(...)
  {
    //Probably an access violation - definately no tip text for you
    ;
  }
#endif
  return 1;
}

long CALCWindow::onCmdClarify(FXObject*,FXSelector,void*)
{
  clarify=TRUE;
  return 1;
}
