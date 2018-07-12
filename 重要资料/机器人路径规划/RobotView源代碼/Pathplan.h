// Pathplan.h: interface for the Pathplan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHPLAN_H__BFAEA81F_F76C_4EF5_AE02_9520E82CCD59__INCLUDED_)
#define AFX_PATHPLAN_H__BFAEA81F_F76C_4EF5_AE02_9520E82CCD59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include<math.h>
//using namespace move;

#define MAX  200
#define MH    12
#define NW    20





//////////////////////////////////////////////////////////////////////////////////////////
/*地图中的位置环境信息，x为栅格的中心横坐标，y为栅格的中心纵坐标，height表示此位置的高度*/
//////////////////////////////////////////////////////////////////////////////////////////
struct position{
    int    x ;
    int    y ;
    int    height;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*路径结构，pos[]为路径所经过的栅格组合，用位置数组表示，pathlength表示路径所经过的栅格个数，即路径长度。*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
struct path{
    position  pos[100];
    int       pathlength;
};

class Pathplan{

private:
	//mapBase *pmapbase;  //存放地图指针
	//dataBase *pdatabase; //存放数据库指针
	int m_VMap[MH][NW];                 //放置虚拟地图
	position m_aimpos;                //目标位置
	position m_Oposition[MH*NW];        //用来存放有障碍物的栅格
	int m_onum;                       //有障碍物的栅格的个数
	int m_Kvh;                        //危险临界值
	float m_kg;                         //为一个常数，用来计算虚拟高度时用到
	float m_r;                          //为一个常数，用来计算虚拟高度时用到
	position m_robotpos;              //机器人的初始位置
	path m_S[MAX];                    //路径表
	path m_F[MAX];                    //路径表
	path m_MI[MAX];                    //路径表
	int m_Spathnum;                   //S表中路径的条数
	int m_Fpathnum;                   //F表中路径的条数
	int m_Mpathnum;                   //M表中路径的条数
	position m_pos[MAX];              //一个栅格周围的栅格坐标
	int m_posnum;                     //一个栅格周围的栅格个数
	position m_Midpos[MAX];           //符合分流条件的栅格位置
	int m_midnum;                     //符合分流条件的栅格个数
	path m_midpath[MAX];              //保存分流的结果
	path m_intopath;                  //保存汇流结果
	path m_resultpath;                //最终得到的最优路径
	position m_nextstep;              //下一步机器人要到达的位置
	int m_removeItem[MAX];
	int m_removeNum;

public:
    //把数据库中指针传到自己定义的指针中去
	//void setDatabase(dataBase *pdata);
	path GetPath();
	//把图库中的指针传到自己定义的指针中去
	//void setMapbase(mapBase *pmap);
	void GetVmap(int map[MH][NW]);
	//利用从图库传来的指针，把地图库中的相关信息传给m_VMap，m_VMap中就得到了初始的地图。
	void plan(int map[MH][NW]);

    //设置初始地图中所有障碍物的位置，放入一数组中，并把障碍物的个数放入m_onum
	void setOposition(/*int map[MH][NW]*/);

	//把初始地图转化为具有虚拟高度的虚拟地图
	void setVmap(float Kg, float r, int onum, position Oposition[NW*MH]);

	//从虚拟地图中得到危险临界值
	int getKVH(position Oposition[MH*NW]); 

	//设置机器人目前的位置，根据定位模块得到的接口
	void setRobotPos(int x,int y); 

	//把一条路径放入路径表中
	void pathIntoPlist(path S[MAX], path p ,int &num);

	//得到一个栅格周围的栅格及个数
	void getAround(position u);

	//得到一个栅格周围栅格的最小虚拟高度
	int getMinHeight(position u);

	//对一栅格进行储水操作
	void reserveWater(position &u, int  min);

	//得到一栅格u周围的某些栅格，这些栅格具有以下性质：虚拟高度小于m_Kvh，也小于u的虚拟高度，保存在midpos[]中，个数保存在midnum中
	void setMidPos(position u);

	//测试一个栅格是否在一条路径中
	int testPosInPath(position u,path l);

	//得到分流后的各路径
	void  getMidPath(path p, position pos[MAX]);

	//得到汇流后的路径
	void getIntoPath(path p, path q, position s);

	//把一条路径从路径表中删除
	void removePath(int  n, path  s[MAX],int &num);

	//进行消环操作
	void removeCircle(path &p);

	//得到最短路径
	void getShortest(path s[MAX],int num);

	//设置机器人下一步要到达的位置
	void setNextStep(path  p);

	//根据以上的方法调用进行路径规划
	void pathPlanAll(/*mapBase *pmap,dataBase *pdata,*/int aimx,int aimy);
	void removeOfItem(int i);//如果m_removeItem中没有元素与i相等，则把i放到次数组的最后
	int pathEqual(path p,path q);//测试p和q两条路径是否相同
public:
	Pathplan();
	virtual ~Pathplan();
};

#endif // !defined(AFX_PATHPLAN_H__BFAEA81F_F76C_4EF5_AE02_9520E82CCD59__INCLUDED_)