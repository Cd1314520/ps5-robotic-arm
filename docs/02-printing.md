# 2. Printing the parts

All the printable parts are in [`hardware/stl/`](../hardware/stl/). They're numbered
roughly in assembly order, from the base up.

| File | Part |
|---|---|
| `01-base.stl` | Base |
| `02-base-mount-mg996r.stl` | Mount for the base servo (MG996R) |
| `03-waist.stl` | Waist: the part that rotates on the base |
| `04-shoulder-mount-mg996r.stl` | Mount for the shoulder servo (MG996R) |
| `05-arm-01.stl` | Arm 01: upper arm |
| `06-elbow-mount-mg996r.stl` | Mount for the elbow servo (MG996R) |
| `07-arm-02.stl` | Arm 02: forearm |
| `08-arm-03.stl` | Arm 03: wrist |
| `09-gripper-base.stl` | Gripper base |
| `10-gripper-gear-2.stl` | Gripper gear 2 |

**The rest of the gripper** (gear 1, the links and the fingers) is in the Fusion 360
file [`hardware/source/gripper-assembly.f3d`](../hardware/source/gripper-assembly.f3d).

<!-- TODO: export gear1 / grip link / Gripper from the .f3d as STLs, with quantities -->

To export them yourself, open the file in Fusion 360 (free for personal use),
right-click each body → **Save As Mesh** → STL.

## Print settings

These are the settings I used, on a Bambu Lab A1 in Bambu Studio:

| Setting | Value | Why |
|---|---|---|
| Material | PLA (Bambu PLA Basic) | Easy to print, stiff enough for a desk arm |
| Nozzle | 0.4 mm | Standard |
| Layer height | 0.2 mm | |
| Infill | 40 %, gyroid | Stronger than the usual 15–20 %. The arm parts carry the weight of everything above them |
| Walls | 5 | Most of a part's strength is in its walls, especially around screw holes and servo pockets |
| Top / bottom layers | 9 / 7 | |
| Supports | Off | |
| Brim | Auto | Helps the first layer stick |

## Check the fit before you print everything

Every printer prints a little differently. A servo pocket or screw hole that fits on my
printer might be tight or loose on yours. Before printing the big parts, print one
small part, such as a servo mount, and check:

- Does the servo slide into its pocket without forcing it?
- Do the screws bite into the holes?

If not, scale or adjust the model, or tune your printer's dimensional accuracy, before
you print the rest.
