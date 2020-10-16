import math

def position(angle1,angle2):
    angle1=angle1*math.pi/180
    angle2=angle2*math.pi/180

    x_pos=0.13*math.cos(angle1)+0.1*math.cos(angle1+angle2)
    y_pos=0.13*math.sin(angle1)+0.1*math.sin(angle1+angle2)
    position=[int(x_pos*100),int(y_pos*100)]
    return position
