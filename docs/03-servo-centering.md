# 3. Center every servo before attaching its horn

**Skip this step and you will probably break a servo. I broke more than one this way.**

## Why

A hobby servo can only turn through a fixed range, about 180°. The **horn** (the plastic
or metal arm that screws onto the servo's output shaft) fits onto a toothed shaft, so you
can push it on at almost any angle.

If you push the horn on at a random angle, "90° in software" no longer means "joint in
the middle" on the real arm. Later, when the software tells the joint to move inside a
range that looks safe, the joint runs into the frame or into its own end stop and keeps
pushing. That is a **stall**: the motor is powered but can't move. A stalled servo draws
its maximum current, heats up, strips its gears, and can burn out the small
potentiometer (position sensor) inside it. You'll usually hear it first, as a steady
buzz or whine without any movement.

The fix is simple. Command the servo to 90° (the middle of its travel), hold the joint
in its middle position by hand, and **then** push the horn on.

## What you need

- The PCA9685, Arduino and external 5 V supply, wired as in
  [05 – Wiring and power](05-wiring-and-power.md). Only the electronics, not the arm.
- Arduino IDE with the Adafruit PWM Servo Driver library installed. See
  [06 – Firmware](06-firmware.md), steps 1–2.
- [`firmware/servo_center/servo_center.ino`](../firmware/servo_center/servo_center.ino)

## Steps

1. Open `servo_center.ino` in the Arduino IDE. Set `CH` to the channel you'll plug
   the servo into (0–5). Plugging every servo into channel 0 for this step works too.
2. Plug **one** servo into that channel on the PCA9685. Brown/black wire goes to `GND`,
   red to `V+`, orange/yellow to `PWM`.
3. Turn on the external 5 V supply and upload the sketch.
4. The servo turns to 90° and holds there. Leave it powered.
   - **If it keeps spinning and never stops:** you have a 360° "continuous rotation"
     servo. It controls speed, not angle, so it can't be used as a joint. Unplug it and
     use a 180° servo. See [Troubleshooting](09-troubleshooting.md#the-servo-spins-forever).
5. Mark the shaft position, for example with a dot of marker on the spline, or leave
   the servo powered until it's installed in the arm.
6. Repeat for each of the six servos.

During assembly ([04 – Assembly](04-assembly.md)), put each joint in its middle
position before you fit and screw down its horn.

> **Safety:** If any servo buzzes and doesn't move, switch off the 5 V supply right away.
> That buzz means the servo is stalled.

## How the sketch works

The PCA9685 drives servos with **PWM** (pulse-width modulation): 50 times per second it
sends a short pulse, and the pulse width tells the servo which angle to go to. The
board counts each 20 ms period in 4096 steps. In this project, 150 steps is 0°, 600
steps is 180°, so the middle is `(150 + 600) / 2 = 375`. The sketch sends `375` once and
then does nothing. The PCA9685 keeps repeating the pulse on its own.
