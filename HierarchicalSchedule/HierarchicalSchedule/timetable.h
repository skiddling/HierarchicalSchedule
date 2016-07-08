#pragma once
#include "classunit.h"

class TimeTable
{
public:
	static vector<Course> courses_;//所有的科目,所有的课表公用一个科目队列
	vector<Teacher> teachers_;//所有的老师,每个课表一个独立的老师队列
	vector<ClassUnit> class_units_que_;//记录所有课，形成一个队列，每个课表独立拥有这么一个队列
	vector<vector<ClassUnit *> > time_table_;//总课表
	TimeTable(int rows, int cols, vector<Teacher> teachers, vector<ClassUnit> class_units_que);

private:
	void GetRandTable(int rows, int cols);

};