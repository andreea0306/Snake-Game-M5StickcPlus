#include <M5StickCPlus.h>
#include <vector>
#include "Defines.h"
//#include "Snake.hpp"

class Food{
private:
  int x, y;
  int level;
  std::vector<std::pair<int,int>> body;
  Snake snk;
public:
  Food(Snake s) {
    snk = s;
    //level = 0;
    x = (rand()%(WIDTH/GRID_SIZE))*GRID_SIZE;
    y = (rand()%(HEIGHT/GRID_SIZE))*GRID_SIZE;
    for(int i=0; i<=snk.body.size();i++) {
      if(snk.body[i].first == x || snk.body[i].second == y) {
        x = (random()%(WIDTH/GRID_SIZE))*GRID_SIZE;
        y = (random()%(HEIGHT/GRID_SIZE))*GRID_SIZE;
      }
    }
  }

  Food(int X, int Y) {
    x = X;
    y = Y;
  }

  void drawFood(){
    switch(level) {
      case 0:
        M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, RED);
        break;
      case 1:
        M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, BLUE);
        break;
      case 2:
        M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, GREEN);
        break;
      
    }
    
  }

  int get_x_food(){
    return x;
  }

  int get_y_food() {
    return y;
  }

  void setLevel(int lvl) {
    level = lvl;
  }

};
