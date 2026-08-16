import pygame, serial, time

PORT  = 'COM4'
BAUD  = 115200
SPEED = 0.5      # 速度,越大越快
DEAD  = 0.15        # 摇杆死区

# 通道: [最小角, 最大角]
LIMIT = {
    0: [0,   90],
    1: [50,  125],
    2: [90,  180],
    3: [45,  135],
    4: [30,  105],
    5: [70,  110],
}

# ===== 控制映射:想换就只改这三块 =====
# (摇杆轴号, 控制的通道, 方向 +1/-1)
AXIS_CONTROL = [
    (0, 0, +1),   # 左摇杆 左右 → 通道0 底座
    (1, 1, -1),   # 左摇杆 上下 → 通道1 肩
    (2, 3, +1),   # 右摇杆 左右 → 通道3 腕俯仰
    (3, 2, -1),   # 右摇杆 上下 → 通道2 肘
]
BTN_CONTROL  = (9, 10, 4)   # (L1, R1, 通道4 腕旋转)
TRIG_CONTROL = (4, 5, 5)     # (L2轴, R2轴, 通道5 夹爪)

ser = serial.Serial(PORT, BAUD); time.sleep(2)
pygame.init(); pygame.joystick.init()
js = pygame.joystick.Joystick(0); js.init()
print("已连接:", js.get_name())

cur = {ch: (lo + hi) / 2 for ch, (lo, hi) in LIMIT.items()}
clock = pygame.time.Clock()

def dz(v):
    return 0 if abs(v) < DEAD else v

def drive(ch, val):
    if val == 0: return
    lo, hi = LIMIT[ch]
    new = min(hi, max(lo, cur[ch] + SPEED * val))
    if int(new) != int(cur[ch]):
        ser.write(f"{ch} {int(new)}\n".encode())
    cur[ch] = new

while True:
    pygame.event.pump()
    for axis, ch, d in AXIS_CONTROL:
        drive(ch, dz(js.get_axis(axis)) * d)
    down, up, ch = BTN_CONTROL
    drive(ch, (1 if js.get_button(up) else 0) - (1 if js.get_button(down) else 0))
    la, ra, ch = TRIG_CONTROL
    drive(ch, (js.get_axis(ra) + 1) / 2 - (js.get_axis(la) + 1) / 2)
    clock.tick(50)