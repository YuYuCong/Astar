function Y=compute_angle(X,Xsum,n)%Y是引力，斥力与x轴的角度向量,X是起点坐标，Xsum是目标和障碍的坐标向量,是(n+1)*2矩阵
  for i=1:n+1%n是障碍数目
      deltaXi=Xsum(i,1)-X(1)
      deltaYi=Xsum(i,2)-X(2)
      ri=sqrt(deltaXi^2+deltaYi^2)
      if deltaXi>0
          theta=asin(deltaXi/ri)
      else
          theta=pi-asin(deltaXi/ri)
      end
      if i==1%表示是目标
          angle=theta
      else
          angle=pi+theta
      end     
      Y(i)=angle%保存每个角度在Y向量里面，第一个元素是与目标的角度，后面都是与障碍的角度
  end
      