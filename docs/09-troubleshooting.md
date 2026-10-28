# 9. Troubleshooting

I ran into every one of these problems while building the arm.

## The servo spins forever

**Symptom:** you send an angle and the servo keeps turning instead of stopping.

**Cause:** it's a **360° continuous-rotation** servo. The pulse sets its *speed*, not
its *angle*, so it can never hold a joint in place. My first batch of SG90s were this
kind. They look identical to the normal ones.

**Fix:** buy **180°** servos. Check the listing for "180°", not "360°" or "continuous".
Keep the 360° ones for wheels.

## A servo buzzes, gets hot, or stops working

**Symptom:** buzzing or whining with no movement. The servo gets hot. Later it jitters,
moves the wrong way, or doesn't move at all.

**Cause:** **stall**. The servo is pushing against a hard stop (the frame, another
part, or its own internal end stop) and can't reach the angle it was told. It draws
maximum current, overheats, strips its gears, and can kill the feedback potentiometer.
I lost several servos this way. The root cause was horns fitted at random angles, so
the "safe" software range ran into physical stops.

**Fix:**
- Cut power immediately whenever you hear the buzz.
- Center every servo before fitting its horn: [03 – Servo centering](03-servo-centering.md).
- Measure your soft limits so they stop short of every contact: [08 – Calibration](08-calibration.md).

## The Arduino resets or uploads fail when servos are connected

**Cause:** the servos are powered from the Arduino's 5 V pin. An MG996R draws more
current than the Arduino can supply, so the board browns out.

**Fix:** power the servos from an external 5 V supply on the PCA9685's `V+` terminal. See
[05 – Wiring and power](05-wiring-and-power.md).

## The Arduino can't find the PCA9685 (I²C)

**Symptom:** nothing moves, and an I²C scan shows no device.

**Cause (mine):** SDA/SCL connected to the wrong pins.

**Fix:** on the UNO, **SDA = A4** and **SCL = A5**. Also check `VCC` and `GND` to the
PCA9685.

To check, upload this scanner as a new sketch, open the Serial Monitor at 115200, and
press the Arduino's reset button. You should see `Found device at 0x40`.

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("Scan done");
}

void loop() {}
```

## The gripper is weak or doesn't close

First find out whether it's a power problem or a mechanical one:

1. Power off and remove the gripper servo's horn.
2. Move the gripper by hand.
   - **Stiff or catching:** the problem is mechanical. On mine, the pivots and gear
     teeth were binding. A little **petroleum jelly (Vaseline)** on the pivots and gear
     teeth made it move smoothly and stopped the stalling.
   - **Moves freely:** check that the servo is on the external 5 V supply, not the
     Arduino.

After that, my gripper still grips gently. That's the limit of a 9 g MG90S driving
this linkage, not a fault. **Don't try to add force in software.** Pushing it harder
only stalls the servo and strips the printed gear teeth. Instead, add grippy material
(rubber or foam) to the fingertips.

## The arm tips over

**Cause:** the shoulder swings too far, so the center of mass moves past the base.

**Fix:** narrow the shoulder (ch1) range. Mine is 50–125. See
[08 – Calibration](08-calibration.md).

## `control.py` can't open the COM port

**Symptom:** `could not open port 'COM4'` / `PermissionError` / `Access is denied`.

**Cause:** another program is using the port. Usually it's the Arduino IDE's **Serial
Monitor**.

**Fix:** close the Serial Monitor (or any other serial program), then run `control.py`
again. Also check that `PORT` in `control.py` matches the port shown under
**Tools → Port** in the Arduino IDE.

## The arm shakes while moving

**Fix:** keep `SPEED = 0.5` in `control.py`. When I set it higher, the arm shook.

## Wrong joint moves, or L1/R1 do nothing

**Cause:** your controller's axis/button numbers differ from mine.

**Fix:** run `finder.py` and update the mapping in `control.py`. See
[07 – PC control](07-pc-control.md#3-find-your-controllers-button-numbers-with-finderpy).
