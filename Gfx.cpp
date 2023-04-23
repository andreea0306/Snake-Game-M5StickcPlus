#include <M5StickCPlus.h>
#include "Defines.h"
#pragma once
class Gfx {
private: 
  int width;
  int height;
  int grid_size;
public:
  Gfx() {
  }
  Gfx(int w, int h, int gs) {
    width = w;
    height = h;
    grid_size = gs;
  }
  
  void drawBackgroundMap() {
    for(int i=0;i<width;i = i+grid_size) {
      for(int j=0;j<height;j = j+grid_size) {
        if((i+j)%2==0) {
          M5.Lcd.fillRect(i,j, grid_size, grid_size, C0);
        } else {
          M5.Lcd.fillRect(i,j, grid_size, grid_size, C1);
        }
      }
    }
  }
  
	void drawMenu() {
	  
	  //M5.Lcd.fillScreen(BLACK);
	  drawBackgroundMap();
	  M5.Lcd.setCursor(10, 60);
	  M5.Lcd.setTextSize(2);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("SNAKE GAME");
	  //M5.Lcd.setCursor(14, 60);
	  M5.Lcd.fillRect(30, 100, 50, 15, BLACK);
	  M5.Lcd.fillRect(70, 100, 15, 50, BLACK);
	  M5.Lcd.fillRoundRect(70, 170, 15, 15, 2, RED);
	  M5.Lcd.setCursor(20, 210);
	  M5.Lcd.setTextSize(1);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("Press menu button ");
	  M5.Lcd.setCursor(40, 220);
	  M5.Lcd.setTextSize(1);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("to start...");
	}

	void drawInstructions() {
	  //M5.Lcd.fillScreen(BLACK);
	  drawBackgroundMap();
	  M5.Lcd.setCursor(1,50);
	  M5.Lcd.setTextSize(1);
	  M5.Lcd.setTextColor(BLACK);
	  M5.Lcd.println("Instructions:\n\n");
	  M5.Lcd.println("Use the accelerometer to control the snake.\n");
	  M5.Lcd.println("Collect food to grow");
	  M5.Lcd.println("your snake and ");
	  M5.Lcd.println("increase your score.\n");
	  M5.Lcd.println("Avoid colliding with ");
	  M5.Lcd.println("the walls or your own body.");
	  
	  // getting the battery percentage of the device
	  int battery = M5.Axp.GetBatVoltage();
  	  float percentage = ((float)battery - 3.3) / 0.9 * 100;
  	  M5.Lcd.println(" ");
  	  M5.Lcd.printf("Battery percentage is around:%.2f%%\n",percentage);
	  //M5.Lcd.print(percentage);	  
	}
};
