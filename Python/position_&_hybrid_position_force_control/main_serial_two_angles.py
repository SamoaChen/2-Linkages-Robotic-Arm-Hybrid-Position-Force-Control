import serial 
import time
import control
import waiting
import inverse_kinematic
import math

ser=serial.Serial('/dev/ttyACM0',baudrate=9600,timeout=1)
time.sleep(5)

angle_desired=0
angle2_desired=0

angle=0
angle2=0

err_ki=0
err2_ki=0

err_ki_old=0
err2_ki_old=0

err_count=0
err2_count=0

out=1
out2=1

def getValues():
    global angle
    global angle2

    ser.write(b'g')
    angle=int(ser.readline().decode('ascii'))
    ser.write(b'h')
    angle2=int(ser.readline().decode('ascii'))
def write_pwm_Values(pwm):
    ser.write(b'p')
    pwm=str(pwm)
    pwm_str_encoded=str.encode(pwm+'*')
    ser.write(pwm_str_encoded)
   
def write_pwm2_Values(pwm2):
    ser.write(b'q')
    pwm2=str(pwm2)
    pwm2_str_encoded=str.encode(pwm2+'*')
    ser.write(pwm2_str_encoded)

def write_two_pwm_Values(pwm,pwm2):
    ser.write(b'r')
    pwm=str(pwm)
    pwm2=str(pwm2)
    pwm_out_str_encoded=str.encode(pwm+'*'+pwm2+'*')

    ser.write(pwm_out_str_encoded)

def err_clear():
    global out
    global out2

    global err_ki
    global err2_ki

    global err_ki_old
    global err2_ki_old

    global err_count
    global err2_count

    if err_ki==err_ki_old:
        err_count+=1
    if err2_ki==err2_ki_old:
        err2_count+=1

    if err_count>100:
        err_ki=0
        err_count=0
        out=0
    err_ki_old=err_ki
    if err2_count>100:
        err2_ki=0
        err2_count=0
        out2=0
    err2_ki_old=err2_ki

def out_function():
    global out
    global out2

    global angle_desired
    global angle2_desired

    if out==0 and out2==0:
        angle_desired=int(input("Desired Angle: "))
        angle2_desired=int(input("Desired Angle2: "))
        out=1
        out2=1

def motor_angle_control():
    global err_ki
    global err2_ki

    start=time.perf_counter()
    while True:
        # print(err_ki)
        waiting.waiting(start,70)
        start=time.perf_counter()
        getValues()
        # print(angle)
        result=control.control_func(err_ki,angle,angle_desired)
        
       # print(result[0])

        result2=control.control_func2(err_ki,angle2,angle2_desired)

        write_two_pwm_Values(int(result[0]),int(result2[0]))
       # print(result2[0])

        out_function()
        
        err_ki=result[1]
        err2_ki=result2[1]

        err_clear()

while True:
#    command=input("input: ")
#    if command=='g':
#        getValues()
#        print(angle)
#    elif command=='p':
#        pwm=int(input("pwm: "))
#        write_pwm_Values(pwm)
    
    angle_desired=int(input('type desired angle1: '))
    angle2_desired=int(input('type desired angle2: '))
    ser.reset_input_buffer()
    motor_angle_control()
