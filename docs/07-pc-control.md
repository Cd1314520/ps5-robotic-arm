# 7. PC control with the PS5 controller

`control.py` runs on your PC. It reads the DualSense controller and sends joint commands
to the Arduino over USB.

It uses **velocity control**: how far you push a stick sets how fast a joint moves, not
where it goes. Let go and the joint stops where it is.

## 1. Install Python

1. Download Python 3 from <https://www.python.org/downloads/>.
2. In the installer, tick **"Add python.exe to PATH"**, then install.
3. Open a terminal (Windows: search for "PowerShell") and check:

   ```bash
   python --version
   ```

## 2. Install the two libraries

```bash
pip install pygame pyserial
```

- `pygame` reads the game controller.
- `pyserial` talks to the Arduino over the USB serial port.

## 3. Find your controller's button numbers with `finder.py`

Different drivers number the DualSense buttons differently. On my PC, L1/R1 were
buttons **9 and 10**, not the 10 and 11 that many guides list. So check yours first.

1. Connect the DualSense to the PC with a USB cable.
2. In the repo folder, run:

   ```bash
   python finder.py
   ```

3. It prints every axis (`轴`) and button (`键`) value about five times per second.
   Move one stick or press one button at a time and note which number changes:
   - Sticks: axis values go from −1 to 1.
   - L2/R2 triggers: axis values go from −1 (released) to 1 (fully pressed).
   - L1/R1: a button changes from 0 to 1.
4. Press `Ctrl+C` to stop.

## 4. Edit `control.py`

Open `control.py` in a text editor.

```python
PORT  = 'COM4'     # ← your Arduino's COM port, from chapter 6
SPEED = 0.5        # keep at 0.5; faster makes the arm shake
```

If your numbers from step 3 differ from mine, change these:

```python
AXIS_CONTROL = [
    (0, 0, +1),   # (axis number, channel, direction)
    (1, 1, -1),
    (2, 3, +1),
    (3, 2, -1),
]
BTN_CONTROL  = (9, 10, 4)   # (L1 button, R1 button, channel 4)
TRIG_CONTROL = (4, 5, 5)    # (L2 axis, R2 axis, channel 5)
```

If a joint moves the opposite way from the stick, flip its direction between `+1` and
`-1`.

**Why `SPEED = 0.5`:** each frame (50 per second) a joint moves at most `SPEED` degrees,
so at full stick that's 25° per second. When I tried faster speeds, the whole arm shook.

## 5. Run it

1. Close the Arduino Serial Monitor if it's open, because it would block the COM port.
2. Turn on the external 5 V supply.
3. **Keep one hand near the power switch the first time.**
4. Run:

   ```bash
   python control.py
   ```

   The Arduino resets and homes the arm one joint at a time. `control.py` waits 2 s
   for that, then prints `已连接: <controller name>` ("connected").
5. Press `Ctrl+C` to quit.

## Controls (default mapping)

| Input | Joint |
|---|---|
| Left stick left/right | ch0 base |
| Left stick up/down | ch1 shoulder |
| Right stick up/down | ch2 elbow |
| Right stick left/right | ch3 wrist pitch |
| L1 / R1 | ch4 wrist rotate |
| L2 / R2 | ch5 gripper |

> **Safety:** Move one joint at a time. On a 5 V 2 A supply, moving several joints at
> once can make the supply sag and the servos twitch.
