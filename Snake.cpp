#include <vector>
#include <utility> 
#include <M5StickCPlus.h>
#include "Defines.h"
#include "Gfx.cpp"
class Snake{
private :
  int direction;
  Gfx gfx;
public:
  
  std::vector<std::pair<int,int>> body;

  Snake(int x, int y, Gfx graphics) {
    direction = 0;
    body.push_back(std::make_pair(x,y)); // add the head of the snake
    body.push_back(std::make_pair(x-GRID_SIZE,y));
    body.push_back(std::make_pair(x-2*GRID_SIZE,y));
    gfx = graphics;
  }

  void drawSnake() {
    for(int i=0; i<body.size();i++) {
      M5.Lcd.fillRect(body[i].first, body[i].second, GRID_SIZE, GRID_SIZE, BLACK);
    }
  }

  void moveSnake() {
    std::pair<int,int> head = std::make_pair(body[0].first, body[0].second);
    //std::pair<int,int> tail = std::make_pair(getSnakeTail().first, getSnakeTail().second);
    // right
    if(direction == 0) {
      head.first += GRID_SIZE;
    } else if(direction == 1) { // down
      head.second += GRID_SIZE;
    } else if(direction == 2) { // left
      head.first -= GRID_SIZE;
    } else if(direction == 3) { // up
      head.second -= GRID_SIZE;
    }
    body.insert(body.begin(), head);
    body.pop_back();
    gfx.drawBackgroundMap();
    
  }

  void setSnakeDirection(int newDirection) {
    direction = newDirection;
  }

  bool checkSnakeCollision() {
    std::pair<int,int> head = std::make_pair(body[0].first, body[0].second);
    // checking collison with walls
    if(head.first < 0 || head.first >= WIDTH || head.second < 0 || head.second >= HEIGHT){
      return true;
    }
    // checking collision with body
    for(int i=1; i<body.size(); i++) {
      if(head.first == body[i].first && head.second == body[i].second) {
        return true;
      }
    }
    return false;
  }
  std::pair<int,int> getSnakeTail() {
    return body[body.size()-1];
  }
  void growSnake() {
    body.push_back(getSnakeTail());
  }
  
  void setGfx(Gfx graphics) {
  	gfx = graphics;
  }
};
