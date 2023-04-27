#include <M5StickCPlus.h>
#include <vector>
#include <utility> 
#include <EEPROM.h>

#include "Gfx.cpp"
#include "Food.cpp"
#include "Snake.cpp"

// variables used

// score variable
int score;
// used for using the accelerometer sensor
MPU6886 IMU; // create an instance of the MPU6886 class 
Gfx gfx(WIDTH, HEIGHT, GRID_SIZE); // create an instance of graphic class
int snakeSpeed; // storages the value of speed depending on level dificulty

int address = 0; // address where maxScore will be stored
int maxScore = EEPROM.read(address); // value of maxScore from eeprom 

// function for game logic
void gameLoop() {
  // variables for storage the output of sensor
  float ax = 0, ay = 0, az = 0;
  score = 0;
  Food food;
  Snake snake(18, 18, gfx);
  gfx.drawBackgroundMap();

  bool gameOver = false;
  // game play
  while(!gameOver) {
    // getting data from sensor
    M5.IMU.getAccelData(&ax,&ay,&az);
    // compare data from sensor to a user defined thrashold 
    if(ax*10 > THRESHOLD) {
      snake.setSnakeDirection(2);
    } else if(ax*10 < -THRESHOLD) {
      snake.setSnakeDirection(0);
    }
    else if(ay*10 > THRESHOLD) {
      snake.setSnakeDirection(1);
    } else if(ay*10 < -THRESHOLD){
      snake.setSnakeDirection(3);
    }
    snake.moveSnake();
    
    //checking collision with food
    if(snake.checkSnakeCollision()) {
      gameOver = true;
      M5.Lcd.setCursor(10, 60);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.print("Score: ");
      M5.Lcd.println(score);
      maxScore = EEPROM.read(address);
      //Serial.print(maxScore);
      if(score > maxScore) {
        EEPROM.write(address, score);
        EEPROM.commit();
        maxScore = score;
        Serial.print(maxScore);
      }
      M5.Lcd.print("Max score:");
      M5.Lcd.print(EEPROM.read(address));
      
    } else if(snake.body[0].first == food.get_x_food() && snake.body[0].second == food.get_y_food()){
      food = Food();
      score++;

      snake.growSnake();
    }
    
    snake.drawSnake();
    food.drawFood();
    
    // increasing dificulty by increasing the speed of snake
    if(score >= 0 && score < 5) {
      snakeSpeed = SNAKE_SPEED_EASY;
    } else if(score >= 5 && score < 10){
      snakeSpeed = SNAKE_SPEED_MEDIUM;
    } else {
      snakeSpeed = SNAKE_SPEED_HARD;
    }
    //Serial.print(snakeSpeed);
    // speed of snake
    delay(snakeSpeed); 
  }
  
}

void setup() {
  M5.begin();
  gfx.drawMenu();
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);
  Wire.begin();
  M5.IMU.Init();
  Serial.begin(9600); 
  M5.Lcd.setRotation(0);  
  EEPROM.begin(1000);
}

void loop(){
 
  if(digitalRead(M5_BUTTON_RST) == LOW) {
    gfx.drawInstructions();
    delay(8000);
    gfx.drawMenu();
  } else if(digitalRead(M5_BUTTON_HOME) == LOW){
    gameLoop();
    delay(2000);
    gfx.drawMenu();
  } else {
    while(digitalRead(M5_BUTTON_RST) == HIGH && digitalRead(M5_BUTTON_HOME) == HIGH) {
      delay(50);
    }
  }
  delay(50);
}
