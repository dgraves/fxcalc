/* CALCWindow.cpp
 * Copyright (C) 2000,2001,2002 Dustin Graves <dgraves@computer.org>
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
#include "fox/fx.h"
#include "fox/fxkeys.h"
#include "fox/FXElement.h"
#include "fox/FXArray.h"

#include "icons.h"
#include "CALCdefs.h"
#include "CALCHelp.h"
#include "CALCButton.h"
#include "CALCLabel.h"
#include "CALCStatBox.h"
#include "CALCPrefsBox.h"
#include "CALCWindow.h"

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
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_PREFERENCES,CALCWindow::onCmdPreferences),
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

  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DISPLAYFONT,CALCWindow::onCmdDisplayFont),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_BUTTONFONT,CALCWindow::onCmdButtonFont),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_CALCWIDTH,CALCWindow::onCmdCalcWidth),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_CALCHEIGHT,CALCWindow::onCmdCalcHeight),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_NUMDIGITS,CALCWindow::onCmdNumDigits),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_COLOR_DISPLAY,CALCWindow::ID_COLOR_CLEARALL,CALCWindow::onCmdBackColor),
  FXMAPFUNCS(SEL_COMMAND,CALCWindow::ID_TEXTCOLOR_DISPLAY,CALCWindow::ID_TEXTCOLOR_CLEARALL,CALCWindow::onCmdTextColor),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_NUMDIGITS,CALCWindow::onUpdNumDigits),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_CALCWIDTH,CALCWindow::onUpdCalcWidth),
  FXMAPFUNC(SEL_UPDATE,CALCWindow::ID_CALCHEIGHT,CALCWindow::onUpdCalcHeight),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_COLOR_DISPLAY,CALCWindow::ID_COLOR_CLEARALL,CALCWindow::onUpdBackColor),
  FXMAPFUNCS(SEL_UPDATE,CALCWindow::ID_TEXTCOLOR_DISPLAY,CALCWindow::ID_TEXTCOLOR_CLEARALL,CALCWindow::onUpdTextColor),

  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTDISPLAYFONT,CALCWindow::onCmdDefaultDisplayFont),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTBUTTONFONT,CALCWindow::onCmdDefaultButtonFont),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTMODE,CALCWindow::onCmdDefaultMode),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTWIDTH,CALCWindow::onCmdDefaultWidth),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTHEIGHT,CALCWindow::onCmdDefaultHeight),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTNUMDIGITS,CALCWindow::onCmdDefaultNumDigits),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTCOLORS,CALCWindow::onCmdDefaultColors),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_DEFAULTTEXTCOLORS,CALCWindow::onCmdDefaultTextColors),

  //The popup dialog
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,CALCWindow::ID_LCD,CALCWindow::onPopupMenu),
  FXMAPFUNCS(SEL_RIGHTBUTTONRELEASE,CALCWindow::ID_BINARY,CALCWindow::ID_PI,CALCWindow::onPopupMenu),
  FXMAPFUNC(SEL_COMMAND,CALCWindow::ID_CLARIFY,CALCWindow::onCmdClarify)
};

FXIMPLEMENT(CALCWindow,FXMainWindow,CALCWindowMap,ARRAYNUMBER(CALCWindowMap))

CALCWindow::CALCWindow(FXApp* app)
: FXMainWindow(app,"fxcalc",NULL,NULL,DECOR_ALL), //DECOR_TITLE|DECOR_MINIMIZE|DECOR_CLOSE|DECOR_BORDER|DECOR_MENU),
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
  clarify(FALSE),
  lcdfont(NULL),
  btnfont(NULL)
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
  new FXMenuCommand(editmenu,"Pr&eferences...\tCtrl+E",NULL,this,ID_PREFERENCES);
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
  lcd=new CALCLabel(lcdframe,"0.\t"LCD_HELP,NULL,JUSTIFY_RIGHT|FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X);
  lcd->setTarget(this);
  lcd->setSelector(ID_LCD);
  lcd->setBackColor(getApp()->getBackColor());
  lcd->enable(); //So it gets the mouse clicks

  modesFrame=new FXHorizontalFrame(this,LAYOUT_FILL_X,0,0,0,0, 4,4,2,2, 2,2);
  FXHorizontalFrame* baseFrame=new FXHorizontalFrame(modesFrame,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  baseBtns[BASE_HEX]=new FXRadioButton(baseFrame,"Hex\t"HEXADECIMAL_HELP,this,ID_HEXADECIMAL,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  baseBtns[BASE_DEC]=new FXRadioButton(baseFrame,"Dec\t"DECIMAL_HELP,this,ID_DECIMAL,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  baseBtns[BASE_OCT]=new FXRadioButton(baseFrame,"Oct\t"OCTAL_HELP,this,ID_OCTAL,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  baseBtns[BASE_BIN]=new FXRadioButton(baseFrame,"Bin\t"BINARY_HELP,this,ID_BINARY,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  modeSwitch=new FXSwitcher(modesFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);
  FXHorizontalFrame* repFrame=new FXHorizontalFrame(modeSwitch,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  repBtns[REP_DEGREES]=new FXRadioButton(repFrame,"Degrees\t"DEGREES_HELP,this,ID_DEGREES,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  repBtns[REP_RADIANS]=new FXRadioButton(repFrame,"Radians\t"RADIANS_HELP,this,ID_RADIANS,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  repBtns[REP_GRADIENTS]=new FXRadioButton(repFrame,"Gradients\t"GRADIENTS_HELP,this,ID_GRADIENTS,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXHorizontalFrame* wordFrame=new FXHorizontalFrame(modeSwitch,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  repBtns[REP_DLWORD]=new FXRadioButton(wordFrame,"dlword\t"DLWORD_HELP,this,ID_DLWORD,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  repBtns[REP_LWORD]=new FXRadioButton(wordFrame,"lword\t"LWORD_HELP,this,ID_LWORD,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  repBtns[REP_WORD]=new FXRadioButton(wordFrame,"word\t"WORD_HELP,this,ID_WORD,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  repBtns[REP_BYTE]=new FXRadioButton(wordFrame,"byte\t"BYTE_HELP,this,ID_BYTE,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  FXHorizontalFrame* numpad=new FXHorizontalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,2,2, FRAMEGAP,0);

  //Scientific buttons
  scientificFrame=new FXHorizontalFrame(numpad,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

  //Left scientific button segment
  FXVerticalFrame* leftSci=new FXVerticalFrame(scientificFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXHorizontalFrame* optFrame=new FXHorizontalFrame(leftSci,PACK_UNIFORM_WIDTH|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  modeBtns[MODE_INV]=new FXCheckButton(optFrame,"Inv\t"INV_HELP,this,ID_INV,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  modeBtns[MODE_HYP]=new FXCheckButton(optFrame,"Hyp\t"HYP_HELP,this,ID_HYP,ICON_BEFORE_TEXT|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  FXHorizontalFrame* sciLeftFrame=new FXHorizontalFrame(leftSci,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, FRAMEGAP);
  FXVerticalFrame* statFrame=new FXVerticalFrame(sciLeftFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  statBtns[STAT_SHOW]=new CALCButton(statFrame,"Sta\t"SHOWSTATBOX_HELP,NULL,this,ID_SHOWSTATBOX,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  statBtns[STAT_MEAN]=new CALCButton(statFrame,"Ave\t"MEAN_HELP,NULL,this,ID_MEAN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  statBtns[STAT_SUM]=new CALCButton(statFrame,"Sum\t"SUM_HELP,NULL,this,ID_SUM,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  statBtns[STAT_STANDARDDEV]=new CALCButton(statFrame,"s\t"STANDARDDEV_HELP,NULL,this,ID_STANDARDDEV,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  statBtns[STAT_ADD]=new CALCButton(statFrame,"Dat\t"ADDSTAT_HELP,NULL,this,ID_ADDSTAT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  FXMatrix* sciMatrix=new FXMatrix(sciLeftFrame,5,MATRIX_BY_ROWS|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  funcBtns[FUNC_FIXED]=new CALCButton(sciMatrix,"F-E\t"FIXED_HELP,NULL,this,ID_FIXED,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_DMS]=new CALCButton(sciMatrix,"dms\t"DMS_HELP,NULL,this,ID_DMS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_SIN]=new CALCButton(sciMatrix,"sin\t"SIN_HELP,NULL,this,ID_SIN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_COS]=new CALCButton(sciMatrix,"cos\t"COS_HELP,NULL,this,ID_COS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_TAN]=new CALCButton(sciMatrix,"tan\t"TAN_HELP,NULL,this,ID_TAN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_LPAREN]=new CALCButton(sciMatrix,"(\t"LPAREN_HELP,NULL,this,ID_LPAREN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_EXPONENT]=new CALCButton(sciMatrix,"exp\t"EXPONENT_HELP,NULL,this,ID_EXPONENT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_POWY]=new CALCButton(sciMatrix,"x^y\t"POWY_HELP,NULL,this,ID_POWY,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_POW3]=new CALCButton(sciMatrix,"x^3\t"POW3_HELP,NULL,this,ID_POW3,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_POW2]=new CALCButton(sciMatrix,"x^2\t"POW2_HELP,NULL,this,ID_POW2,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  //Right scientific button segment
  FXVerticalFrame* rightSci=new FXVerticalFrame(scientificFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);

  FXLabel* parenLabel=new FXLabel(rightSci," \t"PARENLABEL_HELP,NULL,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  parenLabel->setTarget(this);
  parenLabel->setSelector(ID_PARENLABEL);
  parenLabel->enable(); //So it gets the mouse clicks

  FXVerticalFrame* funFrame=new FXVerticalFrame(rightSci,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  funcBtns[FUNC_RPAREN]=new CALCButton(funFrame,")\t"RPAREN_HELP,NULL,this,ID_RPAREN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_LN]=new CALCButton(funFrame,"ln\t"LN_HELP,NULL,this,ID_LN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_LOG]=new CALCButton(funFrame,"log\t"LOG_HELP,NULL,this,ID_LOG,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_FACTORIAL]=new CALCButton(funFrame,"n!\t"FACTORIAL_HELP,NULL,this,ID_FACTORIAL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_INVERT]=new CALCButton(funFrame,"1/x\t"INVERT_HELP,NULL,this,ID_INVERT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  //The right hand side button group.  Memory, digits, and operators
  FXHorizontalFrame* standardFrame=new FXHorizontalFrame(numpad,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, FRAMEGAP,0);

  //Group with memory and PI
  FXVerticalFrame* leftStan=new FXVerticalFrame(standardFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXLabel* memLabel=new FXLabel(leftStan," \t"MEMORYLABEL_HELP,NULL,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  memLabel->setTarget(this);
  memLabel->setSelector(ID_MEMORYLABEL);
  memLabel->enable(); //So it gets the mouse clicks

  FXVerticalFrame* memFrame=new FXVerticalFrame(leftStan,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  memoryBtns[MEMORY_CLEAR]=new CALCButton(memFrame,"MC\t"MEMORYCLEAR_HELP,NULL,this,ID_MEMORYCLEAR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  memoryBtns[MEMORY_RECALL]=new CALCButton(memFrame,"MR\t"MEMORYRECALL_HELP,NULL,this,ID_MEMORYRECALL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  memoryBtns[MEMORY_STORE]=new CALCButton(memFrame,"MS\t"MEMORYSTORE_HELP,NULL,this,ID_MEMORYSTORE,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  memoryBtns[MEMORY_ADD]=new CALCButton(memFrame,"M+\t"MEMORYADD_HELP,NULL,this,ID_MEMORYADD,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  funcBtns[FUNC_PI]=new CALCButton(memFrame,"PI\t"PI_HELP,NULL,this,ID_PI,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  //Group with digits and operators
  FXVerticalFrame* rightStan=new FXVerticalFrame(standardFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,8);
  FXHorizontalFrame* controlFrame=new FXHorizontalFrame(rightStan,PACK_UNIFORM_WIDTH|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  clearBtns[CLEAR_BACKSPACE]=new CALCButton(controlFrame,"Erase\t"BACKSPACE_HELP,NULL,this,ID_BACKSPACE,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  clearBtns[CLEAR_ENTRY]=new CALCButton(controlFrame,"CE\t"CLEARENTRY_HELP,NULL,this,ID_CLEARENTRY,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  clearBtns[CLEAR_ALL]=new CALCButton(controlFrame,"C\t"CLEARALL_HELP,NULL,this,ID_CLEARALL,FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  FXVerticalFrame* digFrame=new FXVerticalFrame(rightStan,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXHorizontalFrame* rightMain=new FXHorizontalFrame(digFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  FXMatrix* digitMatrix=new FXMatrix(rightMain,4,MATRIX_BY_ROWS|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  digitBtns[DIGIT_7]=new CALCButton(digitMatrix,"7\t"SEVEN_HELP,NULL,this,ID_7,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_4]=new CALCButton(digitMatrix,"4\t"FOUR_HELP,NULL,this,ID_4,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_1]=new CALCButton(digitMatrix,"1\t"ONE_HELP,NULL,this,ID_1,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_0]=new CALCButton(digitMatrix,"0\t"ZERO_HELP,NULL,this,ID_0,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  digitBtns[DIGIT_8]=new CALCButton(digitMatrix,"8\t"EIGHT_HELP,NULL,this,ID_8,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_5]=new CALCButton(digitMatrix,"5\t"FIVE_HELP,NULL,this,ID_5,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_2]=new CALCButton(digitMatrix,"2\t"TWO_HELP,NULL,this,ID_2,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_UMINUS]=new CALCButton(digitMatrix,"+/-\t"UMINUS_HELP,NULL,this,ID_UMINUS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  digitBtns[DIGIT_9]=new CALCButton(digitMatrix,"9\t"NINE_HELP,NULL,this,ID_9,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_6]=new CALCButton(digitMatrix,"6\t"SIX_HELP,NULL,this,ID_6,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  digitBtns[DIGIT_3]=new CALCButton(digitMatrix,"3\t"THREE_HELP,NULL,this,ID_3,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_DECPOINT]=new CALCButton(digitMatrix,".\t"DECPOINT_HELP,NULL,this,ID_DECPOINT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  operatorBtns[OPERATOR_DIVIDE]=new CALCButton(digitMatrix,"/\t"DIVIDE_HELP,NULL,this,ID_DIVIDE,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_MULTIPLY]=new CALCButton(digitMatrix,"*\t"MULTIPLY_HELP,NULL,this,ID_MULTIPLY,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_MINUS]=new CALCButton(digitMatrix,"-\t"SUBTRACT_HELP,NULL,this,ID_SUBTRACT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_PLUS]=new CALCButton(digitMatrix,"+\t"ADD_HELP,NULL,this,ID_ADD,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  advancedMatrix=new FXMatrix(rightMain,4,MATRIX_BY_ROWS|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  operatorBtns[OPERATOR_MODULUS]=new CALCButton(advancedMatrix,"mod\t"MODULUS_HELP,NULL,this,ID_MODULUS,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_OR]=new CALCButton(advancedMatrix,"or\t"OR_HELP,NULL,this,ID_OR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_LEFTSHIFT]=new CALCButton(advancedMatrix,"lsh\t"LEFTSHIFT_HELP,NULL,this,ID_LEFTSHIFT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_EQUAL]=new CALCButton(advancedMatrix,"=\t"EQUAL_HELP,NULL,this,ID_EQUAL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  operatorBtns[OPERATOR_AND]=new CALCButton(advancedMatrix,"and\t"AND_HELP,NULL,this,ID_AND,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_XOR]=new CALCButton(advancedMatrix,"xor\t"XOR_HELP,NULL,this,ID_XOR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_NOT]=new CALCButton(advancedMatrix,"not\t"NOT_HELP,NULL,this,ID_NOT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_TRUNC]=new CALCButton(advancedMatrix,"int\t"TRUNCATE_HELP,NULL,this,ID_TRUNCATE,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  simpleFrame=new FXVerticalFrame(rightMain,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  operatorBtns[OPERATOR_SIMPLE_SQRT]=new CALCButton(simpleFrame,"sqrt\t"SQRT_HELP,NULL,this,ID_SQRT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_SIMPLE_PERCENT]=new CALCButton(simpleFrame,"%\t"PERCENT_HELP,NULL,this,ID_PERCENT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_SIMPLE_INVERT]=new CALCButton(simpleFrame,"1/x\t"INVERT_HELP,NULL,this,ID_INVERT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  operatorBtns[OPERATOR_SIMPLE_EQUAL]=new CALCButton(simpleFrame,"=\t"EQUAL_HELP,NULL,this,ID_EQUAL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  hexFrame=new FXHorizontalFrame(digFrame,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  hexdigitBtns[HEXDIGIT_A]=new CALCButton(hexFrame,"A\t"A_HELP,NULL,this,ID_A,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  hexdigitBtns[HEXDIGIT_B]=new CALCButton(hexFrame,"B\t"B_HELP,NULL,this,ID_B,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  hexdigitBtns[HEXDIGIT_C]=new CALCButton(hexFrame,"C\t"C_HELP,NULL,this,ID_C,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  hexdigitBtns[HEXDIGIT_D]=new CALCButton(hexFrame,"D\t"D_HELP,NULL,this,ID_D,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  hexdigitBtns[HEXDIGIT_E]=new CALCButton(hexFrame,"E\t"E_HELP,NULL,this,ID_E,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);
  hexdigitBtns[HEXDIGIT_F]=new CALCButton(hexFrame,"F\t"F_HELP,NULL,this,ID_F,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,BUTTONWIDTH,BUTTONHEIGHT);

  //Hot keys
  baseBtns[BASE_HEX]->addHotKey(KEY_F5);
  baseBtns[BASE_DEC]->addHotKey(KEY_F6);
  baseBtns[BASE_OCT]->addHotKey(KEY_F7);
  baseBtns[BASE_BIN]->addHotKey(KEY_F8);
  repBtns[REP_DEGREES]->addHotKey(KEY_F2);
  repBtns[REP_RADIANS]->addHotKey(KEY_F3);
  repBtns[REP_GRADIENTS]->addHotKey(KEY_F4);
  repBtns[REP_DLWORD]->addHotKey(KEY_F12);
  repBtns[REP_LWORD]->addHotKey(KEY_F2);
  repBtns[REP_WORD]->addHotKey(KEY_F3);
  repBtns[REP_BYTE]->addHotKey(KEY_F4);
  modeBtns[MODE_INV]->addHotKey(KEY_i);
  modeBtns[MODE_INV]->addHotKey(KEY_I);
  modeBtns[MODE_HYP]->addHotKey(KEY_h);
  modeBtns[MODE_HYP]->addHotKey(KEY_H);
  statBtns[STAT_SHOW]->addHotKey(MKUINT(KEY_s,CONTROLMASK));
  statBtns[STAT_SHOW]->addHotKey(MKUINT(KEY_S,CONTROLMASK));
  statBtns[STAT_MEAN]->addHotKey(MKUINT(KEY_a,CONTROLMASK));
  statBtns[STAT_MEAN]->addHotKey(MKUINT(KEY_A,CONTROLMASK));
  statBtns[STAT_SUM]->addHotKey(MKUINT(KEY_t,CONTROLMASK));
  statBtns[STAT_SUM]->addHotKey(MKUINT(KEY_T,CONTROLMASK));
  statBtns[STAT_STANDARDDEV]->addHotKey(MKUINT(KEY_d,CONTROLMASK));
  statBtns[STAT_STANDARDDEV]->addHotKey(MKUINT(KEY_D,CONTROLMASK));
  statBtns[STAT_ADD]->addHotKey(KEY_Insert);
  funcBtns[FUNC_FIXED]->addHotKey(KEY_v);
  funcBtns[FUNC_FIXED]->addHotKey(KEY_V);
  funcBtns[FUNC_DMS]->addHotKey(KEY_m);
  funcBtns[FUNC_DMS]->addHotKey(KEY_M);
  funcBtns[FUNC_SIN]->addHotKey(KEY_s);
  funcBtns[FUNC_SIN]->addHotKey(KEY_S);
  funcBtns[FUNC_COS]->addHotKey(KEY_o);
  funcBtns[FUNC_COS]->addHotKey(KEY_O);
  funcBtns[FUNC_TAN]->addHotKey(KEY_t);
  funcBtns[FUNC_TAN]->addHotKey(KEY_T);
  funcBtns[FUNC_LPAREN]->addHotKey(MKUINT(KEY_parenleft,SHIFTMASK));
  funcBtns[FUNC_EXPONENT]->addHotKey(KEY_x);
  funcBtns[FUNC_EXPONENT]->addHotKey(KEY_X);
  funcBtns[FUNC_POWY]->addHotKey(KEY_y);
  funcBtns[FUNC_POWY]->addHotKey(KEY_Y);
  funcBtns[FUNC_POW3]->addHotKey(MKUINT(KEY_numbersign,SHIFTMASK));
  funcBtns[FUNC_POW2]->addHotKey(MKUINT(KEY_at,SHIFTMASK));
  funcBtns[FUNC_RPAREN]->addHotKey(MKUINT(KEY_parenright,SHIFTMASK));
  funcBtns[FUNC_LN]->addHotKey(KEY_n);
  funcBtns[FUNC_LN]->addHotKey(KEY_N);
  funcBtns[FUNC_LOG]->addHotKey(KEY_l);
  funcBtns[FUNC_LOG]->addHotKey(KEY_L);
  funcBtns[FUNC_FACTORIAL]->addHotKey(MKUINT(KEY_exclam,SHIFTMASK));
  funcBtns[FUNC_INVERT]->addHotKey(KEY_r);
  funcBtns[FUNC_INVERT]->addHotKey(KEY_R);
  memoryBtns[MEMORY_CLEAR]->addHotKey(MKUINT(KEY_l,CONTROLMASK));
  memoryBtns[MEMORY_CLEAR]->addHotKey(MKUINT(KEY_L,CONTROLMASK));
  memoryBtns[MEMORY_RECALL]->addHotKey(MKUINT(KEY_r,CONTROLMASK));
  memoryBtns[MEMORY_RECALL]->addHotKey(MKUINT(KEY_R,CONTROLMASK));
  memoryBtns[MEMORY_STORE]->addHotKey(MKUINT(KEY_m,CONTROLMASK));
  memoryBtns[MEMORY_STORE]->addHotKey(MKUINT(KEY_M,CONTROLMASK));
  memoryBtns[MEMORY_ADD]->addHotKey(MKUINT(KEY_p,CONTROLMASK));
  memoryBtns[MEMORY_ADD]->addHotKey(MKUINT(KEY_P,CONTROLMASK));
  funcBtns[FUNC_PI]->addHotKey(KEY_p);
  funcBtns[FUNC_PI]->addHotKey(KEY_P);
  clearBtns[CLEAR_BACKSPACE]->addHotKey(KEY_BackSpace);
  clearBtns[CLEAR_ENTRY]->addHotKey(KEY_Delete);
  clearBtns[CLEAR_ENTRY]->addHotKey(KEY_KP_Delete);
  clearBtns[CLEAR_ALL]->addHotKey(KEY_Escape);
  digitBtns[DIGIT_7]->addHotKey(KEY_7);
  digitBtns[DIGIT_7]->addHotKey(KEY_KP_7);
  digitBtns[DIGIT_4]->addHotKey(KEY_4);
  digitBtns[DIGIT_4]->addHotKey(KEY_KP_4);
  digitBtns[DIGIT_1]->addHotKey(KEY_1);
  digitBtns[DIGIT_1]->addHotKey(KEY_KP_1);
  digitBtns[DIGIT_0]->addHotKey(KEY_0);
  digitBtns[DIGIT_0]->addHotKey(KEY_KP_0);
  digitBtns[DIGIT_8]->addHotKey(KEY_8);
  digitBtns[DIGIT_8]->addHotKey(KEY_KP_8);
  digitBtns[DIGIT_5]->addHotKey(KEY_5);
  digitBtns[DIGIT_5]->addHotKey(KEY_KP_5);
  digitBtns[DIGIT_2]->addHotKey(KEY_2);
  digitBtns[DIGIT_2]->addHotKey(KEY_KP_2);
  operatorBtns[OPERATOR_UMINUS]->addHotKey(KEY_F9);
  digitBtns[DIGIT_9]->addHotKey(KEY_9);
  digitBtns[DIGIT_9]->addHotKey(KEY_KP_9);
  digitBtns[DIGIT_6]->addHotKey(KEY_6);
  digitBtns[DIGIT_6]->addHotKey(KEY_KP_6);
  digitBtns[DIGIT_3]->addHotKey(KEY_3);
  digitBtns[DIGIT_3]->addHotKey(KEY_KP_3);
  operatorBtns[OPERATOR_DECPOINT]->addHotKey(KEY_period);
  operatorBtns[OPERATOR_DECPOINT]->addHotKey(KEY_KP_Decimal);
  operatorBtns[OPERATOR_DECPOINT]->addHotKey(KEY_comma);
  operatorBtns[OPERATOR_DIVIDE]->addHotKey(KEY_slash);
  operatorBtns[OPERATOR_DIVIDE]->addHotKey(KEY_KP_Divide);
  operatorBtns[OPERATOR_MULTIPLY]->addHotKey(MKUINT(KEY_asterisk,SHIFTMASK));
  operatorBtns[OPERATOR_MULTIPLY]->addHotKey(KEY_KP_Multiply);
  operatorBtns[OPERATOR_MINUS]->addHotKey(KEY_minus);
  operatorBtns[OPERATOR_MINUS]->addHotKey(KEY_KP_Subtract);
  operatorBtns[OPERATOR_PLUS]->addHotKey(MKUINT(KEY_plus,SHIFTMASK));
  operatorBtns[OPERATOR_PLUS]->addHotKey(KEY_KP_Add);
  operatorBtns[OPERATOR_MODULUS]->addHotKey(MKUINT(KEY_percent,SHIFTMASK));
  operatorBtns[OPERATOR_OR]->addHotKey(MKUINT(KEY_bar,SHIFTMASK));
  operatorBtns[OPERATOR_LEFTSHIFT]->addHotKey(MKUINT(KEY_less,SHIFTMASK));
  operatorBtns[OPERATOR_EQUAL]->addHotKey(KEY_equal);
  operatorBtns[OPERATOR_EQUAL]->addHotKey(KEY_Return);
  operatorBtns[OPERATOR_EQUAL]->addHotKey(KEY_KP_Enter);
  operatorBtns[OPERATOR_AND]->addHotKey(MKUINT(KEY_ampersand,SHIFTMASK));
  operatorBtns[OPERATOR_XOR]->addHotKey(MKUINT(KEY_asciicircum,SHIFTMASK));
  operatorBtns[OPERATOR_NOT]->addHotKey(MKUINT(KEY_asciitilde,SHIFTMASK));
  operatorBtns[OPERATOR_TRUNC]->addHotKey(KEY_semicolon);
  operatorBtns[OPERATOR_SIMPLE_SQRT]->addHotKey(MKUINT(KEY_at,SHIFTMASK));
  operatorBtns[OPERATOR_SIMPLE_PERCENT]->addHotKey(MKUINT(KEY_percent,SHIFTMASK));
  operatorBtns[OPERATOR_SIMPLE_INVERT]->addHotKey(KEY_r);
  operatorBtns[OPERATOR_SIMPLE_INVERT]->addHotKey(KEY_R);
  operatorBtns[OPERATOR_SIMPLE_EQUAL]->addHotKey(KEY_equal);
  operatorBtns[OPERATOR_SIMPLE_EQUAL]->addHotKey(KEY_Return);
  operatorBtns[OPERATOR_SIMPLE_EQUAL]->addHotKey(KEY_KP_Enter);
  hexdigitBtns[HEXDIGIT_A]->addHotKey(KEY_a);
  hexdigitBtns[HEXDIGIT_A]->addHotKey(KEY_A);
  hexdigitBtns[HEXDIGIT_B]->addHotKey(KEY_b);
  hexdigitBtns[HEXDIGIT_B]->addHotKey(KEY_B);
  hexdigitBtns[HEXDIGIT_C]->addHotKey(KEY_c);
  hexdigitBtns[HEXDIGIT_C]->addHotKey(KEY_C);
  hexdigitBtns[HEXDIGIT_D]->addHotKey(KEY_d);
  hexdigitBtns[HEXDIGIT_D]->addHotKey(KEY_D);
  hexdigitBtns[HEXDIGIT_E]->addHotKey(KEY_e);
  hexdigitBtns[HEXDIGIT_E]->addHotKey(KEY_E);
  hexdigitBtns[HEXDIGIT_F]->addHotKey(KEY_f);
  hexdigitBtns[HEXDIGIT_F]->addHotKey(KEY_F);

  baseBtns[BASE_LAST]=NULL;
  repBtns[REP_LAST]=NULL;
  modeBtns[MODE_LAST]=NULL;
  digitBtns[DIGIT_LAST]=NULL;
  hexdigitBtns[HEXDIGIT_LAST]=NULL;
  operatorBtns[OPERATOR_LAST]=NULL;
  funcBtns[FUNC_LAST]=NULL;
  memoryBtns[MEMORY_LAST]=NULL;
  statBtns[STAT_LAST]=NULL;
  clearBtns[CLEAR_LAST]=NULL;
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
  if(lcdfont) delete lcdfont;
  if(btnfont) delete btnfont;
}

void CALCWindow::create()
{
  FXFontDesc fontdesc;
  const FXchar* fontspec;

  //Dimensions
  FXint x=getApp()->reg().readIntEntry("SETTINGS","x",10);
  FXint y=getApp()->reg().readIntEntry("SETTINGS","y",10);
  FXint w1=getApp()->reg().readIntEntry("SETTINGS","standardwidth",0);
  FXint h1=getApp()->reg().readIntEntry("SETTINGS","standardheight",0);
  FXint w2=getApp()->reg().readIntEntry("SETTINGS","scientificwidth",0);
  FXint h2=getApp()->reg().readIntEntry("SETTINGS","scientificheight",0);

  //Fonts and colors
  fontspec=getApp()->reg().readStringEntry("SETTINGS","displayfont",NULL);
  if(fontspec&&fxparsefontdesc(fontdesc,fontspec))
  {
    lcdfont=new FXFont(getApp(),fontdesc);
    setDisplayFont(lcdfont);
  }

  fontspec=getApp()->reg().readStringEntry("SETTINGS","buttonfont",NULL);
  if(fontspec&&fxparsefontdesc(fontdesc,fontspec))
  {
    btnfont=new FXFont(getApp(),fontdesc);
    setButtonFont(btnfont);
  }

  FXColor displayclr=getApp()->reg().readColorEntry("SETTINGS","displaycolor",getApp()->getBackColor());
  FXColor digitclr=getApp()->reg().readColorEntry("SETTINGS","digitcolor",getApp()->getBaseColor());
  FXColor hexdigitclr=getApp()->reg().readColorEntry("SETTINGS","hexdigitcolor",getApp()->getBaseColor());
  FXColor operatorclr=getApp()->reg().readColorEntry("SETTINGS","operatorcolor",getApp()->getBaseColor());
  FXColor functionclr=getApp()->reg().readColorEntry("SETTINGS","functioncolor",getApp()->getBaseColor());
  FXColor memoryclr=getApp()->reg().readColorEntry("SETTINGS","memorycolor",getApp()->getBaseColor());
  FXColor statclr=getApp()->reg().readColorEntry("SETTINGS","statcolor",getApp()->getBaseColor());
  FXColor baseclr=getApp()->reg().readColorEntry("SETTINGS","numbasecolor",getApp()->getBackColor());
  FXColor repclr=getApp()->reg().readColorEntry("SETTINGS","repcolor",getApp()->getBackColor());
  FXColor invclr=getApp()->reg().readColorEntry("SETTINGS","invcolor",getApp()->getBackColor());
  FXColor hypclr=getApp()->reg().readColorEntry("SETTINGS","hypcolor",getApp()->getBackColor());
  FXColor backspaceclr=getApp()->reg().readColorEntry("SETTINGS","backspacecolor",getApp()->getBaseColor());
  FXColor clearentryclr=getApp()->reg().readColorEntry("SETTINGS","clearentrycolor",getApp()->getBaseColor());
  FXColor clearallclr=getApp()->reg().readColorEntry("SETTINGS","clearallcolor",getApp()->getBaseColor());

  FXColor displaytxtclr=getApp()->reg().readColorEntry("SETTINGS","displaytextcolor",FXRGB(0,0,0));
  FXColor digittxtclr=getApp()->reg().readColorEntry("SETTINGS","digittextcolor",VALUE);
  FXColor hexdigittxtclr=getApp()->reg().readColorEntry("SETTINGS","hexdigittextcolor",FUNCTION);
  FXColor operatortxtclr=getApp()->reg().readColorEntry("SETTINGS","operatortextcolor",OPERATOR);
  FXColor functiontxtclr=getApp()->reg().readColorEntry("SETTINGS","functiontextcolor",FUNCTION);
  FXColor memorytxtclr=getApp()->reg().readColorEntry("SETTINGS","memorytextcolor",OPERATOR);
  FXColor stattxtclr=getApp()->reg().readColorEntry("SETTINGS","stattextcolor",FUNCTION);
  FXColor basetxtclr=getApp()->reg().readColorEntry("SETTINGS","numbasetextcolor",FXRGB(0,0,0));
  FXColor reptxtclr=getApp()->reg().readColorEntry("SETTINGS","reptextcolor",FXRGB(0,0,0));
  FXColor invtxtclr=getApp()->reg().readColorEntry("SETTINGS","invtextcolor",FXRGB(0,0,0));
  FXColor hyptxtclr=getApp()->reg().readColorEntry("SETTINGS","hyptextcolor",FXRGB(0,0,0));
  FXColor backspacetxtclr=getApp()->reg().readColorEntry("SETTINGS","backspacetextcolor",OPERATOR);
  FXColor clearentrytxtclr=getApp()->reg().readColorEntry("SETTINGS","clearentrytextcolor",OPERATOR);
  FXColor clearalltxtclr=getApp()->reg().readColorEntry("SETTINGS","clearalltextcolor",OPERATOR);

  //Settings
  FXuint m=getApp()->reg().readUnsignedEntry("SETTINGS","mode",CALC_SCIENTIFIC);
  ndigits=getApp()->reg().readUnsignedEntry("SETTINGS","digits",ndigits);  //Use default initialized value if it's not present
  digitgrouping=getApp()->reg().readIntEntry("SETTINGS","grouping",FALSE);
  if(getApp()->reg().readIntEntry("SETTINGS","tooltips",FALSE))
    handle(this,MKUINT(ID_USETOOLTIPS,SEL_COMMAND),NULL);

  //Make sure it's within a valid range
  //Digit box gives a leeway of 2, so this value is considered valid
  //but if it's any larger than that we make it equal to the float.h size
  if(ndigits>CALCDBL_DIG+2)
    ndigits=CALCDBL_DIG;

  setMode(m);

  width[CALC_STANDARD-1]=w1;
  height[CALC_STANDARD-1]=h1;
  width[CALC_SCIENTIFIC-1]=w2;
  height[CALC_SCIENTIFIC-1]=h2;
  position(x,y,width[m-1],height[m-1]);

  setDisplayColor(displayclr);
  setDigitColor(digitclr);
  setHexDigitColor(hexdigitclr);
  setOperatorColor(operatorclr);
  setFunctionColor(functionclr);
  setMemoryColor(memoryclr);
  setStatColor(statclr);
  setBaseColor(baseclr);
  setRepColor(repclr);
  setInvColor(invclr);
  setHypColor(hypclr);
  setBackspaceColor(backspaceclr);
  setClearEntryColor(clearentryclr);
  setClearAllColor(clearallclr);

  setDisplayTextColor(displaytxtclr);
  setDigitTextColor(digittxtclr);
  setHexDigitTextColor(hexdigittxtclr);
  setOperatorTextColor(operatortxtclr);
  setFunctionTextColor(functiontxtclr);
  setMemoryTextColor(memorytxtclr);
  setStatTextColor(stattxtclr);
  setBaseTextColor(basetxtclr);
  setRepTextColor(reptxtclr);
  setInvTextColor(invtxtclr);
  setHypTextColor(hyptxtclr);
  setBackspaceTextColor(backspacetxtclr);
  setClearEntryTextColor(clearentrytxtclr);
  setClearAllTextColor(clearalltxtclr);

  FXMainWindow::create();
}

void CALCWindow::setFont(FXLabel* targets[],FXFont* font)
{
  FXint i=0;
  while(targets[i]!=NULL)
    targets[i++]->setFont(font);
}

void CALCWindow::setBackColor(FXLabel* target,FXColor color)
{
  target->setBackColor(color);
  target->setHiliteColor(makeHiliteColor(color));
  target->setShadowColor(makeShadowColor(color));
}

void CALCWindow::setBackColor(FXLabel* targets[],FXColor color)
{
  FXint i=0;
  FXColor h=makeHiliteColor(color);
  FXColor s=makeShadowColor(color);
  while(targets[i]!=NULL)
  {
    targets[i]->setBackColor(color);
    targets[i]->setHiliteColor(h);
    targets[i++]->setShadowColor(s);
  }
}

void CALCWindow::setTextColor(FXLabel* targets[],FXColor color)
{
  FXint i=0;
  while(targets[i]!=NULL)
    targets[i++]->setTextColor(color);
}

void CALCWindow::setBaseColor(FXColor color)
{
  FXint i=0;
  while(baseBtns[i]!=NULL)
    ((FXRadioButton*)baseBtns[i++])->setRadioColor(color);
}

void CALCWindow::setDisplayColor(FXColor color)
{
  lcd->setBackColor(color);
}

void CALCWindow::setRepColor(FXColor color)
{
  FXint i=0;
  while(repBtns[i]!=NULL)
    ((FXRadioButton*)repBtns[i++])->setRadioColor(color);
}

void CALCWindow::setDisplayTextColor(FXColor color)
{
  lcd->setTextColor(color);
}

FXColor CALCWindow::getDisplayColor() const
{
  return lcd->getBackColor();
}

FXColor CALCWindow::getDisplayTextColor() const
{
  return lcd->getTextColor();
}

void CALCWindow::setDisplayFont(FXFont* font)
{
  lcd->setFont(font);
}

FXFont* CALCWindow::getDisplayFont() const
{
  return lcd->getFont();
}

void CALCWindow::setButtonFont(FXFont* font)
{
  setFont(baseBtns,font);
  setFont(repBtns,font);
  setFont(modeBtns,font);
  setFont(digitBtns,font);
  setFont(hexdigitBtns,font);
  setFont(operatorBtns,font);
  setFont(funcBtns,font);
  setFont(memoryBtns,font);
  setFont(statBtns,font);
  setFont(clearBtns,font);
}

void CALCWindow::setMode(FXuint m)
{
  //Store current width and height
  width[mode-1]=getWidth();
  height[mode-1]=getHeight();

  if(m==CALC_STANDARD)
  {
    //Hide scientific menus
    extrasSep->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    hexCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    decCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    octCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    binCommand->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);

    //Hide scientific buttons
    modesFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    scientificFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    funcBtns[FUNC_PI]->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    advancedMatrix->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    simpleFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    hexFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
  }
  else
  {
    //Show scientific menus
    extrasSep->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    hexCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    decCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    octCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    binCommand->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);

    //Show scientific buttons
    modesFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    scientificFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    funcBtns[FUNC_PI]->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    advancedMatrix->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
    simpleFrame->handle(this,MKUINT(ID_HIDE,SEL_COMMAND),NULL);
    hexFrame->handle(this,MKUINT(ID_SHOW,SEL_COMMAND),NULL);
  }

  handle(this,MKUINT(ID_CLEARALL,SEL_COMMAND),NULL);
  base=BASE_TEN;
  rep=DEGREES;
  mode=m;
  resize(width[m-1]>0?width[m-1]:getDefaultWidth(),height[m-1]>0?height[m-1]:getDefaultHeight());
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
//Maybe toggling digit grouping shouldn't be finishing the entry
//  started=FALSE;
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
      FXint fin=(str[0]=='-')?1:0;
      count=str.findb('.')-3;
      while(count>fin)
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
  FXFontDesc fontdesc;
  FXchar fontspec[200];

  //Make sure the width and height for this mode are current
  width[mode-1]=getWidth();
  height[mode-1]=getHeight();

  //Dimensions
  getApp()->reg().writeIntEntry("SETTINGS","x",getX());
  getApp()->reg().writeIntEntry("SETTINGS","y",getY());
  getApp()->reg().writeIntEntry("SETTINGS","standardwidth",width[CALC_STANDARD-1]);
  getApp()->reg().writeIntEntry("SETTINGS","standardheight",height[CALC_STANDARD-1]);
  getApp()->reg().writeIntEntry("SETTINGS","scientificwidth",width[CALC_SCIENTIFIC-1]);
  getApp()->reg().writeIntEntry("SETTINGS","scientificheight",height[CALC_SCIENTIFIC-1]);

  //Fonts and colors
  getDisplayFont()->getFontDesc(fontdesc);
  fxunparsefontdesc(fontspec,fontdesc);
  getApp()->reg().writeStringEntry("SETTINGS","displayfont",fontspec);

  getButtonFont()->getFontDesc(fontdesc);
  fxunparsefontdesc(fontspec,fontdesc);
  getApp()->reg().writeStringEntry("SETTINGS","buttonfont",fontspec);

  getApp()->reg().writeColorEntry("SETTINGS","displaycolor",getDisplayColor());
  getApp()->reg().writeColorEntry("SETTINGS","digitcolor",getDigitColor());
  getApp()->reg().writeColorEntry("SETTINGS","hexdigitcolor",getHexDigitColor());
  getApp()->reg().writeColorEntry("SETTINGS","operatorcolor",getOperatorColor());
  getApp()->reg().writeColorEntry("SETTINGS","functioncolor",getFunctionColor());
  getApp()->reg().writeColorEntry("SETTINGS","memorycolor",getMemoryColor());
  getApp()->reg().writeColorEntry("SETTINGS","statcolor",getStatColor());
  getApp()->reg().writeColorEntry("SETTINGS","numbasecolor",getBaseColor());
  getApp()->reg().writeColorEntry("SETTINGS","repcolor",getRepColor());
  getApp()->reg().writeColorEntry("SETTINGS","invcolor",getInvColor());
  getApp()->reg().writeColorEntry("SETTINGS","hypcolor",getHypColor());
  getApp()->reg().writeColorEntry("SETTINGS","backspacecolor",getBackspaceColor());
  getApp()->reg().writeColorEntry("SETTINGS","clearentrycolor",getClearEntryColor());
  getApp()->reg().writeColorEntry("SETTINGS","clearallcolor",getClearAllColor());

  getApp()->reg().writeColorEntry("SETTINGS","displaytextcolor",getDisplayTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","digittextcolor",getDigitTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","hexdigittextcolor",getHexDigitTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","operatortextcolor",getOperatorTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","functiontextcolor",getFunctionTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","memorytextcolor",getMemoryTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","stattextcolor",getStatTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","numbasetextcolor",getBaseTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","reptextcolor",getRepTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","invtextcolor",getInvTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","hyptextcolor",getHypTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","backspacetextcolor",getBackspaceTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","clearentrytextcolor",getClearEntryTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","clearalltextcolor",getClearAllTextColor());

  //Settings
  getApp()->reg().writeUnsignedEntry("SETTINGS","mode",getMode());
  getApp()->reg().writeUnsignedEntry("SETTINGS","digits",ndigits);
  getApp()->reg().writeIntEntry("SETTINGS","grouping",digitgrouping);
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

long CALCWindow::onCmdPreferences(FXObject*,FXSelector,void*)
{
  CALCPrefsBox dialog(this);
  dialog.execute();
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

  msg.format("Scientific Calculator (IEEE 754 %s-bit floating point)\nVersion "PROG_VERSION"\n\nCopyright (C) 2000-2002 Dustin Graves (dgraves@computer.org)\n\n"\
"This software uses the FOX Platform Independent GUI Toolkit Library.\n"\
"The FOX Library is Copyright (C) 1997,2000-2002 Jeroen van der Zijp and is\n"\
"available freely under the GNU Lesser Public License at the following site:\n"\
"http://www.fox-toolkit.org",bits.text());

#ifdef HAVE_DOUBLEDOUBLE
  msg.append("\n\n"\
"This software contains code from version 2.2 of the `doubledouble' library.\n"\
"The doubledouble library is Copyright (C) 1997 Keith Martin Briggs and is\n"\
"available freely under the GNU General Public License at the following site:\n"\
"http://www.btexact.com/people/briggsk2/doubledouble.html");
#endif

  FXDialogBox about(this,"About Box",DECOR_TITLE|DECOR_BORDER);
  FXHorizontalFrame* buttons=new FXHorizontalFrame(&about,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  new FXButton(buttons,"&Close",NULL,&about,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  new FXHorizontalSeparator(&about,SEPARATOR_RIDGE|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  FXVerticalFrame* aboutframe=new FXVerticalFrame(&about,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(aboutframe,"About fxcalc");
  new FXHorizontalSeparator(aboutframe,SEPARATOR_LINE|LAYOUT_FILL_X);
  FXHorizontalFrame* aboutlabels=new FXHorizontalFrame(aboutframe,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(aboutlabels,NULL,bigcalc,JUSTIFY_LEFT|LAYOUT_CENTER_Y,0,0,0,0, 20,20,20,20);
  new FXLabel(aboutlabels,msg,NULL,JUSTIFY_LEFT|LAYOUT_CENTER_Y,0,0,0,0, 0,20,20,20);

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
    statBox->show(PLACEMENT_CURSOR);
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
    if((((pos-val.count(grp))+1)<max)&&((base!=BASE_EIGHT)||octIsValid(val,max)))
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
	  FXint fin=(val[0]=='-')?1:0;

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

          while(count>fin)
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
//simply exp(y*log(x)) and x can not be <= 0
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

  if((pos-val.count(' '))<max)
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
  {
    if(val>=((CALCdouble)0.0))
      setLabelText(sqrt(val));
    else
      lcd->setText("Invalid value for operation.");
  }
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

long CALCWindow::onCmdDisplayFont(FXObject*,FXSelector,void*)
{
  FXFontDialog dialog(this,"Change Display Font",DECOR_BORDER|DECOR_TITLE);
  FXFontDesc fontdesc;

  lcd->getFont()->getFontDesc(fontdesc);
  dialog.setFontSelection(fontdesc);
  if(dialog.execute())
  {
    FXFont *oldfont=lcdfont;
    dialog.getFontSelection(fontdesc);
    lcdfont=new FXFont(getApp(),fontdesc);
    lcdfont->create();
    setDisplayFont(lcdfont);
    if(oldfont) delete oldfont;
  }
  return 1;
}

long CALCWindow::onCmdButtonFont(FXObject*,FXSelector,void*)
{
  FXFontDialog dialog(this,"Change Display Font",DECOR_BORDER|DECOR_TITLE);
  FXFontDesc fontdesc;

  baseBtns[0]->getFont()->getFontDesc(fontdesc);
  dialog.setFontSelection(fontdesc);
  if(dialog.execute())
  {
    FXFont *oldfont=btnfont;
    dialog.getFontSelection(fontdesc);
    btnfont=new FXFont(getApp(),fontdesc);
    btnfont->create();
    setButtonFont(btnfont);
    if(oldfont) delete oldfont;
  }
  return 1;
}

long CALCWindow::onCmdCalcWidth(FXObject* sender,FXSelector,void*)
{
  FXint cw=getDefaultWidth();
  sender->handle(this,MKUINT(ID_GETINTVALUE,SEL_COMMAND),(void*)&cw);
  resize(cw>0?cw:getDefaultWidth(),getHeight());
  return 1;
}

long CALCWindow::onCmdCalcHeight(FXObject* sender,FXSelector,void*)
{
  FXint ch=getDefaultWidth();
  sender->handle(this,MKUINT(ID_GETINTVALUE,SEL_COMMAND),(void*)&ch);
  resize(getWidth(),ch>0?ch:getDefaultHeight());
  return 1;
}

long CALCWindow::onCmdNumDigits(FXObject* sender,FXSelector,void*)
{
  FXint digits=CALCDBL_DIG;
  sender->handle(this,MKUINT(ID_GETINTVALUE,SEL_COMMAND),(void*)&digits);
  ndigits=digits;

  //Refresh display
  setLabelText(getLabelText());
  return 1;
}

long CALCWindow::onCmdBackColor(FXObject*,FXSelector sel,void* ptr)
{
  FXColor color=(FXColor)(long)ptr;
  switch(SELID(sel))
  {
    case ID_COLOR_DISPLAY: setDisplayColor(color); break;
    case ID_COLOR_DIGITS: setDigitColor(color); break;
    case ID_COLOR_HEXDIGITS: setHexDigitColor(color); break;
    case ID_COLOR_OPERATORS: setOperatorColor(color); break;
    case ID_COLOR_FUNCTIONS: setFunctionColor(color); break;
    case ID_COLOR_MEMORY: setMemoryColor(color); break;
    case ID_COLOR_STATISTICS: setStatColor(color); break;
    case ID_COLOR_BASE: setBaseColor(color); break;
    case ID_COLOR_REPRESENTATION: setRepColor(color); break;
    case ID_COLOR_INV: setInvColor(color); break;
    case ID_COLOR_HYP: setHypColor(color); break;
    case ID_COLOR_BACKSPACE: setBackspaceColor(color); break;
    case ID_COLOR_CLEARENTRY: setClearEntryColor(color); break;
    case ID_COLOR_CLEARALL: setClearAllColor(color); break;
  }
  return 1;
}

long CALCWindow::onCmdTextColor(FXObject*,FXSelector sel,void* ptr)
{
  FXColor color=(FXColor)(long)ptr;
  switch(SELID(sel))
  {
    case ID_TEXTCOLOR_DISPLAY: setDisplayTextColor(color); break;
    case ID_TEXTCOLOR_DIGITS: setDigitTextColor(color); break;
    case ID_TEXTCOLOR_HEXDIGITS: setHexDigitTextColor(color); break;
    case ID_TEXTCOLOR_OPERATORS: setOperatorTextColor(color); break;
    case ID_TEXTCOLOR_FUNCTIONS: setFunctionTextColor(color); break;
    case ID_TEXTCOLOR_MEMORY: setMemoryTextColor(color); break;
    case ID_TEXTCOLOR_STATISTICS: setStatTextColor(color); break;
    case ID_TEXTCOLOR_BASE: setBaseTextColor(color); break;
    case ID_TEXTCOLOR_REPRESENTATION: setRepTextColor(color); break;
    case ID_TEXTCOLOR_INV: setInvTextColor(color); break;
    case ID_TEXTCOLOR_HYP: setHypTextColor(color); break;
    case ID_TEXTCOLOR_BACKSPACE: setBackspaceTextColor(color); break;
    case ID_TEXTCOLOR_CLEARENTRY: setClearEntryTextColor(color); break;
    case ID_TEXTCOLOR_CLEARALL: setClearAllTextColor(color); break;
  }
  return 1;
}

long CALCWindow::onUpdCalcWidth(FXObject* sender,FXSelector,void*)
{
  FXint cw=getWidth();
  sender->handle(this,MKUINT(ID_SETINTVALUE,SEL_COMMAND),(void*)&cw);
  return 1;
}

long CALCWindow::onUpdCalcHeight(FXObject* sender,FXSelector,void*)
{
  FXint ch=getHeight();
  sender->handle(this,MKUINT(ID_SETINTVALUE,SEL_COMMAND),(void*)&ch);
  return 1;
}

long CALCWindow::onUpdNumDigits(FXObject* sender,FXSelector,void*)
{
  sender->handle(this,MKUINT(ID_SETINTVALUE,SEL_COMMAND),(void*)&ndigits);
  return 1;
}

long CALCWindow::onUpdBackColor(FXObject* sender,FXSelector sel,void*)
{
  FXColor color;
  switch(SELID(sel))
  {
    case ID_COLOR_DISPLAY: color=getDisplayColor(); break;
    case ID_COLOR_DIGITS: color=getDigitColor(); break;
    case ID_COLOR_HEXDIGITS: color=getHexDigitColor(); break;
    case ID_COLOR_OPERATORS: color=getOperatorColor(); break;
    case ID_COLOR_FUNCTIONS: color=getFunctionColor(); break;
    case ID_COLOR_MEMORY: color=getMemoryColor(); break;
    case ID_COLOR_STATISTICS: color=getStatColor(); break;
    case ID_COLOR_BASE: color=getBaseColor(); break;
    case ID_COLOR_REPRESENTATION: color=getRepColor(); break;
    case ID_COLOR_INV: color=getInvColor(); break;
    case ID_COLOR_HYP: color=getHypColor(); break;
    case ID_COLOR_BACKSPACE: color=getBackspaceColor(); break;
    case ID_COLOR_CLEARENTRY: color=getClearEntryColor(); break;
    case ID_COLOR_CLEARALL: color=getClearAllColor(); break;
  }
  sender->handle(this,MKUINT(ID_SETINTVALUE,SEL_COMMAND),(void*)&color);
  return 1;
}

long CALCWindow::onUpdTextColor(FXObject* sender,FXSelector sel,void*)
{
  FXColor color;
  switch(SELID(sel))
  {
    case ID_TEXTCOLOR_DISPLAY: color=getDisplayTextColor(); break;
    case ID_TEXTCOLOR_DIGITS: color=getDigitTextColor(); break;
    case ID_TEXTCOLOR_HEXDIGITS: color=getHexDigitTextColor(); break;
    case ID_TEXTCOLOR_OPERATORS: color=getOperatorTextColor(); break;
    case ID_TEXTCOLOR_FUNCTIONS: color=getFunctionTextColor(); break;
    case ID_TEXTCOLOR_MEMORY: color=getMemoryTextColor(); break;
    case ID_TEXTCOLOR_STATISTICS: color=getStatTextColor(); break;
    case ID_TEXTCOLOR_BASE: color=getBaseTextColor(); break;
    case ID_TEXTCOLOR_REPRESENTATION: color=getRepTextColor(); break;
    case ID_TEXTCOLOR_INV: color=getInvTextColor(); break;
    case ID_TEXTCOLOR_HYP: color=getHypTextColor(); break;
    case ID_TEXTCOLOR_BACKSPACE: color=getBackspaceTextColor(); break;
    case ID_TEXTCOLOR_CLEARENTRY: color=getClearEntryTextColor(); break;
    case ID_TEXTCOLOR_CLEARALL: color=getClearAllTextColor(); break;
  }
  sender->handle(this,MKUINT(ID_SETINTVALUE,SEL_COMMAND),(void*)&color);
  return 1;
}

long CALCWindow::onCmdDefaultDisplayFont(FXObject*,FXSelector,void*)
{
  setDisplayFont(getApp()->getNormalFont());
  return 1;
}

long CALCWindow::onCmdDefaultButtonFont(FXObject*,FXSelector,void*)
{
  setButtonFont(getApp()->getNormalFont());
  return 1;
}

long CALCWindow::onCmdDefaultMode(FXObject*,FXSelector,void*)
{
  setMode(CALC_SCIENTIFIC);
  return 1;
}

long CALCWindow::onCmdDefaultWidth(FXObject*,FXSelector,void*)
{
  resize(getDefaultWidth(),getHeight());
  return 1;
}

long CALCWindow::onCmdDefaultHeight(FXObject*,FXSelector,void*)
{
  resize(getWidth(),getDefaultHeight());
  return 1;
}

long CALCWindow::onCmdDefaultNumDigits(FXObject*,FXSelector,void*)
{
  ndigits=CALCDBL_DIG;
  return 1;
}

long CALCWindow::onCmdDefaultColors(FXObject*,FXSelector,void*)
{
  setDisplayColor(getApp()->getBackColor());
  setDigitColor(getApp()->getBaseColor());
  setHexDigitColor(getApp()->getBaseColor());
  setOperatorColor(getApp()->getBaseColor());
  setFunctionColor(getApp()->getBaseColor());
  setMemoryColor(getApp()->getBaseColor());
  setStatColor(getApp()->getBaseColor());
  setBaseColor(getApp()->getBackColor());
  setRepColor(getApp()->getBackColor());
  setInvColor(getApp()->getBackColor());
  setHypColor(getApp()->getBackColor());
  setBackspaceColor(getApp()->getBaseColor());
  setClearEntryColor(getApp()->getBaseColor());
  setClearAllColor(getApp()->getBaseColor());

  return 1;
}

long CALCWindow::onCmdDefaultTextColors(FXObject*,FXSelector,void*)
{
  setDisplayTextColor(FXRGB(0,0,0));
  setDigitTextColor(VALUE);
  setHexDigitTextColor(FUNCTION);
  setOperatorTextColor(OPERATOR);
  setFunctionTextColor(FUNCTION);
  setMemoryTextColor(OPERATOR);
  setStatTextColor(FUNCTION);
  setBaseTextColor(FXRGB(0,0,0));
  setRepTextColor(FXRGB(0,0,0));
  setInvTextColor(FXRGB(0,0,0));
  setHypTextColor(FXRGB(0,0,0));
  setBackspaceTextColor(OPERATOR);
  setClearEntryTextColor(OPERATOR);
  setClearAllTextColor(OPERATOR);

  return 1;
}
