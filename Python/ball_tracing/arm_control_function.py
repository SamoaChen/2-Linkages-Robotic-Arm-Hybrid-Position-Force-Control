import math as mt
import numpy as np
#import time
#start=time.perf_counter()
## MODEL PARAMETERS
m1=0.2004
m2=0
m3=0.04726
l1c=0.095
l1=0.13
l2c=0.058
l2=0.1
g=9.8

#control parameters
kp=np.array([[0.3,0],[0,0.3]])

ki=np.array([[0.015,0],[0,0.015]])

def force_control(theta1,theta2,force_desired,force_measured,err):
    #angle conversion
    theta1=theta1*mt.pi/180
    theta2=theta2*mt.pi/180
    
    ## MODEL PLANT
    ## The EOM WHEN ALPHA=0, THETADOT=0
    H1=g*l2c*m3*mt.cos(theta1 + theta2) + g*l1*m2*mt.cos(theta1) + g*l1*m3*mt.cos(theta1) + g*l1c*m1*mt.cos(theta1)
    H2=g*l2c*m3*mt.cos(theta1 + theta2)

    H=np.array([[H1],[H2]])

    ## JACOBIAN
    J11=-l2*mt.sin(theta1 + theta2) - l1*mt.sin(theta1)
    J21=l2*mt.cos(theta1 + theta2) + l1*mt.cos(theta1)
    J12=-l2*mt.sin(theta1 + theta2)
    J22=l2*mt.cos(theta1 + theta2)

    J=np.array([[J11, J12],
               [J21, J22]])

    ## ENDEFECTOR FORCE EXPRESSED WITH TORQUE
    #torque=np.array([[0],[0]]) #no input torque at start

    #F_tip=np.linalg.solve(np.transpose(J),(torque-H))
    #print('Inital Endeffector Foce:')
    #print(F_tip)

    F_dir=np.array([[force_desired[0]],[force_desired[1]]])
    F_measured=np.array([[force_measured[0]],[force_measured[1]]])
    
    F_err=F_dir-F_measured

    err_vector=np.array([[err[0]],[err[1]]])+F_err
    
    torque_ctrl=H+np.transpose(J).dot(kp.dot(F_err)+ki.dot(err_vector))
    
    return_value=[float(500*torque_ctrl[0]),float(1200*torque_ctrl[1]),float(err_vector[0]),float(err_vector[1])]
    
    return return_value
