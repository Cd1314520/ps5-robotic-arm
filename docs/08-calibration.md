# 8. Calibration: find your own soft limits

**Don't copy my limits and hope they fit.** Print tolerances, where each horn landed on
its spline, and small assembly differences all shift where each joint hits something.
Measure your own.

These are mine, for reference:

| ch | Joint | Min | Max | Note |
|---|---|---|---|---|
| 0 | Base | 0 | 90 | |
| 1 | Shoulder | 50 | 125 | Kept narrow: a big shoulder swing tips the whole arm over |
| 2 | Elbow | 90 | 180 | |
| 3 | Wrist pitch | 45 | 135 | |
| 4 | Wrist rotate | 30 | 105 | |
| 5 | Gripper | 70 | 110 | 70 = closed, 110 = open |

## What "safe" means

For each joint, find the two angles where it **comes close to** touching the frame,
another part, the table, or its own end stop. Then pull each one back a few degrees.
The joint should never press against anything, because pressing means stalling (see
[03 – Servo centering](03-servo-centering.md)).

Also watch the whole arm. If a joint can swing far enough to tip the arm over (on my arm
that was the shoulder), stop there.

## Method: one joint at a time, with the Serial Monitor

You'll temporarily widen one channel's limit in the firmware, probe it in small steps,
then write the real values back.

1. In `arm_control.ino`, set the channel you're testing to a wide range, for example for
   the elbow (ch2):

   ```cpp
   int minAngle[NUM] = { 0, 50,   0, 45,  30,  70};
   int maxAngle[NUM] = {90,125, 180,135, 105, 110};
   ```

   Leave every other channel alone. Upload.
2. Open the Serial Monitor (115200, New Line). The arm homes.
3. **Hand on the power switch.** Starting from the middle, send the angle in 5° steps
   toward one end, for example `2 85`, `2 80`, `2 75`, and so on. After each step, look
   and listen.
4. Stop at the first sign of trouble: the joint touches something, it buzzes, or the
   arm starts to tip. Cut power if it buzzes. Go back 5–10° and write that number
   down.
5. Do the same toward the other end.
6. For the **gripper**, "closed" is where the fingers just meet, not where they're
   squeezing hard. Squeezing is a stall.
7. Put your measured values for that channel into **both** files:
   - `firmware/arm_control/arm_control.ino`: `minAngle` / `maxAngle`
   - `control.py`: `LIMIT`
8. Upload again, close the Serial Monitor, and repeat for the next joint.

The two files must match. The firmware is the final guard, and `control.py`'s limits
keep the stick from "winding up" past a limit the firmware would ignore.
