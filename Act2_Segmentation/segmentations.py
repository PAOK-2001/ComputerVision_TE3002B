import cv2
import numpy as np


path_kmeans ='Results/Comparison/kmeans/'
path_meanShift='Results/Comparison/meanShift/'
path_watershed='Results/Comparison/watershed/'
path_dt = 'Results/Comparison/distance_transform/'
path_noisy_watershed = 'Results/Comparison/watershed_noisy/'
path_noisy_meanShift = 'Results/Comparison/meanShift_noisy/'
path_noisy_kmeans = 'Results/Comparison/kmeans_noisy/'
watersheded = []
kmeans = []
mean_shift = []
dt = []

def load_images(num):
    images = []
    for i in range(num):
        images.append(cv2.imread("RoadSigns/road" + str(i) + ".png", cv2.IMREAD_GRAYSCALE))
    return images

def write_results(img_vec, path):
    for i in range(0,len(img_vec)):
        img_path = path + "RoadSign" + str(i) + ".png"
        cv2.imwrite(img_path, img_vec[i])

def image_process(images):
    for pic in images:
        #K-means segmentation
        flat_img = np.float32(pic.reshape((-1,1)))
        num_clusters = 3
        convergence_crit = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
        ret, labels, centers = cv2.kmeans(flat_img, num_clusters, None, convergence_crit, num_clusters, cv2.KMEANS_RANDOM_CENTERS)
        centers = np.uint8(centers)
        img_segmented_kmeans = centers[labels.flatten()]
        img_segmented_kmeans = img_segmented_kmeans.reshape((pic.shape))
        cv2.imshow('K-Means', img_segmented_kmeans)
        kmeans.append(img_segmented_kmeans)
        
        #Mean shift segmentation
        shifted = cv2.pyrMeanShiftFiltering(cv2.cvtColor(pic, cv2.COLOR_GRAY2BGR), 21, 51)
        gray = cv2.cvtColor(shifted, cv2.COLOR_BGR2GRAY)
        cv2.imshow('Mean shift', gray)
        mean_shift.append(gray)
        dist_transform = cv2.distanceTransform(pic, cv2.DIST_L2, 3)

        #Watershed
        # Apply thresholding to the grayscale image
        ret, binarized = cv2.threshold(pic, 0, 255,  cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
        structuring_element = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
        # Separate into the known background and foreground
        background = cv2.dilate(binarized, structuring_element, iterations=3)
        distance_transform = cv2.distanceTransform(binarized, cv2.DIST_L2, 5)
        dt_normalized = cv2.normalize(distance_transform, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)
        dt.append(dt_normalized)

        ret, foreground = cv2.threshold(distance_transform, 0.4 * distance_transform.max(), 255, 0)
        # Mark the reamining unknown area
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
        cv2.imshow("Watershed", watershed_img)
        watersheded.append(watershed_img)

        cv2.imshow('Original', pic)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    return
def add_noise(_images):
    noisy_images = []
    for i in range(len(_images)):
        noise = np.random.normal(0,10,_images[i].shape)
        noise_pos = noise.clip(min = 0)
        noise_pos = noise_pos.reshape(_images[i].shape).astype('uint8')
        noise_neg = abs(noise.clip(max = 0))
        noise_neg = noise_neg.reshape(_images[i].shape).astype('uint8')
        noisy_img = cv2.add(_images[i], noise_pos)
        noisy_img = cv2.subtract(noisy_img, noise_neg)
        noisy_images.append(noisy_img)
    return noisy_images

images = load_images(10)
image_process(images)
write_results(watersheded, path_watershed)
write_results(mean_shift, path_meanShift)
write_results(kmeans, path_kmeans)
write_results(dt,path_dt)
images = add_noise(images)

watersheded.clear()
mean_shift.clear()
kmeans.clear()

image_process(images)
write_results(watersheded, path_noisy_watershed)
write_results(mean_shift, path_noisy_meanShift)
write_results(kmeans, path_noisy_kmeans)
    