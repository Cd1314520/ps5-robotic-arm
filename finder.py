import pygame
pygame.init(); pygame.joystick.init()
js = pygame.joystick.Joystick(0); js.init()
print(js.get_name(), "轴数:", js.get_numaxes(), "键数:", js.get_numbuttons())
while True:
    pygame.event.pump()
    axes = [round(js.get_axis(i), 2) for i in range(js.get_numaxes())]
    btns = [js.get_button(i) for i in range(js.get_numbuttons())]
    print("轴", axes, " 键", btns)
    pygame.time.wait(200)