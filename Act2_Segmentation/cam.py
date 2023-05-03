import cv2
import numpy as np
import random

def image_process(pic):
    pic_color = pic.copy()
    pic = cv2.cvtColor(pic, cv2.COLOR_BGR2GRAY)

    #K-means segmentation | Result var: img_segmented_kmeans
    flat_img = np.float32(pic.reshape((-1,1)))
    num_clusters = 3
    convergence_crit = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
    ret, labels, centers = cv2.kmeans(flat_img, num_clusters, None, convergence_crit, num_clusters, cv2.KMEANS_RANDOM_CENTERS)
    centers = np.uint8(centers)
    img_segmented_kmeans = centers[labels.flatten()]
    img_segmented_kmeans = img_segmented_kmeans.reshape((pic.shape))
    
    #Mean shift segmentation | Result var: shifted
    shifted = cv2.pyrMeanShiftFiltering(cv2.cvtColor(pic, cv2.COLOR_GRAY2BGR), 21, 51)

    #Watershed | Result var: watershed_img
    # Apply thresholding to the grayscale image
    ret, binarized = cv2.threshold(pic, 0, 255,  cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    structuring_element = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    # Separate into the known background and foreground
    background = cv2.dilate(binarized, structuring_element, iterations=3)
    distance_transform = cv2.distanceTransform(binarized, cv2.DIST_L2, 3)
    dt_normalized = cv2.normalize(distance_transform, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)
    ret, foreground = cv2.threshold(distance_transform, 0.4 * distance_transform.max(), 255, 0)
    # Mark the remaining unknown area
    foreground = np.uint8(foreground)
    remainder = cv2.subtract(background,foreground)
    # Place markers in the local minimum
    ret, local_minimum = cv2.connectedComponents(foreground)
    local_minimum+=1
    # Mark unknown areas as 0
    local_minimum[remainder == 255] = 0
    watershed_img = cv2.cvtColor(pic, cv2.COLOR_GRAY2BGR).copy()
    markers = cv2.watershed(watershed_img, local_minimum)
    watershed_img[markers == -1] = [0,255,255]

    # Color segments
    marker_image = np.uint8(markers)*50
    img_segmented_kmeans = cv2.cvtColor(img_segmented_kmeans, cv2.COLOR_GRAY2BGR)
    img_segmented_kmeans = cv2.applyColorMap(img_segmented_kmeans, cv2.COLORMAP_TURBO)
    shifted = cv2.applyColorMap(shifted, cv2.COLORMAP_TURBO)
    marker_image = cv2.applyColorMap(marker_image, cv2.COLORMAP_TURBO)

    mosaic = cv2.vconcat([ \
        cv2.hconcat([pic_color,img_segmented_kmeans]),\
        cv2.hconcat([shifted,marker_image]) \
    ])

    mosaic = cv2.resize(mosaic, None, fx=0.75, fy=0.75, interpolation=cv2.INTER_LINEAR)

    cv2.imshow("video",mosaic)

    return

rand_colors = []

vid = cv2.VideoCapture(0)
  
while(True):
      
    ret, frame = vid.read()
  
    processed_images = image_process(frame)
      
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
  
vid.release()
cv2.destroyAllWindows()
    