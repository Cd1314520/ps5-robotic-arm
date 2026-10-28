# PS5-Controlled 6-DOF Robotic Arm

A 3D-printed desktop robotic arm that you drive live with a PlayStation 5 DualSense
controller. This repo is a **step-by-step tutorial**: you don't need any electronics or
programming background. Every step explains what to do and why.

<!-- TODO: photo / GIF of the finished arm -->

## What you'll build

- **6 joints** ("6 degrees of freedom"): base, shoulder, elbow, wrist pitch, wrist
  rotate, gripper
- **Arduino UNO + PCA9685** servo driver, **3× MG996R + 3× MG90S** servos
- **PS5 controller → PC → Arduino**: a small Python program turns stick movements into
  joint movements
- **Soft limits** (software-enforced safe ranges) on every joint, checked both on the
  PC and on the Arduino

## How it works

```
[PS5 DualSense] --USB--> [PC: control.py]
                              |  pygame reads the sticks; stick = joint speed
                              |  sends "<channel> <angle>\n" over USB @115200 baud
                              v
                        [Arduino UNO: arm_control.ino]
                              |  parses the line, clamps to soft limits
                              |  I²C
                              v
                    [PCA9685 servo driver, address 0x40]
                              |  servo pulses (PWM, 50 Hz)
                              v
       ch0 base      ch1 shoulder      ch2 elbow       (MG996R)
       ch3 wrist pitch   ch4 wrist rotate   ch5 gripper (MG90S 180°)
```

## Tutorial

| # | Chapter | What you'll do |
|---|---|---|
| 1 | [Parts and tools](docs/01-parts-and-tools.md) | Shopping list |
| 2 | [Printing](docs/02-printing.md) | Which parts to print and how |
| 3 | [Servo centering](docs/03-servo-centering.md) | Set every servo to 90° **before** fitting horns |
| 4 | [Assembly](docs/04-assembly.md) | Build the arm from the base up |
| 5 | [Wiring and power](docs/05-wiring-and-power.md) | PCA9685, external 5 V, common ground |
| 6 | [Firmware](docs/06-firmware.md) | Arduino IDE, library, upload |
| 7 | [PC control](docs/07-pc-control.md) | Python, `finder.py`, `control.py` |
| 8 | [Calibration](docs/08-calibration.md) | Measure safe ranges for **your** arm |
| 9 | [Troubleshooting](docs/09-troubleshooting.md) | Every mistake I made, and the fix |

## Repo layout

```
control.py                    PC program: PS5 controller → serial commands
finder.py                     Prints your controller's axis/button numbers
firmware/arm_control/         Arduino firmware for the arm
firmware/servo_center/        Tool: hold one servo at 90° while you fit its horn
hardware/stl/                 3D-printable parts
hardware/source/              Fusion 360 source for the gripper
docs/                         The tutorial
```

## Before you start: safety

- The MG996R servos are strong enough to pinch hard. Treat the arm as a machine, not a
  toy.
- **The arm moves on its own whenever the Arduino resets.** That happens at power-up,
  after an upload, and every time `control.py` or the Serial Monitor connects. Keep your
  hands clear.
- **A buzzing servo that isn't moving is stalling.** Cut power immediately or it will
  destroy itself.
- Keep one hand near the power switch the first few times you run it.

Each chapter repeats the warnings that apply to its own step.

## Known limitations

- Joints move one at a time; no inverse kinematics.
- No pose record/replay.
- The 5 V 2 A supply is the limit for any coordinated motion.

## Credits

The arm's overall layout was inspired by Dejan's
[DIY Arduino Robot Arm](https://howtomechatronics.com/tutorials/arduino/diy-arduino-robot-arm-with-smartphone-control/)
on HowToMechatronics. The parts in this repo are my own models, and the electronics and
control (PCA9685, PS5 controller, PC-side velocity control) are different.

## License

Code and 3D models: MIT, see [LICENSE](LICENSE). Provided as-is; you are responsible for the safety
of your own build.
