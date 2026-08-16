/*
 * 6-DOF robotic arm firmware — Arduino UNO + PCA9685
 *
 * Protocol: one command per line over USB serial @115200
 *     "<channel> <angle>\n"      e.g. "0 45\n"
 *
 * Angles are clamped to the per-channel soft limits below. These limits are
 * calibrated to ONE physical build — see README before running on your own.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const uint8_t  NUM_CH    = 6;
const uint16_t SERVO_HZ  = 50;
const uint16_t PULSE_MIN = 150;  // ~0 deg  (counts out of 4096 @ 50Hz)
const uint16_t PULSE_MAX = 600;  // ~180 deg

// Soft limits, mirrored in control.py. Both sides clamp — double guard.
// ch: 0 base, 1 shoulder, 2 elbow, 3 wrist pitch, 4 wrist rotate, 5 gripper
const uint8_t LIMIT_LO[NUM_CH] = {  0,  50,  90, 45, 30,  70};
const uint8_t LIMIT_HI[NUM_CH] = { 90, 125, 180, 135, 105, 110};

void writeAngle(uint8_t ch, uint8_t angle) {
    pwm.setPWM(ch, 0, map(angle, 0, 180, PULSE_MIN, PULSE_MAX));
}

void setup() {
    Serial.begin(115200);
    pwm.begin();
    pwm.setPWMFreq(SERVO_HZ);
    delay(10);

    // Home to mid-range of each joint's safe span.
    for (uint8_t ch = 0; ch < NUM_CH; ch++) {
        writeAngle(ch, (LIMIT_LO[ch] + LIMIT_HI[ch]) / 2);
        delay(200);  // one joint at a time — 5V 2A can't take six at once
    }
    Serial.println(F("ready"));
}

void loop() {
    static char line[16];
    static uint8_t n = 0;

    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            line[n] = '\0';
            n = 0;
            int ch, angle;
            if (sscanf(line, "%d %d", &ch, &angle) == 2 && ch >= 0 && ch < NUM_CH) {
                writeAngle(ch, constrain(angle, LIMIT_LO[ch], LIMIT_HI[ch]));
            }
        } else if (n < sizeof(line) - 1) {
            line[n++] = c;
        }
        // ponytail: overlong lines just drop the tail; the PC never sends >8 chars
    }
}
