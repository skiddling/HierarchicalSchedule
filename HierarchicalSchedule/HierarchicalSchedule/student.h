#pragma once
#include "teacher.h"

class Student
{
public:
	int student_no;//学生序号，自己用
	string student_id_;//学生给的序号,防止名字相同
	string student_name_;
	vector<Course> courses_;//要上哪些科目
	map<Course, pair<int, int> > time_table_;//最终的学生的课表
	Student();
	Student(string student_id, string student_name, vector<Course> courses);

private:

};