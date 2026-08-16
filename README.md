# PS5-Controlled 6-DOF Robotic Arm

A 3D-printed desktop robotic arm driven live from a PlayStation 5 DualSense controller.
Six servos on an Arduino UNO + PCA9685, commanded over USB serial from a Python program
that turns stick deflection into joint velocity.

Built and calibrated on real hardware — the joint limits in this repo are measured, not guessed.

## Architecture

```
[PS5 DualSense] --USB--> [PC: control.py]
                              |  pygame reads sticks -> integrates to joint angles
                              |  pyserial writes "<ch> <angle>\n" @115200
                              v
                        [Arduino UNO]
                              |  sscanf parse -> constrain to soft limits
                              |  I2C
                              v
                    [PCA9685, addr 0x40]
                              |  6x PWM @50Hz
                              v
       ch0 base      ch1 shoulder   ch2 elbow      (MG996R)
       ch3 wrist pitch  ch4 wrist rotate  ch5 gripper  (MG90S 180deg)
```

Velocity control, not position control: the stick sets how fast a joint moves, and the
angle is integrated on the PC. Soft limits are enforced on **both** sides — the Python
side for responsiveness, the firmware side so a bad packet can't drive a joint into a stop.

## Hardware

| Part | Qty | Notes |
|---|---|---|
| Arduino UNO R3 | 1 | |
| PCA9685 16-ch PWM driver | 1 | I2C address `0x40` |
| MG996R servo | 3 | base, shoulder, elbow |
| MG90S servo (180 deg) | 3 | wrist pitch, wrist rotate, gripper |
| 5V power supply | 1 | **5A or better** — see Power below |
| PS5 DualSense controller | 1 | USB |
| 3D-printed frame | — | not included in this repo |

Servo power goes into the PCA9685 **V+ screw terminal from the external supply**, never
from the Arduino's 5V pin — the onboard regulator cannot source MG996R current and will
brown out the board mid-upload.

Ground the external supply to the Arduino GND. Without a shared ground the PWM signal
has no reference and the servos jitter.

## Power

This build ran on 5V 2A, which is enough for the manual one-joint-at-a-time control here
and nothing more. A single MG996R stalls at roughly 2.5A. If you move several joints
together, or add any kind of automation, **use 5A or more.** A 1000uF electrolytic across
the servo rail smooths the current spikes.

## Calibrated soft limits

Degrees, as enforced in both `control.py` and the firmware:

| ch | Joint | Min | Max |
|---|---|---|---|
| 0 | Base rotation | 0 | 90 |
| 1 | Shoulder | 50 | 125 |
| 2 | Elbow | 90 | 180 |
| 3 | Wrist pitch | 45 | 135 |
| 4 | Wrist rotate | 30 | 105 |
| 5 | Gripper | 70 | 110 |

The shoulder range is deliberately narrow — a wide swing tips the whole arm over.

**These numbers belong to one specific printed frame.** Print tolerances, horn splines,
and assembly angles all shift them. Re-derive them for your own build before running
`control.py` (see Safety).

## Controls

| Input | Joint |
|---|---|
| Left stick X | ch0 base |
| Left stick Y | ch1 shoulder |
| Right stick Y | ch2 elbow |
| Right stick X | ch3 wrist pitch |
| L1 / R1 | ch4 wrist rotate |
| L2 / R2 | ch5 gripper |

## Setup

```bash
pip install pygame pyserial
```

1. Flash `firmware/arm_control/arm_control.ino` (needs the Adafruit PWM Servo Driver library).
2. Run `python finder.py` with the controller plugged in. It prints every axis and button
   index live. **Do this first** — DualSense indices vary by driver. On this machine L1/R1
   came out as buttons 9 and 10, not the commonly documented 10 and 11.
3. Set `PORT` in `control.py` to your Arduino's COM port, and fix the axis/button indices
   from step 2 if they differ.
4. `python control.py`

`SPEED = 0.5` in `control.py` is deliberate. Higher values make this arm visibly shake.

## Safety

The MG996R servos have enough torque to pinch hard. Treat this as a machine, not a toy.

- **Keep a hand on the power switch the first time you run it.** A wrong axis index sends a
  joint to a limit at full speed.
- **Calibrate before you attach horns.** Command the servo to 90 degrees, power it, *then*
  fit the horn with the joint in a neutral pose. Attaching a horn at the wrong angle means
  the servo hits a physical stop inside its software range.
- **A stalled servo destroys itself.** Held against a stop it overheats, strips its gears,
  and can kill the feedback pot. Two servos were lost this way during the build. If a servo
  buzzes without moving, cut power immediately — that sound is stall current.
- **Do not raise gripper force in software.** The gripper is gentle because a 9g MG90S
  through this linkage is gentle. Driving it harder strips the printed gear teeth. Add
  rubber or foam to the fingertips instead.
- Soft limits protect the frame, not you. Keep fingers out of the workspace while powered.
- Bystanders and pets clear of the arm. Nothing here is rated for anything but a desk.

## Known limitations

- No inverse kinematics — joints are driven individually.
- No pose record/replay.
- The 5V 2A supply is the binding constraint on anything coordinated.
- Gripper force is at the mechanical ceiling of the MG90S.

## Roadmap

- Vision-guided grasping with a USB webcam
- Pose record and replay
- Inverse kinematics

## License

MIT — see [LICENSE](LICENSE). Provided as-is; you are responsible for the safety of
your own build.
