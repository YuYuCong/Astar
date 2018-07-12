/*
;===================================================================
;A* Pathfinder (Version 1.0) by William Yu.
;===================================================================
;Last updated 2017/10/24 -- Visual Studio --  C++

////////////////////////////////////////////////////////////////////
//调用这个库的方法示例：
//main.cpp
int main(int argc, char * argv[])
{
int array[Height][Width] = {
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1 },
{ 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
{ 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
{ 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};
CAstar *maze = new CAstar(array);
CPoint *start = new CPoint(1, 1);
CPoint *end = new CPoint(6, 10);
CPoint* parent = maze->FindPath(start, end, false);

vector<vector<int>> path;
vector<int> temp;
path.clear();
temp.clear();

while (parent != NULL)
{
temp.clear();
temp.push_back(parent->X);
temp.push_back(parent->Y);
path.push_back(temp);
parent = parent->m_pParentPoint;
}
for(int i=0;i < path.size(); i++)
cout << path[i][0] << "," << path[i][1] << endl;

while (1);
return 0;
}
//////////////////////////////////////////////////////////////////////
 */

//#include "stdafx.h"  
#include <stdlib.h>  
#include <iostream>  
#include <vector>  
#include <algorithm>  
using namespace  std;



class CPoint
{
public:
	CPoint(int x, int y) :X(x), Y(y), m_pParentPoint(NULL), G(0), F(0), H(0)
	{

	}

	~CPoint()
	{
	}

	void calcF() {
		this->F = this->H + this->G;
	}
public:
	int X;
	int Y;
	int G;
	int H;
	int F;
	CPoint* m_pParentPoint;
};

bool CompF(const CPoint* pl, const CPoint* pr)
{
	return pl->F < pr->F;
}



class CAstar
{
public:
	CAstar(int textureMap[][Width])
	{
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				m_textureMap[i][j] = 0;
			}
		}

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				m_textureMap[i][j] = textureMap[i][j];
			}
		}
	}

	CPoint* FindPath(CPoint* start, CPoint* end, bool IsIgnoreCorner)
	{
		m_listOpen.push_back(start);//将起始点放到开启列表中  

		while (m_listOpen.size())
		{
			CPoint* tempStart = getMinFPoint(); //获取F值最低的点  

			removeFromOpenList(tempStart);//将这个店中开启列表中删除  
			m_listClose.push_back(tempStart);//将这个点放到关闭列表中  

			std::vector<CPoint*> surroundPoints = SurrroundPoints(tempStart, IsIgnoreCorner);//获取F值最低点相邻的点  

			tPointList::iterator _iter = surroundPoints.begin();

			//遍历这些相邻点  
			for (_iter; _iter != surroundPoints.end(); ++_iter)
			{
				CPoint *point = *_iter;

				if (inOpenList(point->X, point->Y))//如果这个点在开启列表中  
					FoundPoint(tempStart, point);//重新计算G值，如果G值更小，则更新父节点，并且重新计算F值，否则什么都不做  
				else
					NotFoundPoint(tempStart, end, point);//不在开启列表中，则加入开启列表，计算G值,F值，设定父节点  
			}
			if (inOpenList(end->X, end->Y))//目标结点已经在开启列表中  
			{
				//返回在开启列表中的父节点  
				for (int i = 0; i < m_listOpen.size(); i++)
				{
					if (m_listOpen[i]->X == end->X && m_listOpen[i]->Y == end->Y)
					{
						return m_listOpen[i];
					}
				}
			}
		}
		return end;
	}

	bool CanReach(int x, int y)
	{
		return m_textureMap[x][y] == 0;
	}

	bool inCloseList(int x, int y)
	{
		CPoint* p = new CPoint(x, y);

		tPointList::iterator _iter = m_listClose.begin();
		for (_iter; _iter != m_listClose.end(); ++_iter)
		{
			CPoint* temp = *_iter;
			if (temp->X == p->X && temp->Y == p->Y)
				return true;
		}

		if (p)
		{
			delete p;
			p = NULL;
		}
		return false;
	}

	bool inOpenList(int x, int y)
	{
		CPoint *p = new CPoint(x, y);

		tPointList::iterator _iter = m_listOpen.begin();
		for (_iter; _iter != m_listOpen.end(); ++_iter)
		{
			CPoint* temp = *_iter;
			if (temp->X == p->X && temp->Y == p->Y)
				return true;
		}

		if (p)
		{
			delete p;
			p = NULL;
		}
		return false;
	}

	bool CanReach(CPoint* start, int x, int y, bool IsIgnoreCorner)
	{
		if (!CanReach(x, y) || inCloseList(x, y))
			return false;
		else
		{
			if ((abs(x - start->X) + abs(y - start->Y)) == 1)
				return true;
			else
			{
				if (CanReach(abs(x - 1), y) && CanReach(x, abs(y - 1)))
					return true;
				else
					return IsIgnoreCorner;
			}
		}
	}

	std::vector<CPoint*> SurrroundPoints(CPoint* point, bool IsIgnoreCorner)
	{
		tPointList surroundPoints;

		for (int x = point->X - 1; x <= point->X + 1; x++)
			for (int y = point->Y - 1; y <= point->Y + 1; y++)
			{

				if (CanReach(point, x, y, IsIgnoreCorner))
				{

					CPoint *p = new CPoint(x, y);

					surroundPoints.push_back(p);
				}
				else
				{

				}
			}

		return surroundPoints;
	}



	CPoint* getMinFPoint()
	{

		tPointList tempList;

		for (int i = 0; i < (int)m_listOpen.size(); i++)
		{
			tempList.push_back(m_listOpen[i]);
		}
		sort(tempList.begin(), tempList.end(), CompF);


		if (tempList.size())
		{
			return tempList[0];
		}

	}

	void removeFromOpenList(CPoint* point)
	{
		tPointList::iterator _iter = m_listOpen.begin();
		for (_iter; _iter != m_listOpen.end(); ++_iter)
		{
			m_listOpen.erase(_iter);
			break;
		}
	}


	void FoundPoint(CPoint* tempStart, CPoint* point)
	{
		int G = CalcG(tempStart, point);
		if (G < point->G)
		{
			point->m_pParentPoint = tempStart;
			point->G = G;
			point->calcF();
		}
	}

	void NotFoundPoint(CPoint* tempStart, CPoint* end, CPoint* point)
	{
		point->m_pParentPoint = tempStart;


		point->G = CalcG(tempStart, point);
		point->H = CalcH(end, point);
		point->calcF();
		m_listOpen.push_back(point);
	}

	int CalcG(CPoint* start, CPoint* point)
	{
		int G = (abs(point->X - start->X) + abs(point->Y - start->Y)) == 2 ? STEP : OBLIQUE;
		int parentG = point->m_pParentPoint != NULL ? point->m_pParentPoint->G : 0;
		return G + parentG;
	}

	int CalcH(CPoint* end, CPoint* point)
	{
		int step = abs(point->X - end->X) + abs(point->Y - end->Y);
		return step * STEP;
	}

private:
	static const int STEP = 10;
	static const int OBLIQUE = 14;

	typedef std::vector<CPoint*> tPointList;

	tPointList m_listOpen;
	tPointList m_listClose;


	int m_textureMap[Height][Width];


};