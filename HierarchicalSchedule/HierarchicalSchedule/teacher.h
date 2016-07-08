#pragma once
#include "course.h"

class Teacher
{
public:
	int teacher_id_;
	string teacher_name_;
	vector<Course> courses_;//一个老师可能会对应多个不同名字的科目
	map<Course, vector<int> > course_table_;//记录每个科目上多少次课
	Teacher(int teacher_id, string teacher_name, vector<Course> courses, map<Course, vector<int> > course_table);
private:

};
