import RPi.GPIO as gp
import picamera
import time
import os
gp.setwarnings(False)
gp.setmode(gp.BOARD)
camera=picamera.PiCamera()

gp.setup(7,gp.OUT)
gp.setup(11,gp.OUT)

def cameraA_select():
    os.system('i2cset -y 1 0x70 0x00 0x01')
    gp.output(7,False)
    gp.output(11,False)
def cameraB_select():
    os.system('i2cset -y 1 0x70 0x00 0x02')
    gp.output(7,True)
    gp.output(11,False)
    
def ball_capture():
    cameraA_select()
    camera.capture('camera_up.jpg')

    cameraB_select()
    camera.capture('camera_down.jpg')
#ball_capture()
