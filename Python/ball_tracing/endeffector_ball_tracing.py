import serial 
import time
import arm_control_function
import position
import waiting
import math
import ball_location

ser=serial.Serial('/dev/ttyACM0',baudrate=9600,timeout=1)
time.sleep(5)

x_position_desired=0
y_position_desired=0

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
out_pos=1

def getValues():
    global angle
    global angle2

    ser.write(b'g')
    angle=int(ser.readline().decode('ascii'))
    angle=360*angle/(2*5736)
    ser.write(b'h')
    angle2=int(ser.readline().decode('ascii'))
    angle2=360*angle2/(2*11400)
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
    global out_pos

    global x_position_desired
    global y_position_desired
    
    if out==0 and out2==0:
        write_two_pwm_Values(0,0)#clear up control buffer
        if out_pos==1:
            calibrate=input("Press enter to calibrate arm position")
            x_position_desired=18
            y_position_desired=10
            out_pos=0
        elif out_pos==0:
            reposition=input("Reposition the object")
            position_desired=ball_location.ball_location_find()
            x_position_desired=int(position_desired[2]/10)
            y_position_desired=int(-position_desired[1]/10)
            start_condition=input("Press enter to start tracing")
            out_pos=1
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
        waiting.waiting(start,150)
        start=time.perf_counter()
        getValues()
        err=[err_ki,err2_ki]
        position_desired=[x_position_desired,y_position_desired]
        position_measured=position.position(angle,angle2) 
        result=arm_control_function.force_control(angle,angle2,position_desired,position_measured,err)
        
        print(position_measured)
        
        result=pwm_off_set(result)

        write_two_pwm_Values(int(result[0]),int(result[1]))
        #print(result)

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
    pause=input("Wait for the arm to calibrate")
    position_desired=ball_location.ball_location_find()
    x_position_desired=int(position_desired[2]/10)
    y_position_desired=int(-position_desired[1]/10)
    start_condition=input("Press enter to start tracing")
    
    ser.reset_input_buffer()
    motor_position_control()
