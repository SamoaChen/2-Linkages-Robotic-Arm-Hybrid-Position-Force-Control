def control_func(err_ki,angle_measured,angle_desired):
    kp=60
    ki=0.004
    err=angle_desired-angle_measured
    err_ki+=err*ki
    pwm=err*kp+err_ki
    if pwm>255:
        pwm=255
    elif pwm<-255:
        pwm=-255
    result=[pwm,err_ki]
    return result

def control_func2(err_ki,angle_measured,angle_desired):
    kp=60
    ki=0.003
    err=angle_desired-angle_measured
    err_ki+=err*ki
    pwm=err*kp+err_ki
    if pwm>255:
        pwm=255
    elif pwm<-255:
        pwm=-255
    result=[pwm,err_ki]
    return result

def torque_to_pwm(torque1,torque2):
    pwm1=(255/0.98)*torque1
    pwm2=(255/1.176)*torque2
    pwm=[pwm1,pwm2]
    return pwm


