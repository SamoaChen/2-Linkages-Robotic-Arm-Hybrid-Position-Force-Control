import numpy as np
import cv2
import image_capture
import center_location
import time

def ball_location_find():
    #TAKE IMAGES FROM BOTH CAMERAS (OPTIONAL)
    image_capture.ball_capture()
    print("Finished Image Capturing")
    
    #FIND CENTER PIXEL LOCATIONS OF BALLS IN TWO IMAGES
    ball_center=center_location.ball_center_find()

    #CAMERA INTRINSIC + EXTRINSIC PARAMETERS
    P_up=np.matrix([[748.52,1650.7,723.12,28611],[-979,86.303,1499.8,-1.3127e+05],[0.63453,-0.059503,0.77061,29.423]])
    P_down=np.matrix([[758.21,1668.9,718.81,-54026],[-944.17,109.39,1536,-1.2537e+05],[0.63451,-0.035199,0.77211,30.684]])


    #CONSTRUCT A MATRIXS FOR TWO CAMERAS
    #construct image frame location
    xu_p=ball_center[0]
    yu_p=ball_center[1]
    xd_p=ball_center[2]
    yd_p=ball_center[3]

    #construct subarray of P
    P_up1=P_up[0,:]
    P_up2=P_up[1,:]
    P_up3=P_up[2,:]

    P_down1=P_down[0,:]
    P_down2=P_down[1,:]
    P_down3=P_down[2,:]

    #form A matrixs
    A_up=np.concatenate(((yu_p*P_up3-P_up2),(P_up1-xu_p*P_up3),(xu_p*P_up2-yu_p*P_up1)),axis=0)
    A_down=np.concatenate(((yd_p*P_down3-P_down2),(P_down1-xd_p*P_down3),(xd_p*P_down2-yd_p*P_down1)),axis=0)
    A=np.concatenate((A_up,A_down),axis=0)

    #CALCULATE BALL WORLD COORDINATE LOCATION
    U, S, V =np.linalg.svd(A)
    V=np.transpose(V)
    scale_factor=V[3,3]
    world_location=V[:,3]/scale_factor

    return world_location[0:3]


