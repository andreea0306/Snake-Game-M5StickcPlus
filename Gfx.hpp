#include <M5StickCPlus.h>
#include "Defines.h"
#pragma once
class Gfx {
private: 
  int width;
  int height;
  int grid_size;
  short int level;
public:
  Gfx() {
  }
  Gfx(int w, int h, int gs) {
    width = w;
    height = h;
    grid_size = gs;
    level = 0;
  }
  
  void drawBackgroundMap() {
    for(int i=0;i<width;i = i+grid_size) {
      for(int j=0;j<height;j = j+grid_size) {
        if((i+j)%2==0) {
          switch(level){
            case 0:
              M5.Lcd.fillRect(i,j, grid_size, grid_size, C0);
              break;
            case 1:
              M5.Lcd.fillRect(i,j, grid_size, grid_size, C2);
              break;
            case 2:
              M5.Lcd.fillRect(i,j, grid_size, grid_size, C4);
              break;
          }
        } else {
          switch(level){
            case 0:
              M5.Lcd.fillRect(i,j, grid_size, grid_size, C1);
              break;
            case 1:
              M5.Lcd.fillRect(i,j, grid_size, grid_size, C3);
              break;
            case 2:
              M5.Lcd.fillRect(i,j, grid_size, grid_size, C5);
              break;              
          }
        }
      }
    }
  }

  void updateMap(int x, int y) {
    if((x+y)%2==0) {
          switch(level){
            case 0:
              M5.Lcd.fillRect(x,y, grid_size, grid_size, C0);
              break;
            case 1:
              M5.Lcd.fillRect(x,y, grid_size, grid_size, C2);
              break;
            case 2:
              M5.Lcd.fillRect(x,y, grid_size, grid_size, C4);
              break;
          }
        } else {
          switch(level){
            case 0:
              M5.Lcd.fillRect(x,y, grid_size, grid_size, C1);
              break;
            case 1:
              M5.Lcd.fillRect(x,y, grid_size, grid_size, C3);
              break;
            case 2:
              M5.Lcd.fillRect(x,y, grid_size, grid_size, C5);
              break;              
          }
        }
  }

  void setLevel(int lvl) {
    this->level = lvl;
  }
  
	void drawMenu() {
	  
    level = 0;
	  drawBackgroundMap();
	  M5.Lcd.setCursor(base_cursorX+10, base_cursorY+60);
	  M5.Lcd.setTextSize(2);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("SNAKE GAME");
    // x axys, y axis, width of rect, height rect, color
	  M5.Lcd.fillRect(base_cursorX+30, base_cursorY+100, 50, 15, BLACK);
	  M5.Lcd.fillRect(base_cursorX+70, base_cursorY+100, 15, 50, BLACK);
    // x axys, y axis, width of rect, height rect, corner radius, color
	  M5.Lcd.fillRoundRect(base_cursorX+70, base_cursorY+170, 15, 15, 2, RED);
	  M5.Lcd.setCursor(base_cursorX+20, base_cursorY+210);
	  M5.Lcd.setTextSize(1);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("Press menu button ");
	  M5.Lcd.setCursor(base_cursorX+40, base_cursorY+220);
	  M5.Lcd.setTextSize(1);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("to start...");
	}

	void drawInstructions() {
	  //M5.Lcd.fillScreen(BLACK);
    level = 0;
	  drawBackgroundMap();
	  M5.Lcd.setCursor(1,50);
	  M5.Lcd.setTextSize(1);
	  M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextWrap(true);
    M5.Lcd.println("Instructions:");
    M5.Lcd.print("Use the accelerometer to control the snake. Collect food to grow  your snake and increase your score. Avoid colliding with the walls or your own tail.");

    /*
	  // getting the battery percentage of the device
	  int battery = M5.Axp.GetBatVoltage();
  	  float percentage = ((float)battery - 3.3) / 0.9 * 100;
  	  M5.Lcd.println(" ");
  	  M5.Lcd.printf("Battery percentage is around:%.2f%%\n",percentage);
	  //M5.Lcd.print(percentage);	  
    */
	}

  void printScore(int score, int maxScore) {
    M5.Lcd.setCursor(10, 60);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.print("Score: ");
    M5.Lcd.println(score);
    M5.Lcd.println("Max score:");
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.print(maxScore);
  }
 
};
