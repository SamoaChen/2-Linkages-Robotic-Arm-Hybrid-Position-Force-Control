clear all;
close all;

theta1=pi/4;
theta2=-pi/2;
l1c=0.5;
l1=1;
l2c=0.5;
l2=1;
% H1=[cos(theta1), -sin(theta1), 0;
%     sin(theta1), cos(theta1),0;
%     0, 0, 1];
% H2=[1, 0, l1;
%     0, 1,0;
%     0, 0, 1];
% H3=[cos(theta2), -sin(theta2), 0;
%     sin(theta2), cos(theta2),0;
%     0, 0, 1];
% H4=[1, 0, l2;
%     0, 1,0;
%     0, 0, 1];
% 
% H=H1*H2*H3*H4;
% H=H(1:2,1:3)
%% EQUATION THAT RELATES THE ENDEFFECTOR MOTION TO JOINT ANGLES
% dx3dt=-l1*sin(theta1)*w1-l2c*sin(theta1+theta2)*w1-l2c*sin(theta1+theta2)*w2
% dy3dt=l1*cos(theta1)*w1+l2c*cos(theta1+theta2)*w1+l2c*cos(theta1+theta2)*w2

J11=- l2c*sin(theta1 + theta2) - l1*sin(theta1);
J21=l2c*cos(theta1 + theta2) + l1*cos(theta1);
J12=-l2c*sin(theta1 + theta2);
J22=l2c*cos(theta1 + theta2);

J=[J11, J12;
    J21, J22];



%%external force in the inertia frame

F =[7.3500, -17.1500].';

torque=[0,0].';%no input torque at start
F_tip=(transpose(J))\(torque-H)