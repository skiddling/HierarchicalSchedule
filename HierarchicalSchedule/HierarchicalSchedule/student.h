#pragma once
#include "teacher.h"

class Student
{
public:
	int student_id_;
	string student_name_;
	vector<Course> courses_;//Ҫ����Щ��Ŀ
	map<Course, pair<int, int> > time_table_;//���յ�ѧ���Ŀα�
	Student(int student_id, string student_name, vector<Course> courses);

private:

};