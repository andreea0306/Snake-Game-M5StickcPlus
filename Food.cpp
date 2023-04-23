#include <M5StickCPlus.h>
#include "Defines.h"

class Food{
private:
  int x, y;
public:
  Food() {
    x = (rand()%(WIDTH/GRID_SIZE))*GRID_SIZE;
    y = (rand()%(HEIGHT/GRID_SIZE))*GRID_SIZE;
  }

  void drawFood(){
    M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, RED);
  }

  int get_x_food(){
    return x;
  }

  int get_y_food() {
    return y;
  }
};
