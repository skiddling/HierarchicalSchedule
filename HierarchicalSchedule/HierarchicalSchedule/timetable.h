#pragma once
#include "classunit.h"

class TimeTable
{
public:
	vector<Course> courses;//���еĿ�Ŀ
	vector<Teacher> teachers;//���е���ʦ
	vector<vector<ClassUnit *> > time_table_;//�ܿα�
	TimeTable();

};