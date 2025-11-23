#include <Arduino.h>


int led_00 = 10;
int led_01 = 10;
int led_02 = 10;
int led_03 = 10;

int sw_00 = 10;
int sw_01 = 10;
int sw_02 = 10;
int sw_03 = 10;

bool flag_00 = false;
bool flag_01 = false;
bool flag_02 = false;
bool flag_03 = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(sw_00,  INPUT );
  pinMode(sw_01,  INPUT );
  pinMode(sw_02,  INPUT );
  pinMode(sw_03,  INPUT );
  pinMode(led_00, OUTPUT);
  pinMode(led_01, OUTPUT);
  pinMode(led_02, OUTPUT);
  pinMode(led_03, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  flag_00 = digitalRead(sw_00);
  flag_01 = digitalRead(sw_01);
  flag_02 = digitalRead(sw_02);
  flag_03 = digitalRead(sw_03);

  digitalWrite(led_00, flag_00);
  digitalWrite(led_01, flag_01);
  digitalWrite(led_02, flag_02);
  digitalWrite(led_03, flag_03);
  delay(10);
}