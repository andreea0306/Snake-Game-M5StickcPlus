#include <M5StickCPlus.h>
#include <vector>
#include <utility> 

#define HEIGHT 234
#define WIDTH 135
#define GRID_SIZE 9
#define THRESHOLD 5
// light green
const unsigned short DARK_GREEN  = M5.Lcd.color565(10, 220,10);

// more draker green
const unsigned short LIGHT_GREEN = M5.Lcd.color565(40, 200, 60);

// map codes
#define C0 LIGHT_GREEN
#define C1 DARK_GREEN

// variables used
// contor used for generating the map
//static int cont = 0;
// score variable
int score;
// used for using the accelerometer sensor
MPU6886 IMU; // create an instance of the MPU6886 class 

void drawBackgroundMap() {
  static int cont = 0;
  for(int i=0;i<WIDTH;i = i+GRID_SIZE) {
    for(int j=0;j<HEIGHT;j = j+GRID_SIZE) {
      if((i+j)%2==0) {
        M5.Lcd.fillRect(i,j, GRID_SIZE, GRID_SIZE, C0);
      } else {
        M5.Lcd.fillRect(i,j, GRID_SIZE, GRID_SIZE, C1);
      }
      cont++;
    }
  }
}

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

class Snake{
private :
  int direction;

public:
  std::vector<std::pair<int,int>> body;

  Snake(int x, int y) {
    direction = 0;
    body.push_back(std::make_pair(x,y)); // add the head of the snake
    body.push_back(std::make_pair(x-GRID_SIZE,y));
    body.push_back(std::make_pair(x-2*GRID_SIZE,y));
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
    body.insert(body.begin(), head/*std::make_pair(head.first, head.second)*/);
    body.pop_back();
    drawBackgroundMap();
    
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
};

void gameLoop() {
  float ax = 0, ay = 0, az = 0;
  score = 0;
  Food food;
  Snake snake(18, 18);
  drawBackgroundMap();
//  food.drawFood();
//  snake.drawSnake();
  bool gameOver = false;
  
  while(!gameOver) {
       // drawBackgroundMap();

    M5.IMU.getAccelData(&ax,&ay,&az);
    if(ax*10 > THRESHOLD) {
      snake.setSnakeDirection(2);
      //Serial.print("Snake is going righ\n");
    } else if(ax*10 < -THRESHOLD) {
      snake.setSnakeDirection(0);
     // Serial.print("Snake is going left\n");
    }
    else if(ay*10 > THRESHOLD) {
      snake.setSnakeDirection(1);
      //Serial.print("Snake is going up\n");
    } else if(ay*10 < -THRESHOLD){
      snake.setSnakeDirection(3);
      //Serial.print("Snake is going down\n");
    }
    snake.moveSnake();
    
    //checking collision with food
    if(snake.checkSnakeCollision()) {
      gameOver = true;
      //drawBackgroundMap();
      M5.Lcd.setCursor(10, 60);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.println(score);
      
    } else if(snake.body[0].first == food.get_x_food() && snake.body[0].second == food.get_y_food()){
      food = Food();
      score++;
      snake.growSnake();
    }
    
    snake.drawSnake();
    food.drawFood();
    delay(500);
    
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
  M5.Lcd.println("Instructions:");
  M5.Lcd.println(" ");
  M5.Lcd.println(" ");
  M5.Lcd.println("Use the accelerometer to control the snake.");
  M5.Lcd.println(" ");
  M5.Lcd.println("Collect food to grow");
  M5.Lcd.println("your snake and ");
  M5.Lcd.println("increase your score.");
  M5.Lcd.println(" ");
  M5.Lcd.println("Avoid colliding with ");
  M5.Lcd.println("the walls or your own tail.");
}

void setup() {
  M5.begin();
  drawMenu();
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);
  Wire.begin();
  M5.IMU.Init();
  Serial.begin(9600); 
  M5.Lcd.setRotation(0);  
}

void loop(){

  if(digitalRead(M5_BUTTON_RST) == LOW) {
    drawInstructions();
    delay(8000);
    drawMenu();
  } else if(digitalRead(M5_BUTTON_HOME) == LOW){
    /*
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println("ADD CODE TO START GAME");
    delay(5000);
    drawMenu();   
    */
    gameLoop();
    delay(2000);
    drawMenu();
    delay(200);
  } else {
    while(digitalRead(M5_BUTTON_RST) == HIGH && digitalRead(M5_BUTTON_HOME) == HIGH) {
      delay(50);
    }
  }
  delay(50);

}
