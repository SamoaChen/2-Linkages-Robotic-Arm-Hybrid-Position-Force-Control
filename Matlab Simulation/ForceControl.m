clearvars;
close all;
%% ERROR IN PERCENTAGE (ASSUME SAME PERCENTAGE FOR ALL PARAMETERS)
p=0.05;
p1=0.01; %error for angle measurements
%% MODEL PARAMETERS
m1=0.19703; %kg
m2=0;  %kg
m3=0.04226; %kg
theta1=pi/6;
theta2=-pi/2;
w1=0;
w2=0;
alpha1=0;
alpha2=0;
alpha=[alpha1,alpha2].';
l1c=0.095; %m
l1=0.13;
l2c=0.047; %m
l2=0.11;
g=9.8;

%% ACTUAL PARAMETERS
m1_p=m1+m1*p;
m2_p=m2+m2*p;
m3_p=m3+m3*p;
theta1_p1=theta1+theta1*p1;
theta2_p1=theta2+theta2*p1;
% w1_p=w1+w1*p;
% w2_p=w2+w2*p;
% alpha1=0;
% alpha2=0;
% alpha=[alpha1,alpha2].';
l1c_p=l1c+l1c*p;
l1_p=l1+l1*p;
l2c_p=l2c+l2c*p;
l2_p=l2+l2*p;
g_p=9.8;

%% PLOT POSITION OF ROBOTIC ARM
l1_x=[0,l1*cos(theta1)];
l1_y=[0,l1*sin(theta1)];
l2_x=[l1*cos(theta1),l1*cos(theta1)+l2*cos(theta1+theta2)];
l2_y=[l1*sin(theta1),l1*sin(theta1)+l2*sin(theta1+theta2)];
figure(1)
plot(0,0,'o')
hold on
plot(l2_x(1),l2_y(1),'.')
hold on
plot(l2_x(2),l2_y(2),'.')
hold on
plot(l1_x,l1_y,'LineWidth',1.1)
hold on
plot(l2_x,l2_y,'LineWidth',1.1)
xlim([-0.3, 0.3])
ylim([-0.3, 0.3])
title('Robotic Arm Position','FontSize',14)
xlabel('Horizontal Location (m)','FontSize',12)
ylabel('Vertical Location (m)','FontSize',12)
axis equal
set(gca,'FontSize',10)
set(gca,'GridAlpha',0.3)

%% MODEL PLANT
%%The EOM WHEN ALPHA=0, THETADOT=0 
H1=g*l2c*m3*cos(theta1 + theta2) + g*l1*m2*cos(theta1) + g*l1*m3*cos(theta1) + g*l1c*m1*cos(theta1);
H2=g*l2c*m3*cos(theta1 + theta2);

H=[H1, H2].';

%%JACOBIAN
J11=- l2*sin(theta1 + theta2) - l1*sin(theta1);
J21=l2*cos(theta1 + theta2) + l1*cos(theta1);
J12=-l2*sin(theta1 + theta2);
J22=l2*cos(theta1 + theta2);

J=[J11, J12;
    J21, J22];
%%ENDEFECTOR FORCE EXPRESSED WITH TORQUE
torque=[0,0].';%no input torque at start
F_tip=(transpose(J))\(torque-H);

%% ACTUAL PLANT
%%The EOM WHEN ALPHA=0, THETADOT=0 
H1_p=g_p*l2c_p*m3_p*cos(theta1_p1 + theta2_p1) + g_p*l1_p*m2_p*cos(theta1_p1) + g_p*l1_p*m3_p*cos(theta1_p1) + g_p*l1c_p*m1_p*cos(theta1_p1);
H2_p=g_p*l2c_p*m3_p*cos(theta1_p1 + theta2_p1);

H_p=[H1_p, H2_p].';

%%JACOBIAN
J11_p=- l2_p*sin(theta1_p1 + theta2_p1) - l1_p*sin(theta1_p1);
J21_p=l2_p*cos(theta1_p1 + theta2_p1) + l1_p*cos(theta1_p1);
J12_p=-l2_p*sin(theta1_p1 + theta2_p1);
J22_p=l2_p*cos(theta1_p1 + theta2_p1);

J_p=[J11_p, J12_p;
    J21_p, J22_p];
%%ENDEFECTOR FORCE EXPRESSED WITH TORQUE
torque=[0,0].';%no input torque at start
F_tip=(transpose(J_p))\(torque-H_p);

%% CONTROLLER 

time=[];%for curve fit
x_force=[];%for curve fit
y_force=[];%for curve fit
Kp=0.1;
Ki=0.1;
Fd=[0,-0.3].';%desired endeffector force
Fe=Fd-F_tip;
ERR_sum=0;%initiate integral
for t=[0:1:150]
    %%FORCE READING
    F_tip=(transpose(J_p))\(torque-H_p);
%     %%PLOT THE FORCE READING
%     figure(1)
%     plot(t,F_tip(1),'.');
%     hold on
    %%GATHER POINTS
    time=[time,t];
    x_force=[x_force,F_tip(1)];
    y_force=[y_force,F_tip(2)];
    %%CALCULATING FORCE ERROR
    Fe=Fd-F_tip;
    %%UPDATE ERR_sum
    ERR_sum=ERR_sum+Fe;
    %%CALCULATING CONTROL INPUT
    torque=H+transpose(J)*(Fd+Kp*Fe+Ki*(ERR_sum));
end
    
%%PLOT THE RESPONSE
figure(2)
plot(time,x_force,'LineWidth',1.1);
hold on
plot(time,y_force,'LineWidth',1.1);
title('Endeffector Force Response','FontSize',14)
xlabel('Time','FontSize',12)
ylabel('Force Response','FontSize',12)
set(gca,'FontSize',10)
grid on
set(gca,'GridAlpha',0.3)
legend({'F_x','F_y'},'Location','Northeast')


