import sys
import math

f = open('test.dw', 'w')

f.write('push\n')
f.write('move\n')
f.write('250 250 0\n')
f.write('rotate\n')
f.write('x 20\n')
f.write('rotate\n')
f.write('z 20\n')

frames = 100
r = 200
theta = 360 / frames

for t in range(frames):
    f.write(f'rotate\ny {theta}\n')
    f.write('sphere\n0 0 0 75\n')
    x = r * math.cos (t * theta * math.pi / -180)
    z = r * math.sin (t * theta * math.pi / -180)
    f.write(f'sphere\n{x} 0 {z} 25\n')
    f.write('push\n')
    f.write(f'move\n{x} 0 {z}\n')
    f.write('rotate\nz 60\n')
    xx = r / 4 * math.cos (t * theta * math.pi / -90)
    zz = r / 4 * math.sin (t * theta * math.pi / -90)
    f.write(f'sphere\n{xx} 0 {zz} 10\n')
    f.write('pop\n')
    f.write('push\n')
    f.write(f'rotate\nz {t * theta * -1}\n')
    f.write('torus\n0 0 0 5 90\n')
    f.write('pop\n')
    f.write('push\n')
    f.write(f'rotate\nz {t * theta}\n')
    f.write('torus\n0 0 0 5 90\n')
    f.write('pop\n')
    f.write(f'save\nframe{str(t).zfill(3)}.png\n')
    f.write('clear\n')

f.close()
