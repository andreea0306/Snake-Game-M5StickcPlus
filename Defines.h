#include <M5StickCPlus.h>
#pragma once
#define HEIGHT 234
#define WIDTH 135
#define GRID_SIZE 9
#define THRESHOLD 5
#define THRESHOLD2 3
#define SNAKE_SPEED_EASY 500
#define SNAKE_SPEED_MEDIUM 300
#define SNAKE_SPEED_HARD 100
#define STARTX 18
#define STARTY 18
#define NOTE_C5  523
#define TONE_DURATION 1000

#define basic_delay 50
#define instructions_delay 8000 // 8 s for reading the instructions
#define menu_delay 2000
#define baud_rate 9600
#define EEPROM_SIZE 1

#define buzzerPin 10
#define address 0 // address where maxScore will be stored

#define base_cursorX 0
#define base_cursorY 0

#ifndef _DARK_GREEN
// light green
const unsigned short DARK_GREEN  = M5.Lcd.color565(10, 220,10);
#endif

#ifndef _LIGHT_GREEN
// dark green
const unsigned short LIGHT_GREEN = M5.Lcd.color565(40, 200, 60);
#endif

#ifndef _LIGHT_ORANGE
// light orange
const unsigned short LIGHT_ORANGE = M5.Lcd.color565(200, 120, 0);
#endif

#ifndef _DARK_ORANGE
// dark orange
const unsigned short DARK_ORANGE = M5.Lcd.color565(255, 200, 60);
#endif

#ifndef _LIGHT_RED
// light red
const unsigned short LIGHT_RED = M5.Lcd.color565(255, 0, 0);
#endif

#ifndef _DARK_RED
// dark red
const unsigned short DARK_RED = M5.Lcd.color565(255, 60, 30);
#endif

#ifndef _RED_APPLE
const unsigned short RED_APPLE = M5.Lcd.color565(255, 0, 0);
#endif

#ifndef _GREEN_APPLE
const unsigned short GREEN_APPLE = M5.Lcd.color565(0, 255, 0);
#endif

// map codes
// easy game
#define C0 LIGHT_GREEN
#define C1 DARK_GREEN
// medium game
#define C2 LIGHT_ORANGE
#define C3 DARK_ORANGE
// hard game
#define C4 LIGHT_RED
#define C5 DARK_RED
