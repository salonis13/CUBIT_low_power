import cv2
import numpy as np
  
# read the image file
img = cv2.imread('CU.png', 2)

img = cv2.resize(img, (168,144), interpolation = cv2.INTER_AREA)

ret, bw_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
  
# converting to its binary form
bw = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
bw[1][bw[1] > 127] = 1

one_d_array = (bw[1].ravel())
print(bw[1].tolist())
print(one_d_array)
cv2.imshow("Binary", bw_img)
one_d_array = np.transpose(one_d_array)
np.savetxt('bitmap_raw.h', bw[1], fmt='%d', delimiter=',')   # X is an array


appendText=','
names=open("bitmap_raw.h",'r')
updatedNames=open("bitmap.h",'a')
for name in names:
  updatedNames.write(name.rstrip() + appendText + '\n')
updatedNames.close()


cv2.waitKey(0)
cv2.destroyAllWindows()