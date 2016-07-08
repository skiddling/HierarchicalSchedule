#pragma once
#include "teacher.h"

class Student
{
public:
	int student_id_;
	string student_name_;
	vector<Course> courses_;//要上哪些科目
	map<Course, pair<int, int> > time_table_;//最终的学生的课表
	Student(int student_id, string student_name, vector<Course> courses);

private:

};