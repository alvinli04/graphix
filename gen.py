import sys
import math

f = open('test.dw', 'w')

for t in range(100):
    theta = t * 10 + 0.25 * t * t
    r = 100 - t
    f.write(f'clear\nsphere\n{-1*r} 0 0 30\nsphere\n{r} 0 0 30\nident\nrotate\ny {theta}\nrotate\nx {t * .3}\nmove\n250 250 -250\napply\n')
    f.write(f'save\nframe{str(t).zfill(3) }.png\n')

for t in range(50):
    f.write(f'clear\ntorus\n0 0 0 50 {50 + 50 * t}\nident\nrotate\nx 30\nrotate\ny 30\nmove\n250 250 -250\napply\n')
    f.write(f'save\nframe{str(t+100).zfill(3) }.png\n')

f.close()
