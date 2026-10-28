# 1. Parts and tools

> **Prices are rough estimates** (USD, typical online listings in October 2026), not
> what I paid. They change a lot by seller, so check current listings.

## Electronics

| Part | Qty | Approx. price | Notes |
|---|---|---|---|
| Arduino UNO R3 | 1 | $6–15 (compatible board) | The microcontroller. An official board costs more |
| PCA9685 16-channel PWM servo driver | 1 | $8–15 | I²C, default address `0x40` |
| MG996R servo | 3 | $18–28 per 4-pack | Base, shoulder, elbow. The big, strong joints |
| MG90S servo, **180°**, metal gear | 3 | $14–20 per 4-pack | Wrist pitch, wrist rotate, gripper |
| 5 V power supply | 1 | $6–15 | I used **5 V 2 A**: enough for one joint at a time. Get **5 V 5 A+** if you want to move several joints at once or automate anything. See [05](05-wiring-and-power.md#how-big-a-supply) |
| Barrel jack → screw terminal adapter | 1 | a few dollars | Connects the wall supply to the PCA9685's `V+` terminal |
| Jumper wires | ~5 | a few dollars | Arduino ↔ PCA9685 (5V, GND, SDA, SCL) |
| USB cable for the UNO | 1 | often included with the board | USB-A to USB-B |
| PS5 DualSense controller | 1 | $55–85 | |
| USB-C cable for the controller | 1 | a few dollars | Must be a data cable, not charge-only |

**Rough total:** about **$60–110** for the electronics, plus the controller if you don't
already have one, plus filament and screws.

> **Buy 180° servos, not 360° "continuous rotation" ones.** They look identical, but a
> 360° servo can't hold an angle, so it's useless as a joint. I made this mistake. See
> [Troubleshooting](09-troubleshooting.md#the-servo-spins-forever).

## Hardware

| Screw | Qty |
|---|---|
| M2 | 10 |
| M2.5 | 10 |
| M3 | 4 |

Also useful:
- Petroleum jelly (Vaseline): lubricates the gripper pivots and gears
- Rubber or foam: grippy fingertips (optional)

## 3D-printed parts

See [02 – Printing](02-printing.md).

## Tools

- 3D printer
- Small Phillips screwdrivers
- Multimeter (recommended): checks polarity and continuity before you power on
- A Windows PC with USB ports. This project has only been tested on Windows.

## Software (all free)

- Arduino IDE 2 + Adafruit PWM Servo Driver Library: [06 – Firmware](06-firmware.md)
- Python 3 + `pygame` + `pyserial`: [07 – PC control](07-pc-control.md)
