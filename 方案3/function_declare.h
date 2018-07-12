#ifndef FUNCTION_DECLARE_H
#define FUNCTION_DECLARE_H

#include "include_all.h"

////////////////////////////////////////////////////////////////////////////////////////
/// Global Variables  
const int ratio = 20;

const int Width = 20;
const int Height = 20;

////////////////////////////////////////////////////////////////////////////////////////
/// Function headers 
/*oppencv»æÖÆÍ¼Ïñ*/
#define OpenCV_show
#ifdef OpenCV_show
void showmap(int map[][Width], Point & begin, Point & target, vector<vector<int>>&path_list);
#endif

////////////////////////////////////////////////////////////////////////////////////////

#endif // !FUNCTION_DECLARE_H