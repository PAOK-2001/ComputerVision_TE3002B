import cv2
import numpy as np
import os
# Lista de rutas de imágenes por clase
classes = ["no_entry","curve_right","left_turn","bike_crossing","speed_limit_100","stop"]
features = {}
for sign_class in classes:
    features[sign_class] = [[]]

# Configuración del detector y el extractor sift
sift = cv2.SIFT_create()
folder_path = "../GTSRB-Training_fixed/GTSRB/training/"
# Extraer características para cada clase
for path in classes:
    for img_path in os.listdir("../GTSRB-Training_fixed/GTSRB/training/" + path):
        full_path = folder_path + path + '/' + img_path
        image = cv2.imread(full_path)
        # Get the original dimensions
        height, width = image.shape[:2]
        new_height, new_width = height * 10, width * 10
        image = cv2.resize(image, (new_width, new_height))
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        # keypoints = sift.detect(gray_image, None)
        keypoints, descriptors = sift.detectAndCompute(gray_image, None)
        if type(descriptors) != type(None):
            features[path].extend(descriptors)
    np.savetxt("FeatureMatrix_" + path + ".txt", features[path],fmt='%s')
        # draw only keypoints location,not size and orientation
        #img2 = cv2.drawKeypoints(gray_image, keypoints, None, color=(0,255,0), flags=0)
    #     cv2.imshow("Putes", img2)
    #     cv2.waitKey(0)
    # if type(features[path]) == type([]):
    #     features[path] = np.array(features[path])
    # print(features[path].shape)
    #np.savetxt("FeatureMatrix_" + path + ".txt", features[path], delimiter=',')

    # Preprocesamiento de la imagen si es necesario
    # gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # keypoints, descriptors = sift.detectAndCompute(gray_image, None)
    # all_features.append(descriptors)


# Convertir la lista de características en una matriz de datos

# Guardar la matriz de datos en un archivo

