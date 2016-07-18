#pragma once
#include "prefix.h"

class ClassUnit;

class Teacher
{
public:
	int teacher_no;
	string teacher_id_;
	string teacher_name_;
	vector<Course> courses_;//一个老师可能会对应多个不同名字的科目
	map<Course, int> courses_num_;//每一个科目上多少个班
	vector<ClassUnit *> units_que_;//具体每个老师上的班级
	Teacher();
	Teacher(string teacher_id, string teacher_name, vector<Course> courses, map<Course, int> courses_num);
	void GetCouSort();
	void AssignUnits(vector<int> &randgroups, vector<vector<int> > &randrooms, vector<int> &roompos);//将每一个课都进行分配

private:
	
};
