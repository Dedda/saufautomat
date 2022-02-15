#ifndef PINS
#define PINS

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
// Pins for Arduino Mega

// LCD
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 4
#define LCD_D7 5
#define LCD_EN 6
#define LCD_RS 7

// Beverages
#define BTN_BEER 16
#define BTN_SHOTS 15
#define BTN_LONGDRINK 8
#define BTN_COCKTAIL 17
#define BTN_NON_ALCOHOL 14

#define BTN_RESET 49

#define SD_DATA 53

#define BTN_TERMINAL 10

#else
// Pins for other Arduino Boards (only semi verified for UNO and no others)

// LCD
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 4
#define LCD_D7 5
#define LCD_EN 6
#define LCD_RS 7

// Beverages
#define BTN_BEER 14
#define BTN_SHOTS 15
#define BTN_LONGDRINK 16
#define BTN_COCKTAIL 17
#define BTN_NON_ALCOHOL 18

#define BTN_RESET 19

#define SD_DATA 10

#define BTN_TERMINAL 8

#endif

#endif