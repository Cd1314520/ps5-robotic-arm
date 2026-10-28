// 舵机归中工具:装舵盘(horn)之前先跑这个,让舵机停在 90°。
// 每次只接一个舵机到 CH 通道,上传,等它转到中间,再装舵盘。
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int CH = 0;  // 要归中的通道 0-5

void setup() {
  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setPWM(CH, 0, 375);  // 375 = (150 + 600) / 2 = 90°,只设一次
}

void loop() {
  // 空的 —— 舵机停在中间不动
}
