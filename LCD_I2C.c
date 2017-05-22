//Previously private variables, now created as static variables
//IMPORTANT: This means that the library can only control ONE display at any given time
static uint8_t _Addr;             // I2C Address of the IO expander
static uint8_t _backlightPinMask; // Backlight IO pin mask
static uint8_t _backlightStsMask; // Backlight status mask
static uint8_t _En;               // LCD expander word for enable pin
static uint8_t _Rw;               // LCD expander word for R/W pin
static uint8_t _Rs;               // LCD expander word for Register Select pin
static uint8_t _data_pins[4];     // LCD data lines