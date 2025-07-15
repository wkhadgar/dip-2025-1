#%%
import os
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

from modules.image_loader import ImageLoader
from modules.statistical_tools import StatisticalTools

#%% Generating and analyzing images

#%%
cv.namedWindow('img', cv.WINDOW_KEEPRATIO)
img = np.random.randint(low=0, high=256, size=(5,3), dtype=np.uint8)
print(img)
print('\n')

cv.imshow('img', img)
key = cv.waitKey(0)
cv.destroyAllWindows()

#%%
print(img)
print('\n')

r, c = 0, 0
print(f'r:{r}, c:{c}, {img[0,1]}\n')

r, c = 0, 1
print(f'r:{r}, c:{c}, {img[0,1]}\n')

cv.namedWindow('img', cv.WINDOW_KEEPRATIO)
cv.imshow('img', img)
key = cv.waitKey(0)
cv.destroyAllWindows()

#%%
cv.namedWindow('img', cv.WINDOW_KEEPRATIO)

while True:
        
    img = np.random.randint(low=0, high=256, size=(5,3), dtype=np.uint8)
    print(img)
    cv.imshow('img', img)
    
    key = cv.waitKey(500) & 0xFF
    
    match key:
        
        case 113: #ord('q'):
            cv.destroyAllWindows()
            break


#%%
statsTools = StatisticalTools()
statsTools.print_image_info(img)

#%% Creating images
#img = np.zeros((50, 50), dtype=np.int16)
img = 127*np.ones((50, 50), dtype='uint8')

cv.namedWindow('img', cv.WINDOW_KEEPRATIO)
cv.imshow('img', img)
key = cv.waitKey(0)
cv.destroyAllWindows()

#%% Creating an image filled with a scalar
img = 255*np.ones((50, 50), dtype=np.uint8)

cv.namedWindow('img', cv.WINDOW_KEEPRATIO)
cv.imshow('img', img)
key = cv.waitKey(0)
cv.destroyAllWindows()

#%% Creating random images uniformly distributed
img = np.ones((250, 250), dtype=np.uint8)
cv.randu(img, 0, 255)

plt.figure(figsize=(16,4))
plt.subplot(121), plt.imshow(img, cmap='gray', vmin=0, vmax=255)
plt.subplot(122), plt.hist(img.ravel(), 256, [0, 256])
plt.show()

#%% Initializing a color image with random values, uniformly distributed
img = np.ones((250, 250, 3), dtype=np.uint8)
statsTools.print_image_info(img)
print('\n')

bgr = cv.split(img)
cv.randu(bgr[0], 0, 255)
cv.randu(bgr[1], 0, 255)
cv.randu(bgr[2], 0, 255)
img = cv.merge(bgr)
statsTools.print_image_info(img)

plt.figure(figsize=(16,4))
plt.subplot(121), plt.imshow(img, cmap='gray', vmin=0, vmax=255)
plt.subplot(122)
color = ('b', 'g', 'r')
for i, col in enumerate(color):
    histr = cv.calcHist([img], [i], None, [256], [0, 256])
    plt.plot(histr, color = col)
    plt.xlim([0, 256])
plt.show()

#%% Initializing a color image with random values, normally distributed
img = np.ones((250, 250, 3), dtype=np.uint8)
bgr = cv.split(img)
cv.randn(bgr[0], 127, 40)
cv.randn(bgr[1], 127, 40)
cv.randn(bgr[2], 127, 40)
img = cv.merge(bgr)
statsTools.print_image_info(img)
print('\n')

plt.figure(figsize=(16,4))
plt.subplot(121), plt.imshow(img, cmap='gray', vmin=0, vmax=255)
plt.subplot(122)
for i, col in enumerate(color):
    histr = cv.calcHist([img], [i], None, [256], [0, 256])
    plt.plot(histr, color = col)
    plt.xlim([0, 256])
plt.show()

#%% Convert image to different ranges
img = np.ones((3, 3), dtype=np.float32)

cv.randn(img, 0, 1)
print("Normally distributed random values = \n", img, "\n\n")
statsTools.print_image_info(img)
print('\n')

cv.normalize(img, img, 255, 0, cv.NORM_MINMAX)
print("Normalized = \n", img, "\n\n")
statsTools.print_image_info(img)
print('\n')

img = np.asarray(img, dtype=np.uint8)
print("Converted to uint8 = \n", img, "\n\n")
statsTools.print_image_info(img)
print('\n')

#%% Loading and analyzing images
#%%
imgloader = ImageLoader()
url = 'https://raw.githubusercontent.com/tfvieira/dip-2025-1/refs/heads/main/img/lena.png'
img = imgloader.load_image_from_url(url)

cv.namedWindow('img', cv.WINDOW_KEEPRATIO)
cv.imshow('img', img)
key = cv.waitKey(0)
cv.destroyAllWindows()

#%%
statsTools.print_image_info(img)

