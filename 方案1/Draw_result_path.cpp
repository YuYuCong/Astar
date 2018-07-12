#include"function_declare.h"

/// Function definitions  
/**
* @function Draw_result_path
*/
//在端口上图形化表示，比较糙的做法
void show_result_path(int table[][HEIGHT], struct param list[][HEIGHT])
{
	int i, j;
	for (j = HEIGHT - 1; j >= 0; j--)
	{
		for (i = 0; i<WIDTH; i++)
		{
			if (i == begin_x && j == begin_y)
				cout << "⊕";
			else if (i == end_x && j == end_y)
				cout << "⊕";
			else if (table[i][j] == -1)
				cout << "* ";
			else if (list[i][j].dir >= 0)
				cout << "+ ";
			else cout << "- ";
		}
		cout << endl;
	}
}


void Draw_result_path(int table[][HEIGHT], struct param list[][HEIGHT],vector<Point>&path)
{
	//检查path的数据
	for (int i = 0; i <path.size(); i++) {
		cout << path[i] << ' ';
	}
	cout << '\n';

	Mat img(WIDTH*ratio, HEIGHT*ratio, CV_8UC3, Scalar(255,255,255));//caitu

	Point center;

	int radius = ratio/2;
	int lineType = 8;

	int i, j;
	for (j = HEIGHT - 1; j >= 0; j--)
	{
		for (i = 0; i < WIDTH; i++)
		{
			if (i == begin_x && j == begin_y)//起点
			{
				center.x = i * ratio;
				center.y = j * ratio;
				circle(img, center, radius, Scalar(0, 233, 0), -1, lineType);
			}
			else if (i == end_x && j == end_y)//终点
			{
				center.x = i * ratio;
				center.y = j * ratio;
				circle(img, center, radius, Scalar(255, 97, 0), -1, lineType);
			}
			else if (table[i][j] == -1)//障碍物
			{
				// Rectangle函数参数： 图片， 左上角， 右下角， 颜色， 线条粗细， 线条类型，点类型  
				center.x = i*ratio - ratio / 2;
				center.y = j*ratio - ratio / 2;
				rectangle(img, center, Point(center.x + ratio, center.y + ratio), Scalar(0, 0, 0), 1, 4, 0);
			}
			else if (list[i][j].dir >= 0)//路径标识
			{
				//center.x = i*ratio - ratio / 4;
				//center.y = j*ratio - ratio / 4;
				//rectangle(img, center, Point(center.x + ratio / 2, center.y + ratio / 2), Scalar(127, 255, 121), 3, 4, 0);
			
				//..这两种样式二选一
				//center.x = i*ratio;
				//center.y = j*ratio;
				//circle(img, center, ratio / 4 , Scalar(127, 255, 121), -1, lineType);
			}
			else
			{
			}
		}
		cout << endl;
	}
	imshow("Path Planing", img);

	for (i = 0; i < path.size(); i++)//路径标识
	{
		waitKey(500);
		//center.x = (path[i].x-1)*ratio - ratio / 4;
		//center.y = (path[i].y-1)*ratio - ratio / 4;
		//rectangle(img, center, Point(center.x + ratio / 2, center.y + ratio / 2), Scalar(127, 255, 121), 3, 4, 0);
		center.x = (path[i].x - 1)*ratio;
		center.y = (path[i].y - 1)*ratio;
		circle(img, center, ratio / 4, Scalar(127, 255, 121), -1, lineType);
		imshow("Path Planing", img);
	}

}