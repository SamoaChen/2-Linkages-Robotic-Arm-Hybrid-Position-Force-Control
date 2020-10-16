import cv2
import numpy as np
import time

def ball_center_find():
    #PIXEL LOCATION OF BALL IN CAMERA UP IMAGE
    image='camera_up.jpg'
    img=cv2.imread(image,1)
    img_orig=img.copy()
    img=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    img=cv2.GaussianBlur(img,(15,15),cv2.BORDER_DEFAULT)
    all_circs=cv2.HoughCircles(img,cv2.HOUGH_GRADIENT,0.9,100,param1=50,param2=30,minRadius=1, maxRadius=110)
    all_circs_rounded=np.uint16(np.around(all_circs))
    circle_up=all_circs_rounded[0,0]

    #PIXEL LOCATION OF BALL IN CAMERA DOWN IMAGE
    image='camera_down.jpg'
    img=cv2.imread(image,1)
    img_orig=img.copy()
    img=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    all_circs=cv2.HoughCircles(img,cv2.HOUGH_GRADIENT,0.9,100,param1=50,param2=30,minRadius=1, maxRadius=160)
    all_circs_rounded=np.uint16(np.around(all_circs))
    circle_down=all_circs_rounded[0,0]

    return [circle_up[0],circle_up[1],circle_down[0],circle_down[1]]
    
#print(ball_center_find())

