#pragma once
#include "classroom.h"

class Student
{
public:
	int student_id_;
	string student_name_;
	vector<Course> courses_;
	vector<ClassRoom> time_table_;
	Student();

private:

};