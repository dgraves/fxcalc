/* CALCHelp.h
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

#ifndef _CALCHELP_H_
#define _CALCHELP_H_

//Help messages for calculator widgets

#define LCD_HELP "Data Display\n\nDisplays entered numbers \n"\
"and computation results.  "

#define HEXADECIMAL_HELP "Hexadeximal (Base-16) Number Conversion\n\n"\
"Displays data in the hexadecimal number system.  \n"\
"Max value is 0xFFFFFFFFFFFFFFFF (64-bits all set to one).  \n\n"\
"Keyboard accelerator: F5"

#define DECIMAL_HELP "Decimal (Base-10) Number Conversion\n\n"\
"Displays data in decimal number system.  \n\n"\
"Keyboard accelerator: F6"

#define OCTAL_HELP "Octal (Base-8) Number Conversion\n\n"\
"Displays data in the octal number system.  \n"\
"Max Value is 01777777777777777777777 (64-bits all set to one).  \n\n"\
"Keyboard accelerator: F7"

#define BINARY_HELP "Binary (Base-2) Number Conversion\n\n"\
"Displays data in the binary number system.  \n"\
"Max Value is 64-bits all set to one.  \n\n"\
"Keyboard accelerator: F8"

#define DEGREES_HELP "Trigonometric Representation in Degrees\n\n"\
"Treats data as degrees for trigonometric functions.  \n\n"\
"Keyboard accelerator: F2"

#define RADIANS_HELP "Trigonometric Representation in Radians\n\n"\
"Treats data as radians for trigonometric functions.  \n\n"\
"Keyboard accelerator: F3"

#define GRADIENTS_HELP "Trigonometric Representation in Gradients\n\n"\
"Treats data as gradients for trigonometric functions.  \n\n"\
"Keyboard accelerator: F4"

#define DLWORD_HELP "Double Long Word\n\n"\
"Treats number as a double long word.  \n"\
"Displays up to 64-bits of data.  \n\n"\
"Keyboard accelerator: F12"

#define LWORD_HELP "Long Word\n\n"\
"Treats number as a long word.  \n"\
"Displays up to 32-bits of data.  \n\n"\
"Keyboard accelerator: F2"

#define WORD_HELP "Word\n\n"\
"Treats number as a word.  \n"\
"Displays up to 16-bits of data.  \n\n"\
"Keyboard accelerator: F3"

#define BYTE_HELP "Byte\n\n"\
"Treats number as a byte.  \n"\
"Displays up to 8-bits of data.  \n\n"\
"Keyboard accelerator: F4"

#define INV_HELP "Invert Functions\n\n"\
"Perform inverse operation for select functions.  \n"\
"When checked, labels for functions with inverse\n"\
"operations will be changed.  \n\n"\
"Keyboard accelerator: I"

#define HYP_HELP "Hyperbolic Functionality\n\n"\
"Perform hyperbolic functions for sin, cos, and tan.  \n"\
"When checked, labels for functions with hyperbolic\n"\
"operations will be changed.  \n\n"\
"Keyboaed accelerator: H"

#define SHOWSTATBOX_HELP "Display Statistics Box\n\n"\
"Displays the statistics box for usage.  Causes other \n"\
"statistics buttons to become usable.  \n\n"\
"Keyboard accelerator: Ctrl+S"

#define MEAN_HELP "Mean\n\n"\
"Computes the mean of the values contained in the \n"\
"statistics box.  Result is displayed in the main calculator display.  \n"\
"If \"inv\" is checked, will compute the mean of the squares.  \n\n"\
"Only available when the statistics box is present.  \n\n"\
"Keyboard accelerator: Ctrl+A"

#define SUM_HELP "Sum\n\n"\
"Computes the sum of the values contained in the \n"\
"statistics box.  Result is displayed in the main calculator display.  \n"\
"If \"inv\" is checked, will compute the sum of the squares.  \n\n"\
"Only available when the statistics box is present.  \n\n"\
"Keyboard accelerator: Ctrl+T"

#define STANDARDDEV_HELP "Sum\n\n"\
"Computes the standard deviation of the values contained in the \n"\
"statistics box, with a population value of n-1.  Result is displayed \n"\
"in the main calculator display.  If \"inv\" is checked, will compute \n"\
"the standard deviation with a population value of n.  \n\n"\
"Only available when the statistics box is present.  \n\n"\
"Keyboard accelerator: Ctrl+D"

#define ADDSTAT_HELP "Statistics Data\n\n"\
"Adds the contents of the main calculator display \n"\
"to the statistics box.  \n\n"\
"Only available when the statistics box is present.  \n\n"\
"Keyboard accelerator: Ins"

#define FIXED_HELP "Scientific Notation\n\n"\
"Toggles scientific notation on and off.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: V"

#define DMS_HELP "Degrees Minutes Seconds\n\n"\
"Convert notation from degrees decimal to DMS.  \n"\
"If \"inv\" is checked, will convert notation from \n"\
"DMS to degees decimal.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: M"

#define SIN_HELP "Sine Function\n\n"\
"Computes the sine of a value.  \n"\
"If \"inv\" is checked, will compute arc sine.  \n"\
"If \"hyp\" is checked, will compute hyperbolic sine.  \n"\
"If both \"hyp\" and \"inv\" are checked, will compute \n"\
"the arc hyperbolic sine.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: S"

#define COS_HELP "Cosine Function\n\n"\
"Computes the cosine of a value.  \n"\
"If \"inv\" is checked, will compute arc cosine.  \n"\
"If \"hyp\" is checked, will compute hyperbolic cosine.  \n"\
"If both \"hyp\" and \"inv\" are checked, will compute \n"\
"the arc hyperbolic cosine.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: O"

#define TAN_HELP "Tangent Function\n\n"\
"Computes the tangent of a value.  \n"\
"If \"inv\" is checked, will compute arc tangent.  \n"\
"If \"hyp\" is checked, will compute hyperbolic tangent.  \n"\
"If both \"hyp\" and \"inv\" are checked, will compute \n"\
"the arc hyperbolic tangent.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: T"

#define LPAREN_HELP "Left Parenthese\n\n"\
"Starts a new parentheses set.  \n\n"\
"Keyboard accelerator: ("

#define EXPONENT_HELP "Exponent\n\n"\
"Add an exponent to the current value.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: X"

#define POWY_HELP "Raise X to the Power Y\n\n"\
"Computes the value of x raised to the power y.  \n"\
"If \"inv\" is checked, will compute the yth root of x.  \n"\
"To use, enter a value for x, press this button, \n"\
"enter a value for y, and press '=' to see result.  \n\n"\
"Keyboard accelerator: Y"

#define POW3_HELP "X Cubed\n\n"\
"Computes cube of the current value.  \n"\
"If \"inv\" is checked, will compute the cube root.  \n\n"\
"Keyboard accelerator: #"

#define POW2_HELP "X Squared\n\n"\
"Computes square of the current value.  \n"\
"If \"inv\" is checked, will compute the square root.  \n\n"\
"Keyboard accelerator: @"

#define PARENLABEL_HELP "Parentheses Nesting Level\n\n"\
"Indicates the number of open parentheses, or the number \n"\
"left parentheses without matching right parentheses.  "

#define RPAREN_HELP "Right Parenthese\n\n"\
"Closes an open parentheses set.  \n\n"\
"Keyboard accelerator: )"

#define LN_HELP "Natural Logarithm\n\n"\
"Computes the natural logarithm (base-e) for the \n"\
"current value.  If \"inv\" is checked, will compute \n"\
"e raised to the power of the current value.  \n\n"\
"Keyboard accelerator: N"

#define LOG_HELP "Common Logarithm\n\n"\
"Computes the common logarithm (base-10) for the \n"\
"current value.  If \"inv\" is checked, will compute \n"\
"10 raised to the power of the current value.  \n\n"\
"Keyboard accelerator: L"

#define FACTORIAL_HELP "Factorial\n\n"\
"Computes the factorial of the current value.  \n\n"\
"Keyboard accelerator: !"

#define INVERT_HELP "Reciprocal\n\n"\
"Computes the reciprocal of the current value.  \n\n"\
"Keyboard accelerator: R"

#define MEMORYLABEL_HELP "Memory Usage\n\n"\
"Displays an 'M' to indicate that \n"\
"a value is stored in memory.  "

#define MEMORYCLEAR_HELP "Memory Clear\n\n"\
"Clears the stored value from memory.  \n\n"\
"Keyboard accelerator: Ctrl+L"

#define MEMORYRECALL_HELP "Memory Recall\n\n"\
"Loads stored value from memory to the \n"\
"main calculator display.  Value remains in memory.  \n\n"\
"Keyboard accelerator: Ctrl+R"

#define MEMORYSTORE_HELP "Memory Store\n\n"\
"Stores current value in memory.  Replaces previous \n"\
"value stored in memory if one is present.  \n\n"\
"Keyboard accelerator: Ctrl+M"

#define MEMORYADD_HELP "Memory Add\n\n"\
"Adds the current value to the value stored in memory.  \n\n"\
"Keyboard accelerator: Ctrl+P"

#define PI_HELP "Pi\n\n"\
"Displays the value of pi.  \n"\
"If \"inv\" is checked, will display the value of 2*pi.  \n"\
"Only available with the decimal number system.  \n\n"\
"Keyboard accelerator: P"

#define BACKSPACE_HELP "Erase\n\n"\
"Erases last digit from currently displayed value.  \n\n"\
"Keyboard accelerator: Backspace"

#define CLEARENTRY_HELP "Clear Entry\n\n"\
"Clears current value from display.  \n\n"\
"Keyboard accelerator: Delete"

#define CLEARALL_HELP "Clear All\n\n"\
"Clears all values and pending operations.  \n\n"\
"Keyboard accelerator: Escape"

#define SEVEN_HELP "Se7en\n\n"\
"Adds the number seven to the display.  \n\n"\
"Keyboard accelerator: 7"

#define EIGHT_HELP "Eight\n\n"\
"Adds the number eight to the display.  \n\n"\
"Keyboard accelerator: 8"

#define NINE_HELP "Nine\n\n"\
"Adds the number nine to the display.  \n\n"\
"Keyboard accelerator: 9"

#define DIVIDE_HELP "Divide\n\n"\
"Executes division operation.  \n\n"\
"Keyboard accelerator: /"

#define FOUR_HELP "Four\n\n"\
"Adds the number four to the display.  \n\n"\
"Keyboard accelerator: 4"

#define FIVE_HELP "Five\n\n"\
"Adds the number five to the display.  \n\n"\
"Keyboard accelerator: 5"

#define SIX_HELP "Six\n\n"\
"Adds the number six to the display.  \n\n"\
"Keyboard accelerator: 6"

#define MULTIPLY_HELP "Multiply\n\n"\
"Executes multiplication operation.  \n\n"\
"Keyboard accelerator: *"

#define ONE_HELP "One\n\n"\
"Adds the number One to the display.  \n\n"\
"Keyboard accelerator: 1"

#define TWO_HELP "Two\n\n"\
"Adds the number two to the display.  \n\n"\
"Keyboard accelerator: 2"

#define THREE_HELP "Three\n\n"\
"Adds the number three to the display.  \n\n"\
"Keyboard accelerator: 3"

#define SUBTRACT_HELP "Subtract\n\n"\
"Executes subtraction operation.  \n\n"\
"Keyboard accelerator: -"

#define ZERO_HELP "Zero\n\n"\
"Adds zero to the display.  \n\n"\
"Keyboard accelerator: 0"

#define UMINUS_HELP "Unary Plus/Minus\n\n"\
"Changes sign of current value.  \n\n"\
"Keyboard accelerator: F9"

#define DECPOINT_HELP "Decimal Point\n\n"\
"Adds a decimal point to the currently displayed value.  \n\n"\
"Keyboard accelerator: . or ,"

#define ADD_HELP "Add\n\n"\
"Executes addition operation.  \n\n"\
"Keyboard accelerator: +"

#define MODULUS_HELP "Modulus\n\n"\
"Computes the remainder of a division operation.  \n\n"\
"Keyboard accelerator: %"

#define OR_HELP "Bitwise Or\n\n"\
"Computes bitwise or.  \n"\
"Mantissa will be truncated.  \n\n"\
"Keyboard accelerator: |"

#define LEFTSHIFT_HELP "Bitwise Left Shift\n\n"\
"Computes bitwise left shift.  \n"\
"If \"inv\" is checked, will compute bitwise right shift.  \n"\
"To use, enter a value, press this button, \n"\
"enter the number of positions to shift, and press '=' to see result.  \n\n"\
"Keyboard accelerator: <"

#define EQUAL_HELP "Equal\n\n"\
"Computes pending operations and displays result \n"\
"in main calculator display.  Clicking multiple \n"\
"times repeats last operation.  \n\n"\
"Keyboard accelerator: = or Enter or Return"

#define AND_HELP "Bitwise And\n\n"\
"Computes bitwise and.  \n"\
"Mantissa will be truncated.  \n\n"\
"Keyboard accelerator: &"

#define XOR_HELP "Bitwise Exclusive Or\n\n"\
"Computes bitwise xor.  \n"\
"Mantissa will be truncated.  \n\n"\
"Keyboard accelerator: ^"

#define NOT_HELP "Bitwise Not\n\n"\
"Inverts all bits.  \n"\
"Mantissa will be truncated.  \n\n"\
"Keyboard accelerator: ~"

#define TRUNCATE_HELP "Truncate\n\n"\
"Truncates current fractional value, removing mantissa.  \n"\
"If \"inv\" is checked, will remove integer, \n"\
"leaving only the mantissa.  \n\n"\
"Keyboard accelerator: ;"

#define SQRT_HELP "Square Root\n\n"\
"Computes square root of the current value.  \n\n"\
"Keyboard accelerator: @"

#define PERCENT_HELP "Percentage\n\n"\
"Computes percentage from an operation.  \n"\
"To use, enter a value, press the * button, \n"\
"enter the percentage value, and press this \n"\
"button to see percentage of second value contained \n"\
"by the first.  \n\n"\
"Alternatively, enter a value, press the (*,/,+,-) button, \n"\
"enter the percentage value, press this button, and then \n"\
"press = to use calculated percentage as a variable in the \n"\
"selected operation.  \n\n"\
"Keyboard accelerator: %"

#define A_HELP "A\n\n"\
"Adds the hexadecimal digit A to the display.  \n"\
"Only available with the hexadecimal number system.  \n\n"\
"Keyboard accelerator: A"

#define B_HELP "B\n\n"\
"Adds the hexadecimal digit B to the display.  \n"\
"Only available with the hexadecimal number system.  \n\n"\
"Keyboard accelerator: B"

#define C_HELP "C\n\n"\
"Adds the hexadecimal digit C to the display.  \n"\
"Only available with the hexadecimal number system.  \n\n"\
"Keyboard accelerator: C"

#define D_HELP "D\n\n"\
"Adds the hexadecimal digit D to the display.  \n"\
"Only available with the hexadecimal number system.  \n\n"\
"Keyboard accelerator: D"

#define E_HELP "E\n\n"\
"Adds the hexadecimal digit E to the display.  \n"\
"Only available with the hexadecimal number system.  \n\n"\
"Keyboard accelerator: E"

#define F_HELP "F\n\n"\
"Adds the hexadecimal digit F to the display.  \n"\
"Only available with the hexadecimal number system.  \n\n"\
"Keyboard accelerator: F"


//Help messages for stat box widgets
#define STATVALUES_HELP "Data List\n\n"\
"Displays data intended for \n"\
"statistical operations.  "\

#define STATFORWARD_HELP "Raise Window\n\n"\
"Brings calculator window forward, \n"\
"giving it focus.  "

#define STATLOAD_HELP "Load Data\n\n"\
"Loads selected list item to \n"\
"main calculator display.  "

#define STATDELETE_HELP "Delete Data\n\n"\
"Removes selected list item, if \n"\
"one exists.  "

#define STATCLEAR_HELP "Clear List\n\n"\
"Removes all items present in \n"\
"the list.  "

#define STATPOPULATION_HELP "Population\n\n"\
"Indicates the number of items \n"\
"present in the list.  Refered \n"\
"to as the population.  "


//Help messages for the digit box
#define DIGITNUMDIGITS_HELP "Display Digits\n\n"\
"The total number of digits for the \n"\
"calculator to display.  This value \n"\
"includes the digits in both the mantissa \n"\
"and integer portions of the number.  \n\n"\
"A value of 9 would allow a number of \n"\
"the form 1234.56789 to be displayed.  "

#define DIGITDEFAULTDIGITS_HELP "Default Digits\n\n"\
"Sets the number of display digits to \n"\
"the system specified default.  This \n"\
"value will vary from system to system, \n"\
"depending on the available floating \n"\
"point precision.  "\

#endif
