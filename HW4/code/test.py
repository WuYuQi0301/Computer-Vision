import cv2
import numpy as np
img = cv2.imread("lab1.bmp")
l,a,b = cv2.split(img)
x = np.array(l)
y = np.array(a)
z = np.array(b)

print(np.mean(a))
print(np.mean(x))
print(np.mean(y))