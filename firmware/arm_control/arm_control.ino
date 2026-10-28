/*
 * 6-DOF robotic arm firmware — Arduino UNO + PCA9685 (I2C 0x40)
 *
 * Protocol: one command per line over USB serial @115200
 *     "<channel> <angle>\n"      e.g. "0 45\n"
 *
 * Angles are clamped to the per-channel soft limits below. These limits are
 * calibrated to ONE physical build — see docs/08-calibration.md.
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  // 默认地址 0x40
#define SERVOMIN 150   // 0°   (50Hz 下 4096 计数里的脉宽)
#define SERVOMAX 600   // 180°
const int NUM = 6;
// 软限位,和 control.py 里的 LIMIT 一致,两边各限一次
//                     0   1   2   3    4    5
int minAngle[NUM] = { 0, 50, 90, 45,  30,  70};
int maxAngle[NUM] = {90,125,180,135, 105, 110};

void writeAngle(int ch, int angle){
  pwm.setPWM(ch, 0, map(angle, 0, 180, SERVOMIN, SERVOMAX));
}

void setup(){
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);
  // 上电归位:每个关节移到软限位中间,和 control.py 的起始角度一致。
  // 一次只动一个 —— 5V 2A 电源带不动几个舵机同时启动。
  // 6 x 300ms < control.py 打开串口后等待的 2 秒。
  for (int ch = 0; ch < NUM; ch++){
    writeAngle(ch, (minAngle[ch] + maxAngle[ch]) / 2);
    delay(300);
  }
}

void loop(){
  static char buf[16];
  static byte idx = 0;
  while (Serial.available()){
    char c = Serial.read();
    if (c == '\n'){
      buf[idx] = 0; idx = 0;
      int ch, angle;
      if (sscanf(buf, "%d %d", &ch, &angle) == 2 && ch >= 0 && ch < NUM){
        writeAngle(ch, constrain(angle, minAngle[ch], maxAngle[ch]));
      }
    } else if (idx < 15){
      buf[idx++] = c;
    }
  }
}
