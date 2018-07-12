%引力计算
function [Yatx,Yaty]=compute_Attract(X,Xsum,k,angle)%输入参数为当前坐标，目标坐标，增益常数,分量和力的角度
%把路径上的临时点作为每个时刻的Xgoal
R=(X(1)-Xsum(1,1))^2+(X(2)-Xsum(1,2))^2;%路径点和目标的距离平方
r=sqrt(R);%路径点和目标的距离
%deltax=Xgoal(1)-X(1);
%deltay=Xgoal(2)-X(2);
Yatx=k*r*cos(angle);%angle=Y(1)
Yaty=k*r*sin(angle);
%也可以这样编
%function y=compute_Attract(X,Xgoal,k)
%y=[Yafx,Yafy]
%引力在x,y方向的分量放在y向量里
end