#include <M5StickCPlus.h>
#include <vector>
#include <utility>
#include <EEPROM.h>
#include <Ticker.h>
#include <stdio.h>
#include "Gfx.hpp"
#include "Snake.hpp"
#include "Food.hpp"


int button_home = M5_BUTTON_HOME; // external interrupt  pin
int button_rst = M5_BUTTON_RST;

volatile  bool interrupt_home_process_status = {
  !triggered_home                                     // start with no switch press pending,  ie false (!triggered_home)
};
bool initialisation_complete =            false;  // inhibit any interrupts until initialisation is complete
bool initialisation_complete2 =            false;  // inhibit any interrupts until initialisation is complete

//
// ISR for  handling interrupt triggers arising from associated home button 
//
volatile bool startGame = false;
void  button_home_interrupt_handler()
{
  if (initialisation_complete == true)
  {
    if (interrupt_home_process_status == !triggered_home) {
      interrupt_home_process_status  = triggered_home;
      startGame = true;
    }
  }
}

volatile  bool interrupt_rst_process_status = {
  !triggered_rst                                   // start with no switch press pending,  ie false (!triggered_rst)
};

//
// ISR for  handling interrupt triggers arising from associated rst button 
//
void  button_rst_interrupt_handler()
{
  if (initialisation_complete == true)
  {
    if (interrupt_rst_process_status == !triggered_rst) {
      interrupt_rst_process_status  = triggered_rst;
      startGame = true;
    }
  }
}

// variables used

const int cwave[] {
  220, 110, 220, 110, 260, 110, 330, 110,
  220, 110, 220, 110, 260, 110, 330, 110,
  290, 150, 290, 150, 350, 150, 440, 150,
  290, 150, 290, 150, 350, 150, 440, 150,
  200, 100, 200, 100, 260, 100, 300, 100,
  200, 100, 200, 100, 260, 100, 300, 100,
  170,  90, 170,  90, 230,  90, 260,  90,
  170,  90, 170,  90, 230,  90, 260,  90,

  220, 110, 220, 110, 260, 110, 330, 110,
  220, 110, 220, 110, 260, 110, 330, 110,
  290, 150, 290, 150, 330, 150, 440, 150,
  290, 150, 290, 150, 330, 150, 440, 150,
  200, 100, 200, 100, 270, 100, 290, 100,
  200, 100, 200, 100, 260, 100, 290, 100,
  180,  90, 180,  90, 230,  90, 260, 90,
  180,  90, 180,  90, 230,  90, 260, 90,



  220, 110, 220, 110, 260, 110, 330, 110,
  220, 110, 220, 110, 260, 110, 330, 110,
  290, 150, 290, 150, 350, 150, 440, 150,
  290, 150, 290, 150, 350, 150, 440, 150,
  200, 100, 200, 100, 260, 100, 290, 100,
  200, 100, 200, 100, 260, 100, 290, 100,
  170,  90, 180,  90, 230,  90, 260, 90,
  170,  90, 180,  90, 230,  90, 260, 90,
};

// score variable
int score;
// used for using the accelerometer sensor
MPU6886 IMU; // create an instance of the MPU6886 class
Gfx gfx(WIDTH, HEIGHT, GRID_SIZE); // create an instance of graphic class
int snakeSpeed; // storages the value of speed depending on level dificulty

hw_timer_t *timer = NULL;
Ticker serialTimer;
int maxScore = EEPROM.read(address); // value of maxScore from eeprom
//  flag used for showing that system is in demo mode
int flag = 0;

void playTone(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
  noTone(BUZZER_PIN);
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
  while (!gameOver) {

    // getting data from sensor
    M5.IMU.getAccelData(&ax, &ay, &az);
    // compare data from sensor to a user defined thrashold
    if (ax * 10 > THRESHOLD && snake.getSnakeDirection() != 0) { // if direction is already opposite the command has no effect
      snake.setSnakeDirection(2); // left
    } else if (ax * 10 < -THRESHOLD && snake.getSnakeDirection() != 2) {
      snake.setSnakeDirection(0); // right
    }
    else if (ay * 10 > THRESHOLD && snake.getSnakeDirection() != 3) {
      snake.setSnakeDirection(1); // down
    } else if (ay * 10 < -THRESHOLD && snake.getSnakeDirection() != 1) {
      snake.setSnakeDirection(3); // up
    }
    snake.moveSnake();

    //checking collision with food
    if (snake.checkSnakeCollision()) {
      gameOver = true;

      maxScore = EEPROM.read(address);
      //Serial.print(maxScore);
      if (score > maxScore) {
        EEPROM.write(address, score);
        EEPROM.commit();
        maxScore = score;
        //Serial.print(maxScore);
      }
      gfx.printScore(score, EEPROM.read(address));

    } else if (snake.body[0].first == food.get_x_food() && snake.body[0].second == food.get_y_food()) {
      food = Food(snake);
      score++;
      playTone(NOTE_C5, TONE_DURATION);
      snake.growSnake();
      if (score == 5) {
        gfx.setLevel(1);
        snake.setGfx(gfx);
        gfx.drawBackgroundMap();
      } else if (score == 10) {
        gfx.setLevel(2);
        snake.setGfx(gfx);
        gfx.drawBackgroundMap();
      }

    }

    snake.drawSnake();
    food.drawFood();

    // increasing dificulty by increasing the speed of snake
    if (score >= 0 && score < 5) {
      food.setLevel(0);
      gfx.setLevel(0);
      snake.setGfx(gfx);
      snakeSpeed = SNAKE_SPEED_EASY;
    } else if (score >= 5 && score < 10) {
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

void demoGame() {
  //timer.detach();
  detachInterrupt(digitalPinToInterrupt(button_home));
  detachInterrupt(digitalPinToInterrupt(button_rst));
  timerEnd(timer);
  startGame = true;
  flag = 1;
  score = 0;
  Snake snake(STARTX, STARTY, gfx);
  Food food(snake);
  gfx.drawBackgroundMap();

  bool gameOver = false;
  // game play
  while (!gameOver) {

    int distanceX = food.get_x_food() - snake.getSnakeHead().first;
    int distanceY = food.get_y_food() - snake.getSnakeHead().second;

    if (distanceX > 0 && snake.getSnakeDirection() != 2) {
      snake.setSnakeDirection(0);
    } else if (distanceX < 0 && snake.getSnakeDirection() != 0) {
      snake.setSnakeDirection(2);
    } else if (distanceY > 0 && snake.getSnakeDirection() != 3) {
      snake.setSnakeDirection(1);
    } else if (distanceY < 0 && snake.getSnakeDirection() != 1) {
      snake.setSnakeDirection(3);
    }

    snake.moveSnake();

    //checking collision with food
    if (snake.checkSnakeCollision()) {
      gameOver = true;
      maxScore = EEPROM.read(address);
      //Serial.print(maxScore);
      if (score > maxScore) {
        EEPROM.write(address, score);
        EEPROM.commit();
        maxScore = score;
        //Serial.print(maxScore);
      }
      gfx.printScore(score, EEPROM.read(address));

    } else if (snake.body[0].first == food.get_x_food() && snake.body[0].second == food.get_y_food()) {
      food = Food(snake);
      score++;
      playTone(NOTE_C5, TONE_DURATION);
      snake.growSnake();
      if (score == 5) {
        gfx.setLevel(1);
        //food.setLevel(1);
        snake.setGfx(gfx);
        gfx.drawBackgroundMap();
      } else if (score == 10) {
        gfx.setLevel(2);
        //food.setLevel(2);
        snake.setGfx(gfx);
        gfx.drawBackgroundMap();
      }
    }

    snake.drawSnake();
    food.drawFood();

    // increasing dificulty by increasing the speed of snake
    if (score >= 0 && score < 5) {
      food.setLevel(0);
      gfx.setLevel(0);
      snake.setGfx(gfx);
      snakeSpeed = SNAKE_SPEED_EASY;
    } else if (score >= 5 && score < 10) {
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
  delay(3000);
  //timer.attach(15.0, demoGame);

  flag = 0;
  gfx.drawMenu();
  startGame = false;
  attachInterrupt(digitalPinToInterrupt(button_home),
                  button_home_interrupt_handler,
                  interrupt_trigger_home_type);
  attachInterrupt(digitalPinToInterrupt(button_rst),
                  button_rst_interrupt_handler,
                  interrupt_trigger_rst_type);
}

void playIntro() {
  int waveOffset;
  //Serial.println(sizeof(cwave) / sizeof(int));
  for (waveOffset = 0; waveOffset < sizeof(cwave) / sizeof(int); waveOffset++) {
    if (startGame == true) {
      noTone(BUZZER_PIN);
      return;
    }
    // sendDirections();
    delay(150);
    tone(BUZZER_PIN, cwave[waveOffset], 150);
  }
}

void callBackTimer() {
  
  demoGame();
}
volatile bool timerTrigger = false;
void IRAM_ATTR timerISR() {
  timerTrigger = true;
  timerWrite(timer, 0);
}



void setup() {
  M5.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  gfx.drawMenu();
  randomSeed(analogRead(1));

  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);

  attachInterrupt(digitalPinToInterrupt(button_home),
                  button_home_interrupt_handler,
                  interrupt_trigger_home_type);
  attachInterrupt(digitalPinToInterrupt(button_rst),
                  button_rst_interrupt_handler,
                  interrupt_trigger_rst_type);

  Wire.begin();
  M5.IMU.Init();
  Serial.begin(baud_rate);
  //timer.attach(15.0, callBackTimer); // restart timer when M5_BUTTON_RST is pressed
  //serialTimer.attach(0.6, sendDirections);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timerISR, true);
  timerAlarmWrite(timer, 15000000, true);
  timerAlarmEnable(timer);

  serialTimer.attach(0.6, sendDirections);
  M5.Lcd.setRotation(0);
  EEPROM.begin(EEPROM_SIZE);
  initialisation_complete = true;
}

void sendDirections() {
  float ax = 0, ay = 0, az = 0;
  M5.IMU.getAccelData(&ax, &ay, &az);
  // compare data from sensor to a user defined thrashold
  if (ax * 10 > THRESHOLD2) { // if direction is already opposite the command has no effect
    // left
    Serial.write("2");
    //Serial.println("2");
  } else if (ax * 10 < -THRESHOLD2 ) { // right
    Serial.write("0");
    //Serial.println("0");
  }
  else if (ay * 10 > THRESHOLD2 ) { // down
    Serial.write("1");
    //Serial.println("1");
  } else if (ay * 10 < -THRESHOLD2 ) {
    // up
    Serial.write("3");
    //Serial.println("3");
  }
  // delay(300);

}



void loop() {
 
  if(timerTrigger) {
    timerTrigger = false;
    demoGame();
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &timerISR, true);
    timerAlarmWrite(timer, 15000000, true);
    timerAlarmEnable(timer);
    timerStart(timer);
  }
  if (interrupt_home_process_status == triggered_home) {

    detachInterrupt(digitalPinToInterrupt(button_home));
    interrupt_home_process_status = !triggered_home;
    timerEnd(timer);
    if (flag == 0) {
      gameLoop();
      delay(menu_delay);
      gfx.drawMenu();
      //timer.attach(15.0, demoGame);
      timer = timerBegin(0, 80, true);
      timerAttachInterrupt(timer, &timerISR, true);
      timerAlarmWrite(timer, 15000000, true);
      timerAlarmEnable(timer);
      timerStart(timer);
      startGame = false;

      attachInterrupt(digitalPinToInterrupt(button_home),
                      button_home_interrupt_handler,
                      interrupt_trigger_home_type);
    }
  } 
  if (interrupt_rst_process_status == triggered_rst) {
    timerEnd(timer);
    interrupt_rst_process_status = !triggered_rst;
    gfx.drawInstructions();
    delay(instructions_delay);
    gfx.drawMenu();
    //timer.attach(15.0, demoGame);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &timerISR, true);
    timerAlarmWrite(timer, 15000000, true);
    timerAlarmEnable(timer);
    timerStart(timer);
    startGame = false;
  }
  if (startGame == false) {
    playIntro();
  }
  delay(10);
}
