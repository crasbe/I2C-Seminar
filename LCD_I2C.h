//YWROBOT
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h


// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

extern void initDisp(char lcd_Addr, char lcd_cols, char lcd_rows);
extern void begin(char cols, char rows, char charsize);
extern void clear();
extern void home();
extern void noDisplay();
extern void display();
extern void noBlink();
extern void blink();
extern void noCursor();
extern void cursor();
extern void scrollDisplayLeft();
extern void scrollDisplayRight();
extern void printLeft();
extern void printRight();
extern void leftToRight();
extern void rightToLeft();
extern void shiftIncrement();
extern void shiftDecrement();
extern void noBacklight();
extern void backlight();
extern void autoscroll();
extern void noAutoscroll();
extern void createChar(char, char[]);
extern void setCursor(char, char);
extern void writeChar(char);  //TODO: Nutzung prüfen und evtl. ersetzen/entfernen (im Original virtual void oder virtual size_t)
extern void command(char);
extern void init();

////compatibility API function aliases
extern void blink_on();						// alias for blink()
extern void blink_off();       					// alias for noBlink()
extern void cursor_on();      	 					// alias for cursor()
extern void cursor_off();      					// alias for noCursor()
extern void setBacklight(char new_val);				// alias for backlight() and nobacklight()
extern void load_custom_character(char char_num, char *rows);	// alias for createChar()
extern void printstr(char*, unsigned int len);

//Private functions
extern void init_priv();
extern void sendDisp(char, char);
extern void write4bits(char);
extern void expanderWrite(char);
extern void pulseEnable(char);
extern char _Addr;
extern char _displayfunction;
extern char _displaycontrol;
extern char _displaymode;
extern char _numlines;
extern char _cols;
extern char _rows;
extern char _backlightval;
#endif
