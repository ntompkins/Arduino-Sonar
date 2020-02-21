// IEP II, GROUP 26, PROJECT 4: SONAR MACHINE
// Nathan Tompkins | 4/29/19

#include <Servo.h>.
#include <SPI.h>
#include <TFT_ST7735.h>

TFT_ST7735 screen = TFT_ST7735(128, 160);
Servo servo;

const int X_MIN = 0;
const int Y_MIN = 0;
const int X_CENTER = 80;
const int Y_CENTER = 74;
const int X_MAX = 128;
const int Y_MAX = 160;
const int SpeedSwitch = A2;

const int DLED = 2;
const int BUZZ = 3;
const int ECHO = 4;
const int TRIG = 5;

long duration;
int m = 1;
int d = 0;
int pos = 0;
int speedVal = 50;

void setup(void)
{
  // Ultrasonic
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  // Buzzer
  pinMode(BUZZ, OUTPUT);

  // Servo
  servo.attach(6);

  // Screen
  screen.init();
  screen.setRotation(3); // Rotate to landscape mod
  screen.fillScreen(TFT_BLACK);
  screen.drawLine(80, 0, 80, 128, TFT_WHITE);
  screen.drawLine(80, 0, 160, 128, TFT_WHITE);
  screen.drawLine(80, 0, 0, 128, TFT_WHITE);
  screen.drawCircle(80, 0, 40, TFT_WHITE);
  screen.drawCircle(80, 0, 80, TFT_WHITE);
  screen.drawCircle(80, 0, 120, TFT_WHITE);
}

void loop(void)
{
  // Left to right
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    checkSpeed();
    drawRadar();
    servo.write(pos);
    delay(speedVal);
  }

  redraw();

  // Right to left
  for (pos = 180; pos >= 0; pos -= 1) {
    checkSpeed();
    drawRadar();
    servo.write(pos);
    delay(speedVal);
  }

  redraw();
}

void drawRadar() {
  d = readDistance();
  if (d < 10) {
    screen.drawLine(X_CENTER, Y_MIN, -200 * cos(radians(pos * m)), 200 * sin(radians(pos * m)), TFT_RED);
    tone(BUZZ, 300);
    digitalWrite(DLED, HIGH);
  }
  else if (d < 20) {
    screen.drawLine(X_CENTER, Y_MIN, -200 * cos(radians(pos * m)), 200 * sin(radians(pos * m)), TFT_YELLOW);
    tone(BUZZ, 150);
    digitalWrite(DLED, LOW);
  }
  else if (d < 30) {
    screen.drawLine(X_CENTER, Y_MIN, -200 * cos(radians(pos * m)), 200 * sin(radians(pos * m)), TFT_GREEN);
    tone(BUZZ, 100);
    digitalWrite(DLED, LOW);
  }
  else {
    screen.drawLine(X_CENTER, Y_MIN, -200 * cos(radians(pos * m)), 200 * sin(radians(pos * m)), TFT_BLUE);
    noTone(BUZZ);
    digitalWrite(DLED, LOW);
  }
}

void redraw() {
  // Redraw white lines
  screen.fillScreen(TFT_BLACK);
  screen.drawLine(80, 0, 80, 128, TFT_WHITE);
  screen.drawLine(80, 0, 160, 128, TFT_WHITE);
  screen.drawLine(80, 0, 0, 128, TFT_WHITE);
  screen.drawCircle(80, 0, 40, TFT_WHITE);
  screen.drawCircle(80, 0, 80, TFT_WHITE);
  screen.drawCircle(80, 0, 120, TFT_WHITE);
}

int readDistance() {
  // Ultrasonic Read
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}

void checkSpeed() {
 if(analogRead(SpeedSwitch) > 100) {
    speedVal = 250;
  }
  else {
    speedVal = 50;
  }
}
