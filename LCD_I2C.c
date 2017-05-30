//Previously private variables, now created as static variables
//IMPORTANT: This means that the library can only control ONE display at any given time
static uint8_t _Addr;             // I2C Address of the IO expander
static uint8_t _backlightPinMask; // Backlight IO pin mask
static uint8_t _backlightStsMask; // Backlight status mask
static uint8_t _En;               // LCD expander word for enable pin
static uint8_t _Rw;               // LCD expander word for R/W pin
static uint8_t _Rs;               // LCD expander word for Register Select pin
static uint8_t _data_pins[4];     // LCD data lines
static uint8_t _dirMask     = 0xFF;// mark all as INPUTs
static uint8_t _shadow      = 0x0; // no values set

// General LCD commands - generic methods used by the rest of the commands
// ---------------------------------------------------------------------------
void command(uint8_t value) 
{
   send(value, COMMAND);
}

void write(uint8_t value)
{
   send(value, LCD_DATA);
}

//
// Switch on the backlight
void backlight ( void )
{
   setBacklight(255);
}

//
// Switch off the backlight
void noBacklight ( void )
{
   setBacklight(0);
}

//
// Switch fully on the LCD (backlight and LCD)
void on ( void )
{
   display();
   backlight();
}

//
// Switch fully off the LCD (backlight and LCD) 
void off ( void )
{
   noBacklight();
   noDisplay();
}


// Write to CGRAM of new characters
void createChar(uint8_t location, uint8_t charmap[]) 
{
   location &= 0x7;            // we only have 8 locations 0-7
   
   command(LCD_SETCGRAMADDR | (location << 3));
   delayMicroseconds(30);
   
   for (uint8_t i = 0; i < 8; i++)
   {
      write(charmap[i]);      // call the virtual write method
      delayMicroseconds(40);
   }
}


void clear()
{
   command(LCD_CLEARDISPLAY);             // clear display, set cursor position to zero
   delayMicroseconds(HOME_CLEAR_EXEC);    // this command is time consuming
}

void home()
{
   command(LCD_RETURNHOME);             // set cursor position to zero
   delayMicroseconds(HOME_CLEAR_EXEC);  // This command is time consuming
}

void setCursor(uint8_t col, uint8_t row)
{
   const byte row_offsetsDef[]   = { 0x00, 0x40, 0x14, 0x54 }; // For regular LCDs
   const byte row_offsetsLarge[] = { 0x00, 0x40, 0x10, 0x50 }; // For 16x4 LCDs
   
   if ( row >= _numlines ) 
   {
      row = _numlines-1;    // rows start at 0
   }
   
   // 16x4 LCDs have special memory map layout
   // ----------------------------------------
   if ( _cols == 16 && _numlines == 4 )
   {
      command(LCD_SETDDRAMADDR | (col + row_offsetsLarge[row]));
   }
   else 
   {
      command(LCD_SETDDRAMADDR | (col + row_offsetsDef[row]));
   }
   
}

// Turn the display on/off
void noDisplay() 
{
   _displaycontrol &= ~LCD_DISPLAYON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void display() 
{
   _displaycontrol |= LCD_DISPLAYON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void noCursor() 
{
   _displaycontrol &= ~LCD_CURSORON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void cursor() 
{
   _displaycontrol |= LCD_CURSORON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns on/off the blinking cursor
void noBlink() 
{
   _displaycontrol &= ~LCD_BLINKON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void blink() 
{
   _displaycontrol |= LCD_BLINKON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) 
{
   command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight(void) 
{
   command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void) 
{
   _displaymode |= LCD_ENTRYLEFT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void) 
{
   _displaymode &= ~LCD_ENTRYLEFT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This method moves the cursor one space to the right
void moveCursorRight(void)
{
   command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
}

// This method moves the cursor one space to the left
void moveCursorLeft(void)
{
   command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
}


// This will 'right justify' text from the cursor
void autoscroll(void) 
{
   _displaymode |= LCD_ENTRYSHIFTINCREMENT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) 
{
   _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
   command(LCD_ENTRYMODESET | _displaymode);
}

// PUBLIC METHODS
// ---------------------------------------------------------------------------
// When the display powers up, it is configured as follows:
// 0. LCD starts in 8 bit mode
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a application starts (and the
// LiquidCrystal constructor is called).
// A call to begin() will reinitialize the LCD.
//
void begin(uint8_t cols, uint8_t lines, uint8_t dotsize) 
{
   init();
   if (lines > 1) 
   {
      _displayfunction |= LCD_2LINE;
   }
   _numlines = lines;
   _cols = cols;
   
   // for some 1 line displays you can select a 10 pixel high font
   // ------------------------------------------------------------
   if ((dotsize != LCD_5x8DOTS) && (lines == 1)) 
   {
      _displayfunction |= LCD_5x10DOTS;
   }
   
   // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
   // according to datasheet, we need at least 40ms after power rises above 2.7V
   // before sending commands. Arduino can turn on way before 4.5V so we'll wait 
   // 50
   // ---------------------------------------------------------------------------
   delay (100); // 100ms delay
   
   //put the LCD into 4 bit or 8 bit mode
   // -------------------------------------
   if (! (_displayfunction & LCD_8BITMODE)) 
   {
      // this is according to the hitachi HD44780 datasheet
      // figure 24, pg 46
      
      // we start in 8bit mode, try to set 4 bit mode
      // Special case of "Function Set"
      send(0x03, FOUR_BITS);
      delayMicroseconds(4500); // wait min 4.1ms
      
      // second try
      send ( 0x03, FOUR_BITS );
      delayMicroseconds(150); // wait min 100us
      
      // third go!
      send( 0x03, FOUR_BITS );
      delayMicroseconds(150); // wait min of 100us
      
      // finally, set to 4-bit interface
      send ( 0x02, FOUR_BITS );
      delayMicroseconds(150); // wait min of 100us

   } 
   else 
   {
      // this is according to the hitachi HD44780 datasheet
      // page 45 figure 23
      
      // Send function set command sequence
      command(LCD_FUNCTIONSET | _displayfunction);
      delayMicroseconds(4500);  // wait more than 4.1ms
      
      // second try
      command(LCD_FUNCTIONSET | _displayfunction);
      delayMicroseconds(150);
      
      // third go
      command(LCD_FUNCTIONSET | _displayfunction);
      delayMicroseconds(150);

   }
   
   // finally, set # lines, font size, etc.
   command(LCD_FUNCTIONSET | _displayfunction);
   delayMicroseconds ( 60 );  // wait more
   
   // turn the display on with no cursor or blinking default
   _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
   display();
   
   // clear the LCD
   clear();
   
   // Initialize to default text direction (for romance languages)
   _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
   // set the entry mode
   command(LCD_ENTRYMODESET | _displaymode);

   backlight();

}

void initLCD(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlighPin, t_backlighPol pol = POSITIVE )
{
   config(lcd_Addr, En, Rw, Rs, d4, d5, d6, d7);
   setBacklightPin(backlighPin, pol);
}

void setBacklightPin ( uint8_t value, t_backlighPol pol = POSITIVE )
{
   _backlightPinMask = ( 1 << value );
   _polarity = pol;
   setBacklight(BACKLIGHT_OFF);
}

void setBacklight( uint8_t value ) 
{
   // Check if backlight is available
   // ----------------------------------------------------
   if ( _backlightPinMask != 0x0 )
   {
      // Check for polarity to configure mask accordingly
      // ----------------------------------------------------------
      if  (((_polarity == POSITIVE) && (value > 0)) || 
           ((_polarity == NEGATIVE ) && ( value == 0 )))
      {
         _backlightStsMask = _backlightPinMask & LCD_BACKLIGHT;
      }
      else 
      {
         _backlightStsMask = _backlightPinMask & LCD_NOBACKLIGHT;
      }
      _i2cio.write( _backlightStsMask );
   }
}

int init()
{
   int status = 0;
   
   // initialize the backpack IO expander
   // and display functions.
   // ------------------------------------------------------------------------
   if ( _i2cio.begin ( _Addr ) == 1 )
   {
      _i2cio.portMode ( OUTPUT );  // Set the entire IO extender to OUTPUT
      _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
      status = 1;
      _i2cio.write(0);  // Set the entire port to LOW
   }
   return ( status );
}

void config (uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                                uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
{
   _Addr = lcd_Addr;
   
   _backlightPinMask = 0;
   _backlightStsMask = LCD_NOBACKLIGHT;
   _polarity = POSITIVE;
   
   _En = ( 1 << En );
   _Rw = ( 1 << Rw );
   _Rs = ( 1 << Rs );
   
   // Initialise pin mapping
   _data_pins[0] = ( 1 << d4 );
   _data_pins[1] = ( 1 << d5 );
   _data_pins[2] = ( 1 << d6 );
   _data_pins[3] = ( 1 << d7 );   
}

void send(uint8_t value, uint8_t mode) 
{
   // No need to use the delay routines since the time taken to write takes
   // longer that what is needed both for toggling and enable pin an to execute
   // the command.
   
   if ( mode == FOUR_BITS )
   {
      write4bits( (value & 0x0F), COMMAND );
   }
   else 
   {
      write4bits( (value >> 4), mode );
      write4bits( (value & 0x0F), mode);
   }
}

void write4bits ( uint8_t value, uint8_t mode ) 
{
   uint8_t pinMapValue = 0;
   
   // Map the value to LCD pin mapping
   // --------------------------------
   for ( uint8_t i = 0; i < 4; i++ )
   {
      if ( ( value & 0x1 ) == 1 )
      {
         pinMapValue |= _data_pins[i];
      }
      value = ( value >> 1 );
   }
   
   // Is it a command or data
   // -----------------------
   if ( mode == LCD_DATA )
   {
      mode = _Rs;
   }
   
   pinMapValue |= mode | _backlightStsMask;
   pulseEnable ( pinMapValue );
}

void pulseEnable (uint8_t data)
{
   write (data | _En);   // En HIGH
   write (data & ~_En);  // En LOW
}

int write ( uint8_t value )
{
   int status = 0;

   if ( is_initialized())
   {
      // Only write HIGH the values of the ports that have been initialised as
      // outputs updating the output shadow of the device
      _shadow = ( value & ~(_dirMask) );
	  //TODO: Hier die richtigen I2C-Funktionen einsetzen und nicht mehr die Wire-Librabry des Arduino
      Wire.beginTransmission ( _i2cAddr );
      Wire.send ( _shadow );

      status = Wire.endTransmission ();
   }
   return ( (status == 0) );	//TODO: Statusüberprüfung an USB-ITS-Gerät anpassen
}