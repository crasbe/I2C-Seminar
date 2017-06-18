/**
 * @file LCD_I2C.h
 * 
 * @brief Library fuer I2C-LCDs
 * 
 * Basierend auf der NewLiquidCrystal-Library fuer Arduino-kompatible Systeme
 * von Francisco Malpartida
 * veroeffentlicht unter CC--BY-SA 3.0
 * 
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 */
 
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h
#include "i2cusb.h"

/*!
 @defined 
 @abstract   Performs a bitwise shift.
 @discussion Defines _BV bit shift which is very dependent macro defined by
 Atmel.

    \note The bit shift is performed by the compiler which then inserts the
    result into the code. Thus, there is no run-time overhead when using
    _BV().
*/
#ifndef _BV    
#define _BV(bit) (1 << (bit))
#endif

/*!
 @defined 
 @abstract   All these definitions shouldn't be used unless you are writing 
 a driver.
 @discussion All these definitions are for driver implementation only and
 shouldn't be used by applications.
 */
// LCD Commands
// ---------------------------------------------------------------------------
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
// ---------------------------------------------------------------------------
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off and cursor control
// ---------------------------------------------------------------------------
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
// ---------------------------------------------------------------------------
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
// ---------------------------------------------------------------------------
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00


// Define COMMAND and DATA LCD Rs (used by send method).
// ---------------------------------------------------------------------------
#define COMMAND                 0
#define LCD_DATA                1
#define FOUR_BITS               2


/*!
 @defined 
 @abstract   Defines the duration of the home and clear commands
 @discussion This constant defines the time it takes for the home and clear
 commands in the LCD - Time in microseconds.
 */
#define HOME_CLEAR_EXEC      2000

/*!
    @defined 
    @abstract   Backlight off constant declaration
    @discussion Used in combination with the setBacklight to swith off the
 LCD backlight. @set setBacklight
*/
#define BACKLIGHT_OFF           0

/*!
 @defined 
 @abstract   Backlight on constant declaration
 @discussion Used in combination with the setBacklight to swith on the
 LCD backlight. @set setBacklight
 */
#define BACKLIGHT_ON          255

/*!
 @typedef 
 @abstract   Define backlight control polarity
 @discussion Backlight control polarity. @see setBacklightPin.
 */
typedef enum { POSITIVE, NEGATIVE } t_backlighPol;

/*!
@method
@abstract   LCD definition
@discussion Defines all necessary variables, replacement for original constructor
@param      lcd_Addr[in] I2C address of the IO expansion module. For I2CLCDextraIO,
    the address can be configured using the on board jumpers.
    @param      En[in] LCD En (Enable) pin connected to the IO extender module
    @param      Rw[in] LCD Rw (Read/write) pin connected to the IO extender module
    @param      Rs[in] LCD Rs (Reset) pin connected to the IO extender module
    @param      d4[in] LCD data 0 pin map on IO extender module
    @param      d5[in] LCD data 1 pin map on IO extender module
    @param      d6[in] LCD data 2 pin map on IO extender module
    @param      d7[in] LCD data 3 pin map on IO extender module
*/
void initLCD (uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t backlighPin, t_backlighPol pol);

/*!
@function
@abstract   LCD initialization.
@discussion Initializes the LCD to a given size (col, row). This methods
initializes the LCD, therefore, it MUST be called prior to using any other
method from this class.

This method is abstract, a base implementation is available common to all LCD
drivers. Should it not be compatible with some other LCD driver, a derived
implementation should be done on the driver specif class.

@param      cols[in] the number of columns that the display has
@param      rows[in] the number of rows that the display has
@param      charsize[in] character size, default==LCD_5x8DOTS
*/
void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

/*!
@function
@abstract   Clears the LCD.
@discussion Clears the LCD screen and positions the cursor in the upper-left 
corner. 

This operation is time consuming for the LCD.

@param      none
*/
void clear();

/*!
@function
@abstract   Sets the cursor to the upper-left corner.
@discussion Positions the cursor in the upper-left of the LCD. 
That is, use that location in outputting subsequent text to the display. 
To also clear the display, use the clear() function instead.

This operation is time consuming for the LCD.

@param      none
*/
void home();

/*!
@function
@abstract   Turns off the LCD display.
@discussion Turns off the LCD display, without losing the text currently 
being displayed on it.

@param      none
*/
void noDisplay();

/*!
@function
@abstract   Turns on the LCD display.
@discussion Turns on the LCD display, after it's been turned off with 
noDisplay(). This will restore the text (and cursor location) that was on 
the display prior to calling noDisplay().

@param      none
*/
void display();

/*!
@function
@abstract   Turns off the blinking of the LCD cursor.

@param      none
*/
void noBlink();

/*!
@function
@abstract   Display the cursor of the LCD.
@discussion Display the blinking LCD cursor. If used in combination with 
the cursor() function, the result will depend on the particular display.

@param      none
*/
void blink();

/*!
@function
@abstract   Hides the LCD cursor.

@param      none
*/
void noCursor();

/*!
@function
@abstract   Display the LCD cursor.
@discussion Display the LCD cursor: an underscore (line) at the location 
where the next character will be written.

@param      none
*/
void cursor();

/*!
@function
@abstract   Scrolls the contents of the display (text and cursor) one space 
to the left.

@param      none
*/
void scrollDisplayLeft();

/*!
@function
@abstract   Scrolls the contents of the display (text and cursor) one space 
to the right.

@param      none
*/
void scrollDisplayRight();

/*!
@function
@abstract   Set the direction for text written to the LCD to left-to-right.
@discussion Set the direction for text written to the LCD to left-to-right. 
All subsequent characters written to the display will go from left to right, 
but does not affect previously-output text.

This is the default configuration.

@param      none
*/
void leftToRight();

/*!
@function
@abstract   Set the direction for text written to the LCD to right-to-left.
@discussion Set the direction for text written to the LCD to right-to-left. 
All subsequent characters written to the display will go from right to left, 
but does not affect previously-output text.

left-to-right is the default configuration.

@param      none
*/
void rightToLeft();

/*!
@function
@abstract   Moves the cursor one space to the left.
@discussion 
@param      none
*/   
void moveCursorLeft();


/*!
@function
@abstract   Moves the cursor one space to the right.

@param      none
*/
void moveCursorRight();

/*!
@function
@abstract   Turns on automatic scrolling of the LCD.
@discussion Turns on automatic scrolling of the LCD. This causes each 
character output to the display to push previous characters over by one 
space. If the current text direction is left-to-right (the default), 
the display scrolls to the left; if the current direction is right-to-left, 
the display scrolls to the right. 
This has the effect of outputting each new character to the same location on 
the LCD.

@param      none
*/
void autoscroll();

/*!
@function
@abstract   Turns off automatic scrolling of the LCD.
@discussion Turns off automatic scrolling of the LCD, this is the default
configuration of the LCD.

@param      none
*/
void noAutoscroll();

/*!
@function
@abstract   Creates a custom character for use on the LCD.
@discussion Create a custom character (glyph) for use on the LCD. 
Most chipsets only support up to eight characters of 5x8 pixels. Therefore,
this methods has been limited to locations (numbered 0 to 7). 

The appearance of each custom character is specified by an array of eight 
bytes, one for each row. The five least significant bits of each byte 
determine the pixels in that row. To display a custom character on screen, 
write()/print() its number, i.e. lcd.print (char(x)); // Where x is 0..7.

@param      location[in] LCD memory location of the character to create
(0 to 7)
@param      charmap[in] the bitmap array representing each row of the character.
*/
void createChar(uint8_t location, uint8_t charmap[]);


/*!
@function
@abstract   Creates a custom character for use on the LCD.
@discussion Create a custom character (glyph) for use on t{he LCD.
Most chipsets only support up to eight characters of 5x8 pixels. Therefore,
this methods has been limited to locations (numbered 0 to 7).

The appearance of each custom character is specified by an array of eight
bytes, one for each row. The five least significant bits of each byte
determine the pixels in that row. To display a custom character on screen,
write()/print() its number, i.e. lcd.print (char(x)); // Where x is 0..7.

This method take the character defined in program memory.

@param      location[in] LCD memory location of the character to create
(0 to 7)
@param      charmap[in] the bitmap array representing each row of the character.
			Usage for flash defined characters:
			const char str_pstr[] PROGMEM = {0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0};
*/
void createChar(uint8_t location, const char *charmap);


/*!
@function
@abstract   Position the LCD cursor.
@discussion Sets the position of the LCD cursor. Set the location at which 
subsequent text written to the LCD will be displayed.

@param      col[in] LCD column
@param      row[in] LCD row - line.
*/
void setCursor(uint8_t col, uint8_t row);

/*!
@function
@abstract   Switch-on the LCD backlight.
@discussion Switch-on the LCD backlight.
The setBacklightPin has to be called before setting the backlight for
this method to work. @see setBacklightPin. 
*/
void backlight ( void );

/*!
@function
@abstract   Switch-off the LCD backlight.
@discussion Switch-off the LCD backlight.
The setBacklightPin has to be called before setting the backlight for
this method to work. @see setBacklightPin. 
*/   
void noBacklight ( void );

/*!
@function
@abstract   Switch on the LCD module.
@discussion Switch on the LCD module, it will switch on the LCD controller
and the backlight. This method has the same effect of calling display and
backlight. @see display, @see backlight
*/
void on ( void );

/*!
@function
@abstract   Switch off the LCD module.
@discussion Switch off the LCD module, it will switch off the LCD controller
and the backlight. This method has the same effect of calling noDisplay and
noBacklight. @see display, @see backlight
*/   
void off ( void );

/*
@function
@abstract   Sets the pin to control the backlight.
@discussion Sets the pin in the device to control the backlight. This device
doesn't support dimming backlight capability.

@param      0: backlight off, 1..255: backlight on.
*/
void setBacklightPin ( uint8_t value, t_backlighPol pol );

/*!
@function
@abstract   Switch-on/off the LCD backlight.
@discussion Switch-on/off the LCD backlight.
The setBacklightPin has to be called before setting the backlight for
this method to work. @see setBacklightPin.

@param      value: backlight mode (HIGH|LOW)
*/
void setBacklight ( uint8_t value );

//Previously private methods/functions and variables

/*!
@method     
@abstract   Writes an 4 bit value to the LCD.
@discussion Writes 4 bits (the least significant) to the LCD control data lines.
@param      value[in] Value to write to the LCD
@param      more[in]  Value to distinguish between command and data.
COMMAND == command, DATA == data.
*/
void write4bits(uint8_t value, uint8_t mode);

/*!
@method     
@abstract   Pulse the LCD enable line (En).
@discussion Sends a pulse of 1 uS to the Enable pin to execute an command
or write operation.
*/
void pulseEnable(uint8_t);
