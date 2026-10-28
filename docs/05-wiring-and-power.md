# 5. Wiring and power

## The boards

- **Arduino UNO**: the microcontroller. It receives commands from the PC over USB and
  tells the servo driver what to do.
- **PCA9685**: a 16-channel servo driver board. It generates the servo pulses so the
  Arduino doesn't have to. The Arduino talks to it over **I²C**, a two-wire bus:
  `SDA` carries data and `SCL` carries the clock. Every I²C device has an address. The
  PCA9685's default address is `0x40`.

## Wiring diagram

```
                 USB (to PC)
                     |
            +--------+--------+                 +------------------------+
            |   Arduino UNO   |                 |        PCA9685         |
            |                 |                 |                        |
            |             5V  |---------------->| VCC   (logic power)    |
            |            GND  |---------------->| GND                    |
            |             A4  |---------------->| SDA                    |
            |             A5  |---------------->| SCL                    |
            +-----------------+                 |                        |
                                                | V+  screw terminal  <--+---- external 5V (+)
                                                | GND screw terminal  <--+---- external 5V (-)
                                                |                        |
                                                | ch0  base         MG996R
                                                | ch1  shoulder     MG996R
                                                | ch2  elbow        MG996R
                                                | ch3  wrist pitch  MG90S
                                                | ch4  wrist rotate MG90S
                                                | ch5  gripper      MG90S
                                                +------------------------+
```

| From | To | Why |
|---|---|---|
| Arduino `5V` | PCA9685 `VCC` | Powers the PCA9685's chip only, not the servos |
| Arduino `GND` | PCA9685 `GND` | Shared ground (see below) |
| Arduino `A4` | PCA9685 `SDA` | I²C data. On the UNO, SDA is **A4** |
| Arduino `A5` | PCA9685 `SCL` | I²C clock. On the UNO, SCL is **A5** |
| External 5 V `+` | PCA9685 `V+` screw terminal | Servo power |
| External 5 V `−` | PCA9685 `GND` screw terminal | Servo power return |

Each servo plugs into a 3-pin column on the PCA9685. Brown/black goes to `GND`, red to
`V+`, orange/yellow to `PWM`. Check the orientation before you power on.

I used a barrel-jack-to-screw-terminal adapter so a 5 V wall supply could feed the
`V+` terminal.

> **Warning: A4/A5, not the digital pins.** I first connected SDA/SCL to the wrong pins
> and the Arduino couldn't find the PCA9685 at all. On the UNO, I²C is on **A4 (SDA)**
> and **A5 (SCL)**.

## Power: never run the servos from the Arduino's 5 V pin

The Arduino's 5 V pin can only supply a small current. One MG996R can draw more than
that when it starts moving, and much more when it stalls (its datasheet lists about
2.5 A stall current). When I powered servos from the Arduino, the board browned out
(its voltage dropped and it reset), and uploads failed.

So the servos get their own **external 5 V supply** on the PCA9685's `V+` terminal. The
Arduino's 5 V only powers the PCA9685's logic chip through `VCC`.

### How big a supply?

My arm runs on a **5 V 2 A** supply. That's enough to move **one joint at a time** by
hand with the controller, which is how this project works. The firmware also moves the
joints to their start position one at a time at power-on for the same reason.

If you want to move several joints at once, or add any automation, use **5 V 5 A or
more**.

> **Safety:** Check the polarity (`+`/`−`) on the screw terminal before you switch on.
> Reversed power can destroy the PCA9685 and every servo plugged into it.

## Common ground

A signal voltage only means something relative to a ground. The Arduino sends I²C
signals and the PCA9685 sends servo pulses, so the Arduino, the PCA9685 and the
external supply all have to share the same ground. This is called a **common ground**.
Without it, servos jitter or don't respond.

Wire Arduino `GND` → PCA9685 `GND` (in the table above), **and** make sure the external
supply's `−` is connected to the Arduino's `GND` too. If you're not sure the PCA9685
already links its two `GND` points, run a wire from the external supply's `−` to an
Arduino `GND` pin. An extra ground wire does no harm.

## Before you power on

- [ ] SDA → A4, SCL → A5
- [ ] Arduino 5 V goes only to `VCC`, not to `V+`
- [ ] External supply polarity checked
- [ ] Every servo plugged in the right way round (brown/black = GND)
- [ ] Grounds connected
