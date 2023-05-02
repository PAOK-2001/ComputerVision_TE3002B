import cv2 as cv
import os
import numpy as np
from IPython.display import Image, display
from matplotlib import pyplot as plt

global images, segmented
images = []
segmented = []

def load_images(path):
    for image in os.listdir(path):
        images.append(cv.imread(path + "/" + image))

def segment_image(img):
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Apply thresholding to the grayscale image
    ret, binarized = cv.threshold(gray, 0, 255,  cv.THRESH_BINARY_INV + cv.THRESH_OTSU)
    structuring_element = cv.getStructuringElement(cv.MORPH_RECT, (3, 3))
    # Separate into the known background and foreground
    background = cv.dilate(binarized, structuring_element, iterations=3)
    distance_transform = cv.distanceTransform(binarized, cv.DIST_L2, 5)
    ret, foreground = cv.threshold(distance_transform, 0.4 * distance_transform.max(), 255, 0)
    # Mark the reamining unknown area
    foreground = np.uint8(foreground)
    remainder = cv.subtract(background,foreground)
    # Place markers in the local minimum
    ret, local_minimum = cv.connectedComponents(foreground)
    local_minimum+=1
    # Mark unknwon areas as 0
    local_minimum[remainder == 255] = 0
    segment_image = img.copy()
    markers = cv.watershed(segment_image, local_minimum)
    segment_image[markers == -1] = [0,255,255]
    segmented.append(segment_image)

def write_results(path):
    for i in range(0,len(images)):
        img_path = path + "RoadSign" + str(i)+".png"
        print(img_path)
        img_disp = cv.hconcat([images[i],segmented[i]])
        cv.imshow("Compare",img_disp)
        cv.imwrite(img_path,img_disp)
        cv.waitKey(0)

if __name__ == "__main__":
    load_images("RoadSigns")
    for image in images:
        segment_image(image)
    write_results("Results/Watershed/")
