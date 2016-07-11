#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

class Course
{
public:
	int course_id_;
	string course_name_;
	Course();
	Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		return this->course_name_ < a.course_name_;
	}
};
