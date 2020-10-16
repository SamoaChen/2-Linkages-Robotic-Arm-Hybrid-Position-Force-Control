def sensor_to_weight(analog):
    weight=0.0056*analog*analog+0.9833*analog+78.874
    return weight

def weight_to_sensor(weight):
    sensor=-0.0006*weight*weight+0.8099*weight-55.2
    return sensor
