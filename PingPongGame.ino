#include <Adafruit_NeoPixel.h>
#define PIN 6
#define WIDTH 8 // Ширина
#define HEIGHT 8 // Высота
#define NUMPIXELS WIDTH * HEIGHT // Колличество пикселей
#define PADDLE_SIZE 2 // Размер площадки игрока
#define BUTTON_R 3 // Пин кнопки первого игрока
#define BUTTON_L 4 // Пин кнопки второго игрока
#define DELAY 50

Adafruit_NeoPixel NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int ballX = random(WIDTH);
int ballY = random(3,6); // Мяч появляется в верхней или нижней зоне
int ballDirX = random(2) * 2 - 1; // Случайное направление по горизонтали
int ballDirY = random(2) * 2 - 1; // Случайное направление по вертикали
int paddleY1 = HEIGHT / 2 - PADDLE_SIZE / 2; // Начальная позиция 1ой площадки по оси Y
int paddleX1 = 0; // Начальная позиция 1ой площадки по оси X
int paddleY2 = HEIGHT / 2 - PADDLE_SIZE / 2; // Начальная позиция 2ой площадки по оси Y
int paddleX2 = WIDTH - 1; // Начальная позиция 2ой площадки по оси X
int scorePlayer1 = 0;
int scorePlayer2 = 0;

int getPixelNumber(int x, int y) {
  // Функция для преобразования координат x, y в номер светодиода в матрице
  if (y % 2 == 0)
  {
    // Четные строки идут в прямом направлении
    return y * WIDTH + x;
  } 
  else
  {
    // Нечетные строки идут в обратном направлении
    return y * WIDTH + (WIDTH - 1 - x);
  }

void resetBall() // Функция возвращения мяча
 {
  ballX = random(WIDTH);
  ballY = random(2) * (HEIGHT - 1);
  ballDirX = random(2) * 2 - 1;
  ballDirY = random(2) * 2 - 1;
 }

void setup()
{
   NeoPixel.begin();
   pinMode(BUTTON_R, INPUT_PULLUP);
   pinMode(BUTTON_L, INPUT_PULLUP);
   Serial.begin(9600);  
   digitalWrite(BUTTON_R, HIGH);
   digitalWrite(BUTTON_L, HIGH);
}

void loop() 
{ 
   // Отображение нажатия кнопок
  NeoPixel.clear();
  if (digitalRead(BUTTON_R) == LOW) {
    Serial.println("Button R is pressed");
    delay(DELAY);
  }

  else if (digitalRead(BUTTON_L) == LOW) {
    Serial.println("Button L is pressed");
    delay(DELAY);
  }
   // Обновление позиции мяча
  ballX += ballDirX;
  ballY += ballDirY;

  // Проверка столкновения со стенками
  if (ballX <= 0 || ballX >= WIDTH - 1) {
    ballDirX *= -1;
  }
  if (ballY <= 0 || ballY >= HEIGHT - 1) {
    ballDirY *= -1;
  }

    // Проверка на пропуск мяча игроком
  if (ballY == 0 && (ballX < paddleY1 || ballX >= paddleY1 + PADDLE_SIZE)) 
  {
    scorePlayer2++;
    resetBall();
  }
   else if (ballY == HEIGHT - 1 && (ballX < paddleY1 || ballX >= paddleY1 + PADDLE_SIZE)) {
    scorePlayer1++;
    resetBall();
  }

    // Управление площадкой игрока 1
  if (!digitalRead(BUTTON_R) && paddleY1 > 0)
   {
    paddleY1--;
   }
  if (digitalRead(BUTTON_R) && paddleY1 < HEIGHT - PADDLE_SIZE)
   {
    paddleY1++;
  }

// Управление площадкой игрока 2
  if (!digitalRead(BUTTON_L) && paddleY2 > 0)
   {
    paddleY2--;
   }
  if (digitalRead(BUTTON_L) && paddleY2 < HEIGHT - PADDLE_SIZE)
   {
    paddleY2++;
  }

  // Отображение мяча
  NeoPixel.setPixelColor(getPixelNumber(ballX, ballY), NeoPixel.Color(150, 0, 0));

  // Отображение площадки 1 игрока
  for (int i = 0; i < PADDLE_SIZE; i++)
  {
    NeoPixel.setPixelColor(getPixelNumber(paddleY1 + i, HEIGHT - 1), NeoPixel.Color(0, 150, 0));
  }

// Отображение площадки 2 игрока
  for (int i = 0; i < PADDLE_SIZE; i++)
  {
    NeoPixel.setPixelColor(getPixelNumber(paddleY2 + i, HEIGHT - 1), NeoPixel.Color(0, 150, 0));
  }

   NeoPixel.show();
   delay(500); // Скорость мяча

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
