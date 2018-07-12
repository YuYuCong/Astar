#pragma once  
#ifndef FUNCTION_DECLARE_H
#define FUNCTION_DECLARE_H

#include "include_all.h"

/// Global Variables
#define WIDTH 30
#define HEIGHT 30

#define ratio 20

#define begin_x 3
#define begin_y 3
#define end_x 8
#define end_y 18

#define NEIGHBOUR 8  //搜索八方位
const int m[NEIGHBOUR] = { -1,1,0,0,-1,-1,1,1 };
const int n[NEIGHBOUR] = { 0,0,-1,1,-1,1,-1,1 };
#define table(i,j) *(*(table+i)+j)  

//定义每个格子的参数列表  
struct param
{
	int f;
	int g;
	int h;
	int dir;//记录方向  
	bool state;//记录状态（开列表和闭列表）  
};

/// Function headers 
/**/
void Draw_result_path(int table[][HEIGHT], struct param list[][HEIGHT],vector<Point>&path);
/**/
void show_result_path(int table[][HEIGHT], struct param list[][HEIGHT]);
/**/
vector<Point> A_star(int table[][HEIGHT], struct param list[][HEIGHT]);
/**/

#endif // !FUNCTION_DECLARE_H