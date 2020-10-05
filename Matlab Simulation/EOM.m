clear all;
close all;
syms t theta1(t) theta2(t) l1c l1 l2c l2 theta1dot(t) theta2dot(t) m1 m2 m3 g

x1=cos(theta1)*l1c;
% dx1dt=diff(x1,t);
y1=sin(theta1)*l1c;
% dy1dt=diff(y1,t);
dx1dt=-l1c*sin(theta1)*theta1dot;
dy1dt=l1c*cos(theta1)*theta1dot;

x2=cos(theta1)*l1;
% dx2dt=diff(x2,t);
y2=sin(theta1)*l1;
% dy2dt=diff(y2,t);
dx2dt=-l1*sin(theta1)*theta1dot;
dy2dt=l1*cos(theta1)*theta1dot;

x3=l1*cos(theta1)+l2c*cos(theta1+theta2);
% dx3dt=diff(x3,t);
y3=l1*sin(theta1)+l2c*sin(theta1+theta2);
% dy3dt=diff(y3,t);
dx3dt=-l1*sin(theta1)*theta1dot-l2c*sin(theta1+theta2)*theta1dot-l2c*sin(theta1+theta2)*theta2dot;
dy3dt=l1*cos(theta1)*theta1dot+l2c*cos(theta1+theta2)*theta1dot+l2c*cos(theta1+theta2)*theta2dot;

K1=0.5*m1*(dx1dt^2+dy1dt^2);
K2=0.5*m2*(dx2dt^2+dy2dt^2);
K3=0.5*m3*(dx3dt^2+dy3dt^2);
K=K1+K2+K3;
P1=m1*g*y1;
P2=m2*g*y2;
P3=m3*g*y3;
P=P1+P2+P3;
L=K-P;

% dLdtheta1dot=diff(L,theta1dot);
dLdtheta1dot_t=(m3*(2*(l2c*cos(theta1 + theta2) + l1*cos(theta1))*(l1*theta1dot*cos(theta1) + l2c*theta1dot*cos(theta1 + theta2) + l2c*theta2dot*cos(theta1 + theta2)) + 2*(l2c*sin(theta1 + theta2) + l1*sin(theta1))*(l1*theta1dot*sin(theta1) + l2c*theta1dot*sin(theta1 + theta2) + l2c*theta2dot*sin(theta1 + theta2))))/2 + (m2*(2*l1^2*theta1dot*cos(theta1)^2 + 2*l1^2*theta1dot*sin(theta1)^2))/2 + (m1*(2*l1c^2*theta1dot*cos(theta1)^2 + 2*l1c^2*theta1dot*sin(theta1)^2))/2;
% dLdtheta1=diff(L,theta1);
% dLdtheta2dot=diff(L,theta2dot);
dLdtheta2dot_t=(m3*(2*l2c*sin(theta1 + theta2)*(l1*theta1dot*sin(theta1) + l2c*theta1dot*sin(theta1 + theta2) + l2c*theta2dot*sin(theta1 + theta2)) + 2*l2c*cos(theta1 + theta2)*(l1*theta1dot*cos(theta1) + l2c*theta1dot*cos(theta1 + theta2) + l2c*theta2dot*cos(theta1 + theta2))))/2;
% dLdtheta2=diff(L,theta2);

T1_first=(m3*((2*l1*sin(theta1(t)) + 2*l2c*sin(theta1(t) + theta2(t)))*(l2c*sin(theta1(t) + theta2(t))*diff(theta1dot(t), t) + l2c*sin(theta1(t) + theta2(t))*diff(theta2dot(t), t) + l1*sin(theta1(t))*diff(theta1dot(t), t) + l2c*cos(theta1(t) + theta2(t))*theta1dot(t)*(theta1dot + theta2dot) + l2c*cos(theta1(t) + theta2(t))*theta2dot(t)*(theta1dot + theta2dot) + l1*cos(theta1(t))*theta1dot(t)*theta1dot) - (2*l2c*sin(theta1(t) + theta2(t))*(theta1dot + theta2dot) + 2*l1*sin(theta1(t))*theta1dot)*(l2c*cos(theta1(t) + theta2(t))*theta1dot(t) + l2c*cos(theta1(t) + theta2(t))*theta2dot(t) + l1*cos(theta1(t))*theta1dot(t)) + (2*l1*cos(theta1(t)) + 2*l2c*cos(theta1(t) + theta2(t)))*(l2c*cos(theta1(t) + theta2(t))*diff(theta1dot(t), t) + l2c*cos(theta1(t) + theta2(t))*diff(theta2dot(t), t) + l1*cos(theta1(t))*diff(theta1dot(t), t) - l2c*sin(theta1(t) + theta2(t))*theta1dot(t)*(theta1dot + theta2dot) - l2c*sin(theta1(t) + theta2(t))*theta2dot(t)*(theta1dot + theta2dot) - l1*sin(theta1(t))*theta1dot(t)*theta1dot) + (2*l2c*cos(theta1(t) + theta2(t))*(theta1dot + theta2dot) + 2*l1*cos(theta1(t))*theta1dot)*(l1*sin(theta1(t))*theta1dot(t) + l2c*sin(theta1(t) + theta2(t))*theta1dot(t) + l2c*sin(theta1(t) + theta2(t))*theta2dot(t))))/2 + (m2*(2*l1^2*cos(theta1(t))^2*diff(theta1dot(t), t) + 2*l1^2*sin(theta1(t))^2*diff(theta1dot(t), t)))/2 + (m1*(2*l1c^2*cos(theta1(t))^2*diff(theta1dot(t), t) + 2*l1c^2*sin(theta1(t))^2*diff(theta1dot(t), t)))/2;
T2_first=(m3*(2*l2c*cos(theta1(t) + theta2(t))*(l2c*cos(theta1(t) + theta2(t))*diff(theta1dot(t), t) + l2c*cos(theta1(t) + theta2(t))*diff(theta2dot(t), t) + l1*cos(theta1(t))*diff(theta1dot(t), t) - l2c*sin(theta1(t) + theta2(t))*theta1dot(t)*(theta1dot + theta2dot) - l2c*sin(theta1(t) + theta2(t))*theta2dot(t)*(theta1dot + theta2dot) - l1*sin(theta1(t))*theta1dot(t)*theta1dot) + 2*l2c*sin(theta1(t) + theta2(t))*(l2c*sin(theta1(t) + theta2(t))*diff(theta1dot(t), t) + l2c*sin(theta1(t) + theta2(t))*diff(theta2dot(t), t) + l1*sin(theta1(t))*diff(theta1dot(t), t) + l2c*cos(theta1(t) + theta2(t))*theta1dot(t)*(theta1dot + theta2dot) + l2c*cos(theta1(t) + theta2(t))*theta2dot(t)*(theta1dot + theta2dot) + l1*cos(theta1(t))*theta1dot(t)*theta1dot) - 2*l2c*sin(theta1(t) + theta2(t))*(theta1dot + theta2dot)*(l2c*cos(theta1(t) + theta2(t))*theta1dot(t) + l2c*cos(theta1(t) + theta2(t))*theta2dot(t) + l1*cos(theta1(t))*theta1dot(t)) + 2*l2c*cos(theta1(t) + theta2(t))*(theta1dot + theta2dot)*(l1*sin(theta1(t))*theta1dot(t) + l2c*sin(theta1(t) + theta2(t))*theta1dot(t) + l2c*sin(theta1(t) + theta2(t))*theta2dot(t))))/2;

T1=T1_first-(- g*m3*(l2c*cos(theta1 + theta2) + l1*cos(theta1)) - g*l1*m2*cos(theta1) - g*l1c*m1*cos(theta1));
% 
T2=T2_first-((m3*(2*(l2c*theta1dot*sin(theta1 + theta2) + l2c*theta2dot*sin(theta1 + theta2))*(l1*theta1dot*cos(theta1) + l2c*theta1dot*cos(theta1 + theta2) + l2c*theta2dot*cos(theta1 + theta2)) - 2*(l2c*theta1dot*cos(theta1 + theta2) + l2c*theta2dot*cos(theta1 + theta2))*(l1*theta1dot*sin(theta1) + l2c*theta1dot*sin(theta1 + theta2) + l2c*theta2dot*sin(theta1 + theta2))))/2 - g*l2c*m3*cos(theta1 + theta2))

