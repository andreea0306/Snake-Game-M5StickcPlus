#include <M5StickCPlus.h>
#include "Defines.h"

class Food{
private:
  int x, y;
  int level;
public:
  Food() {
    x = (rand()%(WIDTH/GRID_SIZE))*GRID_SIZE;
    y = (rand()%(HEIGHT/GRID_SIZE))*GRID_SIZE;
  }

  void drawFood(){
    switch(level) {
      case 2:
        M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, GREEN_APPLE);
        break;
      default :
        M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, RED_APPLE);
    }
    
  }

  int get_x_food(){
    return x;
  }

  int get_y_food() {
    return y;
  }

  void setLevel(int lvl) {
    this->level = lvl;
  }
};
