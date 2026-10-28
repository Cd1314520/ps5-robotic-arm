# 6. Firmware: Arduino IDE, library, upload

The firmware is the program that runs on the Arduino. It reads lines like `2 120` from
USB, keeps the angle inside that joint's safe range, and tells the PCA9685 to move the
servo.

## 1. Install the Arduino IDE

Download Arduino IDE 2 from <https://www.arduino.cc/en/software> and install it.

## 2. Install the servo driver library

1. In the IDE, open **Tools → Manage Libraries…**
2. Search for **Adafruit PWM Servo Driver Library** (by Adafruit) and click **Install**.
   If it asks to install dependencies, choose **Install all**.

## 3. Open and upload the firmware

1. Open [`firmware/arm_control/arm_control.ino`](../firmware/arm_control/arm_control.ino).
   The folder and the `.ino` file must keep the same name, because the Arduino IDE
   requires it.
2. Plug the Arduino into the PC with its USB cable.
3. **Tools → Board → Arduino AVR Boards → Arduino Uno.**
4. **Tools → Port** → pick the COM port that shows "Arduino Uno". Note it (for example
   `COM4`), because you'll need it in [07 – PC control](07-pc-control.md).
5. Click **Upload** (the → button). Wait for "Done uploading".

If the upload fails, see [Troubleshooting](09-troubleshooting.md).

## What the firmware does

```
on power-up / reset:
    move each joint, one at a time, to the middle of its safe range
forever:
    read a line "<ch> <angle>"
    clamp angle to that channel's soft limits
    send it to the PCA9685
```

- **Homing at start-up.** The firmware moves each joint, one at a time and 0.3 s apart,
  to the middle of its range. `control.py` assumes the arm starts there, so the first
  stick movement is smooth instead of a sudden jump. Joints are moved one at a time
  because a 5 V 2 A supply can't start six servos at once.
- **Soft limits.** A **soft limit** is a range enforced in software (here, for
  example, the shoulder can only go from 50° to 125°), as opposed to a physical stop.
  The firmware clamps every command to `minAngle`/`maxAngle`. `control.py` clamps
  too, so there are two independent guards. A bad command can't drive a joint into the
  frame. The values in this repo fit **my** arm. Measure yours in
  [08 – Calibration](08-calibration.md).

> **Safety: the arm moves when the Arduino resets.** That happens at power-up, after
> every upload, and **every time a program opens the serial port** (`control.py` or the
> Serial Monitor). Keep your hands out of the arm's reach whenever you do any of these.

## Test it without the controller

1. **Tools → Serial Monitor.** Set the baud rate to **115200** and the line ending to
   **New Line**.
2. The arm homes when the monitor opens.
3. Type `0 60` and press Enter. The base should turn to 60°.
4. Try `0 200`. The base stops at 90°, its soft limit.
5. **Close the Serial Monitor** before you run `control.py`. Only one program can use
   a COM port at a time.
