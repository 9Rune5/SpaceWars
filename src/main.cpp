#include <TFT_eSPI.h> // Hardware-specific library
#include "rocket.h"
#include "brod1.h"
#include "bullet.h"
#include "ebullet.h"
#include "life.h"
#include "rover.h"
#include "earth.h"
#include "ex.h"
#include "ex2.h"
#include "ricon.h"
#include "back2.h"
#include "sens.h"
#include "buum.h"
#include "gameOver.h"
#include <Tone32.h>

#define BUZZER_CHANNEL 0

#define TFT_GREY 0x5AEB
#define lightblue 0x2D18
#define orange 0xFB60
#define purple 0xFB9B

TFT_eSPI tft = TFT_eSPI();
int score = 0;
float bulletX[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float bulletY[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};

float EbulletX[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float EbulletY[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};

float rocketX[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float rocketY[10] = {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20};
float rocketSpeed = 0.22;
int rockets = 3;

int counter = 0;
int rcounter = 0;
int Ecounter = 0;
int level = 1;
float x = 10;
float y = 20;

float ey = 18;
float ex = 170;

float es = 0.1;

float bx = -50;
float by = 0;

int pom = 0;  // pressdebounce for fire
int pom2 = 0; // pressdebounce for rockets
float speed = 0.42;
int blinkTime = 0;
int eHealth = 50;
int mHealth = eHealth;
int lives = 4;
int ly[4] = {0, 0, 0, 0};
int ri[3] = {0, 0, 0};
int fireTime = 100;
int fireCount = 0;
float EbulletSpeed = 0.42;
int rDamage = 8; // rocket damage
int tr = 0;

int pom3 = 0;
bool sound = 1; // sound on or off

enum Phase { startScreen, gamePlaying, gameDone};
Phase phase = startScreen;

float spaceX[30];
float spaceY[30];

void setup(void)
{
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(25, OUTPUT);       // led2
  pinMode(33, OUTPUT);       // led1
  pinMode(26, OUTPUT);       // led3
  pinMode(BUTTON_LORALEFT, INPUT);         // LORA built in buttons
  pinMode(BUTTON_LORARIGHT, INPUT);
  digitalWrite(26, 1);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 240, 135, back2);

  for (int i = 0; i < 30; i++)
  {
    spaceX[i] = random(5, 235);
    spaceY[i] = random(18, 132);
  }

  while (digitalRead(13) == 1) // wait until button a is pressed.............
    int nezz = 0;
  digitalWrite(26, 0);
}

void restart()
{
  counter = 0;
  rcounter = 0;
  Ecounter = 0;
  level = 1;
  x = 10;
  y = 20;
  ey = 18;
  ex = 170;
  es = 0.1;
  bx = -50;
  by = 0;

  rockets = 3;
  rDamage = 8;
  lives = 4;
  score = 0;
  ri[0] = 0;
  ri[1] = 0;
  ri[2] = 0;
  ly[0] = 0;
  ly[1] = 0;
  ly[2] = 0;
  ly[3] = 0;
  ey = 44;
  speed = 0.42;
  eHealth = 50;
  mHealth = eHealth;
  EbulletSpeed = 0.42;
  rocketSpeed = 0.22;

  for (int i = 0; i < 10; i++)
  {
    bulletX[i] = -20;
    EbulletX[i] = -20;
    rocketX[i] = -20;
  }
}

void newLevel()
{
  level++;
  speed = speed + 0.05;
  EbulletSpeed = EbulletSpeed + 0.1;
  eHealth = 50 + (level * 5);
  mHealth = eHealth;
  es = 0.05 + (0.035 * level);

  rockets = 3;
  rDamage = 8 + (level * 2);
  rocketSpeed = 0.22 + (level * 0.02);
  ri[0] = 0;
  ri[1] = 0;
  ri[2] = 0;

  ey = 44;

  for (int i = 0; i < 10; i++)
  {
    bulletX[i] = -20;
    EbulletX[i] = -20;
    rocketX[i] = -20;
  }

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4);
  tft.print("Level " + String(level));
  tft.setCursor(0, 22, 2);

  tft.println("Enemy speed : " + String(es));
  tft.println("Enemy health : " + String(eHealth));
  tft.println("Enemy bullet speed : " + String(EbulletSpeed));
  tft.println("Remaining lives: " + String(lives));
  tft.println("My speed : " + String(speed));
  tft.println("Rocket damage : " + String(rDamage));
  tft.println("Rocket speed : " + String(rocketSpeed));

  tft.pushImage(170, 5, 55, 54, earth[level - 1]);
  tft.pushImage(170, 61, 72, 72, sens);
  delay(2600);

  while (digitalRead(13) == 1) // wait until button a is pressed.............
    int nezz = 0;

  tft.fillScreen(TFT_BLACK);

  tft.drawLine(0, 16, 240, 16, lightblue);
  tft.drawLine(0, 134, 240, 134, lightblue);

  tft.setCursor(200, 0, 2);
  tft.print(score);

  tft.fillRect(120, 3, 70, 7, TFT_GREEN);
  tft.drawRect(119, 2, 72, 9, TFT_GREY);
}

void loop()
{

  if (phase == startScreen)
  {
    restart();
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);
    tft.pushImage(0, 0, 240, 135, back2);
    while (digitalRead(13) == 1)
    {
      int nezz = 0;
    }
    tft.fillScreen(TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 4);
    tft.print("Level " + String(level));
    tft.setCursor(0, 22, 2);

    tft.println("Enemy speed : " + String(es));
    tft.println("Enemy health : " + String(eHealth));
    tft.println("Enemy bullet speed : " + String(EbulletSpeed));
    tft.println("Remaining lives: " + String(lives));
    tft.println("My speed : " + String(speed));
    tft.println("Rocket damage : " + String(rDamage));
    tft.println("Rocket speed : " + String(rocketSpeed));

    tft.pushImage(170, 5, 55, 54, earth[level - 1]);
    tft.pushImage(170, 61, 72, 72, sens);
    delay(1000);

    while (digitalRead(13) == 1) // wait until button a is pressed.............
      int nezz = 0;

    tft.fillScreen(TFT_BLACK);

    tft.drawLine(0, 16, 240, 16, lightblue);
    tft.drawLine(0, 134, 240, 134, lightblue);

    tft.setCursor(200, 0, 2);
    tft.print(score);

    tft.fillRect(120, 3, 70, 7, TFT_GREEN);
    tft.drawRect(119, 2, 72, 9, TFT_GREY);

    phase = gamePlaying;
  }

  if (phase == gamePlaying)
  {                                      // playing phase
    if (digitalRead(BUTTON_DOWN) == 0 and y < 94)
      y = y + speed;

    if (digitalRead(BUTTON_UP) == 0 and y > 18)
      y = y - speed;

    if (digitalRead(BUTTON_RIGHT) == 0 and x < 125)
      x = x + speed;

    if (digitalRead(BUTTON_LEFT) == 0 and x > 0)
      x = x - speed;

    if (digitalRead(BUTTON_A) == 0) // fire button A button
    {
      if (pom == 0)
      {
        pom = 1;

        bulletX[counter] = x + 34;
        bulletY[counter] = y + 15;
        counter = counter + 1;
      }
    }
    else
      pom = 0;

    if (digitalRead(BUTTON_B) == 0 && rockets > 0) // Rocket button B button
    {
      if (pom2 == 0)
      {
        pom2 = 1;
        rockets--;
        rocketX[rcounter] = x + 34;
        rocketY[rcounter] = y + 14;
        rcounter = rcounter + 1;
        ri[rockets] = -100;
        tft.fillRect(70 + (rockets * 14), 0, 8, 14, TFT_BLACK);
      }
    }
    else
      pom2 = 0;

    if (digitalRead(BUTTON_LORARIGHT) == 0) // mute sound
    {
      if (pom3 == 0)
      {
        pom3 = 1;
        sound = !sound;
      }
    }
    else
      pom3 = 0;

    for (int i = 0; i < 30; i++)
    { // drawStars..........................................
      tft.drawPixel(spaceX[i], spaceY[i], TFT_BLACK);
      spaceX[i] = spaceX[i] - 0.5;
      tft.drawPixel(spaceX[i], spaceY[i], TFT_GREY);
      if (spaceX[i] < 0)
      {
        tft.drawPixel(spaceX[i], spaceY[i], TFT_BLACK);

        spaceX[i] = 244;
      }
    }

    tft.pushImage(x, y, 49, 40, brod1);
    tft.pushImage(ex, ey, 55, 54, earth[level - 1]);

    for (int i = 0; i < 10; i++)
    { // firing bulletts
      if (bulletX[i] > 0)
      {
        tft.pushImage(bulletX[i], bulletY[i], 8, 8, bullet);
        bulletX[i] = bulletX[i] + 0.6;
      }
      if (bulletX[i] > 240)
        bulletX[i] = -30;
    }

    for (int i = 0; i < 10; i++)
    { // firing rockets
      if (rocketX[i] > 0)
      {
        tft.pushImage(rocketX[i], rocketY[i], 24, 12, rocket);
        rocketX[i] = rocketX[i] + rocketSpeed;
      }
      if (rocketX[i] > 240)
        rocketX[i] = -30;
    }

    // delay(1);

    for (int j = 0; j < 10; j++) // did my bullet hit enemy
    {
      if (bulletX[j] > ex + 20 && bulletY[j] > ey + 2 && bulletY[j] < ey + 52)
      {
        tft.pushImage(bulletX[j], bulletY[j], 12, 12, ex2);
        if (sound == 1)
        {
          tone(BUZZER_PIN, NOTE_C5, 12, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
        }
        else
        {
          delay(12);
        }
        tft.fillRect(bulletX[j], bulletY[j], 12, 12, TFT_BLACK);
        bulletX[j] = -50;
        score++;
        tft.setCursor(200, 0, 2);
        tft.print(score);
        eHealth--;
        tr = map(eHealth, 0, mHealth, 0, 70);
        tft.fillRect(120, 3, 70, 7, TFT_BLACK);
        tft.fillRect(120, 3, tr, 7, TFT_GREEN);

        if (eHealth <= 0)
        {
          tft.pushImage(ex, ey, 55, 55, buum);
          tone(BUZZER_PIN, NOTE_E4, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_D4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_G5, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_C3, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
          delay(700);
          newLevel();
        }
        digitalWrite(25, 1);
        blinkTime = 1;
      }
    }

    for (int j = 0; j < 10; j++) // did my ROCKET hit enemy
    {
      if (rocketX[j] + 18 > ex && rocketY[j] > ey + 2 && rocketY[j] < ey + 52)
      {
        tft.pushImage(rocketX[j], rocketY[j], 24, 24, explosion);
        if (sound == 1)
        {
          tone(BUZZER_PIN, NOTE_C3, 40, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
        }
        else
        {
          delay(40);
        }
        tft.fillRect(rocketX[j], rocketY[j], 24, 24, TFT_BLACK);
        // delay(30);

        rocketX[j] = -50;
        score += 12;
        tft.setCursor(200, 0, 2);
        tft.print(score);
        eHealth = eHealth - rDamage;
        tr = map(eHealth, 0, mHealth, 0, 70);
        tft.fillRect(120, 3, 70, 7, TFT_BLACK);
        tft.fillRect(120, 3, tr, 7, TFT_GREEN);

        if (eHealth <= 0)
        {
          tft.pushImage(ex, ey, 55, 55, buum);
          tone(BUZZER_PIN, NOTE_E4, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_D4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_G5, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_C3, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
          delay(700);
          newLevel();
        }
        digitalWrite(25, 0);
        blinkTime = 1;
      }
    }

    for (int j = 0; j < 10; j++) // Am I hit
    {
      if (EbulletX[j] < x + 30 && EbulletX[j] > x + 4 && EbulletY[j] > y + 4 && EbulletY[j] < y + 36)
      {
        EbulletX[j] = -50;
        ly[lives - 1] = -40;
        tft.fillRect((lives - 1) * 14, 0, 14, 14, TFT_BLACK);
        lives--;
        if (lives == 0)
        {
          tft.pushImage(x, y, 55, 55, buum);
          tone(BUZZER_PIN, NOTE_G4, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_B4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_C5, 100, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_A4, 80, BUZZER_CHANNEL);
          tone(BUZZER_PIN, NOTE_F4, 280, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
          delay(500);
          tft.fillScreen(TFT_BLACK);
          phase = gameDone;
        }

        digitalWrite(33, 1);
        blinkTime = 1;
        if (sound == 1)
        {
          tone(BUZZER_PIN, NOTE_C6, 4, BUZZER_CHANNEL);
          noTone(BUZZER_PIN, BUZZER_CHANNEL);
        }
        else
        {
          delay(4);
        }
      }
    }

    ey = ey + es;
    if (ey > 80)
      es = es * -1;

    if (ey < 18)
      es = es * -1;

    if (blinkTime > 0)
      blinkTime++;

    if (blinkTime > 2)
    {
      digitalWrite(25, 0);
      digitalWrite(33, 0);
      blinkTime = 0;
    }

    for (int i = 0; i < 10; i++)
    { // enemy shoots
      if (EbulletX[i] > -10)
      {
        tft.pushImage(EbulletX[i], EbulletY[i], 7, 7, ebullet);
        EbulletX[i] = EbulletX[i] - EbulletSpeed;
      }
    }

    for (int i = 0; i < 4; i++) // draw lifes
      tft.pushImage(i * 14, ly[i], 12, 11, life);
    for (int i = 0; i < 3; i++) // draw lifes
      tft.pushImage(70 + (i * 14), ri[i], 8, 14, ricon);

    fireCount++;
    if (fireTime == fireCount)
    {
      EbulletX[Ecounter] = ex + 5;
      EbulletY[Ecounter] = ey + 24;
      fireCount = 0;
      fireTime = random(110 - (level * 15), 360 - (level * 30));
      Ecounter++;
    }
    if (counter == 9)
      counter = 0;

    if (rcounter == 3)
      rcounter = 0;

    if (Ecounter == 9)
      Ecounter = 0;
  }
  if (phase == gameDone) // game over phase
  {

    tft.fillScreen(TFT_BLACK);
    tft.pushImage(0, 0, 240, 135, gameOver);
    tft.setCursor(24, 54, 2);
    tft.print("Score : " + String(score));
    tft.setCursor(24, 69, 2);
    tft.print("Level : " + String(level));
    while (digitalRead(13) == 1)
    {
      int nezz = 0;
    }
    phase = startScreen;
  }
}
