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

# Simulation
## *EOM of the robotic manipulator*
The generalized mass matrix and gravitational and coriolis effect terms are calculated with matlab's symbolic tool box, and the complex EOM is be simplified by assuming the acceleration and velocity equals to 0 (force control is assumed to take place at static configuration)

```matlab
#-------------THE MASS MATRIX
M11=l1^2*m2 + l1^2*m3 + l1c^2*m1 + l2c^2*m3 + 2*l1*l2c*m3*cos(theta2);

M12=l2c*m3*(l2c + l1*cos(theta2));

M21=l2c*m3*(l2c + l1*cos(theta2));

M22=l2c^2*m3;

M=[M11,M12;
   M21,M22];
```

```matlab
#------------THE GRAVITATIONAL AND CORIOLIS EFFECT TERMS
H1=- l1*l2c*m3*sin(theta2)*w2^2 - 2*l1*l2c*m3*w1*sin(theta2)*w2 + g*l2c*m3*cos(theta1 + theta2) + g*l1*m2*cos(theta1) + g*l1*m3*cos(theta1) + g*l1c*m1*cos(theta1);

H2=-l2c*m3*(l1*sin(theta2)*w1^2 + 2*l1*w2*sin(theta2)*w1 - g*cos(theta1 + theta2));
```
## *Jacobian Matrix*
The jacobian matrix is being derived by hand and tabulated in matlab for this two linkages manipulator
```matlab
#------------THE JACOBIAN MATRIX
J11=- l2c*sin(theta1 + theta2) - l1*sin(theta1);
J21=l2c*cos(theta1 + theta2) + l1*cos(theta1);
J12=-l2c*sin(theta1 + theta2);
J22=l2c*cos(theta1 + theta2);

J=[J11, J12;
    J21, J22];
```
## *Centralized force controler*
The centralized force controler is being derived with a feed forward term and an feed back term
```matlab
#------------THE FORCE CONTROLER
%%CALCULATING FORCE ERROR
Fe=Fd-F_tip;
%%UPDATE ERR_sum
ERR_sum=ERR_sum+Fe;
%%CALCULATING CONTROL INPUT
torque=H+transpose(J)*(Fd+Kp*Fe+Ki*(ERR_sum));
```
A 5% of physical parameter measurements error, and a 1% angle measurements error are being assigned to the simulated model. The configuration of the robotic manipulator and the x direction and y direction force responses are plotted

<p float="left">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Manipulator_Config.png" width="45%" height="45%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Force_Response.png" width="45%" height="45%">
</p>

# Hardware & softwar prepairation
## Arm design & building
The robotic manipulator is composed of two arms. Both arm are designed with a length of 10 cm to maximize the torque output of the geared motors without compromising too much of the reachable task space. Because the only available constructing material for the arm cardboard sheets dissected from amazon shipping packages, which has pretty low material strength when used individually, the strength of the arms are maximized by stacking several layers of cardboard sheets. Since the internal force that the arms would experiece are planner, the supporting patterns inside of each layer of cardboard sheet is aligned so that the arm as a whole can sustain internal couples induced by end effector force easier.

<p float="left">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Second_Arm_Assembly.JPG" width="45%" height="45%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/New_Second_Arm_Interior_Glue.JPG" width="45%" height="45%">
</p>
 
The couplers for connecting motors and arms are designed to be embeded inside of the arms to decrease the distance of the arm from the motor, so as a result decreasing potential undesired torques that could cause connection point failure. Because there are force sensor wires that runs through the second arm, a tunnel has been carved out for organizing those wires to prevent potential tangling with the rotor.

<p float="left">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/New_Second_Arm.JPG" width="45%" height="45%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/First_Arm.JPG" width="45%" height="45%">
</p>

## Angle calibration and overshoot prevention
The encoders of the dc motors don't have refernce starting points. In order to have consistant angle readings, the robotic arm system is being self-calibrated every time the system is initialized. Four lever switches are being glued to the two joints of the robotic manipulator. If the power is on, two arms will keep rotating counter clockwise until the swiches are triggered. A initial angle values are then being assigned to each encoder

<img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Initialization.gif" width="100%" height="100%">
*Initialization*

The same system is also being adopted to prevent overshoot of the arm motion. When the arm moves out of defined angle range, the lever switch will be triggered, and an interrupt function that recalibrate robotic manipulator will be performed.

<p float="left">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Safe_Switch.JPG" width="45%" height="45%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Safe_Switch_2.JPG" width="45%" height="45%">
</p>


## Force sensor calibration
The force and sensor reading relationship are being measured by attaching the FSR(force sensitive resistor) to the bottom of a plastic plate, with rice gradually added to the plate. The corresponding weight of the rice and the plate are tabulated with respect to the sensor readings of the FSR.

<p float="left">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/FSR_Curve_Measure.JPG" width="45%" height="45%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/FSR_Curve_Measure_Rice1.JPG" width="46.7%" height="46.7%">
</p>

<img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Sensor_Graph.png" width="91%" height="91%">
The relationship are plotted as scatter plot, and a polynomial fit is being applied

## Stereo camera construction & calibration
A stereo camera is made by combining two raspberry pi 5 MP camreas, the distance between the centers of the two cameras are maximized without compromize the overlapping vision area

<p float="left">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Stereo_Camera_1.JPG" width="30%" height="30%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Stereo_Camera_4.JPG" width="30%" height="30%">
   <img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Stereo_Camera_2.JPG" width="30%" height="30%">
</p>

The cameras are calibrated with the help of matlab camera calibration application, and the internal and external parameter matrixs are calculated

<img src="https://github.com/SamoaChen/2-Linkages-Robotic-Arm-Hybrid-Position-Force-Control/blob/master/images/Camera_Calibration.png" width="50%" height="50%">

The A matrixs for two cameras are calculated, and the 3d coordinates of an object detected in two cameras are calculated by inputing the pixel locations of the object in two cameras' picture frame into the A matrixs. Due to the A matrixs rank deficient nature, the solution can be solved by calculating its V vector corresponding to its smallest signular value with SVD. 

```matlab
#------------CALCULATING 3D COORDINATES FROM PIXEL LOCATIONS
%%FORM A3 MATRIX
%PIXEL LOCATIONS IN THE UPPER AND LOWER CAMERA FRAME
xu_p=1214;
yu_p=87;
xd_p=904;
yd_p=155;

%A MATRIXS OF TWO CAMERAS
A_up=[yu_p*P_up3-P_up2;
      P_up1-xu_p*P_up3;
      xu_p*P_up2-yu_p*P_up1];

A_down=[yd_p*P_down3-P_down2;
        P_down1-xd_p*P_down3;
        xd_p*P_down2-yd_p*P_down1];
       
%CALCULAT WORLD COORDINATE
A=vertcat(A_up,A_down);

%FIND SINGULAR VALUE DECOMPOSITION AND V VECTOR CORRESPONDING TO SMALLEST SINGULAR VALUE
[~,S,V]=svd(A);

%THE 3D COORDINATE IN THE WORLD COORDINATE
X3D=V(:,end)*(1/V(end));

```

# Algorithm Implementation
## Position control
## Hybrid position force control
## Object tracing
