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

enum
{
  CALC_STANDARD  =0x01,
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
  NUM_DLWORD=0x08       //Double long word
};

enum
{
  NOTATION_NONE=0x01,
  NOTATION_SCI =0x02
};

//Numeric base array index values
enum
{
  BASE_HEX=0,
  BASE_DEC=1,
  BASE_OCT=2,
  BASE_BIN=3,
  BASE_LAST=4
};

//Representation array index values
enum
{
  REP_DEGREES=0,
  REP_RADIANS=1,
  REP_GRADIENTS=2,
  REP_DLWORD=3,
  REP_LWORD=4,
  REP_WORD=5,
  REP_BYTE=6,
  REP_LAST=7
};

//Mode array index values
enum
{
  MODE_INV=0,
  MODE_HYP=1,
  MODE_LAST=2
};

//Digit buttons array index values
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
  DIGIT_9=9,
  DIGIT_LAST=10
};

//Hex digit buttons array index values
enum
{
  HEXDIGIT_A=0,
  HEXDIGIT_B=1,
  HEXDIGIT_C=2,
  HEXDIGIT_D=3,
  HEXDIGIT_E=4,
  HEXDIGIT_F=5,
  HEXDIGIT_LAST=6,
};

//Opertor buttons array index values
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
  OPERATOR_SIMPLE_EQUAL=17,
  OPERATOR_LAST=18
};

//Function buttons array index values
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
  FUNC_PI=15,
  FUNC_LAST=16
};

//Memory buttons array index values
enum
{
  MEMORY_CLEAR=0,
  MEMORY_RECALL=1,
  MEMORY_STORE=2,
  MEMORY_ADD=3,
  MEMORY_LAST=4
};

//Stat buttons array index values
enum
{
  STAT_SHOW=0,
  STAT_MEAN=1,
  STAT_SUM=2,
  STAT_STANDARDDEV=3,
  STAT_ADD=4,
  STAT_LAST=5
};

//Clear buttons array index values
enum
{
  CLEAR_BACKSPACE=0,
  CLEAR_ENTRY=1,
  CLEAR_ALL=2,
  CLEAR_LAST=3
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
  FXint width[2];       //Storage for standard and scientific window widths
  FXint height[2];      //Storage for standard and scientific window heights
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
  FXFont* lcdfont;
  FXFont* btnfont;
  CALCLabel* lcd;

  FXHorizontalFrame* modesFrame;
  FXSwitcher* modeSwitch;
  FXHorizontalFrame* scientificFrame;
  FXMatrix* advancedMatrix;
  FXVerticalFrame* simpleFrame;
  FXHorizontalFrame* hexFrame;

  FXMenuPane* filemenu;
  FXMenuPane* editmenu;
  FXMenuPane* viewmenu;
  FXMenuPane* helpmenu;
  FXMenuSeparator* extrasSep;
  FXMenuCommand* hexCommand;
  FXMenuCommand* decCommand;
  FXMenuCommand* octCommand;
  FXMenuCommand* binCommand;
  FXTooltip* tooltip;
  CALCStatBox* statBox;

  FXLabel* baseBtns[5];
  FXLabel* repBtns[8];
  FXLabel* modeBtns[3];
  FXLabel* digitBtns[11];
  FXLabel* hexdigitBtns[7];
  FXLabel* operatorBtns[19];
  FXLabel* funcBtns[17];
  FXLabel* memoryBtns[5];
  FXLabel* statBtns[6];
  FXLabel* clearBtns[4];

  CALCdouble powY(const CALCdouble& x,const CALCdouble& y);
  CALCdouble truncate(const CALCdouble& x);
  CALCdouble processOp(FXuint op,CALCdouble val);
  CALCdouble getLabelText() const;
  void setLabelText(CALCdouble val);
  void pasteData(FXuchar*,FXuint);
  FXbool octIsValid(FXString&,FXint);

  CALCWindow() {}

  void setFont(FXLabel* targets[],FXFont* font);
  void setBackColor(FXLabel* targets,FXColor color);
  void setBackColor(FXLabel* targets[],FXColor color);
  void setTextColor(FXLabel* targets[],FXColor color);

public:
  CALCWindow(FXApp* app);
  ~CALCWindow();

  virtual void create();

  void setDisplayColor(FXColor color);
  void setDigitColor(FXColor color) {setBackColor(digitBtns,color);}
  void setHexDigitColor(FXColor color) {setBackColor(hexdigitBtns,color);}
  void setOperatorColor(FXColor color) {setBackColor(operatorBtns,color);}
  void setFunctionColor(FXColor color) {setBackColor(funcBtns,color);}
  void setMemoryColor(FXColor color) {setBackColor(memoryBtns,color);}
  void setStatColor(FXColor color) {setBackColor(statBtns,color);}
  void setBaseColor(FXColor color);
  void setRepColor(FXColor color);
  void setInvColor(FXColor color) {((FXCheckButton*)modeBtns[MODE_INV])->setBoxColor(color);}
  void setHypColor(FXColor color) {((FXCheckButton*)modeBtns[MODE_HYP])->setBoxColor(color);}
  void setBackspaceColor(FXColor color) {setBackColor(clearBtns[CLEAR_BACKSPACE],color);}
  void setClearEntryColor(FXColor color) {setBackColor(clearBtns[CLEAR_ENTRY],color);}
  void setClearAllColor(FXColor color) {setBackColor(clearBtns[CLEAR_ALL],color);}
  void setDisplayTextColor(FXColor color);
  void setDigitTextColor(FXColor color) {setTextColor(digitBtns,color);}
  void setHexDigitTextColor(FXColor color) {setTextColor(hexdigitBtns,color);}
  void setOperatorTextColor(FXColor color) {setTextColor(operatorBtns,color);}
  void setFunctionTextColor(FXColor color) {setTextColor(funcBtns,color);}
  void setMemoryTextColor(FXColor color) {setTextColor(memoryBtns,color);}
  void setStatTextColor(FXColor color) {setTextColor(statBtns,color);}
  void setBaseTextColor(FXColor color) {setTextColor(baseBtns,color);}
  void setRepTextColor(FXColor color) {setTextColor(repBtns,color);}
  void setInvTextColor(FXColor color) {((FXCheckButton*)modeBtns[MODE_INV])->setTextColor(color);}
  void setHypTextColor(FXColor color) {((FXCheckButton*)modeBtns[MODE_HYP])->setTextColor(color);}
  void setBackspaceTextColor(FXColor color) {clearBtns[CLEAR_BACKSPACE]->setTextColor(color);}
  void setClearEntryTextColor(FXColor color) {clearBtns[CLEAR_ENTRY]->setTextColor(color);}
  void setClearAllTextColor(FXColor color) {clearBtns[CLEAR_ALL]->setTextColor(color);}

  FXColor getDisplayColor() const;
  FXColor getDigitColor() const {return digitBtns[0]->getBackColor();}
  FXColor getHexDigitColor() const {return hexdigitBtns[0]->getBackColor();}
  FXColor getOperatorColor() const {return operatorBtns[0]->getBackColor();}
  FXColor getFunctionColor() const {return funcBtns[0]->getBackColor();}
  FXColor getMemoryColor() const {return memoryBtns[0]->getBackColor();}
  FXColor getStatColor() const {return statBtns[0]->getBackColor();}
  FXColor getBaseColor() const {return ((FXRadioButton*)baseBtns[0])->getRadioColor();}
  FXColor getRepColor() const {return ((FXRadioButton*)repBtns[0])->getRadioColor();}
  FXColor getInvColor() const {return ((FXCheckButton*)modeBtns[MODE_INV])->getBoxColor();}
  FXColor getHypColor() const {return ((FXCheckButton*)modeBtns[MODE_HYP])->getBoxColor();}
  FXColor getBackspaceColor() const {return clearBtns[CLEAR_BACKSPACE]->getBackColor();}
  FXColor getClearEntryColor() const {return clearBtns[CLEAR_ENTRY]->getBackColor();}
  FXColor getClearAllColor() const {return clearBtns[CLEAR_ALL]->getBackColor();}
  FXColor getDisplayTextColor() const;
  FXColor getDigitTextColor() const {return digitBtns[0]->getTextColor();}
  FXColor getHexDigitTextColor() const {return hexdigitBtns[0]->getTextColor();}
  FXColor getOperatorTextColor() const {return operatorBtns[0]->getTextColor();}
  FXColor getFunctionTextColor() const {return funcBtns[0]->getTextColor();}
  FXColor getMemoryTextColor() const {return memoryBtns[0]->getTextColor();}
  FXColor getStatTextColor() const {return statBtns[0]->getTextColor();}
  FXColor getBaseTextColor() const {return baseBtns[0]->getTextColor();}
  FXColor getRepTextColor() const {return repBtns[0]->getTextColor();}
  FXColor getInvTextColor() const {return modeBtns[MODE_INV]->getTextColor();}
  FXColor getHypTextColor() const {return modeBtns[MODE_HYP]->getTextColor();}
  FXColor getBackspaceTextColor() const {return clearBtns[CLEAR_BACKSPACE]->getTextColor();}
  FXColor getClearEntryTextColor() const {return clearBtns[CLEAR_ENTRY]->getTextColor();}
  FXColor getClearAllTextColor() const {return clearBtns[CLEAR_ALL]->getTextColor();}

  void setDisplayFont(FXFont* font);
  FXFont* getDisplayFont() const;

  void setButtonFont(FXFont* font);
  FXFont* getButtonFont() const {return digitBtns[0]->getFont();}

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
  long onCmdPreferences(FXObject*,FXSelector,void*);
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

  long onCmdDisplayFont(FXObject*,FXSelector,void*);
  long onCmdButtonFont(FXObject*,FXSelector,void*);
  long onCmdCalcWidth(FXObject*,FXSelector,void*);
  long onCmdCalcHeight(FXObject*,FXSelector,void*);
  long onCmdNumDigits(FXObject*,FXSelector,void*);
  long onCmdBackColor(FXObject*,FXSelector,void*);
  long onCmdTextColor(FXObject*,FXSelector,void*);
  long onUpdCalcWidth(FXObject*,FXSelector,void*);
  long onUpdCalcHeight(FXObject*,FXSelector,void*);
  long onUpdNumDigits(FXObject*,FXSelector,void*);
  long onUpdBackColor(FXObject*,FXSelector,void*);
  long onUpdTextColor(FXObject*,FXSelector,void*);

  long onCmdDefaultDisplayFont(FXObject*,FXSelector,void*);
  long onCmdDefaultButtonFont(FXObject*,FXSelector,void*);
  long onCmdDefaultMode(FXObject*,FXSelector,void*);
  long onCmdDefaultWidth(FXObject*,FXSelector,void*);
  long onCmdDefaultHeight(FXObject*,FXSelector,void*);
  long onCmdDefaultNumDigits(FXObject*,FXSelector,void*);
  long onCmdDefaultColors(FXObject*,FXSelector,void*);
  long onCmdDefaultTextColors(FXObject*,FXSelector,void*);

  long onPopupMenu(FXObject*,FXSelector,void*);
  long onCmdClarify(FXObject*,FXSelector,void*);

  enum
  {
    ID_QUIT=FXMainWindow::ID_LAST,
    ID_COPY,
    ID_PASTE,
    ID_PREFERENCES,
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

    ID_DISPLAYFONT,
    ID_BUTTONFONT,
    ID_CALCWIDTH,
    ID_CALCHEIGHT,
    ID_NUMDIGITS,
    ID_COLOR_DISPLAY,
    ID_COLOR_DIGITS,
    ID_COLOR_HEXDIGITS,
    ID_COLOR_OPERATORS,
    ID_COLOR_FUNCTIONS,
    ID_COLOR_MEMORY,
    ID_COLOR_STATISTICS,
    ID_COLOR_BASE,
    ID_COLOR_REPRESENTATION,
    ID_COLOR_INV,
    ID_COLOR_HYP,
    ID_COLOR_BACKSPACE,
    ID_COLOR_CLEARENTRY,
    ID_COLOR_CLEARALL,
    ID_TEXTCOLOR_DISPLAY,
    ID_TEXTCOLOR_DIGITS,
    ID_TEXTCOLOR_HEXDIGITS,
    ID_TEXTCOLOR_OPERATORS,
    ID_TEXTCOLOR_FUNCTIONS,
    ID_TEXTCOLOR_MEMORY,
    ID_TEXTCOLOR_STATISTICS,
    ID_TEXTCOLOR_BASE,
    ID_TEXTCOLOR_REPRESENTATION,
    ID_TEXTCOLOR_INV,
    ID_TEXTCOLOR_HYP,
    ID_TEXTCOLOR_BACKSPACE,
    ID_TEXTCOLOR_CLEARENTRY,
    ID_TEXTCOLOR_CLEARALL,

    ID_DEFAULTDISPLAYFONT,
    ID_DEFAULTBUTTONFONT,
    ID_DEFAULTMODE,
    ID_DEFAULTWIDTH,
    ID_DEFAULTHEIGHT,
    ID_DEFAULTNUMDIGITS,
    ID_DEFAULTCOLORS,
    ID_DEFAULTTEXTCOLORS,

    ID_CLARIFY,
    ID_LAST
  };
};

#endif
