#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
#include <avr/power.h>
#endif
#define PIN 6
#define WIDTH 16 // Width
#define HEIGHT 16 // Height
#define NUMPIXELS WIDTH * HEIGHT // Number of pixels
#define PADDLE_SIZE 4 // The size of the player's paddle
#define BUTTON_1 3 // Pin of the first player's button
#define BUTTON_2 4 // Pin of the second player's button

int ballX = random(WIDTH);
int ballY = random(7,9); // The ball appears in the upper or lower zone
int ballDirX = 1; // Horizontal direction
int ballDirY = 1; // Vertical direction
int paddleY1 = 0; // The starting position of the 1 site on the Y axis
int paddleX1 = WIDTH/2; // The starting position of the 1 site on the X axis
int paddleY2 = HEIGHT-1; // The starting position of the 2 site on the Y axis
int paddleX2 = WIDTH/2; // The starting position of the 2 site on the X axis
int scorePlayer1 = 0;
int scorePlayer2 = 0;

Adafruit_NeoPixel NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
   NeoPixel.begin();
   NeoPixel.show();
   Serial.begin(9600);
   pinMode(BUTTON_1, INPUT_PULLUP);
   pinMode(BUTTON_2, INPUT_PULLUP);

}

int getPixelNumber(int x, int y)
{
  // Function for converting x, y coordinates to LED number
  if (y % 2 == 0)
  {
    return y * WIDTH + x;
  } 
  else
  {
    return y * WIDTH + (WIDTH - 1 - x);
  }
}

void resetBall() // Ball return function
 {
   ballX = random(WIDTH);
   ballY = random(7,9);
   ballDirX = 1;
   ballDirY = 1;
 }

void loop()
 {
  NeoPixel.clear();

   ballX += ballDirX;
   ballY += ballDirY;

  // Checking for collisions with walls
  if (ballX == 0 || ballX == WIDTH - 1) 
  {
    ballDirX *= -1;
  }

  // Checking for collisions with player sites
   if ( (ballY == 0 || ballY == HEIGHT - 1) && (ballX > paddleX1 && ballX <= paddleX1 + PADDLE_SIZE))
   {
    ballDirY *= -1;
   }
   else if ( (ballY == 0 || ballY == HEIGHT - 1) && (ballX > paddleX2 && ballX <= paddleX2 + PADDLE_SIZE))
   {
    ballDirY *= -1;
   }

  // Checking for the passing of the ball by the player
  if (ballY == 0 && (ballX < paddleX1 || ballX >= paddleX1 + PADDLE_SIZE)) 
  {
    scorePlayer2++;
    resetBall();
  }
   else if (ballY == HEIGHT - 1 && (ballX < paddleX2 || ballX >= paddleX2 + PADDLE_SIZE)) 
   {
    scorePlayer1++;
    resetBall();
   }

  // Managing Player 1's paddle
  if (!digitalRead(BUTTON_1) && paddleX1 < WIDTH - PADDLE_SIZE)
   {
    paddleX1++;
   }
  if (digitalRead(BUTTON_1) && paddleX1 > 0)
   {
    paddleX1--;
  }

   // Managing Player 2's paddle
  if (!digitalRead(BUTTON_2)&& paddleX2 > 0)
   {
    paddleX2--;
   }
  if (digitalRead(BUTTON_2) && paddleX2 < WIDTH - PADDLE_SIZE )
   {
    paddleX2++;
  }

  // Displaying the paddle of 1 player
  for (int i = 0; i < PADDLE_SIZE; i++)
  {
    NeoPixel.setPixelColor(getPixelNumber(paddleX1 + i, 0), NeoPixel.Color(0, 0, 150));
  }

  // Displaying the paddle of 2 player
  for (int i = 0; i < PADDLE_SIZE; i++)
  {
    NeoPixel.setPixelColor(getPixelNumber(paddleX2 + i, HEIGHT - 1), NeoPixel.Color(0, 150, 0));
  }

  // Displaying the ball
   NeoPixel.setPixelColor(getPixelNumber(ballX, ballY), NeoPixel.Color(150, 0, 0));

   NeoPixel.show();
   delay(250); // The speed of the ball

   // Withdrawal of the current account every second
  static unsigned long LastScoreUpdateTime = 0;
  if (millis() - LastScoreUpdateTime >= 1000)
   {
    Serial.print("Player 1: ");
    Serial.print(scorePlayer1);
    Serial.print(" - Player 2: ");
    Serial.println(scorePlayer2);
    LastScoreUpdateTime = millis();
   }
 }
