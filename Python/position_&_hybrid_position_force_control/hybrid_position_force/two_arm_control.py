import serial 
import time
import arm_control_function
import position_force_measured
import position
import waiting
import math

ser=serial.Serial('/dev/ttyACM0',baudrate=9600,timeout=1)
time.sleep(5)

x_position_desired=0
y_force_desired=0

angle=0
angle2=0

fsr_value=0

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
    global fsr_value

    ser.write(b'g')
    angle=int(ser.readline().decode('ascii'))
    angle=360*angle/(2*5736)
    ser.write(b'h')
    angle2=int(ser.readline().decode('ascii'))
    angle2=360*angle2/(2*11400)
    ser.write(b'f')
    fsr_value=int(ser.readline().decode('ascii'))

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

    if (err_count>100) and (err2_count>100):
        err_ki=0
        err2_ki=0
        err_count=0
        err2_count=0
        out=0
        out2=0
    
    err_ki_old=err_ki
    err2_ki_old=err2_ki

def out_function():
    global out
    global out2

    global x_position_desired
    global y_force_desired
    
    if out==0 and out2==0:
        write_two_pwm_Values(0,0)#clear up control buffer
        x_position_desired=int(input("x location desired: "))
        y_force_desired=-1*int(input("y location desired: "))/10
        out=1
        out2=1

def pwm_off_set(result):
    if result[0]>0:
        result[0]=result[0]+30
    elif result[0]<0:
        result[0]=result[0]-30
    if result[1]>0:
        result[1]=result[1]+30
    elif result[1]<0:
        result[1]=result[1]-30
    
    return result

def motor_position_control():
    global err_ki
    global err2_ki

    start=time.perf_counter()
    while True:
        # print(err_ki)
        waiting.waiting(start,200)
        start=time.perf_counter()
        getValues()
        err=[err_ki,err2_ki]
        position_force_desired=[x_position_desired,y_force_desired]
        position_force_value_measured=position_force_measured.position_force_measured(angle,angle2,fsr_value) 
        result=arm_control_function.force_control(angle,angle2,position_force_desired,position_force_value_measured,err)
        
        print(position_force_desired)
        print(position_force_value_measured)
        print(fsr_value)
        #print(err)
        #print(position_measured)

        result=pwm_off_set(result)
        write_two_pwm_Values(int(result[0]),int(result[1]))

        out_function()
        
        err_ki=result[2]
        err2_ki=result[3]

        err_clear()

while True:
#    command=input("input: ")
#    if command=='g':
#        getValues()
#        print(angle)
#    elif command=='p':
#        pwm=int(input("pwm: "))
#        write_pwm_Values(pwm)
    
    x_position_desired=int(input('type desired x location (cm): '))
    y_force_desired=-1*int(input('type desired force (g): '))/10
    ser.reset_input_buffer()
    motor_position_control()
