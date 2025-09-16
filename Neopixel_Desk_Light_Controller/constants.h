#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Adafruit_NeoPixel.h>
#include <arduino.h>

#define MULTI_THREADED false

#define pot1 3
#define pot2 4
#define button1 20
#define extDet 21
#define NEOPIXEL_PIN 6

#define MAX_POT_VALUE 4095
#define DEBOUNCE_TIME 10
#define BAUD_RATE 115200
#define MAX_NUM_PARAM 7
#define STACK_SIZE 1024

#define EEPROM_SIZE 16
#define DEFAULT_NUM_PIXELS 10
#define MAX_NUM_PIXELS 300
#define DEFAULT_BRIGHTNESS 10
#define DEFAULT_STARTING_MODE 1
#define DEFAULT_COLOR_1 Adafruit_NeoPixel::Color(255,255,255);
#define DEFAULT_COLOR_2 Adafruit_NeoPixel::Color(0,255,128);
#define DEFAULT_NUM_ON 1
#define DEFAULT_NUM_OFF 2

#define WELCOME_MESSAGE "*******************************************\r\n* Neopixel Desk Light Controller          *\r\n* Version 1.0                             *\r\n* NF 2025                                 *\r\n*******************************************\r\nType \"Help\" to see all available commands\r\n"
#define ERROR_TOO_MANY_PARAMETERS "\a\x1b[31mToo Many Parameters entered! Type 'Help' to see command syntax\x1b[0m"
#define ERROR_TOO_FEW_PARAMETERS "\a\x1b[31mToo Few Parameters entered! Type 'Help' to see command syntax\x1b[0m"
#define ERROR_UNRECOGNIZED_COMMAND "\a\x1b[31mUnrecognized command! Please check your spelling, or type 'Help' to see all commands\x1b[39m"
#define HELP_LIST "\x1b[35mAvailable commands:\r\n\x1b[36mHelp\x1b[39m\r\n - Used to list all available commands\r\n\x1b[36mHelp modes\x1b[39m\r\n - Print all mode ID numbers and descriptions\r\n\x1b[36mSet color \x1b[32m[Red Value 0-255] [Green Value 0-255] [Blue Value 0-255]\x1b[39m\r\n - Sets the pixels to a given color\r\n\x1b[36mSet number pixels \x1b[32m[Number]\x1b[39m\r\n - Changes amount of pixels controlled (can slow down animations)\r\n\x1b[36mSet number \x1b[32m['on' or 'off'] [Number]\x1b[39m\r\n - Changes amount of pixels on (main color) or off (alt color) during the solid color chase animations\r\n\x1b[36mSet saved color \x1b[32m['main' or 'alt'] [Red Value 0-255] [Green Value 0-255] [Blue Value 0-255]\x1b[39m\r\n - Sets the solid color saved in memory\r\n\x1b[36mSet mode \x1b[32m[Mode #]\x1b[39m\r\n - Sets the current mode\r\n\x1b[36mSet start mode \x1b[32m[Mode #]\x1b[39m\r\n - Sets the mode to default to on startup\r\n\x1b[36mSet brightness \x1b[32m[Brightness value 1-255]\x1b[39m\r\n - Sets the brightness of the strip"
#define ERROR_NUM_PIXELS "\a\x1b[31mInvalid Number of Pixels!\x1b[39m\r\n Please Enter a number between 1 and " + String(MAX_NUM_PIXELS)
#define ERROR_INVALID_MODE(MIN, MAX) "\a\x1b[31mInvalid mode!\x1b[39m Please enter a number between " + String(MIN) + " and " + String(MAX)
#define ERROR_INVALID_BRIGHTNESS "\a\x1b[31mPlease Enter a non-zero brigtness value!\x1b[39m"
#define MODES_INTRODUCTION "\x1b[36;2mAVAILABLE MODES\x1b[0m\r\n\x1b[33;24m[#]\tName\x1b[0m\r\n"
#define SPECIFY_MAIN_OR_ALT "\a\x1b[31mPlease Specify Main or Alt\x1b[0m"
#define SPECIFY_ON_OR_OFF "\a\x1b[31mPlease Specify On or Off\x1b[0m"
#define CURSOR ">> "

#endif