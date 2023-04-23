#include <M5StickCPlus.h>
#pragma once
#define HEIGHT 234
#define WIDTH 135
#define GRID_SIZE 9
#define THRESHOLD 5
#define SNAKE_SPEED_EASY 500
#define SNAKE_SPEED_MEDIUM 300
#define SNAKE_SPEED_HARD 100

#ifndef _DARK_GREEN
// light green
const unsigned short DARK_GREEN  = M5.Lcd.color565(10, 220,10);
#endif

#ifndef _LIGHT_GREEN
// dark green
const unsigned short LIGHT_GREEN = M5.Lcd.color565(40, 200, 60);
#endif
// map codes
#define C0 LIGHT_GREEN
#define C1 DARK_GREEN
