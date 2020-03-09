#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define LINE_PIN 3
extern const float pomery_pohybou[360][4];
int spd = 150;
byte a, b;
int angle;
int anti_angle;
int x_b, y_b, x_g, y_g;

LiquidCrystal lcd(26, 27, 28, 29, 40, 41);
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(LINE_PIN, INPUT);
  butons_setup();
  NXT_setup();
  NXT_nort_set();
  rpi_setup();
  LCD_setup();
  bno.begin();
  bno.setExtCrystalUse(true);
  menu();
}
void loop() {

  x_b = get_x_ball();
  x_g = get_x_goal(); 

  if (digitalRead(LINE_PIN)) {
    line_event();
  }
  else {
    if (x_g != 62) {
      if (x_b != -1) {
        movement(x_b, 510, x_g);
      }
      else {
        movement(180, 510, x_g);
      }
    } else {
      if (x_b != -1) {
        movement(x_b, 510, NXT_angle());
      }
      else {
        movement(180, 510, NXT_angle());
      }
    }

  }
}

void na_mieste(int vstup) {
  int16_t speeds[4] = {0, 0, 0, 0};
  int compensation;
  compensation = PID(speeds, 0, vstup);
  rotate_motor(1, speeds[0] + compensation);
  rotate_motor(2, speeds[1] + compensation);
  rotate_motor(3, speeds[2] + compensation);
  rotate_motor(4, speeds[3] + compensation);
}
void movement(int smer , int rychlost , int feedback) {
  smer = (smer + 90) % 360; //ja viem ze som to mohol prehodit v poli //TODO
  int16_t speeds[4] = {pomery_pohybou[smer][0]*rychlost, pomery_pohybou[smer][1]*rychlost, pomery_pohybou[smer][2]*rychlost, pomery_pohybou[smer][3]*rychlost};
  int compensation;
  compensation = PID(speeds, 0, feedback);


  rotate_motor(1, speeds[0] + compensation);
  rotate_motor(2, speeds[1] + compensation);
  rotate_motor(3, speeds[2] + compensation);
  rotate_motor(4, speeds[3] + compensation);
}
void off_motors() {
  rotate_motor(1, 0);
  rotate_motor(2, 0);
  rotate_motor(3, 0);
  rotate_motor(4, 0);
}
void vpred(int vstup) {
  int16_t speeds[4] = { -spd, -spd, spd, spd};
  int compensation;
  compensation = PID(speeds, 0, vstup);
  rotate_motor(1, speeds[0] + compensation);
  rotate_motor(2, speeds[1] + compensation);
  rotate_motor(3, speeds[2] + compensation);
  rotate_motor(4, speeds[3] + compensation);
}
