// Pathplan.cpp: implementation of the Pathplan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RobotView.h"
#include "Pathplan.h"
#include "RobotViewView.h"
#include "MainFrm.h"
#include "Route.h"
#include<iostream>
#include<stdio.h>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Pathplan::Pathplan()
{

}

Pathplan::~Pathplan()
{

}

///////////////////////////////////////////////////////////////

path Pathplan:: GetPath()
{
	return m_resultpath;
}

void Pathplan::GetVmap(int map[MH][NW])
{
	for(int i=0;i<MH;i++)
		for(int j=0;j<NW;j++)
			map[i][j]=m_VMap[i][j];
}
void Pathplan::removeOfItem(int i)//m_removeItem[]中存放要从8路径表S中删除的路径的索引
{
	int j,flag=0;
	for(j=0;j<m_removeNum;j++){
		if(m_removeItem[j]==i){
			flag=1;
			break;
		}
	}
	if(flag==0){
		m_removeItem[m_removeNum]=i;
		m_removeNum++;
	}
}

int Pathplan::pathEqual(path p,path q)// 判断两条路径是否相同，如果p与q相同，则返回1，否则，返回0
{
	int i;
	if(p.pathlength!=q.pathlength)
		return 0;
	else{
		for(i=0;i<p.pathlength;i++){
			if(!(p.pos[i].x==q.pos[i].x&&p.pos[i].y==q.pos[i].y))
				return 0;
		}
		return 1;
	}
}

//利用从图库传来的指针，把地图库中的相关信息传给m_VMap，m_VMap中就得到了初始的地图。	
void Pathplan::plan(int map[MH][NW])
{
	int i,j;
	for(i=0;i<MH;i++)
	{
		for(j=0;j<NW;j++){
			m_VMap[i][j]=map[i][j];//pinitmap->getEachRouteMap(i,j);
		}
	}
}

//设置初始地图中所有障碍物的位置，放入一数组中，并把障碍物的个数放入m_onum
void Pathplan::setOposition()
{
	int i,j;
	m_onum=0;
	for(i=0;i<MH;i++)
	{
		for(j=0;j<NW;j++)
		{
			if(m_VMap[i][j]==1)
			{
				m_Oposition[m_onum].x=i;
				m_Oposition[m_onum].y=j;
				m_Oposition[m_onum].height=1;
				m_onum++;
			}
		}
	}
}




//////////////////////////////////////////////////////////////////////////////

//把初始地图转化为具有虚拟高度的虚拟地图
void Pathplan::setVmap(float Kg, float r, int onum, position Oposition[NW*MH])
{
	int Kobs=20;
	int i,j,k;
	int Mid[MAX],temp;

 	for(i=0;i<MH;i++)
	{
		for(j=0;j<NW;j++)
		{
			Mid[0]=Kg*(sqrt((m_aimpos.x-i)*(m_aimpos.x-i))+sqrt((m_aimpos.y-j)*(m_aimpos.y-j)));//目标点对栅格的影响
			for(k=1;k<=onum;k++)//各个障碍物对栅格的影响
			{
				Mid[k]=r*(80-(sqrt((m_aimpos.x-Oposition[k-1].x)*(m_aimpos.x-Oposition[k-1].x))+sqrt((m_aimpos.y-Oposition[k-1].y)*(m_aimpos.y-Oposition[k-1].y))))/(sqrt((Oposition[k-1].x-i)*(Oposition[k-1].x-i))+sqrt((Oposition[k-1].y-j)*(Oposition[k-1].y-j)));
			}
			if(m_onum==0)
				temp=0;
			else
				temp=Mid[1];
			for(int l=1;l<=onum;l++){
				if(temp<Mid[l])
					temp=Mid[l];
			}
			m_VMap[i][j]=temp+Mid[0];//取最大值与目的点对栅格的影响为栅格的虚拟高度
		}
	}
	for(i=0;i<onum;i++){
		Oposition[i].height=80;
		m_VMap[Oposition[i].x][Oposition[i].y]=80;
	}

}
///////////////////////////////////////////////////////////////////////////




//从虚拟地图中得到危险临界值
int Pathplan::getKVH(position Oposition[MH*NW])
{
	int i;
	int temp= Oposition[0].height;
	for(i=0;i<m_onum;i++)
	{
		if(Oposition[i].height<temp)//有障碍物的栅格中最小的虚拟高度
		{
			temp=Oposition[i].height;
		}
	}
     m_Kvh=temp;
	return m_Kvh ;
}

//设置机器人目前的位置，根据定位模块得到的接口
void Pathplan::setRobotPos(int x,int y)
{ 
	m_robotpos.x=x;
	m_robotpos.y=y;
	m_robotpos.height=m_VMap[x][y];
}

//把一条路径放入路径表中
void Pathplan::pathIntoPlist(path S[MAX], path p,int &num)
{
	S[num]=p;
	num++;
}

//得到一个栅格周围的栅格及个数
void Pathplan::getAround(position u)
{
	if(u.x==0&&u.y==0)
	{
		 m_pos[0].x=1;
		 m_pos[0].y=0;
		 m_pos[0].height=m_VMap[1][0];
		 m_pos[1].x=1;
		 m_pos[1].y=1;
		 m_pos[1].height=m_VMap[1][1];
		 m_pos[2].x=0;
		 m_pos[2].y=1;
		 m_pos[2].height=m_VMap[0][1];
		 m_posnum=3;  //u为左下角栅格时
	}
	else if(u.x==0&&u.y==NW-1)
	{
		m_pos[0].x=0;
		 m_pos[0].y=NW-2;
		 m_pos[0].height=m_VMap[0][NW-2];
		 m_pos[1].x=1;
		 m_pos[1].y=NW-2;
		 m_pos[1].height=m_VMap[1][NW-2];
		 m_pos[2].x=1;
		 m_pos[2].y=NW-1;
		 m_pos[2].height=m_VMap[1][NW-1];
		 m_posnum=3;  //u为右下角栅格时
	}else if(u.x==MH-1&&u.y==NW-1)
	{
		m_pos[0].x=MH-1;
		 m_pos[0].y=NW-2;
		 m_pos[0].height=m_VMap[MH-1][NW-2];
		 m_pos[1].x=MH-2;
		 m_pos[1].y=NW-2;
		 m_pos[1].height=m_VMap[MH-2][NW-2];
		 m_pos[2].x=MH-2;
		 m_pos[2].y=NW-1;
		 m_pos[2].height=m_VMap[MH-2][NW-1];
		 m_posnum=3;  //u为右上角栅格时
	}else if(u.x==MH-1&&u.y==0)
	{
		m_pos[0].x=MH-1;
		 m_pos[0].y=1;
		 m_pos[0].height=m_VMap[MH-1][1];
		 m_pos[1].x=MH-2;
		 m_pos[1].y=1;
		 m_pos[1].height=m_VMap[MH-2][1];
		 m_pos[2].x=MH-2;
		 m_pos[2].y=0;
		 m_pos[2].height=m_VMap[MH-2][0];
		 m_posnum=3;  //u为左上角栅格时
	}
	else if(u.x==0)
	{
		 m_pos[0].x=0;
		 m_pos[0].y=u.y-1;
		 m_pos[0].height=m_VMap[0][u.y-1];
		 m_pos[1].x=1;
		 m_pos[1].y=u.y-1;
		 m_pos[1].height=m_VMap[1][u.y-1];
		 m_pos[2].x=1;
		 m_pos[2].y=u.y;
		 m_pos[2].height=m_VMap[1][u.y];
		 m_pos[3].x=1;
		 m_pos[3].y=u.y+1;
		 m_pos[3].height=m_VMap[1][u.y+1];
		 m_pos[4].x=0;
		 m_pos[4].y=u.y+1;
		 m_pos[4].height=m_VMap[0][u.y+1];
		 m_posnum=5;  //u为最下行栅格时
	}else if(u.y==0)
	{
		 m_pos[0].x=u.x-1;
		 m_pos[0].y=0;
		 m_pos[0].height=m_VMap[u.x-1][0];
		 m_pos[1].x=u.x-1;
		 m_pos[1].y=1;
		 m_pos[1].height=m_VMap[u.x-1][1];
		 m_pos[2].x=u.x;
		 m_pos[2].y=1;
		 m_pos[2].height=m_VMap[u.x][1];
		 m_pos[3].x=u.x+1;
		 m_pos[3].y=1;
		 m_pos[3].height=m_VMap[u.x+1][1];
		 m_pos[4].x=u.x+1;
		 m_pos[4].y=0;
		 m_pos[4].height=m_VMap[u.x+1][0];
		 m_posnum=5;  //u为最左行栅格时
	}else if(u.x==MH-1)
	{
		 m_pos[0].x=MH-1;
		 m_pos[0].y=u.y-1;
		 m_pos[0].height=m_VMap[MH-1][u.y-1];
		 m_pos[1].x=MH-2;
		 m_pos[1].y=u.y-1;
		 m_pos[1].height=m_VMap[MH-2][u.y-1];
		 m_pos[2].x=MH-2;
		 m_pos[2].y=u.y;
		 m_pos[2].height=m_VMap[MH-2][u.y];
		 m_pos[3].x=MH-2;
		 m_pos[3].y=u.y+1;
		 m_pos[3].height=m_VMap[MH-2][u.y+1];
		 m_pos[4].x=MH-1;
		 m_pos[4].y=u.y+1;
		 m_pos[4].height=m_VMap[MH-1][u.y+1];
		 m_posnum=5;  //u为最上行栅格时
	}else if(u.y==NW-1)
	{
		 m_pos[0].x=u.x-1;
		 m_pos[0].y=NW-1;
		 m_pos[0].height=m_VMap[u.x-1][NW-1];
		 m_pos[1].x=u.x-1;
		 m_pos[1].y=NW-2;
		 m_pos[1].height=m_VMap[u.x-1][NW-2];
		 m_pos[2].x=u.x;
		 m_pos[2].y=NW-2;
		 m_pos[2].height=m_VMap[u.x][NW-2];
		 m_pos[3].x=u.x+1;
		 m_pos[3].y=NW-2;
		 m_pos[3].height=m_VMap[u.x+1][NW-2];
		 m_pos[4].x=u.x+1;
		 m_pos[4].y=NW-1;
		 m_pos[4].height=m_VMap[u.x+1][NW-1];
		 m_posnum=5;  //u为最右行栅格时
	}
	else
	{
		 m_pos[0].x=u.x;
		 m_pos[0].y=u.y-1;
		 m_pos[0].height=m_VMap[u.x][u.y-1];
		 m_pos[1].x=u.x+1;
		 m_pos[1].y=u.y-1;
		 m_pos[1].height=m_VMap[u.x+1][u.y-1];
		 m_pos[2].x=u.x+1;
		 m_pos[2].y=u.y;
		 m_pos[2].height=m_VMap[u.x+1][u.y];
		 m_pos[3].x=u.x+1;
		 m_pos[3].y=u.y+1;
		 m_pos[3].height=m_VMap[u.x+1][u.y+1];
		 m_pos[4].x=u.x;
		 m_pos[4].y=u.y+1;
		 m_pos[4].height=m_VMap[u.x][u.y+1];
		 m_pos[5].x=u.x-1;
		 m_pos[5].y=u.y+1;
		 m_pos[5].height=m_VMap[u.x-1][u.y+1];
		 m_pos[6].x=u.x-1;
		 m_pos[6].y=u.y;
		 m_pos[6].height=m_VMap[u.x-1][u.y];
		 m_pos[7].x=u.x-1;
		 m_pos[7].y=u.y-1;
		 m_pos[7].height=m_VMap[u.x-1][u.y-1];
		 m_posnum=8;  //对中间的栅格做的操作
	}
}

//得到一个栅格周围栅格的最小虚拟高度
int Pathplan::getMinHeight(position u)
{
	getAround(u);
	int i=0;
	int heig;
	heig=m_pos[0].height;
	
	for(i=0;i<m_posnum;i++)
	{
		if (m_pos[i].height<heig)
		{
			heig=m_pos[i].height;
		}
	}
	return heig;
}

//对一栅格进行储水操作
void Pathplan::reserveWater(position &u, int  min)
{
	while(u.height<= min)
	{
		u.height=u.height+1;//幅度为1
	}
	m_VMap[u.x][u.y]=u.height;
}

//得到一栅格u周围的某些栅格，这些栅格具有以下性质：虚拟高度小于m_Kvh，也小于u的虚拟高度，保存在midpos[]中，个数保存在midnum中
void Pathplan::setMidPos(position u)
{
	int i;
	m_midnum=0;
	for(i=0;i<m_posnum;i++)
	{ 
		if(m_pos[i].height<u.height&&m_pos[i].height<m_Kvh)
		{
			m_Midpos[m_midnum]=m_pos[i];
			m_midnum++;
		}
	}
}

//测试一个栅格是否在一条路径中
int Pathplan::testPosInPath(position u,path l)
{
	int i;
	for(i=l.pathlength-1;i>=0;i--)
	//for(i=0;i<l.pathlength;i++)
	{
		if(l.pos[i].x==u.x&&l.pos[i].y==u.y)
		{
			return 1;
		}
	}
    return 0;
}

//得到分流后的各路径
void  Pathplan::getMidPath(path p, position pos[MAX])
{
	int i;
	for(i=0;i<m_midnum;i++)
	{
		m_midpath[i]=p;
		m_midpath[i].pos[p.pathlength]=pos[i];
		m_midpath[i].pathlength=p.pathlength+1;
	}
}

//得到汇流后的路径
void Pathplan::getIntoPath(path p, path q, position s)
{
	int i,j,pm;
	m_intopath.pathlength=0;
	for(i=0;i<p.pathlength;i++)
	{
		if(p.pos[i].x==s.x&&p.pos[i].y==s.y)
		{
			pm=i;
			break;
		}
	}
	if(pm<=q.pathlength-1)
	{
		m_intopath=p;
	}
	else
	{
		for(i=0;i<q.pathlength;i++)
		{
			m_intopath.pos[i]=q.pos[i];
			m_intopath.pathlength++;
		}
		for(j=pm+1;j<p.pathlength;j++)
		{
			m_intopath.pos[m_intopath.pathlength]=p.pos[j];
			m_intopath.pathlength++;
		}
	}
}

//把第n条路径从路径表中删除
void Pathplan::removePath(int  n, path  s[MAX],int &num)
{
	int i;
	for(i=n;i<num;i++)
	{
		s[i-1]=s[i];
	}
	num--;
}

//进行消环操作
void Pathplan::removeCircle(path &p)
{
	int i,j,k;
	for(i=0;i<p.pathlength;i++)
	{
		for(j=i+1;j<p.pathlength;j++)
		{
			if(p.pos[i].x==p.pos[j].x&&p.pos[i].y==p.pos[j].y)
			{
				for(k=0;k<p.pathlength-j-1;k++)
				{
					p.pos[i+k]=p.pos[j+k];
				}
				p.pathlength=p.pathlength-j+i;
			}
			//
		}
	}
}

//得到最短路径
void Pathplan::getShortest(path s[MAX],int num)
{
	int i;
	m_resultpath=s[0];
	for(i=0;i<num;i++)
	{
		if(s[i].pathlength<m_resultpath.pathlength)
		{
			m_resultpath=s[i];
		}
	}
}

//设置机器人下一步要到达的位置
void Pathplan::setNextStep(path  p)
{
	m_nextstep=p.pos[1];
}

//根据以上的方法调用进行路径规划,参数x为目标点横坐标，y为目标点中坐标
void Pathplan::pathPlanAll(/*mapBase *pmap,dataBase *pdata,*/int aimx,int aimy)
{
	int i; 
	int j;
	int k;


	m_Kvh=59;
    m_kg=1;
    m_r=0.4;

    m_Spathnum=0; //路径表S初始化为空表
    m_Mpathnum=0; //路径表MI初始化为空表

	m_aimpos.x=aimx; //目标点横坐标 
	m_aimpos.y=aimy; //目标点纵坐标
	m_aimpos.height=m_VMap[m_aimpos.x][m_aimpos.y];


//    plan(/*pmapbase*/);//通过从图库传来的指针，设置本地地图信息

    setOposition( );//求出障碍物的位置和数目，保存在m_Oposition[]和m_onum中



    setVmap(m_kg,  m_r,  m_onum, m_Oposition);//把地图信息转换成虚拟地图信息
/*	CRobotViewApp* myapp=(CRobotViewApp*)AfxGetApp();
    CMainFrame* mainframe=(CMainFrame*)myapp->m_pMainWnd;
	GetVmap(mainframe->VirtualMap);
	Route* route=(Route*)mainframe->m_wndSplitter_Horizontal.GetPane(1,0);
	route->DrawRoute();*/


	for(i=0;i<40;i++){
		for(j=0;j<20;j++){
			if(m_VMap[i][j]<10)
				printf("  %d   ",m_VMap[i][j]);
			else if(m_VMap[i][j]!=100)
				printf(" %d   ",m_VMap[i][j]);
			else
				printf("%d   ",m_VMap[i][j]);
		}
		//printf("\n");
	}

    if(m_onum==0)
	{
		m_Kvh=59;
	}
	else
	{
		;//	m_Kvh=getKVH(m_Oposition);//得到危险临界值
	}

 //   setRobotPos(/*pdatabase*/);//得到机器人目前的位置

	if(m_aimpos.x==m_robotpos.x&&m_aimpos.y==m_robotpos.y)
	{
		;//如果目标点和机器人位置相同，则不动，否则，进行路径规划算法
	}
	else
	{
		path us;
		us.pos[0]=m_robotpos;
		us.pathlength=1;
		pathIntoPlist(m_S, us,m_Spathnum);//把机器人初始位置放到路径标m_S中

		m_Fpathnum=0;//置F为空

		int flag=1;//判断m_S中是否存在符合条件的路径的标志，为1时，表示存在，继续循环

		while(flag!=0)
		{
			flag=0;
			
			if(m_Spathnum==0)
				break; //如果m_S中没有路径存在，则结束循环，跳出看有没有可行路径
			else
			{
				m_Mpathnum=0; 
				path L=m_S[0];//取m_S的第一条路径
				removePath(1,m_S,m_Spathnum); //把第一条路径从路径表m_S中取出然后删除

				
				if(L.pos[L.pathlength-1].x==m_aimpos.x&&L.pos[L.pathlength-1].y==m_aimpos.y)
				{
					pathIntoPlist(m_F, L,m_Fpathnum);//此路径正好是一条可行路径，则把此路径放到路径表F中

					for(i=0;i<m_Spathnum;i++)//测试是否可继续循环
					{
						if(!(m_S[i].pos[m_S[i].pathlength-1].x==m_aimpos.x&&m_S[i].pos[m_S[i].pathlength-1].y==m_aimpos.y))
							flag=1;
					}
					continue;
				}
				else//此路径终点不是目标点
				{
					int exist=0;//为0时，表示虚拟高度都大于m_Kvh
					int  high=getMinHeight(L.pos[L.pathlength-1]);

					for(i=0;i<m_posnum;i++)
					{
						if(m_pos[i].height<m_Kvh)
						{
							exist=1;
							break;
						}
					}
					if(exist==0)
					{
						for(i=0;i<m_Spathnum;i++)//测试是否可继续循环
						{
							if(!(m_S[i].pos[m_S[i].pathlength-1].x==m_aimpos.x&&m_S[i].pos[m_S[i].pathlength-1].y==m_aimpos.y))
								flag=1;
						}
						continue;
					}
					else{
						int allmax=0;//为0时，表示虚拟高度都大于L末端栅格的虚拟高度
						
						for(i=0;i<m_posnum;i++)
						{
							if(m_pos[i].height<L.pos[L.pathlength-1].height)
							{
								allmax=1;
								break;
							}
						}
						if(allmax==0)
						{
							reserveWater(L.pos[L.pathlength-1], high);
						}

						setMidPos(L.pos[L.pathlength-1]);

						position temp;

						for(i=0;i<m_midnum;i++)//对midpos[]中各元素进行排序
						{
							for(j=0;j<m_midnum-i;j++)
							{
								if(m_Midpos[i].height<m_Midpos[j].height)
								{
									temp=m_Midpos[i];
									m_Midpos[i]=m_Midpos[j];
									m_Midpos[j]=temp;
								}
							}
						}

						getMidPath(L, m_Midpos); //分流操作

						int inflag=0;//标志路径是否符合直接放到M,0适合
      ////////////////////////////////////////////////////////////////////////////
						m_removeNum=0;
						for(i=0;i<m_midnum;i++)
						{
							inflag=0;
							for(j=0;j<m_Spathnum;j++)
							{
								if(testPosInPath(m_Midpos[i],m_S[j]))
								{
									removeOfItem(j);
									inflag=1;
									getIntoPath(m_S[j],m_midpath[i],m_Midpos[i]);//汇流操作


									int equalflag=0;//如果汇流结果和MI中的某条路径相同，则不把它放到MI中
									for(int g=0;g<m_Mpathnum;g++){
										if(pathEqual(m_intopath,m_MI[g])==1)
											equalflag=1;
									}
									if(equalflag==0)



										pathIntoPlist(m_MI,m_intopath,m_Mpathnum);//把汇流结果放入路径表M中

								}
							}
							if(inflag==0)
								pathIntoPlist(m_MI,m_midpath[i],m_Mpathnum);
						}


						//对m_removeItem[]中的元素进行排序，最大的在前面，以便从m_S中删除时，序号不会变
						int temp1;
						for(i=0;i<m_removeNum;i++){
							for(j=i+1;j<m_removeNum;j++){
								if(m_removeItem[i]<m_removeItem[j]){
									temp1=m_removeItem[i];
									m_removeItem[i]=m_removeItem[j];
									m_removeItem[j]=temp1;
								}
							}
						}

						for(int in=0;in<m_removeNum;in++){
							for(int ou=0;ou<m_Spathnum;ou++){
								if(m_removeItem[in]==ou)
									removePath(ou+1,m_S,m_Spathnum);
							}
						}


						if(m_Mpathnum!=0){
							for(i=0;i<m_Mpathnum;i++){
								removeCircle(m_MI[i]);
								pathIntoPlist(m_S,m_MI[i],m_Spathnum);
							}
						}
					}
				}
			}
			for(i=0;i<m_Spathnum;i++)
			{
				if(!(m_S[i].pos[m_S[i].pathlength-1].x==m_aimpos.x&&m_S[i].pos[m_S[i].pathlength-1].y==m_aimpos.y))
					flag=1;
			}
		}
		for(i=0;i<m_Spathnum;i++){
			pathIntoPlist(m_F,m_S[i],m_Fpathnum);
		}
		if(m_Fpathnum!=0)
		{
			for(i=0;i<m_Fpathnum;i++){
				removeCircle(m_F[i]);//对每条路径进行消环操作
			}
			
			getShortest(m_F, m_Fpathnum);//得到最短可行路径
			setNextStep(m_resultpath);//设置下一步要到达的位置
			
		}
		else
		{
			;//找不到可行路径
		}
	}
}
//	main(){
	//dataBase *data1;
	//mapBase *map1;
//	Pathplan npath;
//	npath.pathPlanAll(/*map1,data1,*/3,7);
//	}
