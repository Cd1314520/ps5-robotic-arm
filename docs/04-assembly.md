# 4. Assembly

<!-- TODO: photos for each step -->

Build from the base up, one joint at a time. Every joint follows the same pattern:

1. **Center the servo first.** Run [`servo_center`](03-servo-centering.md) so the servo
   sits at 90°.
2. **Mount the servo body** in its printed mount with the screws.
3. **Put the joint in its middle position by hand.** Halfway through its natural
   range of motion, not touching anything.
4. **Fit the horn** onto the servo's output shaft in that position, and screw it to the
   part it drives.
5. **Check by hand** (with the servo unpowered, gently) that the joint can move both ways
   from the middle without hitting anything nearby.

> **Safety:** Don't force a joint by hand while its servo is powered. The servo will
> fight you and can strip its gears.

## Order

| Step | Joint | Servo | Channel |
|---|---|---|---|
| 1 | Base rotation | MG996R | ch0 |
| 2 | Shoulder | MG996R | ch1 |
| 3 | Elbow | MG996R | ch2 |
| 4 | Wrist pitch | MG90S | ch3 |
| 5 | Wrist rotate | MG90S | ch4 |
| 6 | Gripper | MG90S | ch5 |

Write down which channel each servo will plug into as you go. This is easier than
tracing six identical cables later.

## Gripper

- Before you fit the gripper servo's horn, move the gears and fingers by hand. They
  should move smoothly. If they bind, put a little **petroleum jelly (Vaseline)** on the
  pivots and gear teeth. A binding gripper makes the servo stall.
- Optional: stick rubber or foam on the fingertips for better grip. The MG90S isn't
  strong, so friction does more than force. See
  [Troubleshooting](09-troubleshooting.md#the-gripper-is-weak-or-doesnt-close).

## Cables

Leave enough slack in each servo cable for the joint's full range of motion, with the
arm fully stretched out. A cable pulled tight can stop a joint, which causes a stall, or
pull out of the PCA9685.

## Next

Wire everything up in [05 – Wiring and power](05-wiring-and-power.md). Then find each
joint's real safe range in [08 – Calibration](08-calibration.md) before you use the
controller.
