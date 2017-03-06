#pragma once
//#include "course.h"
#include "interruptflag.h"

class Prefix
{
public:
	int pre_id_;//该前缀的编号，从1开始
	//bool satisfied_;//该序列是否被满足
	vector<Course> pattern_;//该前缀的模式
	//后续模式所有可以继续用的时间段，是一个只有0和1的数字串，1表示可以用，0表示不能用
	vector<int> avl_time_;
	//每个具体的序列当中的可用的时间段
	//map<vector<int>, int> avl_map;//每个可能序列的时间表,value是指在eachvaltime当中的下标
	map<vector<int>, bool> avl_tab_;//所有该序列的具体路径,1表示可用，0表示已经被占用了
	Prefix();
	Prefix(int rows);
	Prefix(vector<Course> pattern, int pre_id, int rows);

private:

};