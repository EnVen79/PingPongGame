#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
#include <avr/power.h>
#endif
#define PIN 6
#define WIDTH 16 // Ширина
#define HEIGHT 16 // Высота
#define NUMPIXELS WIDTH * HEIGHT // Колличество пикселей
#define PADDLE_SIZE 4 // Размер площадки игрока
#define BUTTON_1 3 // Пин кнопки первого игрока
#define BUTTON_2 4 // Пин кнопки второго игрока

int ballX = random(WIDTH);
int ballY = random(7,9); // Мяч появляется в верхней или нижней зоне
int ballDirX = 1; // Направление по горизонтали
int ballDirY = 1; // Направление по вертикали
int paddleY1 = 0; // Начальная позиция 1ой площадки по оси Y
int paddleX1 = WIDTH/2; // Начальная позиция 1ой площадки по оси X
int paddleY2 = HEIGHT-1; // Начальная позиция 2ой площадки по оси Y
int paddleX2 = WIDTH/2; // Начальная позиция 2ой площадки по оси X
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
  // Функция для преобразования координат x, y в номер светодиода
  if (y % 2 == 0)
  {
    return y * WIDTH + x;
  } 
  else
  {
    return y * WIDTH + (WIDTH - 1 - x);
  }
}

void resetBall() // Функция возвращения мяча
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

  // Проверка столкновения со стенками
  if (ballX == 0 || ballX == WIDTH - 1) 
  {
    ballDirX *= -1;
  }

  // Проверка столкновения с площадками игроков
   if ( (ballY == 0 || ballY == HEIGHT - 1) && (ballX > paddleX1 && ballX <= paddleX1 + PADDLE_SIZE))
   {
    ballDirY *= -1;
   }
   else if ( (ballY == 0 || ballY == HEIGHT - 1) && (ballX > paddleX2 && ballX <= paddleX2 + PADDLE_SIZE))
   {
    ballDirY *= -1;
   }

  // Проверка на пропуск мяча игроком
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

  // Управление площадкой игрока 1
  if (!digitalRead(BUTTON_1) && paddleX1 < WIDTH - PADDLE_SIZE)
   {
    paddleX1++;
   }
  if (digitalRead(BUTTON_1) && paddleX1 > 0)
   {
    paddleX1--;
  }

   // Управление площадкой игрока 2
  if (!digitalRead(BUTTON_2)&& paddleX2 > 0)
   {
    paddleX2--;
   }
  if (digitalRead(BUTTON_2) && paddleX2 < WIDTH - PADDLE_SIZE )
   {
    paddleX2++;
  }

  // Отображение площадки 1 игрока
  for (int i = 0; i < PADDLE_SIZE; i++)
  {
    NeoPixel.setPixelColor(getPixelNumber(paddleX1 + i, 0), NeoPixel.Color(0, 0, 150));
  }

  // Отображение площадки 2 игрока
  for (int i = 0; i < PADDLE_SIZE; i++)
  {
    NeoPixel.setPixelColor(getPixelNumber(paddleX2 + i, HEIGHT - 1), NeoPixel.Color(0, 150, 0));
  }

  // Отображение мяча
   NeoPixel.setPixelColor(getPixelNumber(ballX, ballY), NeoPixel.Color(150, 0, 0));

   NeoPixel.show();
   delay(250); // Скорость мяча

   // Вывод текущего счета каждую секунду
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
