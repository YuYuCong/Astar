#include"function_declare.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* @function main
*/
int main(int argc, char** argv)
{
	//**************************************************************************************************************//
	//**  算法的目的：获取一组目标点、障碍点的坐标vector<Point> points，给出路径节点以及拐点vector<Point> path    **//
	//**************************************************************************************************************//
	/**
	* @function
	*/

	int table[WIDTH][HEIGHT] = { 0 };
	struct param list[WIDTH][HEIGHT];
	vector<Point> path;



	int64 st1, et1;
	st1 = cvGetTickCount();
	////////////////////////////////////////////////////////////////
	path = A_star(table, list);
	////////////////////////////////////////////////////////////////
	//时间显示
	et1 = cvGetTickCount();
	cout << "times cost:" << (et1 - st1) / (double)cvGetTickFrequency() / 1000.0 << "milliseconds\n\n";


	
	//show_result_path(table, list);
	Draw_result_path(table, list, path);

	//while(1);
	waitKey(0);
	return 0;
}



