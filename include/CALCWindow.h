/* CALCWindow.h
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

#ifndef _CALCWINDOW_H_
#define _CALCWINDOW_H_

#include "fox/fx.h"
#include "fox/FXElement.h"
#include "fox/FXArray.h"
#include "CALCdefs.h"

enum
{
  CALC_STANDARD=0x01,
  CALC_SCIENTIFIC=0x02
};

enum
{
  BASE_TWO    =0x01,
  BASE_EIGHT  =0x02,
  BASE_TEN    =0x04,
  BASE_SIXTEEN=0x08
};

enum
{
  DEGREES  =0x01,
  RADIANS  =0x02,
  GRADIENTS=0x04
};

enum
{
  NUM_BYTE  =0x01,
  NUM_WORD  =0x02,
  NUM_LWORD =0x04,      //Long word
  NUM_DLWORD=0x8        //Double long word
};

enum
{
  NOTATION_NONE=0x01,
  NOTATION_SCI =0x02
};

//modes
enum
{
  MODE_HEX=0,
  MODE_DEC=1,
  MODE_OCT=2,
  MODE_BIN=3,
  MODE_DEGREES=4,
  MODE_RADIANS=5,
  MODE_GRADIENTS=6,
  MODE_DLWORD=7,
  MODE_LWORD=8,
  MODE_WORD=9,
  MODE_BYTE=10,
  MODE_INV=11,
  MODE_HYP=12
};

//Digit buttons
enum
{
  DIGIT_0=0,
  DIGIT_1=1,
  DIGIT_2=2,
  DIGIT_3=3,
  DIGIT_4=4,
  DIGIT_5=5,
  DIGIT_6=6,
  DIGIT_7=7,
  DIGIT_8=8,
  DIGIT_9=9
};

//Hex digit buttons
enum
{
  DIGIT_A=0,
  DIGIT_B=1,
  DIGIT_C=2,
  DIGIT_D=3,
  DIGIT_E=4,
  DIGIT_F=5
};

//Opertor buttons
enum
{
  OPERATOR_UMINUS=0,
  OPERATOR_DECPOINT=1,
  OPERATOR_DIVIDE=2,
  OPERATOR_MULTIPLY=3,
  OPERATOR_MINUS=4,
  OPERATOR_PLUS=5,
  OPERATOR_MODULUS=6,
  OPERATOR_OR=7,
  OPERATOR_LEFTSHIFT=8,
  OPERATOR_EQUAL=9,
  OPERATOR_AND=10,
  OPERATOR_XOR=11,
  OPERATOR_NOT=12,
  OPERATOR_TRUNC=13,
  OPERATOR_SIMPLE_SQRT=14,
  OPERATOR_SIMPLE_PERCENT=15,
  OPERATOR_SIMPLE_INVERT=16,
  OPERATOR_SIMPLE_EQUAL=17
};

//Function buttons
enum
{
  FUNC_FIXED=0,
  FUNC_DMS=1,
  FUNC_SIN=2,
  FUNC_COS=3,
  FUNC_TAN=4,
  FUNC_LPAREN=5,
  FUNC_EXPONENT=6,
  FUNC_POWY=7,
  FUNC_POW3=8,
  FUNC_POW2=9,
  FUNC_RPAREN=10,
  FUNC_LN=11,
  FUNC_LOG=12,
  FUNC_FACTORIAL=13,
  FUNC_INVERT=14,
  FUNC_PI=15
};

//Memory buttons
enum
{
  MEMORY_CLEAR=0,
  MEMORY_RECALL=1,
  MEMORY_STORE=2,
  MEMORY_ADD=3
};

//Stat buttons
enum
{
  STAT_SHOW=0,
  STAT_MEAN=1,
  STAT_SUM=2,
  STAT_STANDARDDEV=3,
  STAT_ADD=4
};

//Clear buttons
enum
{
  CLEAR_BACKSPACE=0,
  CLEAR_ENTRY=1,
  CLEAR_ALL=2
};

//Had some trouble with the templated constructElms
struct stackPair {FXuint op; CALCdouble value;};
inline void constructElms(struct stackPair* ptr,unsigned int n){ptr=new stackPair[n];}

class CALCLabel;
class CALCStatBox;

class CALCWindow : public FXMainWindow
{
  FXDECLARE(CALCWindow)
protected:
  FXuint mode;
  FXuint base;
  FXuint rep;
  FXuint word;
  FXuint notation;
  FXuint op;
  FXuint ndigits;       //Number of digits to display
  FXbool inv;
  FXbool hyp;
  FXbool digitgrouping;
  FXbool started;
  FXbool mantissa;
  FXbool exponent;
  FXbool clarify;
  FXString clipping;

  CALCdouble value;
  CALCdouble memory;

  struct stackPair storePair;
  FXArray<stackPair> stack;

  FXIcon* bigcalc;
  FXIcon* smallcalc;
  CALCLabel* lcd;

  FXHorizontalFrame* modesFrame;
  FXSwitcher* modeSwitch;
  FXHorizontalFrame* scientificFrame;
  FXMatrix* piMatrix;
  FXMatrix* advancedMatrix;
  FXMatrix* simpleMatrix;
  FXMatrix* hexMatrix;

  FXMenuPane* filemenu;
  FXMenuPane* editmenu;
  FXMenuPane* viewmenu;
  FXMenuPane* helpmenu;
  FXMenuSeparator* extrasSep;
  FXMenuCommand* hexCommand;
  FXMenuCommand* decCommand;
  FXMenuCommand* octCommand;
  FXMenuCommand* binCommand;
  FXToolTip* tooltip;
  CALCStatBox* statBox;

  FXLabel* modeBtns[13];
  FXButton* digitBtns[10];
  FXButton* hexdigitBtns[6];
  FXButton* operatorBtns[18];
  FXButton* funcBtns[16];
  FXButton* memoryBtns[4];
  FXButton* statBtns[5];
  FXButton* clearBtns[3];

  CALCdouble powY(const CALCdouble& x,const CALCdouble& y);
  CALCdouble truncate(const CALCdouble& x);
  CALCdouble processOp(FXuint op,CALCdouble val);
  CALCdouble getLabelText() const;
  void setLabelText(CALCdouble val);
  void pasteData(FXuchar*,FXuint);
  FXbool octIsValid(FXString&,FXint);

  CALCWindow() {}

public:
  CALCWindow(FXApp* app);
  ~CALCWindow();

  virtual void create();

  void setMode(FXuint m);
  FXuint getMode() const;

  void setBase(FXuint b);
  FXuint getBase() const;

  void setWordType(FXuint w);
  FXuint getWordType() const;

  void setRepresentation(FXuint r);
  FXuint getRepresentation() const;

  void setNotation(FXuint n);
  FXuint getNotation() const;

  void setDigitGrouping(FXbool b);
  FXbool getDigitGrouping() const;

  long onClipboardRequest(FXObject*,FXSelector,void*);
  long onDNDDrop(FXObject*,FXSelector,void*);
  long onDNDRequest(FXObject*,FXSelector,void*);

  long onCmdGetLCDText(FXObject*,FXSelector,void*);
  long onCmdSetLCDText(FXObject*,FXSelector,void*);
  long onCmdPasteLCDText(FXObject*,FXSelector,void*);
  long onCmdGetLCDValue(FXObject*,FXSelector,void*);
  long onCmdSetLCDValue(FXObject*,FXSelector,void*);

  long onCmdQuit(FXObject*,FXSelector,void*);
  long onCmdCopy(FXObject*,FXSelector,void*);
  long onCmdPaste(FXObject*,FXSelector,void*);
  long onUpdPaste(FXObject*,FXSelector,void*);
  long onCmdNumDigits(FXObject*,FXSelector,void*);
  long onCmdDigitGrouping(FXObject*,FXSelector,void*);
  long onUpdDigitGrouping(FXObject*,FXSelector,void*);
  long onCmdUseTooltips(FXObject*,FXSelector,void*);
  long onUpdUseTooltips(FXObject*,FXSelector,void*);
  long onCmdAbout(FXObject*,FXSelector,void*);

  long onCmdMode(FXObject*,FXSelector,void*);
  long onUpdMode(FXObject*,FXSelector,void*);

  long onCmdBase(FXObject*,FXSelector,void*);
  long onUpdBase(FXObject*,FXSelector,void*);

  long onCmdRep(FXObject*,FXSelector,void*);
  long onUpdRep(FXObject*,FXSelector,void*);

  long onCmdWordType(FXObject*,FXSelector,void*);
  long onUpdWordType(FXObject*,FXSelector,void*);

  long onCmdInv(FXObject*,FXSelector,void*);
  long onCmdHyp(FXObject*,FXSelector,void*);
  long onUpdInv(FXObject*,FXSelector,void*);
  long onUpdHyp(FXObject*,FXSelector,void*);

  long onUpdParenLabel(FXObject*,FXSelector,void*);
  long onUpdMemoryLabel(FXObject*,FXSelector,void*);

  long onCmdBackSpace(FXObject*,FXSelector,void*);
  long onCmdClearEntry(FXObject*,FXSelector,void*);
  long onCmdClearAll(FXObject*,FXSelector,void*);

  long onCmdShowStatBox(FXObject*,FXSelector,void*);
  long onCmdMean(FXObject*,FXSelector,void*);
  long onCmdSum(FXObject*,FXSelector,void*);
  long onCmdStandardDev(FXObject*,FXSelector,void*);
  long onCmdAddStat(FXObject*,FXSelector,void*);
  long onUpdStatButtons(FXObject*,FXSelector,void*);

  long onCmdDigit(FXObject*,FXSelector,void*);
  long onCmdOperator(FXObject*,FXSelector,void*);
  long onCmdSqrt(FXObject*,FXSelector,void*);
  long onCmdPercent(FXObject*,FXSelector,void*);
  long onCmdNot(FXObject*,FXSelector,void*);
  long onCmdTruncate(FXObject*,FXSelector,void*);
  long onCmdUnaryMinus(FXObject*,FXSelector,void*);
  long onCmdDecPoint(FXObject*,FXSelector,void*);
  long onUpdLeftShift(FXObject*,FXSelector,void*);
  long onUpdTruncate(FXObject*,FXSelector,void*);

  long onCmdHexDigit(FXObject*,FXSelector,void*);
  long onUpdHexDigit(FXObject*,FXSelector,void*);
  long onUpdOctDigit(FXObject*,FXSelector,void*);
  long onUpdBinDigit(FXObject*,FXSelector,void*);

  long onCmdFixed(FXObject*,FXSelector,void*);
  long onCmdDMS(FXObject*,FXSelector,void*);
  long onCmdSin(FXObject*,FXSelector,void*);
  long onCmdCos(FXObject*,FXSelector,void*);
  long onCmdTan(FXObject*,FXSelector,void*);
  long onUpdFixed(FXObject*,FXSelector,void*);
  long onUpdDMS(FXObject*,FXSelector,void*);
  long onUpdSin(FXObject*,FXSelector,void*);
  long onUpdCos(FXObject*,FXSelector,void*);
  long onUpdTan(FXObject*,FXSelector,void*);

  long onCmdLParen(FXObject*,FXSelector,void*);
  long onCmdExponent(FXObject*,FXSelector,void*);
  long onCmdPowY(FXObject*,FXSelector,void*);
  long onCmdPow3(FXObject*,FXSelector,void*);
  long onCmdPow2(FXObject*,FXSelector,void*);
  long onUpdExponent(FXObject*,FXSelector,void*);
  long onUpdPowY(FXObject*,FXSelector,void*);
  long onUpdPow3(FXObject*,FXSelector,void*);
  long onUpdPow2(FXObject*,FXSelector,void*);

  long onCmdRParen(FXObject*,FXSelector,void*);
  long onCmdLn(FXObject*,FXSelector,void*);
  long onCmdLog(FXObject*,FXSelector,void*);
  long onCmdFactorial(FXObject*,FXSelector,void*);
  long onCmdInvert(FXObject*,FXSelector,void*);
  long onUpdLn(FXObject*,FXSelector,void*);
  long onUpdLog(FXObject*,FXSelector,void*);

  long onCmdMemoryClear(FXObject*,FXSelector,void*);
  long onCmdMemoryRecall(FXObject*,FXSelector,void*);
  long onCmdMemoryStore(FXObject*,FXSelector,void*);
  long onCmdMemoryAdd(FXObject*,FXSelector,void*);

  long onCmdPI(FXObject*,FXSelector,void*);
  long onUpdPI(FXObject*,FXSelector,void*);

  long onPopupMenu(FXObject*,FXSelector,void*);
  long onCmdClarify(FXObject*,FXSelector,void*);

  enum
  {
    ID_QUIT=FXMainWindow::ID_LAST,
    ID_COPY,
    ID_PASTE,
    ID_NUMDIGITS,
    ID_DIGITGROUPING,
    ID_USETOOLTIPS,
    ID_ABOUT,

    ID_LCD,
    ID_GETLCDTEXT,
    ID_SETLCDTEXT,
    ID_PASTELCDTEXT,
    ID_GETLCDVALUE,
    ID_SETLCDVALUE,

    ID_STANDARD,
    ID_SCIENTIFIC,

    ID_BINARY,
    ID_OCTAL,
    ID_DECIMAL,
    ID_HEXADECIMAL,
   
    ID_DEGREES,
    ID_RADIANS,
    ID_GRADIENTS,

    ID_BYTE,
    ID_WORD,
    ID_LWORD,
    ID_DLWORD,

    ID_INV,
    ID_HYP,

    ID_PARENLABEL,
    ID_MEMORYLABEL,

    ID_BACKSPACE,
    ID_CLEARENTRY,
    ID_CLEARALL,

    ID_SHOWSTATBOX,
    ID_MEAN,
    ID_SUM,
    ID_STANDARDDEV,
    ID_ADDSTAT,

    ID_0,
    ID_1,
    ID_2,
    ID_3,
    ID_4,
    ID_5,
    ID_6,
    ID_7,
    ID_8,
    ID_9,
    ID_A,
    ID_B,
    ID_C,
    ID_D,
    ID_E,
    ID_F,

    ID_MULTIPLY,
    ID_DIVIDE,
    ID_ADD,
    ID_SUBTRACT,
    ID_MODULUS,       //Start of extended operator set
    ID_AND,
    ID_OR,
    ID_XOR,
    ID_LEFTSHIFT,     //End of extended operator set
    ID_EQUAL,

    ID_SQRT,          //Simple operator set
    ID_PERCENT,       //Simple operator set
    ID_NOT,           //Extended operator set
    ID_TRUNCATE,
    ID_UMINUS,
    ID_DECPOINT,

    ID_FIXED,
    ID_DMS,
    ID_SIN,
    ID_COS,
    ID_TAN,

    ID_LPAREN,
    ID_EXPONENT,
    ID_POWY,
    ID_POW3,
    ID_POW2,

    ID_RPAREN,
    ID_LN,
    ID_LOG,
    ID_FACTORIAL,
    ID_INVERT,

    ID_MEMORYCLEAR,
    ID_MEMORYRECALL,
    ID_MEMORYSTORE,
    ID_MEMORYADD,

    ID_PI,

    ID_CLARIFY,
    ID_LAST
  };
};

#endif
