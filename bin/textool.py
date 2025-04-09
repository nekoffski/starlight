#!/usr/bin/env python3.10

import sys
import cv2


filename = sys.argv[1]
image = cv2.imread(filename=filename)
filtered = cv2.Canny(image, 150, 250)


cv2.imshow('Texture', image)
cv2.imshow('Bump map', filtered)
cv2.waitKey(0)
cv2.destroyAllWindows()
