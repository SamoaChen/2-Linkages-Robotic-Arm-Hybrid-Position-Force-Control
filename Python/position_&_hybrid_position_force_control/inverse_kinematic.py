import math 
l1=0.13
l2=0.11

def inverse(x,y):
    theta2=math.acos((x*x+y*y-l1*l1-l2*l2)/(2*l1*l2))
    theta1=math.atan(y/x)-math.atan((l2*math.sin(theta2))/(l1+l2*math.cos(theta2)))
    
    theta=[theta1,theta2]
    return theta
