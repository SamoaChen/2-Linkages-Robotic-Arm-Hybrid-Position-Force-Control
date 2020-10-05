# Robotic Arm Hybrid Position Force Control
![Robotic Arm](https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Arm_Display_Final_with_Curtain_edited.jpg)
* This is a robotic project built during quarantine time without using 3D printer or laser cutter, the body of the arm is made purely out of stacked cardboard
* This project achieved a position control on a 2 linkages robotic arm 
* This project achieved a centralized hybrid position force control on a 2 linkages robotic arm
* The robotic arm system also integrated self calibrating and angle overshoot proof mechanisms

# Main equipments and material used
* Arduino Uno 
* Raspberry Pi 3b+
* 2x geared motor with encoder
* Lever switches 
* Force sensitive resistor
* L298N motor driver 
* Regular cardboard
* Epoxy

# *Simulation*

# *Hardware & softwar prepairation*
## *Arm design & building*
The robotic manipulator is composed of two arms. Both arm are designed with a length of 10 cm to maximize the torque output of the geared motors without compromising too much of the reachable task space. Because the only available constructing material for the arm cardboard sheets dissected from amazon shipping packages, which has pretty low material strength when used individually, the strength of the arms are maximized by stacking several layers of cardboard sheets. Since the internal force that the arms would experiece are planner, the supporting patterns inside of each layer of cardboard sheet is aligned so that the arm as a whole can sustain internal couples induced by end effector force easier.
|![Second_Arm_Assembly](https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Second_Arm_Assembly.JPG)|
|:--:| 
|*First iteration of the design of the second arm Layers*|

The couplers for connecting motors and arms are designed to be embeded inside of the arms to decrease the distance of the arm from the motor, so as a result decreasing potential undesired torques that could cause connection point failure. Because there are force sensor wires that runs through the second arm, a tunnel has been carved out for organizing those wires to prevent potential tangling with the rotor.
|![New_Second_Arm_Interior_Glue](https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/New_Second_Arm_Interior_Glue.JPG)|
|:--:| 
|*Internal tunnel carved out for hiding force sensor wires*|
|![New_Second_Arm](https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/New_Second_Arm.JPG)|
|:--:| 
|*Version of the finished second arm*|
![First_Arm](https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/First_Arm.JPG)
|:--:| 
|*Version of the finished first arm*|


## *Initialization and overshoot prevention*
## *Force sensor calibration*
## *Stereo camera construction & calibration*

# *Algorithm Implementation*
## *Position control*
## *Hybrid position force control*
## *Object tracing*
