#pragma once
#include "course.h"

class Prefix
{
public:
	int pre_id_;//该前缀的前缀号,一个科目的前缀好就是0
	vector<Course> pre_pat_;//模式的前缀
	vector<int> avl_time_;//后续模式可以继续用的时间段
	vector<int> used_time_;//已经占用的时间
	Prefix();
	Prefix(int rows);
	Prefix(vector<Course> pre_pat);

private:

};