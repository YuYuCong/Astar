#include"function_declare.h"
#include "AStarLibrary.h"
int main(int argc, char * argv[])
{
	int array[Height][Width] = {
		{ 1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     1, 1, 1, 1, 1,   1, 1, 1, 1, 1,    },
		{ 1, 0, 0, 0, 1,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1,    },
		{ 1, 0, 0, 0, 1,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1,    },
		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1,    },
		{ 1, 1, 1, 0, 0,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   1, 1, 1, 1, 1,    },

		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },  //5
		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
		{ 1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },

		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 1, 0, 0, 1, },  //10
		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 1, 0, 0, 1, },
		{ 1, 0, 0, 0, 0,   0, 0, 0, 1, 1,     1, 1, 0, 0, 0,   0, 1, 0, 0, 1, },
		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 0, 0, 0,   0, 1, 0, 0, 1, },
		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 0, 0, 0,   0, 1, 0, 0, 1, },
	
		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 1, 1, 1,   1, 1, 0, 0, 1,     },  //15
		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 1, 0, 0,   0, 0, 0, 0, 1,     },
		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 1, 0, 0,   0, 0, 0, 0, 1,     },
		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1,     },
		{ 1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     }
	};
	int s_x = 15;
	int s_y = 3;
	int e_x =6;
	int e_y = 16;
	CAstar *maze = new CAstar(array);
	CPoint *start = new CPoint(s_x, s_y);
	CPoint *end = new CPoint(e_x, e_y);
	CPoint* parent = maze->FindPath(start, end, false);

	vector<vector<int>> path_list;
	vector<int> temp;
	path_list.clear();
	temp.clear();

	while (parent != NULL)
	{
		temp.clear();
		temp.push_back(parent->X);
		temp.push_back(parent->Y);
		path_list.push_back(temp);
		parent = parent->m_pParentPoint;
	}
	reverse(path_list.begin(), path_list.end());
	//¼ì²éÂ·¾¶
	//for(int i=0;i < path_list.size(); i++){
	//cout << path_list[i][0] << "," << path_list[i][1] << endl;
	//}

#ifdef OpenCV_show
	showmap(array,Point(s_x, s_y),Point(e_x, e_y), path_list);
#endif

	//while (1);
	waitKey(0);
	return 0;
}

