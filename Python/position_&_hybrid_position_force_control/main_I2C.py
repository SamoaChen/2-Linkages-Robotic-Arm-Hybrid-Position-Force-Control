import RPi.GPIO as GPIO
from control import control_func
from smbus import SMBus
#INITIATE I2C CONNECTION
addr=0x8 #bus adddress
bus=SMBus(1) #indicates /dev/ic2-1
#INITIATE ANGLE MEASURING PORTS
A_pin=20
B_pin=21

GPIO.setmode(GPIO.BCM)
GPIO.setup(A_pin, GPIO.IN)
GPIO.setup(B_pin, GPIO.IN)
#PARAMETERS FOR MEASURED ANGLES
global counter
counter=0
angle=0
counter_old=0
#PARAMETERS FOR CONTROL
dir=0
pwm=0
err_ki=0
result=[pwm,err_ki]

#INTERRUPT CALL FUNCTION
def inputchange1(channel):
    global counter
    if GPIO.input(B_pin)==0:
        counter+=1
    else:
        counter-=1
def inputchange2(channel):
    global counter
    if GPIO.input(A_pin)==0:
       counter-=1
    else:
       counter+=1
#interrupt
GPIO.add_event_detect(A_pin,GPIO.RISING,callback=inputchange1,bouncetime=1)
GPIO.add_event_detect(B_pin,GPIO.RISING,callback=inputchange2,bouncetime=1)

#PARAMETERS FOR DESIRED ANGLE
angle_desired=-1;
while angle_desired!=361:
    angle_desired=input("Desired Angle: ")
    angle_desired=int(angle_desired)

    #FUNCTION FOR MEASURING CURRENT ANGLE
    while True:
        angle=360*counter/11472
        print angle
        counter_old=counter
        #CALCULATING PWM
        result=control_func(err_ki,angle,angle_desired)
        err_ki=result[1]
        #COMMUNICATE WITH ARDUINO
        pwm=int(result[0])
        print pwm
        if pwm<0:
            pwm*=-1
            dir=1
        else:
            dir=0
        bus.write_byte(addr,dir)
        
        bus.write_byte(addr,pwm+2)#LOST A 2 UNIT ACCURACY
