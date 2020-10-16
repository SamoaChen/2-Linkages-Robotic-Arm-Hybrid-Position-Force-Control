import  sensor as sr
import math

def position_force_measured(angle1,angle2,sensor_value):
    angle1=angle1*math.pi/180
    angle2=angle2*math.pi/180

    x_pos=0.13*math.cos(angle1)+0.1*math.cos(angle1+angle2)

    y_force=sr.sensor_to_weight(sensor_value)/10


    measured_value=[int(x_pos*100),int(-1*y_force)]

    return measured_value

