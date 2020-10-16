import serial 
import time
import control
import waiting

ser=serial.Serial('/dev/ttyACM0',baudrate=9600,timeout=1)
time.sleep(3)

angle_desired=0;
angle=0
err_ki=0
err_ki_old=0
err_count=0
out=1

def getValues():
    global angle
    ser.write(b'g')
    angle=int(ser.readline().decode('ascii'))
def write_pwm_Values(pwm):
    ser.write(b'p')
    pwm=str(pwm)
    pwm_str_encoded=str.encode(pwm+'*')
    ser.write(pwm_str_encoded)

def err_clear():
    global out
    global err_ki
    global err_ki_old
    global err_count
    if err_ki==err_ki_old:
        err_count+=1
    if err_count>100:
        err_ki=0
        err_count=0
        out=0
    err_ki_old=err_ki

def out_function():
    global out
    global angle_desired
    if out==0:
        angle_desired=int(input("Desired Angle: "))
        out=1
def angle_input():
    global angle_desired
    if err_count==0:
        angle_desired=int(input("desired angle: "))

def motor_angle_control():
    global err_ki
    start=time.perf_counter()
    while True:
        print(err_ki)
        waiting.waiting(start,100)
        start=time.perf_counter()
        getValues()
       # print(angle)
        result=control.control_func(err_ki,angle,angle_desired)
        print(result[0])
        write_pwm_Values(int(result[0]))
        out_function()
        err_ki=result[1]
        err_clear()
    


while True:
#    command=input("input: ")
#    if command=='g':
#        getValues()
#        print(angle)
#    elif command=='p':
#        pwm=int(input("pwm: "))
#        write_pwm_Values(pwm)
    angle_desired=int(input('type desired angle: '))
    motor_angle_control()
