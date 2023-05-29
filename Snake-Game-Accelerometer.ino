#include <M5StickCPlus.h>
#include <vector>
#include <utility> 
#include <EEPROM.h>
#include <Ticker.h>
#include "Gfx.hpp"
#include "Snake.hpp"
#include "Food.hpp"


// variables used

// score variable
int score;
// used for using the accelerometer sensor
MPU6886 IMU; // create an instance of the MPU6886 class 
Gfx gfx(WIDTH, HEIGHT, GRID_SIZE); // create an instance of graphic class
int snakeSpeed; // storages the value of speed depending on level dificulty
Ticker timer; // timer
int maxScore = EEPROM.read(address); // value of maxScore from eeprom 
//  flag used for showing that system is in demo mode
int flag = 0;

void playTone(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  noTone(buzzerPin);
}


// function for game logic
void gameLoop() {
  // variables for storage the output of sensor
  float ax = 0, ay = 0, az = 0;
  score = 0;
  Snake snake(STARTX, STARTY, gfx);
  Food food(snake);
  bool gameOver = false;
  gfx.drawBackgroundMap();
  // game play
  while(!gameOver) {
    
    // getting data from sensor
    M5.IMU.getAccelData(&ax,&ay,&az);
    // compare data from sensor to a user defined thrashold 
    if(ax*10 > THRESHOLD && snake.getSnakeDirection() != 0) { // if direction is already opposite the command has no effect
      snake.setSnakeDirection(2); // left
      Serial.write("2");
    } else if(ax*10 < -THRESHOLD && snake.getSnakeDirection() != 2) {
      snake.setSnakeDirection(0); // right
      Serial.write("0");
    }
    else if(ay*10 > THRESHOLD && snake.getSnakeDirection() != 3) {
      snake.setSnakeDirection(1); // down
      Serial.write("1");
    } else if(ay*10 < -THRESHOLD && snake.getSnakeDirection() != 1){
      snake.setSnakeDirection(3); // up
      Serial.write("3");
    }
    snake.moveSnake();
    
    //checking collision with food
    if(snake.checkSnakeCollision()) {
      gameOver = true;
            
      maxScore = EEPROM.read(address);
      //Serial.print(maxScore);
      if(score > maxScore) {
        EEPROM.write(address, score);
        EEPROM.commit();
        maxScore = score;
        //Serial.print(maxScore);
      }
      gfx.printScore(score, EEPROM.read(address));
     
    } else if(snake.body[0].first == food.get_x_food() && snake.body[0].second == food.get_y_food()){
      food = Food(snake);
      score++;
      playTone(NOTE_C5, TONE_DURATION);
      snake.growSnake();
      if(score == 5) {
        gfx.setLevel(1);
        snake.setGfx(gfx);
        gfx.drawBackgroundMap();
      } else if(score == 10) {
        gfx.setLevel(2);
        snake.setGfx(gfx);
        gfx.drawBackgroundMap();
      }
      
    }
    
    snake.drawSnake();
    food.drawFood();
    
    // increasing dificulty by increasing the speed of snake
    if(score >= 0 && score < 5) {
      food.setLevel(0);
      gfx.setLevel(0);
      snake.setGfx(gfx);
      snakeSpeed = SNAKE_SPEED_EASY;
    } else if(score >= 5 && score < 10){
      gfx.setLevel(1);
      snake.setGfx(gfx);
      food.setLevel(1);
      snakeSpeed = SNAKE_SPEED_MEDIUM;
    } else {
      gfx.setLevel(2);
      snake.setGfx(gfx);
      food.setLevel(2);
      snakeSpeed = SNAKE_SPEED_HARD;
    }
    //Serial.print(snakeSpeed);
    // speed of snake
    delay(snakeSpeed); 
  }
  
}

void resetInterruptHandler() {
  resetTimer();
}

//  reset timer
void resetTimer() {
  timer.detach();
  timer.attach(15.0, demoGame);
}

void demoGame() {
  timer.detach();
  flag = 1;
    // variables for storage the output of sensor
  float ax = 0, ay = 0, az = 0;
  score = 0;
  Snake snake(STARTX, STARTY, gfx);
  Food food(snake);
  gfx.drawBackgroundMap();

  bool gameOver = false;
  // game play
  while(!gameOver) {
    
    // getting data from sensor
    //M5.IMU.getAccelData(&ax,&ay,&az);
    ax = random(-5000,5000) / 10000.0;
    ay = random(-5000,5000) / 10000.0;
    Serial.println(ax);
    Serial.println(ay);
    delay(500);
    // compare data from sensor to a user defined thrashold 
    if(ax*10 > THRESHOLD2 && snake.getSnakeDirection() != 0) { // if direction is already opposite the command has no effect
      snake.setSnakeDirection(2); // left
    } else if(ax*10 < -THRESHOLD2 && snake.getSnakeDirection() != 2) {
      snake.setSnakeDirection(0); // right
    }
    else if(ay*10 > THRESHOLD2 && snake.getSnakeDirection() != 3) {
      snake.setSnakeDirection(1); // down
    } else if(ay*10 < -THRESHOLD2 && snake.getSnakeDirection() != 1){
      snake.setSnakeDirection(3); // up
    }
    snake.moveSnake();
    
    //checking collision with food
    if(snake.checkSnakeCollision()) {
      gameOver = true;
      gfx.drawBackgroundMap();
      
      maxScore = EEPROM.read(address);
      //Serial.print(maxScore);
      if(score > maxScore) {
        EEPROM.write(address, score);
        EEPROM.commit();
        maxScore = score;
        //Serial.print(maxScore);
      }
      gfx.printScore(score, EEPROM.read(address));
     
    } else if(snake.body[0].first == food.get_x_food() && snake.body[0].second == food.get_y_food()){
      food = Food(snake);
      
      score++;
      playTone(NOTE_C5, TONE_DURATION);
      snake.growSnake();
    }
    
    snake.drawSnake();
    food.drawFood();
    
    // increasing dificulty by increasing the speed of snake
    if(score >= 0 && score < 5) {
      gfx.setLevel(0);
      snake.setGfx(gfx);
      snakeSpeed = SNAKE_SPEED_EASY;
    } else if(score >= 2 && score < 10){
      gfx.setLevel(1);
      snake.setGfx(gfx);
      snakeSpeed = SNAKE_SPEED_MEDIUM;
    } else {
      gfx.setLevel(2);
      snake.setGfx(gfx);
      food.setLevel(2);
      snakeSpeed = SNAKE_SPEED_HARD;
    }
    //Serial.print(snakeSpeed);
    // speed of snake
    delay(snakeSpeed); 
  }
  delay(3000);
  timer.attach(15.0, demoGame);
  flag = 0;
  gfx.drawMenu();
}
// when game starts timer is disabled
void disableTimer() {
  timer.detach();
}


void setup() {
  M5.begin(); 
  pinMode(buzzerPin, OUTPUT);
  gfx.drawMenu();
  randomSeed(analogRead(0));
  attachInterrupt(M5_BUTTON_RST, resetInterruptHandler, FALLING);
  timer.attach(15.0, demoGame); // restart timer when M5_BUTTON_RST is pressed
  attachInterrupt(M5_BUTTON_HOME, disableTimer, FALLING);
  
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);
  Wire.begin();
  M5.IMU.Init();
  Serial.begin(baud_rate); 
  M5.Lcd.setRotation(0);  
  EEPROM.begin(EEPROM_SIZE);
}

void loop(){
  
  if(digitalRead(M5_BUTTON_RST) == LOW) {
    gfx.drawInstructions();
    delay(instructions_delay);
    gfx.drawMenu();
  } else if(digitalRead(M5_BUTTON_HOME) == LOW){
    if(flag == 0) {
      gameLoop();
      delay(menu_delay);
      gfx.drawMenu();
      timer.attach(15.0, demoGame);
    }
  } else {
    while(digitalRead(M5_BUTTON_RST) == HIGH && digitalRead(M5_BUTTON_HOME) == HIGH) {
      
      delay(basic_delay);
    }
  }
  delay(basic_delay);
}
