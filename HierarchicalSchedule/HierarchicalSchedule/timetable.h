#pragma once
#include "classunit.h"

class TimeTable
{
public:
	vector<Course> courses;//所有的科目
	vector<Teacher> teachers;//所有的老师
	vector<vector<ClassUnit *> > time_table_;//总课表
	TimeTable();

};